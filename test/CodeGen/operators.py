# RUN: pylir %s -emit-pylir -o - -c -S | FileCheck %s

# CHECK-LABEL: @"bin_ops$impl[0]"
# CHECK-SAME: %{{[[:alnum:]]+}}
# CHECK-SAME: %[[A:[[:alnum:]]+]]
# CHECK-SAME: %[[B:[[:alnum:]]+]]

def bin_ops(a, b):
    a + b
    # CHECK: py.call @pylir__add__(%[[A]], %[[B]])

    a - b
    # CHECK: py.call @pylir__sub__(%[[A]], %[[B]])

    a | b
    # CHECK: py.call @pylir__or__(%[[A]], %[[B]])

    a ^ b
    # CHECK: py.call @pylir__xor__(%[[A]], %[[B]])

    a & b
    # CHECK: py.call @pylir__and__(%[[A]], %[[B]])

    a << b
    # CHECK: py.call @pylir__lshift__(%[[A]], %[[B]])

    a >> b
    # CHECK: py.call @pylir__rshift__(%[[A]], %[[B]])

    a * b
    # CHECK: py.call @pylir__mul__(%[[A]], %[[B]])

    a / b
    # CHECK: py.call @pylir__div__(%[[A]], %[[B]])

    a // b
    # CHECK: py.call @pylir__floordiv__(%[[A]], %[[B]])

    a @ b
    # CHECK: py.call @pylir__matmul__(%[[A]], %[[B]])


# CHECK-LABEL: @"unary_ops$impl[0]"
# CHECK-SAME: %{{[[:alnum:]]+}}
# CHECK-SAME: %[[A:[[:alnum:]]+]]

def unary_ops(a):
    -a
    # CHECK: py.call @pylir__neg__(%[[A]])
    +a
    # CHECK: py.call @pylir__pos__(%[[A]])
    ~a
    # CHECK: py.call @pylir__invert__(%[[A]])

# CHECK-LABEL: @"boolean_ops$impl[0]"
# CHECK-SAME: %{{[[:alnum:]]+}}
# CHECK-SAME: %[[A:[[:alnum:]]+]]
# CHECK-SAME: %[[B:[[:alnum:]]+]]
def boolean_ops(a, b):
    global c
    # Testing with calls on purpose to check in the output that side effects
    # are correctly sequenced

    c = a() and b()
    # CHECK: %[[TUPLE:.*]] = py.makeTuple ()
    # CHECK: %[[DICT:.*]] = py.constant(#py.dict<{}>)
    # CHECK: %[[RES:.*]] = py.call @pylir__call__(%[[A]], %[[TUPLE]], %[[DICT]])
    # CHECK: %[[BOOL:.*]] = py.constant(#py.ref<@builtins.bool>)
    # CHECK: %[[TYPE:.*]] = py.typeOf %[[RES]]
    # CHECK: %[[IS_BOOL:.*]] = py.is %[[TYPE]], %[[BOOL]]
    # CHECK: cf.cond_br %[[IS_BOOL]], ^[[CONTINUE:.*]](%[[RES]] : !py.dynamic), ^[[CALC_BOOL:[[:alnum:]]+]]
    # CHECK: ^[[CALC_BOOL]]:
    # CHECK: %[[TUPLE:.*]] = py.makeTuple (%[[RES]])
    # CHECK: %[[DICT:.*]] = py.constant(#py.dict<{}>)
    # CHECK: %[[RES_AS_BOOL:.*]] = py.call @pylir__call__(%[[BOOL]], %[[TUPLE]], %[[DICT]])
    # CHECK: cf.br ^[[CONTINUE]](%[[RES_AS_BOOL]] : !py.dynamic)
    # CHECK: ^[[CONTINUE]](%[[RES_AS_BOOL:.*]]: !py.dynamic loc({{.*}})):
    # CHECK: %[[RES_AS_I1:.*]] = py.bool.toI1 %[[RES_AS_BOOL]]
    # CHECK: cf.cond_br %[[RES_AS_I1]], ^[[CALCULATE_B_BLOCK:.*]], ^[[DEST:.*]](%[[RES]] : !py.dynamic)

    # CHECK: ^[[CALCULATE_B_BLOCK]]:
    # CHECK: %[[TUPLE:.*]] = py.makeTuple ()
    # CHECK: %[[DICT:.*]] = py.constant(#py.dict<{}>)
    # CHECK: %[[RES:.*]] = py.call @pylir__call__(%[[B]], %[[TUPLE]], %[[DICT]])
    # CHECK: cf.br ^[[DEST]](%[[RES]] : !py.dynamic)

    # CHECK: ^[[DEST]](
    # CHECK-SAME: %[[ARG:[[:alnum:]]+]]
    # CHECK: py.store %[[ARG]] : !py.dynamic into @c

    c = a() or b()
    # CHECK: %[[TUPLE:.*]] = py.makeTuple ()
    # CHECK: %[[DICT:.*]] = py.constant(#py.dict<{}>)
    # CHECK: %[[RES:.*]] = py.call @pylir__call__(%[[A]], %[[TUPLE]], %[[DICT]])
    # CHECK: %[[BOOL:.*]] = py.constant(#py.ref<@builtins.bool>)
    # CHECK: %[[TYPE:.*]] = py.typeOf %[[RES]]
    # CHECK: %[[IS_BOOL:.*]] = py.is %[[TYPE]], %[[BOOL]]
    # CHECK: cf.cond_br %[[IS_BOOL]], ^[[CONTINUE:.*]](%[[RES]] : !py.dynamic), ^[[CALC_BOOL:[[:alnum:]]+]]
    # CHECK: ^[[CALC_BOOL]]:
    # CHECK: %[[TUPLE:.*]] = py.makeTuple (%[[RES]])
    # CHECK: %[[DICT:.*]] = py.constant(#py.dict<{}>)
    # CHECK: %[[RES_AS_BOOL:.*]] = py.call @pylir__call__(%[[BOOL]], %[[TUPLE]], %[[DICT]])
    # CHECK: cf.br ^[[CONTINUE]](%[[RES_AS_BOOL]] : !py.dynamic)
    # CHECK: ^[[CONTINUE]](%[[RES_AS_BOOL:.*]]: !py.dynamic loc({{.*}})):
    # CHECK: %[[RES_AS_I1:.*]] = py.bool.toI1 %[[RES_AS_BOOL]]
    # CHECK: cf.cond_br %[[RES_AS_I1]], ^[[DEST:.*]](%[[RES]] : !py.dynamic), ^[[CALCULATE_B_BLOCK:[[:alnum:]]+]]

    # CHECK: ^[[CALCULATE_B_BLOCK]]:
    # CHECK: %[[TUPLE:.*]] = py.makeTuple ()
    # CHECK: %[[DICT:.*]] = py.constant(#py.dict<{}>)
    # CHECK: %[[RES:.*]] = py.call @pylir__call__(%[[B]], %[[TUPLE]], %[[DICT]])
    # CHECK: cf.br ^[[DEST]](%[[RES]] : !py.dynamic)

    # CHECK: ^[[DEST]](
    # CHECK-SAME: %[[ARG:[[:alnum:]]+]]
    # CHECK: py.store %[[ARG]] : !py.dynamic into @c

    c = not a
    # CHECK: %[[BOOL:.*]] = py.constant(#py.ref<@builtins.bool>)
    # CHECK: %[[TYPE:.*]] = py.typeOf %[[A]]
    # CHECK: %[[IS_BOOL:.*]] = py.is %[[TYPE]], %[[BOOL]]
    # CHECK: cf.cond_br %[[IS_BOOL]], ^[[CONTINUE:.*]](%[[A]] : !py.dynamic), ^[[CALC_BOOL:[[:alnum:]]+]]
    # CHECK: ^[[CALC_BOOL]]:
    # CHECK: %[[TUPLE:.*]] = py.makeTuple (%[[A]])
    # CHECK: %[[DICT:.*]] = py.constant(#py.dict<{}>)
    # CHECK: %[[RES_AS_BOOL:.*]] = py.call @pylir__call__(%[[BOOL]], %[[TUPLE]], %[[DICT]])
    # CHECK: cf.br ^[[CONTINUE]](%[[RES_AS_BOOL]] : !py.dynamic)
    # CHECK: ^[[CONTINUE]](%[[RES_AS_BOOL:.*]]: !py.dynamic loc({{.*}})):
    # CHECK: %[[RES_AS_I1:.*]] = py.bool.toI1 %[[RES_AS_BOOL]]
    # CHECK: %[[TRUE:.*]] = arith.constant true
    # CHECK: %[[INVERTED:.*]] = arith.xori %[[TRUE]], %[[RES_AS_I1]]
    # CHECK: %[[AS_BOOL:.*]] = py.bool.fromI1 %[[INVERTED]]
    # CHECK: py.store %[[AS_BOOL]] : !py.dynamic into @c


