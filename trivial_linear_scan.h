//
// Created by weber on 01.07.2017.
//

#ifndef ORAMEVALUATOR_LINEAR_SCAN_H
#define ORAMEVALUATOR_LINEAR_SCAN_H

#include "primitives.h"
#include "helper.h"

class TrivialLinearScan {
public:
    /**
     * costs of read access on standard Linear Scan
     * @param m: number of elements of array
     * @param b: number of bits per array element
     * @return costs for read access
     */
    static inline outType& read(uint64_t m, uint64_t b) {
        // tMux(m, b)
        return c_mux(m, b);
    }

    /**
     * costs of write access on standard Linear Scan
     * @param m: number of elements of array
     * @param b: number of bits per array element
     * @return costs for write access
     */
    static inline outType& write(uint64_t m, uint64_t b) {
        // m*mux(b)+decode(m)
        return (m*c_mux(b))+c_decode(myLog2(m));
    }
};
#endif //ORAMEVALUATOR_LINEAR_SCAN_H