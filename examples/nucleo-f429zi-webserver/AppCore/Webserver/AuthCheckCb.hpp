/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Authentication/AuthenticationCheckCallback.hpp"

namespace AppCore::Webserver {
    class AuthCheckCb final : public Stm32NetXHttpWebServer::AuthenticationCheckCallback {
    public:
        AuthCheckCb() = delete;

        explicit AuthCheckCb(Stm32NetXHttpWebServer::BaseServer &server)
            : Stm32NetXHttpWebServer::AuthenticationCheckCallback(server) {
        }

        CallbackResult authenticationCallback(Stm32NetXHttpWebServer::ServerHttpMethod requestType,
            Stm32NetXHttpWebServer::BaseServer::Resource &request, Stm32NetXHttpWebServer::BaseServer::Name &name,
            Stm32NetXHttpWebServer::BaseServer::Password &password,
            Stm32NetXHttpWebServer::BaseServer::Realm &realm) override;
    };
}
