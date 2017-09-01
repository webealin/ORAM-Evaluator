//
// calculates circuit complexity fast by using minimized formulas
// Created by weber on 30.06.2017.
//
#include "fast_calculator.h"

#include <iostream>
#include <cmath>
#include "helper.h"
#include "primitives.h"

outType& c_acc_BT(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c) {
    uint64_t newM = m;
    uint64_t newB = b;
    uint16_t r = 0;
    uint16_t d = myLog2(newM);
    auto* outT = new outType;
    *outT = {0, 0, 0};

    while (counter-- >= 0 && newM > 2) {
        d = myLog2(newM);

        // TODO: von Wolfram Alpha kürzen lassen
        uint64_t evictGates = (2*d-3)*(7*B-7+(3*B+1)*(newB+2*d+1))+B*(newB+2*d+1)+2*B-2;
        uint64_t evictTraffic = 256*(evictGates+(2*d-3)*(3*B+1)*(newB+2*d+1)) + 2*(d-1)*d-4;

        if(counter == 0)
            outT->traffic += 256*d;

        outT->gates += evictGates + (2*c*(d+1)-d) + d*B*(2+d+newB);
        outT->traffic += evictTraffic + 256*((2*c*(d+1)-d)) + 256*(d*B*(2+d+newB) + d*(B*(newB+1+d))+newB+2*d+1)+4*d;

        newM = (uint64_t) ceil((double) newM/c);
        newB = c*d;
        r++;
    }
    outT->rounds = (uint64_t) 6*r;
    outT->gates += (newM-1)*c*d + newM*c*d + 2*newM;
    outT->traffic += 256*((newM-1)*c*d + newM*c*d + 2*newM);
    return *outT;
}

void print_acc_BT(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c) {
    outType& out = c_acc_BT(m, b, B, counter, c);
    std::cout << "BT: " << out << std::endl;
    delete &out;
}

outType& c_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c) {
    std::cout << "c_BT: TODO - implement me!" << std::endl;
    return *(new outType);
}

void print_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c) {
    outType& out = c_BT(noAcc, values, m, b, B, counter, c);
    std::cout << "BT: " << out << std::endl;
    delete &out;
}

//       PATH          //
outType& c_acc_Path(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    uint64_t newM = m;
    uint64_t newB = b;
    uint16_t r = 0;
    auto d = (uint16_t)(myLog2(newM)-1);
    auto* outT = new outType;
    *outT = {0, 0, 0};

    while (counter-- >= 0 && newM > 2*c) {
        d = (uint16_t)(myLog2(newM)-1);
        // TODO: von Wolfram Alpha kürzen lassen
        outType& evict = c_B2Y(s, newB+2*d+2) + c_B2Y(B*d, d) + (B*d+s)*(c_LZC(d)+c_mux_chain(B*d+s, newB+2*d+2)+d*(B*3*c_lin_gate() + c_comp_mag(myLog2(d)))-2*c_lin_gate());

        if(counter == 0)
            outT->traffic += 256*(d+1);

        outT->gates += (2*c*(d+1)-d) + d*B*(3+d+newB);
        outT->traffic += 256*((2*c*(d+1)-d)) + 256*(d*B*(3+d+newB)) + 256*(d*(B*(newB+2+d))+newB+2*d+2)+4*d;
        *outT += s*(c_comp_eq(d+1) + 2*c_lin_gate() + c_mux(newB)) + evict;

        newM = (uint64_t) ceil((double) newM/c);
        newB = c*d;
        r++;
    }
    outT->rounds = (uint64_t) 4*r;
    outT->gates += (newM-1)*c*d + newM*c*d + 2*newM;
    outT->traffic += 256*((newM-1)*c*d + newM*c*d + 2*newM);
    return *outT;
}

void print_acc_Path(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    outType& out = c_acc_Path(m, b, B, counter, s, c);
    std::cout << "Path: " << out << std::endl;
    delete &out;
}

outType& c_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    std::cout << "c_Path: TODO - implement me!" << std::endl;
    return *(new outType);
}

void print_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    outType& out = c_Path(noAcc, values, m, b, B, counter, s, c);
    std::cout << "Path: " << out << std::endl;
    delete &out;
}

