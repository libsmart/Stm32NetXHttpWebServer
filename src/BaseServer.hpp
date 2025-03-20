/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Stm32NetXHttpWebServer.hpp"
#include "Loggable.hpp"
#include "Nameable.hpp"
#include "EventFlags/EventFlags.hpp"
#include "Stm32NetX.hpp"

extern "C" {
#include "nx_web_http_server.h"
}

namespace Stm32NetXHttpWebServer {
    class BaseServer : protected NX_WEB_HTTP_SERVER, public Stm32ItmLogger::Loggable, public Stm32Common::Nameable {
        friend class Request;

    public:
        using Flags = enum: ULONG {
            NONE = 0,
            IS_CREATED = 1UL << 0,
            IS_CONNECTED = 1UL << 1,
            IS_INITIALIZED = 1UL << 2,
            IS_PACKET_ALLOCATED = 1UL << 3,
            THE_END = 1UL << 31
        };

        using HTTP_METHOD = enum : UINT {
            METHOD_NONE = 0,
            METHOD_GET,
            METHOD_PUT,
            METHOD_POST,
            METHOD_DELETE,
            METHOD_HEAD
        };


        BaseServer() : NX_WEB_HTTP_SERVER() { ; }


        /**
         * Checks if the base server has been successfully created.
         *
         * @return True if the base server instance is created, false otherwise.
         *
         * This method determines whether the base server has been initialized by
         * checking the internal flag that represents the creation state.
         */
        bool isCreated();


#if defined(LIBSMART_STM32NETX_ENABLE_TLS) && defined(NX_WEB_HTTPS_ENABLE)

#endif

    protected:
        Stm32ThreadX::EventFlags flags{"Stm32NetXHttpWebServer::Client::flags", getLogger()};

    private:
        Stm32NetX::NetX *nx{};
    };
}
