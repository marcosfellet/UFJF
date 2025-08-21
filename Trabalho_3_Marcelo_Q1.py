import numpy as np
import plotly.graph_objects as go
# Variaveis conhecidas
 
Rs = 10e3 # resistencia de fonte
R1 = 51e3  # resistencia de base 1
R2 = 5.5*1000 # resistencia de base 2
Rf = [160e3, 82e3 ,47e3 ] # resistencia de realimentacao
RC = 10e3 # resistencia de coletor
RE = 0.5e3  # resistencia de emissor
r_pi = 4.570e3 # resistencia do modelo pi 
I_C = 5.47e-6 # corrente C.C. de coletor 
VT = 25e-3 # tensao de limiar

# Configuracao do Grafico
# Lista de estilos de linha e cores
line_styles = ["solid", "dash", "dot"]
colors = ["blue", "red", "green"]


# Criacao da figura
fig = go.Figure()

# Loop para cada Rf
for i, rf_val in enumerate(Rf):
    beta = -1 / rf_val
    lista_ganho = []
    for beta_tbj in np.logspace(0, 4, num=1000):
        gm = beta_tbj *(I_C/VT)
        A = -gm*(((1/RC)+(1/rf_val))**(-1))*(((1/Rs)+(1/R2)+(1/R1)+(1/rf_val)+(1/r_pi))**(-1))
        Af = A / (1 + A*beta)
        Af_v =  Af / Rs
        lista_ganho.append(np.abs(Af_v))
    
    # Adiciona cada traco com legenda em Latex
    fig.add_trace(
        go.Scatter(
            x=np.logspace(0, 4, num=1000),
            y=lista_ganho,
            mode="lines",
            name=f"$R_F = {rf_val / 1000:.0f} \, \mathrm{{k\Omega}}$",  # Notacao Latex
            line=dict(
                color=colors[i],
                dash=line_styles[i],  # Estilo de linha (solido, tracejado, pontilhado,)
                width=2,
            ),
        )
    )

# Ajustes do layout para coincidir com a imagem
fig.update_layout(
    title= None,

        xaxis=dict(
        type="log",
        title=None,
        showline=True,
        linecolor="black",
        linewidth=2,
        #tickvals=[10, 100, 1000],
    ),
    yaxis=dict(
        title=None,
        showline=True,
        linecolor="black",
        linewidth=2,
        range=[0, 20],
    ),
    plot_bgcolor="white",
    annotations=[
        dict(
            x=1.15, 
            y=-0.01,  
            xref="paper",
            yref="paper",  # Usar "paper" para coordenadas normalizadas
            text="β",
            showarrow=False,
            font=dict(size=14, color="black"),
            xanchor="left",
            yanchor="top",
        ),
        dict(
            x=0.20, y=0.97, xref="x", yref="paper",
            text="|vo/vi|", showarrow=False,
            font=dict(size=14, color="black"),
            xanchor="center", yanchor="bottom",
        ),
    ],
    margin=dict(l=50, r=50, t=50, b=50),
    height=500,
    width=800,  
)

#fig.show()


fig.write_image("grafico_ganho.png") 

    
# Para comparar o valor de Af_v teorico com o obtido pela simulacao  
beta_tbj = 200
rf_val = 47e3
gm = beta_tbj *(I_C/VT)
beta = -1/rf_val
A = -gm*(((1/RC)+(1/rf_val))**(-1))*((1/Rs)+(1/R2)+(1/R1)+(1/rf_val)+(1/r_pi))**(-1)
Af = A / (1 + A*beta)
Af_v =  Af / Rs      
print("/////////////////////////////////////////////////"),
print(Af_v),
print("/////////////////////////////////////////////////"),
print(A),
print(1+beta*A),

