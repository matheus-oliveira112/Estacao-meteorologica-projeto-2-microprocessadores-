/*
 * set_unidades.c
 *
 *  Created on: Jul 1, 2025
 *      Author: mathe
 */


#include "main.h"
#include "header.h"

void digito_invalido() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("DIGITO INVALIDO! ");
	HAL_Delay(1500);
}

void setando_unidade() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("Setando           unidade... ");
	HAL_Delay(1500);
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("UNIDADE SETADA! ");
	HAL_Delay(1500);
}

void modo_operador() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("Verificando... ");
	HAL_Delay(1500);
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("MODO OPERADOR ");
	HAL_Delay(1500);
}

void modo_adm() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("Verificando... ");
	HAL_Delay(1500);
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("MODO ADM ");
	HAL_Delay(1500);
}

void senha_invalida() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("Verificando... ");
	HAL_Delay(1500);
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("SENHA INVALIDA! ");
	HAL_Delay(1500);
}

void invalido() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("INVALIDO!       DIGITE NOVAMENTE ");
	HAL_Delay(1500);
}

void mes_alterado() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("MES ALTERADO! ");
	HAL_Delay(1500);
}

void dia_alterado() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("DIA ALTERADO! ");
	HAL_Delay(1500);
}

void hora_alterada() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("HORA ALTERADA! ");
	HAL_Delay(1500);
}

void minuto_alterado() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("MINUTO ALTERADO! ");
	HAL_Delay(1500);
}

void vento_alterado() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("VELOCIDADE MAX. ALTERADA! ");
	HAL_Delay(1500);
}

void nivel_rio_alterado() {
	limpa_display();
	HAL_Delay(10);
	str_DISPLAY("NIVEL MAX. DO   RIO ALTERADO! ");
	HAL_Delay(1500);
}

