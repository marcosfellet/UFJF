#include "conta.h"
#include <iostream>
using namespace std;

void Conta::exibeDados()
{
	cout << "Titular: " << this->titular << endl; // this-> : ponteiro que aponta para o objeto corrente
	cout << "Numero: " << this->numero << endl;  // Objeto corrente: o objeto que invocou o "exibeDados" (c1 ou c2)
}
