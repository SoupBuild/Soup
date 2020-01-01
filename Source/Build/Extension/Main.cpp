#include <iostream>

import SoupBuildCore;

#define DllExport __declspec( dllexport )

class MyBuildTask : public Soup::Build::IBuildTask, public Opal::Memory::ReferenceCounted
{
public:
    const char* GetName() const noexcept override final
    {
        return "MyBuildTask";
    }

    Soup::Build::BuildSystemResult Execute(Soup::Build::IBuildState& state) noexcept override final
    {
        return 0;
    }
};

DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
{
    buildSystem.RegisterTask(std::make_shared<MyBuildTask>());
    return 0;
}