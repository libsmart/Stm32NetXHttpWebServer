/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "AuthenticationCheckCallback.hpp"

using namespace Stm32NetXHttpWebServer;

UINT AuthenticationCheckCallback::callback(NX_WEB_HTTP_SERVER *server_ptr,
                                           UINT request_type, CHAR *resource, CHAR **name,
                                           CHAR **password, CHAR **realm) {
    server.log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("%s::%s[%s]::callback()\r\n", COMPONENT_NAME, CLASS_NAME, server.getName());


    BaseServer::Resource resourceString(resource);
    BaseServer::Name nameString{};
    LIBSMART_ARRAYFILL(nameBuffer);
    BaseServer::Password passwordString{};
    LIBSMART_ARRAYFILL(passwordBuffer);
    BaseServer::Realm realmString{};
    LIBSMART_ARRAYFILL(realmBuffer);

    try {
        const auto ret = authenticationCallback(static_cast<ServerHttpMethod>(request_type),
                                                resourceString, nameString,
                                                passwordString, realmString);

        if (ret.isOk()) {
            strncpy(nameBuffer, nameString.c_str(), sizeof(nameBuffer));
            *name = nameBuffer;

            strncpy(passwordBuffer, passwordString.c_str(), sizeof(passwordBuffer));
            *password = passwordBuffer;

            strncpy(realmBuffer, realmString.c_str(), sizeof(realmBuffer));
            *realm = realmBuffer;

            return static_cast<UINT>(ret.value());
        }
    } catch (...) { ; }
    return NX_WEB_HTTP_ERROR;
}
