//
// Created by weber on 04.07.2017.
//

#ifndef ORAMEVALUATOR_SETTINGS_H_H
#define ORAMEVALUATOR_SETTINGS_H_H

#include "types/evalParam.h"

class Settings {
protected:
    double_t latency{};                       // latency in ms
    double_t bandwidth{};                     // bandwidth in MBit/s
    double_t calc{};                          // calculation time in AND/ms

    uint8_t securityParam = 80;               // corresponds to security parameter 80
private:
    Settings() = default;
public:
    static Settings& get() {
        static Settings instance;
        return instance;
    }
    inline void set(double_t latency, double_t bandwidth, double_t calc, uint8_t sp) {
        get().latency = latency;
        get().bandwidth = bandwidth;
        get().calc = calc;
        get().securityParam = sp;
    }

    double_t getLatency() { return get().latency; }
    double_t getCalc() { return get().calc; }
    double_t getBandwidth() { return get().bandwidth; }
    uint8_t getSecurity() { return get().securityParam; }

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
};

// standard evaluation range for Binary Tree bucket size B
struct StandardBTB : evalParam {
    StandardBTB(uint16_t d) : evalParam((uint16_t) (2*d), (uint16_t) (4*d), 1, d) { }
};

// standard evaluation range for Path ORAM bucket size B
struct StandardPathB : evalParam {
    StandardPathB(uint16_t d) : evalParam(4, (uint16_t)(4 + d/2), 1, 2) { }
};

// standard evaluation range for Path-SC ORAM bucket size B
struct StandardPathSCB : evalParam {
    StandardPathSCB(uint16_t d) : evalParam(4, (uint16_t)(4 + d/2), 2, 0) { }
};

// standard evaluation range for SCORAM bucket size B
struct StandardScoramB : evalParam {
    StandardScoramB(uint16_t d) : evalParam(6, (uint16_t)(6 + d/2), 1, 2) { }
};

struct StandardPathS : evalParam {
    StandardPathS() : evalParam(
        (uint16_t) (ceil(1.2*Settings::get().getSecurity())-7),
        (uint16_t) (ceil(1.2*Settings::get().getSecurity())-2),
        1, 1) { }
};

static constexpr uint16_t ScoramS[8] = {4, 5, 8, 13, 18, 23, 34, 50};

struct StandardScoramS : evalParam {
    StandardScoramS(uint16_t d) : evalParam(
        (d >= 10 && d <= 24)?
            (uint16_t)(Settings::get().getSecurity() - 15 + ScoramS[(((d % 2 == 1)? d+1 : d) - 10) / 2]) :
            (uint16_t)(Settings::get().getSecurity() + 0.32589*pow(d, 2) - 8.7411*d + 40),
        (d >= 10 && d <= 24)?
            (uint16_t)(Settings::get().getSecurity() - 15 + ScoramS[(((d % 2 == 1)? d+1 : d) - 10) / 2]) :
            (uint16_t)(Settings::get().getSecurity() + 0.32589*pow(d-1, 2) - 8.7411*(d-1) + 40),
        1, 1) {  }
};

#endif //ORAMEVALUATOR_SETTINGS_H_H
