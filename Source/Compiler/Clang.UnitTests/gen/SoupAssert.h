#pragma once

class Assert
{
public:
    static void Fail(const std::string& message)
    {
        auto errorMessage = "Assert Failed: " + message;
        throw std::logic_error(errorMessage.c_str());
    }

    static void IsTrue(bool value, const std::string& message)
    {
        if (!value)
        {
            Fail(message);
        }
    }

    static void IsFalse(bool value, const std::string& message)
    {
        if (value)
        {
            Fail(message);
        }
    }

    template<typename T> struct is_shared_ptr : std::false_type {};
    template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

    template<typename T>
    static typename std::enable_if<std::is_pointer<T>::value || is_shared_ptr<T>::value>::type NotNull(T value, const std::string& message)
    {
        if (value == nullptr)
        {
            Fail(message);
        }
    }

    template<typename T>
    static typename std::enable_if<std::is_pointer<T>::value || is_shared_ptr<T>::value>::type AreEqual(
        T expected,
        T actual,
        const std::string& message)
    {
        if (expected == nullptr)
        {
            Fail("Expected was null, use IsNull instead.");
        }
        else if (actual == nullptr)
        {
            Fail("Actual was null, use IsNull if this is expected.");
        }
        else if (*expected != *actual)
        {
            Fail(message);
        }
    }

    template<typename T>
    static typename std::enable_if<!std::is_pointer<T>::value && !is_shared_ptr<T>::value>::type AreEqual(
        const T& expected,
        const T& actual,
        const std::string& message)
    {
        if (expected != actual)
        {
            Fail(message);
        }
    }

    static void AreEqual(
        const std::string& expected,
        const std::string& actual,
        const std::string& message)
    {
        if (expected != actual)
        {
            auto errorExpected = 
                message + 
                " Expected<" + expected +
                "> Actual<" + actual + ">";
            Fail(errorExpected);
        }
    }

    template<typename T>
    static typename std::enable_if<std::is_pointer<T>::value || is_shared_ptr<T>::value>::type AreNotEqual(
        T expected,
        T actual,
        const std::string& message)
    {
        if (expected == nullptr)
        {
            Fail("Expected was null, use IsNull instead.");
        }
        else if (actual == nullptr)
        {
            Fail("Actual was null, use IsNull if this is expected.");
        }
        else if (*expected == *actual)
        {
            Fail(message);
        }
    }

    template<typename T>
    static typename std::enable_if<!std::is_pointer<T>::value && !is_shared_ptr<T>::value>::type AreNotEqual(
        const T& expected,
        const T& actual,
        const std::string& message)
    {
        if (expected == actual)
        {
            Fail(message);
        }
    }
};
