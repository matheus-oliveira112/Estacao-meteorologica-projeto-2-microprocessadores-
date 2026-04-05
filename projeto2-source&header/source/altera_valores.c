/*
 * altera_vento.c
 *
 *  Created on: Jul 2, 2025
 *      Author: mathe
 */

#include "main.h"
#include "header.h"

float altera_vento() {
	char tecla_conversao[4];
	char velocidade_vento;
	char i;

	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("Velocidade: ");
	cmd_DISPLAY(0xC0);
	for(i=0; i<3; i++) {
		  tecla_conversao[i]=le_teclas();
		  HAL_Delay(10);
		  char_DISPLAY(tecla_conversao[i]);
		  if(i==1) {
			  char_DISPLAY('.');
		  }
	}
	velocidade_vento = converte_tres_digitos_float(tecla_conversao[0], tecla_conversao[1], tecla_conversao[2]);
	vento_alterado();
	return velocidade_vento;
}

float altera_nivel_rio(char d) {
	char tecla_conversao[3];
	char nivel_rio;
	char i;

	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("Nivel do rio: ");
	cmd_DISPLAY(0xC0);
	for(i=0; i<3; i++) {
		  tecla_conversao[i]=le_teclas();
		  HAL_Delay(10);
		  char_DISPLAY(tecla_conversao[i]);
		  if((i==0)&&(d=='1')) {
			  char_DISPLAY('.');
		  }
	}
	if (d=='1') {
	nivel_rio = converte_tres_digitos_metros(tecla_conversao[0], tecla_conversao[1], tecla_conversao[2]);
	}
	else if (d=='2') {
	nivel_rio = converte_tres_digitos_centimetros(tecla_conversao[0], tecla_conversao[1], tecla_conversao[2]);
	}
	nivel_rio_alterado();
	return nivel_rio;
}

