// Copyright 2022 Markus Böck
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef PYLIRPY_TRANSFORM_PASSES
#define PYLIRPY_TRANSFORM_PASSES

include "mlir/Pass/PassBase.td"

def ExpandPyDialect : Pass<"pylir-expand-py-dialect"> {
    let summary = "Expands ops of the Py dialect to operations that can be lowered";
    let constructor = "::pylir::Py::createExpandPyDialectPass()";
    let dependentDialects = ["::pylir::Py::PylirPyDialect",
                             "::mlir::arith::ArithmeticDialect",
                             "::mlir::cf::ControlFlowDialect"];
}

def FoldHandles : Pass<"pylir-fold-handles", "::mlir::ModuleOp"> {
    let summary = "Fold py.globalHandle";
    let constructor = "::pylir::Py::createFoldHandlesPass()";
    let dependentDialects = ["::pylir::Py::PylirPyDialect"];

    let statistics = [
        Statistic<"m_noLoadHandlesRemoved", "No-load Handles removed",
            "Amount of handles that were removed as they were never loaded">,
        Statistic<"m_singleStoreHandlesConverted", "Single-store Handles converted",
            "Amount of handles converted to values as they only had a single constant store">
    ];
}

def HandleLoadStoreElimination : Pass<"pylir-handle-load-store-elimination"> {
    let summary = "Eliminate loads and stores of handles";
    let constructor = "::pylir::Py::createHandleLoadStoreEliminationPass()";

    let statistics = [
        Statistic<"m_loadRemoved", "handle loads removed",
            "Amount of loads of handles that could be removed">,
        Statistic<"m_storesRemoved", "Stores to handles removed",
            "Amount of stores to handles removed">
    ];
}

def Monomorph : Pass<"pylir-monomorph", "::mlir::ModuleOp"> {
	let summary = "Monomorph module";
	let constructor = "::pylir::Py::createMonomorphPass()";
	let dependentDialects = ["::pylir::Py::PylirPyDialect", "::pylir::TypeFlow::TypeFlowDialect"];

	let statistics = [
		Statistic<"m_valuesReplaced", "Values replaced", "Amount of values that have been replaced with constants">,
		Statistic<"m_callsChanged", "Calls changed", "Amount of call instructions that were changed">,
		Statistic<"m_functionsCloned", "Function clones", "Amount of functions which have been cloned with more specific types">,
		Statistic<"m_typesOverDefined", "Types over defined", "Amount of types that have been marked over defined">,
	];

	let options = [
        Option<"m_maxTypeSize", "max-type-size", "std::size_t", "6",
                "Max amount of types a recursive type can have as function argument without being marked over defined">,
    ];
}

def Inliner : Pass<"pylir-inliner", "::mlir::ModuleOp"> {
	let summary = "Inline functions";
	let constructor = "::pylir::Py::createInlinerPass()";
	let dependentDialects = ["::pylir::Py::PylirPyDialect"];

	let statistics = [
        Statistic<"m_callsInlined", "Calls inlined", "Amount of call sites inlined">,
	];

	let options = [
		Option<"m_maxFuncGrowth", "max-func-growth", "std::uint32_t", "150", "Percent a function is allowed to grow">,
		Option<"m_maxModuleGrowth", "max-module-growth", "std::uint32_t", "200", "Percent the module is allowed to grow">,
		Option<"m_maxRecursiveInlines", "max-recursive-inlines", "std::uint32_t", "4", "Amount of times a recursive function might be recursively inlined">,
	];
}

def TrialInliner : Pass<"pylir-trial-inliner", "::mlir::ModuleOp"> {
	let summary = "Inline functions using trials";
	let constructor = "::pylir::Py::createTrialInlinerPass()";
	let dependentDialects = ["::pylir::Py::PylirPyDialect"];

	let statistics = [
		Statistic<"m_callsInlined", "Calls inlined", "Amount of call sites inlined">,
		Statistic<"m_cacheHits", "Cache hits", "Amount of times an inlining trial did not have to be performed due to a cache hit">,
		Statistic<"m_cacheMisses", "Caches misses", "Amount of times an inlining trial had to be performed as it was not in cache">,
		Statistic<"m_recursionLimitReached", "Recursion limits reached", "Amount of times a recursion chain was disabled in a caller">,
	];

	let options = [
		Option<"m_minCalleeSizeReduction", "min-callee-size-reduction", "std::uint32_t", "50",
			    "Percent the callee must have decreased in size after having been inlined in the caller to be deemed profitable">,
		Option<"m_maxRecursionDepth", "max-recursion-depth", "std::size_t", "4",
				"Maximum amount of times a recursive function deemed profitable may be inlined">,
        Option<"m_optimizationPipeline", "optimization-pipeline", "std::string",
               [{"canonicalize,pylir-sroa,canonicalize"}],
               "Optimization pipeline used to perform the inlining trials">,
	];
}

def SROA  : Pass<"pylir-sroa"> {
    let summary = "Scalar Replacement Of Aggregates";
    let constructor = "::pylir::Py::createSROAPass()";
    let dependentDialects = ["::pylir::Py::PylirPyDialect"];

    let statistics = [
        Statistic<"m_aggregatesRemoved", "Aggregates removed", "Amount of aggregates that were removed">,
        Statistic<"m_readOpsRemoved", "Aggregate reads removed", "Amount of reads from aggregates that were removed">,
        Statistic<"m_writeOpsRemoved", "Aggregate writes removed", "Amount of writes to aggregates that were removed">,
    ];
}

#endif
