//
// Created by weber on 14.10.2017.
//

#ifndef ORAMEVALUATOR_MINSETTINGS_H
#define ORAMEVALUATOR_MINSETTINGS_H

#include <cstdint>
#include "outType.h"

struct minSettings {
    std::string type;
    uint16_t c{UINT16_MAX};
    uint16_t count{UINT16_MAX};
    outType* out{};

    minSettings() : out(new outType) { }
    minSettings(std::string type, uint16_t c, uint16_t count, outType* out) :
            type(type), c(c), count(count), out(out) { }
};


inline std::ostream& operator<<(std::ostream& ostr, const minSettings& s) {
    return ostr << "type: " << s.type << " c: " << s.c << " count: " << s.count;
}

struct btSettings : public minSettings {
    uint16_t B{UINT16_MAX};
    btSettings() = default;
    btSettings(std::string type, uint16_t B, uint16_t c, uint16_t count, outType* out) :
            minSettings(type, c, count, out), B(B) { }
};


inline std::ostream& operator<<(std::ostream& ostr, const btSettings& s) {
    return ostr << "type: " << s.type << " B: " << s.B << " c: " << s.c << " count: " << s.count;
}

struct pathSettings : public btSettings {
    uint16_t stash{UINT16_MAX};
    pathSettings() = default;
    pathSettings(std::string type, uint16_t B, uint16_t c, uint16_t count, uint16_t stash, outType* out) :
            btSettings(type, B, c, count, out), stash(stash) { }
};

inline std::ostream& operator<<(std::ostream& ostr, const pathSettings& s) {
    return ostr << "type: " << s.type << " B: " << s.B << " stash: " << s.stash << " c: " << s.c << " count: " << s.count;
}

#endif //ORAMEVALUATOR_MINSETTINGS_H
