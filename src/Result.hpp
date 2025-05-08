/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include "NetXHttpReturnValues.hpp"
#include "Result.hpp"
#include "Exception/NetXRuntimeException.hpp"
#include "Result/Result.hpp"

namespace Stm32NetXHttpWebServer {
    // using Result_t = Stm32Common::Result<void, ReturnValues>;
    using genericResult_t = Stm32Common::Result<void, UINT>;

    using nxResult_t = Stm32Common::Result<void, Stm32NetX::NetXReturnValues::NetXReturnValue>;
    using nxHttpResult_t = Stm32Common::Result<void, Stm32NetX::NetXHttpReturnValues::NetXHttpReturnValue>;

}
