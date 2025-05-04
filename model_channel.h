#include <vector>
#include <complex>
#include <random>
#include <iostream>


using cd = std::complex<double>;

class Channel {
    public:
        std::vector<cd> add_noise(const std::vector<cd>& signal, double SNR_dB, unsigned int seed = 0);
};

std::vector<uint8_t> generateRandBits(size_t n, unsigned int seed);