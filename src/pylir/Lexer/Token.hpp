
#pragma once

#include <llvm/ADT/APInt.h>

#include <cstdint>
#include <string>
#include <utility>
#include <variant>

namespace pylir
{
enum class TokenType : std::uint8_t
{
    SyntaxError,
    Newline,
    Identifier,
    FalseKeyword,
    NoneKeyword,
    TrueKeyword,
    AndKeyword,
    AsKeyword,
    AssertKeyword,
    AsyncKeyword,
    AwaitKeyword,
    BreakKeyword,
    ClassKeyword,
    ContinueKeyword,
    DefKeyword,
    DelKeyword,
    ElifKeyword,
    ElseKeyword,
    ExceptKeyword,
    FinallyKeyword,
    ForKeyword,
    FromKeyword,
    GlobalKeyword,
    IfKeyword,
    ImportKeyword,
    InKeyword,
    IsKeyword,
    LambdaKeyword,
    NonlocalKeyword,
    NotKeyword,
    OrKeyword,
    PassKeyword,
    RaiseKeyword,
    ReturnKeyword,
    TryKeyword,
    WhileKeyword,
    WithKeyword,
    YieldKeyword,
    StringLiteral,
    BytesLiteral,
    IntegerLiteral,
    FloatingPointLiteral,
    ComplexLiteral,
    Plus,
    Minus,
    Times,
    PowerOf,
    Divide,
    IntDivide,
    Remainder,
    AtSign,
    ShiftLeft,
    ShiftRight,
    BitAnd,
    BitOr,
    BitXor,
    BitNegate,
    Walrus,
    LessThan,
    GreaterThan,
    LessOrEqual,
    GreaterOrEqual,
    Equal,
    NotEqual,
    OpenParentheses,
    CloseParentheses,
    OpenSquareBracket,
    CloseSquareBracket,
    OpenBrace,
    CloseBrace,
    Comma,
    Colon,
    Dot,
    SemiColon,
    Assignment,
    Arrow,
    PlusAssignment,
    MinusAssignment,
    TimesAssignment,
    DivideAssignment,
    IntDivideAssignment,
    RemainderAssignment,
    AtAssignment,
    BitAndAssignment,
    BitOrAssignment,
    BitXorAssignment,
    ShiftRightAssignment,
    ShiftLeftAssignment,
    PowerOfAssignment
};

class Token
{
    int m_offset;
    int m_size;
    int m_fileId;
    TokenType m_tokenType;

public:
    using Variant = std::variant<std::monostate, std::string, llvm::APInt, double>;

private:
    Variant m_value;

public:
    Token(int offset, int size, int fileId, TokenType tokenType, Variant value = {})
        : m_offset(offset), m_size(size), m_fileId(fileId), m_tokenType(tokenType), m_value(std::move(value))
    {
    }

    [[nodiscard]] int getOffset() const
    {
        return m_offset;
    }

    [[nodiscard]] int getSize() const
    {
        return m_size;
    }

    [[nodiscard]] int getFileId() const
    {
        return m_fileId;
    }

    [[nodiscard]] TokenType getTokenType() const
    {
        return m_tokenType;
    }

    [[nodiscard]] const Variant& getValue() const
    {
        return m_value;
    }
};

namespace Diag
{
template <class T, class>
struct LocationProvider;

template <>
struct LocationProvider<Token, void>
{
    static std::size_t getPos(const Token& value) noexcept
    {
        return value.getOffset();
    }

    static std::pair<std::size_t, std::size_t> getRange(const Token& value) noexcept
    {
        return {value.getOffset(), value.getOffset() + value.getSize()};
    }
};
} // namespace Diag

} // namespace pylir
