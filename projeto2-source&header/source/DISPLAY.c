/*
 * DISPLAY.c
 *
 *  Created on: Jun 28, 2025
 *      Author: mathe
 */

#include "main.h"
#include "header.h"
#define RS 9
#define EN 8


void cmd_DISPLAY(unsigned char cmd)			//envia os comandos para o DISPLAY
{
	GPIOA->ODR &= ~(1<<RS); //RS=0

	GPIOB->ODR &= 0x0FFF; //limpa a parte alta do barramento de dados
	GPIOB->ODR |= ((cmd<<8) & (0xF000)); //transfere a parte alta do comando

	GPIOA->ODR |= (1<<EN);  //EN=1
	HAL_Delay(0);
	GPIOA->ODR &= ~(1<<EN); //EN=0
	HAL_Delay(0);

	GPIOB->ODR &= 0x0FFF; //limpa a parte alta do barramento de dados
	GPIOB->ODR |= ((cmd << 12) & (0xF000)); //transfere a parte baixa do comando pro barramento de dados

	GPIOA->ODR |= (1<<EN);  //EN=1
	HAL_Delay(0);
	GPIOA->ODR &= ~(1<<EN); //EN=0
	HAL_Delay(0);
}


void INIT_DISPLAY(){

	GPIOA->ODR &= ~(1<<RS); //RS=0

	GPIOB->ODR &= 0x0FFF;
	GPIOB->ODR |= (0x30<<8) ;

	GPIOA->ODR |= (1<<EN);  //EN=1
	HAL_Delay(0);
	GPIOA->ODR &= ~(1<<EN); //EN=0
	HAL_Delay(0);

	GPIOB->ODR &= 0x0FFF;
	GPIOB->ODR |= (0x30<<8) ;

	GPIOA->ODR |= (1<<EN);  //EN=1
	HAL_Delay(0);
	GPIOA->ODR &= ~(1<<EN); //EN=0
	HAL_Delay(0);

	GPIOB->ODR &= 0x0FFF;
	GPIOB->ODR |= (0x30<<8) ;

	GPIOA->ODR |= (1<<EN);  //EN=1
	HAL_Delay(0);
	GPIOA->ODR &= ~(1<<EN); //EN=0
	HAL_Delay(0);

	GPIOB->ODR &= 0x0FFF;
	GPIOB->ODR |= (0x20<<8) ;

	GPIOA->ODR |= (1<<EN);  //EN=1
	HAL_Delay(0);
	GPIOA->ODR &= ~(1<<EN); //EN=0
	HAL_Delay(0);

	cmd_DISPLAY(0x28);
	cmd_DISPLAY(0x0C);
	cmd_DISPLAY(0x06);
}


void char_DISPLAY(unsigned char cmd)		//envia os caracteres para o DISPLAY
{
	GPIOA->ODR |= (1<<RS); //RS=1

	GPIOB->ODR &= 0x0FFF; //limpa a parte alta do barramento de dados
	GPIOB->ODR |= ((cmd<<8) & (0xF000)); //transfere a parte alta do comando

	GPIOA->ODR |= (1<<EN);  //EN=1
	HAL_Delay(0);
	GPIOA->ODR &= ~(1<<EN); //EN=0
	HAL_Delay(0);

	GPIOB->ODR &= 0x0FFF; //limpa a parte alta do barramento de dados
	GPIOB->ODR |= ((cmd << 12) & (0xF000)); //transfere a parte baixa do comando pro barramento de dados

	GPIOA->ODR |= (1<<EN);  //EN=1
	HAL_Delay(0);
	GPIOA->ODR &= ~(1<<EN); //EN=0
	HAL_Delay(0);
}


void str_DISPLAY(char *frase)			//escreve as strings no display
{
	char index = 0;
	while (frase[index] != '\0') {

		if (index == 16)
		{
			cmd_DISPLAY(0xC0);
			char_DISPLAY(frase[index]);
		}
		else
		{
			char_DISPLAY(frase[index]);
		}
		index++;
	}
}


void limpa_display() {
	cmd_DISPLAY(0x01);
	cmd_DISPLAY(0x80);
}
