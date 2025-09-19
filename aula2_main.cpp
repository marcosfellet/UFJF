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
    int f[4];
    int coe[4];
    float x1, y;
    float x[4], x_r[4];
    x[4] = 1;
    x_r[4] = 1;
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
    
    cout<<"Digite os valores de x da equação de terceiro grau na ordem decrescente: "<<endl;
    
    for(int j=0; j<3; j++)
    {
        std::cin >> x1;
        x[j] = x1;
    }
    
    for(int i=0; i<4; i++)
    {
        coe[i] = fatorial(f[i]);
    }
    
    for(int j=3; j>=1; j--)
    {
        x_r[j] = potencia(x[j], j);
    }
    
    /*
    cout<<"Resultado da soma: "<<s<<endl;
    /*cout<<"Coeficientes do polinomio: "<<endl;
    
    
    for(int i=0; i<4; i++)
    {
        cout<<coe[i]<<endl;
    }*/
    
    for(int j=0; j<3; j++)
    {
        y += x_r[j]*coe[j];
    }
    
    cout<<"Coeficientes do polinomio: "<<y<<endl;
    
    
    
    
    return 0;
}

