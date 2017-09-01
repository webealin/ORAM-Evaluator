//
// Created by alina on 31.08.17.
//

#include <cstdint>

#ifndef ORAMEVALUATOR_EVALPARAM_H
#define ORAMEVALUATOR_EVALPARAM_H

struct evalParam {
    uint16_t min;
    uint16_t max;
    uint16_t step_m;
    uint16_t step_p;

    evalParam(uint16_t min, uint16_t max, uint16_t step_m, uint16_t step_p) :
            min(min), max(max), step_m(step_m), step_p(step_p) { }
};

#endif //ORAMEVALUATOR_EVALPARAM_H