import numpy as np

def calcular_beta(f, fs) -> float:
    '''
    Calcula beta a partir da frequência de rejeição f0 e amostragem fs
    '''
    w0 = 2.0 * np.pi * f / fs
    return float(np.cos(w0))


def filtro_notch(f, fs, alpha = 0.98):
    '''
    Implementa um filtro IIR NOTCH
    Retorna os coeficientes (b, a) direto de f0, fs e alpha
    '''
    beta = calcular_beta(f, fs)
    gain = 0.5 * (1.0 + alpha)
    b = np.array([gain, -(1.0 + alpha) * beta, gain], dtype=float)
    a = np.array([1.0, -beta * (1.0 + alpha), alpha], dtype=float)
    return b, a
    

def gera_sinal(f = 200, ciclos = 100, delta_f = 210, SNR = 40, N = 30) -> list:
    '''
    Gera sinal senoidal com ruido branco

    f => frequencia do sinal, ciclos => ciclos do sinal,
    delta_f => valor de alteracao da frequencia do sinal, 
    SNR => relacao sinal ruido em dB
    N => amostrar por ciclo
    '''
    f2 = f + delta_f
    if f2 > 500:
        f2 = 500
    t1 = 1/f
    t2 = 1/f2
    t_total = t1 + t2
    Fs = N*f # frequencia de amostragem com 20 amostrar por ciclo
    t = np.arange(0, t_total, 1 / Fs)
    f_inst = np.where(t < t1, f, f2)
    T = 1/Fs
    fase = 2* np.pi * f_inst * T
    fase = fase - fase[0]
    sinal = np.sin(fase)
    potencia_sinal = np.mean(sinal ** 2)
    potencia_ruido = potencia_sinal / (10 ** (SNR / 10))
    amplitude_ruido = np.sqrt(potencia_ruido)
    num_amostras = ciclos * N # Numero de amostras
    ruido = amplitude_ruido * np.random.uniform(num_amostras)
    return sinal + ruido


# ---------------------------------------------------------------------------
# ---------------------- Parametros Sugeridos -------------------------------
# ---------------------------------------------------------------------------
# N = 6000
Mu = 1e-5
k2 = 1 - 0.96**2



