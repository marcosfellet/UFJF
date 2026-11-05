#=================================
#============ Imports ============
#=================================
import numpy as np
from scipy import signal
import random


def amostragem_sincrona(f_fund, harmonicos = [1, 15, 17, 25], ciclos = 100, fator_nyquist = 4):
    '''
    Funcao que calcula a frequencia de amostragem e as amostras para evitar o 
    vazamento espectral
    '''
    f_max = max(harmonicos)*f_fund
    Fs = fator_nyquist*f_max
    amostras_por_ciclo = fator_nyquist*max(harmonicos)
    amostras_sincronas = amostras_por_ciclo*ciclos
    return amostras_sincronas, Fs

def sinal(ciclos = 100, harmonicos = [1, 15, 17, 25], f_fund = 60, fase = 0, amplitude = [1, 0.2, 0.2, 0.1]):
    '''
    Funcao que retorna o sinal, o numero de amostras e a frequencia de amostragem
    '''
    N_amostras, Fs = amostragem_sincrona(f_fund, harmonicos, ciclos, fator_nyquist = 4)
    t = np.arange(N_amostras) / Fs 
    sinal_gerado = 0 
    for i in np.arange(0, len(harmonicos)):
        sinal_gerado += amplitude[i]*np.cos(2*np.pi*t*f_fund*harmonicos[i] + fase)
    return sinal_gerado, N_amostras, Fs

def filtro_notch(alpha = 0.98, f_fund = 60):
    '''
    Funcao que retorna o numerador e o denominador do filtro IIR Notch
    '''
    _, Fs = amostragem_sincrona(f_fund = f_fund)
    w_0 = 2*np.pi*f_fund/Fs
    Q_factor = w_0/(2*(1 - alpha))
    num, den = signal.iirnotch(f_fund, Q_factor, Fs)
    return num, den

def filtragem(alpha = 0.98):
    '''
    Funcao que retorna o sinal filtrado
    '''
    sinal_original, _, _ = sinal()
    num, den = filtro_notch(alpha)
    return signal.lfilter(num, den, sinal_original)

def thd(alpha=0.98):
    sinal_original, _, Fs = sinal()
    sinal_filtrado = filtragem(alpha)
    sinal_fundamental = sinal_original - sinal_filtrado

    # Calcula o raio dos polos do filtro notch
    num, den = filtro_notch(alpha)
    r = np.max(np.abs(np.roots(den)))

    # Amostras a descartar para o transitório cair para 1%
    n_trans = int(np.ceil(np.log(0.01) / np.log(r)))

    # Garante que não vai descartar mais que o sinal
    n_trans = min(n_trans, len(sinal_original) // 2)

    # RMS apenas após o transitório
    sinal_fundamental_RMS = np.sqrt(np.mean(sinal_fundamental[n_trans:]**2))
    sinal_filtrado_RMS = np.sqrt(np.mean(sinal_filtrado[n_trans:]**2))

    return sinal_filtrado_RMS / sinal_fundamental_RMS

#===============================
#========== Parametros =========
#===============================
# f_fund = 60 # frequencia em Hertz
# w = 2 * np.pi * f_fund 
# harmonicos = [1, 15, 17, 25]
# amplitudes = [1, 0.2, 0.2, 0.1]
iteracoes = 2
alpha = np.zeros(iteracoes)
thd_vals = np.zeros(iteracoes)
while(True):
    for i in range(iteracoes):
        alpha[i] = round(random.uniform(0.90, 0.99), 2)
        thd_vals[i] = round(thd(alpha[i])*100,2)
    if alpha[0] != alpha[1]:
        for j in range(iteracoes):
            print(f"THD para alpha = {alpha[j]}: {thd_vals[j]} %")
        break


