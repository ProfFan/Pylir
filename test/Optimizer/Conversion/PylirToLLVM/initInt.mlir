// RUN: pylir-opt %s -convert-pylir-to-llvm --split-input-file | FileCheck %s

py.globalValue const @builtins.type = #py.type
py.globalValue const @builtins.int = #py.type
py.globalValue const @builtins.tuple = #py.type

func.func @foo(%value : index) -> !py.dynamic {
    %0 = py.constant(#py.ref<@builtins.int>)
    %1 = pyMem.gcAllocObject %0
    %2 = pyMem.initIntUnsigned %1 to %value
    return %2 : !py.dynamic
}

// CHECK-LABEL: llvm.func @foo
// CHECK-SAME: %[[VALUE:[[:alnum:]]+]]
// CHECK: %[[MEMORY:.*]] = llvm.call @pylir_gc_alloc
// CHECK: %[[GEP:.*]] = llvm.getelementptr %[[MEMORY]][0, 1]
// CHECK-NEXT: llvm.call @mp_init_u64(%[[GEP]], %[[VALUE]])
// CHECK-NEXT: llvm.return %[[MEMORY]]

func.func @bar(%value : index) -> !py.dynamic {
    %0 = py.constant(#py.ref<@builtins.int>)
    %1 = pyMem.gcAllocObject %0
    %2 = pyMem.initIntSigned %1 to %value
    return %2 : !py.dynamic
}

// CHECK-LABEL: llvm.func @bar
// CHECK-SAME: %[[VALUE:[[:alnum:]]+]]
// CHECK: %[[MEMORY:.*]] = llvm.call @pylir_gc_alloc
// CHECK: %[[GEP:.*]] = llvm.getelementptr %[[MEMORY]][0, 1]
// CHECK-NEXT: llvm.call @mp_init_i64(%[[GEP]], %[[VALUE]])
// CHECK-NEXT: llvm.return %[[MEMORY]]
