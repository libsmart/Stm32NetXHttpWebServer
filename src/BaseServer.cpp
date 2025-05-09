/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "BaseServer.hpp"
#include <climits>
#include <stdexcept>

#include "NetXHttpReturnValues.hpp"
#include "Address/Address.hpp"
#include "Address/AddressPrinter.hpp"
#include "Exception/NetXHttpWebServerException.hpp"
#include "NetXReturnValues.hpp"

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
                                               http_server_name, ip_ptr, server_port, media_ptr, stack_ptr, stack_size,
                                               pool_ptr,
                                               authentication_check, request_notify
    );

    if (ret != NX_SUCCESS) {
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

    if (ret != NX_SUCCESS) {
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

    if (ret != NX_SUCCESS) {
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

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_stop() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseServer::type_get_extended(CHAR *name, UINT name_length, CHAR *http_type_string, UINT http_type_string_max_size,
                                   UINT *string_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::type_get_extended()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_type_get_extended
    const auto ret = nx_web_http_server_type_get_extended(this, name, name_length, http_type_string,
                                                          http_type_string_max_size, string_size);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] =
                "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_type_get_extended() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseServer::param_get(NX_PACKET *packet_ptr, UINT param_number, CHAR *param_ptr, UINT *param_size,
                           UINT max_param_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::param_get()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_param_get
    const auto ret = nx_web_http_server_param_get(packet_ptr, param_number, param_ptr, param_size, max_param_size);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_param_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseServer::query_get(NX_PACKET *packet_ptr, UINT query_number, CHAR *query_ptr, UINT *query_size,
                           UINT max_query_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::query_get()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_query_get
    const auto ret = nx_web_http_server_query_get(packet_ptr, query_number, query_ptr, query_size, max_query_size);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_query_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

nxHttpResult_t BaseServer::callback_data_send(void *data_ptr, ULONG data_length) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::callback_data_send()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_callback_data_send
    const auto ret = nx_web_http_server_callback_data_send(this, data_ptr, data_length);

    if (ret != NX_SUCCESS) {
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header()", res, NetXHttpWebServerException)
    }

    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::callback_generate_response_header(NX_PACKET **packet_pptr, const CHAR *status_code,
                                                       const UINT content_length,
                                                       const CHAR *content_type, const CHAR *additional_header) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::callback_generate_response_header()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_callback_generate_response_header
    auto ret = nx_web_http_server_callback_generate_response_header(this,
                                                                          packet_pptr,
                                                                          const_cast<CHAR *>(status_code),
                                                                          content_length,
                                                                          const_cast<CHAR *>(content_type),
                                                                          const_cast<CHAR *>(additional_header));

    // ret = 0x01;
    // ret = 0xFFF;
    // ret = 0x30003;
    if (ret != NX_SUCCESS) {
        // const auto nxHttpRet = NetXHttpReturnValues::find(ret);
        // if (nxHttpRet.has_value()) {
            // auto res = nxHttpResult_t::err(nxHttpRet.value());
            // LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header()", res, NetXHttpWebServerException)
        // }

        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header()", res, NetXHttpWebServerException)
    }

    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::callback_packet_send(NX_PACKET *packet_ptr) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
        ->printf("%s::%s[%s]::callback_packet_send()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_callback_packet_send
    const auto ret = nx_web_http_server_callback_packet_send(this, packet_ptr);

    if (ret != NX_SUCCESS) {
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header()", res, NetXHttpWebServerException)
    }
    return nxHttpResult_t::ok();
}
