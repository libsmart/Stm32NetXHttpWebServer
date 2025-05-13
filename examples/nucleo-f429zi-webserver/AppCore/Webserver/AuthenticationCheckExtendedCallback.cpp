/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "AuthenticationCheckExtendedCallback.hpp"

using namespace Stm32NetXHttpWebServer;

AuthenticationCheckExtendedCallback::CallbackResult
AppCore::Webserver::AuthenticationCheckExtendedCallback::authenticationCallback(
    ServerHttpMethod requestType, BaseServer::Resource &request,
    BaseServer::Name &name, BaseServer::Password &password,
    BaseServer::Realm &realm) {

    realm.print("Hello World");
    name.print("admin");
    password.print("123456");

    return CallbackResult::ok(CallbackValue::Id::BASIC_AUTHENTICATE);
}
