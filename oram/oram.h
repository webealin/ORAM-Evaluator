//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_ORAM_H
#define ORAMEVALUATOR_ORAM_H

#include "../helper.h"
#include "../primitives.h"

class ORAM {
protected:
    uint64_t m;     // number of elements to store
    uint64_t b;     // bit width of element to store (payload)
    uint64_t bb;    // block width (payload + metadata)
    std::string type;
public:
    ORAM(uint64_t m, uint64_t b, uint64_t bb, std::string type) : m(m), b(b), bb(bb), type(type){}
    virtual ~ORAM() {}
    virtual outType& c_init(bool values) = 0;
    virtual outType& c_acc(uint64_t b) = 0;
    virtual outType& c_add() = 0;
    virtual outType& c_RAR(uint64_t b) = 0;
    virtual outType& c_amortized(uint16_t noAcc, bool values) = 0;
};

#endif //ORAMEVALUATOR_ORAM_H