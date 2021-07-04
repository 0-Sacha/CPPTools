
#include <iostream>

namespace UnitTest {

    struct TestCopy {
        TestCopy() {
            std::clog << "Construct" << std::endl;
        }

        TestCopy(const TestCopy& t) {
            std::clog << "Copy" << std::endl;
        }

        TestCopy(TestCopy&& t) {
            std::clog << "Move" << std::endl;
        }
    };

}