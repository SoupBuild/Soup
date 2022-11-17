
%top{
module;

// #define SHOW_TOKENS

#pragma warning(disable:4102)
#pragma warning(disable:4189)
#pragma warning(disable:4702)

# include <iostream>
# include <string>
# include <sstream>

module Soup.Core;

enum class SMLToken : int
{
    EndOfFile = 0,
    Newline,
    KeyLiteral,
    Integer,
    Decimal,
    Colon,
    Comma,
    OpenBracket,
    CloseBracket,
    OpenBrace,
    CloseBrace,
    StringLiteral,
    True,
    False,
    Error,
};

}

%class{
private:
    // wide string to accumulate string literal
    std::string _stringLiteral;

protected:
    std::string GetString()
    {
        return std::move(_stringLiteral);
    }

private:
    void ClearString()
    {
        _stringLiteral.clear();
    }

    void AddCharacter(char value)
    {
        _stringLiteral.push_back(value);
    }

    void AddString(const char* value, size_t size)
    {
        _stringLiteral.append(value, size);
    }
}

%option fast unicode noline nodefault namespace=Soup::Core::SML

// BasicCharacter: 
//      Tab Character, Letter, Number, Marks, Punctuation, Symbols, Separators
//      except "Quotation Mark", "Reverse Solidus"
//      (Everything except controls)

// KeyCharacter:
//      Letter, Numbers
// TODO: Remove extra symbols

