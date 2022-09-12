
%top{
module;

// #define SHOW_TOKENS

#pragma warning(disable:4102)
#pragma warning(disable:4189)
#pragma warning(disable:4702)

#include <iostream>
#include <string>

module Soup.Core;

enum class LanguageReferenceToken : int
{
    EndOfFile = 0,
    Name,
    Divider,
    Integer,
    Decimal,
    Unknown,
};

}

%option fast unicode noline nodefault namespace=Soup::Core::LR

name            [A-Za-z][\w#+.]*
divider         \|
integer         \d+
decimal         \.\d+

%%
{name}          return (int)LanguageReferenceToken::Name;
{divider}       return (int)LanguageReferenceToken::Divider;
{integer}       return (int)LanguageReferenceToken::Integer;
{decimal}       return (int)LanguageReferenceToken::Decimal;
.               return (int)LanguageReferenceToken::Unknown;
%%

namespace Soup::Core
{

class LanguageReferenceParser : public LR::Lexer
{
public:
    LanguageReferenceParser(const reflex::Input& input) :
        Lexer(input),
        name(),
        version(0, 1, 0) // TODO: Have a default for now
    {
    }

    bool TryParse()
    {
        // Parse the name
        MoveNext();
        switch (currentToken)
        {
            case LanguageReferenceToken::Name:
                name = text();
                break;
            default:
                return false;
        }

        // Check for optional version
        MoveNext();
        switch (currentToken)
        {
            case LanguageReferenceToken::Divider:
                return TryParseVersion();
            case LanguageReferenceToken::EndOfFile:
                return true;
            default:
                return false;
        }
    }

    LanguageReference GetResult()
    {
        return LanguageReference(std::move(name), std::move(version));
    }

    bool TryParseVersion()
    {
        int major;
        std::optional<int> minor;
        std::optional<int> patch;

        // Parse the major version
        MoveNext();
        switch (currentToken)
        {
            case LanguageReferenceToken::Integer:
                major = std::stoi(text());
                break;
            default:
                return false;
        }

        // Check for optional minor version
        MoveNext();
        switch (currentToken)
        {
            case LanguageReferenceToken::Decimal:
                // Skip the period
                minor = std::stoi(text() + 1);
            case LanguageReferenceToken::EndOfFile:
                version = Opal::SemanticVersion(major, minor, patch);
                return true;
            default:
                return false;
        }

        // Check for optional patch version
        MoveNext();
        switch (currentToken)
        {
            case LanguageReferenceToken::Decimal:
                // Skip the period
                patch = std::stoi(text() + 1);
            case LanguageReferenceToken::EndOfFile:
                version = Opal::SemanticVersion(major, minor, patch);
                return true;
            default:
                return false;
        }

        // Verify end of file
        MoveNext();
        switch (currentToken)
        {
            case LanguageReferenceToken::EndOfFile:
                version = Opal::SemanticVersion(major, minor, patch);
                return true;
            default:
                return false;
        }
    }

private:
    LanguageReferenceToken MoveNext()
    {
        currentToken = (LanguageReferenceToken)lex();

        #ifdef SHOW_TOKENS
            switch (currentToken)
            {
                case LanguageReferenceToken::EndOfFile:
                    std::cout << "Token: " << "EndOfFile" << '\n';
                    break;
                case LanguageReferenceToken::Name:
                    std::cout << "Token: " << "Name" << '\n';
                    break;
                case LanguageReferenceToken::Divider:
                    std::cout << "Token: " << "Divider" << '\n';
                    break;
                case LanguageReferenceToken::Integer:
                    std::cout << "Token: " << "Integer" << '\n';
                    break;
                case LanguageReferenceToken::Decimal:
                    std::cout << "Token: " << "Decimal" << '\n';
                    break;
                case LanguageReferenceToken::Unknown:
                    std::cout << "Token: " << "Unknown" << '\n';
                    break;
                default:
                    std::cout << "Token: " << "UNKNOWN" << '\n';
                    break;
            }
        #endif

        return currentToken;
    }

    LanguageReferenceToken currentToken;
    std::string name;
    Opal::SemanticVersion version;
};

/*static*/ bool LanguageReference::TryParse(const std::string& value, LanguageReference& result)
{
    auto parser = LanguageReferenceParser(value);
    if (parser.TryParse())
    {
      result = parser.GetResult();
      return true;
    }
    else
    {
      return false;
    }
}

}