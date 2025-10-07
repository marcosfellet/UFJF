void func1(int a)
{
  a = += 3;
}
void func2(int vet[])
{
  vet[0] += 10;
}

int main()
{
  int var = 5;
  int v[3] = {10, 3};
  func1(var); // apos a execucao da funcao, a e desalocado e some apos o fim da execucao
  func2(v); // v e mantido durante a funcao, nao e desalocado, portanto, ao final da execucao da func2, v possui os valores de vet. Pois seria muito custoso para o processador 
           // desalocar e alocar grande quantidade de dados
}

