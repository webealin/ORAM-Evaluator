cmake_minimum_required(VERSION 3.7)
project(ORAMEvaluator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        primitives.cpp primitives.h
        oram/oram.h
        oram/linear_scan_oram.cpp oram/linear_scan_oram.h
        oram/binary_tree_oram.cpp oram/binary_tree_oram.h
        oram/path_oram.cpp oram/path_oram.h
        oram/optimized_square_root.cpp oram/optimized_square_root.h
        helper.h settings.h
        fast_calculator.cpp fast_calculator.h
        oram/oram_factory.h
        evaluation/evaluator.cpp evaluation/evaluator.h
        evaluation/plotter.cpp evaluation/plotter.h
        evaluation/csvFileWriter.cpp evaluation/csvFileWriter.h
        paper_benchmarks.cpp paper_benchmarks.h types/outType.h types/evalParam.h evaluation/access_functions.h)
add_executable(ORAMEvaluator ${SOURCE_FILES})