
#ifndef tevtcpbase
#define tevtcpbase

#include <memory>
#include <utility>

#include "asio/asio.hpp"
#include "bufReader.h"
#include "internal/tevdefs.h"
#include "session.h"

namespace tev{

using asio::ip::tcp;

class TCPsession: public std::enable_shared_from_this<TCPsession>{
  public:
    TEVINLINE 
    TCPsession(tcp::socket socket, std::shared_ptr<Session> dialogue, std::size_t maxLength);

    template<typename CB>
    // Read fixed length
    void asyncRead(std::size_t length, CB callback);
    template<typename CB>
    // Read until token
    void asyncRead(char token, CB callback);
    template<typename CB>
    // Read until token
    void asyncRead(const char* token, CB callback);
    template<typename CF, typename CB>
    // Read until completionCondition returns 0
    void asyncRead(CF completionCondition, CB callback);

    template<typename CB>
    void asyncWrite(const std::string& message, CB callback);

    template<typename Rep, typename Period>
    using duration = std::chrono::duration<Rep, Period>;
    using Timer = asio::steady_timer;
    template<typename Rep, typename Period, typename CB>
    Timer defer(const duration<Rep, Period>& timeout, CB callback);
    template<typename Rep, typename Period>
    Timer expire(const duration<Rep, Period>& timeout);
    template<typename Rep, typename Period, typename CB>
    Timer expire(const duration<Rep, Period>& timeout, CB callback);

    TEVINLINE void close();

    TEVINLINE std::string getMessage(std::size_t length);
    TEVINLINE std::string getMessage();

    TEVINLINE std::string getRemoteIP() const;
    TEVINLINE unsigned short getRemotePort() const;

  protected:
    friend BufReader::BufReader(asio::streambuf& buf);

    tcp::socket socket_;
    std::shared_ptr<Session> dialogue_;

  private:
    TEVINLINE void handleReadError(const std::error_code& ec);
    asio::streambuf message_;
};

template<typename CB>
void TCPsession::asyncRead(std::size_t length, CB callback){
    auto self(this->shared_from_this());
    asio::async_read(socket_, message_, asio::transfer_exactly(length),
    [this, self, length, callback](const std::error_code& ec, std::size_t rxLength) mutable{
        if(!ec){
            callback(*this, BufReader(message_)(rxLength));
        }else{
            handleReadError(ec);
        }
    });
}

template<typename CB>
void TCPsession::asyncRead(const char* token, CB callback){
    auto self(this->shared_from_this());
    asio::async_read_until(socket_, message_, token,
    [this, self, token, callback](const std::error_code ec, std::size_t rxLength) mutable{
        if(!ec){
            callback(*this, BufReader(message_)(rxLength));
        }else{
            handleReadError(ec);
        }
    });
}

template<typename CB>
void TCPsession::asyncRead(char token, CB callback){
    auto self(this->shared_from_this());
    asio::async_read_until(socket_, message_, token,
    [this, self, token, callback](const std::error_code ec, std::size_t rxLength) mutable{
        if(!ec){
            callback(*this, BufReader(message_)(rxLength));
        }else{
            handleReadError(ec);
        }
    });
}

template<typename CF, typename CB>
void TCPsession::asyncRead(CF completionCondition, CB callback){
    auto self(this->shared_from_this());
    asio::async_read(socket_, message_, 
    [this, completionCondition](const std::error_code& error, std::size_t bytes){
        return completionCondition(error, bytes, BufReader(message_));
    },
    [this, self, callback](const std::error_code ec, std::size_t rxLength) mutable{
        if(!ec){
            callback(*this, BufReader(message_)(rxLength));
        }else{
            handleReadError(ec);
        }
    });
}


template<typename CB>
void TCPsession::asyncWrite(const std::string& message, CB callback){
    auto self(this->shared_from_this());
    asio::async_write(socket_, asio::const_buffer(message.c_str(), message.length()),
    [this, self, callback](const std::error_code& ec, std::size_t) mutable{
        if(!ec){
            callback(*this);
        }else{
            dialogue_->onWriteError(*this, ec.message());
        }
    });
}

template<typename Rep, typename Period, typename CB>
TCPsession::Timer TCPsession::defer(
    const TCPsession::duration<Rep, Period>& timeout, CB callback){
    auto& ctx = static_cast<asio::io_context&>(socket_.get_executor().context());
    Timer timer(ctx, std::chrono::steady_clock::now() + timeout);
    timer.async_wait([callback](const std::error_code& ec){
        if(ec != asio::error::operation_aborted){
            callback(ec);
        }
    });
    return timer;
}

template<typename Rep, typename Period>
TCPsession::Timer TCPsession::expire(const TCPsession::duration<Rep, Period>& timeout){
    return defer(timeout, [this](const std::error_code& ec){
        close();
    });
}

template<typename Rep, typename Period, typename CB>
TCPsession::Timer TCPsession::expire(
    const TCPsession::duration<Rep, Period>& timeout, CB callback){
    return std::move(defer(timeout, [this, callback](const std::error_code& ec){
        callback(ec);
        close();
    }));
}

}

#ifdef TEVHDRONLY
#include "src/tcpResponse.cxx"
#endif

#endif