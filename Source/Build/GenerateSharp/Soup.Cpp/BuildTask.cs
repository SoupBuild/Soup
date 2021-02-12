

using System;

namespace Soup.Build
{
    public class BuildTask : IBuildTask
    {
        public void Execute()
        {
            Console.WriteLine("Do it!");
        }
    }
}
