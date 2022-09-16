
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
    Key,
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
//      Letter, Numbers, (TEMP: + #)
// TODO: Remove extra symbols

%x QUOTE
basic_char      [\t\p{L}\p{N}\p{M}\p{P}\p{S}\p{Z}]{-}["\\]
whitespace      [ \t]+
newline         \r?\n
comment         "//".*?\r?\n
key             [\p{L}\p{N}\+#]+
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
{key}           return (int)SMLToken::Key;
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
    bool TryParseTableContents(std::unordered_map<std::string, SMLValue>& tableValue)
    {
        // Odd move next to allow for optional extra delimiter checks at end
        MoveNext();

        // Allow zero or more newlines at the start of a table
        while (_currentToken == SMLToken::Newline)
        {
            MoveNext();
        }

        while (true)
        {
            // Parse the next value
            // Note: The delimiter check will read the first token of next item
            switch (_currentToken)
            {
                case SMLToken::Integer:
                    // Integer is a special case of Key and should be allowed
                case SMLToken::Key:
                {
                    // Key token already matched 
                    auto key = str();

                    // Verify match assign
                    MoveNext();
                    if (_currentToken != SMLToken::Colon)
                        return false;

                    // Parse the value
                    MoveNext();
                    SMLValue value;
                    if (!TryParseValue(value))
                        return false;

                    tableValue.emplace(std::move(key), std::move(value));

                    // Check for trailing delimiter
                    if (!TryParseDelimiter())
                        return false;

                    break;
                }
                default:
                {
                    // Caller will verify final token
                    return true;
                }
            }
        }
    }

    bool TryParseArray(std::vector<SMLValue>& arrayValue)
    {
        // Odd move next to allow for optional extra delimiter checks at end
        MoveNext();

        // Allow zero or more newlines at the start of a table
        while (_currentToken == SMLToken::Newline)
        {
            MoveNext();
        }

        while (true)
        {
            // Parse the value if not at end
            // Note: The delimiter check will read the first token of next item
            switch (_currentToken)
            {
                case SMLToken::CloseBracket:
                {
                    // At the end of the array
                    return true;
                }
                default:
                {
                    // Assume that it is a value
                    SMLValue value;
                    if (!TryParseValue(value))
                        return false;

                    arrayValue.push_back(std::move(value));

                    // Check for trailing delimiter
                    if (!TryParseDelimiter())
                        return false;

                    // Check if end of array
                    if (_currentToken == SMLToken::CloseBrace)
                            return true;
                }
            }
        }
    }

    bool TryParseValue(SMLValue& value)
    {
        // Check the type of the value
        switch (_currentToken)
        {
            case SMLToken::StringLiteral:
            {
                auto string = GetString();
                value = SMLValue(std::move(string));
                return true;
            }
            case SMLToken::Integer:
            {
                auto integer = std::stoi(text());
                value = SMLValue(integer);
                return true;
            }
            case SMLToken::True:
            {
                value = SMLValue(true);
                return true;
            }
            case SMLToken::False:
            {
                value = SMLValue(false);
                return true;
            }
            case SMLToken::OpenBrace:
            {
                std::unordered_map<std::string, SMLValue> tableValue;
                if (TryParseTableContents(tableValue))
                {
                    // Verify we are at the end of the content
                    if (_currentToken != SMLToken::CloseBrace)
                        return false;

                    value = SMLValue(SMLTable(std::move(tableValue)));
                    return true;
                }
                else
                {
                    return false;
                }
            }
            case SMLToken::OpenBracket:
            {
                std::vector<SMLValue> arrayValue;
                if (TryParseArray(arrayValue))
                {
                    value = SMLValue(SMLArray(std::move(arrayValue)));
                    return true;
                }
                else
                {
                    return false;
                }
            }
            default:
            {
                return false;
            }
        }
    }

    bool TryParseDelimiter()
    {
        MoveNext();
        switch (_currentToken)
        {
            case SMLToken::Comma:
            {
                // Move next to match multiple newline delimiter result
                MoveNext();
                return true;
            }
            case SMLToken::Newline:
            {
                // Newline delimiter is one or more
                while (_currentToken == SMLToken::Newline)
                {
                    MoveNext();
                }

                return true;
            }
            default:
            {
                // Some form of delimiter is required
                return false;
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