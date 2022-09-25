//  Licensed under the Apache License v2.0 with LLVM Exceptions.
//  See https://llvm.org/LICENSE.txt for license information.
//  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "PylirPyDialect.hpp"

#include <mlir/Dialect/Arithmetic/IR/Arithmetic.h>
#include <mlir/Dialect/ControlFlow/IR/ControlFlow.h>
#include <mlir/Dialect/ControlFlow/IR/ControlFlowOps.h>
#include <mlir/IR/Matchers.h>

#include <llvm/ADT/TypeSwitch.h>

#include <pylir/Optimizer/Interfaces/DialectCostInterface.hpp>

#include "PylirPyAttributes.hpp"
#include "PylirPyOps.hpp"
#include "PylirPyTypes.hpp"
#include "TypeRefineableInterface.hpp"

#include "pylir/Optimizer/PylirPy/IR/PylirPyDialect.cpp.inc"

namespace
{

struct PylirPyCostInterface : public pylir::DialectCostInterface
{
    using pylir::DialectCostInterface::DialectCostInterface;

    std::size_t getCost(mlir::Operation* op) const override
    {
        return llvm::TypeSwitch<mlir::Operation*, std::size_t>(op)
            .Case<pylir::Py::GetSlotOp, pylir::Py::SetSlotOp, pylir::Py::ObjectFromTypeObjectInterface>(
                [](auto typeObjectUsers)
                { return mlir::matchPattern(typeObjectUsers.getTypeObject(), mlir::m_Constant()) ? 1 : 10; })
            .Case([](pylir::Py::UnreachableOp) { return 0; })
            .Case([](pylir::Py::RaiseOp) { return 5; })
            .Case<pylir::Py::FunctionCallOp, pylir::Py::FunctionInvokeOp>([](auto) { return 10; })
            .Case([](pylir::Py::MROLookupOp) { return 20; })
            .Case<pylir::Py::MakeTupleOp, pylir::Py::MakeTupleExOp, pylir::Py::MakeListOp, pylir::Py::MakeListExOp,
                  pylir::Py::MakeSetOp, pylir::Py::MakeSetExOp>([](auto op)
                                                                { return op.getIterExpansion().empty() ? 2 : 30; })
            .Case<pylir::Py::MakeDictOp, pylir::Py::MakeDictExOp>([](auto op)
                                                                  { return op.getMappingExpansion().empty() ? 2 : 30; })
            .Default(std::size_t{1});
    }
};
} // namespace

void pylir::Py::PylirPyDialect::initialize()
{
    addOperations<
#define GET_OP_LIST
#include "pylir/Optimizer/PylirPy/IR/PylirPyOps.cpp.inc"
        >();
    initializeTypes();
    initializeAttributes();
    initializeExternalModels();
    addInterfaces<PylirPyCostInterface>();
}

mlir::Operation* pylir::Py::PylirPyDialect::materializeConstant(::mlir::OpBuilder& builder, ::mlir::Attribute value,
                                                                ::mlir::Type type, ::mlir::Location loc)
{
    if (type.isa<Py::DynamicType>())
    {
        return builder.create<Py::ConstantOp>(loc, type, value);
    }
    if (mlir::arith::ConstantOp::isBuildableWith(value, type))
    {
        return builder.create<mlir::arith::ConstantOp>(loc, type, value);
    }
    return nullptr;
}

mlir::LogicalResult pylir::Py::PylirPyDialect::verifyOperationAttribute(mlir::Operation* op,
                                                                        mlir::NamedAttribute attribute)
{
    return op->emitOpError("Unknown dialect attribute ") << attribute.getName();
}
