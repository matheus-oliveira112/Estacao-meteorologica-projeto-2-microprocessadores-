/*
 * func_valida.c
 *
 *  Created on: Jul 1, 2025
 *      Author: mathe
 */

#include "main.h"
#include "header.h"

char valida_mes(char mes)		//Faz a validacao de mes quando o operador esta trocando a data
{
    if (mes >= 1 && mes <= 12)
        return 1;
    else
        return 0;
}

char valida_dia(char dia, char mes)			//Faz a validacao de dia quando o operador esta trocando a data
{
    if (dia < 1)
        return 0;

    // Fevereiro
    if (mes == 2)
    {
        if (dia <= 28)
            return 1;
        else
            return 0;
    }
    // Meses com 30 dias
    else if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
    {
        if (dia <= 30)
            return 1;
        else
            return 0;
    }
    // Meses com 31 dias
    else
    {
        if (dia <= 31)
            return 1;
        else
            return 0;
    }
}

char valida_hora(char hora)			//Faz a validacao da hora quando o operador esta trocando a data
{
    if (hora <= 23)
        return 1;
    else
        return 0;
}

char valida_minuto(char minuto)		//Faz a validacao dos minutos quando o operador esta trocando a data
{
    if (minuto <= 59)
        return 1;
    else
        return 0;
}

float media(float *variavel) {			//Faz a media das amostras coletadas

	float soma = 0;
	char i;

	for (i = 0; i < 10; i++) {
		soma += variavel[i];
	}

	return (soma/10.0f);
}
