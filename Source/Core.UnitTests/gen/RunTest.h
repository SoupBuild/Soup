#pragma once

struct TestState
{
    int FailCount;
    int PassCount;

    TestState& operator+=(const TestState& rhs)
    {
        FailCount += rhs.FailCount;
        PassCount += rhs.PassCount;
        return *this;
    }
};

TestState RunTest(
    std::string className,
    std::string testName,
    std::function<void(void)> test)
{
    try
    {
        // std::cout << "Running: " << testName << std::endl;
        test();
        // std::cout << "PASS: " << testName << std::endl;
        return TestState{ 0, 1 };
    }
    catch (std::exception& ex)
    {
        std::cout << "FAIL: " << className << "::" << testName << std::endl;
        std::cout << typeid(ex).name() << std::endl;

        if (!std::string(ex.what()).empty())
        {
            std::cout << ex.what() << std::endl;
        }
    }
    catch (...)
    {
        std::cout << "FAIL: " << className << "::" << testName << std::endl;
        std::cout << "Unknown error..." << std::endl;
    }

    return TestState{ 1, 0 };
}

// DEPRECATED
TestState RunTest(
    std::function<void(void)> test)
{
    return RunTest("UNKNOWN", "UNKNOWN", test);
}