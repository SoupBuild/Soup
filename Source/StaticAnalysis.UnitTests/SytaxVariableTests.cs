using Antlr4.Runtime;
using Soup.StaticAnalysis.AST;
using System.Collections.Generic;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class SyntaxVariableTests
    {
        [Fact]
        public void GlobalVariableDeclaration()
        {
            var source = new AntlrInputStream(
                @"int GlobalVariable;");

            var expected = new TranslationUnit()
            {
                Declarations = new DeclarationSequence()
                {
                    Declarations = new List<Declaration>()
                    {
                        new SimpleDefinition()
                        {
                            DeclarationSpecifierSequence = new DeclarationSpecifierSequence(
                                new List<Node>()
                                {
                                    new PrimitiveDataTypeNode(PrimitiveDataType.Int),
                                }),
                            InitializerDeclaratorList = new InitializerDeclaratorList(
                                new List<InitializerDeclarator>()
                                {
                                    new InitializerDeclarator()
                                    {
                                        Declarator = new Identifier("GlobalVariable"),
                                    },
                                }),
                        }
                    },
                },
            };

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }

        [Fact]
        public void GlobalVariableDeclarationInitializer()
        {
            var source = new AntlrInputStream(
                @"int GlobalVariable = 1;");

            var expected = new TranslationUnit()
            {
                Declarations = new DeclarationSequence()
                {
                    Declarations = new List<Declaration>()
                    {
                        new SimpleDefinition()
                        {
                            DeclarationSpecifierSequence = new DeclarationSpecifierSequence(
                                new List<Node>()
                                {
                                    new PrimitiveDataTypeNode(PrimitiveDataType.Int),
                                }),
                            InitializerDeclaratorList = new InitializerDeclaratorList(
                                new List<InitializerDeclarator>()
                                {
                                    new InitializerDeclarator()
                                    {
                                        Declarator = new Identifier("GlobalVariable"),
                                        Initializer = new IntegerLiteral(1),
                                    },
                                }),
                        },
                    },
                },
            };

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }
    }
}
