// Copyright 2022 Markus Böck
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <array>
#include <llvm/ADT/StringRef.h>

namespace pylir::Builtins
{

struct Builtin
{
    llvm::StringLiteral name;
    bool isPublic;
};

#define COMPILER_BUILTIN(cppName, intrName) constexpr Builtin Pylir##cppName = {#intrName, false};
#include "CompilerBuiltins.def"

#define BUILTIN(x, s, isPublic, ...) constexpr Builtin x = {s, isPublic};
#include "BuiltinsModule.def"

constexpr std::array allBuiltins = {
#define BUILTIN(x, ...) x,
#include "BuiltinsModule.def"

#define COMPILER_BUILTIN(cppName, ...) Pylir##cppName,
#include "CompilerBuiltins.def"
};
} // namespace pylir::Py::Builtins
