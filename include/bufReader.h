// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevbufreader
#define tevbufreader

#include <string>

#include "asio.hpp"

namespace tev{

class BufReader{
  public:
    inline BufReader(asio::streambuf& buf);
    inline std::string peek(int len);
    inline std::string operator () (std::size_t len);
    inline std::string operator () ();

  private:
    asio::streambuf& buf_;
};

BufReader::BufReader(asio::streambuf& buf): buf_(buf){
};

std::string BufReader::peek(int len){
    asio::streambuf::const_buffers_type bufs = buf_.data();
    if(len > 0){
        std::string ret(asio::buffers_begin(bufs),
                asio::buffers_begin(bufs) + len);
        return ret;
    }else if(len < 0){
        std::string ret(asio::buffers_end(bufs) + len,
                asio::buffers_end(bufs));
        return ret;
    }else{
        return std::string{};
    }
}

std::string BufReader::operator () (std::size_t len){
    asio::streambuf::const_buffers_type bufs = buf_.data();
    std::string ret(asio::buffers_begin(bufs),
            asio::buffers_begin(bufs) + len);
    buf_.consume(len);
    return ret;
}

std::string BufReader::operator () (){
    return (*this)(buf_.size());
}

}

#endif