/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#pragma once

#include "ServerCallback.hpp"

namespace AppCore::Webserver {
    using namespace Stm32NetXHttpWebServer;

    class RequestCallback : public ServerCallback {
    public:
        RequestCallback() = delete;

        explicit RequestCallback(BaseServer &server) : ServerCallback(server) { ; }

        UINT notifyCallback(ServerHttpMethod requestType, BaseServer::Resource &resource, Packet &packet) override;

    private:
        bool checkLogin(ServerHttpMethod requestType, BaseServer::Resource &resource, Packet &packet);

        UINT handleLogin(ServerHttpMethod requestType, BaseServer::Resource &resource, Packet &packet);

        uint32_t getNewCookie(char *cookie, size_t sz);


        uint32_t authenticatedCookie = 0;
    };
}
