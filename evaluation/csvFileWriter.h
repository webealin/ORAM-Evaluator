//
// Created by weber on 27.07.2017.
//

#ifndef ORAMEVALUATOR_FILEWRITER_H
#define ORAMEVALUATOR_FILEWRITER_H

#include "evaluator.h"
#include <list>
#include <iostream>
#include <fstream>

class CSVFileWriter {
private:
    std::string outGates;
    std::string outTraffic;
    std::string outRounds;
    std::string filename;
public:
    CSVFileWriter(std::string filename) : filename(filename) {}
    void addLine(uint64_t lineX);
    void addOutType(outType* out);
    void writeFiles();
};

#endif //ORAMEVALUATOR_FILEWRITER_H
