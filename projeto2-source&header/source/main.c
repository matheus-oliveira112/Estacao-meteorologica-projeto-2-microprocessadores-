/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "header.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

//VARIAVEL QUE SIMULA OS SEGUNDO DENTRO DO RELOGIO INTERNO
char contador = 0;

//VARIAVEL QUE GUARDA O DIA, MES, HORA E MINUTO DO RELOGIO INTERNO
char str[4] = {5, 7, 14, 0};  // {dia, mês, hora, minuto}

// Tabela de dias por mês (índice 0 = janeiro)
const char dias_por_mes[12] = {
	31, // Janeiro
	28, // Fevereiro
	31, // Março
	30, // Abril
	31, // Maio
	30, // Junho
	31, // Julho
	31, // Agosto
	30, // Setembro
	31, // Outubro
	30, // Novembro
	31  // Dezembro
};

//VARIAVEL PRINCIPAL DA MAQUINA DE ESTADOS
char estado = 0;

//GUARDAM OS VALORES MAXIMOS DE VELOCIDADE DO VENTO E NIVEL DO RIO
float vento_max_km = 72;
float vento_max_m = 20;
float nivel_max_m = 5;
float nivel_max_cm = 500;

//VARIAVEIS QUE SETAM A UNIDADE DE TEMPERATURA, VELOCIDADE DO VENTO E NIVEL DO RIO
char unidade_temperatura = 0;
char unidade_vento = 0;
char unidade_rio = 0;

//VARIAVEIS UTILIZADAS NA HORA DE ARMAZENAR AS VARIAVEIS
char inc = 0;
float temperatura[10] = {0};
float radiacao[10] = {0};
float vento[10] = {0};
float rio[10] = {0};
float media_temperatura = 0;
float media_radiacao = 0;
float media_vento = 0;
float media_rio = 0;

//VARIAVEIS UTILIZADAS PARA GUARDAR AS ULTIMAS AFERICOES
float ultimo_temperatura = 0;
float ultimo_radiacao = 0;
float ultimo_vento = 0;
float ultimo_rio = 0;
char ultimo_horario[4] = {0};

//VARIAVEIS UTILIZADAS NA HORA DE VERIFICAR OS VALORES MAXIMOS
char contador_rio = 0;
char modo = 4;
float ultimo_vento_extremo_km;
float ultimo_vento_extremo_m;
float ultimo_rio_extremo_m;
float ultimo_rio_extremo_cm;
char ultimo_relogio[4];

