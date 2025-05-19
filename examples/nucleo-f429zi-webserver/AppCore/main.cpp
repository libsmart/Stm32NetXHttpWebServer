/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

/**
 * This file holds the main setup() and loop() functions for C++ code.
 * If a RTOS is used, loop() is called in the main task and setup() is called before RTOS initialization.
 * @see App_ThreadX_Init() in Core/Src/app_threadx.c
 */

#include "main.hpp"
#include "eth.h"
#include "globals.hpp"
#include "RunEvery.hpp"
#include "RunOnce.hpp"
#include "RunThreadEvery.hpp"
#include "RunThreadOnce.hpp"
#include "Stm32NetX.hpp"
#include "Authentication/AuthenticationCheckCallback.hpp"
#include "Authentication/AuthenticationCheckExtendedCallback.hpp"
#include "Command/RegisterCommands.hpp"
#include "Dns/Dns.hpp"
#include "Exception/NetXHttpWebServerException.hpp"
#include "Packet/Packet.hpp"
#include "Secure/X509.hpp"
#include "String/FixedString.hpp"
#include "Webserver/AuthCheckCb.hpp"
#include "Webserver/AuthenticationCheckExtendedCallback.hpp"
#include "Webserver/RequestCallback.hpp"


extern unsigned char server_cert_der[];
extern unsigned char server_cert_key_der[];
extern unsigned int server_cert_der_len;
extern unsigned int server_cert_key_der_len;

/**
 * @brief Setup function.
 * This function is called once at the beginning of the program before ThreadX is initialized.
 * @see main() in Core/Src/main.c
 */
