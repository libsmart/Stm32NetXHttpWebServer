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
    class AuthenticationCheckExtendedCallback : public CallbackCapable<UINT, NX_WEB_HTTP_SERVER *, UINT, CHAR *, CHAR **,
                UINT *, CHAR **, UINT *, CHAR **, UINT *> {
    public:
        static constexpr const char *COMPONENT_NAME = Stm32NetXHttpWebServer::COMPONENT_NAME;
        static constexpr char CLASS_NAME[] = "AuthenticationCheckExtendedCallback";
        const char *INSTANCE_NAME;

        AuthenticationCheckExtendedCallback() = delete;

        explicit AuthenticationCheckExtendedCallback(BaseServer &server) : INSTANCE_NAME(server.getName()), server(server) { ; }

        struct CallbackValue final : Printable {
            enum class Id : UINT {
                DONT_AUTHENTICATE = NX_WEB_HTTP_DONT_AUTHENTICATE,
                BASIC_AUTHENTICATE = NX_WEB_HTTP_BASIC_AUTHENTICATE,
                DIGEST_AUTHENTICATE = NX_WEB_HTTP_DIGEST_AUTHENTICATE
            };

            CallbackValue(const Id id) : id(id) { ; }
            CallbackValue(const UINT id) : id(static_cast<Id>(id)) { ; }

            explicit constexpr operator UINT() const { return static_cast<UINT>(id); }

            size_t printTo(PrintInterface &printObject) const override {
                return printObject.print(static_cast<UINT>(id));
            }

        private:
            Id id;
        };

        using CallbackResult = Result<CallbackValue, UINT>;

        virtual CallbackResult authenticationCallback(ServerHttpMethod requestType,
                                            BaseServer::Resource &request,
                                            BaseServer::Name &name,
                                            BaseServer::Password &password,
                                            BaseServer::Realm &realm) = 0;

    private:
        UINT callback(NX_WEB_HTTP_SERVER *server_ptr,
                      UINT request_type,
                      CHAR *resource,
                      CHAR **name,
                      UINT *name_length,
                      CHAR **password,
                      UINT *password_length,
                      CHAR **realm,
                      UINT *realm_length) override;

        char nameBuffer[BaseServer::Name::capacity() + 1]{};
        char passwordBuffer[BaseServer::Password::capacity() + 1]{};
        char realmBuffer[BaseServer::Realm::capacity() + 1]{};

        BaseServer &server;
    };
}
