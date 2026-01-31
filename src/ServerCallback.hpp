/*
 * SPDX-FileCopyrightText: 2026 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Server.hpp"
#include "ServerHttpMethod.hpp"
#include "Callback/CallbackCapable.hpp"
#include "String/FixedString.hpp"
#include "Packet/Packet.hpp"

extern "C" {
#include "nx_web_http_server.h"
}

namespace Stm32NetXHttpWebServer {
    class ServerCallback : public Stm32Common::CallbackCapable<UINT, NX_WEB_HTTP_SERVER *, UINT, CHAR *, NX_PACKET *> {
    public:
        static constexpr const char *COMPONENT_NAME = Stm32NetXHttpWebServer::COMPONENT_NAME;
        static constexpr char CLASS_NAME[] = "ServerCallback";
        const char *INSTANCE_NAME;

        ServerCallback() = delete;

        explicit ServerCallback(Server &server) : INSTANCE_NAME(server.getName()), server(server) { ; }

        virtual UINT notifyCallback(ServerHttpMethod requestType, BaseServer::Resource &resource, Packet &packet);


        virtual nxHttpResult_t generateResponseHeader(Packet &responsePkt, const char *statusCode, UINT contentLength,
                                                      const char *contentType, const char *additionalHeader);


        virtual nxHttpResult_t packetSend(Packet &responsePkt);

        template<size_t N1, size_t N2, size_t N3>
        nxHttpResult_t responseSendExtended(String::FixedString<N1> &header,
                                            String::FixedString<N2> &information,
                                            String::FixedString<N3> &additional_information);

        template<size_t N1>
        nxHttpResult_t responseSendExtended(String::FixedString<N1> &header);

        template<size_t N1>
        nxHttpResult_t responseSendExtended(const char (&header)[N1]);

    private:
        UINT callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource,
                      NX_PACKET *packet_ptr) override;

    protected:
        Server &server;
    };

    template<size_t N1, size_t N2, size_t N3>
    nxHttpResult_t ServerCallback::responseSendExtended(String::FixedString<N1> &header,
                                                        String::FixedString<N2> &information,
                                                        String::FixedString<N3> &additional_information) {
        return server.callback_response_send_extended(header.c_str(), header.size(),
                                                      information.c_str(), information.size(),
                                                      additional_information.c_str(), additional_information.size());
    }

    template<size_t N1>
    nxHttpResult_t ServerCallback::responseSendExtended(String::FixedString<N1> &header) {
        return server.callback_response_send_extended(header.c_str(), header.size(),
                                                      nullptr, 0,
                                                      nullptr, 0);
    }

    template<size_t N1>
    nxHttpResult_t ServerCallback::responseSendExtended(const char (&header)[N1]) {
        return server.callback_response_send_extended(header, N1 - 1,
                                                      nullptr, 0,
                                                      nullptr, 0);
    }
}
