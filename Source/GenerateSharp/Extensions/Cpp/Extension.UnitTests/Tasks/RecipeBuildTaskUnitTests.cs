// <copyright file="RecipeBuildTaskUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Cpp.UnitTests
{
    public class RecipeBuildTaskUnitTests
    {
        [Fact]
        public void Initialize_Success()
        {
            var buildState = new MockBuildState();
            var uut = new RecipeBuildTask(buildState);
        }

        [Fact]
        public void Build_Executable()
        {
            // Register the test systems
            var testListener = new TestTraceListener();
            using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
            {
                // Setup the input build state
                var buildState = new MockBuildState();
                var state = buildState.ActiveState;
                state.Add("PackageRoot", new Value("C:/PackageRoot/"));
                state.Add("BuildFlavor", new Value("debug"));
                state.Add("PlatformLibraries", new Value(new ValueList()));
                state.Add("PlatformIncludePaths", new Value(new ValueList()));
                state.Add("PlatformLibraryPaths", new Value(new ValueList()));
                state.Add("PlatformPreprocessorDefinitions", new Value(new ValueList()));

                var uut = new RecipeBuildTask(buildState);

                uut.Execute();
 
                // Verify expected logs
                Assert.Equal(
                    new List<string>()
                    {
                    },
				    testListener.GetMessages());

                // Verify build state
                var expectedBuildOperations = new List<BuildOperation>();

                Assert.Equal(
                    expectedBuildOperations,
                    buildState.GetBuildOperations());

                // TODO: Verify output build state
            }
        }
    }
}
