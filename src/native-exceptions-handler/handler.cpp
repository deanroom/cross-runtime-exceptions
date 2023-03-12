#include "handler/handler_main.h"

using namespace crashpad;

int main(int argc, char **argv) {
    auto sources = new UserStreamDataSources();
    HandlerMain(argc, argv, sources);
    return 0;
}
