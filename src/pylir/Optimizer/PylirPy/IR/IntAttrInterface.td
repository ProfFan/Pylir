// Copyright 2022 Markus Böck
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef PYLIRPY_INTERFACES_INT_ATTR_INTERFACE
#define PYLIRPY_INTERFACES_INT_ATTR_INTERFACE

include "mlir/IR/OpBase.td"

def IntAttrInterface : AttrInterface<"IntAttrInterface"> {
    let cppNamespace = "::pylir::Py";

    let methods = [
        InterfaceMethod<"", "const ::pylir::BigInt&", "getIntegerValue", (ins)>
    ];

    let returnType = "::pylir::BigInt";
    let convertFromStorage = "$_self.getIntegerValue()";
}

#endif
