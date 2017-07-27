//
// calculates circuit complexity fast by using minimized formulas
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_FAST_CALCULATOR_H
#define ORAMEVALUATOR_FAST_CALCULATOR_H

#include <cstdint>
#include "helper.h"

//           Linear  Scan           //
outType& c_acc_LS(uint64_t m, uint64_t b, bool write);
outType& c_LS(uint64_t m, uint64_t b, uint16_t noRead, uint16_t noWrite);

//         Linear Scan ORAM         //
outType& c_acc_LSO(uint64_t m, uint64_t b);
outType& c_LSO(uint64_t m, uint64_t b, uint16_t a);

//         Binary Tree ORAM         //
outType& c_acc_BT(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c);
void test_BT_formulas();

//       PATH          //
outType& c_acc_Path(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void test_path_formulas();

//       PATH-SC          //
outType& c_acc_PSC(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void test_pathSC_formulas();

//       SCORAM          //
outType& c_acc_SCORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void test_SCORAM_formulas();

//       Circuit ORAM          //
outType& c_acc_CORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c);
void test_CORAM_formulas();

#endif //ORAMEVALUATOR_FAST_CALCULATOR_H
