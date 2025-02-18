# Copyright 2022 Markus Böck
#
# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(LLVM_TARGET_DEFINITIONS PylirMemOps.td)
mlir_tablegen(PylirMemOps.h.inc -gen-op-decls)
mlir_tablegen(PylirMemOps.cpp.inc -gen-op-defs)
mlir_tablegen(PylirMemOpsTypes.h.inc -gen-typedef-decls --typedefs-dialect="pyMem")
mlir_tablegen(PylirMemOpsTypes.cpp.inc -gen-typedef-defs --typedefs-dialect="pyMem")
mlir_tablegen(PylirMemOpsAttributes.h.inc -gen-attrdef-decls --attrdefs-dialect="pyMem")
mlir_tablegen(PylirMemOpsAttributes.cpp.inc -gen-attrdef-defs --attrdefs-dialect="pyMem")
mlir_tablegen(PylirMemOpsDialect.h.inc -gen-dialect-decls -dialect="pyMem")
mlir_tablegen(PylirMemOpsDialect.cpp.inc -gen-dialect-defs -dialect="pyMem")
add_public_tablegen_target(PylirMemOpsIncGen)
add_dependencies(mlir-headers PylirMemOpsIncGen)

add_mlir_dialect_library(PylirMemDialect
        PylirMemDialect.cpp
        PylirMemOps.cpp

        DEPENDS
        PylirMemOpsIncGen

        LINK_LIBS PUBLIC MLIRIR PylirSupport MLIRInferTypeOpInterface
        )
if (PYLIR_USE_PCH)
    target_link_libraries(PylirMemDialect PRIVATE COMMON_PCH)
    target_precompile_headers(PylirMemDialect PRIVATE <mlir/IR/Operation.h> <mlir/IR/BuiltinOps.h>)
endif ()
