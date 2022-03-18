// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics 
// All rights reserved. Use of this source code is governed by 
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevtcpservice
#define tevtcpservice

#include <cstdlib>
#include <memory>
#include <utility>

#include "asio/asio.hpp"
#include "internal/tcpResponse.h"
#include "session.h"
#include "worker.h"

namespace tev{

enum IPver{V4ORV6, V4ONLY, V6ONLY};

template<IPver acceptVer = V4ORV6>
class Service{};

template<>
class Service<V4ORV6>{
  public:
    // port: port this service to listen
    Service(short port):
        acceptorV4_(Worker::getContext(), tcp::endpoint(tcp::v4(), port)),
        acceptorV6_(Worker::getContext(), tcp::endpoint(tcp::v6(), port)){
            std::error_code ec;
            // Some system don't support this flag
            // Use error_code to prevent throwing an exception
            acceptorV6_.set_option(asio::ip::v6_only(true), ec);
    }

    // addrV4: IPv4 address to listen
    // addrV6: IPv6 address to listen
    // port: port this service to listen
    Service(const char* addrV4, const char* addrV6, short port):
        acceptorV4_(Worker::getContext(), tcp::endpoint(asio::ip::address::from_string(addrV4), port)),
        acceptorV6_(Worker::getContext(), tcp::endpoint(asio::ip::address::from_string(addrV6), port)){
            std::error_code ec;
            acceptorV6_.set_option(asio::ip::v6_only(true), ec);
    }

    template<typename DF>
    // dialogueFactory: A callable returning a std::shared_ptr to session object
    void start(DF dialogueFactory, std::size_t maxLength = 65535){
        auto handler = [this, dialogueFactory, maxLength](
            const std::error_code& ec, tcp::socket socket){
            auto dialogue(dialogueFactory());

            if(!ec){
                // This shared pointer is held by the completion handler created in start()
                // The life of this session is thereby extended to the execution of the handler
                std::make_shared<internal::TCPresponse>(
                    std::move(socket), dialogue, maxLength
                )->start();
            }else{
                internal::TCPresponse failed(std::move(socket), dialogue, maxLength);
                dialogue->onConnectError(failed, ec.message());
            }

            start(dialogueFactory, maxLength);
        };

        acceptorV4_.async_accept(handler);
        acceptorV6_.async_accept(handler);
    }

    void stop(){
        acceptorV4_.close();
        acceptorV6_.close();
    }
        
  private:
    tcp::acceptor acceptorV4_;
    tcp::acceptor acceptorV6_;
};

template<>
class Service<V4ONLY>{
  public:
    // port: port this service to listen
    Service(short port):
        acceptor_(Worker::getContext(), tcp::endpoint(tcp::v4(), port)){
    }

    // addrV4: IPv4 address to listen
    // port: port this service to listen
    Service(const char* addrV4, short port):
        acceptor_(Worker::getContext(), tcp::endpoint(asio::ip::address::from_string(addrV4), port)){
    }

    template<typename DF>
    // dialogueFactory: A callable returning a std::shared_ptr to session object
    void start(DF dialogueFactory, std::size_t maxLength = 65535){
        acceptor_.async_accept(
        [this, dialogueFactory, maxLength](const std::error_code& ec, tcp::socket socket){
            auto dialogue(dialogueFactory());

            if(!ec){
                std::make_shared<internal::TCPresponse>(
                    std::move(socket), dialogue, maxLength
                )->start();
            }else{
                internal::TCPresponse failed(std::move(socket), dialogue, maxLength);
                dialogue->onConnectError(failed, ec.message());
            }

            start(dialogueFactory, maxLength);
        });
    }

    void stop(){
        acceptor_.close();
    }
        
  private:
    tcp::acceptor acceptor_;
};

template<>
class Service<V6ONLY>{
  public:
    // port: port this service to listen
    Service(short port):
        acceptor_(Worker::getContext(), tcp::endpoint(tcp::v6(), port)){
        std::error_code ec;
        acceptor_.set_option(asio::ip::v6_only(true), ec);
    }

    // addrV6: IPv6 address to listen
    // port: port this service to listen
    Service(const char* addrV4, const char* addrV6, short port):
        acceptor_(Worker::getContext(), tcp::endpoint(asio::ip::address::from_string(addrV6), port)){
        std::error_code ec;
        acceptor_.set_option(asio::ip::v6_only(true), ec);
    }

    template<typename DF>
    // dialogueFactory: A callable returning a std::shared_ptr to session object
    void start(DF dialogueFactory, std::size_t maxLength = 65535){
        acceptor_.async_accept(
        [this, dialogueFactory, maxLength](const std::error_code& ec, tcp::socket socket){
            auto dialogue(dialogueFactory());

            if(!ec){
                asio::ip::v6_only option(true);
                socket.set_option(option);
                std::make_shared<internal::TCPresponse>(
                    std::move(socket), dialogue, maxLength
                )->start();
            }else{
                internal::TCPresponse failed(std::move(socket), dialogue, maxLength);
                dialogue->onConnectError(failed, ec.message());
            }

            start(dialogueFactory, maxLength);
        });
    }

    void stop(){
        acceptor_.close();
    }
        
  private:
    tcp::acceptor acceptor_;
};

}

#endif