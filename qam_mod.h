#ifndef QAM_MOD_H
#define QAM_MOD_H

#include <vector>
#include <complex>
#include <cmath>
#include <unordered_map>
#include <cstdint>

using cd = std::complex<double>;

// Перечисление типов модуляции
enum ModulationType {
    QPSK,
    QAM16,
    QAM64
};

class QAM_mod {
public:
    // Модуляция на основе битовой матрицы и типа модуляции
    std::vector<cd> modulate(std::vector<uint8_t>& bits, ModulationType modulationType);

private:
    std::vector<cd> symbols;
    
    // Установить символы на основе карты символов и битов
    void setSymbols(const std::vector<cd>& symbolMap, const std::vector<uint8_t>& bits, size_t symbolSize, double normalizationFactor);

};

#endif // QAM_MOD_H