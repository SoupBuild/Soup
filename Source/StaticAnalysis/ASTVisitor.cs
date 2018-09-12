
using Antlr4.Runtime.Misc;
using Soup.StaticAnalysis.AST;

namespace Soup.StaticAnalysis
{
    /// <summary>
    /// Abstract syntax tree visitor
    /// </summary>
    public class ASTVisitor : CppParserBaseVisitor<Node>
    {
        /// <summary>
        /// Visit function definition
        /// </summary>
        public override Node VisitTranslationUnit([NotNull] CppParser.TranslationUnitContext context)
        {
            var result = new TranslationUnit();

            // Check for the optional declaration sequences
            var declarationSequence = context.declarationSequence();
            if (declarationSequence != null)
            {
                result.Declarations = (DeclarationSequence)Visit(declarationSequence);
            }

            return result;
        }

        /// <summary>
        /// Visit declaration sequence
        /// </summary>
        public override Node VisitDeclarationSequence([NotNull] CppParser.DeclarationSequenceContext context)
        {
            // Handle the recursive rule
            DeclarationSequence sequence;
            var childSequence = context.declarationSequence();
            if (childSequence != null)
            {
                sequence = (DeclarationSequence)Visit(childSequence);
            }
            else
            {
                sequence = new DeclarationSequence();
            }

            // Handle the new item
            var declaration = context.declaration();
            sequence.Declarations.Add((Declaration)Visit(declaration));

            return sequence;
        }

        /// <summary>
        /// Visit function definition
        /// </summary>
        public override Node VisitFunctionDefinition([NotNull] CppParser.FunctionDefinitionContext context)
        {
            Identifier name = null;
            var declarator = Visit(context.declarator());
            if (declarator is Identifier identifier)
            {
                name = identifier;
            }

            return new FunctionDefinition()
            {
                Name = name,
            };
        }

        /// <summary>
        /// Visit unqualified identifier
        /// </summary>
        public override Node VisitUnqualifiedIdentifier([NotNull] CppParser.UnqualifiedIdentifierContext context)
        {
            var identifier = context.Identifier();
            if (identifier != null)
            {
                return new Identifier()
                {
                    Value = identifier.GetText(),
                };
            }

            return base.VisitUnqualifiedIdentifier(context);
        }

        /// <summary>
        /// Visit namespace definition
        /// </summary>
        public override Node VisitIntegerLiteral([NotNull] CppParser.IntegerLiteralContext context)
        {
            return base.VisitIntegerLiteral(context);
        }
    }
}
