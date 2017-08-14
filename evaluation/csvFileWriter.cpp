//
// Created by weber on 27.07.2017.
//
#include "csvFileWriter.h"

void CSVFileWriter::writeFiles() {
    writeGates();
    writeTraffic();
    writeRounds();
}

void CSVFileWriter::writeGates() {
    std::ofstream gateFile;
    gateFile.open (filename + "_gates.csv");
    gateFile << outGates;
    gateFile.close();
}

void CSVFileWriter::writeTraffic() {
    std::ofstream trafficFile;
    trafficFile.open (filename + "_traffic.csv");
    trafficFile << outTraffic;
    trafficFile.close();
}

void CSVFileWriter::writeRounds() {
    std::ofstream roundsFile;
    roundsFile.open (filename + "_rounds.csv");
    roundsFile << outRounds;
    roundsFile.close();
}

void CSVFileWriter::addLine(uint16_t lineX) {
    outGates += "\n" + std::to_string(lineX);
    outTraffic += "\n" + std::to_string(lineX);
    outRounds += "\n" + std::to_string(lineX);
}

void CSVFileWriter::addOutType(outType* out) {
    outGates +=  ", " + std::to_string(out->gates);
    outTraffic +=  ", " + std::to_string(out->traffic);
    outRounds +=  ", " + std::to_string(out->rounds);
    delete out;
}

void CSVFileWriter::addOutType(outType out) {
    outGates +=  ", " + std::to_string(out.gates);
    outTraffic +=  ", " + std::to_string(out.traffic);
    outRounds +=  ", " + std::to_string(out.rounds);
}
