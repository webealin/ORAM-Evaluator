#include <iostream>
#include "fast_calculator.h"
#include "evaluation/evaluator.h"
#include "evaluation/plotter.h"

void evaluate() {
    std::cout << "Evaluation:" << std::endl;
    auto* eval = new Evaluator();
    eval->evaluate_acc_exact(1, 0, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact(10, 10, true, (uint64_t) pow(2, 30), 32);
    delete eval;
}

void evaluate_fast() {
    std::cout << "Evaluation - fast:" << std::endl;
    auto* eval = new Evaluator();
    eval->evaluate_acc_exact_fast(1, 0, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact_fast(10, 10, (uint64_t) pow(2, 30), 32);
    delete eval;
}

void plot()  {
    std::cout << "Plot results:" << std::endl;
    auto* plotter = new Plotter();
    plotter->plot_acc_elements(1, evalParam{1, 10, 1, 1}, 32, "test");
    delete plotter;
}

int main() {
    std::cout << Mixed_ORAM_slow(1, true, (uint64_t) pow(2, 10), 32, 4, 4, 89, UINT16_MAX) << std::endl;
    /*measure(run_benchmarks);
    measure(test_fast_formulas);
    measure(evaluate);
    measure(evaluate_fast);
    measure(plot);*/
    return 0;
}