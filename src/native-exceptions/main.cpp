#include <iostream>
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"
#include "native-exceptions.h"


int main() {

    CLI::App app{"Native Exception Demo"};

    //ErrorNumber error_number{ErrorNumber::NormalThrow};
    int error_number = 0;
    app.add_option("-e,--error", error_number, "A specified error number which is\n"
                                               "\t0: normal exception\n"
                                               "\t1: EXC_BAD_ACCESS\n"
                                               "\t2: EXC_CRASH\n"
                                               "\t3: EXCEPTION_STACK_OVERFLOW");

    CLI11_PARSE(app);
    throw_exception((ErrorNumber) error_number);
    return 0;
}
