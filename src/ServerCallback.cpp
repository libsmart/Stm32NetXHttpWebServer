/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "ServerCallback.hpp"

#include "globals.hpp"
#include "wwwroot.h"

using namespace Stm32NetXHttpWebServer;

UINT ServerCallback::notifyCallback(ServerHttpMethod requestType,
                                    resourceString_t &resourceString,
                                    Packet &packet) {
    server.log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::notifyCallback()\r\n", COMPONENT_NAME, CLASS_NAME, server.getName());

    Packet responsePacket(nullptr, server.getLogger());

    if (requestType == ServerHttpMethod::GET && resourceString == "/") {
        generateResponseHeader(responsePacket,
                               NX_WEB_HTTP_STATUS_OK,
                               index_html_len,
                               "text/html",
                               "Server: NetX WEB HTTP 5.10\r\n");

        try {
            responsePacket.dataAppend(index_html, index_html_len);
            packetSend(responsePacket);
        } catch (const std::exception &e) {
            responsePacket.release();
        }

        return (NX_WEB_HTTP_CALLBACK_COMPLETED);
    }

    return 0;
}

nxHttpResult_t ServerCallback::generateResponseHeader(Packet &responsePkt, const char *statusCode, UINT contentLength,
                                                      const char *contentType, const char *additionalHeader) {
    server.log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::generateResponseHeader()\r\n", COMPONENT_NAME, CLASS_NAME, server.getName());

    NX_PACKET *pkt;
    const auto ret = server.callback_generate_response_header(&pkt, statusCode, contentLength, contentType, additionalHeader);
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


    resourceString_t resourceString(resource);
    Packet packet(packet_ptr);

    return notifyCallback(static_cast<ServerHttpMethod>(request_type), resourceString, packet);
}
