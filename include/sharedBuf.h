// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevsharedbuf
#define tevsharedbuf

#include <memory>
#include <string>
#include <vector>

#include "asio.hpp"

namespace tev{

class SharedBuffer{
  public:
    inline SharedBuffer(){}
    
    inline SharedBuffer(const std::string& data){
        data_.push_back(data);
    }
    
    inline SharedBuffer(std::string&& data){
        data_.emplace_back(data);
    }

    // copy and append content to the buffer without incurring massive relocation
    // this operation can be chained, as the reference of buffer itself is returned
    inline SharedBuffer& append(const std::string& data){
        data_.push_back(data);
        return *this;
    }

    // move and append content to the buffer without incurring massive relocation
    // this operation can be chained, as the reference of buffer itself is returned
    inline SharedBuffer& append(std::string&& data){
        data_.emplace_back(data);
        return *this;
    }

    typedef asio::const_buffer value_type;

    struct const_iterator: public std::vector<std::string>::const_iterator{
        using Iter = std::vector<std::string>::const_iterator;
        const_iterator(const Iter& iter): Iter(iter){}

        // dereference operation is expected to return a reference
        // but this actually works, the fuck.
        const asio::const_buffer operator* (){
            const std::string& str = Iter::operator*();
            return asio::const_buffer(str.c_str(), str.length());
        }
    };

    inline const_iterator begin() const{
        return const_iterator(data_.begin());
    }

    inline const_iterator end() const{
        return const_iterator(data_.end());
    }

  private:
    std::vector<std::string> data_;
};

inline SharedBuffer& operator<<(SharedBuffer& buf, const std::string& data){
    return buf.append(data);
}

inline SharedBuffer& operator<<(SharedBuffer& buf, const char* data){
    return buf.append(std::string(data));
}

}

#endif