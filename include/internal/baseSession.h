// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevsessionbase
#define tevsessionbase

#include <memory>
#include <utility>

#include "asio.hpp"
#include "bufReader.h"
#include "session.h"
#include "sharedBuf.h"
#include "streambuf.h"
#include "tevdefs.h"
#include "timer.h"

namespace tev{
namespace internal{

template <typename T, typename S>
class BaseSession: public std::enable_shared_from_this<BaseSession<T, S> >{
  public:
    BaseSession(S stream, std::shared_ptr<Session> dialogue, std::size_t maxLength);

    // Read fixed length
    template <typename CB>
    void asyncRead(std::size_t length, CB&& callback);
    // Read until token character
    template <typename CB>
    void asyncRead(char token, CB&& callback);
    // Read until token string
    template <typename CB>
    void asyncRead(const char* token, CB&& callback);
    // Read until completionCondition returns 0
    template <typename CF, typename CB>
    void asyncRead(CF&& completionCondition, CB&& callback);

    template <typename CB>
    void asyncWrite(const std::string& message, CB&& callback);
    template <typename CB>
    void asyncWrite(SharedBuffer bufs, CB&& callback);

    inline std::string getMessage(std::size_t length);
    inline std::string getMessage();

  protected:
    friend BufReader::BufReader(asio::streambuf& buf);

    S stream_;
    std::shared_ptr<Session> dialogue_;

    void handleReadError(const std::error_code& ec);
    streambuf message_;
};

template <typename T, typename S>
BaseSession<T, S>::BaseSession(S stream, std::shared_ptr<Session> dialogue, std::size_t maxLength):
    stream_(std::move(stream)), dialogue_(dialogue), message_(maxLength){
}

template <typename T, typename S>
template <typename CB>
void BaseSession<T, S>::asyncRead(std::size_t length, CB&& callback){
    auto self(this->shared_from_this());
    asio::async_read(stream_, static_cast<asio::streambuf&>(message_), asio::transfer_exactly(length),
    [self, TEVCAPTMOVE(callback)](const std::error_code& ec, std::size_t rxLength) mutable{
        if(!ec){
            callback(*std::static_pointer_cast<T>(self));
        }else{
            self->handleReadError(ec);
        }
    });
}

template <typename T, typename S>
template <typename CB>
void BaseSession<T, S>::asyncRead(const char* token, CB&& callback){
    auto self(this->shared_from_this());
    asio::async_read_until(stream_, static_cast<asio::streambuf&>(message_), token,
    [self, TEVCAPTMOVE(callback)](const std::error_code ec, std::size_t rxLength) mutable{
        if(!ec){
            callback(*std::static_pointer_cast<T>(self));
        }else{
            self->handleReadError(ec);
        }
    });
}

template <typename T, typename S>
template <typename CB>
void BaseSession<T, S>::asyncRead(char token, CB&& callback){
    auto self(this->shared_from_this());
    asio::async_read_until(stream_, static_cast<asio::streambuf&>(message_), token,
    [self, TEVCAPTMOVE(callback)](const std::error_code ec, std::size_t rxLength) mutable{
        if(!ec){
            callback(*std::static_pointer_cast<T>(self));
        }else{
            self->handleReadError(ec);
        }
    });
}

template <typename T, typename S>
template <typename CF, typename CB>
void BaseSession<T, S>::asyncRead(CF&& completionCondition, CB&& callback){
    auto self(this->shared_from_this());
    asio::async_read(stream_, static_cast<asio::streambuf&>(message_), 
    [this, TEVCAPTMOVE(completionCondition)](const std::error_code& error, std::size_t bytes){
        return completionCondition(error, bytes, BufReader(static_cast<asio::streambuf&>(message_)));
    },
    [self, TEVCAPTMOVE(callback)](const std::error_code ec, std::size_t rxLength) mutable{
        if(!ec){
            callback(*std::static_pointer_cast<T>(self));
        }else{
            self->handleReadError(ec);
        }
    });
}

template <typename T, typename S>
template <typename CB>
void BaseSession<T, S>::asyncWrite(const std::string& message, CB&& callback){
    auto self(this->shared_from_this());
    asio::async_write(stream_, asio::const_buffer(message.c_str(), message.length()),
    [self, TEVCAPTMOVE(callback)](const std::error_code& ec, std::size_t) mutable{
        if(!ec){
            callback(*std::static_pointer_cast<T>(self));
        }else{
            self->dialogue_->onWriteError(*std::static_pointer_cast<T>(self), ec.message());
        }
    });
}

template <typename T, typename S>
template <typename CB>
void BaseSession<T, S>::asyncWrite(SharedBuffer bufs, CB&& callback){
    auto self(this->shared_from_this());
    asio::async_write(stream_, bufs,
    [self, TEVCAPTMOVE(callback)](const std::error_code& ec, std::size_t) mutable{
        if(!ec){
            callback(*std::static_pointer_cast<T>(self));
        }else{
            self->dialogue_->onWriteError(*std::static_pointer_cast<T>(self), ec.message());
        }
    });
}

template <typename T, typename S>
std::string BaseSession<T, S>::getMessage(std::size_t length){
    return message_.getMessage(length);
}

template <typename T, typename S>
std::string BaseSession<T, S>::getMessage(){
    return message_.getMessage();
}

template <typename T, typename S>
void BaseSession<T, S>::handleReadError(const std::error_code& ec){
    if(ec == asio::error::eof || ec == asio::error::connection_reset){
        dialogue_->onDisconnect(*static_cast<T*>(this));
    }else{
        dialogue_->onReadError(*static_cast<T*>(this), ec.message());
    }
}

} // namespace internal
} // namespace tev

#endif