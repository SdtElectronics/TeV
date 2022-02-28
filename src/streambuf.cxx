// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef TEVHDRONLY
#include "internal/streambuf.h"
#endif

namespace tev{
namespace internal{

std::string streambuf::getMessage(std::size_t length){
    std::istream stream(this);
    std::string result(length, ' ');
    stream.read(&result[0], length);
    return result;
}

std::string streambuf::getMessage(){
    std::istream stream(this);
    std::string ret(std::istreambuf_iterator<char>(stream), {});
    return ret;
}

}
}