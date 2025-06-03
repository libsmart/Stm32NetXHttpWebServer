/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "BaseServer.hpp"
#include <climits>
#include <stdexcept>

#include "Address/Address.hpp"
#include "Address/AddressPrinter.hpp"
#include "Exception/NetXException.hpp"
#include "Exception/NetXHttpWebServerException.hpp"
#include "Result/NetXReturnValues.hpp"
#include "Result/NetXResult.hpp"

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

nxHttpResult_t BaseServer::cache_info_callback_set(cache_info_get_callback cache_info_get) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::cache_info_callback_set()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_cache_info_callback_set
    const auto ret = nx_web_http_server_cache_info_callback_set(this, cache_info_get);

    if (ret != NX_SUCCESS) {
        auto nxRet = NetXReturn::find(ret);
        if (nxRet.has_value()) {
            auto d = nxHttpResult_t::err(nxRet.value().get());
            LIBSMART_EXCEPTION("nx_web_http_server_cache_info_callback_set()", d, NetXHttpWebServerException)
        }

        auto nxHttpRet = Common::NetXHttpReturn::find(ret);
        if (nxHttpRet.has_value()) {
            auto d = nxHttpResult_t::err(nxHttpRet.value().get());
            LIBSMART_EXCEPTION("nx_web_http_server_cache_info_callback_set()", d, NetXHttpWebServerException)
        }
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_cache_info_callback_set()", res, NetXHttpWebServerException)
    }

    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::callback_data_send(void *data_ptr, ULONG data_length) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::callback_data_send()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_callback_data_send
    const auto ret = nx_web_http_server_callback_data_send(this, data_ptr, data_length);

    if (ret != NX_SUCCESS) {
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_callback_data_send()", res, NetXHttpWebServerException)
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
        auto nxRet = NetXReturn::find(ret);
        if (nxRet.has_value()) {
            auto d = nxHttpResult_t::err(nxRet.value().get());
            LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header()", d, NetXHttpWebServerException)
        }

        auto nxHttpRet = Common::NetXHttpReturn::find(ret);
        if (nxHttpRet.has_value()) {
            auto d = nxHttpResult_t::err(nxHttpRet.value().get());
            LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header()", d, NetXHttpWebServerException)
        }
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header()", res, NetXHttpWebServerException)
    }

    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::callback_generate_response_header_extended(NX_PACKET **packet_pptr, CHAR *status_code,
                                                                      UINT status_code_length, UINT content_length,
                                                                      CHAR *content_type, UINT content_type_length,
                                                                      CHAR *additional_header,
                                                                      UINT additional_header_length) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::callback_generate_response_header_extended()\r\n", COMPONENT_NAME, CLASS_NAME,
                     getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_callback_generate_response_header_extended
    auto ret = nx_web_http_server_callback_generate_response_header_extended(this,
                                                                             packet_pptr, status_code,
                                                                             status_code_length, content_length,
                                                                             content_type, content_type_length,
                                                                             additional_header,
                                                                             additional_header_length);

    if (ret != NX_SUCCESS) {
        auto nxRet = NetXReturn::find(ret);
        if (nxRet.has_value()) {
            auto d = nxHttpResult_t::err(nxRet.value().get());
            LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header_extended()", d,
                               NetXHttpWebServerException)
        }

        auto nxHttpRet = Common::NetXHttpReturn::find(ret);
        if (nxHttpRet.has_value()) {
            auto d = nxHttpResult_t::err(nxHttpRet.value().get());
            LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header_extended()", d,
                               NetXHttpWebServerException)
        }
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_callback_generate_response_header_extended()", res,
                           NetXHttpWebServerException)
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
        LIBSMART_EXCEPTION("nx_web_http_server_callback_packet_send()", res, NetXHttpWebServerException)
    }
    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::callback_response_send(CHAR *header, CHAR *information, CHAR *additional_info) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::callback_response_send()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_callback_response_send
    const auto ret = nx_web_http_server_callback_response_send(this, header, information, additional_info);

    if (ret != NX_SUCCESS) {
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_callback_response_send()", res, NetXHttpWebServerException)
    }
    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::callback_response_send_extended(const CHAR *header, const UINT header_length,
                                                           const CHAR *information,
                                                           const UINT information_length, const CHAR *additional_info,
                                                           const UINT additional_info_length) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::callback_response_send_extended()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_callback_response_send_extended
    const auto ret = nx_web_http_server_callback_response_send_extended(this, const_cast<CHAR *>(header),
                                                                        header_length,
                                                                        const_cast<CHAR *>(information),
                                                                        information_length,
                                                                        const_cast<CHAR *>(additional_info),
                                                                        additional_info_length);

    if (ret != NX_SUCCESS) {
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_callback_response_send_extended()", res, NetXHttpWebServerException)
    }
    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::content_get(NX_PACKET *packet_ptr, ULONG byte_offset, CHAR *destination_ptr,
                                       UINT destination_size, UINT *actual_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::content_get()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_content_get
    const auto ret = nx_web_http_server_content_get_extended(this, packet_ptr, byte_offset,
                                                    destination_ptr, destination_size, actual_size);

    if (ret != NX_SUCCESS) {
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_content_get()", res, NetXHttpWebServerException)
    }
    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::content_length_get(NX_PACKET *packet_ptr, ULONG *content_length) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::content_length_get()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_content_length_get
    const auto ret = nx_web_http_server_content_length_get(packet_ptr, content_length);

    if (ret != NX_SUCCESS) {
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_content_length_get()", res, NetXHttpWebServerException)
    }
    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::get_entity_content(NX_PACKET **packet_pptr, ULONG *available_offset,
                                              ULONG *available_length) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::get_entity_content()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

#if !defined(NX_WEB_HTTP_MULTIPART_ENABLE)
    auto res = nxHttpResult_t::err(NX_NOT_ENABLED);
    LIBSMART_EXCEPTION("nx_web_http_server_get_entity_header()", res, NetXHttpWebServerException)
#endif

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_get_entity_content
    const auto ret = nx_web_http_server_get_entity_content(this, packet_pptr, available_offset, available_length);

    if (ret != NX_SUCCESS) {
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_get_entity_content()", res, NetXHttpWebServerException)
    }
    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::get_entity_header(NX_PACKET **packet_pptr, UCHAR *entity_header_buffer, ULONG buffer_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::get_entity_header()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

#if !defined(NX_WEB_HTTP_MULTIPART_ENABLE)
    auto res = nxHttpResult_t::err(NX_NOT_ENABLED);
    LIBSMART_EXCEPTION("nx_web_http_server_get_entity_header()", res, NetXHttpWebServerException)
#endif

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_get_entity_header
    const auto ret = nx_web_http_server_get_entity_header(this, packet_pptr, entity_header_buffer, buffer_size);

    if (ret != NX_SUCCESS) {
        auto a = Common::NetXHttpReturn::find(ret);
        if (a.has_value()) {
            auto b = a.value();
            auto c = a.value().get();
            auto d = nxHttpResult_t::err(c);
            LIBSMART_EXCEPTION("nx_web_http_server_get_entity_header(x)", d, NetXHttpWebServerException)
        }
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_get_entity_header()", res, NetXHttpWebServerException)
    }
    return nxHttpResult_t::ok();
}

nxHttpResult_t BaseServer::param_get(NX_PACKET *packet_ptr, UINT param_number, CHAR *param_ptr, UINT *param_size,
                                     UINT max_param_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::param_get()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_param_get
    const auto ret = nx_web_http_server_param_get(packet_ptr, param_number, param_ptr, param_size, max_param_size);

    if (ret != NX_SUCCESS) {
        auto nxRet = NetXReturn::find(ret);
        if (nxRet.has_value()) {
            auto d = nxHttpResult_t::err(nxRet.value().get());
            LIBSMART_EXCEPTION("nx_web_http_server_param_get()", d, NetXHttpWebServerException)
        }

        auto nxHttpRet = Common::NetXHttpReturn::find(ret);
        if (nxHttpRet.has_value()) {
            auto d = nxHttpResult_t::err(nxHttpRet.value().get());
            LIBSMART_EXCEPTION("nx_web_http_server_param_get()", d, NetXHttpWebServerException)
        }
        auto res = nxHttpResult_t::err(ret);
        LIBSMART_EXCEPTION("nx_web_http_server_param_get()", res, NetXHttpWebServerException)
    }

    return nxHttpResult_t::ok();
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

nxHttpResult_t
BaseServer::authenticate_check_set(authentication_check_extended_callback authentication_check_extended) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32NetXHttpWebServer::BaseServer[%s]::authenticate_check_set()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_authenticate_check_set
    const auto ret = nx_web_http_server_authentication_check_set(this, authentication_check_extended);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] =
                "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_authentication_check_set() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return nxHttpResult_t::ok();
}

