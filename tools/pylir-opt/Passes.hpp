//  Licensed under the Apache License v2.0 with LLVM Exceptions.
//  See https://llvm.org/LICENSE.txt for license information.
//  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <mlir/Pass/Pass.h>

#include <memory>

namespace pylir::test
{
#define GEN_PASS_REGISTRATION
#define GEN_PASS_DECL_TESTALIASSETTRACKERPASS
#define GEN_PASS_DECL_TESTHELLOWORLDPASS
#define GEN_PASS_DECL_TESTINLINEALLPASS
#define GEN_PASS_DECL_TESTINLINERINTERFACEPASS
#define GEN_PASS_DECL_TESTLINKERPASS
#define GEN_PASS_DECL_TESTLOOPINFOPASS
#define GEN_PASS_DECL_TESTMEMORYSSAPASS
#define GEN_PASS_DECL_TESTTYPEFLOWPASS
#define GEN_PASS_DECL_TESTADDCHANGEPASS
#include "Passes.h.inc"
} // namespace pylir::test