# CHECK-LABEL: @"aug_assign_ops$impl[0]"
# CHECK-SAME: %{{[[:alnum:]]+}}
# CHECK-SAME: %[[A:[[:alnum:]]+]]
# CHECK-SAME: %[[B:[[:alnum:]]+]]
def aug_assign_ops(a, b):
    a += b
    # CHECK: %[[A_1:.*]] = py.call @pylir__iadd__(%[[A]], %[[B]])
    a -= b
    # CHECK: %[[A_2:.*]] = py.call @pylir__isub__(%[[A_1]], %[[B]])
    a *= b
    # CHECK: %[[A_3:.*]] = py.call @pylir__imul__(%[[A_2]], %[[B]])
    a /= b
    # CHECK: %[[A_4:.*]] = py.call @pylir__idiv__(%[[A_3]], %[[B]])
    a //= b
    # CHECK: %[[A_5:.*]] = py.call @pylir__ifloordiv__(%[[A_4]], %[[B]])
    a %= b
    # CHECK: %[[A_6:.*]] = py.call @pylir__imod__(%[[A_5]], %[[B]])
    a @= b
    # CHECK: %[[A_7:.*]] = py.call @pylir__imatmul__(%[[A_6]], %[[B]])
    a &= b
    # CHECK: %[[A_8:.*]] = py.call @pylir__iand__(%[[A_7]], %[[B]])
    a |= b
    # CHECK: %[[A_9:.*]] = py.call @pylir__ior__(%[[A_8]], %[[B]])
    a ^= b
    # CHECK: %[[A_10:.*]] = py.call @pylir__ixor__(%[[A_9]], %[[B]])
    a >>= b
    # CHECK: %[[A_11:.*]] = py.call @pylir__irshift__(%[[A_10]], %[[B]])
    a <<= b
    # CHECK: %[[A_12:.*]] = py.call @pylir__ilshift__(%[[A_11]], %[[B]])
    return a
    # CHECK: return %[[A_12]]
