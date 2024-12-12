#ifndef SIMPLE_TEST_FRAMEWORK_H
#define SIMPLE_TEST_FRAMEWORK_H

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <stdexcept>

class TestFramework {
public:
    using TestFunction = std::function<void()>;

    static TestFramework& getInstance() {
        static TestFramework instance;
        return instance;
    }

    void addTest(const std::string& testName, TestFunction func) {
        tests.push_back({testName, func});
    }

    void runAllTests() {
        size_t passed = 0;
        size_t failed = 0;

        for (const auto& [name, func] : tests) {
            try {
                func();
                std::cout << "[PASS] " << name << std::endl;
                passed++;
            } catch (const std::exception& e) {
                std::cout << "[FAIL] " << name << " - " << e.what() << std::endl;
                failed++;
            }
        }

        std::cout << "\nTests completed: " << passed + failed 
                  << " | Passed: " << passed 
                  << " | Failed: " << failed << "\n";
    }

    // Delete copy/move constructors for singleton
    TestFramework(const TestFramework&) = delete;
    TestFramework& operator=(const TestFramework&) = delete;

private:
    TestFramework() = default;

    std::vector<std::pair<std::string, TestFunction>> tests;
};

// Helper macros for ease of use
#define ASSERT_TRUE(condition) \
    if (!(condition)) throw std::runtime_error("Assertion failed: " #condition);

#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) throw std::runtime_error("Assertion failed: " #expected " != " #actual);

#define TEST_CASE(name) \
    static void name(); \
    static bool name##_registered = [](){ \
        TestFramework::getInstance().addTest(#name, name); \
        return true; \
    }(); \
    static void name()

#endif 
