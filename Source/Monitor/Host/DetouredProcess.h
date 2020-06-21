
class DetouredProcess
{
public:
    DetouredProcess()
    {
    }

    void RunProcess(
        const std::string& application,
        const std::vector<std::string>& arguments)
    {
        std::stringstream argumentsValue;
        argumentsValue << "\"" << application << "\"";
        for (auto& arg : arguments)
        {
            argumentsValue << " " << arg;
        }

        std::string argumentsString = argumentsValue.str();

        // Setup the process creation parameters
        LPSECURITY_ATTRIBUTES processAttributes = nullptr;
        LPSECURITY_ATTRIBUTES threadAttributes = nullptr;
        bool inheritHandles = false;
        DWORD creationFlags = 0;
        void* environment = nullptr;

        STARTUPINFOA startupInfo = {};
        ZeroMemory(&startupInfo, sizeof(STARTUPINFOA));
        startupInfo.cb = sizeof(startupInfo);
        // startupInfo.hStdError = childStdErrWrite._handle;
        // startupInfo.hStdOutput = childStdOutWrite._handle;
        // startupInfo.hStdInput = childStdInRead._handle;
        // startupInfo.dwFlags |= STARTF_USESTDHANDLES;

        PROCESS_INFORMATION processInfo = {};
        ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

        std::string workingDirectory = "D:\\Repos\\Soup\\Source\\Monitor\\";

        // Start the process
        if (!DetourCreateProcessWithDllA(
            application.c_str(),
            const_cast<char*>(argumentsString.c_str()),
            processAttributes,
            threadAttributes,
            inheritHandles,
            creationFlags,
            environment,
            workingDirectory.c_str(),
            &startupInfo,
            &processInfo,
            "D:\\Repos\\Soup\\Source\\Monitor\\Host\\out\\bin\\MSVC\\debug\\Monitor.Detours.dll",
            nullptr))
        {
            auto error = GetLastError();
            std::cout << "Failed to create detoured process." << std::endl;
            return;
        }
    }
};