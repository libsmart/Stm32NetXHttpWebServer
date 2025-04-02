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


        BaseServer() : BaseServer(nullptr, Stm32ItmLogger::emptyLogger) { ; }

        explicit BaseServer(Stm32ItmLogger::LoggerInterface &logger)
            : BaseServer(nullptr, logger) { ; }

        explicit BaseServer(const char *name)
            : BaseServer(name, Stm32ItmLogger::emptyLogger) { ; }

        BaseServer(const char *name, Stm32ItmLogger::LoggerInterface &logger)
            : NX_WEB_HTTP_SERVER(), Loggable(&logger), Nameable(name) { ; }

        /**
         * Checks if the base server has been successfully created.
         *
         * @return True if the base server instance is created, false otherwise.
         *
         * This method determines whether the base server has been initialized by
         * checking the internal flag that represents the creation state.
         */
        bool isCreated();

        using authentication_check_callback = UINT (*)(NX_WEB_HTTP_SERVER *server_ptr,
                                                       UINT request_type, CHAR *resource, CHAR **name,
                                                       CHAR **password, CHAR **realm);
        using request_notify_callback = UINT (*)(NX_WEB_HTTP_SERVER *server_ptr,
                                                 UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);

        UINT create(CHAR *http_server_name,
                    NX_IP *ip_ptr,
                    UINT server_port,
                    FX_MEDIA *media_ptr,
                    VOID *stack_ptr,
                    ULONG stack_size,
                    NX_PACKET_POOL *pool_ptr,
                    authentication_check_callback authentication_check,
                    request_notify_callback request_notify
        );

        UINT del();

        UINT start();

        UINT stop();

        UINT type_get_extended(CHAR *name, UINT name_length,
                               CHAR *http_type_string, UINT http_type_string_max_size, UINT *string_size);

        UINT param_get(NX_PACKET *packet_ptr, UINT param_number, CHAR *param_ptr, UINT *param_size,
                       UINT max_param_size);

        UINT query_get(NX_PACKET *packet_ptr, UINT query_number, CHAR *query_ptr, UINT *query_size,
                       UINT max_query_size);


#if defined(LIBSMART_STM32NETX_ENABLE_TLS) && defined(NX_WEB_HTTPS_ENABLE)

#endif

    protected:
        Stm32ThreadX::EventFlags flags{"Stm32NetXHttpWebServer::Client::flags", getLogger()};

    private:
        Stm32NetX::NetX *nx{};
    };
}
