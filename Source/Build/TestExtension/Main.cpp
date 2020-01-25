#include <iostream>

import SoupBuildCore;

#define DllExport __declspec(dllexport)

class TestBuildTask : public Opal::Memory::ReferenceCounted<Soup::Build::IBuildTask>
{
public:
    const char* GetName() const noexcept override final
    {
        return "TestBuild";
    }

    Soup::Build::OperationResult Execute(Soup::Build::IBuildState& state) noexcept override final
    {
        return 0;
    }
};

extern "C"
{
    DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
    {
        Opal::Memory::Reference<TestBuildTask> testBuildTask = new TestBuildTask();
        buildSystem.RegisterTask(testBuildTask);
        return 0;
    }
}