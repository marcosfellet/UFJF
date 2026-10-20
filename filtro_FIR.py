#======================================
#============= Imports ================
#======================================
import numpy as np
from scipy.fft import fft, fftfreq
from scipy import signal
import matplotlib.pyplot as plt
from math import gcd


#======================================
#============ IFuncoes ================
#======================================

def amostragem_sincrona(f_fund, harmonicos, ciclos = 100, fator_nyquist = 4):
    '''
    Função responsável por escolher a quantidade de amostras, 
    a frequência de amostragem e o número de ciclos
    para evitar vazamento espectral
    '''
    f_max = max(harmonicos)*f_fund
    Fs = fator_nyquist*f_max
    amostras_por_ciclo = fator_nyquist*max(harmonicos)
    amostras_sincronas = amostras_por_ciclo*ciclos
    return amostras_sincronas, Fs

def sinal(ciclos = 100, harmonicos = [5, 7, 9, 21, 23, 25], f_fund = 90, amplitude = 1, fase = 0):
    '''
    Gera um sinal com componentes harmonicas
    '''
    harmonicos.insert(0, 1) # adiciona a frequencia fundamental a posicao 0 do vetor
    N_amostras, Fs = amostragem_sincrona(f_fund, harmonicos, ciclos, fator_nyquist = 4)
    t = np.arange(N_amostras) / Fs # vetor de tempo
    sinal = 0 # valor inicial para o loop
    # loop que computa o sinal com harmonicas
    for i in harmonicos:
        if i != 1:
            amplitude = 0.3*amplitude

        sinal += amplitude*np.cos(2*np.pi*t*f_fund*i + fase)
        
    
    return sinal, N_amostras, Fs

def numero_coeficientes(delta_f, Fs):
    numero_coef = int(np.ceil(1.65 * Fs / delta_f))
    if numero_coef % 2 == 0:
        numero_coef += 1 # corrige caso o numero de coeficientes der par (LP => impar)
    return numero_coef

def filtro_FIR(delta_f, freq_corte, Fs):
    '''
    Filtro passa-baixas com janela de Hamming
    '''
    num_coef = numero_coeficientes(delta_f, Fs)
    return signal.firwin(num_coef, freq_corte, window='hamming', fs=Fs)

def filtro_FIR_bd(delta_f, banda, desejado, Fs):
    '''
    Filtro passa-banda com janela de Hamming
    '''
    num_coef = numero_coeficientes(delta_f, Fs)
    return signal.firls(num_coef, banda, desejado, fs=Fs)


#======================================
#=========== paramentros ==============
#======================================
# Ultimos 3 numeros da minha matricula 009 => f = 90
f_fund = 90
amplitude = 1
fator_corte = 2
harmonicos = [5, 7, 9, 21, 23, 25]
evitar_zero_log = 1e-12
banda = []
for harmonico in harmonicos:
    banda.append(harmonico*f_fund)
desejado = [0, 0, 0, 1, 1, 0]
#======================================
#=========== sinal ====================
#======================================

sinal_original, numero_amostras, Fs = sinal()
# Espectro em frequencia do sinal
sinal_espectro = fft(sinal_original)
sinal_freq = fftfreq()
sinal_freq_pos = sinal_freq[:numero_amostras // 2]

#======================================
#=========== filtro LP ================
#======================================

freq_corte = f_fund*fator_corte
delta_f = np.abs(freq_corte - f_fund*harmonicos[0]) # faixa de transicao
coef_filtro_LP = filtro_FIR(delta_f, freq_corte, Fs)
freq_filtro_LP, resposta_filtro_LP = signal.freqz(coef_filtro_LP, worN=4096, fs = Fs)
mag_LP = 20*np.log10(resposta_filtro_LP + evitar_zero_log)
# Sinal filtrado: 
sinal_filtrado_LP = signal.lfilter(coef_filtro_LP, 1, sinal_original) # Filtro causal

#======================================
#=========== filtro BP ================
#======================================
filtro_bp = filtro_FIR_bd(delta_f, banda, desejado, Fs)
freq_filtro_BP, resposta_filtro_BP = signal.freqz(filtro_bp, worN=4096, fs = Fs)
mag_BP = 20*np.log10(resposta_filtro_BP + evitar_zero_log)

#======================================
#============== Plots =================
#======================================
fig, axs = plt.subplots(2,2, figsize = (15,10))
eixo_temporal = sinal_original[:1/f_fund]
# espectro do sinal original
axs[0].plot(eixo_temporal, sinal_original, linewidth = 1.5, color = 'blue')
axs[0].set_xlabel("Tempo (s)")
axs[0].set_ylabel("Amplitude do Sinal")
axs[0].set_title("Sinal no Domínio do Tempo", fontweight = 'bold')
axs[0].grid('True', alpha = 0.3)









