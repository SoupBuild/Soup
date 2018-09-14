using Antlr4.Runtime;
using Soup.StaticAnalysis.AST;
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
    }
}
