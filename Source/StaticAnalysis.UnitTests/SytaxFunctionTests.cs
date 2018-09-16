using Antlr4.Runtime;
using Soup.StaticAnalysis.AST;
using System.Collections.Generic;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class SyntaxTreeTests
    {
        [Fact]
        public void SimpleFunctionRegular()
        {
            var source = new AntlrInputStream(
                @"void Function(){}");

            var expected = new TranslationUnit()
            {
                Declarations = new DeclarationSequence()
                {
                    Declarations = new List<Declaration>()
                    {
                        new FunctionDefinition()
                        {
                            ReturnType = new DeclarationSpecifierSequence()
                            {
                                Specifiers = new List<Node>()
                                {
                                    new PrimitiveDataTypeNode(PrimitiveDataType.Void),
                                }
                            },
                            Identifier = new Identifier("Function"),
                            ParameterList = null,
                            Body = new RegularFunctionBody()
                            {
                                Statements = new CompoundStatement(),
                            },
                        },
                    },
                },
            };

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }

        [Fact]
        public void SimpleFunctionDefault()
        {
            var source = new AntlrInputStream(
                @"void Function() = default;");

            var expected = new TranslationUnit()
            {
                Declarations = new DeclarationSequence()
                {
                    Declarations = new List<Declaration>()
                    {
                        new FunctionDefinition()
                        {
                            ReturnType = new DeclarationSpecifierSequence()
                            {
                                Specifiers = new List<Node>()
                                {
                                    new PrimitiveDataTypeNode(PrimitiveDataType.Void),
                                }
                            },
                            Identifier = new Identifier("Function"),
                            ParameterList = null,
                            Body = new DefaultFunctionBody(),
                        },
                    },
                },
            };

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }

        [Fact]
        public void SimpleFunctionDelete()
        {
            var source = new AntlrInputStream(
                @"void Function() = delete;");

            var expected = new TranslationUnit()
            {
                Declarations = new DeclarationSequence()
                {
                    Declarations = new List<Declaration>()
                    {
                        new FunctionDefinition()
                        {
                            ReturnType = new DeclarationSpecifierSequence()
                            {
                                Specifiers = new List<Node>()
                                {
                                    new PrimitiveDataTypeNode(PrimitiveDataType.Void),
                                }
                            },
                            Identifier = new Identifier("Function"),
                            ParameterList = null,
                            Body = new DeleteFunctionBody(),
                        },
                    },
                },
            };

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }
    }
}
