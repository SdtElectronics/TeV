// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevstreambuf
#define tevstreambuf

#include <string>

#include "asio.hpp"
#include "tevdefs.h"

namespace tev{
namespace internal{

class streambuf: public asio::streambuf{
  public:
    using asio::streambuf::streambuf;

    TEVINLINE std::string getMessage(std::size_t length);
    TEVINLINE std::string getMessage();
};

}
}

#ifdef TEVHDRONLY
#include "src/streambuf.cxx"
#endif

#endif