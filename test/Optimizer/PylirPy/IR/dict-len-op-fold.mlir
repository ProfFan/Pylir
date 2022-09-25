// RUN: pylir-opt %s -canonicalize --split-input-file | FileCheck %s

py.globalValue @builtins.type = #py.type
py.globalValue @builtins.dict = #py.type
py.globalValue @builtins.str = #py.type

func.func @test() -> index {
    %0 = py.constant(#py.dict<{#py.str<"test"> to #py.ref<@builtins.str>}>)
    %2 = py.dict.len %0
    return %2 : index
}

// CHECK-LABEL: @test
// CHECK-DAG: %[[C1:.*]] = arith.constant 1
// CHECK: return %[[C1]]