//       PATH-SC          //
outType& c_acc_PathSC(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    auto* outT = new outType;
    uint64_t out = 0;
    uint64_t newM = m;
    uint64_t newB = b;
    auto d = (uint16_t) (myLog2(m)-1);

    while(counter-- >= 0 && newM > 2*c) {
        d = (uint16_t) (myLog2(newM)-1);
        uint64_t lca = (uint64_t)(floor((double) d/2))* myLog2(d)+d-1;  // TODO
        uint16_t logBD = myLog2(B*d);
        outType& sort1 = c_sort(B*d+s, newB+2*d+2, myLog2(d));
        outType& sort2 = c_sort((uint64_t) 2*B*d+s, newB+2*d+2, logBD);
        out += B*d*(2*logBD+newB+d+2)+2*c*d-d+2*c + sort1.gates +
               (B*d+s)*(lca+6*logBD-3)-2*logBD+1 + 2*sort2.gates;
        newM = (uint64_t) ceil((double) newM/c);
        newB = c*d;
        delete &sort1;
        delete &sort2;
    }
    *outT = {out + newM * (c*d + myLog2(newM) - 1), 0, 0};
    return *outT;
}

void print_acc_PathSC(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    outType& out = c_acc_PathSC(m, b, B, counter, s, c);
    std::cout << "Path-SC: " << out << std::endl;
    delete &out;
}

outType& c_PathSC(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    std::cout << "c_PathSC: TODO - implement me!" << std::endl;
    return *(new outType);
}

void print_PathSC(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    outType& out = c_PathSC(noAcc, values, m, b, B, counter, s, c);
    std::cout << "Path-SC: " << out << std::endl;
    delete &out;
}

//       SCORAM          //
outType& c_acc_SCORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    auto* outT = new outType;
    uint64_t out = 0;
    uint64_t newM = m;
    uint64_t newB = b;
    auto d = (uint16_t) (myLog2(m)-1);

    while(counter-- >= 0 && newM > 2*c) {
        d = (uint16_t) (myLog2(newM)-1);
        out += d*(B*(13*newB+75+41*d)-33-8*d+2*c)+c*2+s*(8*newB+32*d+44)-20;
        newM = (uint64_t) ceil((double) newM/c);
        newB = c*d;
    }
    *outT = {out + newM * (c*d + myLog2(newM) - 1), 0, 0};
    return *outT;
}

void print_acc_SCORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    outType& out = c_acc_SCORAM(m, b, B, counter, s, c);
    std::cout << "SCORAM: " << out << std::endl;
    delete &out;
}

outType& c_SCORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    std::cout << "c_SCORAM: TODO - implement me!" << std::endl;
    return *(new outType);
}

void print_SCORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    outType& out = c_SCORAM(noAcc, values, m, b, B, counter, s, c);
    std::cout << "SCORAM: " << out << std::endl;
    delete &out;
}

//       Circuit ORAM          //
outType& c_acc_CORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    auto* outT = new outType;
    uint64_t out = 0;
    uint64_t newM = m;
    uint64_t newB = b;
    auto d = (uint16_t) (myLog2(m)-1);

    while(counter-- >= 0 && newM > 2*c) {
        d = (uint16_t) (myLog2(newM)-1);
        uint16_t logD = myLog2(d);
        uint64_t t = logD*(d+logD);
        out += 2*(c-1+2*(s*(d+t+1)+logD))+d*(B*(5*newB+13*d+21+4*t)+2*c-1+36*logD+4*newB+8*d);
        newM = (uint64_t) ceil((double) newM/c);
        newB = c*d;
    }

    *outT = {out + newM * (c*d + myLog2(newM) - 1), 0, 0};
    return *outT;
}

void print_acc_CORAM(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    outType& out = c_acc_CORAM(m, b, B, counter, s, c);
    std::cout << "Circuit ORAM: " << out << std::endl;
    delete &out;
}

outType& c_CORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    std::cout << "c_CORAM: TODO - implement me!" << std::endl;
    return *(new outType);
}

void print_CORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    outType& out = c_CORAM(noAcc, values, m, b, B, counter, s, c);
    std::cout << "Circuit ORAM: " << out << std::endl;
    delete &out;
}

outType& c_acc_SQRORAM(uint32_t noAcc, uint64_t m, uint64_t b, int16_t counter, uint16_t c) {
    outType* out = new outType;
    *out = {UINT64_MAX, UINT64_MAX, UINT16_MAX};
    return *out;
}

outType& c_SQRORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, int16_t counter, uint16_t c) {
    std::cout << "c_SQRORAM: TODO - implement me!" << std::endl;
    return *(new outType);
}

void test_fast_formulas() {
    print_acc_BT((uint64_t) pow(2, 30), 32, 120, 8, 16);
    print_acc_Path((uint64_t) pow(2, 30), 32, 4, 8, 89, 16);
    print_acc_PathSC((uint64_t) pow(2, 30), 32, 4, 8, 89, 16);
    print_acc_SCORAM((uint64_t) pow(2, 30), 32, 6, 8, 141, 16);
    print_acc_CORAM((uint64_t) pow(2, 30), 32, 4, 8, 89, 16);
}