void setup() {
    Stm32ItmLogger::logger.setSeverity(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("::setup()");

    dummyCpp = 0;
    dummyCandCpp = 0;

    ::AppCore::Command::RegisterCommands()();

    Serial3.begin();
    // print welcome message
    Serial3.print(F("startup "));
    Serial3.print(FIRMWARE_NAME);
    Serial3.print(F(" v"));
    Serial3.print(FIRMWARE_VERSION);
    Serial3.print(F(" "));
    Serial3.println(FIRMWARE_COPY);
    Serial3.flush();
    delay(500);
    Serial3.println(F("OK"));
    Serial3.flush();
}

UCHAR buffer[1440];

UINT my_request_notify(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr) {
    UINT status = NX_SUCCESS;
    ULONG offset, length;
    NX_PACKET *response_pkt;
    // Stm32NetX::Packet responsePacket;

    const Stm32Common::String::FixedString<100> res(resource);

    if (request_type == NX_WEB_HTTP_SERVER_GET_REQUEST) {
        char str_ptr1[100];
        char str_ptr2[100];
        UINT str_size = 0;

        Logger.printf("URL = %s\r\n", server_ptr->nx_web_http_server_request_resource);
        Logger.printf("resource = %s\r\n", resource);

        // LIBSMART_ARRAYFILL(str_ptr1);
        // LIBSMART_ARRAYFILL(str_ptr2);
        // webServer.type_get_extended(str_ptr1, sizeof(str_ptr1), str_ptr2, sizeof(str_ptr2), &str_size);
        // Logger.printf("name = %s\r\n", str_ptr1);
        // Logger.printf("type = %s\r\n", str_ptr2);

        // LIBSMART_ARRAYFILL(str_ptr1);
        // LIBSMART_ARRAYFILL(str_ptr2);
        // webServer.param_get(packet_ptr, 0, str_ptr, &str_size, sizeof(str_ptr));
        // Logger.printf("param_ptr = %s\r\n", str_ptr);

        // LIBSMART_ARRAYFILL(str_ptr1);
        // LIBSMART_ARRAYFILL(str_ptr2);
        // webServer.query_get(packet_ptr, 0, str_ptr1, &str_size, sizeof(str_ptr1));
        // Logger.printf("query_ptr = %s\r\n", str_ptr1);

        /*
        if (res == "/") {
            try {
                webServer.callback_generate_response_header(&response_pkt,
                                                            NX_WEB_HTTP_STATUS_OK,
                                                            index_html_len,
                                                            "text/html",
                                                            "Server: NetX WEB HTTP 5.10\r\n");
            } catch (const Stm32NetXHttpWebServer::NetXHttpWebServerException &e) {
                // Stm32ItmLogger::logger.println(e);
                Stm32ItmLogger::logger.println(e.what());
            } catch (...) {
                throw;
            }
            /* Now add data to the packet. #1#
            status = nx_packet_data_append(response_pkt, index_html, index_html_len,
                                           server_ptr->nx_web_http_server_packet_pool_ptr, NX_WAIT_FOREVER);


            if (status == NX_SUCCESS) {
                if (nx_web_http_server_callback_packet_send(server_ptr, response_pkt) !=
                    NX_SUCCESS) {
                    nx_packet_release(response_pkt);
                }
            }
        }
        */






        /*
        if (res == "/img/Logo_mit_URL_Transparent_300.png") {
            status = nx_web_http_server_callback_generate_response_header(server_ptr,
                                                                          &response_pkt, NX_WEB_HTTP_STATUS_OK,
                                                                          img_Logo_mit_URL_Transparent_300_png_len,
                                                                          "text/html",
                                                                          "Server: NetX WEB HTTP 5.10\r\n");

            /* Now add data to the packet. #1#
            status = nx_packet_data_append(response_pkt, img_Logo_mit_URL_Transparent_300_png,
                                           img_Logo_mit_URL_Transparent_300_png_len,
                                           server_ptr->nx_web_http_server_packet_pool_ptr, NX_WAIT_FOREVER);

            if (status == NX_SUCCESS) {
                if (nx_web_http_server_callback_packet_send(server_ptr, response_pkt) != NX_SUCCESS) {
                    nx_packet_release(response_pkt);
                }
            }
        }
        */


        return (NX_SUCCESS);
    }

    /* Process multipart data. */
    if (request_type == NX_WEB_HTTP_SERVER_POST_REQUEST) {
        /* Get the content header. */
        while (nx_web_http_server_get_entity_header(server_ptr, &packet_ptr, buffer,
                                                    sizeof(buffer)) == NX_SUCCESS) {
            /* Header obtained successfully. Get the content data location. */
            while (nx_web_http_server_get_entity_content(server_ptr, &packet_ptr, &offset, &length) == NX_SUCCESS) {
                /* Write content data to buffer. */
                nx_packet_data_extract_offset(packet_ptr, offset, buffer, length, &length);
                buffer[length] = 0;
            }
        }

        /* Generate HTTP header. */
        const auto ret = webServer.callback_generate_response_header(
                                                                      &response_pkt, NX_WEB_HTTP_STATUS_OK, 800,
                                                                      "text/html",
                                                                      "Server: NetX WEB HTTP 5.10\r\n");

        if (ret.isOk()) {
            if (nx_web_http_server_callback_packet_send(server_ptr, response_pkt) != NX_SUCCESS) {
                nx_packet_release(response_pkt);
            }
        }
    } else {
        /* Indicate we have not processed the response to client yet.*/
        return (NX_SUCCESS);
    }

    /* Indicate the response to client is transmitted. */
    return (NX_WEB_HTTP_CALLBACK_COMPLETED);
}


void loopOnce() {
    Stm32ItmLogger::logger.setSeverity(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("::loopOnce()");

#ifdef TX_ENABLE_STACK_CHECKING
    tx_thread_stack_error_notify(Stack_Error_Handler);
#endif

    static char hostname[] = FIRMWARE_NAME"-000000";
    snprintf(hostname, sizeof(hostname), FIRMWARE_NAME"-%02X%02X%02X",
             heth.Init.MACAddr[3], heth.Init.MACAddr[4], heth.Init.MACAddr[5]);
    Stm32NetX::NX->getConfig()->hostname = hostname;
    Stm32NetX::NX->begin();

    while (!Stm32NetX::NX->isIpSet()) {
        delay(500);
    }


    // static Stm32NetXHttpWebServer::ServerCallback webServerCallback(webServer);
    static AppCore::Webserver::RequestCallback webServerCallback(webServer);
    // static AppCore::Webserver::AuthenticationCheckExtendedCallback webServerAuthenticationCheckExtendedCallback(webServer);
    static AppCore::Webserver::AuthCheckCb webServerAuthenticationCheckCallback(webServer);

    static uint8_t webServerStack[1024 * 10]{};
    webServer.create(
        webServer.getNameNonConst(),
        Stm32NetX::NX->getIpInstance(),
        80,
        nullptr,
        &webServerStack,
        sizeof(webServerStack),
        Stm32NetX::NX->getPacketPool(),
        nullptr, //webServerAuthenticationCheckCallback.getBounce(),
        webServerCallback.getBounce()
    );


/*
    static UCHAR buffer[10240]{};
    static NX_SECURE_X509_CERT server_certificate{};

    static Secure::X509 x509(&server_certificate, &Stm32ItmLogger::logger);
    x509.certificateInitialize(
        server_cert_der, server_cert_der_len,
        buffer, sizeof(buffer),
        server_cert_key_der, server_cert_key_der_len,
        NX_SECURE_X509_KEY_TYPE_RSA_PKCS1_DER
        // NX_SECURE_X509_KEY_TYPE_EC_DER
        );

    static NX_SECURE_TLS_CRYPTO nx_crypto_tls_ciphers{};
    static CHAR crypto_metadata[12*1024 * NX_WEB_HTTP_SERVER_SESSION_MAX]{};
    static UCHAR server_tls_packet_buffer[16500 * NX_WEB_HTTP_SERVER_SESSION_MAX]{};


    webServer.secure_configure(&nx_crypto_tls_ciphers,
        crypto_metadata, sizeof(crypto_metadata), server_tls_packet_buffer,
        sizeof(server_tls_packet_buffer), &server_certificate, NX_NULL, 0,
        NX_NULL, 0, NX_NULL, 0);
*/

    // webServer.authenticate_check_set(webServerAuthenticationCheckExtendedCallback.getBounce());

    webServer.start();
}

/**
 * @brief This function is the main loop that executes continuously.
 * The function is called inside the mainLoopThread().
 * @see mainLoopThread() in AZURE_RTOS/App/app_azure_rtos.c
 */
void loop() {
    Serial3.loop();

    static Stm32Common::RunEvery re1(3000);
    re1.loop([]() {
        // telnetServer.broadcast()->printf("counter = %d\r\n", dummyCpp);
    });

    static Stm32Common::RunEvery re2(300);
    re2.loop([]() {
        HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, dummyCpp & 1 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, dummyCpp & 2 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, dummyCpp & 4 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        // Logger.printf("counter = %d\r\n", dummyCpp);
        dummyCpp++;
        dummyCandCpp++;
    });
}


/**
 * @brief This function handles fatal errors.
 * @see Error_Handler() in Core/Src/main.c
 */
[[noreturn]] void errorHandler() {
#if ENABLE_FP==1
    fpSensor.errorHandler();
#endif

    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

    while (true) {
        for (uint32_t i = (SystemCoreClock / 10); i > 0; i--) { UNUSED(i); }
        HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
}


[[noreturn]] void Stack_Error_Handler(TX_THREAD *thread_ptr) {
    Logger.print("==> Stack_Error_Handler() called in thread ");
    Logger.println(thread_ptr->tx_thread_name);
    Logger.print("    Stack size: ");
    Logger.println(thread_ptr->tx_thread_stack_size);
    Error_Handler();
    __disable_irq();
    for (;;) { ; }
}
