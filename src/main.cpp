﻿#include "Logger.h"
#include "RunnerSeri.h"
#include "RunnerMaze.h"

int main(int argc, char* argv[]) {
    try {
        //RunnerSeri{}();
        RunnerMaze{}();
    } catch (const std::exception& ex) {
        LOGGER(error, "exception occured: " << ex.what());
    }

    return 0;
}
