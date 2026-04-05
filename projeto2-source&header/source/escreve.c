/*
 * escreve.c
 *
 *  Created on: Jul 4, 2025
 *      Author: mathe
 */

#include "main.h"
#include "header.h"

void escreve_hora(char *ultimo_horario) {
	char d1,d2;
	char i;

	limpa_display();
	for (i=0; i<4; i++) {
	  		converte_inteiro_para_dois_digitos(ultimo_horario[i], &d1, &d2);
	  		HAL_Delay(10);
	  		char_DISPLAY(d1);
	  		HAL_Delay(10);
	  		char_DISPLAY(d2);
	  		if(i==0) {
	  			HAL_Delay(10);
	  			char_DISPLAY('/');
	  		}
	  		if(i==1) {
	  			HAL_Delay(10);
	  			char_DISPLAY(' ');
	  		}
	  		if(i==2) {
	  			HAL_Delay(10);
	  			char_DISPLAY(':');
	  		}
	  }
}

void escreve_variavel(float ultimo) {
	char aux;
	char digitos[4];
	char i;

	aux = converte_float_para_digitos(ultimo, digitos);

	 for(i=0; i<aux; i++) {
	  	HAL_Delay(10);
	  	char_DISPLAY(digitos[i]);
	  	if(i==(aux-2)) {
	  		HAL_Delay(10);
	  		char_DISPLAY('.');
	  	}
	 }

}
