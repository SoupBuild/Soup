#include <iostream>

import SoupBuildCore;

#define DllExport __declspec( dllexport )

class MyBuildTask : public Opal::Memory::ReferenceCounted<Soup::Build::IBuildTask>
{
public:
    ~MyBuildTask()
    {
    }

    const char* GetName() const noexcept override final
    {
        return "MyBuildTask";
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
        Opal::Memory::Reference<MyBuildTask> myTask = new MyBuildTask();
        buildSystem.RegisterTask(myTask);
        return 0;
    }
}