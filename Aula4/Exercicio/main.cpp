#include "robot.h"

using namespace std;

int main()
{
	Robot r1, r2; // objetos da classe Robot
	float a = 0.47, b = 10.63; // parametros para mudar a velocidade
	float t = 5; // tempo em segundos
	// Robo 1
	r1.robo = {0,0}; // inicialização das posições
	r1.robo = {10,3}; // inicialização das velocidades
	r1.showPos();
	r1.move(t);
	r1.changeSpeed(a,b);
	// Robo 2
	r2.robo = {0,0}; // inicialização das posições
	r2.robo = {6,12}; // inicialização das velocidades
	r2.showPos();
	r2.move(t);
	r2.changeSpeed(a, b);


}
