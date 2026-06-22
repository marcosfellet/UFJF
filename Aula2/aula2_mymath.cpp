
// mymath.cpp
float potencia(float b, int exp)
{
	float r = b;
	for(int i=1;i<exp;i++)
	{
		r *=b;
	}
	return r;
}

int soma(int a, int b)
{
	return a + b;
}

int soma(int a, int b, int c)
{
	return a + b + c;
}

float soma (int a, float b)
{
	return a + b;
}

int fatorial(int a)
{
    
    int resultado = 1;

    for(int i = 1; i <= a; i++)
    {
        resultado *= i; 
        
    }
    return resultado;
    
}
