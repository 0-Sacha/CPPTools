
#include "../TestCore/TestCore.h"

namespace CPPTools::Test {

    class TestCopy {
    public:
        TestCopy(const std::string& name) 
            : m_Name(name)
        {
            CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Default (copy string) Constructor", "TestCopy(const std::string& name)");
        }

		TestCopy(std::string&& name)
			: m_Name(std::move(name))
		{
			CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Default (move string) Constructor", "TestCopy(std::string&& name)");
		}


		TestCopy(TestCopy& other)
			: m_Name("Copy of " + other.m_Name)
		{
			CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Copy Constructor", "TestCopy(TestCopy&)");
		}

        TestCopy(const TestCopy& other)
			: m_Name("Copy of " + other.m_Name) 
		{
            CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Const Copy Constructor", "TestCopy(const TestCopy&)");
        }

        TestCopy(TestCopy&& other) noexcept
			: m_Name("Move of " + other.m_Name) 
		{
            CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Move Constructor", "TestCopy(TestCopy&&)");
		}

		TestCopy(const TestCopy&& other) noexcept
			: m_Name("Move of " + other.m_Name) 
		{
			CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Const Move Constructor", "TestCopy(const TestCopy&&)");
		}


        TestCopy& operator=(TestCopy& other) {

			m_Name = "Copy Of " + other.m_Name;

            CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Copy Assignment", "TestCopy& operator=(TestCopy&)");
			return *this;
        }

		TestCopy& operator=(const TestCopy& other) {

			m_Name = "Copy Of " + other.m_Name;

			CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Const Copy Assignment", "TestCopy& operator=(const TestCopy&)");
			return *this;
		}

		TestCopy& operator=(TestCopy&& other) noexcept {

			m_Name = "Move Of " + other.m_Name;

			CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Move Assignment", "TestCopy& operator=(TestCopy&&)");
			return *this;
		}

		TestCopy& operator=(const TestCopy&& other) noexcept {

			m_Name = "Move Of " + other.m_Name;

			CPPTOOLS_TEST_INFO("UnwantedCopy {} : {:C:red} ( {:C:red} )", m_Name, "Const Move Assignment", "TestCopy& operator=(const TestCopy&&)");
			return *this;
		}

    private:
        std::string m_Name;
    };

}