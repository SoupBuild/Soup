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

        [Theory]
        [InlineData("char", PrimitiveDataType.Char)]
        [InlineData("char16_t", PrimitiveDataType.Char16)]
        [InlineData("char32_t", PrimitiveDataType.Char32)]
        [InlineData("wchar_t", PrimitiveDataType.WChar)]
        [InlineData("bool", PrimitiveDataType.Bool)]
        [InlineData("short", PrimitiveDataType.Short)]
        [InlineData("int", PrimitiveDataType.Int)]
        [InlineData("long", PrimitiveDataType.Long)]
        [InlineData("signed", PrimitiveDataType.Signed)]
        [InlineData("unsigned", PrimitiveDataType.Unsigned)]
        [InlineData("float", PrimitiveDataType.Float)]
        [InlineData("double", PrimitiveDataType.Double)]
        [InlineData("void", PrimitiveDataType.Void)]
        [InlineData("auto", PrimitiveDataType.Auto)]
        public void GlobalPrimitiveVariable(string typeString, PrimitiveDataType type)
        {
            var source = new AntlrInputStream(
                $"{typeString} GlobalVariable = 1;");

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
                                    new PrimitiveDataTypeNode(type),
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

        [Fact]
        public void GlobalTwoVariableDeclarationInitializer()
        {
            var source = new AntlrInputStream(
                @"int GlobalVariable1 = 1, GlobalVariable2 = 2;");

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
                                        Declarator = new Identifier("GlobalVariable1"),
                                        Initializer = new IntegerLiteral(1),
                                    },
                                    new InitializerDeclarator()
                                    {
                                        Declarator = new Identifier("GlobalVariable2"),
                                        Initializer = new IntegerLiteral(2),
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
