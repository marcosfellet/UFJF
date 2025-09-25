#ifndef ROBOT_H
#define ROBOT_H

#include <string>

struct Parametros
	{
	    float pos[2]; // posicao em x e y
	    float vel[2]; // velocidade em em x e y
	}; 
	
class Robot
{
private: //Atributos privados só podem ser acessados pela classe e seus metodos
	    //Atributo privado
public://Podem ser acessados direto no main
	 //Atributo publico
	Parametros robo;
    //int id[2];
    // Metodo publico
	void showPos(); //Exibe a posicao x e y do robo 
	void move(float t); // move o robô com as velocidades x e y pelo tempo determinado
	void changeSpeed(float, float); // função que modifica as velocidades x e y

};

#endif
