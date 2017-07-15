//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_PRIMITIVES_H
#define ORAMEVALUATOR_PRIMITIVES_H

#include "helper.h"
#include <cmath>
#include "settings.h"

uint64_t c_Y2B(uint64_t m, uint64_t b);
uint64_t c_B2Y(uint64_t m, uint64_t b);
uint64_t c_sShare(uint64_t m, uint64_t b, bool values);

uint64_t c_mux(uint64_t b);
uint64_t c_mux(uint64_t m, uint64_t b);
uint64_t c_mux_chain(uint64_t m, uint64_t b);

uint64_t c_adder(uint64_t b);
uint64_t c_adder_carry(uint64_t b);

uint64_t c_comp_eq(uint64_t b);
uint64_t c_comp_mag(uint64_t b);

uint64_t c_LZC(uint64_t b);
uint64_t c_decode(uint64_t b);
uint64_t c_dDup(uint64_t m, uint64_t b);

uint64_t c_condSwap(uint64_t b);
uint64_t c_compEl(uint64_t b, uint64_t b2);
uint64_t c_sort(uint64_t m, uint64_t b, uint16_t b2);
uint64_t c_shuffle(uint64_t m, uint64_t b);

uint64_t c_rand(uint64_t b);

#endif //ORAMEVALUATOR_PRIMITIVES_H
