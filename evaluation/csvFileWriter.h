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
    std::string outTime;
    std::string filename;
public:
    explicit CSVFileWriter(std::string filename) : filename(std::move(filename)) { }
    void addLine(uint16_t lineX);
    void addHeader(std::string header);
    void addEmpty();
    void addOutType(outType out);
    void addOutType_paper(outType out);
    void addOutType(Evaluator::btSettings minSettings);
    void addOutType(Evaluator::pathSettings minSettings);
    void addOutType(Evaluator::sqrSettings minSettings);
    void writeFiles();
    void writeGates();
    void writeTraffic();
    void writeRounds();
    void writeTime();
};

#endif //ORAMEVALUATOR_FILEWRITER_H
