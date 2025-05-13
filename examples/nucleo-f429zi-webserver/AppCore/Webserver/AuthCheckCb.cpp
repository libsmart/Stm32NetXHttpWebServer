/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "AuthCheckCb.hpp"

using namespace AppCore::Webserver;
using namespace Stm32NetXHttpWebServer;

AuthenticationCheckCallback::CallbackResult AuthCheckCb::authenticationCallback(
    ServerHttpMethod requestType,
    BaseServer::Resource &request, BaseServer::Name &name,
    BaseServer::Password &password, BaseServer::Realm &realm) {


    name.print("admin");
    password.print("123456");
    realm.print("Hello World");

    return CallbackResult::ok(CallbackValue::Id::BASIC_AUTHENTICATE);
}
