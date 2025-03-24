/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "BaseServer.hpp"
#include <climits>
#include <stdexcept>
#include "Address/Address.hpp"
#include "Address/AddressPrinter.hpp"

using namespace Stm32NetX;
using namespace Stm32NetXHttpWebServer;

#if __EXCEPTIONS
#include <stdexcept>
#define LIBSMART_HANDLE_ERROR(fmt, ...)                                          \
do {                                                                    \
char buffer[snprintf(nullptr, 0, fmt, __VA_ARGS__) + 1]{};              \
snprintf(buffer, sizeof(buffer), fmt, __VA_ARGS__);                     \
log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer); \
throw std::runtime_error(buffer);                                       \
} while (0);
#else
#define LIBSMART_HANDLE_ERROR(fmt, ...)                                          \
do {                                                                    \
char buffer[snprintf(nullptr, 0, fmt, __VA_ARGS__) + 1]{};              \
snprintf(buffer, sizeof(buffer), fmt, __VA_ARGS__);                     \
log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer); \
return ret;                                                             \
} while (0);
#endif


bool BaseServer::isCreated() {
    return flags.isSet(IS_CREATED);
}

UINT BaseServer::create(CHAR *http_server_name, NX_IP *ip_ptr, UINT server_port, FX_MEDIA *media_ptr, void *stack_ptr,
                        ULONG stack_size, NX_PACKET_POOL *pool_ptr, authentication_check_callback authentication_check,
                        request_notify_callback request_notify) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::create(\"%s\", %d)\r\n",
                     http_server_name, server_port);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_create
    const auto ret = nx_web_http_server_create(this,
        http_server_name, ip_ptr, server_port, media_ptr, stack_ptr, stack_size, pool_ptr,
        authentication_check, request_notify
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_create() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseServer::del() {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
        ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::del()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_delete
    const auto ret = nx_web_http_server_delete(this);

    std::memset(static_cast<NX_WEB_HTTP_SERVER *>(this), 0, sizeof(NX_WEB_HTTP_SERVER));

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_delete() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseServer::start() {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
    ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::start()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_start
    const auto ret = nx_web_http_server_start(this);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_start() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseServer::stop() {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
->printf("Stm32NetXHttpWebServer::BaseServer[%s]::stop()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_stop
    const auto ret = nx_web_http_server_stop(this);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_stop() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}
