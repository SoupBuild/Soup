using Antlr4.Runtime;
using System.IO;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class FileTests
    {
        [Fact]
        public void BlockComment()
        {
            RunTest(@"Examples/BlockComment.cpp");
        }

        [Fact]
        public void HelloWorld()
        {
            RunTest(@"Examples/HelloWorld.cpp");
        }

        [Fact]
        public void LineComment()
        {
            RunTest(@"Examples/LineComment.cpp");
        }

        [Fact]
        public void Namespace()
        {
            RunTest(@"Examples/Namespace.cpp");
        }

        private void RunTest(string filename)
        {
            using (var file = File.OpenRead(filename))
            {
                var inputSource = new AntlrInputStream(file);
                var ast = TestUtils.GenerateAST(inputSource);
                Assert.NotNull(ast);
            }
        }
    }
}
