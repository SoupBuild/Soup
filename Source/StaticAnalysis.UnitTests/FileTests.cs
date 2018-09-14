using Antlr4.Runtime;
using System.IO;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class FileTests
    {
        [Fact(Skip="notready")]
        public void BlockComment()
        {
            RunTest(@"Examples/BlockComment.cpp");
        }

        [Fact(Skip = "notready")]
        public void HelloWorld()
        {
            RunTest(@"Examples/HelloWorld.cpp");
        }

        [Fact(Skip = "notready")]
        public void LineComment()
        {
            RunTest(@"Examples/LineComment.cpp");
        }

        [Fact(Skip = "notready")]
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