#if defined(LIBSMART_STM32NETX_ENABLE_TLS) && defined(NX_WEB_HTTPS_ENABLE)
nxHttpResult_t BaseServer::secure_configure(const NX_SECURE_TLS_CRYPTO *crypto_table,
                                            void *metadata_buffer,
                                            ULONG metadata_size,
                                            UCHAR *packet_buffer,
                                            UINT packet_buffer_size,
                                            NX_SECURE_X509_CERT *identity_certificate,
                                            NX_SECURE_X509_CERT *trusted_certificates[],
                                            UINT trusted_certs_num,
                                            NX_SECURE_X509_CERT *remote_certificates[],
                                            UINT remote_certs_num,
                                            UCHAR *remote_certificate_buffer,
                                            UINT remote_cert_buffer_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::secure_configure()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/netx-duo/netx-duo-web-http/chapter3.md#nx_web_http_server_secure_configure
    const auto ret = nx_web_http_server_secure_configure(this,
                                                         crypto_table,
                                                         metadata_buffer,
                                                         metadata_size,
                                                         packet_buffer,
                                                         packet_buffer_size,
                                                         identity_certificate,
                                                         trusted_certificates,
                                                         trusted_certs_num,
                                                         remote_certificates,
                                                         remote_certs_num,
                                                         remote_certificate_buffer,
                                                         remote_cert_buffer_size
    );

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] =
                "Stm32NetXHttpWebServer::BaseServer[%s]: nx_web_http_server_secure_configure() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return nxHttpResult_t::ok();
}
#endif
