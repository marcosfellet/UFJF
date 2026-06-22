#include "robot.h"
#include <iostream>
using namespace std;

void Robot::showPos() //Exibe a posicao x e y do robo 
{
    cout << "Robo: " << this->robo.id << endl;
    cout << "Posicao em x: " << this->robo.pos[0] << endl;
    cout << "Posicao em y: " << this->robo.pos[1] << endl;
} 
void Robot::move(float t) // move o robô com as velocidades x e y pelo tempo determinado
{
    robo.pos[0] = t*robo.vel[0];
    robo.pos[1] = t*robo.vel[1];
}

void Robot::changeSpeed(float a, float b) // muda a velocidade com que o robo se locomove 
{
    robo.vel[0] *= b*1.57/a;
    robo.vel[1] *= (a*a)/b;
}

