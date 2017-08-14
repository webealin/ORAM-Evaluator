//
// calculates circuit complexity fast by using minimized formulas
// Created by weber on 30.06.2017.
//
#include "fast_calculator.h"

#include <iostream>
#include <cmath>
#include "helper.h"
#include "primitives.h"

outType& c_acc_LS(uint64_t m, uint64_t b, bool write) {
    auto* outT = new outType;
    *outT = {write? m*(b+2) : b*(m-1), 0, 0};
    return *outT;
}

outType& c_LS(uint64_t m, uint64_t b, uint16_t noRead, uint16_t noWrite) {
    auto* outT = new outType;
    *outT = {noRead*b*(m-1) + noWrite*m*(b+2), 0, 0};
    return *outT;
}

outType& c_acc_LSO(uint64_t m, uint64_t b, uint16_t bi) {
    auto* out = new outType;
    *out = {m*(b+bi-1), 0, 0};//512*m*(b+bi-(1/2)), 2};
    return *out;
}

outType& c_LSO(uint64_t m, uint64_t b, uint16_t bi, uint16_t a) {
    auto* out = new outType;
    *out = {m*(b+bi-1), 0, 0};//(m*b)/a + 512*m*(b+bi-(1/2)), static_cast<uint16_t>(2*a+1)};
    return *out;
}

outType& c_acc_BT(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t c) {
    auto* outT = new outType;
    uint64_t out = 0;
    uint64_t newM = m;
    uint64_t newB = b;
    uint16_t d = myLog2(m);

    while(counter-- >= 0 && newM > c) {
        d = myLog2(newM);
        out += B*(d*(7*newB+13*d+6)-8*newB-27)+d*(2*newB+2*c+4*d-19)-3*newB+2*c+16;
        newM = (uint64_t) ceil((double) newM/c);
        newB = c*d;
    }
    *outT = {out + newM * (c*d + myLog2(newM) - 1), 0, 0};
    return *outT;
    // return d*c_RAR_LSO(B, b+1, d) +  d*(c-1) + c*(d+2) + lumu +(2*d-3)*(c_Pop_LSO(B, b+2*d+1, d)+b+2*d+1)+(4*d-5)*c_Add_LSO(B, b+2*d+1, d);
}

void test_BT_formulas() {
    // Circuit ORAM Benchmarks: use new formulas, bucketsize, counter and dynamic blocksize
    for(uint16_t c = 2; c <= 64; c *= 2)
        std::cout << "Binary Tree: c = " << c << ": " << c_acc_BT((uint64_t) pow((double)2, 30), 32, 120, 8, c) << std::endl;
}

//       PATH          //
outType& c_acc_Path(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    auto* outT = new outType;
    uint64_t out = 0;
    uint64_t newM = m;
    uint64_t newB = b;
    auto d = (uint16_t) (myLog2(m)-1);

    while(counter-- >= 0 && newM > 2*c) {       // 2c because d = logm -1!
        d = (uint16_t) (myLog2(newM)-1);
        uint64_t lca = (uint64_t)(floor((double) d/2))* myLog2(d)+d-1;  // TODO
        out += d*B*(newB+d+3)+2*c*(d+1)-d+(B*d+s)*(lca+(B*d+s)*(newB+2*d+2)+d*(B+myLog2(d)));
        newM = (uint64_t) ceil((double) newM/c);
        newB = c*d;
    }
    *outT = {out + newM * (c*d + myLog2(newM) - 1), 0, 0};
    return *outT;
}

void test_path_formulas() {
    // Circuit ORAM Benchmarks: use new formulas, bucketsize, counter and dynamic blocksize
    for(uint16_t c = 2; c <= 64; c *= 2)
        std::cout << "Path: c = " << c << ": " << c_acc_Path((uint64_t) pow((double)2, 30), 32, 4, 8, 89, c) << std::endl;
}

//       PATH-SC          //
outType& c_acc_PSC(uint64_t m, uint64_t b, uint16_t B, int16_t counter, uint16_t s, uint16_t c) {
    auto* outT = new outType;
    uint64_t out = 0;
    uint64_t newM = m;
    uint64_t newB = b;
    auto d = (uint16_t) (myLog2(m)-1);

    while(counter-- >= 0 && newM > 2*c) {
        d = (uint16_t) (myLog2(newM)-1);
        uint64_t lca = (uint64_t)(floor((double) d/2))* myLog2(d)+d-1;  // TODO
        uint16_t logBD = myLog2(B*d);
        out += B*d*(2*logBD+newB+d+2)+2*c*d-d+2*c + c_sort(B*d+s, newB+2*d+2, myLog2(d)).gates +
               (B*d+s)*(lca+6*logBD-3)-2*logBD+1 + 2*c_sort((uint64_t) 2*B*d+s, newB+2*d+2, logBD).gates;
        newM = (uint64_t) ceil((double) newM/c);
        newB = c*d;
    }
    *outT = {out + newM * (c*d + myLog2(newM) - 1), 0, 0};
    return *outT;
}

void test_pathSC_formulas() {
    // Circuit ORAM Benchmarks: use new formulas, bucketsize, counter and dynamic blocksize
    for(uint16_t c = 2; c <= 64; c *= 2)
        std::cout << "PathSC: c = " << c << ": " << c_acc_PSC((uint64_t) pow((double)2, 30), 32, 4, 8, 89, c) << std::endl;
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

void test_SCORAM_formulas() {
    // Circuit ORAM Benchmarks: use new formulas, bucketsize, counter and dynamic blocksize
    for(uint16_t c = 2; c <= 64; c *= 2)
        std::cout << "SCORAM: c = " << c << ": " << c_acc_SCORAM((uint64_t) pow((double)2, 30), 32, 6, 8, 141, c) << std::endl;
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

    //return d*c_RAR_LSO(B, b+1, d+1)+d*(c-1)+c*(d+2)+lumu+2*(-1+s*(2*(d+myLog2(d)*(d+myLog2(d)))+2)+ 2*myLog2(d)+
    //        d*(18*myLog2(d)+2*b+4*d+2+c_Add_LSO(B, b+2*d+2, d+1)+c_RAR_LSO(B, b+d+1, d+1)+B*(2*(d+myLog2(d)*(d+myLog2(d)))+2)));
    *outT = {out + newM * (c*d + myLog2(newM) - 1), 0, 0};
    return *outT;
}

void test_CORAM_formulas() {
    // Circuit ORAM Benchmarks: use new formulas, bucketsize, counter and dynamic blocksize
    for(uint16_t c = 2; c <= 64; c *= 2)
        std::cout << "CORAM: c = " << c << ": " << c_acc_CORAM((uint64_t) pow((double)2, 30), 32, 4, 8, 141, c) << std::endl;
}

void test_fast_formulas() {
    measure(test_BT_formulas);
    measure(test_path_formulas);
    measure(test_pathSC_formulas);
    measure(test_SCORAM_formulas);
    measure(test_CORAM_formulas);
}