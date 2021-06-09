
#pragma once

#include <pylir/Lexer/Token.hpp>

#include <variant>

namespace pylir::Syntax
{
struct Expression;

template <class T>
struct CommaList
{
    std::unique_ptr<T> firstExpr;
    std::vector<std::pair<Token, std::unique_ptr<T>>> remainingExpr;
    std::optional<Token> trailingComma;
};

using ExpressionList = CommaList<Expression>;

struct Enclosure;

struct Atom
{
    struct Literal
    {
        Token token;
    };

    struct Identifier
    {
        Token token;
    };

    std::variant<Literal, Identifier, std::unique_ptr<Enclosure>> variant;
};

struct Primary;

struct AttributeRef
{
    std::unique_ptr<Primary> primary;
    Token dot;
    Token identifier;
};

struct Subscription
{
    std::unique_ptr<Primary> primary;
    Token openSquareBracket;
    ExpressionList expressionList;
    Token closeSquareBracket;
};

struct Slicing
{
    std::unique_ptr<Primary> primary;
    struct ProperSlice
    {
        std::unique_ptr<Expression> optionalLowerBound;
        Token firstColon;
        std::unique_ptr<Expression> optionalUpperBound;
        Token secondColon;
        std::unique_ptr<Expression> optionalStride;
    };
    CommaList<std::variant<ProperSlice, std::unique_ptr<Expression>>> sliceList;
};

struct Comprehension;

struct AssignmentExpression;

struct Call
{
    struct PositionalItem
    {
        struct Star
        {
            Token asterisk;
            std::unique_ptr<Expression> expression;
        };
        std::variant<std::unique_ptr<AssignmentExpression>, Star> variant;
    };

    struct PositionalArguments
    {
        PositionalItem firstItem;
        std::vector<std::pair<Token, PositionalItem>> rest;
    };

    struct KeywordItem
    {
        Token identifier;
        Token assignmentOperator;
        std::unique_ptr<Expression> expression;
    };

    struct StarredAndKeywords
    {
        struct Expression
        {
            Token asterisk;
            std::unique_ptr<Expression> expression;
        };
        using Variant = std::variant<KeywordItem, Expression>;
        Variant first;
        std::vector<std::pair<Token, Variant>> rest;
    };

    struct KeywordArguments
    {
        struct Expression
        {
            Token doubleAsterisk;
            std::unique_ptr<Expression> expression;
        };
        using Variant = std::variant<KeywordItem, Expression>;
        Variant first;
        std::vector<std::pair<Token, Variant>> rest;
    };

    struct ArgumentList
    {
        PositionalArguments positionalArguments;
        std::optional<Token> firstComma;
        std::optional<StarredAndKeywords> starredAndKeywords;
        std::optional<Token> secondComma;
        std::optional<KeywordArguments> keywordArguments;
    };

    std::unique_ptr<Primary> primary;
    Token openParentheses;
    std::variant<std::monostate, std::pair<ArgumentList, std::optional<Token>>, std::unique_ptr<Comprehension>> variant;
    Token closeParentheses;
};

struct Primary
{
    std::variant<Atom, AttributeRef, Subscription, Slicing, Call> variant;
};

struct AwaitExpr
{
    Token awaitToken;
    Primary primary;
};

struct UExpr;

struct Power
{
    std::variant<AwaitExpr, Primary> variant;
    std::optional<std::pair<Token, std::unique_ptr<UExpr>>> rightHand;
};

struct UExpr
{
    std::variant<Power, std::pair<Token, std::unique_ptr<UExpr>>> variant;
};

struct MExpr
{
    struct AtBin
    {
        std::unique_ptr<MExpr> lhs;
        Token atToken;
        std::unique_ptr<MExpr> rhs;
    };

    struct BinOp
    {
        std::unique_ptr<MExpr> lhs;
        Token binToken;
        UExpr rhs;
    };

    std::variant<UExpr, AtBin, BinOp> variant;
};

struct AExpr
{
    struct BinOp
    {
        std::unique_ptr<AExpr> lhs;
        Token binToken;
        MExpr rhs;
    };

    std::variant<MExpr, BinOp, BinOp> variant;
};

struct ShiftExpr
{
    struct BinOp
    {
        std::unique_ptr<ShiftExpr> lhs;
        Token binToken;
        AExpr rhs;
    };

    std::variant<AExpr, BinOp> variant;
};

struct AndExpr
{
    struct BinOp
    {
        std::unique_ptr<AndExpr> lhs;
        Token bitAndToken;
        ShiftExpr rhs;
    };

    std::variant<ShiftExpr, BinOp> variant;
};

struct XorExpr
{
    struct BinOp
    {
        std::unique_ptr<XorExpr> lhs;
        Token bitXorToken;
        AndExpr rhs;
    };

    std::variant<AndExpr, BinOp> variant;
};

struct OrExpr
{
    struct BinOp
    {
        std::unique_ptr<OrExpr> lhs;
        Token bitOrToken;
        XorExpr rhs;
    };

    std::variant<XorExpr, BinOp> variant;
};

struct Comparison
{
    OrExpr left;
    struct Operator
    {
        Token firstToken;
        std::optional<Token> secondToken;
    };
    std::vector<std::pair<Operator, OrExpr>> rest;
};

struct NotTest
{
    std::variant<Comparison, std::pair<Token, std::unique_ptr<NotTest>>> variant;
};

struct AndTest
{
    struct BinOp
    {
        std::unique_ptr<AndExpr> lhs;
        Token andToken;
        NotTest rhs;
    };

    std::variant<NotTest, BinOp> variant;
};

struct OrTest
{
    struct BinOp
    {
        std::unique_ptr<OrExpr> lhs;
        Token orToken;
        AndTest rhs;
    };

    std::variant<AndTest, BinOp> variant;
};

struct AssignmentExpression
{
    std::optional<std::pair<Token, Token>> identifierAndWalrus;
    std::unique_ptr<Expression> expression;
};

struct ConditionalExpression
{
    OrTest value;
    struct Suffix
    {
        Token ifToken;
        OrTest test;
        Token elseToken;
        std::unique_ptr<Expression> elseValue;
    };
    std::optional<Suffix> suffix;
};

struct LambdaExpression;

struct Expression
{
    std::variant<ConditionalExpression, std::unique_ptr<LambdaExpression>> variant;
};

struct ParameterList;

struct LambdaExpression
{
    Token lambdaToken;
    std::unique_ptr<ParameterList> parameterList;
    Token colonToken;
    Expression expression;
};



} // namespace pylir::Syntax
