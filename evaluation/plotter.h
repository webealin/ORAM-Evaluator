//
// Created by weber on 27.07.2017.
//

#ifndef ORAMEVALUATOR_PLOTTER_H
#define ORAMEVALUATOR_PLOTTER_H

#include <list>
#include "evaluator.h"
#include "csvFileWriter.h"

class Plotter : Evaluator {
private:
    void plot(uint16_t noAcc, bool values, uint16_t d, uint64_t b, CSVFileWriter* writer, funcTypes func);

public:
    void plot_acc_elements(evalParam dParam, uint64_t b, const std::string& filename);
    void plot_elements(uint16_t noAcc, bool values, evalParam dParam, uint64_t b, const std::string& filename);

    void plot_acc_bitwidth(uint64_t m, evalParam bParam, const std::string& filename);
    void plot_bitwidth(uint16_t noAcc, bool values, uint64_t m, evalParam bParam, const std::string& filename);
};

#endif //ORAMEVALUATOR_PLOTTER_H
