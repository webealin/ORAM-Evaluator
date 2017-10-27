//
// Created by alina on 30.08.17.
//

#include "paper_benchmarks.h"
#include "evaluation/csvFileWriter.h"
#include "evaluation/access_functions.h"

void test_GKK() {
    std::cout << "GKK Benchmarks: use old formulas, no fixed bucket-sizes, no counter, and no dynamic block-size" << std::endl;
    for(uint16_t d = 20; d >= 16; d--)
        ORAMFactory::acc_GKK_BT(d, 512, 16);
}

void test_CORAM() {
    std::cout << "Circuit ORAM Benchmarks: use new formulas, counter and dynamic block-size" << std::endl;
    ORAMFactory::acc_LSO_old(1, 30, 32);
    ORAMFactory::acc_BT(30, 32, 120, 16, 8, false);
    ORAMFactory::acc_Path("Path", 30, 32, 4, 16, 89, 8);
    ORAMFactory::acc_Path("PathSC", 30, 32, 4, 16, 89, 8);
    ORAMFactory::acc_Path("Scoram", 30, 32, 6, 16, 141, 8);
    ORAMFactory::acc_Path("CORAM", 30, 32, 3, 16, 89, 8);
    ORAMFactory::acc_Path("CORAM", 30, 32, 2, 16, 32, 8);
}

void plot_add_TLS(uint64_t m, uint64_t b, CSVFileWriter *writer) {
    outType& out = TrivialLinearScan::c_write_old(m, b);
    writer->addOutType_paper(out);
    delete &out;
}

void plot_add_BT(uint64_t m, uint64_t b, CSVFileWriter *writer) {
    outType& out = acc_BT_slow(1, false, m, b, 120, 16, 8);
    writer->addOutType_paper(out);
    delete &out;
}

void plot_add_Path(uint64_t m, uint64_t b, CSVFileWriter *writer) {
    outType& out = acc_Path_slow(1, false, m, b, 4, 16, 89, 8);
    writer->addOutType_paper(out);
    delete &out;
}

void plot_add_PathSC(uint64_t m, uint64_t b, CSVFileWriter *writer) {
    outType& out = acc_PathSC_slow(1, false, m, b, 4, 16, 89, 8);
    writer->addOutType_paper(out);
    delete &out;
}

void plot_add_SCORAM(uint64_t m, uint64_t b, CSVFileWriter *writer) {
    outType& out = acc_SCORAM_slow(1, false, m, b, 6, 16, StandardScoramS(myLog2(m)).min, 8);
    writer->addOutType_paper(out);
    delete &out;
}

void SQORAM_add_CORAM(uint64_t m, uint64_t b, CSVFileWriter* writer) {
    SQR_CORAM* path = (SQR_CORAM*) ORAMFactory::create_Path("SQR_CORAM", m, b, 3, 8, 89, UINT16_MAX);       // TODO: stash?
    outType& out = path->c_acc(b);
    writer->addOutType_paper(out);
    delete &out;
    delete path;
}

void SQORAM_add_SQORAM(uint64_t m, uint64_t b, uint32_t acc, CSVFileWriter* writer) {
    outType& out = divideWR(acc_OSQR_slow(acc, false, m, b, 8, UINT16_MAX), acc);
    writer->addOutType_paper(out);
    delete &out;
}

void plot_SQORAM() {
    // set hardware parameters
    Settings::get().set(0.5, 1.03*1000, 10000*4, 80);

    CSVFileWriter* writer = new CSVFileWriter("SQR");
    writer->addHeader("d, TLS 128, SQR 128, CORAM 128, CORAM 2");
    for(uint16_t d = 2; d <= 20; d++) {
        writer->addLine(d);
        auto m = (uint64_t) pow(2, d);
        auto acc = (uint32_t) (floor(sqrt(m * d)));

        plot_add_TLS(m, 128, writer);
        SQORAM_add_SQORAM(m, 128, acc, writer);
        SQORAM_add_CORAM(m, 128, writer);
        std::cout << "Plotted d = " << d << std::endl;
    }
    writer->writeFiles();
    delete writer;
}

void plot_scalingORAM() {
    // set parameters
    Settings::get().set(0.5, 500, 10000*8, 80);

    CSVFileWriter* writer = new CSVFileWriter("scalingORAM");
    writer->addHeader("d, TLS, SQR, CORAM, CORAM2");
    for(uint16_t d = 5; d <= 24; d++) {
        writer->addLine(d);
        auto m = (uint64_t) pow(2, d);
        auto acc = (uint32_t) (floor(sqrt(m * d)));

        if(d <= 20)
            plot_add_TLS(m, 32, writer);
        else writer->addEmpty();

        SQORAM_add_SQORAM(m, 32, acc, writer);
        SQORAM_add_CORAM(m, 32, writer);
        std::cout << "Plotted d = " << d << std::endl;
    }
    writer->writeFiles();
    delete writer;
}

void plot_SCORAM() {
    CSVFileWriter* writer = new CSVFileWriter("SCORAM");
    writer->addHeader("d, TLS, BT, Path, Path-SC, SCORAM");
    for(uint16_t d = 10; d <= 29; d++) {
        writer->addLine(d);
        auto m = (uint64_t) pow(2, d);

        if(d <= 20)
            plot_add_TLS(m, 32, writer);
        else writer->addEmpty();

        plot_add_BT(m, 32, writer);
        plot_add_Path(m, 32, writer);
        plot_add_PathSC(m, 32, writer);
        plot_add_SCORAM(m, 32, writer);

        std::cout << "Plotted d = " << d << std::endl;
    }
    writer->writeFiles();
    delete writer;
}

void run_benchmarks() {
    measure(test_GKK);
    measure(test_CORAM);
    measure(plot_SCORAM);
    measure(plot_SQORAM);
    measure(plot_scalingORAM);
}