//VARIAVEL UTILIZADA PARA MEDIR A FREQUENCIA DA ONDA PULSANTE NO PINO PA.2
volatile float frequency = 0.0f;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	char tecla;
	char i;
	char senha[4];
	char nivel;
	char bloco = 0;


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // CONFIGURA A PRIORIDADE DAS INTERRUPCOES
  HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 0);		//Timer 1 com a maior prioridade, gerando uma onda de 480hz no pino PB.11
  HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
  HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);		//Timer 2 com a segunda maior prioridade, capturando a frequencia da onda que pulsa em PA.2
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);		//Timer 3 com a terceira maior prioridade, com uma interrupcao a cada 1 segundo
  HAL_NVIC_EnableIRQ(TIM3_IRQn);

  //INICIA OS TIMERS E ADC
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
  HAL_ADC_Start(&hadc1);
  HAL_TIM_Base_Start_IT(&htim1);

  //DESLIGA O TIMER 1
  TIM1->CR1 &= ~(1<<0);
  GPIOB->ODR &= ~(1<<11);

  INIT_DISPLAY();

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  ////ESTADO ONDE SÃO LIDAS AS SENHAS
  	  if (estado == 0) {
  		limpa_display();
  		HAL_Delay(10);
  		str_DISPLAY("======ARMs====================== ");		//Funcao que escreve strings no display
  		le_teclas();
  		limpa_display();
  		HAL_Delay(10);
  		str_DISPLAY("Digite a senha: ");
  		cmd_DISPLAY(0xC0);
  		for(i=0; i<4; i++) {
  			senha[i] = le_teclas();
  			HAL_Delay(10);
  			char_DISPLAY('*');
  		}
  		if ((senha[0]=='3')&&(senha[1]=='2')&&(senha[2]=='5')&&(senha[3]=='8')) {
  			estado = 1;			//SETA VARIAVEL ESTADO PARA O ESTADO SEGUINTE
  			nivel = 0;			//VARIAVEL QUE SETA COMO MODO OPERADOR NOS ESTADOS SEGUINTES
  			modo_operador();
  		}
  		else if ((senha[0]=='1')&&(senha[1]=='2')&&(senha[2]=='6')&&(senha[3]=='9')) {
  		  	estado = 1;
  		  	nivel = 1;			//VARIAVEL QUE SETA COMO MODO ADMINISTRADOR NOS ESTADOS SEGUINTES
  		  	modo_adm();
  		}
  		else {
  			senha_invalida();
  		}
  	  }

  	  ////ESTADO ONDE APARECEM AS OPCOES DO MENU
  	  if(estado==1) {

  		  ///BLOCO ONDE APARECEM APENAS AS OPÇÕES DO MODO OPERADOR
  		  if (bloco==0) {

  		  limpa_display();
  		  HAL_Delay(10);
  		  str_DISPLAY("1-UNID. 2-MONIT 3-EVENTO EXTR. ");
  		  tecla = le_teclas();

  		  //TROCA DE UNIDADADES
  		  if(tecla=='1') {
  			troca_unidade:
  			limpa_display();
  			HAL_Delay(10);
  			str_DISPLAY("1-TEMP. 2-VEL_V 3-NIVEL DO RIO ");
  			tecla = le_teclas();

  			switch (tecla) {
  			//TROCA DE UNIDADE DA VARIAVEL TEMPERATURA
  			case ('1'): {
  				temperatura:
  				limpa_display();
  				HAL_Delay(10);
  				str_DISPLAY("1-Celsius       2-Fahrenheit ");
  				tecla = le_teclas();
  				if (tecla=='1') {
  					setando_unidade();
  					unidade_temperatura = 0;		//FLAG QUE SETA UNIDADE COMO GRAUS CELCIUS
  					inc = 0;
  				}
  				else if (tecla=='2') {
  					setando_unidade();
  				  	unidade_temperatura = 1;		//FLAG QUE SETA UNIDADE COMO FAHRENHEIT
  				  	inc = 0;
  				 }
  				else {
  					digito_invalido();
  					goto temperatura;
  				}
  				break;
  			}
  			//TROCA DE UNIDADE DA VARIAVEL VELOCIDADE DO VENTO
  			case ('2'): {
  				vento:
  				limpa_display();
  			  	HAL_Delay(10);
  			  	str_DISPLAY("1-Km/h  2-m/s ");
  			  	tecla = le_teclas();
  			  	if (tecla=='1') {
  			  		setando_unidade();
  			    	unidade_vento = 0;				//FLAG QUE SETA UNIDADE COMO KM/H
  			    	inc = 0;
  			    }
  			  	else if (tecla=='2') {
  			  		setando_unidade();
  			    	unidade_vento = 1;				//FLAG QUE SETA UNIDADE COMO M/S
  			    	inc = 0;
  			    }
  			    else {
  			    	digito_invalido();
  			    	goto vento;
  			    }
  			  	break;
  			}
  			//TROCA DE UNIDADE DA VARIAVEL NIVEL DO RIO
  			case ('3'): {
  				rio:
  			  	limpa_display();
  			  	HAL_Delay(10);
  			  	str_DISPLAY("1-Metros  2-Cm ");
  			  	tecla = le_teclas();
  			  	if (tecla=='1') {
  			    	setando_unidade();
  			    	unidade_rio = 0;				//FLAG QUE SETA UNIDADE COMO M
  			    	inc = 0;
  			    }
  			    else if (tecla=='2') {
  			    	setando_unidade();
  			    	unidade_rio = 1;				//FLAG QUE SETA UNIDADE COMO CM
  			    	inc = 0;
  			    }
  			    else {
  			    	digito_invalido();
  			    	goto rio;
  			    }
  			  	break;
  			}
  			default: {
  				digito_invalido();
  				goto troca_unidade;
  			}
  			}
  		  }

  		  //MONITORAMENTO EM TEMPO REAL DAS VARIAVEIS
  		  else if (tecla=='2'){
  			  estado = 2; 	//Vai para o estado 2 onde seta a variavel que se quer monitar em tempo real
  		  }

  		  //MONITORAMENTO DO ULTIMO EVENTO EXTREMO
  		  else if (tecla=='3') {
  			  estado = 3;	//Vai para o estado 3 onde se visualiza o ultimo evento extremo
  		  }

  		  //LOGOUT
  		  else if (tecla=='9') {
  			  estado = 0;
  		  }

  		  //TROCA PARA OS OPÇÕES DO MODO ADM, isto caso tenha feito o login como adm
  		  else if ((tecla =='*')&&(nivel==1)) {
  			  bloco = 1;
  		  }

  		  else {
  			  digito_invalido();
  		  }
  		}

  		  ///BLOCO ONDE APARECEM APENAS AS OPÇÕES DO MODO ADMINISTRADOR
  		  if (bloco==1) {

  			limpa_display();
  			HAL_Delay(10);
  			str_DISPLAY("4-ALT. D&H      5-ALT. VALORES ");
  			tecla = le_teclas();

  			//ALTERAR DATA E HORA
  			if (tecla=='4') {
  				str[1] = altera_mes();
  				str[0] = altera_dia(str[1]);
  				str[2] = altera_hora();
  				str[3] = altera_minutos();
  			}

  			//ALTERA VALOR MAXIMO DA VELOCIDADE DO VENTO E NIVEL DO RIO
  			else if (tecla=='5') {
  				valor_MAXIMO:
  				limpa_display();
  				HAL_Delay(10);
  				str_DISPLAY("1-VEL_VENTO     2-NIVEL DO RIO ");
  				tecla = le_teclas();

  				//ALTERA VALOR MAXIMO DA VELOCIDADE DO VENTO
  				if(tecla=='1') {
  				valor_vento_max:
  				limpa_display();
  				HAL_Delay(10);
  				str_DISPLAY("1-Valor em Km/h 2-Valor em m/s ");
  				tecla = le_teclas();
  				switch (tecla) {
  				case ('1'): {
  					vento_max_km = altera_vento();			//Altera a velocidade do vento em km/h
  					vento_max_m = vento_max_km/(3.6f);
  					break;
  				}
  				case ('2'): {
  					vento_max_m = altera_vento();			//Altera a velocidade do vento em m/s
  					vento_max_km = vento_max_km*(3.6f);
  					break;
  				}
  				default: {
  					digito_invalido();
  					goto valor_vento_max;
  				}
  				}
  			}
  				//ALTERA VALOR MAXIMO DO NIVEL DO RIO
  				else if(tecla=='2') {
  	  			valor_nivel_max:
  	  			limpa_display();
  	  			HAL_Delay(10);
  	  			str_DISPLAY("1-Valor em m    2-Valor em cm ");
  	  			tecla = le_teclas();

  	  			switch (tecla) {
  	  			case ('1'): {
  	  				nivel_max_m = altera_nivel_rio('1');		//Altera o nivel do rio em metros
  	  				nivel_max_cm = nivel_max_m*(100);
  	  				break;
  	  			}
  	  			case ('2'): {
  	  				nivel_max_cm = altera_nivel_rio('2');		//Altera o nivel do rio em cm
  	  				nivel_max_m = nivel_max_cm/(100);
  	  				break;
  	  			}
  	  			default: {
  	  				digito_invalido();
  	  				goto valor_nivel_max;
  	  			}
  	  			}
  	  			}

  				else {
  					digito_invalido();
  					goto valor_MAXIMO;
  				}
  			}

  			//LOGOUT
  			else if (tecla=='9') {
  			  	estado = 0;
  			  	bloco = 0;
  			}

  			//TROCA PARA OS OPÇÕES DO MODO operador novamente
  			else if (tecla =='*') {
  			  	bloco = 0;
  			}

  			else {
  				digito_invalido();
  			}
  		  }

  	  }

  	  ////ESTADO ONDE SÃO MONITORADAS AS VARIAVEIS EM TEMPO REAL
  	  if (estado==2) {
  		monitora:
  		limpa_display();
  		HAL_Delay(10);
  		str_DISPLAY("1-TEMP.   2-RAD.3-VEL_VEN 4-RIO ");
  		tecla = le_teclas();

  		switch (tecla) {
  		//MONITORA A VARIAVEL TEMPERATURA
  		case ('1'): {
  			escreve_hora(ultimo_horario);		//Escreve no display a data e hora do monitoramento
  			cmd_DISPLAY(0xC0);
  			HAL_Delay(10);
  			str_DISPLAY("TEMP: ");
  			escreve_variavel(ultimo_temperatura);		//Escreve no display a temperatura da ultima amostra
  			if(unidade_temperatura==0) {
  				HAL_Delay(10);
  				str_DISPLAY("C ");
  			}
  			else if (unidade_temperatura==1) {
  				HAL_Delay(10);
  				str_DISPLAY("F ");
  			}
  			HAL_Delay(3500);
  		  	goto monitora;
  		}
  		//MONITORA A VARIAVEL RADIACAO SOLAR
  		case ('2'): {
  			escreve_hora(ultimo_horario);
  			cmd_DISPLAY(0xC0);
  			HAL_Delay(10);
  			str_DISPLAY("RAD: ");
  			escreve_variavel(ultimo_radiacao);			//Escreve no display a luminosidade da ultima amostra
  			HAL_Delay(10);
  			str_DISPLAY("Lumens ");

  			HAL_Delay(3500);
  		  	goto monitora;
  		}
  		//MONITORA A VARIAVEL VELOCIDADE DO VENTO
  		case ('3'): {
  			//display
  			escreve_hora(ultimo_horario);
  			cmd_DISPLAY(0xC0);
  			HAL_Delay(10);
  			str_DISPLAY("VENTO: ");
  			escreve_variavel(ultimo_vento);				//Escreve no display a velocidade do vento da ultima amostra
  			if(unidade_vento==0) {
  			  	HAL_Delay(10);
  			  	str_DISPLAY("Km/h ");
  			}
  			else if (unidade_vento==1) {
  			  	HAL_Delay(10);
  			  	str_DISPLAY("m/s ");
  			}
  			HAL_Delay(3500);
  		  	goto monitora;
  		}
  		//MONITORA A VARIAVEL NIVEL DO RIO
  		case ('4'): {
  			//display
  			escreve_hora(ultimo_horario);
  			cmd_DISPLAY(0xC0);
  			HAL_Delay(10);
  			str_DISPLAY("RIO: ");
  			escreve_variavel(ultimo_rio);				//Escreve no display o nivel do rio da ultima amostra
  			if(unidade_rio==0) {
  			  	HAL_Delay(10);
  			  	str_DISPLAY("m ");
  			}
  			else if (unidade_rio==1) {
  			  	HAL_Delay(10);
  			  	str_DISPLAY("cm ");
  			}
  			HAL_Delay(3500);
  		  	goto monitora;
  		}
  		//VOLTA PARA O MENU
  		case ('9'): {
  		  	estado = 1;
  		  	break;
  		}
  		default: {
  		  	  digito_invalido();
  		  	  goto monitora;
  		}
  		  	  			}
  	  }

  	  ////ESTADO ONDE APARECE O ULTIMO EVENTO EXTREMO
  	  if (estado==3) {
		  limpa_display();

		  //EVENTO ONDE O NIVEL DO RIO E A VELOCIDADE DO VENTO ESTAO ACIMA DOS LIMITES
		  if (modo==1) {
			  escreve_hora(ultimo_relogio);			//Escreve no display a data e hora do evento extremo
			  cmd_DISPLAY(0xC0);
			  HAL_Delay(10);
			  str_DISPLAY("RIO: ");
			  if(unidade_rio==0) {
				  	escreve_variavel(ultimo_rio_extremo_m);		//Escreve no display o valor do nivel do rio no evento extremo, em metros
			    	HAL_Delay(10);
			    	str_DISPLAY("m ");
			  }
			  else if (unidade_rio==1) {
				  	escreve_variavel(ultimo_rio_extremo_cm);	//Escreve no display o valor do nivel do rio no evento extremo, em centimetros
			    	HAL_Delay(10);
			    	str_DISPLAY("cm ");
			  }
			  HAL_Delay(10);
			  str_DISPLAY("  & ");
			  HAL_Delay(4500);
			  limpa_display();
			  HAL_Delay(10);
			  str_DISPLAY("VENTO: ");
			  if(unidade_vento==0) {
				  	escreve_variavel(ultimo_vento_extremo_km);		//Escreve no display a velocidade do vento no evento extremo, em km/h
			    	HAL_Delay(10);
			    	str_DISPLAY("Km/h ");
			  }
			  else if (unidade_vento==1) {
				  	escreve_variavel(ultimo_vento_extremo_m);		//Escreve no display a velocidade do vento no evento extremo, em m/s
			    	HAL_Delay(10);
			    	str_DISPLAY("m/s ");
			  }
		  }
		  //EVENTO ONDE SOMENTE O NIVEL DO RIO ESTA ACIMA DOS LIMITES
		  else if(modo==2) {
			  escreve_hora(ultimo_relogio);
			  cmd_DISPLAY(0xC0);
			  HAL_Delay(10);
			  str_DISPLAY("RIO: ");
			  if(unidade_rio==0) {
				  	escreve_variavel(ultimo_rio_extremo_m);
				  	HAL_Delay(10);
				  	str_DISPLAY("m ");
			  }
			  else if (unidade_rio==1) {
				  	escreve_variavel(ultimo_rio_extremo_cm);
				  	HAL_Delay(10);
				  	str_DISPLAY("cm ");
			  }
		  }
		  //EVENTO ONDE SOMENTE A VELOCIDADE DO VENTO ESTA ACIMA DOS LIMITES
		  else if(modo==3) {
			  escreve_hora(ultimo_relogio);
			  cmd_DISPLAY(0xC0);
			  HAL_Delay(10);
			  str_DISPLAY("VENTO: ");
			  if(unidade_vento==0) {
				  escreve_variavel(ultimo_vento_extremo_km);
				  HAL_Delay(10);
				  str_DISPLAY("Km/h ");
			  }
			  else if (unidade_vento==1) {
				  escreve_variavel(ultimo_vento_extremo_m);
				  HAL_Delay(10);
				  str_DISPLAY("m/s ");
			  }
		  }
		  //AINDA NAO OCORRERAM EVENTOS EXTREMOS
		  else if (modo==4) {
			  HAL_Delay(10);
			  str_DISPLAY("NAO OCORRERAM   EVENTOS EXT. ");
		  }
		  HAL_Delay(4500);
		  estado = 1;
  	  	  }


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 149;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 20000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 3599;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */

