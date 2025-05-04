#include "../include/qam_mod.h"
#include "../include/qam_maps.h"
#include <iostream>


namespace {
    using cd = std::complex<double>;
}


// Установливает символы на основе битов
void QAM_mod::setSymbols(const std::vector<cd>& symbolMap, const std::vector<uint8_t>& bits, size_t symbolSize, double normalizationFactor) {
    size_t numSymbols = bits.size() / symbolSize;
    symbols.resize(numSymbols); // Изменяем размер вектора символов

    for (size_t i = 0; i < numSymbols; ++i) {
        int index = 0;
        for (size_t j = 0; j < symbolSize; ++j) {
            index = (index << 1) | bits[i * symbolSize + j];
        }
        symbols[i] = symbolMap[index] / std::sqrt(normalizationFactor);
    }
}

// Модуляция на основе типа модуляции
std::vector<cd> QAM_mod::modulate(std::vector<uint8_t>& bits, ModulationType modulationType) {

    // Проверка валидности modulationType
    if (modulationData.find(modulationType) == modulationData.end()) {
        std::cerr << "Unsupported modulation type" << std::endl;
        return {};
    }

    // Извлечение символной карты, размера символа и нормализующего коэффициента
    const auto& [symbolMap, symbolSize, normalizationFactor] = modulationData.at(modulationType);


    // Добавление нулей, если размер не кратен symbolSize
    while (bits.size() % symbolSize != 0) {
        bits.push_back(0);
    }

    // Установка символов и модуляция
    setSymbols(symbolMap, bits, symbolSize, normalizationFactor);
    

    return symbols;
}
