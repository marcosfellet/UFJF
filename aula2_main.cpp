/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include "mymath.h"


using namespace std;

int main()
{
    int a,b,s,c;
    int f[4], coe[4];
    float x, y;
    float x_r;
    /*a = 10;
    b = 20;
    s = soma(a,b);*/
    cout<<"Digite os coeficientes da equação de terceiro grau: "<<endl;
    cout<<"apenas número inteiros, do maior para o menor grau "<<endl;
    for(int j=0; j<4; j++)
    {
        std::cin >> c;
        f[j] = c;
    }
    
    cout<<"Digite o valor de x da equação de terceiro grau "<<endl;
        
    std::cin >> x;
    
    for(int i=0; i<4; i++)
    {
        coe[i] = fatorial(f[i]);
    }

    x_r = potencia(x, 3);

    
    /*
    cout<<"Resultado da soma: "<<s<<endl;
    /*cout<<"Coeficientes do polinomio: "<<endl;
    
    
    for(int i=0; i<4; i++)
    {
        cout<<coe[i]<<endl;
    }*/
    
    for(int j=0; j<4; j++)
    {
        y += x_r*coe[j];
    }
    
    cout<<"Coeficientes do polinomio: "<<y<<endl;
    
    
    
    
    return 0;
}

