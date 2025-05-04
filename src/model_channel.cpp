#include "../include/model_channel.h"

using cd = std::complex<double>;

std::vector<cd> Channel::add_noise(const std::vector<cd>& signal, double SNR_dB, unsigned int seed) {
    // Вычисляем среднюю мощность сигнала
    double signal_power = 0.0;
    int no_zero = 0;
    for (const auto& s : signal) {
        signal_power += std::norm(s);  // norm дает квадрат модуля комплексного числа
        no_zero++;
    }
    signal_power /= no_zero;  // Средняя мощность сигнала

    // Перевод SNR из dB в линейное значение
    double SNR_linear = std::pow(10.0, SNR_dB / 10.0);

    // Вычисляем мощность шума, исходя из SNR
    double noise_power = signal_power / SNR_linear;

    // Настройка генератора случайных чисел с нормальным распределением и заданным seed
    std::default_random_engine generator(seed != 0 ? seed : std::random_device{}());
    std::normal_distribution distribution(0.0, std::sqrt(noise_power / 2.0));

    // Генерируем шум и добавляем его к сигналу
    std::vector<cd> noisy_signal(signal.size());
    for (size_t i = 0; i < signal.size(); ++i) {
        double real_part = distribution(generator);
        double imag_part = distribution(generator);
        noisy_signal[i] = signal[i] + cd(real_part, imag_part);
    }

    return noisy_signal;
}

// Генератор случайных бит(сид = 0 - рандомный сид)
std::vector<uint8_t> generateRandBits(size_t n, unsigned int seed) {
    std::vector<uint8_t> bits(n);
    
    // Если сид равен 0, используем случайный генератор с временем как сидом
    std::mt19937 generator(seed == 0 ? std::random_device{}() : seed);
    std::uniform_int_distribution<uint8_t> distribution(0, 1);
    
    for (size_t i = 0; i < n; ++i) {
        bits[i] = distribution(generator);
    }
    
    return bits;
}