// RUN: pylir-opt %s -canonicalize --split-input-file | FileCheck %s

py.globalValue @builtins.type = #py.type
py.globalValue @builtins.int = #py.type
py.globalValue @builtins.bool = #py.type

func.func @test1(%arg0 : !py.dynamic) -> i1 {
    %0 = py.constant(#py.int<5>)
    %1 = py.int.cmp eq %0, %arg0
    return %1 : i1
}

// CHECK-LABEL: @test1
// CHECK-SAME: %[[ARG0:[[:alnum:]]+]]
// CHECK-NEXT: %[[C:.*]] = py.constant(#py.int<5>)
// CHECK-NEXT: %[[RESULT:.*]] = py.int.cmp eq %[[ARG0]], %[[C]]
// CHECK-NEXT: return %[[RESULT]]

func.func @test2(%arg0 : !py.dynamic) -> i1 {
    %0 = py.constant(#py.int<5>)
    %1 = py.int.cmp ne %0, %arg0
    return %1 : i1
}

// CHECK-LABEL: @test2
// CHECK-SAME: %[[ARG0:[[:alnum:]]+]]
// CHECK-NEXT: %[[C:.*]] = py.constant(#py.int<5>)
// CHECK-NEXT: %[[RESULT:.*]] = py.int.cmp ne %[[ARG0]], %[[C]]
// CHECK-NEXT: return %[[RESULT]]

func.func @test3(%arg0 : !py.dynamic) -> i1 {
    %0 = py.constant(#py.int<5>)
    %1 = arith.constant 1 : i1
    %2 = py.int.cmp ne %0, %arg0
    %3 = arith.xori %1, %2 : i1
    return %3 : i1
}

// CHECK-LABEL: @test3
// CHECK-SAME: %[[ARG0:[[:alnum:]]+]]
// CHECK-NEXT: %[[C:.*]] = py.constant(#py.int<5>)
// CHECK-NEXT: %[[RESULT:.*]] = py.int.cmp eq %[[ARG0]], %[[C]]
// CHECK-NEXT: return %[[RESULT]]

func.func @test4(%arg0 : !py.dynamic) -> i1 {
    %0 = py.constant(#py.int<5>)
    %1 = arith.constant 1 : i1
    %2 = py.int.cmp eq %0, %arg0
    %3 = arith.xori %1, %2 : i1
    return %3 : i1
}

// CHECK-LABEL: @test4
// CHECK-SAME: %[[ARG0:[[:alnum:]]+]]
// CHECK-NEXT: %[[C:.*]] = py.constant(#py.int<5>)
// CHECK-NEXT: %[[RESULT:.*]] = py.int.cmp ne %[[ARG0]], %[[C]]
// CHECK-NEXT: return %[[RESULT]]

func.func @test5(%arg0 : !py.dynamic) -> i1 {
    %0 = py.constant(#py.int<5>)
    %1 = arith.constant 1 : i1
    %2 = py.int.cmp lt %0, %arg0
    %3 = arith.xori %1, %2 : i1
    return %3 : i1
}

// CHECK-LABEL: @test5
// CHECK-SAME: %[[ARG0:[[:alnum:]]+]]
// CHECK-NEXT: %[[C:.*]] = py.constant(#py.int<5>)
// CHECK-NEXT: %[[RESULT:.*]] = py.int.cmp le %[[ARG0]], %[[C]]
// CHECK-NEXT: return %[[RESULT]]


func.func @test6(%arg0 : !py.dynamic) -> i1 {
    %0 = py.constant(#py.int<5>)
    %1 = arith.constant 1 : i1
    %2 = py.int.cmp le %0, %arg0
    %3 = arith.xori %1, %2 : i1
    return %3 : i1
}

// CHECK-LABEL: @test6
// CHECK-SAME: %[[ARG0:[[:alnum:]]+]]
// CHECK-NEXT: %[[C:.*]] = py.constant(#py.int<5>)
// CHECK-NEXT: %[[RESULT:.*]] = py.int.cmp lt %[[ARG0]], %[[C]]
// CHECK-NEXT: return %[[RESULT]]

func.func @test7(%arg0 : !py.dynamic) -> i1 {
    %0 = py.constant(#py.int<5>)
    %1 = arith.constant 1 : i1
    %2 = py.int.cmp gt %0, %arg0
    %3 = arith.xori %1, %2 : i1
    return %3 : i1
}

// CHECK-LABEL: @test7
// CHECK-SAME: %[[ARG0:[[:alnum:]]+]]
// CHECK-NEXT: %[[C:.*]] = py.constant(#py.int<5>)
// CHECK-NEXT: %[[RESULT:.*]] = py.int.cmp ge %[[ARG0]], %[[C]]
// CHECK-NEXT: return %[[RESULT]]


func.func @test8(%arg0 : !py.dynamic) -> i1 {
    %0 = py.constant(#py.int<5>)
    %1 = arith.constant 1 : i1
    %2 = py.int.cmp ge %0, %arg0
    %3 = arith.xori %1, %2 : i1
    return %3 : i1
}

// CHECK-LABEL: @test8
// CHECK-SAME: %[[ARG0:[[:alnum:]]+]]
// CHECK-NEXT: %[[C:.*]] = py.constant(#py.int<5>)
// CHECK-NEXT: %[[RESULT:.*]] = py.int.cmp gt %[[ARG0]], %[[C]]
// CHECK-NEXT: return %[[RESULT]]

func.func @test_ne_lt() -> i1 {
    %n = arith.constant true
    %0 = py.constant(#py.int<1>)
    %1 = py.constant(#py.int<2>)
    %2 = py.int.cmp eq %0, %1
    %3 = py.int.cmp ne %0, %1
    %4 = py.int.cmp lt %0, %1
    %5 = py.int.cmp le %0, %1
    %6 = py.int.cmp gt %0, %1
    %7 = py.int.cmp ge %0, %1
    %8 = arith.xori %2, %n : i1
    %9 = arith.xori %6, %n : i1
    %10 = arith.xori %7, %n : i1
    %12 = arith.andi %3, %8 : i1
    %13 = arith.andi %12, %4 : i1
    %14 = arith.andi %13, %5 : i1
    %15 = arith.andi %14, %9 : i1
    %16 = arith.andi %15, %10 : i1
    return %16 : i1
}

// CHECK-LABEL: func.func @test_ne_lt
// CHECK-NEXT: %[[C:.*]] = arith.constant true
// CHECK-NEXT: return %[[C]]

func.func @test_ne_gt() -> i1 {
    %n = arith.constant true
    %0 = py.constant(#py.int<2>)
    %1 = py.constant(#py.int<1>)
    %2 = py.int.cmp eq %0, %1
    %3 = py.int.cmp ne %0, %1
    %4 = py.int.cmp lt %0, %1
    %5 = py.int.cmp le %0, %1
    %6 = py.int.cmp gt %0, %1
    %7 = py.int.cmp ge %0, %1
    %8 = arith.xori %2, %n : i1
    %9 = arith.xori %4, %n : i1
    %10 = arith.xori %5, %n : i1
    %12 = arith.andi %3, %8 : i1
    %13 = arith.andi %12, %6 : i1
    %14 = arith.andi %13, %7 : i1
    %15 = arith.andi %14, %9 : i1
    %16 = arith.andi %15, %10 : i1
    return %16 : i1
}

// CHECK-LABEL: func.func @test_ne_gt
// CHECK-NEXT: %[[C:.*]] = arith.constant true
// CHECK-NEXT: return %[[C]]

func.func @test_eq() -> i1 {
    %n = arith.constant true
    %0 = py.constant(#py.bool<True>)
    %1 = py.constant(#py.int<1>)
    %2 = py.int.cmp eq %0, %1
    %3 = py.int.cmp ne %0, %1
    %4 = py.int.cmp lt %0, %1
    %5 = py.int.cmp le %0, %1
    %6 = py.int.cmp gt %0, %1
    %7 = py.int.cmp ge %0, %1
    %8 = arith.xori %3, %n : i1
    %9 = arith.xori %4, %n : i1
    %10 = arith.xori %6, %n : i1
    %12 = arith.andi %2, %8 : i1
    %13 = arith.andi %12, %9 : i1
    %14 = arith.andi %13, %5 : i1
    %15 = arith.andi %14, %10 : i1
    %16 = arith.andi %15, %7 : i1
    return %16 : i1
}

// CHECK-LABEL: func.func @test_eq
// CHECK-NEXT: %[[C:.*]] = arith.constant true
// CHECK-NEXT: return %[[C]]
