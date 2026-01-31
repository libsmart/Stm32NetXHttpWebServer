/*
 * SPDX-FileCopyrightText: 2026 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "BaseServer.hpp"
#include "Packet/Packet.hpp"

namespace Stm32NetXHttpWebServer {
    class Server : public BaseServer {
    public:
        static constexpr const char *COMPONENT_NAME = Stm32NetXHttpWebServer::COMPONENT_NAME;
        static constexpr char CLASS_NAME[] = "Server";
        const char *INSTANCE_NAME{getName()};

        using BaseServer::BaseServer;

        using contentLengthGetResult = PrintableResult<ULONG, Common::NetXHttpReturn::NetXHttpResult>;

        contentLengthGetResult contentLengthGet(Packet &packet);

        contentLengthGetResult contentGet(Packet &packet, ULONG byteOffset, uint8_t *buffer, size_t sz);
    };
}
