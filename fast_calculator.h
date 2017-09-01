//
// calculates circuit complexity fast by using minimized formulas
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_FAST_CALCULATOR_H
#define ORAMEVALUATOR_FAST_CALCULATOR_H

#include <cstdint>
#include "helper.h"
#include "types/outType.h"

//         Binary Tree ORAM         //
outType& c_acc_BT(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c);
outType& c_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c);
void print_acc_BT(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c);
void print_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c);

//               PATH               //
outType& c_acc_Path(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
outType& c_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void print_acc_Path(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void print_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);

//             PATH-SC              //
outType& c_acc_PathSC(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
outType& c_PathSC(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void print_acc_PathSC(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void print_PathSC(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);

//              SCORAM              //
outType& c_acc_SCORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
outType& c_SCORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void print_acc_SCORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void print_SCORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);

//           Circuit ORAM           //
outType& c_acc_CORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
outType& c_CORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void print_acc_CORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void print_CORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);

//        Optimized SQR ORAM        //
outType& c_acc_SQRORAM(uint32_t noAcc, uint64_t m, uint64_t b, int16_t counter, uint16_t c);
outType& c_SQRORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, int16_t counter, uint16_t c);
void print_acc_SQRORAM(uint32_t noAcc, uint64_t m, uint64_t b, int16_t counter, uint16_t c);
void print_SQRORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, int16_t counter, uint16_t c);

void test_fast_formulas();

#endif //ORAMEVALUATOR_FAST_CALCULATOR_H
