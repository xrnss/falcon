#ifndef LOADER_WEBSOCKET_HPP
#define LOADER_WEBSOCKET_HPP

#include <iostream>
#include <string>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include "binary/falconreader.h"

using Client = websocketpp::client<websocketpp::config::asio_client>;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

void on_message(Client *c, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::vector<uint8_t> payloadData(msg->get_payload().begin(), msg->get_payload().end());
    FalconReader reader(payload, 0, false);

    int opCode = reader.getUint8();
    switch (opCode) {
        case 1: {
            // Handshake [B]
        }
    }
}

void connect(int port) {
    Client client;

    client.init_asio();
    client.set_access_channels(websocketpp::log::alevel::all);
    client.clear_access_channels(websocketpp::log::alevel::frame_payload);
    client.clear_access_channels(websocketpp::log::alevel::frame_header);

    client.set_message_handler(std::bind(&on_message, &client, std::placeholders::_1, std::placeholders::_2));

    websocketpp::lib::error_code ec;
    Client::connection_ptr con = client.get_connection(std::string("ws://localhost:") + std::to_string(port), ec);
    if (ec) {
        std::cout << "could not create connection because: " << ec.message() << std::endl;
        return;
    }
    client.connect(con);

    client.run();
}

#endif