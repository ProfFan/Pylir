# Copyright 2022 Markus Böck
#
# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(PylirTablegen)

set(LLVM_TARGET_DEFINITIONS CodeGenIntr.td)
pylir_tablegen(CodeGenIntr.cpp.inc -gen-intrinsics)
add_public_tablegen_target(CodeGenIntrIncGen)

add_library(CodeGen CodeGen.cpp CompilerBuiltins.cpp)
target_link_libraries(CodeGen PUBLIC PylirPyDialect Parser PRIVATE PylirTransformsUtils MLIRFuncDialect)
add_dependencies(CodeGen CodeGenIntrIncGen)
