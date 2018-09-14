using Antlr4.Runtime;
using Soup.StaticAnalysis.AST;
using System.Collections.Generic;
using System.IO;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class SyntaxTreeTests
    {
        [Fact]
        public void EmptyFile()
        {
            var source = new AntlrInputStream(
                @"");

            var expected = new TranslationUnit();

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }

        [Fact]
        public void SimpleFunction()
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
                            ReturnType = null,
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
    }
}
