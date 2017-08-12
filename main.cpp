#include <iostream>
#include "fast_calculator.h"
#include "oram/tree_factory.h"
#include "evaluation/evaluator.h"

void test_GKK() {
    std::cout << "GKK Benchmarks: use old formulas, no fixed bucket-sizes, no counter, and no dynamic block-size" << std::endl;
    for(uint16_t d = 20; d >= 16; d--)
        TreeFactory::acc_GKK_BT(d, 512, 16);
}

void test_CORAM() {
    std::cout << "Circuit ORAM Benchmarks: use new formulas, counter and dynamic block-size" << std::endl;
    TreeFactory::acc_BT(30, 32, 120, 16, 8, false);
    TreeFactory::acc_Path("Path", 30, 32, 4, 16, 89, 8);
    TreeFactory::acc_Path("PathSC", 30, 32, 4, 16, 89, 8);
    TreeFactory::acc_Path("Scoram", 30, 32, 6, 16, 141, 8);
    TreeFactory::acc_Path("CORAM", 30, 32, 4, 16, 141, 8);
}

void test_paper_benchmarks() {
    measure(test_GKK);
    measure(test_CORAM);
}

void evaluate() {
    std::cout << "Evaluation:" << std::endl;
    auto* eval = new Evaluator();
    //eval->evaluate_exact(1, 0, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact_fast(1, 0, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact(10, 10, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact(10, 10, (uint64_t) pow(2, 30), 512);
    eval->evaluate_exact(10, 10, (uint64_t) pow(2, 60), 512);
    //eval->evaluate_exact_fast(10, 10, (uint64_t) pow(2, 60), 512);
    delete eval;
}

int main() {
    measure(test_paper_benchmarks);
    //measure(test_fast_formulas);
    measure(evaluate);
    return 0;
}