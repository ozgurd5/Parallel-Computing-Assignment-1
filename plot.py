"""
Grafik üretim script'i — paralel hesaplama ödevi raporu için.

Çalıştırma:
    pip install matplotlib numpy
    python plot.py

Çıktı: images/ klasörü altında PNG dosyaları.

1M değerleri geldiğinde aşağıdaki veri sözlüklerinde None yerine ölçülen
değerleri yazın; script tekrar çalıştığında grafikler güncellenir.
"""

import os
import matplotlib.pyplot as plt
import numpy as np

OUT_DIR = "images"
os.makedirs(OUT_DIR, exist_ok=True)

ALGORITHMS = ["Quick Sort", "Merge Sort", "Bitonic Sort", "Selection Sort", "Bubble Sort"]
COLORS = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd"]
N_VALUES = [100, 1000, 10000, 100000, 1000000]

# Seri çalışma süreleri (saniye). 1M için ölçüm gelince None yerine yazın.
SERIAL = {
    100:      [0.000006, 0.000034, 0.000021, 0.000013, 0.000020],
    1000:     [0.000055, 0.000213, 0.000164, 0.000565, 0.001021],
    10000:    [0.000680, 0.001796, 0.003498, 0.051797, 0.096148],
    100000:   [0.008325, 0.023166, 0.044366, 5.101403, 16.212982],
    1000000:  [0.087513, 0.198633, 0.410216, 508.595415, 1748.750755],
}

# Paralel çalışma süreleri (saniye).
PARALLEL = {
    2: {
        100:      [0.000028, 0.000025, 0.000036, 0.000021, 0.000021],
        1000:     [0.000060, 0.000114, 0.000092, 0.000164, 0.000393],
        10000:    [0.000432, 0.000926, 0.001691, 0.013079, 0.026534],
        100000:   [0.004863, 0.010021, 0.019888, 1.299180, 3.597136],
        1000000:  [0.044705, 0.103062, 0.199577, 135.270050, 443.663357],
    },
    4: {
        100:      [0.000106, 0.000025, 0.000008, 0.000009, 0.000005],
        1000:     [0.000149, 0.000093, 0.000057, 0.000074, 0.000118],
        10000:    [0.000487, 0.000767, 0.001100, 0.003522, 0.010319],
        100000:   [0.003067, 0.005789, 0.009780, 0.356710, 0.710691],
        1000000:  [0.031562, 0.062203, 0.105127, 37.801512, 122.664615],
    },
    8: {
        100:      [None,     None,     None,     None,     None],  # 100/8 bölünemez
        1000:     [0.000209, 0.000082, 0.000043, 0.000048, 0.000049],
        10000:    [0.000515, 0.000663, 0.000702, 0.001804, 0.002963],
        100000:   [0.002743, 0.004853, 0.007375, 0.128437, 0.221019],
        1000000:  [0.019372, 0.041054, 0.064166, 10.333895, 31.593845],
    },
}


def plot_serial_times():
    """Grafik 1: Seri sürelerin N'e göre log-log değişimi."""
    plt.figure(figsize=(10, 6))
    for i, algo in enumerate(ALGORITHMS):
        xs, ys = [], []
        for n in N_VALUES:
            t = SERIAL[n][i]
            if t is not None:
                xs.append(n)
                ys.append(t)
        plt.plot(xs, ys, "o-", label=algo, color=COLORS[i], linewidth=2, markersize=7)

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Dizi Boyutu (N)", fontsize=12)
    plt.ylabel("Süre (saniye, log ölçek)", fontsize=12)
    plt.title("Seri Algoritmaların Çalışma Süreleri", fontsize=13)
    plt.grid(True, which="both", linestyle="--", alpha=0.5)
    plt.legend(fontsize=10)
    plt.tight_layout()
    path = os.path.join(OUT_DIR, "seri_sureleri.png")
    plt.savefig(path, dpi=120)
    plt.close()
    print(f"Yazildi: {path}")


def plot_serial_vs_parallel(n):
    """Grafik 2: Belirli bir N için seri vs paralel (p=2, 4, 8) bar chart."""
    labels = ALGORITHMS
    x = np.arange(len(labels))
    width = 0.2

    serial_times = SERIAL[n]
    p2 = PARALLEL[2][n]
    p4 = PARALLEL[4][n]
    p8 = PARALLEL[8][n]

    def safe(vals):
        return [v if v is not None else 0 for v in vals]

    fig, ax = plt.subplots(figsize=(11, 6))
    ax.bar(x - 1.5 * width, safe(serial_times), width, label="Seri", color="#444")
    ax.bar(x - 0.5 * width, safe(p2), width, label="Paralel p=2", color=COLORS[0])
    ax.bar(x + 0.5 * width, safe(p4), width, label="Paralel p=4", color=COLORS[1])
    ax.bar(x + 1.5 * width, safe(p8), width, label="Paralel p=8", color=COLORS[2])

    ax.set_yscale("log")
    ax.set_ylabel("Süre (saniye, log ölçek)", fontsize=12)
    ax.set_title(f"Seri vs Paralel Karşılaştırma (N = {n:,})".replace(",", "."), fontsize=13)
    ax.set_xticks(x)
    ax.set_xticklabels(labels, rotation=15)
    ax.legend(fontsize=10)
    ax.grid(True, axis="y", linestyle="--", alpha=0.5)
    plt.tight_layout()
    path = os.path.join(OUT_DIR, f"seri_vs_paralel_{n // 1000}k.png" if n >= 1000 else f"seri_vs_paralel_{n}.png")
    plt.savefig(path, dpi=120)
    plt.close()
    print(f"Yazildi: {path}")


def plot_speedup(n):
    """Grafik 3: Belirli bir N için her algoritmanın p=2/4/8 speedup eğrisi."""
    serial_times = SERIAL[n]
    p_values = [2, 4, 8]
    fig, ax = plt.subplots(figsize=(10, 6))

    for i, algo in enumerate(ALGORITHMS):
        if serial_times[i] is None:
            continue
        speedups = []
        ps = []
        for p in p_values:
            t_p = PARALLEL[p][n][i]
            if t_p is not None:
                speedups.append(serial_times[i] / t_p)
                ps.append(p)
        if not speedups:
            continue
        ax.plot(ps, speedups, "o-", label=algo, color=COLORS[i], linewidth=2, markersize=9)

    ax.plot(p_values, p_values, "k--", alpha=0.5, label="İdeal (S = p)")

    ax.set_xlabel("İşlemci Sayısı (p)", fontsize=12)
    ax.set_ylabel("Speedup (S = T_seri / T_paralel)", fontsize=12)
    ax.set_title(f"Speedup Karşılaştırması (N = {n:,})".replace(",", "."), fontsize=13)
    ax.set_xticks(p_values)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = os.path.join(OUT_DIR, f"speedup_{n // 1000}k.png" if n >= 1000 else f"speedup_{n}.png")
    plt.savefig(path, dpi=120)
    plt.close()
    print(f"Yazildi: {path}")


if __name__ == "__main__":
    plot_serial_times()
    plot_serial_vs_parallel(100000)
    plot_speedup(100000)
    plot_serial_vs_parallel(1000000)
    plot_speedup(1000000)

    print("\nTum grafikler images/ klasorunde.")
