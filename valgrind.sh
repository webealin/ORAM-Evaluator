#!/usr/bin/env bash

/usr/bin/cmake --build cmake-build-debug --target ORAMEvaluator && /usr/bin/valgrind -v --leak-check=full ./cmake-build-debug/ORAMEvaluator