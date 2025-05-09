/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

extern "C" {
#include "nx_web_http_server.h"
}

namespace Stm32NetXHttpWebServer {
    namespace Method {
        struct UNKNOWN {
            static constexpr char name[] = "UNKNOWN";
            explicit constexpr operator const char *() const { return name; }
            static constexpr UINT apiReturnValue = NX_WEB_HTTP_SERVER_UNKNOWN_REQUEST;
            explicit constexpr operator UINT() const { return apiReturnValue; }
        };

        struct GET {
            static constexpr char name[] = "GET";
            explicit constexpr operator const char *() const { return name; }
            static constexpr UINT apiReturnValue = NX_WEB_HTTP_SERVER_GET_REQUEST;
            explicit constexpr operator UINT() const { return apiReturnValue; }
        };

        struct POST {
            static constexpr char name[] = "POST";
            explicit constexpr operator const char *() const { return name; }
            static constexpr UINT apiReturnValue = NX_WEB_HTTP_SERVER_POST_REQUEST;
            explicit constexpr operator UINT() const { return apiReturnValue; }
        };

        struct HEAD {
            static constexpr char name[] = "HEAD";
            explicit constexpr operator const char *() const { return name; }
            static constexpr UINT apiReturnValue = NX_WEB_HTTP_SERVER_HEAD_REQUEST;
            explicit constexpr operator UINT() const { return apiReturnValue; }
        };

        struct PUT {
            static constexpr char name[] = "PUT";
            explicit constexpr operator const char *() const { return name; }
            static constexpr UINT apiReturnValue = NX_WEB_HTTP_SERVER_PUT_REQUEST;
            explicit constexpr operator UINT() const { return apiReturnValue; }
        };

        struct DELETE {
            static constexpr char name[] = "DELETE";
            explicit constexpr operator const char *() const { return name; }
            static constexpr UINT apiReturnValue = NX_WEB_HTTP_SERVER_DELETE_REQUEST;
            explicit constexpr operator UINT() const { return apiReturnValue; }
        };


        using Variant = std::variant<
            UNKNOWN,
            GET,
            POST,
            HEAD,
            PUT,
            DELETE
        >;


        // Eigene Implementierung von 'type_identity' für C++17
        template<typename T>
        struct type_identity {
            using type = T;
        };

        // Hilfsfunktion: Schleife über alle Typen der `Variant`
        template<typename Func, size_t... Is>
        static void forEachTypeImpl(Func &&func, std::index_sequence<Is...>) {
            using VariantType = Variant;
            (..., func(type_identity<std::variant_alternative_t<Is, VariantType> >{}));
        }

        template<typename Func>
        static void forEachType(Func &&func) {
            constexpr size_t VariantSize = std::variant_size<Variant>::value;
            forEachTypeImpl(std::forward<Func>(func), std::make_index_sequence<VariantSize>{});
        }

        static auto find(uint32_t subject) -> std::optional<Variant> {
            std::optional<Variant> result;

            forEachType(
                [&subject, &result](const auto type_info) -> void {
                    using T = typename decltype(type_info)::type;
                    T instance{};
                    if (instance.apiReturnValue == subject) {
                        result = instance;
                    }
                });

            return result;
        }
    }


    struct ServerHttpMethod {
        ServerHttpMethod() : value(Method::UNKNOWN{}) { ; }
        explicit ServerHttpMethod(const Method::Variant &value) : value(value) { ; }

        template<typename E>
        explicit ServerHttpMethod(const E e) : value(e) { ; }

        explicit ServerHttpMethod(const UINT id) { value = Method::find(id).value_or(Method::UNKNOWN{}); }

        template<typename Visitor>
        decltype(auto) visit(Visitor &&v) const {
            return std::visit(std::forward<Visitor>(v), value);
        }

        explicit operator const char *() const { return visit([](auto &&e) { return e.name; }); }
        explicit operator UINT() const { return visit([](auto &&e) { return e.apiReturnValue; }); }

        template<typename T>
        bool is() const { return std::holds_alternative<T>(raw()); }

        template<typename T>
        bool operator==(const T &) const { return is<T>(); }

        const Method::Variant &raw() const { return value; }

    private:
        Method::Variant value;
    };
}
