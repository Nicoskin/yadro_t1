#include "qam_demod.h"
#include "qam_mod.h"
#include "qam_maps.h"

namespace {
    using cd = std::complex<double>;
    using namespace std::complex_literals;  
}


void QAM_demod::generateConstellation(int modulation) {
    switch (modulation) {
        case 2:
            constellation = QPSK_MAP;
            for (auto& point : constellation) {
                point /= std::sqrt(2);
            }
            break;
        case 4:
            constellation = QAM16_MAP;
            for (auto& point : constellation) {
                point /= std::sqrt(10);
            }
            break;
        case 6:
            constellation = QAM64_MAP;
            for (auto& point : constellation) {
                point /= std::sqrt(42);
            }
            break;
    }
}

// Вычисляет мягкие решения на остнове евклидова растояния
std::vector<double> QAM_demod::calculat_softDecision(const std::complex<double>& point) {
    std::vector<double> distances;

    for (const auto& symbol : constellation) {
        double distance = std::pow(std::abs(point - symbol), 2);
        distances.push_back(distance);
    }

    return distances;
}

// Возвращает матрицу мягких решений(строка для 1 cd символа)
std::vector<std::vector<double>> QAM_demod::softDecision(const std::vector<cd>& receivedSignal) {
    std::vector<std::vector<double>> softDecisions;

    for (const auto& point : receivedSignal) {
        softDecisions.push_back(calculat_softDecision(point));
    }

    return softDecisions;
}


// Демодуляция
// Возвращает вектор бит
std::vector<uint8_t> QAM_demod::demodulate(const std::vector<cd>& receivedSignal, ModulationType modulationType) {
    std::vector<uint8_t> bits;
    const auto& [symbolMap, bitsPerSymbol, normalizationFactor] = modulationData.at(modulationType);
    generateConstellation(bitsPerSymbol);

    auto softD = softDecision(receivedSignal);

    // Преобразуем каждый мягкий символ в биты
    for (const auto& symbolDistances : softD) {
        // Найти индекс минимального расстояния вручную
        double minDistance = symbolDistances[0];
        int index = 0;

        for (size_t i = 1; i < symbolDistances.size(); ++i) {
            if (symbolDistances[i] < minDistance) {
                minDistance = symbolDistances[i];
                index = i;
            }
        }
        // Преобразовать индекс в битовую последовательность
        for (int i = bitsPerSymbol - 1; i >= 0; --i) {
            bits.push_back((index >> i) & 1);
        }
    }

    return bits;
}
