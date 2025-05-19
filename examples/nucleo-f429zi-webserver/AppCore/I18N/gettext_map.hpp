/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include "Hash/Fnv1a.hpp"

using namespace Stm32Common::Hash;

#define _(x) AppCore::I18N::gettext(x)
#define __(x) AppCore::I18N::gettext_by_hash(x)

namespace AppCore::I18N {
    enum class Language : uint8_t {
        EN = 0,
        DE,
        FR,
        IT,
        Count
    };

    //inline auto currentLanguage = Language::EN;

    using hash_t = uint32_t;

    struct TranslationEntry {
        hash_t hash;
        const char *translations[static_cast<size_t>(Language::Count) - 1];
    };

    struct TextEntry {
        hash_t hash;
        const char *translations[static_cast<size_t>(Language::Count)];
    };

    constexpr char EMPTY_STRING[] = "";
    constexpr char UNKNOWN_ERROR_STRING[] = "Unknown error";

    inline constexpr TranslationEntry translation_table[] = {
        {FNV1a::hash(UNKNOWN_ERROR_STRING), {"Unbekannter Fehler", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("WebAPI Error:"), {"WebAPI Fehler:", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("Received data malformed"), {"Ungültige Daten empfangen", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash(""), {EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
    };
    inline constexpr size_t translation_table_size = sizeof(translation_table) / sizeof(translation_table[0]);

    inline constexpr TextEntry text_table[] = {
        {FNV1a::hash(UNKNOWN_ERROR_STRING), {UNKNOWN_ERROR_STRING, "Unbekannter Fehler", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_SUCCESS"), {"Successful", "Erfolgreich", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NO_PACKET"), {"No packet", "Kein Paket", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_UNDERFLOW"), {"Underflow", "Unterlauf", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_OVERFLOW"), {"Overflow", "Überlauf", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NO_MAPPING"), {"NX_NO_MAPPING", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_DELETED"), {"Deleted", "Gelöscht", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_POOL_ERROR"), {"NX_POOL_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_PTR_ERROR"), {"NX_PTR_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_WAIT_ERROR"), {"NX_WAIT_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_SIZE_ERROR"), {"NX_SIZE_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_OPTION_ERROR"), {"NX_OPTION_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_DELETE_ERROR"), {"NX_DELETE_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_CALLER_ERROR"), {"NX_CALLER_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_INVALID_PACKET"), {"NX_INVALID_PACKET", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_INVALID_SOCKET"), {"NX_INVALID_SOCKET", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_ENABLED"), {"NX_NOT_ENABLED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_ALREADY_ENABLED"), {"NX_ALREADY_ENABLED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_ENTRY_NOT_FOUND"), {"NX_ENTRY_NOT_FOUND", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NO_MORE_ENTRIES"), {"NX_NO_MORE_ENTRIES", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_ARP_TIMER_ERROR"), {"NX_ARP_TIMER_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_RESERVED_CODE0"), {"NX_RESERVED_CODE0", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_WAIT_ABORTED"), {"NX_WAIT_ABORTED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_IP_INTERNAL_ERROR"), {"NX_IP_INTERNAL_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_IP_ADDRESS_ERROR"), {"Invalid ip address", "Ungültige IP-Adresse", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_ALREADY_BOUND"), {"NX_ALREADY_BOUND", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_PORT_UNAVAILABLE"), {"NX_PORT_UNAVAILABLE", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_BOUND"), {"NX_NOT_BOUND", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_RESERVED_CODE1"), {"NX_RESERVED_CODE1", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_SOCKET_UNBOUND"), {"NX_SOCKET_UNBOUND", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_CREATED"), {"Not ready", "Nicht bereit", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_SOCKETS_BOUND"), {"NX_SOCKETS_BOUND", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NO_RESPONSE"), {"NX_NO_RESPONSE", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_POOL_DELETED"), {"NX_POOL_DELETED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_ALREADY_RELEASED"), {"NX_ALREADY_RELEASED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_RESERVED_CODE2"), {"NX_RESERVED_CODE2", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_MAX_LISTEN"), {"NX_MAX_LISTEN", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_DUPLICATE_LISTEN"), {"NX_DUPLICATE_LISTEN", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_CLOSED"), {"NX_NOT_CLOSED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_LISTEN_STATE"), {"NX_NOT_LISTEN_STATE", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_IN_PROGRESS"), {"NX_IN_PROGRESS", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_CONNECTED"), {"No connection", "Keine Verbindung", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_WINDOW_OVERFLOW"), {"NX_WINDOW_OVERFLOW", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_ALREADY_SUSPENDED"), {"NX_ALREADY_SUSPENDED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_DISCONNECT_FAILED"), {"NX_DISCONNECT_FAILED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_STILL_BOUND"), {"NX_STILL_BOUND", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_SUCCESSFUL"), {"Not successful", "Nicht erfolgreich", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_UNHANDLED_COMMAND"), {"NX_UNHANDLED_COMMAND", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NO_FREE_PORTS"), {"No free ports", "Keine freien Ports", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_INVALID_PORT"), {"Invalid port", "Ungültiger Port", EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_INVALID_RELISTEN"), {"NX_INVALID_RELISTEN", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_CONNECTION_PENDING"), {"NX_CONNECTION_PENDING", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_TX_QUEUE_DEPTH"), {"NX_TX_QUEUE_DEPTH", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_IMPLEMENTED"), {"NX_NOT_IMPLEMENTED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_SUPPORTED"), {"NX_NOT_SUPPORTED", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_INVALID_INTERFACE"), {"NX_INVALID_INTERFACE", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_INVALID_PARAMETERS"), {"NX_INVALID_PARAMETERS", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NOT_FOUND"), {"NX_NOT_FOUND", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_CANNOT_START"), {"NX_CANNOT_START", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_NO_INTERFACE_ADDRESS"), {"NX_NO_INTERFACE_ADDRESS", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_INVALID_MTU_DATA"), {"NX_INVALID_MTU_DATA", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_DUPLICATED_ENTRY"), {"NX_DUPLICATED_ENTRY", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_PACKET_OFFSET_ERROR"), {"NX_PACKET_OFFSET_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_OPTION_HEADER_ERROR"), {"NX_OPTION_HEADER_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_CONTINUE"), {"NX_CONTINUE", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash("NX_TCPIP_OFFLOAD_ERROR"), {"NX_TCPIP_OFFLOAD_ERROR", EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
        {FNV1a::hash(EMPTY_STRING), {EMPTY_STRING, EMPTY_STRING, EMPTY_STRING, EMPTY_STRING}},
    };
    inline constexpr size_t text_table_size = sizeof(text_table) / sizeof(text_table[0]);

    const char *gettext(const char *original);

    template<size_t N>
    const char *_(const char (&str)[N]) {
        return gettext(str);
    }

    const char *gettext(hash_t origHash);

    inline const char *gettext_by_hash(const char *original) {
        return gettext(FNV1a::hash(original));
    }

    template<size_t N>
    const char *gettext_by_hash(const char (&original)[N]) {
        return gettext(FNV1a::hash(original));
    }

    // template<size_t N>
    // const char *__(const char (&str)[N]) {
    //     return gettext_by_hash(str);
    // }

    void set_language(Language lang);

    Language get_current_language();
}
