/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#pragma once
#include "NetXHttpReturnValues.pch"
#include "Enum/EnumRegistry.hpp"


namespace Stm32NetX::Common {
    using namespace Stm32Common;
    using namespace Stm32Common::Hash;

    struct NetXHttpReturn {
        enum class Id : UINT {
            NONE = UINT_MAX,
            /// HTTP internal error
            WEB_HTTP_ERROR = NX_WEB_HTTP_ERROR,
            /// HTTP timeout occurred
            WEB_HTTP_TIMEOUT = NX_WEB_HTTP_TIMEOUT,
            /// HTTP error
            WEB_HTTP_FAILED = NX_WEB_HTTP_FAILED,
            /// HTTP authentication not needed
            WEB_HTTP_DONT_AUTHENTICATE = NX_WEB_HTTP_DONT_AUTHENTICATE,
            /// HTTP basic authentication requested
            WEB_HTTP_BASIC_AUTHENTICATE = NX_WEB_HTTP_BASIC_AUTHENTICATE,
            /// HTTP digest authentication requested
            WEB_HTTP_DIGEST_AUTHENTICATE = NX_WEB_HTTP_DIGEST_AUTHENTICATE,
            /// HTTP request not found
            WEB_HTTP_NOT_FOUND = NX_WEB_HTTP_NOT_FOUND,
            /// HTTP end of content area
            WEB_HTTP_DATA_END = NX_WEB_HTTP_DATA_END,
            /// HTTP user callback completed the processing
            WEB_HTTP_CALLBACK_COMPLETED = NX_WEB_HTTP_CALLBACK_COMPLETED,
            /// HTTP supplied pool payload is too small
            WEB_HTTP_POOL_ERROR = NX_WEB_HTTP_POOL_ERROR,
            /// HTTP client not ready for operation
            WEB_HTTP_NOT_READY = NX_WEB_HTTP_NOT_READY,
            /// HTTP client get is complete
            WEB_HTTP_GET_DONE = NX_WEB_HTTP_GET_DONE,
            /// Invalid packet received - length incorrect
            WEB_HTTP_BAD_PACKET_LENGTH = NX_WEB_HTTP_BAD_PACKET_LENGTH,
            /// Received an error code instead of 2xx from server
            WEB_HTTP_REQUEST_UNSUCCESSFUL_CODE = NX_WEB_HTTP_REQUEST_UNSUCCESSFUL_CODE,
            /// Server responds before PUT is complete
            WEB_HTTP_INCOMPLETE_PUT_ERROR = NX_WEB_HTTP_INCOMPLETE_PUT_ERROR,
            /// Password exceeded expected length
            WEB_HTTP_PASSWORD_TOO_LONG = NX_WEB_HTTP_PASSWORD_TOO_LONG,
            /// Username exceeded expected length
            WEB_HTTP_USERNAME_TOO_LONG = NX_WEB_HTTP_USERNAME_TOO_LONG,
            /// Server unable to find query in client request
            WEB_HTTP_NO_QUERY_PARSED = NX_WEB_HTTP_NO_QUERY_PARSED,
            /// Client method (e.g. GET, POST) was missing required information.
            WEB_HTTP_METHOD_ERROR = NX_WEB_HTTP_METHOD_ERROR,
            /// Client request parameter not properly terminated
            WEB_HTTP_IMPROPERLY_TERMINATED_PARAM = NX_WEB_HTTP_IMPROPERLY_TERMINATED_PARAM,
            /// Boundary is already found.
            WEB_HTTP_BOUNDARY_ALREADY_FOUND = NX_WEB_HTTP_BOUNDARY_ALREADY_FOUND,
            /// The Content-Length header was not found.
            WEB_HTTP_MISSING_CONTENT_LENGTH = NX_WEB_HTTP_MISSING_CONTENT_LENGTH,
            /// A searched-for HTTP type extension was not found.
            WEB_HTTP_EXTENSION_NOT_FOUND = NX_WEB_HTTP_EXTENSION_NOT_FOUND,
            /// No matching extension found, return default.
            WEB_HTTP_EXTENSION_MIME_DEFAULT = NX_WEB_HTTP_EXTENSION_MIME_DEFAULT,
        };