/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Buzzer_Pin|Barramento_de_dados_Pin|Barramento_de_dadosB13_Pin|Barramento_de_dadosB14_Pin
                          |Barramento_de_dadosB15_Pin|LED_VERMELHO_Pin|Coluna_1_Pin|Coluna_2_Pin
                          |Coluna_3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, E_Pin|RS_Pin|LED_AMARELO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Linha_1_Pin Linha_2_Pin Linha_3_Pin Linha_4_Pin */
  GPIO_InitStruct.Pin = Linha_1_Pin|Linha_2_Pin|Linha_3_Pin|Linha_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Buzzer_Pin Barramento_de_dados_Pin Barramento_de_dadosB13_Pin Barramento_de_dadosB14_Pin
                           Barramento_de_dadosB15_Pin LED_VERMELHO_Pin Coluna_1_Pin Coluna_2_Pin
                           Coluna_3_Pin */
  GPIO_InitStruct.Pin = Buzzer_Pin|Barramento_de_dados_Pin|Barramento_de_dadosB13_Pin|Barramento_de_dadosB14_Pin
                          |Barramento_de_dadosB15_Pin|LED_VERMELHO_Pin|Coluna_1_Pin|Coluna_2_Pin
                          |Coluna_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : E_Pin RS_Pin LED_AMARELO_Pin */
  GPIO_InitStruct.Pin = E_Pin|RS_Pin|LED_AMARELO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */

