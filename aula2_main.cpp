#include <iostream>
#include "mymath.h"


using namespace std;

int main()
{
    int a,b,s,f;
    a = 4;
    b = 20;
    s = soma(a,b);
    f = fatorial(a);
    cout<<"Resultado da soma: "<<s<<endl;
    cout<<"Resultado da do fatorial: "<<f<<endl;
    return 0;
}
