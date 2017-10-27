//
// Created by weber on 14.10.2017.
//

#ifndef ORAMEVALUATOR_MINSETTINGS_H
#define ORAMEVALUATOR_MINSETTINGS_H

#include <cstdint>
#include "outType.h"

struct minSettings {
    uint16_t c{UINT16_MAX};
    uint16_t count{UINT16_MAX};
    outType* out{};

    minSettings() : out(new outType) { }
    minSettings(uint16_t c, uint16_t count, outType* out) : c(c), count(count), out(out) { }
};

struct btSettings : public minSettings {
    uint16_t B{UINT16_MAX};
    btSettings() = default;
    btSettings(uint16_t B, uint16_t c, uint16_t count, outType* out) : minSettings(c, count, out), B(B) { }
};

struct pathSettings : public btSettings {
    uint16_t stash{UINT16_MAX};
    pathSettings() = default;
    pathSettings(uint16_t B, uint16_t c, uint16_t count, uint16_t stash, outType* out) :
            btSettings(B, c, count, out), stash(stash) { }
};

#endif //ORAMEVALUATOR_MINSETTINGS_H
