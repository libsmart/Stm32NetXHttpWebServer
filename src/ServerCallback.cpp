/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "ServerCallback.hpp"

#include "globals.hpp"
#include "wwwroot.h"

using namespace Stm32NetXHttpWebServer;

UINT ServerCallback::notifyCallback(ServerHttpMethod requestType,
                                    BaseServer::Resource &resource,
                                    Packet &packet) {
    server.log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::notifyCallback(%s, \"%s\", %p)\r\n", COMPONENT_NAME, CLASS_NAME, server.getName(),
                     (const char *) requestType, resource.c_str(), packet.getNxPacket());

    Packet responsePacket(nullptr, server.getLogger());

    if (requestType == Method::GET{} && resource == "/") {
        generateResponseHeader(responsePacket,
                               NX_WEB_HTTP_STATUS_OK,
                               index_html_len,
                               "text/html",
                               "Server: NetX WEB HTTP 5.10\r\n");
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
                               "Server: NetX WEB HTTP 5.10\r\n");
        try {
            responsePacket.dataAppend(img_Logo_mit_URL_Transparent_300_png, img_Logo_mit_URL_Transparent_300_png_len);
            packetSend(responsePacket);
        } catch (...) {
            try { responsePacket.release(); } catch (...) { ; }
        }
        return (NX_WEB_HTTP_CALLBACK_COMPLETED);
    }


    if (requestType == Method::POST{} && resource == "/settings") {
        ULONG length;
        CHAR buffer[1440]{};

        NX_PACKET *pkt = packet.getNxPacket();

        // CHAR param_ptr[100]{};
        // UINT param_size=0;
        // server.query_get(pkt, 0, param_ptr, &param_size, sizeof(param_ptr));

        server.content_length_get(pkt, &length);
        server.log()->printf("Content-Length: %d\r\n", length);

        UINT actual_size{};
        server.content_get(pkt, 0, buffer, sizeof(buffer), &actual_size);
        server.log()->printf("Content: %s\r\n", buffer);


        const char *key = strtok((char *) buffer, "&");
        while (key != NULL) {
            char *value = strchr(key, '=');
            if (value) {
                *value = '\0';
                value++;

                // key enthält den Parameternamen
                // value enthält den Parameterwert
                // Hier kannst du die Parameter weiterverarbeiten

                server.log()->printf("key: %s, value: %s\r\n", key, value);
            }
            key = strtok(NULL, "&");
        }


        generateResponseHeader(responsePacket,
                               NX_WEB_HTTP_STATUS_SEE_OTHER,
                               0,
                               "text/plain",
                               "Server: NetX WEB HTTP 5.10\r\nLocation: /\r\n");
        try {
            packetSend(responsePacket);
        } catch (...) {
            try { responsePacket.release(); } catch (...) { ; }
        }
        return (NX_WEB_HTTP_CALLBACK_COMPLETED);


        /*
        // Get the content header
        while (server.get_entity_header(&pkt, buffer, sizeof(buffer)).isOk()) {
            // Header obtained successfully. Get the content data location
            while (server.get_entity_content(&pkt, &offset, &length).isOk()) {
                // Write content data to buffer
                packet.data_extract_offset(offset, buffer, length, &length);
                buffer[length] = 0;
                server.log()->println((const char *)buffer);
            }
        }
        */

        return (NX_WEB_HTTP_CALLBACK_COMPLETED);
    }

    return 0;
}

nxHttpResult_t ServerCallback::generateResponseHeader(Packet &responsePkt, const char *statusCode, UINT contentLength,
                                                      const char *contentType, const char *additionalHeader) {
    server.log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::generateResponseHeader()\r\n", COMPONENT_NAME, CLASS_NAME, server.getName());

    NX_PACKET *pkt;
    const auto ret = server.callback_generate_response_header(&pkt, statusCode, contentLength, contentType,
                                                              additionalHeader);
    responsePkt = pkt;
    return ret;
}

nxHttpResult_t ServerCallback::packetSend(Packet &responsePkt) {
    server.log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::packetSend()\r\n", COMPONENT_NAME, CLASS_NAME, server.getName());
    return server.callback_packet_send(responsePkt.getNxPacket());
}

UINT ServerCallback::callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource,
                              NX_PACKET *packet_ptr) {
    server.log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::callback()\r\n", COMPONENT_NAME, CLASS_NAME, server.getName());


    BaseServer::Resource resourceString(resource);
    Packet packet(packet_ptr);

    return notifyCallback(static_cast<ServerHttpMethod>(request_type), resourceString, packet);
}
