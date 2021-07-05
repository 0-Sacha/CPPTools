
#include <iostream>

namespace UnitTest {

    struct TestCopy {
        TestCopy() noexcept {
            std::clog << "Construct" << std::endl;
        }

        TestCopy(const TestCopy& t) noexcept {
            std::clog << "Copy" << std::endl;
        }

        TestCopy(TestCopy&& t) noexcept {
            std::clog << "Move" << std::endl;
        }
    };

}