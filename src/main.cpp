#include "cli/CLI.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc == 2) {
        return calc::CLI::evaluateAndPrint(argv[1]);
    }

    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [expression]" << std::endl;
        return 1;
    }

    calc::CLI cli;
    cli.run();
    return 0;
}
