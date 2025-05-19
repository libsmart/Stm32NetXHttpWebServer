/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "RequestCallback.hpp"
#include "wwwroot.h"
#include "Hash/Base58.hpp"
#include "Hash/Md5.hpp"
#include "Hash/MurmurHash3.hpp"

using namespace AppCore::Webserver;

UINT RequestCallback::notifyCallback(ServerHttpMethod requestType, BaseServer::Resource &resource, Packet &packet) {
    auto ret = ServerCallback::notifyCallback(requestType, resource, packet);

    String::FixedString<100> header{};
    const bool loggedIn = checkLogin(requestType, resource, packet);
    if (resource == "/login") return handleLogin(requestType, resource, packet);
    if (!loggedIn && !(resource == "/login")) {
        header.print(NX_WEB_HTTP_STATUS_SEE_OTHER "\r\n");
        header.print("Content-Type: text/html\r\n");
        header.print("Location: /login\r\n");
        header.print("\r\n");
        responseSendExtended(header);
        return (NX_WEB_HTTP_CALLBACK_COMPLETED);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // only authenticated requests beyond this line



    Packet responsePacket(nullptr, server.getLogger());

    if (requestType == Method::GET{} && resource == "/") {
        generateResponseHeader(responsePacket,
                               NX_WEB_HTTP_STATUS_OK,
                               index_html_len,
                               "text/html",
                               nullptr);
        try {
            responsePacket.dataAppend(index_html, index_html_len);
            packetSend(responsePacket);
        } catch (...) {
            try { responsePacket.release(); } catch (...) { ; }
        }
        return (NX_WEB_HTTP_CALLBACK_COMPLETED);
    }


    if (requestType == Method::GET{} && resource == "/img/Logo_mit_URL_Transparent_300.png") {
        generateResponseHeader(responsePacket,
                               NX_WEB_HTTP_STATUS_OK,
                               img_Logo_mit_URL_Transparent_300_png_len,
                               "image/png",
                               nullptr);
        try {
            responsePacket.dataAppend(img_Logo_mit_URL_Transparent_300_png, img_Logo_mit_URL_Transparent_300_png_len);
            packetSend(responsePacket);
        } catch (...) {
            try { responsePacket.release(); } catch (...) { ; }
        }
        return (NX_WEB_HTTP_CALLBACK_COMPLETED);
    }




    return ret;
}

bool RequestCallback::checkLogin(ServerHttpMethod requestType, BaseServer::Resource &resource, Packet &packet) {
    const NX_PACKET *pkt = packet.getNxPacket();
    const CHAR *header_start = reinterpret_cast<CHAR *>(pkt->nx_packet_prepend_ptr);
    const ULONG header_length = pkt->nx_packet_append_ptr - pkt->nx_packet_prepend_ptr;

    // Suche nach dem Anfang des Cookie-Headers
    CHAR *cookie_header = strstr(header_start, "Cookie:");
    if (cookie_header) {
        // Suche nach dem Ende der Zeile
        CHAR *line_end = strstr(cookie_header, "\r\n");
        if (line_end) {
            // Berechne die Länge des Cookie-Headers
            ULONG cookie_length = line_end - (cookie_header + 7); // 7 = Länge von "Cookie:"
            CHAR cookie_value[256] = {0};

            // Stelle sicher, dass der Puffer nicht überläuft
            if (cookie_length < sizeof(cookie_value)) {
                memcpy(cookie_value, cookie_header + 7, cookie_length);
                cookie_value[cookie_length] = '\0';

                // Jetzt kannst du cookie_value analysieren, z.B. nach "session=..."
                CHAR *session_token = strstr(cookie_value, "session=");
                if (session_token) {
                    session_token += strlen("session=");
                    // Suche nach dem Ende des Tokens (z.B. durch Semikolon oder Ende des Strings)
                    CHAR *token_end = strchr(session_token, ';');
                    ULONG token_length = token_end ? (ULONG) (token_end - session_token) : strlen(session_token);

                    CHAR token[64] = {0};
                    if (token_length < sizeof(token)) {
                        memcpy(token, session_token, token_length);
                        token[token_length] = '\0';

                        const auto murmurhash = MurmurHash3::murmur3_32(
                            reinterpret_cast<uint8_t *>(token),
                            token_length, 0);

                        // server.log(Stm32ItmLogger::LoggerInterface::Severity::NOTICE)
                                // ->printf("Cookie: '%s' (0x%08x)\r\n", token, murmurhash);

                        return (murmurhash == authenticatedCookie);
                    }
                }
            }
        }
    }
    return false;
}

UINT RequestCallback::handleLogin(ServerHttpMethod requestType, BaseServer::Resource &resource, Packet &packet) {
    Packet responsePacket(nullptr, server.getLogger());

    if (requestType == Method::GET{}) {
        generateResponseHeader(responsePacket,
                               NX_WEB_HTTP_STATUS_OK,
                               login_html_len,
                               "text/html",
                               "Server: NetX WEB HTTP 5.10\r\n");
        try {
            responsePacket.dataAppend(login_html, login_html_len);
            packetSend(responsePacket);
        } catch (...) {
            try { responsePacket.release(); } catch (...) { ; }
        }
        return (NX_WEB_HTTP_CALLBACK_COMPLETED);
    }


    if (requestType == Method::POST{}) {
        ULONG length{};
        CHAR buffer[1440]{};

        NX_PACKET *pkt = packet.getNxPacket();
        server.content_length_get(pkt, &length);
        server.log()->printf("Content-Length: %d\r\n", length);

        UINT actual_size{};
        server.content_get(pkt, 0, buffer, sizeof(buffer), &actual_size);
        server.log()->printf("Content: %s\r\n", buffer);

        BaseServer::Name user;
        BaseServer::Password pass;

        const char *key = strtok((char *) buffer, "&");
        while (key != NULL) {
            char *value = strchr(key, '=');
            if (value) {
                *value = '\0';
                value++;

                server.log()->printf("key: %s, value: %s\r\n", key, value);

                if (strcmp("user", key) == 0) { user = value; }
                if (strcmp("pass", key) == 0) { pass = value; }
            }
            key = strtok(NULL, "&");
        }

        String::FixedString<200> header{};

        if ((user == "admin") && (pass == "123456")) {
            char cookie[30]{};
            authenticatedCookie = getNewCookie(cookie, sizeof(cookie));
            header.print(NX_WEB_HTTP_STATUS_SEE_OTHER "\r\n");
            header.print("Content-Type: text/html\r\n");
            header.print("Location: /\r\n");
            header.printf("Set-Cookie: session=%s; Path=/; HttpOnly\r\n", cookie);
            header.print("\r\n");
            responseSendExtended(header);
            return (NX_WEB_HTTP_CALLBACK_COMPLETED);
        } else {
            header.print(NX_WEB_HTTP_STATUS_SEE_OTHER "\r\n");
            header.print("Content-Type: text/html\r\n");
            header.print("Location: /login\r\n");
            header.print("\r\n");
            responseSendExtended(header);
            return (NX_WEB_HTTP_CALLBACK_COMPLETED);
        }
    }
    return 0;
}

uint32_t RequestCallback::getNewCookie(char *cookie, const size_t sz) {
    uint8_t hash[16]{};
    char buffer[64]{};
    snprintf(buffer, sizeof(buffer), "%d%d%d", rand(), rand(), rand());
    Md5::inplace_md5_sum(reinterpret_cast<uint8_t *>(buffer), strlen(buffer), hash);

    size_t b58len = sz;
    Base58::b58enc(cookie, &b58len, hash, sizeof(hash));
    const auto murmurhash = MurmurHash3::murmur3_32(reinterpret_cast<uint8_t *>(cookie), strlen(cookie), 0);

    // server.log(Stm32ItmLogger::LoggerInterface::Severity::NOTICE)
            // ->printf("New Cookie: '%s' (0x%08x)\r\n", cookie, murmurhash);

    return murmurhash;
}
