// RUN: pylir-opt %s -convert-pylir-to-llvm --split-input-file | FileCheck %s

func.func @test(%arg : !py.dynamic) -> !py.dynamic {
    %0 = py.typeOf %arg
    return %0 : !py.dynamic
}

// CHECK: @test
// CHECK-SAME: %[[ARG:[[:alnum:]]+]]
// CHECK-NEXT: %[[GEP:.*]] = llvm.getelementptr %[[ARG]][0, 0]
// CHECK-NEXT: %[[RESULT:.*]] = llvm.load %[[GEP]]
// CHECK-NEXT: llvm.return %[[RESULT]]
