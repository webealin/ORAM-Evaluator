//
// Created by weber on 27.07.2017.
//

#ifndef ORAMEVALUATOR_FILEWRITER_H
#define ORAMEVALUATOR_FILEWRITER_H

#include "evaluator.h"
#include <list>
#include <iostream>
#include <fstream>
#include <utility>

class CSVFileWriter {
private:
    std::string outGates;
    std::string outTraffic;
    std::string outRounds;
    std::string filename;
public:
    explicit CSVFileWriter(std::string filename) : filename(std::move(filename)) {
        outGates += "d, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM";
        outTraffic += "d, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM";
        outRounds += "d, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM";
    }
    void addLine(uint16_t lineX);
    void addOutType(outType* out);
    void addOutType(outType out);
    void writeFiles();
    void writeGates();
    void writeTraffic();
    void writeRounds();
};

#endif //ORAMEVALUATOR_FILEWRITER_H
