//
// Created by alina on 01.09.17.
//

#ifndef ORAMEVALUATOR_MAP_BASED_ORAM_H
#define ORAMEVALUATOR_MAP_BASED_ORAM_H

#include "oram.h"
#include "linear_scan_oram.h"

class MapBasedORAM : virtual public ORAM {
protected:
    uint16_t c;                 // packing factor
    ORAM* map{};                // ORAM that stores map

public:
    MapBasedORAM(uint16_t c) : c(c) { }

    virtual ~MapBasedORAM() {
        delete map;
    }

    /**
     * construction of the structure using counter steps before utilizing Linear Scan ORAM
     * @param counter
     */
    inline virtual void build(uint16_t counter) {
        auto newM = (uint64_t) ceil((double) m / c);

        if(recursionCond(counter)) {
            map = createMap(newM);
            dynamic_cast<MapBasedORAM*> (map)->build((uint16_t) (counter-1));
        }
        else map = createLSMap(newM);
    }

    virtual ORAM* createMap(uint64_t newM) = 0;
    virtual bool recursionCond(uint16_t counter) = 0;

    inline virtual TrivialLinearScan* createLSMap(uint64_t newM) {
        return new TrivialLinearScan(newM, c*myLog2(m));
    }
};

#endif //ORAMEVALUATOR_MAP_BASED_ORAM_H
