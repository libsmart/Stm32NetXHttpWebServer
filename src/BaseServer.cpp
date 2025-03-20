/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "BaseServer.hpp"
#include <climits>
#include <stdexcept>
#include "Address/Address.hpp"
#include "Address/AddressPrinter.hpp"

using namespace Stm32NetX;
using namespace Stm32NetXHttpWebServer;

bool BaseServer::isCreated() {
    return flags.isSet(IS_CREATED);
}
