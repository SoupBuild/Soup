using Antlr4.Runtime;
using Soup.StaticAnalysis.AST;
using System.Collections.Generic;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class SyntaxNamespaceTests
    {
        [Fact]
        public void SimpleNamedNamespace()
        {
            var source = new AntlrInputStream(
                @"namespace MyNamespace{}");

            var expected = TestUtils.CreateSingleDeclaration(
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
                });

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }

        [Fact]
        public void SimpleUnnamedNamespace()
        {
            var source = new AntlrInputStream(
                @"namespace{}");

            var expected = TestUtils.CreateSingleDeclaration(
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
                });

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }

        [Fact]
        public void SimpleNestedNamespace()
        {
            var source = new AntlrInputStream(
                @"namespace MyNamespace::MySubNamespace{}");

            var expected = TestUtils.CreateSingleDeclaration(
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
                });

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }
    }
}
