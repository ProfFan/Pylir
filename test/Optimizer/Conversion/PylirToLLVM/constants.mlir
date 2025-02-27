// RUN: pylir-opt %s -convert-pylir-to-llvm --split-input-file | FileCheck %s

py.globalValue @builtins.tuple = #py.tuple<()>

func.func @constants() -> !py.dynamic {
    %0 = py.constant(#py.tuple<()>)
    return %0 : !py.dynamic
}

// CHECK: llvm.mlir.global private unnamed_addr constant @[[CONSTANT:const\$[[:alnum:]]*]]
// CHECK-NEXT: %[[UNDEF:.*]] = llvm.mlir.undef
// CHECK-NEXT: %[[TYPE:.*]] = llvm.mlir.addressof @builtins.tuple
// CHECK-NEXT: %[[UNDEF1:.*]] = llvm.insertvalue %[[TYPE]], %[[UNDEF]][0]
// CHECK-NEXT: %[[SIZE:.*]] = llvm.mlir.constant(0 : i{{.*}})
// CHECK-NEXT: %[[UNDEF2:.*]] = llvm.insertvalue %[[SIZE]], %[[UNDEF1]][1]
// CHECK-NEXT: llvm.return %[[UNDEF2]]

// CHECK-LABEL: @constants
// CHECK-NEXT: %[[CONSTANT_ADDRESS:.*]] = llvm.mlir.addressof @[[CONSTANT]]
// CHECK-NEXT: llvm.return %[[CONSTANT_ADDRESS]]

// -----

py.globalValue @builtins.tuple = #py.tuple<()>

func.func @constants() -> !py.dynamic {
    %0 = py.constant(#py.tuple<(#py.unbound)>)
    return %0 : !py.dynamic
}

// CHECK: llvm.mlir.global private unnamed_addr constant @[[CONSTANT:const\$[[:alnum:]]*]]
// CHECK-NEXT: %[[UNDEF:.*]] = llvm.mlir.undef
// CHECK-NEXT: %[[TYPE:.*]] = llvm.mlir.addressof @builtins.tuple
// CHECK-NEXT: %[[UNDEF1:.*]] = llvm.insertvalue %[[TYPE]], %[[UNDEF]][0]
// CHECK-NEXT: %[[SIZE:.*]] = llvm.mlir.constant(1 : i{{.*}})
// CHECK-NEXT: %[[UNDEF2:.*]] = llvm.insertvalue %[[SIZE]], %[[UNDEF1]][1]
// CHECK-NEXT: %[[NULLPTR:.*]] = llvm.mlir.null
// CHECK-NEXT: %[[UNDEF3:.*]] = llvm.insertvalue %[[NULLPTR]], %[[UNDEF2]][2, 0]
// CHECK-NEXT: llvm.return %[[UNDEF3]]

// CHECK-LABEL: @constants
// CHECK-NEXT: %[[CONSTANT_ADDRESS:.*]] = llvm.mlir.addressof @[[CONSTANT]]
// CHECK-NEXT: llvm.return %[[CONSTANT_ADDRESS]]
