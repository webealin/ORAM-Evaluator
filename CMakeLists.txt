cmake_minimum_required(VERSION 3.7)
project(ORAMEvaluator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        primitives.cpp primitives.h
        oram/linear_scan_oram.cpp oram/linear_scan_oram.h
        oram/binary_tree_oram.cpp oram/binary_tree_oram.h
        oram/oram.h
        oram/path_oram.cpp oram/path_oram.h
        helper.h
        fast_calculator.cpp fast_calculator.h trivial_linear_scan.h oram/tree_factory.h settings.h evaluation/evaluator.cpp evaluation/evaluator.h)
add_executable(ORAMEvaluator ${SOURCE_FILES})