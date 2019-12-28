#include <iostream>

import SoupBuildCore;

#define DllExport __declspec( dllexport )

class MyBuildTask : public Soup::Build::IBuildTask
{
public:
    const char* GetName() override final
    {
        return "my Build task";
    }

    void Execute(Soup::Build::IBuildState& state) override final
    {
    }
};

DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
{
    buildSystem.RegisterTask(std::make_shared<MyBuildTask>());
    return 0;
}