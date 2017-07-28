//
// Created by weber on 27.07.2017.
//

#ifndef ORAMEVALUATOR_PLOTTER_H
#define ORAMEVALUATOR_PLOTTER_H

#include <list>
#include "evaluator.h"
#include "csvFileWriter.h"

class Plotter : Evaluator {
public:
    void plot_Elements(Evaluator::evalParam mParam, uint64_t b);
    void plot_Bitwidth(uint64_t m, Evaluator::evalParam bParam);
};

#endif //ORAMEVALUATOR_PLOTTER_H
