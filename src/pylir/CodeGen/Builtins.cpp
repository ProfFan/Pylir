#include <mlir/Dialect/StandardOps/IR/Ops.h>

#include "CodeGen.hpp"

void pylir::CodeGen::createBuiltinsImpl()
{
    auto noDefaultsFunctionDict =
        Py::DictAttr::get(m_builder.getContext(),
                          {{m_builder.getStringAttr("__defaults__"), m_builder.getSymbolRefAttr(Builtins::None)},
                           {m_builder.getStringAttr("__kwdefaults__"), m_builder.getSymbolRefAttr(Builtins::None)}});
    auto loc = m_builder.getUnknownLoc();
    {
        std::vector<std::pair<mlir::Attribute, mlir::Attribute>> members;
        {
            auto newCall = m_builder.create<mlir::FuncOp>(
                loc, "builtins.object.__new__$impl",
                m_builder.getFunctionType({m_builder.getType<Py::DynamicType>(), m_builder.getType<Py::DynamicType>(),
                                           m_builder.getType<Py::DynamicType>(), m_builder.getType<Py::DynamicType>()},
                                          {m_builder.getType<Py::DynamicType>()}));
            mlir::OpBuilder::InsertionGuard guard{m_builder};
            m_builder.setInsertionPointToStart(newCall.addEntryBlock());
            [[maybe_unused]] auto self = newCall.getArgument(0);
            auto clazz = newCall.getArgument(1);
            [[maybe_unused]] auto args = newCall.getArgument(2);
            [[maybe_unused]] auto kw = newCall.getArgument(3);
            m_currentFunc = newCall;
            // TODO: How to handle non `type` derived type objects?
            auto obj = m_builder.create<Py::MakeObjectOp>(loc, clazz);
            m_builder.create<mlir::ReturnOp>(loc, mlir::ValueRange{obj});

            members.emplace_back(
                m_builder.getStringAttr("__new__"),
                Py::ObjectAttr::get(m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Function),
                                    noDefaultsFunctionDict,
                                    m_builder.getSymbolRefAttr(buildFunctionCC(
                                        loc, "builtins.object.__new__$cc", newCall,
                                        {FunctionParameter{"", FunctionParameter::PosOnly, false},
                                         FunctionParameter{"", FunctionParameter::PosRest, false},
                                         FunctionParameter{"", FunctionParameter::KeywordRest, false}}))));
        }
        {
            auto initCall = m_builder.create<mlir::FuncOp>(
                loc, "builtins.object.__init__$impl",
                m_builder.getFunctionType({m_builder.getType<Py::DynamicType>(), m_builder.getType<Py::DynamicType>()},
                                          {m_builder.getType<Py::DynamicType>()}));
            mlir::OpBuilder::InsertionGuard guard{m_builder};
            m_builder.setInsertionPointToStart(initCall.addEntryBlock());
            m_currentFunc = initCall;

            // __init__ may only return None: https://docs.python.org/3/reference/datamodel.html#object.__init__
            m_builder.create<mlir::ReturnOp>(
                loc, mlir::ValueRange{m_builder.create<Py::GetGlobalValueOp>(loc, Builtins::None)});

            members.emplace_back(
                m_builder.getStringAttr("__init__"),
                Py::ObjectAttr::get(m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Function),
                                    noDefaultsFunctionDict,
                                    m_builder.getSymbolRefAttr(
                                        buildFunctionCC(loc, "builtins.object.__init__$cc", initCall,
                                                        {FunctionParameter{"", FunctionParameter::PosOnly, false}}))));
        }
        members.emplace_back(
            m_builder.getStringAttr("__mro__"),
            Py::TupleAttr::get(m_builder.getContext(), {m_builder.getSymbolRefAttr(Builtins::Object)}));

        auto dict = Py::DictAttr::get(m_builder.getContext(), members);
        m_builder.create<Py::GlobalValueOp>(
            loc, Builtins::Object, mlir::StringAttr{}, true,
            Py::ObjectAttr::get(m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Type), dict, llvm::None));
    }
    mlir::FuncOp baseExceptionNew;
    mlir::FuncOp baseExceptionInit;
    {
        std::vector<std::pair<mlir::Attribute, mlir::Attribute>> members;
        {
            auto newCall = m_builder.create<mlir::FuncOp>(
                loc, "builtins.BaseException.__new__$impl",
                m_builder.getFunctionType({m_builder.getType<Py::DynamicType>(), m_builder.getType<Py::DynamicType>(),
                                           m_builder.getType<Py::DynamicType>()},
                                          {m_builder.getType<Py::DynamicType>()}));
            mlir::OpBuilder::InsertionGuard guard{m_builder};
            m_builder.setInsertionPointToStart(newCall.addEntryBlock());
            [[maybe_unused]] auto self = newCall.getArgument(0);
            [[maybe_unused]] auto clazz = newCall.getArgument(1);
            [[maybe_unused]] auto args = newCall.getArgument(2);
            m_currentFunc = newCall;

            auto obj = m_builder.create<Py::MakeObjectOp>(loc, clazz);
            m_builder.create<Py::SetAttrOp>(loc, args, obj, "args");

            members.emplace_back(
                m_builder.getStringAttr("__new__"),
                Py::ObjectAttr::get(
                    m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Function), noDefaultsFunctionDict,
                    m_builder.getSymbolRefAttr(baseExceptionNew = buildFunctionCC(
                                                   loc, "builtins.BaseException.__new__$cc", newCall,
                                                   {FunctionParameter{"", FunctionParameter::PosOnly, false},
                                                    FunctionParameter{"", FunctionParameter::PosRest, false}}))));
        }
        {
            auto initCall = m_builder.create<mlir::FuncOp>(
                loc, "builtins.BaseException.__init__$impl",
                m_builder.getFunctionType({m_builder.getType<Py::DynamicType>(), m_builder.getType<Py::DynamicType>(),
                                           m_builder.getType<Py::DynamicType>()},
                                          {m_builder.getType<Py::DynamicType>()}));
            mlir::OpBuilder::InsertionGuard guard{m_builder};
            m_builder.setInsertionPointToStart(initCall.addEntryBlock());
            [[maybe_unused]] auto self = initCall.getArgument(1);
            [[maybe_unused]] auto args = initCall.getArgument(2);
            m_currentFunc = initCall;

            m_builder.create<Py::SetAttrOp>(loc, args, self, "args");

            members.emplace_back(
                m_builder.getStringAttr("__init__"),
                Py::ObjectAttr::get(
                    m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Function), noDefaultsFunctionDict,
                    m_builder.getSymbolRefAttr(baseExceptionInit = buildFunctionCC(
                                                   loc, formImplName("builtins.BaseException.__init__$cc"), initCall,
                                                   {FunctionParameter{"", FunctionParameter::PosOnly, false},
                                                    FunctionParameter{"", FunctionParameter::PosRest, false}}))));
        }
        members.emplace_back(
            m_builder.getStringAttr("__mro__"),
            Py::TupleAttr::get(m_builder.getContext(), {m_builder.getSymbolRefAttr(Builtins::BaseException),
                                                        m_builder.getSymbolRefAttr(Builtins::Object)}));

        auto dict = Py::DictAttr::get(m_builder.getContext(), members);
        m_builder.create<Py::GlobalValueOp>(
            loc, Builtins::BaseException, mlir::StringAttr{}, true,
            Py::ObjectAttr::get(m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Type), dict, llvm::None));
    }
    auto createExceptionSubclass = [&](llvm::Twine name, std::vector<std::string_view> bases)
    {
        std::vector<std::pair<mlir::Attribute, mlir::Attribute>> members;
        members.emplace_back(m_builder.getStringAttr("__new__"),
                             Py::ObjectAttr::get(m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Function),
                                                 noDefaultsFunctionDict,
                                                 m_builder.getSymbolRefAttr(buildFunctionCC(
                                                     loc, name + ".__new__$cc", baseExceptionNew,
                                                     {FunctionParameter{"", FunctionParameter::PosOnly, false},
                                                      FunctionParameter{"", FunctionParameter::PosRest, false}}))));
        std::vector<mlir::Attribute> attr(1 + bases.size());
        attr.front() = m_builder.getSymbolRefAttr(name.str());
        std::transform(bases.begin(), bases.end(), attr.begin(),
                       [this](std::string_view kind) { return m_builder.getSymbolRefAttr(kind); });
        members.emplace_back(m_builder.getStringAttr("__mro__"), Py::TupleAttr::get(m_builder.getContext(), attr));

        auto dict = Py::DictAttr::get(m_builder.getContext(), members);
        m_builder.create<Py::GlobalValueOp>(
            loc, name.str(), mlir::StringAttr{}, true,
            Py::ObjectAttr::get(m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Type), dict, llvm::None));
    };

    createExceptionSubclass(Builtins::Exception, {Builtins::BaseException, Builtins::Object});
    createExceptionSubclass(Builtins::TypeError, {Builtins::Exception, Builtins::BaseException, Builtins::Object});
    createExceptionSubclass(Builtins::NameError, {Builtins::Exception, Builtins::BaseException, Builtins::Object});
    createExceptionSubclass(Builtins::UnboundLocalError,
                            {Builtins::NameError, Builtins::Exception, Builtins::BaseException, Builtins::Object});
    {
        std::vector<std::pair<mlir::Attribute, mlir::Attribute>> members;
        {
            auto newCall = m_builder.create<mlir::FuncOp>(
                loc, "builtins.NoneType.__new__$impl",
                m_builder.getFunctionType({m_builder.getType<Py::DynamicType>(), m_builder.getType<Py::DynamicType>()},
                                          {m_builder.getType<Py::DynamicType>()}));
            mlir::OpBuilder::InsertionGuard guard{m_builder};
            m_builder.setInsertionPointToStart(newCall.addEntryBlock());
            m_currentFunc = newCall;
            // TODO: probably disallow subclassing NoneType here
            m_builder.create<mlir::ReturnOp>(
                loc, mlir::ValueRange{m_builder.create<Py::GetGlobalValueOp>(loc, Builtins::None)});

            members.emplace_back(
                m_builder.getStringAttr("__new__"),
                Py::ObjectAttr::get(m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Function),
                                    noDefaultsFunctionDict,
                                    m_builder.getSymbolRefAttr(
                                        buildFunctionCC(loc, "builtins.NoneType.__new__$cc", newCall,
                                                        {FunctionParameter{"", FunctionParameter::PosOnly, false}}))));
        }
        members.emplace_back(
            m_builder.getStringAttr("__mro__"),
            Py::TupleAttr::get(m_builder.getContext(), {m_builder.getSymbolRefAttr(Builtins::NoneType),
                                                        m_builder.getSymbolRefAttr(Builtins::Object)}));

        auto dict = Py::DictAttr::get(m_builder.getContext(), members);
        m_builder.create<Py::GlobalValueOp>(
            loc, Builtins::NoneType, mlir::StringAttr{}, true,
            Py::ObjectAttr::get(m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Type), dict, llvm::None));
    }
    m_builder.create<Py::GlobalValueOp>(loc, Builtins::None, mlir::StringAttr{}, true,
                                        Py::ObjectAttr::get(m_builder.getContext(),
                                                            m_builder.getSymbolRefAttr(Builtins::NoneType),
                                                            Py::DictAttr::get(m_builder.getContext(), {}), llvm::None));
    {
        std::vector<std::pair<mlir::Attribute, mlir::Attribute>> members;
        members.emplace_back(
            m_builder.getStringAttr("__mro__"),
            Py::TupleAttr::get(m_builder.getContext(), {m_builder.getSymbolRefAttr(Builtins::Function),
                                                        m_builder.getSymbolRefAttr(Builtins::Object)}));

        auto dict = Py::DictAttr::get(m_builder.getContext(), members);
        m_builder.create<Py::GlobalValueOp>(
            loc, Builtins::Function, mlir::StringAttr{}, true,
            Py::ObjectAttr::get(m_builder.getContext(), m_builder.getSymbolRefAttr(Builtins::Type), dict, llvm::None));
    }
}
