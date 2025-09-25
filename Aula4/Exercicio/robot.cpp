#include "robot.h"
#include <iostream>
using namespace std;

/*void Conta::exibeDados()
{
	cout << "Titular: " << this->titular << endl; // this-> : ponteiro que aponta para o objeto corrente
	cout << "Numero: " << this->numero << endl;  // Objeto corrente: o objeto que invocou o "exibeDados" (c1 ou c2)
}*/

void Robot::showPos() //Exibe a posicao x e y do robo 
{
    cout << "Posição em x: " << this->robo.pos(1) << endl;
    cout << "Posição em y: " << this->robo.pos(2) << endl;
} 
void Robot::move(float t) // move o robô com as velocidades x e y pelo tempo determinado
{
    robo.pos(1) = t*robo.vel(1);
    robo.pos(2) = t*robo.vel(2);
}

void Robot::changeSpeed(float a, float b)
{
    robo.vel(1) *= t/a;
    robo.vel(2) *= t^2/b;
}
