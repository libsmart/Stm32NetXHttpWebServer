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
    class AuthenticationCheckCallback : public CallbackCapable<UINT, NX_WEB_HTTP_SERVER *,
                UINT, CHAR *, CHAR **,
                CHAR **, CHAR **> {
    public:
        static constexpr const char *COMPONENT_NAME = Stm32NetXHttpWebServer::COMPONENT_NAME;
        static constexpr char CLASS_NAME[] = "AuthenticationCheckCallback";
        const char *INSTANCE_NAME;

        AuthenticationCheckCallback() = delete;

        explicit AuthenticationCheckCallback(BaseServer &server) : INSTANCE_NAME(server.getName()), server(server) { ; }

        struct CallbackValue final : Printable {
            enum class Id : UINT {
                DONT_AUTHENTICATE = NX_WEB_HTTP_DONT_AUTHENTICATE,
                BASIC_AUTHENTICATE = NX_WEB_HTTP_BASIC_AUTHENTICATE,
                DIGEST_AUTHENTICATE = NX_WEB_HTTP_DIGEST_AUTHENTICATE
            };

            CallbackValue(const Id id) : id(id) { ; }

            explicit constexpr operator UINT() const { return (UINT) id; }

            size_t printTo(PrintInterface &printObject) const override {
                switch (id) {
                    case Id::DONT_AUTHENTICATE:
                        return printObject.printf("DONT_AUTHENTICATE");
                    case Id::BASIC_AUTHENTICATE:
                        return printObject.printf("BASIC_AUTHENTICATE");
                    case Id::DIGEST_AUTHENTICATE:
                        return printObject.printf("DIGEST_AUTHENTICATE");
                }
                return 0;
            }

        private:
            Id id;
        };

        using CallbackResult = PrintableResult<CallbackValue, UINT>;

        virtual CallbackResult authenticationCallback(ServerHttpMethod requestType,
                                                      BaseServer::Resource &request,
                                                      BaseServer::Name &name,
                                                      BaseServer::Password &password,
                                                      BaseServer::Realm &realm) = 0;

    private:
        UINT callback(NX_WEB_HTTP_SERVER *server_ptr,
                      UINT request_type, CHAR *resource, CHAR **name,
                      CHAR **password, CHAR **realm) override;

        char nameBuffer[BaseServer::Name::capacity() + 1]{};
        char passwordBuffer[BaseServer::Password::capacity() + 1]{};
        char realmBuffer[BaseServer::Realm::capacity() + 1]{};

        BaseServer &server;
    };
}
