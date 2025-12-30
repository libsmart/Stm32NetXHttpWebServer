/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "BaseServer.hpp"
#include "Packet/Packet.hpp"

using namespace Stm32NetX;

namespace Stm32NetXHttpWebServer {
    class Server : public BaseServer {
    public:
        using BaseServer::BaseServer;

        using contentLengthGetResult = PrintableResult<ULONG, Common::NetXHttpReturn::NetXHttpResult>;

        contentLengthGetResult contentLengthGet(Packet &packet);

        contentLengthGetResult contentGet(Packet &packet, ULONG byteOffset, uint8_t *buffer, size_t sz);
    };
}
