//
// Created by weber on 27.07.2017.
//
#include "csvFileWriter.h"

void CSVFileWriter::writeFiles() {
    writeGates();
    writeTraffic();
    writeRounds();
    writeTime();
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

void CSVFileWriter::writeTime() {
    std::ofstream timeFile;
    timeFile.open (filename + "_time.csv");
    timeFile << outTime;
    timeFile.close();
}

void CSVFileWriter::addLine(uint16_t lineX) {
    outGates += "\n" + std::to_string(lineX);
    outTraffic += "\n" + std::to_string(lineX);
    outRounds += "\n" + std::to_string(lineX);
    outTime += "\n" + std::to_string(lineX);
}

void CSVFileWriter::addHeader(std::string header) {
    outGates += header;
    outTraffic += header;
    outRounds += header;
    outTime += header;
}

void CSVFileWriter::addEmpty() {
    outGates +=  ", ";
    outTraffic +=  ", ";
    outRounds +=  ", ";
    outTime +=  ", ";
}

void CSVFileWriter::addOutType(Evaluator::btSettings minSettings) {
    addOutType(*minSettings.out);
    delete minSettings.out;
}

void CSVFileWriter::addOutType(Evaluator::pathSettings minSettings) {
    addOutType(*minSettings.bt->out);
    delete minSettings.bt->out;
    delete minSettings.bt;
}

void CSVFileWriter::addOutType(Evaluator::sqrSettings minSettings) {
    addOutType(*minSettings.out);
    delete minSettings.out;
}

void CSVFileWriter::addOutType(outType out) {
    if(out.gates == UINT64_MAX)
        outGates += ", ";
    else outGates +=  ", " + std::to_string(out.gates);

    if(out.traffic == UINT64_MAX)
        outTraffic += ", ";
    else outTraffic +=  ", " + std::to_string(out.traffic);

    outRounds +=  ", " + std::to_string(out.rounds);

    if(needsTime(out) == UINT64_MAX)
        outTime += ", ";
    else outTime +=  ", " + std::to_string(needsTime(out));
}

void CSVFileWriter::addOutType_paper(outType out) {
    if(out.gates == UINT64_MAX)
        outGates += ", ";
    else outGates +=  ", " + std::to_string(out.gates);

    if(out.traffic == UINT64_MAX)
        outTraffic += ", ";
    else outTraffic +=  ", " + std::to_string(out.traffic / 8);

    outRounds +=  ", " + std::to_string(out.rounds);

    if(needsTime(out) == UINT64_MAX)
        outTime += ", ";
    else outTime +=  ", " + std::to_string(needsTime(out) / 1000);
}
