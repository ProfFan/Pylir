// RUN: pylir-opt %s -expand-py-dialect --split-input-file | FileCheck %s

func @linear_search(%tuple : !py.dynamic) -> !py.dynamic {
    %0, %1 = py.mroLookup "__call__" in %tuple
    return %0 : !py.dynamic
}

// CHECK-LABEL: @linear_search
// CHECK-SAME: %[[TUPLE:[[:alnum:]]+]]
// CHECK: %[[TUPLE_LEN:.*]] = py.tuple.integer.len %[[TUPLE]]
// CHECK: %[[ZERO:.*]] = constant 0
// CHECK: br ^[[CONDITION:[[:alnum:]]+]]
// CHECK-SAME: %[[ZERO]]

// CHECK: ^[[CONDITION]]
// CHECK-SAME: %[[INDEX:[[:alnum:]]+]]

// CHECK: %[[LESS:.*]] = cmpi ult, %[[INDEX]], %[[TUPLE_LEN]]
// CHECK: %[[UNBOUND:.*]] = py.constant #py.unbound
// CHECK: %[[FALSE:.*]] = constant false
// CHECK: cond_br %[[LESS]], ^[[BODY:[[:alnum:]]+]], ^[[END:[[:alnum:]]+]]
// CHECK-SAME: %[[UNBOUND]]
// CHECK-SAME: %[[FALSE]]

// CHECK: ^[[BODY]]:
// CHECK: %[[ENTRY:.*]] = py.tuple.integer.getItem %[[TUPLE]]
// CHECK-SAME: %[[INDEX]]
// CHECK: %[[RESULT:.*]], %[[SUCCESS:.*]] = py.getAttr "__call__" from %[[ENTRY]]
// CHECK: cond_br %[[SUCCESS]], ^[[END]]
// CHECK-SAME: %[[RESULT]]
// CHECK-SAME: %[[SUCCESS]]
// CHECK-SAME: ^[[NOT_FOUND:[[:alnum:]]+]]

// CHECK: ^[[NOT_FOUND]]
// CHECK: %[[ONE:.*]] = constant 1
// CHECK: %[[NEXT:.*]] = addi %[[INDEX]], %[[ONE]]
// CHECK: br ^[[CONDITION]]
// CHECK-SAME: %[[NEXT]]

// CHECK: ^[[END]]
// CHECK-SAME: %[[RESULT:[[:alnum:]]+]]
// CHECK: return %[[RESULT]]