        struct Meta {
            Id id;
            UINT apiId;
#if defined(LIBSMART_ENABLE_ERROR_HASHES)
            FNV1a::fnv1a32_t strHash;
#endif
#if defined(LIBSMART_ENABLE_ERROR_STRINGS)
            const char *name;
#endif
        };

#if defined(LIBSMART_ENABLE_ERROR_HASHES)
#define IF_HASHES(...) , __VA_ARGS__
#else
#define IF_HASHES(...)
#endif

#if defined(LIBSMART_ENABLE_ERROR_STRINGS)
#define IF_STRINGS(...) , __VA_ARGS__
#else
#define IF_STRINGS(...)
#endif


#define DATA_LINE(x) { Id::x, NX_##x \
IF_HASHES(FNV1a::hash("NX_" #x)) \
IF_STRINGS("NX_" #x) \
}

        static constexpr Meta data[] = {
            {Id::NONE, UINT_MAX, FNV1a::hash("NONE"), "NONE"},
            DATA_LINE(WEB_HTTP_ERROR),
            DATA_LINE(WEB_HTTP_TIMEOUT),
            DATA_LINE(WEB_HTTP_FAILED),
            DATA_LINE(WEB_HTTP_DONT_AUTHENTICATE),
            DATA_LINE(WEB_HTTP_BASIC_AUTHENTICATE),
            DATA_LINE(WEB_HTTP_DIGEST_AUTHENTICATE),
            DATA_LINE(WEB_HTTP_NOT_FOUND),
            DATA_LINE(WEB_HTTP_DATA_END),
            DATA_LINE(WEB_HTTP_CALLBACK_COMPLETED),
            DATA_LINE(WEB_HTTP_POOL_ERROR),
            DATA_LINE(WEB_HTTP_NOT_READY),
            DATA_LINE(WEB_HTTP_GET_DONE),
            DATA_LINE(WEB_HTTP_BAD_PACKET_LENGTH),
            DATA_LINE(WEB_HTTP_REQUEST_UNSUCCESSFUL_CODE),
            DATA_LINE(WEB_HTTP_INCOMPLETE_PUT_ERROR),
            DATA_LINE(WEB_HTTP_PASSWORD_TOO_LONG),
            DATA_LINE(WEB_HTTP_USERNAME_TOO_LONG),
            DATA_LINE(WEB_HTTP_NO_QUERY_PARSED),
            DATA_LINE(WEB_HTTP_METHOD_ERROR),
            DATA_LINE(WEB_HTTP_IMPROPERLY_TERMINATED_PARAM),
            DATA_LINE(WEB_HTTP_BOUNDARY_ALREADY_FOUND),
            DATA_LINE(WEB_HTTP_MISSING_CONTENT_LENGTH),
            DATA_LINE(WEB_HTTP_EXTENSION_NOT_FOUND),
            DATA_LINE(WEB_HTTP_EXTENSION_MIME_DEFAULT),
        };

        static constexpr EnumRegistry registry{data};

        using value = RegistryView<Meta>;

        static constexpr auto extractApiId(const Meta &e) { return e.apiId; };

        static constexpr std::optional<value> find(const UINT code) {
            auto a = registry.find<UINT, extractApiId>(code);
            if (a.has_value()) return value(&a.value());
            return std::nullopt;
        }

        // static constexpr auto extractId(const Meta &e) { return e.id; };
        // static constexpr value find(const Id code) {
        //     return value(registry.find<Id, extractId>(code));
        // }


        struct NetXHttpResult {
            NetXHttpResult() { ; }

            template<typename E>
            explicit NetXHttpResult(const E e) : _v(e) { ; }

            NetXHttpResult(const Common::NetXHttpReturn::Meta &e) : _v(e) { ; }
            NetXHttpResult(const UINT e) : apiId(e) { _v.apiId = e; }
            NetXHttpResult(const char *e) : msg(e) { ; }


            UINT apiReturnValue() const {
                return _v.apiId;
            }

#if defined(LIBSMART_ENABLE_ERROR_HASHES)
            FNV1a::fnv1a32_t strHash() const {
                return _v.strHash;
            }
#endif

#if defined(LIBSMART_ENABLE_ERROR_STRINGS)
            const char *apiReturnString() const {
                return _v.name;
            }
#endif

            const char *message() const {
                return msg;
            }

            const Common::NetXHttpReturn::Meta &raw() const { return _v; }

        private:
            Common::NetXHttpReturn::Meta _v = data[0];
            UINT apiId = UINT_MAX;
            const char *msg = "";
        };
    };
}
