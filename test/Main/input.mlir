// RUN: pylir %s -o - -S -emit-llvm | FileCheck %s
// RUN: pylir-opt %s -o %t.mlirbc -emit-bytecode
// RUN: pylir %t.mlirbc -o - -S -emit-llvm | FileCheck %s

py.globalValue const @const$ = #py.tuple<(#py.str<"__slots__">)>
py.globalValue @builtins.type = #py.type<slots = {__slots__ = #py.ref<@const$>}>
py.globalValue @builtins.tuple = #py.type
py.globalValue @builtins.str = #py.type
py.globalValue @builtins.function = #py.type
py.globalValue @builtins.dict = #py.type

func.func @foo(%arg0 : !py.dynamic) -> !py.dynamic {
    %0 = py.typeOf %arg0
    %1 = py.typeOf %0
    %2 = py.getSlot "__slots__" from %0 : %1
    return %2 : !py.dynamic
}

// CHECK-LABEL: define ptr addrspace({{[0-9]+}}) @foo(ptr addrspace({{[0-9]+}}) %{{.*}})
