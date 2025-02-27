// RUN: pylir-opt %s -convert-pylir-to-llvm --split-input-file | FileCheck %s

py.globalValue @builtins.type = #py.type // stub
py.globalValue @builtins.object = #py.type // stub
py.globalValue @builtins.function = #py.type // stub
py.globalValue @builtins.str = #py.type // stub
py.globalValue @builtins.None = #py.type // stub
py.globalValue @builtins.tuple = #py.type

py.globalValue @foo = #py.function<@bar>

func.func @bar(%arg0 : !py.dynamic, %arg1 : !py.dynamic, %arg2 : !py.dynamic) -> !py.dynamic {
    return %arg0 : !py.dynamic
}

// CHECK-LABEL: @foo
// CHECK-NEXT: %[[UNDEF:.*]] = llvm.mlir.undef
// CHECK-NEXT: %[[TYPE:.*]] = llvm.mlir.addressof @builtins.function
// CHECK-NEXT: %[[UNDEF1:.*]] = llvm.insertvalue %[[TYPE]], %[[UNDEF]][0]
// CHECK-NEXT: %[[ADDRESS:.*]] = llvm.mlir.addressof @bar
// CHECK-NEXT: %[[UNDEF2:.*]] = llvm.insertvalue %[[ADDRESS]], %[[UNDEF1]][1]
// CHECK-NEXT: llvm.return %[[UNDEF2]]
