
//  Licensed under the Apache License v2.0 with LLVM Exceptions.
//  See https://llvm.org/LICENSE.txt for license information.
//  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <mlir/Pass/Pass.h>

#include <memory>

namespace pylir
{
#define GEN_PASS_REGISTRATION
#define GEN_PASS_DECL_LOADFORWARDINGPASS
#define GEN_PASS_DECL_SROAPASS
#define GEN_PASS_DECL_FIXPOINTPASS
#include "pylir/Optimizer/Transforms/Passes.h.inc"

} // namespace pylir
