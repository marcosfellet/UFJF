import numpy as np
import matplotlib.pyplot as plt

i_l = np.array([0, 0.07 , 0.19, 0.27, 0.37, 0.44, 0.55, 0.62, 0.74, 0.8, 0.89, 0.97, 1.05, 1.11, 1.19, 1.25])

v_t = np.array([220, 218, 216, 214, 213, 211, 209, 207, 205, 203, 201, 199, 197, 195, 193, 191])


fig, ax = plt.subplots()
ax.plot(i_l, v_t, label = 'Gerador CC Shunt', linewidth = 2)
ax.set(xlabel = 'IL (A)', ylabel = 'VT (V)', title = 'Tensão de Terminal x Corrente de Linha')
ax.legend(loc='upper right')
fig.savefig('./VTxIL')
plt.show()


