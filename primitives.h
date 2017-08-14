//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_PRIMITIVES_H
#define ORAMEVALUATOR_PRIMITIVES_H

#include "helper.h"
#include <cmath>
#include <cassert>
#include "settings.h"

outType& c_lin_gate();

outType& c_Y2B(uint64_t m, uint64_t b);
outType& c_B2Y(uint64_t m, uint64_t b);
outType& c_booleanShare(uint64_t m, uint64_t b);
outType& c_yaoShare(uint64_t m, uint64_t b);

outType& c_mux(uint64_t b);
outType& c_mux(uint64_t m, uint64_t b);
outType& c_mux_chain(uint64_t m, uint64_t b);

outType& c_adder(uint64_t b);
outType& c_adder_carry(uint64_t b);

outType& c_comp_eq(uint64_t b);
outType& c_comp_mag(uint64_t b);

outType& c_LZC(uint64_t b);
outType& c_decode(uint64_t b);
outType& c_dDup(uint64_t m, uint64_t b);

outType& c_condSwap(uint64_t b);
outType& c_compEl(uint64_t b, uint64_t b2);
outType& c_sort(uint64_t m, uint64_t b, uint16_t b2);
outType& c_shuffle(uint64_t m, uint64_t b);

outType& c_rand(uint64_t b);

#endif //ORAMEVALUATOR_PRIMITIVES_H
