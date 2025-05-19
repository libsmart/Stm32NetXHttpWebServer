/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Stm32NetXHttpWebServer.hpp"
#include "Loggable.hpp"
#include "Nameable.hpp"
#include "Result.hpp"
#include "EventFlags/EventFlags.hpp"
#include "Stm32NetX.hpp"
#include "Callback/GenericCallback.hpp"
#include "String/FixedString.hpp"

extern "C" {
#include "nx_web_http_server.h"
}

using namespace Stm32Common;
using namespace Stm32NetX;

namespace Stm32NetXHttpWebServer {
    class BaseServer : protected NX_WEB_HTTP_SERVER, public Stm32ItmLogger::Loggable, public Stm32Common::Nameable {
        friend class Request;

    public:
        static constexpr const char *COMPONENT_NAME = Stm32NetXHttpWebServer::COMPONENT_NAME;
        static constexpr char CLASS_NAME[] = "BaseServer";
        const char *INSTANCE_NAME{getName()};

        using Resource = String::FixedString<NX_WEB_HTTP_MAX_RESOURCE>;
        using Name = String::FixedString<NX_WEB_HTTP_MAX_NAME>;
        using Password = String::FixedString<NX_WEB_HTTP_MAX_PASSWORD>;
        using Realm = String::FixedString<NX_WEB_HTTP_MAX_PASSWORD>;

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

        using cache_info_get_callback = UINT (*)(CHAR *resource, UINT *max_age, NX_WEB_HTTP_SERVER_DATE *date);

        nxHttpResult_t cache_info_callback_set(cache_info_get_callback cache_info_get);

        nxHttpResult_t callback_data_send(VOID *data_ptr, ULONG data_length);

        nxHttpResult_t callback_generate_response_header(NX_PACKET **packet_pptr,
                                                         const CHAR *status_code,
                                                         UINT content_length,
                                                         const CHAR *content_type,
                                                         const CHAR *additional_header);

        nxHttpResult_t callback_generate_response_header_extended(NX_PACKET **packet_pptr,
                                                                  CHAR *status_code,
                                                                  UINT status_code_length,
                                                                  UINT content_length,
                                                                  CHAR *content_type,
                                                                  UINT content_type_length,
                                                                  CHAR *additional_header,
                                                                  UINT additional_header_length);

        nxHttpResult_t callback_packet_send(NX_PACKET *packet_ptr);

        nxHttpResult_t callback_response_send(CHAR *header, CHAR *information, CHAR *additional_info);

        nxHttpResult_t callback_response_send_extended(const CHAR *header, UINT header_length,
                                                       const CHAR *information,
                                                       UINT information_length,
                                                       const CHAR *additional_info,
                                                       UINT additional_info_length);

        nxHttpResult_t content_get(NX_PACKET *packet_ptr,
                                   ULONG byte_offset,
                                   CHAR *destination_ptr,
                                   UINT destination_size,
                                   UINT *actual_size);

        // nxHttpResult_t content_get_extended(NX_PACKET *packet_ptr,
        //                                     ULONG byte_offset,
        //                                     CHAR *destination_ptr,
        //                                     UINT destination_size,
        //                                     UINT *actual_size);

        nxHttpResult_t content_length_get(NX_PACKET *packet_ptr, ULONG *content_length);

        nxHttpResult_t get_entity_content(NX_PACKET **packet_pptr,
                                          ULONG *available_offset,
                                          ULONG *available_length);

        nxHttpResult_t get_entity_header(NX_PACKET **packet_pptr,
                                         UCHAR *entity_header_buffer,
                                         ULONG buffer_size);

        // using gmt_get_callback = VOID (*)(NX_WEB_HTTP_SERVER_DATE *date);
        // nxHttpResult_t gmt_callback_set(gmt_get_callback gmt_get);

        // using invalid_username_password_callback = UINT (*)(CHAR *resource, ULONG client_address, UINT request_type);
        // nxHttpResult_t invalid_userpassword_notify_set(invalid_username_password_callback invalid_username_password);

        // nxHttpResult_t mime_maps_additional_set(NX_WEB_HTTP_SERVER_MIME_MAP *mime_maps, UINT mime_maps_num);

        // nxHttpResult_t response_packet_allocate(NX_PACKET **packet_ptr, ULONG wait_option);

        // nxHttpResult_t packet_content_find(NX_PACKET **packet_ptr, UINT *content_length);

        // nxHttpResult_t packet_get(NX_PACKET **packet_ptr);

        nxHttpResult_t param_get(NX_PACKET *packet_ptr, UINT param_number, CHAR *param_ptr, UINT *param_size,
                       UINT max_param_size);

        UINT query_get(NX_PACKET *packet_ptr, UINT query_number, CHAR *query_ptr, UINT *query_size,
                       UINT max_query_size);

        // nxHttpResult_t response_chunked_set(UINT chunk_size, NX_PACKET *packet_ptr);

        // nxHttpResult_t type_get(CHAR *name, CHAR *http_type_string, UINT *string_size);

        UINT type_get_extended(CHAR *name, UINT name_length,
                               CHAR *http_type_string, UINT http_type_string_max_size, UINT *string_size);

        // using digest_authenticate_callback = UINT (*)(NX_WEB_HTTP_SERVER *server_ptr,
        // CHAR *name_ptr,
        // CHAR *realm_ptr,
        // CHAR *password_ptr,
        // CHAR *method,
        // CHAR *authorization_uri,
        // CHAR *authorization_nc,
        // CHAR *authorization_cnonce);
        // nxHttpResult_t digest_authenticate_notify_set(digest_authenticate_callback digest_authenticate);

        using authentication_check_extended_callback = UINT (*)(
        NX_WEB_HTTP_SERVER *server_ptr,
        UINT request_type,
        CHAR *resource,
        CHAR **name,
        UINT *name_length,
        CHAR **password,
        UINT *password_length,
        CHAR **realm,
        UINT *realm_length);
        nxHttpResult_t authenticate_check_set(authentication_check_extended_callback authentication_check_extended);


#if defined(LIBSMART_STM32NETX_ENABLE_TLS) && defined(NX_WEB_HTTPS_ENABLE)
        nxHttpResult_t secure_configure(const NX_SECURE_TLS_CRYPTO *crypto_table,
                                        VOID *metadata_buffer,
                                        ULONG metadata_size,
                                        UCHAR *packet_buffer,
                                        UINT packet_buffer_size,
                                        NX_SECURE_X509_CERT *identity_certificate,
                                        NX_SECURE_X509_CERT *trusted_certificates[],
                                        UINT trusted_certs_num,
                                        NX_SECURE_X509_CERT *remote_certificates[],
                                        UINT remote_certs_num,
                                        UCHAR *remote_certificate_buffer,
                                        UINT remote_cert_buffer_size);
#endif

    protected:
        Stm32ThreadX::EventFlags flags{"Stm32NetXHttpWebServer::Client::flags", getLogger()};

    private:
        Stm32NetX::NetX *nx{};
    };
}
