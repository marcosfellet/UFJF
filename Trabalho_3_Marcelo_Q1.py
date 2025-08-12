import numpy as np
import plotly.graph_objects as go
# Variaveis conhecidas 
Rs = 10e3 # resistencia de fonte
R1 = 51e3  # resistencia de base 1
R2 = 5.5*1000 # resistencia de base 2
Rf = [160e3, 82e3 ,47e3 ] # resistencia de realimentacao
RC = 10e3 # resistencia de coletor
RE = 0.5e3  # resistencia de emissor
re = 1.5 # resistencia do modelo T


# Configuração do Grafico
# Lista de estilos de linha e cores
line_styles = ["solid", "dash", "dot"]
colors = ["blue", "red", "green"]


# Criação da figura
fig = go.Figure()

# Loop para cada Rf
for i, rf_val in enumerate(Rf):
    beta = -1 / rf_val
    lista_ganho = []
    for beta_tbj in np.logspace(0, 4, num=1000):
        A = -beta_tbj * ((1/RC + 1/rf_val)**(-1)) * ((((1/R1 + 1/Rs + 1/R2 + 1/rf_val)**(-1)) + (beta_tbj + 1)*(RE + re)) / ((beta_tbj + 1)*(RE + re) ))
        Af = A / (1 + A*beta)
        Af_v =  Af / Rs
        lista_ganho.append(np.abs(Af_v))
    
    # Adiciona cada traço com legenda em LaTeX
    fig.add_trace(
        go.Scatter(
            x=np.logspace(0, 4, num=1000),
            y=lista_ganho,
            mode="lines",
            name=f"$R_F = {rf_val / 1000:.0f} \, \mathrm{{k\Omega}}$",  # Notação LaTeX
            line=dict(
                color=colors[i],
                dash=line_styles[i],  # Estilo de linha (sólido, tracejado, pontilhado,)
                width=2,
            ),
        )
    )

# Ajustes do layout para coincidir com a imagem
fig.update_layout(
    title="|vo/vi|",

    xaxis=dict(
        type="log",
        title="β",
        range=[0, 4],  # 10^0 a 10^4 (1 a 10000)
        showline=True,
        linewidth=2,
        linecolor="black",
        ticks="outside",
    ),

    yaxis=dict(
        title="Ganho (V/V)",
        range=[0, 25],
        showline=True,
        linewidth=2,
        linecolor="black",
        ticks="outside",
    ),
    plot_bgcolor="white",
    legend=dict(
        x=0.8,  # Posição da legenda (direita)
        y=1,
        bgcolor="rgba(255, 255, 255, 0.5)",
    ),
    height=500,
    width=800,  
)

#fig.show()


fig.write_image("grafico_ganho.png") 

    
# Para comparar o valor de Af_v teorico com o obtido pela simulacao  
beta_tbj = 100
rf_val = 82e3
beta = -1/rf_val
A = -beta_tbj * ((1/RC + 1/rf_val)**(-1)) * ((((1/R1 + 1/Rs + 1/R2 + 1/rf_val)**(-1)) + (beta_tbj + 1)*(RE + re)) / ((beta_tbj + 1)*(RE + re) ))
Af = A / (1 + A*beta)
Af_v =  Af / Rs      
print(Af_v)
    




























