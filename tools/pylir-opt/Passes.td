// Copyright 2022 Markus Böck
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef TEST_PASSES
#define TEST_PASSES

include "mlir/Pass/PassBase.td"

def TestMemorySSA : Pass<"test-memory-ssa","::mlir::ModuleOp"> {
    let constructor = "::createTestMemorySSA()";
    let dependentDialects = ["::pylir::MemSSA::MemorySSADialect"];
}

def TestInlinerInterface : Pass<"test-inliner-interface", "::mlir::ModuleOp"> {
	let constructor = "::createTestInlinerInterface()";
	let dependentDialects = ["::pylir::Py::PylirPyDialect"];
}

def TestTypeFlow : Pass<"test-type-flow", "::mlir::ModuleOp"> {
    let constructor = "::createTestTypeFlow()";
    let dependentDialects = ["::pylir::TypeFlow::TypeFlowDialect"];
}

def TestAliasSetTracker : Pass<"test-alias-set-tracker", "::mlir::ModuleOp"> {
    let constructor = "::createTestAliasSetTracker()";
}

def TestLoopInfo : Pass<"test-loop-info", "::mlir::ModuleOp"> {
    let constructor = "::createTestLoopInfo()";
}

def TestHelloWorld : Pass<"test-hello-world"> {
    let constructor = "::createTestHelloWorld()";
}

def TestLinker : Pass<"test-linker", "::mlir::ModuleOp"> {
    let constructor = "::createTestLinker()";
}

def TestInlineAll : Pass<"test-inline-all", "::mlir::ModuleOp"> {
    let constructor = "::createTestInlineAll()";

    let statistics = [
    	Statistic<"m_callsInlined", "Calls inlined", "Amount of call sites inlined">,
    	Statistic<"m_recursionLimitReached", "Recursion limits reached", "Amount of times a recursion chain was disabled in a caller">,
    ];

    let options = [
    	Option<"m_maxRecursionDepth", "max-recursion-depth", "std::size_t", "4",
    			"Maximum amount of times a recursive function deemed profitable may be inlined">,
        Option<"m_optimizationPipeline", "optimization-pipeline", "std::string", "",
                "Optimization pipeline used to perform the inlining trials">,
    ];
}

#endif
