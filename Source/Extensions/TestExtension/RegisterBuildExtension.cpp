module;

#define DllExport __declspec(dllexport)

module TestExtension;

extern "C"
{
    DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
    {
        Opal::Memory::Reference<TestBuildTask> testBuildTask = new TestBuildTask();
        buildSystem.RegisterTask(testBuildTask);
        return 0;
    }
}