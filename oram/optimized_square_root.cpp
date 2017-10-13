//
// Created by weber on 20.08.2017.
//

#include "optimized_square_root.h"

outType& OSquareRoot::c_init(bool values) {
    if(values)
        return c_permute();

    auto* out = new outType;
    *out = {0, 0, 0};
    return *out;
}

outType& c_dIdx2(uint16_t b) {
    auto* out = new outType;
    *out = {0, b, 1};
    return *out;
}

outType& c_dBIdx(uint16_t b) {
    auto* out = new outType;
    *out = {0, (uint64_t) (2 * b), 2};
    return *out;
}

outType& OSquareRoot::c_LUMU() {
    return addWR(c_dIdx2(myLog2(m)), c_dBIdx(myLog2(c)) + TrivialLinearScan::c_read(c, myLog2(m)) + TrivialLinearScan::c_write(c, myLog2(m)) + map->c_acc(c*myLog2(m)));
}

outType& OSquareRoot::c_acc(uint64_t b) {
    outType& out = c_LUMU() + c_dIdx2(myLog2(m)) + c_mux(b);
    if (t != 0)
        out += c_B2Y(t, bb) + t*c_comp_eq(myLog2(m)) + (t-1)*(c_mux(b) + c_lin_gate()) + c_Y2B(t, bb);
    t++;
    return out;
}

outType& OSquareRoot::c_acc(uint32_t noAccess, uint64_t b) {
    outType& out = c_acc(b);
    for(uint16_t i = 2; i <= noAccess; i++) {
        outType& acc = c_acc(b);
        out.rounds += acc.rounds;
        out += acc;

        if(t == T+1) {
            outType& epoch = addWR(c_permute(), c_update());
            out.rounds += epoch.rounds;
            out += epoch;
            t = 0;
        }

    }
    return out;
}

outType& OSquareRoot::c_amortized(uint32_t noAcc, bool values) {
    return c_init(values) / noAcc + floor(noAcc/T)*c_epochWO() + c_acc(b);
}

outType& OSquareRoot::c_permute() {
    return c_shuffle(m, bb) + c_Y2B(m, bb) + map->c_init(true);
}

outType& OSquareRoot::c_update() {
    return c_B2Y((uint64_t) (2 * T), bb);
}

outType& OSquareRoot::c_epochWO() {
    return c_update() + c_permute();
}