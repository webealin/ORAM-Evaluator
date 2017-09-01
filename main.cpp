#include <iostream>
#include "fast_calculator.h"
#include "evaluation/evaluator.h"
#include "evaluation/plotter.h"

// just for me to identify accidental changes
/*Evaluation:
Linear Scan: gates: 34359738336 traffic: 8796093014016 rounds: 0 time: 1.13873e+07 ms
Binary Tree ORAM: B: 60 c: 4 count: 8 out: gates: 10965082 traffic: 5434194768 rounds: 54 time: 6278.78 ms time needed: 0.121733
Path ORAM: B: 4 c: 4 count: 7 stash: 89 out: gates: 36329002 traffic: 9348174784 rounds: 32 time: 12243.2 ms time needed: 1.75092
PathSC ORAM: B: 4 c: 4 count: 8 stash: 89 out: gates: 15116217 traffic: 3890798836 rounds: 36 time: 5208.74 ms time needed: 0.347086
SCORAM: B: 6 c: 8 count: 6 stash: 141 out: gates: 4584894 traffic: 1751510416 rounds: 84 time: 2461.78 ms time needed: 3.59307
Circuit ORAM: B: 4 c: 8 count: 6 stash: 89 out: gates: 1571692 traffic: 673919792 rounds: 28 time: 906.364 ms time needed: 2.81784
Optimized Square-Root ORAM: c: 4 count: 7 out: gates: 2394120 traffic: 612895120 rounds: 10 time: 843.443 ms time needed: 0.00111
time needed: 8.63202*/

void evaluate() {
    std::cout << "Evaluation:" << std::endl;
    auto* eval = new Evaluator();
    eval->evaluate_acc_exact(1, 0, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact(10, 10, (uint64_t) pow(2, 30), 32);
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
    measure(run_benchmarks);
    measure(test_fast_formulas);
    measure(evaluate);
    measure(evaluate_fast);
    measure(plot);
    return 0;
}