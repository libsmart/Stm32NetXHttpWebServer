/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Stm32NetXHttpWebServer.hpp"
#include "Exception/NetXException.hpp"

#if __EXCEPTIONS

namespace Stm32NetXHttpWebServer {
    class NetXHttpWebServerException : public Stm32NetX::NetXException {
    public:
        NetXHttpWebServerException(const char *CLASS_NAME, const char *message)
            : NetXException(Stm32NetXHttpWebServer::COMPONENT_NAME, CLASS_NAME, message) { ; }

        explicit NetXHttpWebServerException(const nxHttpResult_t &result)
            : NetXException(result.COMPONENT_NAME, result.CLASS_NAME, result.INSTANCE_NAME, result.FUNCTION_NAME,
                            result.isError() ? result.error().message() : ""), nxHttpRes(result) { ; }

        explicit NetXHttpWebServerException(const nxResult_t &result) : NetXException(result) { ; }

        auto res() const { return nxHttpRes; }

        size_t printTo(Stm32Common::PrintInterface &printObject) const override {
            if (nxHttpRes.isError() && (&nxHttpRes.error().raw() == &Stm32NetX::Common::NetXHttpReturn::data[0])) {
                return NetXException::printTo(printObject);
            }
            return printObject.print(nxHttpRes);
        }

    private:
        nxHttpResult_t nxHttpRes = nxHttpResult_t::err(Stm32NetX::Common::NetXHttpReturn::NetXHttpResult{});
    };
}

#endif