/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)	//interrupcao a cada subida de borda de onda no pino PA.2
{
    if(htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        static uint32_t last = 0;
        uint32_t now = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);		//Captura o numero de contagens quando se tem uma subida
        uint32_t diff;

        if (now >= last)
            diff = now - last;					//numero de contagens da frequencia da onda que esta pulsando no pino PA.2
        else
            diff = (0xFFFF - last + now + 1);

        last = now;

        if (diff != 0)
        {
            frequency = 1000000.0f / (float)diff;			//RETORNA A FREQUENCIA DA ONDA
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	    {
	        GPIOB->ODR ^= (1<<11);		//Quando o nivel do rio estiver 3 minutos acima do nivel maximo, e ligada uma onda de 480hz no pino PB.11
	    }

	else if (htim->Instance == TIM3)
	{
		 contador++;			//Variavel que simula os segundos

		 //RELOGIO INTERNO
		 if (contador >= 60) {
		 		contador = 0;
		 		str[3]++; // minutos

		 		if (str[3] >= 60) {
		 			str[3] = 0;
		 			str[2]++; // horas

		 			if (str[2] >= 24) {
		 				str[2] = 0;
		 				str[0]++; // dias

		 				char dias_no_mes = dias_por_mes[str[1] - 1];

		 				if (str[0] > dias_no_mes) {
		 					str[0] = 1;
		 					str[1]++; // meses

		 					if (str[1] > 12) {
		 						str[1] = 1;
		 					}
		 				}
		 			}
		 		}
		 	}

		 //PARTE QUE AS AMOSTRAS SAO COLETADAS
		 if (contador % 6 == 0) {			//Amostras a cada 6 segundos

			//FAZ AS LEITURAS DOS PINOS A/D E DO SENSOR DE TEMPERATURA
			ADC_ChannelConfTypeDef sConfig = {0};

			// === Radiação solar ===
			sConfig.Channel = ADC_CHANNEL_0;
			sConfig.Rank = ADC_REGULAR_RANK_1;
			sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
			HAL_ADC_ConfigChannel(&hadc1, &sConfig);
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			float valor_rad = HAL_ADC_GetValue(&hadc1);			//Coleta valor digital referente a radicao solar, pino PA.0

			// === Nível do rio ===
			sConfig.Channel = ADC_CHANNEL_1;
			HAL_ADC_ConfigChannel(&hadc1, &sConfig);
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			float valor_rio = HAL_ADC_GetValue(&hadc1);			//Coleta valor digital referente ao nivel do rio, pino PA.1

			// === Temperatura interna ===
			sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
			HAL_ADC_ConfigChannel(&hadc1, &sConfig);
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			float valor_temp = HAL_ADC_GetValue(&hadc1);		//Coleta valor digital referente ao valor da temperatura interna, a partir do sensor

			//COLETA AS AMOSTRAS DE TEMPERATURA, RADIACAO, NIVEL DO RIO E VELOCIDADE DO VENTO
		 	temperatura[inc] = getTemperatura(valor_temp);			//valor em celsius
		 	radiacao[inc] = getRadiacao(valor_rad); 				//valor em lumens
		 	rio[inc] =	getNivel_rio(valor_rio);					//valor em m
		 	vento[inc] = getVento(frequency);  						//valor em km/h

		 	if (unidade_temperatura == 1) {
		 		//Faz a conversão da amostra para F
		 		temperatura[inc] = (temperatura[inc]*1.8f)+32.0f;			//valor em fahrenheit
		 	}
		 	if (unidade_vento == 1) {
		 		//Faz a conversão da amostra para m/s
		 		vento[inc] = vento[inc]/3.6f;								//valor em m/s
		 	}
		 	if (unidade_rio == 1) {
		 		//Faz a conversão da amostra para cm
		 		rio[inc] = rio[inc]*100.0f;									//valor em cm
		 	}

		 	//Guarda as ultimas afericoes e horario/data
		 	ultimo_temperatura = temperatura[inc];
		 	ultimo_radiacao = radiacao[inc];
		 	ultimo_vento = vento[inc];
		 	ultimo_rio = rio[inc];
		 	ultimo_horario[1] = str[1];		//guarda o mes
		 	ultimo_horario[0] =	str[0];		//dia
		 	ultimo_horario[2] =	str[2];		//hora
		 	ultimo_horario[3] = str[3];		//minuto

		 	inc++;		//incrementa a variavel que esta sendo utilizada na coleta das amostras

		 	if (contador==0) {
		 		//FAZ AS MEDIAS DAS VARIAVEIS E AVALIA SE HOUVERAM EVENTO EXTREMOS
		 		if(inc==10) {
		 			media_temperatura = media(temperatura);		//MEDIA DA TEMPERATURA
		 			media_radiacao = media(radiacao);			//MEDIA DA RADIACAO SOLAR
		 			media_vento = media(vento);					//MEDIA DA VELOCIDADE DO VENTO
		 			media_rio = media(rio);						//MEDIA DO NIVEL DO RIO


		 			//AVALIA SE TANTO O NIVEL DO RIO COMO A VELOCIDADE DO VENTO ESTAO ACIMA DO MAXIMO
		 			if(((unidade_vento==0)&&(unidade_rio==0)&&(media_vento>vento_max_km)&&(media_rio>nivel_max_m)) || ((unidade_vento==1)&&(unidade_rio==0)&&(media_vento>vento_max_m)&&(media_rio>nivel_max_m)) ||
		 					((unidade_vento==0)&&(unidade_rio==1)&&(media_vento>vento_max_km)&&(media_rio>nivel_max_cm)) || ((unidade_vento==1)&&(unidade_rio==1)&&(media_vento>vento_max_m)&&(media_rio>nivel_max_cm))) {
		 				GPIOB->ODR |= (1<<5);	//ACENDE LED VERMELHO (NIVEL DO RIO)
		 				GPIOA->ODR |= (1<<12);	//ACENDE LED AMARELO (VELOCIDADE DO VENTO)
		 				modo = 1;					//flag utilizada na main dentro do estado 3
		 				contador_rio++;				//flag utilizada para checar se o nivel do rio esta 3 minutos acima do limite
		 				if(unidade_vento==0) {							//guarda o valor extremo da velocidade do vento
		 					ultimo_vento_extremo_km = media_vento;
		 					ultimo_vento_extremo_m = ultimo_vento_extremo_km/(3.6f);
		 				}
		 				else if(unidade_vento==1) {
		 					ultimo_vento_extremo_m = media_vento;
		 					ultimo_vento_extremo_km = ultimo_vento_extremo_m*(3.6f);
		 				}
		 				if (unidade_rio==0) {							//guarda o valor extremo do nivel do rio
		 					ultimo_rio_extremo_m = media_rio;
		 					ultimo_rio_extremo_cm = ultimo_rio_extremo_m*(100.0f);
		 				}
		 				else if (unidade_rio==1) {
		 					ultimo_rio_extremo_cm = media_rio;
		 					ultimo_rio_extremo_m = ultimo_rio_extremo_cm/(100.0f);
		 				}
		 				ultimo_relogio[1] = str[1];		//guarda o mes
		 				ultimo_relogio[0] =	str[0];		//dia
		 				ultimo_relogio[2] =	str[2];		//hora
		 				ultimo_relogio[3] = str[3];		//minuto
		 				if(contador_rio>=3) {
		 					TIM1->CR1 |= (1<<0);		//Liga o buzzer apos 3 minutos do nivel do rio acima do valor limite
		 				}
		 			}
		 			//AVALIA SE SOMENTE O NIVEL DO RIO ESTA ACIMA DO MAXIMO
		 			else if(((unidade_rio==0)&&(media_rio>nivel_max_m))||((unidade_rio==1)&&(media_rio>nivel_max_cm))) {
		 				GPIOB->ODR |= (1<<5);	//ACENDE LED VERMELHO (NIVEL DO RIO)
		 				GPIOA->ODR &= ~(1<<12);	//APAGA LED AMARELO (VELOCIDADE DO VENTO)
		 				modo = 2;
		 				contador_rio++;
		 				if (unidade_rio==0) {							//guarda o valor extremo do nivel do rio
		 					ultimo_rio_extremo_m = media_rio;
		 					ultimo_rio_extremo_cm = ultimo_rio_extremo_m*(100.0f);
		 				}
		 				else if (unidade_rio==1) {
		 					ultimo_rio_extremo_cm = media_rio;
		 					ultimo_rio_extremo_m = ultimo_rio_extremo_cm/(100.0f);
		 				}
		 				ultimo_relogio[1] = str[1];		//guarda o mes
		 				ultimo_relogio[0] =	str[0];		//dia
		 				ultimo_relogio[2] =	str[2];		//hora
		 				ultimo_relogio[3] = str[3];		//minuto
		 				if(contador_rio>=3) {
		 					TIM1->CR1 |= (1<<0);		//Liga o buzzer apos 3 minutos do nivel do rio acima do valor limite
		 				}
		 			}
		 			//AVALIA SE SOMENTE A VELOCIDADE DO VENTO ESTA ACIMA DO MAXIMO
		 			else if(((unidade_vento==0)&&(media_vento>vento_max_km))||((unidade_vento==1)&&(media_vento>vento_max_m))) {
		 				GPIOB->ODR &= ~(1<<5);	//APAGA LED VERMELHO (NIVEL DO RIO)
		 				GPIOA->ODR |= (1<<12);	//ACENDE LED AMARELO (VELOCIDADE DO VENTO)
		 				modo = 3;
		 				contador_rio = 0;
		 				if(unidade_vento==0) {							//guarda o valor extremo da velocidade do vento
		 					ultimo_vento_extremo_km = media_vento;
		 					ultimo_vento_extremo_m = ultimo_vento_extremo_km/(3.6f);
		 				}
		 				else if(unidade_vento==1) {
		 					ultimo_vento_extremo_m = media_vento;
		 					ultimo_vento_extremo_km = ultimo_vento_extremo_m*(3.6f);
		 				}
		 				ultimo_relogio[1] = str[1];		//guarda o mes
		 				ultimo_relogio[0] =	str[0];		//dia
		 				ultimo_relogio[2] =	str[2];		//hora
		 				ultimo_relogio[3] = str[3];		//minuto
		 				TIM1->CR1 &= ~(1<<0);		//Desliga o buzzer
		 			}
		 			//AVALIA QUANDO NAO HA EVENTOS EXTREMOS
		 			else {
		 				GPIOB->ODR &= ~(1<<5);	//APAGA LED VERMELHO (NIVEL DO RIO)
		 				GPIOA->ODR &= ~(1<<12);	//APAGA LED AMARELO (VELOCIDADE DO VENTO)
		 				contador_rio = 0;
		 				TIM1->CR1 &= ~(1<<0);		//Desliga o buzzer
		 			}

		 		}
		 		inc = 0;
		 	}
		 }
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
