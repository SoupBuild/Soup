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
                            Specifiers = new DeclarationSpecifierSequence()
                            {
                                Specifiers = new List<Node>()
                                {
                                    new PrimitiveDataTypeNode(PrimitiveDataType.Int),
                                }
                            }
                        }
                    },
                },
            };

            var actual = TestUtils.GenerateAST(source);
            Assert.Equal(expected, actual);
        }
    }
}
