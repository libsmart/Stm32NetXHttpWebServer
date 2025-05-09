/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

extern "C" {
#include "nx_web_http_server.h"
}

namespace Stm32NetXHttpWebServer {
    enum class ServerHttpMethod : UINT {
        UNKNOWN = NX_WEB_HTTP_SERVER_UNKNOWN_REQUEST,
        GET = NX_WEB_HTTP_SERVER_GET_REQUEST,
        POST = NX_WEB_HTTP_SERVER_POST_REQUEST,
        HEAD = NX_WEB_HTTP_SERVER_HEAD_REQUEST,
        PUT = NX_WEB_HTTP_SERVER_PUT_REQUEST,
        DELETE = NX_WEB_HTTP_SERVER_DELETE_REQUEST
    };
}
