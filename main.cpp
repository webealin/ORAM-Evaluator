#include <iostream>
#include "fast_calculator.h"
#include "oram/tree_factory.h"
#include "evaluation/evaluator.h"
#include "evaluation/plotter.h"

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
   // measure(test_CORAM);
}

void evaluate() {
    std::cout << "Evaluation:" << std::endl;
    auto* eval = new Evaluator();
    eval->evaluate_exact(1, 0, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact(10, 10, (uint64_t) pow(2, 60), 512);
    delete eval;
}

void evaluate_fast() {
    std::cout << "Evaluation - fast:" << std::endl;
    auto* eval = new Evaluator();
    eval->evaluate_exact_fast(1, 0, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact_fast(10, 10, (uint64_t) pow(2, 60), 512);
    delete eval;
}

void plot()  {
    std::cout << "Plot results:" << std::endl;
    auto* plotter = new Plotter();
    plotter->plot_Elements(Evaluator::evalParam{1, 55, 1, 1}, 512);
    //plotter->plot_Bitwidth(30, Evaluator::evalParam{1, 30, 2, 0});
    delete plotter;
}

int main() {
    test_paper_benchmarks();
    //test_fast_formulas();
    //measure(evaluate);
    //measure(evaluate_fast);
    //measure(plot);

    return 0;
}