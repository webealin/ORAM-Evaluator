//
// Created by weber on 04.07.2017.
//

#ifndef ORAMEVALUATOR_SETTINGS_H_H
#define ORAMEVALUATOR_SETTINGS_H_H

class Settings {
public:
    enum ToCalc {
        GATES, TRAFFIC, ROUNDS
    };
    static ToCalc toCalc;
};

#endif //ORAMEVALUATOR_SETTINGS_H_H
