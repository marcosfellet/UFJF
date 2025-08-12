import numpy as np
from matplotlib import pyplot as plt
import plotly.graph_objects as go
# Variaveis conhecidas 
Rs = 10*1000 # resistencia de fonte
R1 = 51*1000 # resistencia de base 1
R2 = 5.5*1000 # resistencia de base 2
Rf = [82000,160000,47000] # resistencia de realimentacao
RC = 10*1000 # resistencia de coletor
RE = 0.5*1000 # resistencia de emissor
re = 1.5 # resistencia do modelo T



# Variacao de beta do TBJ 
lista_beta = np.logspace(0, 4, num=1000)

# Configuração do Grafico

fig = go.Figure()
list_color = ["blue","red","green"]


# Modelagem do ganho A (V/A) e Beta (A/V)
for val_color, rf_val in enumerate(Rf): 
    beta = -1/rf_val
    lista_ganho = []
    for val, beta_tbj in enumerate(lista_beta):
        A = -beta_tbj*((1/RC + 1/rf_val) ** (-1))*(((1/R1 + 1/Rs + 1/R2 + 1/rf_val)**(-1))/((beta_tbj+1)*(RE + re) + (1/R1 + 1/Rs + 1/R2 + 1/rf_val)))
        Af = (A/(1+A*beta))/10000
        lista_ganho.append(np.abs(Af)) 

    fig.add_trace(
        go.Scatter(
            x=lista_beta,
            y=lista_ganho,
            mode="lines",
            name=f"ganho para Rf = {rf_val}",
            line=dict(color=list_color[val_color]),
        )
    )

fig.update_layout(
    title=f"vo/vi",
    xaxis=dict(type = 'log',title="Beta", gridcolor="lightgray"),
    yaxis=dict(title="Ganho (V/V)", gridcolor="lightgray",range=[0,20]),
    template="plotly_white",
    height=500,
    width=800,
)

fig.show()



    
        
    




























