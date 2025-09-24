/*
Otimiza o jeito de manipular variaveis locais em outras funcoes, pois seu tamanho e sempre o valor do barramento da memoria, se a memoria for de 32 bits, os ponteiros terao 
32 bits, se for de 64 bits, os ponteiros terao 64 bits

Exemplo: e possivel manipular uma variavel de 4 bytes com uma variave de 32/64 bits. O tempo de deslocamento de transmissao de dados e poupado enormemente

Vetores sao ponteiros constantes, os quais podem manipular os valores em suas posicoes

*/

int vet[] = {1,2,3,4,5};
*vet += 1; // incrementa o valor de vet[0]
*(vet+1) *= 3; // multiplica o valor de vet[1] por 3   
