// RUN: pylir-opt %s --test-memory-ssa --split-input-file | FileCheck %s

py.globalValue @builtins.type = #py.type
py.globalValue @builtins.str = #py.type

func @test() -> index {
    %0 = py.constant #py.str<"test">
    %1 = py.makeList ()
    py.list.append %1, %0
    %2 = py.list.len %1
    return %2 : index
}

// CHECK-LABEL: memSSA.region @test
// CHECK-NEXT: %[[LIVE_ON_ENTRY:.*]] = memSSA.liveOnEntry
// CHECK-NEXT: %[[DEF:.*]] = memSSA.def(%[[LIVE_ON_ENTRY]])
// CHECK-NEXT: // {{.*}} py.makeList
// CHECK-NEXT: %[[APPEND:.*]] = memSSA.def(%[[DEF]])
// CHECK-NEXT: // py.list.append
// CHECK-NEXT: memSSA.use(%[[APPEND]])
// CHECK-NEXT: // {{.*}} py.list.len

func @test2(%arg0 : i1) -> index {
    %0 = py.constant #py.str<"test">
    %1 = py.makeList ()
    cond_br %arg0, ^bb1, ^bb2

^bb1:
    py.list.append %1, %0
    br ^bb2

^bb2:
    %2 = py.list.len %1
    return %2 : index
}

// CHECK-LABEL: memSSA.region @test2
// CHECK-NEXT: %[[LIVE_ON_ENTRY:.*]] = memSSA.liveOnEntry
// CHECK-NEXT: %[[DEF:.*]] = memSSA.def(%[[LIVE_ON_ENTRY]])
// CHECK-NEXT: // {{.*}} py.makeList
// CHECK-NEXT: memSSA.br ^[[FIRST:.*]], ^[[SECOND:.*]] (), (%[[DEF]])
// CHECK-NEXT: ^[[FIRST]]:
// CHECK-NEXT: %[[APPEND:.*]] = memSSA.def(%[[DEF]])
// CHECK-NEXT: // py.list.append
// CHECK-NEXT: memSSA.br ^[[SECOND]] (%[[APPEND]])
// CHECK-NEXT: ^[[SECOND]]
// CHECK-SAME: %[[MERGE:[[:alnum:]]+]]
// CHECK-NEXT: memSSA.use(%[[MERGE]])
// CHECK-NEXT: // {{.*}} py.list.len
