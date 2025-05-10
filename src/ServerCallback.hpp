/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "BaseServer.hpp"
#include "ServerHttpMethod.hpp"
#include "Callback/CallbackCapable.hpp"
#include "String/FixedString.hpp"
#include "Packet/Packet.hpp"

extern "C" {
#include "nx_web_http_server.h"
}

using namespace Stm32Common;
using namespace Stm32NetX;

namespace Stm32NetXHttpWebServer {
    class ServerCallback : public Stm32Common::CallbackCapable<UINT, NX_WEB_HTTP_SERVER *, UINT, CHAR *, NX_PACKET *> {
    public:
        static constexpr const char *COMPONENT_NAME = Stm32NetXHttpWebServer::COMPONENT_NAME;
        static constexpr char CLASS_NAME[] = "ServerCallback";
        const char *INSTANCE_NAME;

        ServerCallback() = delete;

        explicit ServerCallback(BaseServer &server) : INSTANCE_NAME(server.getName()), server(server) { ; }

        virtual UINT notifyCallback(ServerHttpMethod requestType, BaseServer::Resource &request, Packet &packet);




        virtual nxHttpResult_t generateResponseHeader(Packet &responsePkt, const char *statusCode, UINT contentLength,
                                                      const char *contentType, const char *additionalHeader);


        virtual nxHttpResult_t packetSend(Packet &responsePkt);


    private:
        UINT callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource,
                      NX_PACKET *packet_ptr) override;

        BaseServer &server;
    };

}
