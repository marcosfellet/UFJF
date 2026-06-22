void foo(int a) // Cria a variavel "a" e copia o valor de "var" para "a" ==> Passagem por Valor
  a += 3;

void foo(int* p) //
  *p += 8;

void foo2(int& a) // Passagem por Referencia
  a += 3;

int main()
{
  int var = 15;
  foo(var); // Como se fosse a = var. var nao e alterada (enderecos separados) ==> quando a passagem por valor e utiizada, a variavel original nao e alterada
  foo(&var); // Como se fosse p = &var. Passa o endereco de "var", logo, a funcao fool consegue alerar o valor de "var" 
  foo2(var); // Como se fosse . Altera a variavel original, renomeando-a na funcao local, sem criar variaveis adicionais. Jeito que mais otimiza o uso da memoria, uma vez que 
            // nao cria variaveis adicionais
}


