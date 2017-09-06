//
// Created by weber on 04.07.2017.
//

#ifndef ORAMEVALUATOR_SETTINGS_H_H
#define ORAMEVALUATOR_SETTINGS_H_H

#include "types/evalParam.h"

class Settings {
public:
    static constexpr double_t latency = 0;                                   // latency in ms
    static constexpr double_t bandwidth = ceil(1.03 * 1024);                 // bandwidth in MBit/s
    static constexpr double_t calc = 10000;                                  // calculation time in AND/ms

    static const uint8_t securityParam = 0;                                  // corresponds to security parameter 80

    static constexpr uint16_t PathS[] = {89, 89, 89, 89, 89};                // TODO
    static constexpr uint16_t ScoramS[] = {141, 141, 141, 141, 141};         // TODO
};

// standard evaluation range for Binary Tree bucket size B
struct StandardBTB : evalParam {
    StandardBTB(uint16_t d) : evalParam((uint16_t) (2*d), (uint16_t) (10*d), 1, d) { }
};

// standard evaluation range for Path ORAM bucket size B
struct StandardPathB : evalParam {
    StandardPathB(uint16_t d) : evalParam(4, d, 1, 2) { }
};

// standard evaluation range for Path-SC ORAM bucket size B
struct StandardPathSCB : evalParam {
    StandardPathSCB(uint16_t d) : evalParam(4, d, 2, 0) { }
};

// standard evaluation range for SCORAM bucket size B
struct StandardScoramB : evalParam {
    StandardScoramB(uint16_t d) : evalParam(6, d, 1, 2) { }
};

struct StandardPathS : evalParam {
    StandardPathS() : evalParam(Settings::PathS[Settings::securityParam], Settings::PathS[Settings::securityParam+1]+20, 1, 1) { }      // TODO:
};

struct StandardScoramS : evalParam {
    StandardScoramS(uint16_t d) :evalParam(Settings::ScoramS[0], Settings::ScoramS[1]+d, 1, 1) { }        // TODO: abhängig von d
};

#endif //ORAMEVALUATOR_SETTINGS_H_H