%x QUOTE
basic_char      [\t\p{L}\p{N}\p{M}\p{P}\p{S}\p{Z}]{-}["\\]
whitespace      [ \t]+
newline         \r?\n
comment         "#".*?\r?\n
key             [\p{L}\p{N}]+
integer         \d+
decimal         \.\d+
basic_string    {basic_char}+

%%
{whitespace}    { /* ignore whitespace */ }
{comment}       { /* ignore comments */ }
{newline}       return (int)SMLToken::Newline;
{integer}       return (int)SMLToken::Integer;
"true"          return (int)SMLToken::True;
"false"         return (int)SMLToken::False;
{key}           return (int)SMLToken::KeyLiteral;
{decimal}       return (int)SMLToken::Decimal;
":"             return (int)SMLToken::Colon;
"["             return (int)SMLToken::OpenBracket;
"]"             return (int)SMLToken::CloseBracket;
"{"             return (int)SMLToken::OpenBrace;
"}"             return (int)SMLToken::CloseBrace;
","             return (int)SMLToken::Comma;
\"              { ClearString(); start(QUOTE); }
.               return (int)SMLToken::Error;

<QUOTE>{
\"              { start(INITIAL); return (int)SMLToken::StringLiteral; }
\\0             { AddCharacter('\0'); }
\\t             { AddCharacter('\t'); } // tab              (U+0009)
\\n             { AddCharacter('\n'); } // linefeed         (U+000A)
\\f             { AddCharacter('\f'); } // form feed        (U+000C)
\\r             { AddCharacter('\r'); } // carriage return  (U+000D)
\\e             { AddCharacter('\r'); } // escape           (U+001B)
\\\"            { AddCharacter('"'); }  // quote            (U+0022)
\\\\            { AddCharacter('\\'); } // backslash        (U+005C)
{basic_string}  { AddString(text(), size()); }
.               { AddString(text(), size()); }
}
%%

namespace Soup::Core
{

/// <summary>
/// Gradient decent SML parser
/// </summary>
class SMLParser : public SML::Lexer
{
public:
    SMLParser(const reflex::Input& input) :
        Lexer(input),
        _currentToken(),
        _root()
    {
    }

    bool TryParse()
    {
        std::unordered_map<std::string, SMLValue> table;
        if (TryParseTableContents(table))
        {
            // Verify we are at the end of the content
            if (_currentToken != SMLToken::EndOfFile)
                return false;

            _root = SMLTable(std::move(table));
            return true;
        }
        else
        {
            return false;
        }
    }

    SMLDocument GetResult()
    {
        return SMLDocument(std::move(_root));
    }

private:
    bool TryParseTable(SMLTable& table)
    {
        std::unordered_map<std::string, SMLValue> tableValues;
        if (TryParseTableContents(tableValues))
        {
            // Verify we are at the end of the content
            if (_currentToken != SMLToken::CloseBrace)
                return false;

            table = SMLTable(std::move(tableValues));
            return true;
        }
        else
        {
            return false;
        }
    }

    bool TryParseTableContents(std::unordered_map<std::string, SMLValue>& tableValues)
    {
        // Odd move next to allow for optional extra delimiter checks at end
        MoveNext();

        // Allow zero or more newlines at the start of a table
        while (_currentToken == SMLToken::Newline)
        {
            MoveNext();
        }

        // Check for the optional first value
        bool hasValue;
        std::string key;
        SMLValue tableValue;
        if (!TryParseTableValue(hasValue, key, tableValue))
            return false;

        // Let the caller verify the end token is correct when zero values
        if (!hasValue)
            return true;

        tableValues.emplace(std::move(key), std::move(tableValue));

        // Check for zero or more optional values
        while (true)
        {
            // Check for trailing delimiter
            bool hasDelimiter;
            bool isComma;
            CheckDelimiter(hasDelimiter, isComma);

            // Let the caller verify the end token is correct when zero values
            if (!hasDelimiter)
                return true;

            if (!TryParseTableValue(hasValue, key, tableValue))
                return false;

            if (!hasValue)
            {
                // If a comma was used then the next value is required
                if (isComma)
                    return false;
                else
                    return true;
            }

            tableValues.emplace(std::move(key), std::move(tableValue));
        }
    }

    bool TryParseTableValue(bool& hasValue, std::string& key, SMLValue& tableValue)
    {
        // Parse the next value
        // Note: The delimiter check will read the first token of next item
        switch (_currentToken)
        {
            case SMLToken::Integer:
                // Integer is a special case of Key and should be allowed
            case SMLToken::KeyLiteral:
            {
                // Key token already matched 
                key = str();

                // Verify match assign
                MoveNext();
                if (_currentToken != SMLToken::Colon)
                    return false;

                // Parse the value
                MoveNext();
                bool internalHasValue;
                if (!TryParseValue(internalHasValue, tableValue))
                    return false;

                // The value is required here
                if (!internalHasValue)
                    return false;

                hasValue = true;
                return true;
            }
            case SMLToken::StringLiteral:
            {
                // Key token already matched
                key = GetString();

                // Verify match assign
                MoveNext();
                if (_currentToken != SMLToken::Colon)
                    return false;

                // Parse the value
                MoveNext();
                bool internalHasValue;
                if (!TryParseValue(internalHasValue, tableValue))
                    return false;

                // The value is required here
                if (!internalHasValue)
                    return false;

                hasValue = true;
                return true;
            }
            default:
            {
                // Caller will verify final token
                hasValue = false;
                return true;
            }
        }
    }

    bool TryParseArray(SMLArray& array)
    {
        std::vector<SMLValue> arrayValues;
        if (TryParseArrayContent(arrayValues))
        {
            // Verify we are at the end of the content
            if (_currentToken != SMLToken::CloseBracket)
                return false;

            array = SMLArray(std::move(arrayValues));
            return true;
        }
        else
        {
            return false;
        }
    }

    bool TryParseArrayContent(std::vector<SMLValue>& arrayValues)
    {
        // Odd move next to allow for optional extra delimiter checks at end
        MoveNext();

        // Allow zero or more newlines at the start of a table
        while (_currentToken == SMLToken::Newline)
        {
            MoveNext();
        }

        // Check for the optional first value
        bool hasValue;
        SMLValue value;
        if (!TryParseValue(hasValue, value))
            return false;

        // Let the caller verify the end token is correct when zero values
        if (!hasValue)
            return true;

        arrayValues.push_back(std::move(value));

        while (true)
        {
            // Check for trailing delimiter
            bool hasDelimiter;
            bool isComma;
            CheckDelimiter(hasDelimiter, isComma);

            // Let the caller verify the end token is correct when zero values
            if (!hasDelimiter)
                return true;

            if (!TryParseValue(hasValue, value))
                return false;

            if (!hasValue)
            {
                // If a comma was used then the next value is required
                if (isComma)
                    return false;
                else
                    return true;
            }

            arrayValues.push_back(std::move(value));
        }
    }

    bool TryParseValue(bool& hasValue, SMLValue& value)
    {
        // Check the type of the value
        switch (_currentToken)
        {
            case SMLToken::StringLiteral:
            {
                auto string = GetString();
                value = SMLValue(std::move(string));
                hasValue = true;
                return true;
            }
            case SMLToken::Integer:
            {
                auto integer = std::stoi(text());
                value = SMLValue(integer);
                hasValue = true;
                return true;
            }
            case SMLToken::True:
            {
                value = SMLValue(true);
                hasValue = true;
                return true;
            }
            case SMLToken::False:
            {
                value = SMLValue(false);
                hasValue = true;
                return true;
            }
            case SMLToken::OpenBrace:
            {
                SMLTable table;
                if (!TryParseTable(table))
                    return false;

                value = SMLValue(std::move(table));
                hasValue = true;
                return true;
            }
            case SMLToken::OpenBracket:
            {
                SMLArray array;
                if (!TryParseArray(array))
                    return false;

                value = SMLValue(std::move(array));
                hasValue = true;
                return true;
            }
            default:
            {
                // We didn't see a value, let the caller continue if possible
                hasValue = false;
                return true;
            }
        }
    }

    void CheckDelimiter(bool& hasDelimiter, bool& isComma)
    {
        MoveNext();
        switch (_currentToken)
        {
            case SMLToken::Comma:
            {
                // Move next to match multiple newline delimiter result
                MoveNext();
                hasDelimiter = true;
                isComma = true;
                break;
            }
            case SMLToken::Newline:
            {
                // Newline delimiter is one or more
                while (_currentToken == SMLToken::Newline)
                {
                    MoveNext();
                }

                hasDelimiter = true;
                isComma = false;
                break;
            }
            default:
            {
                hasDelimiter = false;
                isComma = false;
                break;
            }
        }
    }

    SMLToken MoveNext()
    {
        _currentToken = (SMLToken)lex();

        #ifdef SHOW_TOKENS
            switch (_currentToken)
            {
                case SMLToken::EndOfFile:
                    std::cout << "Token: " << "EndOfFile" << '\n';
                    break;
                case SMLToken::Newline:
                    std::cout << "Token: " << "Newline" << '\n';
                    break;
                case SMLToken::Key:
                    std::cout << "Token: " << "Key" << '\n';
                    break;
                case SMLToken::Integer:
                    std::cout << "Token: " << "Integer" << '\n';
                    break;
                case SMLToken::Decimal:
                    std::cout << "Token: " << "Decimal" << '\n';
                    break;
                case SMLToken::Colon:
                    std::cout << "Token: " << "Colon" << '\n';
                    break;
                case SMLToken::Comma:
                    std::cout << "Token: " << "Comma" << '\n';
                    break;
                case SMLToken::OpenBracket:
                    std::cout << "Token: " << "OpenBracket" << '\n';
                    break;
                case SMLToken::CloseBracket:
                    std::cout << "Token: " << "CloseBracket" << '\n';
                    break;
                case SMLToken::OpenBrace:
                    std::cout << "Token: " << "OpenBrace" << '\n';
                    break;
                case SMLToken::CloseBrace:
                    std::cout << "Token: " << "CloseBrace" << '\n';
                    break;
                case SMLToken::StringLiteral:
                    std::cout << "Token: " << "StringLiteral" << '\n';
                    break;
                case SMLToken::True:
                    std::cout << "Token: " << "True" << '\n';
                    break;
                case SMLToken::False:
                    std::cout << "Token: " << "False" << '\n';
                    break;
                case SMLToken::Error:
                    std::cout << "Token: " << "Error" << '\n';
                    break;
                default:
                    std::cout << "Token: " << "UNKNOWN" << '\n';
                    break;
            }
        #endif

        return _currentToken;
    }

    SMLToken _currentToken;
    SMLTable _root;
};

/*static*/ SMLDocument SMLDocument::Parse(std::istream& stream)
{
    auto parser = SMLParser(stream);
    if (parser.TryParse())
    {
        return parser.GetResult();
    }
    else
    {
        auto line = parser.lineno();
        auto column = parser.columno();
        auto text = parser.text();

        std::stringstream message;
        message << "Failed to parse at " << line << ":" << column << " " << text;
        throw std::runtime_error(message.str());
    }
}

}