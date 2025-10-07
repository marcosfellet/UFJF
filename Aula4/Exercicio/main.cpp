/*******************************************************************************************************************************************
		Exercicio utilizando Struct e Class 
o intuito e simular, de maneira muito simplificada, o movimento de 2 robos. Dessa forma, a variavel "robo" e criada com o tipo definido como 
"Parametros". Ela e utilizada pela classe "Robot", a qual possiu as funcoes necessarias para manipular as informacoes fornecidas pelas 
variaveis "robo".
********************************************************************************************************************************************/
#include "robot.h"

using namespace std;

int main()
{
	Robot r1, r2; // objetos da classe Robot
	float a = 0.47, b = 10.63; // parametros para mudar a velocidade
	float t = 5; // tempo em segundos
	// Robo 1
	r1.robo.id = 1; // identificacao dos robos
	// inicialização das posições
	r1.robo.pos[0] = 0;
	r1.robo.pos[1] = 0;
	// inicialização das velocidades
	r1.robo.vel[0] = 10;
	r1.robo.vel[1] = 3;
	r1.move(t);
	r1.changeSpeed(a,b);
	r1.showPos();
	// Robo 2
	r2.robo.id = 2; // identificacao dos robos
	// inicialização das posições
	r2.robo.pos[0] = 0;
	r2.robo.pos[1] = 0;
	// inicialização das velocidades
	r2.robo.vel[0] = 50;
	r2.robo.vel[1] = 0.42;
	r2.move(t);
	r2.changeSpeed(a, b);
	r2.showPos();


}
