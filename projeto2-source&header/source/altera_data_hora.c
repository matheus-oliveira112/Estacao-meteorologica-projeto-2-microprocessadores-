/*
 * altera_data_hora.c
 *
 *  Created on: Jul 1, 2025
 *      Author: mathe
 */

#include "main.h"
#include "header.h"


char altera_mes() {
	char tecla_conversao[2];
	char data_hora;
	char i;

	mes:
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("MES: ");
	for(i=0; i<2; i++) {
	  	tecla_conversao[i]=le_teclas();
	  	HAL_Delay(10);
	  	char_DISPLAY(tecla_conversao[i]);
	}
	data_hora = converte_dois_digitos(tecla_conversao[0], tecla_conversao[1]);
	if (valida_mes(data_hora)) {
		mes_alterado();
	  	return data_hora;
	}
	else {
		invalido();
	  	goto mes;
	}
}

char altera_dia(char mes) {
	char tecla_conversao[2];
	char data_hora;
	char i;

	dia:
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("DIA: ");
	for(i=0; i<2; i++) {
		tecla_conversao[i]=le_teclas();
		HAL_Delay(10);
		char_DISPLAY(tecla_conversao[i]);
	}
	data_hora = converte_dois_digitos(tecla_conversao[0], tecla_conversao[1]);
	if (valida_dia(data_hora, mes)) {
		dia_alterado();
		return data_hora;
	}
	else {
		invalido();
		goto dia;
	}
}

char altera_hora() {
	char tecla_conversao[2];
	char data_hora;
	char i;

	hora:
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("HORA: ");
	for(i=0; i<2; i++) {
		tecla_conversao[i]=le_teclas();
		HAL_Delay(10);
		char_DISPLAY(tecla_conversao[i]);
	}
	data_hora = converte_dois_digitos(tecla_conversao[0], tecla_conversao[1]);
	if (valida_hora(data_hora)) {
		hora_alterada();
		return data_hora;
	}
	else {
		invalido();
		goto hora;
	}
}

char altera_minutos() {
	char tecla_conversao[2];
	char data_hora;
	char i;

	minutos:
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("MINUTO: ");
	for(i=0; i<2; i++) {
		tecla_conversao[i]=le_teclas();
		HAL_Delay(10);
		char_DISPLAY(tecla_conversao[i]);
	}
	data_hora = converte_dois_digitos(tecla_conversao[0], tecla_conversao[1]);
	if (valida_minuto(data_hora)) {
		minuto_alterado();
		return data_hora;
	}
	else {
		invalido();
		goto minutos;
	}
}
