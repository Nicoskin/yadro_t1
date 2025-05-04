#include <vector>
#include <complex>
#include <cmath>
#include <unordered_map>
#include <cstdint>

#include "qam_mod.h"
#include "model_channel.h"
#include "qam_demod.h"

double BER(std::vector<uint8_t>tx, std::vector<uint8_t>rx){
    int k = 0;
    for(int i=0; i < tx.size(); i++){
        if (tx[i] != rx[i]) k++;
    }
    if (k==0) return 0;

    //std::cout << tx.size() << "  " << k << std::endl;
    double result = static_cast<double>(k) / tx.size();
    return result;
}

// g++ main.cpp -o main qam_demod.cpp qam_mod.cpp model_channel.cpp && ./main > out.txt

int main(){

    QAM_mod QAM_mod; 
    QAM_demod QAM_demod;
    Channel Channel;

    // Цикл по разным модуляциям
    for(size_t mod=0; mod < 3; mod++ ){
        ModulationType moduletion;
        switch (mod) {
            case 0:
                moduletion = ModulationType::QPSK;
                std::cout << "QPSK" << std::endl;
                break;
            case 1:
                moduletion = ModulationType::QAM16;
                std::cout << "QAM16" << std::endl;
                break;
            case 2:
                moduletion = ModulationType::QAM64;
                std::cout << "QAM64" << std::endl;
                break;
        }
        // Цикл по разным SNR
        for(int snr=0; snr < 25; snr++){
            auto bits = generateRandBits(10000, 0);
            
            auto qam_tx = QAM_mod.modulate(bits, moduletion);
            
            auto noise = Channel.add_noise(qam_tx, snr);
            
            auto qam_rx = QAM_demod.demodulate(noise, moduletion);
            
            auto ber = BER(bits, qam_rx);
            std::cout << ber << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}