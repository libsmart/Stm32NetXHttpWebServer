/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "gettext_map.hpp"
#include "globals.hpp"
#include "LoggerInterface.hpp"

using namespace AppCore::I18N;
using namespace Stm32Common::Hash;

Language AppCore::I18N::get_current_language() {
    return Language::EN;
    // auto s = nullptr;
    // if (s == nullptr) return Language::EN;
    // if (static_cast<size_t>(s->language) >= static_cast<size_t>(Language::Count)) return Language::EN;
    // return s->language;
}

void AppCore::I18N::set_language(Language lang) {
    if (static_cast<size_t>(lang) < static_cast<size_t>(Language::Count)) {
        auto s = nullptr;
        // if (s != nullptr) {s->language = lang;}
    }
}

const char *AppCore::I18N::gettext(const char *original) {
    if (get_current_language() == Language::EN) return original;

    const uint32_t origHash = FNV1a::hash(original);
    for (auto [hash, translations]: translation_table) {
        if (hash == origHash) {
            auto ret = translations[static_cast<size_t>(get_current_language()) - 1];
            if (ret == EMPTY_STRING) return original;
            return ret;
        }
    }

    Stm32ItmLogger::logger.setSeverity(Stm32ItmLogger::LoggerInterface::Severity::WARNING);
    Stm32ItmLogger::logger.print("String not found: '");
    Stm32ItmLogger::logger.print(original);
    Stm32ItmLogger::logger.println("'");
    return original;
}

const char * AppCore::I18N::gettext(const hash_t origHash) {
    for (auto [hash, translations]: text_table) {
        if (hash == origHash) {
            auto ret = translations[static_cast<size_t>(get_current_language())];
            if (ret == EMPTY_STRING) return translations[static_cast<size_t>(Language::EN)];
            return ret;
        }
    }
    constexpr char fmt[] = "String [0x%08x] not found";
    static char txt[30]{};
    snprintf(txt, sizeof(txt), fmt, origHash);
    Stm32ItmLogger::logger.setSeverity(Stm32ItmLogger::LoggerInterface::Severity::WARNING)
            ->println(txt);
    return txt;
}
