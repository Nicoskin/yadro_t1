import matplotlib.pyplot as plt
import numpy as np


with open("out.txt", "r") as f:
    data = f.read()

    # Парсинг данных
    lines = data.strip().split('\n')
    modulations = []
    ber_values = []

    current_mod = None
    for line in lines:
        if line in ["QPSK", "QAM16", "QAM64"]:
            current_mod = line
            modulations.append(current_mod)
            ber_values.append([])
        else:
            bers = list(map(float, line.split()))
            ber_values[-1].extend(bers)

# Диапазон SNR 
snr = np.arange(len(ber_values[0]))


plt.figure(figsize=(10, 6))

for i, mod in enumerate(modulations):
    plt.semilogy(snr, ber_values[i], label=mod, marker='o', linestyle='-')

plt.xlabel("SNR (dB)")
plt.ylabel("BER")
plt.grid(True, which="both", ls="--")
plt.legend()
plt.show()