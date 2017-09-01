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
    ORAMFactory::acc_Path("CORAM", 30, 32, 4, 16, 89, 8);
}

void test_SQORAM() {
    std::cout << "Some values from Revisiting SQR Benchmarks" << std::endl;
    ORAMFactory::acc_LSO_old(1, 20, 32);
    ORAMFactory::acc_LSO_old(1, 29, 32);
    std::cout << std::endl;

    ORAMFactory::acc_LSO_old(45, 8, 32);
    ORAMFactory::acc_OSQR(45, 8, 32, 8, UINT16_MAX);
    std::cout << std::endl;

    ORAMFactory::acc_LSO_old(29, 7, 64);
    ORAMFactory::acc_OSQR(29, 7, 64, 8, UINT16_MAX);
    std::cout << std::endl;

    ORAMFactory::acc_LSO_old(8, 4, 512);
    ORAMFactory::acc_OSQR(8, 4, 512, 8, UINT16_MAX);
}

void SQORAM_add_TLS(uint64_t m, uint64_t b, CSVFileWriter* writer) {
    outType& out = TrivialLinearScan::c_write_old(m, b);
    writer->addOutType(out);
    delete &out;
}

void SQORAM_add_CORAM(uint64_t m, uint64_t b, CSVFileWriter* writer) {
    SQR_CORAM* path = (SQR_CORAM*) ORAMFactory::create_Path("SQR_CORAM", m, b, 4, 8, 89, UINT16_MAX);
    outType& out = path->c_acc(b);
    writer->addOutType(out);
    delete &out;
    delete path;
}

void SQORAM_add_SQORAM(uint64_t m, uint64_t b, uint32_t acc, CSVFileWriter* writer) {
    outType& out = divideWR(acc_OSQR_slow(acc, false, m, b, 8, UINT16_MAX), acc);
    writer->addOutType(out);
    delete &out;
}

void plot_SQORAM() {
    CSVFileWriter* writer = new CSVFileWriter("SQR");
    writer->addHeader("d, TLS 128, TLS 256, SQR 128, SQR 256, CORAM 128, CORAM 256");
    for(uint16_t d = 2; d <= 18; d++) {
        writer->addLine(d);
        auto m = (uint64_t) pow(2, d);
        auto acc = (uint32_t) (floor(sqrt(m * d)));

        SQORAM_add_TLS(m, 128, writer);
        SQORAM_add_TLS(m, 256, writer);
        SQORAM_add_SQORAM(m, 128, acc, writer);
        SQORAM_add_SQORAM(m, 256, acc, writer);
        SQORAM_add_CORAM(m, 128, writer);
        SQORAM_add_CORAM(m, 256, writer);
        std::cout << "Plotted d = " << d << std::endl;
    }
    writer->writeFiles();
    delete writer;
}


void run_benchmarks() {
    measure(test_GKK);
    measure(test_CORAM);
    measure(test_SQORAM);
    measure(plot_SQORAM);
}