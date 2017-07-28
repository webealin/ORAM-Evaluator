//
// Created by weber on 27.07.2017.
//
#include "csvFileWriter.h"

void CSVFileWriter::writeFiles() {
    std::ofstream gateFile;
    gateFile.open (filename + "_gates.csv");
    gateFile << outGates;
    gateFile.close();
}

void CSVFileWriter::addLine(uint64_t lineX) {
    outGates += "\n" + lineX;
    outTraffic += "\n" + lineX;
    outRounds += "\n" + lineX;
}

void CSVFileWriter::addOutType(outType* out) {
    outGates +=  ", " + out->gates;
    outTraffic +=  ", " + out->traffic;
    outRounds +=  ", " + out->rounds;
}
