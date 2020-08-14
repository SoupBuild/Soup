# Simple Build Extension
This is a console application that has a custom build extension that alters the build state.

[Source](https://github.com/mwasplund/Soup/tree/master/Samples/SimpleBuildExtension)

## Extension/Recipe.toml
The Recipe file that defines the build extension dynamic library "SimpleBuildExtension" that will register two new build tasks.
```
Name = "SimpleBuildExtension"
Type = "DynamicLibrary"
Version = "1.0.0"
Dependencies = [
    "Opal@0.1.3",
    "Soup.Build@0.3.0",
    "Soup.Build.Utilities@0.3.0",
]

Source = [
    "Register.cpp"
]
```

## Extension/AfterBuildTask.h
A header file defining a custom build Task that will run after the build definition and simply log a message to the console.
```
#pragma once

namespace SimpleBuildExtension
{
    /// <summary>
    /// The simple build task that will run after the main build task
    /// </summary>
    class AfterBuildTask : public Opal::Memory::ReferenceCounted<Soup::Build::IBuildTask>
    {
    public:
        /// <summary>
        /// Get the task name
        /// </summary>
        const char* GetName() const noexcept override final
        {
            return "AfterBuild";
        }

        /// <summary>
        /// Get the run before list
        /// </summary>
        const Soup::Build::IReadOnlyList<const char*>& GetRunBeforeList() const noexcept override final
        {
            return _runBeforeList;
        }

        /// <summary>
        /// Get the run after list
        /// </summary>
        const Soup::Build::IReadOnlyList<const char*>& GetRunAfterList() const noexcept override final
        {
            return _runAfterList;
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        Soup::Build::ApiCallResult TryExecute(
            Soup::Build::IBuildState& buildState) noexcept override final
        {
            auto buildStateWrapper = Soup::Build::Utilities::BuildStateWrapper(buildState);

            try
            {
                // We cannot throw accross the DLL boundary for a build extension
                // So all internal errors must be converted to error codes
                Execute(buildStateWrapper);
                return Soup::Build::ApiCallResult::Success;
            }
            catch(...)
            {
                buildStateWrapper.LogError("Unknown Error!");
                return Soup::Build::ApiCallResult::Error;
            }
        }

    private:
        /// <summary>
        /// The Core Execute task
        /// </summary>
        void Execute(Soup::Build::Utilities::BuildStateWrapper& buildState)
        {
            buildState.LogHighPriority("Running After Build!");
        }

    private:
        static Soup::Build::Utilities::StringList _runBeforeList;
        static Soup::Build::Utilities::StringList _runAfterList;
    };

    Soup::Build::Utilities::StringList AfterBuildTask::_runBeforeList =
        Soup::Build::Utilities::StringList();
    Soup::Build::Utilities::StringList AfterBuildTask::_runAfterList =
        Soup::Build::Utilities::StringList({
            "Build",
        });
}
```

## Extension/BeforeBuildTask.h
A header file defining a custom build Task that will run before the build definition and sets a custom preprocessor definition to show how a user can alter the build state through an extension.
```
#pragma once

namespace SimpleBuildExtension
{
    /// <summary>
    /// The simple build task that will run before the main build task
    /// </summary>
    class BeforeBuildTask : public Opal::Memory::ReferenceCounted<Soup::Build::IBuildTask>
    {
    public:
        /// <summary>
        /// Get the task name
        /// </summary>
        const char* GetName() const noexcept override final
        {
            return "BeforeBuild";
        }

        /// <summary>
        /// Get the run before list
        /// </summary>
        const Soup::Build::IReadOnlyList<const char*>& GetRunBeforeList() const noexcept override final
        {
            return _runBeforeList;
        }

        /// <summary>
        /// Get the run after list
        /// </summary>
        const Soup::Build::IReadOnlyList<const char*>& GetRunAfterList() const noexcept override final
        {
            return _runAfterList;
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        Soup::Build::ApiCallResult TryExecute(
            Soup::Build::IBuildState& buildState) noexcept override final
        {
            auto buildStateWrapper = Soup::Build::Utilities::BuildStateWrapper(buildState);

            try
            {
                // We cannot throw accross the DLL boundary for a build extension
                // So all internal errors must be converted to error codes
                Execute(buildStateWrapper);
                return Soup::Build::ApiCallResult::Success;
            }
            catch(...)
            {
                buildStateWrapper.LogError("Unknown Error!");
                return Soup::Build::ApiCallResult::Error;
            }
        }

    private:
        /// <summary>
        /// The Core Execute task
        /// </summary>
        void Execute(Soup::Build::Utilities::BuildStateWrapper& buildState)
        {
            buildState.LogHighPriority("Running Before Build!");
            auto rootTable = buildState.GetActiveState();
            auto buildTable = rootTable.EnsureValue("Build").EnsureTable();

            // As a sample of what a build extension can do we set a new
            // preprocessor definition to influence the build
            auto preprocessorDefinitions = std::vector<std::string>({
                "SPECIAL_BUILD",
            });
            buildTable.EnsureValue("PreprocessorDefinitions").EnsureList().Append(preprocessorDefinitions);
        }

    private:
        static Soup::Build::Utilities::StringList _runBeforeList;
        static Soup::Build::Utilities::StringList _runAfterList;
    };

    Soup::Build::Utilities::StringList BeforeBuildTask::_runBeforeList =
        Soup::Build::Utilities::StringList({
            "Build",
        });
    Soup::Build::Utilities::StringList BeforeBuildTask::_runAfterList =
        Soup::Build::Utilities::StringList();
}
```

## Extension/Register.cpp
The Cpp file that implements the predefined `RegisterBuildExtension` C method to integrate our build Tasks into the build Engine.
```
#include <string_view>

import Opal;
import Soup.Build;
import Soup.Build.Utilities;

#include "AfterBuildTask.h"
#include "BeforeBuildTask.h"

#define DllExport __declspec(dllexport)
extern "C"
{
    DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
    {
        // Register the before build task
        auto beforeBuildtask = Opal::Memory::Reference<SimpleBuildExtension::BeforeBuildTask>(
            new SimpleBuildExtension::BeforeBuildTask());
        if (buildSystem.TryRegisterTask(beforeBuildtask.GetRaw()) != Soup::Build::ApiCallResult::Success)
            return -1;

        // Register the after build task
        auto afterBuildtask = Opal::Memory::Reference<SimpleBuildExtension::AfterBuildTask>(
            new SimpleBuildExtension::AfterBuildTask());
        if (buildSystem.TryRegisterTask(afterBuildtask.GetRaw()) != Soup::Build::ApiCallResult::Success)
            return -1;

        return 0;
    }
}
```

## Executable/Recipe.toml
The Recipe file that defines the executable "SimpleBuildExtension.Executable". The one interesting part is the relative path reference to the custom build extension through "DevDependnecies".
```
Name = "SimpleBuildExtension.Executable"
Type = "Executable"
Version = "1.0.0"
DevDependencies = [
    "../Extension"
]
Source = [
    "Main.cpp"
]

```

## MyApplication/Main.cpp
A simple main method that prints our "Hello World, Soup Style!" only if the build extension was able to set the custom preprocessor definition "SPECIAL_BUILD".
```
#include <iostream>

int main()
{
#ifdef SPECIAL_BUILD
    std::cout << "Hello World, Soup Style!" << std::endl;
#else
    std::cout << "Hello World..." << std::endl;
#endif

    return 0;
}

```

## .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```
