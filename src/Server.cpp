/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Server.hpp"
#include "Exception/NetXException.hpp"

using namespace Stm32NetXHttpWebServer;

Server::contentLengthGetResult Server::contentLengthGet(Packet &packet) {
    ULONG contentLength{};
    const auto ret = content_length_get(packet.getNxPacket(), &contentLength);
    return ret.isError() ? contentLengthGetResult::err(ret.error()) : contentLengthGetResult::ok(contentLength);
}
