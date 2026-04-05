/*
 * coleta_variaveis.c
 *
 *  Created on: Jul 3, 2025
 *      Author: mathe
 */

#include "main.h"
#include "header.h"

//Coleta os valores da temperatura
float getTemperatura(float adc_raw) {
	const float Vref = 3.3f;          // Tensão de referência (V)
	const float V25 = 1.43f;          // Tensão típica a 25 °C
	const float Avg_Slope = 0.0043f;  // Slope típico (V/°C)

	// Converte ADC raw para tensão
	float Vsense = adc_raw * Vref / 4095.0f;

	// Calcula temperatura em graus Celsius
	float temperature = ((Vsense - V25) / Avg_Slope) + 25.0f;

	return temperature;
}

//Coleta os valores da radiacao solar
float getRadiacao(float dado) {

	const float radiacao_max = 300.0f;

	float rad = dado * radiacao_max / 4095.0f;

	return rad;
}

//Coleta os valores do nivel do rio
float getNivel_rio(float dado) {

	const float nivel_max = 9.90f;

	float rio = dado * nivel_max / 4095.0f;

	return rio;
}

//Coleta os valores da velocidade do vento a partir da frequencia da onda que pulsa no pino PA.2
float getVento(float dado) {
	const float vento_max = 90.0f;

	if(dado>60.0f) {
		dado = 0.0f;
	}

	float vento = dado * vento_max / 60.0f;

	return vento;
}
