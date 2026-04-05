/*
 * teclado.c
 *
 *  Created on: Jun 28, 2025
 *      Author: mathe
 */

#include "main.h"
#include "header.h"
#define BOUNCE 7


char le_teclas(void) {
	char col;
	char lin;
	char teclas[4][3] = {
		{'1','2','3'},
		{'4','5','6'},
		{'7','8','9'},
		{'*','0','#'}
	};

	while(1) {		//Fica preso ate retornar um tecla

	for (col = 7; col < 10; col++) {
		// Coloca todas colunas em 1
		GPIOB->ODR |= (1<<7);		//PB7, PB8 e PB9
		GPIOB->ODR |= (1<<8);
		GPIOB->ODR |= (1<<9);

		// Coloca apenas a coluna atual em 0
		GPIOB->ODR &= ~(1 << col);

		HAL_Delay(0);

		// Verifica se alguma linha esta em 0
		for (lin = 0; lin < 4; lin++) {
			if (!(debounce(lin + 4))) {  // PB3-PB6 -> linhas
				while (!(debounce(lin + 4))); // Espera soltar para o programa seguir
				//Comeca a ler a proxima tecla somente depois da tecla atual ser solta
				return teclas[lin][col-7];
			}
		}
	}
	}
}


short debounce(char d) {

	short key_last = 0;
	short key_now;
	char count = 0;

	while(count<BOUNCE) {

		HAL_Delay(0);
		key_now = ((GPIOA->IDR) & (1<<d));
		if(key_now==key_last){
			count++;
		}
		else {
			count = 0;
		}
		key_last = key_now;
	}


	return key_now;
}

char converte_dois_digitos(char c1, char c2)		//Funcao que converte dois caracteres em um valor em char, ex: '1' e '1' em 11
{
    // Subtrai '0' para converter de ASCII para número
    int dezena = c1 - '0';
    int unidade = c2 - '0';

    return (char)(dezena * 10 + unidade);			//retorna o valor na variavel char
}


float converte_tres_digitos_float(char c1, char c2, char c3)		//Funcao que converte tres caracteres em um valor em float com uma casa decimal
{
    int dezena = c1 - '0';
    int unidade = c2 - '0';
    int decimal = c3 - '0';

    float resultado = (dezena * 10) + unidade + (decimal / 10.0f);

    return resultado;			//retorna o valor na variavel float
}

float converte_tres_digitos_metros(char c1, char c2, char c3)		//Funcao que converte tres caracteres em um valor em float com duas casas decimais
{
    int inteiro = c1 - '0';     // Dígito antes da vírgula
    int decimo = c2 - '0';      // Primeira casa decimal
    int centesimo = c3 - '0';   // Segunda casa decimal

    float resultado = inteiro + (decimo / 10.0f) + (centesimo / 100.0f);

    return resultado;			//retorna o valor na variavel float
}

float converte_tres_digitos_centimetros(char c1, char c2, char c3)		//Funcao que converte tres caracteres em um valor em float sem casas decimais
{
    int centena = c1 - '0';
    int dezena = c2 - '0';
    int unidade = c3 - '0';

    float resultado = (centena * 100.0f) + (dezena * 10.0f) + unidade;

    return resultado;		//retorna o valor na variavel float
}

void converte_inteiro_para_dois_digitos(char valor, char *digito1, char *digito2)		//Converte um valor entre 0 e 99 em dois caracteres
{
    if (valor < 0) valor = 0;
    if (valor > 99) valor = 99;

    char dezena = valor / 10;
    char unidade = valor % 10;

    *digito1 = '0' + dezena;			//retorna nos ponteiros os caracteres
    *digito2 = '0' + unidade;
}

char converte_float_para_digitos(float valor, char *digitos)			//Converte um valor com no maximo 4 digitos em ate 4 caracteres, sendo
{																			//limitado a apenas uma casa decimal
	if (valor < 0.0f)
	        valor = 0.0f;
	if (valor > 999.9f)
	        valor = 999.9f;

	int parte_inteira = (int)valor;
	int parte_decimal = (int)((valor - parte_inteira) * 10 + 0.5f);

	// Corrige caso arredonde para 10
	if (parte_decimal > 9)
	     parte_decimal = 9;

	int num_digitos = 0;

	if (parte_inteira >= 100) {
	     digitos[num_digitos++] = '0' + (parte_inteira /100);
	     digitos[num_digitos++] = '0' + ((parte_inteira/10)%10);
	     digitos[num_digitos++] = '0' + (parte_inteira%10);
	 }
	 else if (parte_inteira >=10) {
	     digitos[num_digitos++] = '0' + (parte_inteira /10);
	     digitos[num_digitos++] = '0' + (parte_inteira%10);
	 }
	 else {
	    digitos[num_digitos++] = '0' + parte_inteira;
	 }

	 digitos[num_digitos++] = '0' + parte_decimal;

	 return num_digitos;				//retorna o numero de digitos e nos ponteiros os caracteres
}


