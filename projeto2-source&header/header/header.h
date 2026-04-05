/*
 * header.h
 *
 *  Created on: Jun 28, 2025
 *      Author: mathe
 */

#ifndef INC_HEADER_H_
#define INC_HEADER_H_

//Funcoes teclado.c
char le_teclas(void);
short debounce(char d);
char converte_dois_digitos(char c1, char c2);
float converte_tres_digitos_float(char c1, char c2, char c3);
float converte_tres_digitos_metros(char c1, char c2, char c3);
float converte_tres_digitos_centimetros(char c1, char c2, char c3);
void converte_inteiro_para_dois_digitos(char valor, char *digito1, char *digito2);
char converte_float_para_digitos(float valor, char *digitos);

//Funcoes DISPLAY.c
void cmd_DISPLAY(unsigned char cmd);
void INIT_DISPLAY();
void char_DISPLAY(unsigned char cmd);
void str_DISPLAY(char *frase);
void limpa_display();

//Funcoes set_unidades.c
void digito_invalido();
void setando_unidade();
void modo_operador();
void modo_adm();
void senha_invalida();
void invalido();
void mes_alterado();
void dia_alterado();
void hora_alterada();
void minuto_alterado();
void vento_alterado();
void nivel_rio_alterado();

//Funcoes func_valida.c
char valida_mes(char mes);
char valida_dia(char dia, char mes);
char valida_hora(char hora);
char valida_minuto(char minuto);
float media(float *variavel);

//Funcoes altera_data_hora.c
char altera_mes();
char altera_dia(char mes);
char altera_hora();
char altera_minutos();

//Funcoes altera_valores.c
float altera_vento();
float altera_nivel_rio(char d);

//Funcoes coleta_variaveis.c
float getTemperatura(float adc_raw);
float getRadiacao(float dado);
float getNivel_rio(float dado);
float getVento(float dado);

//Funcoes escreve.c
void escreve_hora(char *ultimo_horario);
void escreve_variavel(float ultimo);


#endif /* INC_HEADER_H_ */
