/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f421_wk_config.h
  * @brief    header file of work bench config
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* define to prevent recursive inclusion -----------------------------------*/
#ifndef __AT32F421_WK_CONFIG_H
#define __AT32F421_WK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes -----------------------------------------------------------------------*/
#include "stdio.h"
#include "at32f421.h"

/* private includes -------------------------------------------------------------*/
/* add user code begin private includes */
#include "PC_comm_handle.h"
/* add user code end private includes */

/* exported types -------------------------------------------------------------*/
/* add user code begin exported types */

/* add user code end exported types */

/* exported constants --------------------------------------------------------*/
/* add user code begin exported constants */

/* add user code end exported constants */

/* exported macro ------------------------------------------------------------*/
/* add user code begin exported macro */

/* add user code end exported macro */

/* add user code begin dma define */
/* user can only modify the dma define value */
#define DMA1_CHANNEL1_BUFFER_SIZE   0
#define DMA1_CHANNEL1_MEMORY_BASE_ADDR   0
#define DMA1_CHANNEL1_PERIPHERAL_BASE_ADDR  0

//#define DMA1_CHANNEL2_BUFFER_SIZE   1024
//#define DMA1_CHANNEL2_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL2_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL3_BUFFER_SIZE         PC_RX_BUFF_SIZE
//#define DMA1_CHANNEL3_MEMORY_BASE_ADDR    (uint32_t)pc_data.rx_buff
//#define DMA1_CHANNEL3_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL4_BUFFER_SIZE         PC_TX_BUFF_SIZE 
//#define DMA1_CHANNEL4_MEMORY_BASE_ADDR    (uint32_t)pc_data.tx_buff
//#define DMA1_CHANNEL4_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL5_BUFFER_SIZE   0
//#define DMA1_CHANNEL5_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL5_PERIPHERAL_BASE_ADDR   0
/* add user code end dma define */

/* Private defines -------------------------------------------------------------*/
#define DG1_PIN    GPIO_PINS_13
#define DG1_GPIO_PORT    GPIOC
#define SPEAK_PIN    GPIO_PINS_14
#define SPEAK_GPIO_PORT    GPIOC
#define SLEEP_G_PIN    GPIO_PINS_15
#define SLEEP_G_GPIO_PORT    GPIOC
#define CHECK_FAN_PIN    GPIO_PINS_1
#define CHECK_FAN_GPIO_PORT    GPIOA
#define AD_HOT_PIN    GPIO_PINS_2
#define AD_HOT_GPIO_PORT    GPIOA
#define KEY_CH1_PIN    GPIO_PINS_3
#define KEY_CH1_GPIO_PORT    GPIOA
#define KEY_CH2_PIN    GPIO_PINS_4
#define KEY_CH2_GPIO_PORT    GPIOA
#define KEY_CH3_PIN    GPIO_PINS_5
#define KEY_CH3_GPIO_PORT    GPIOA
#define HOT_PWM_PIN    GPIO_PINS_6
#define HOT_PWM_GPIO_PORT    GPIOA
#define HOT_K_PIN    GPIO_PINS_7
#define HOT_K_GPIO_PORT    GPIOA
#define A_PIN    GPIO_PINS_0
#define A_GPIO_PORT    GPIOB
#define B_PIN    GPIO_PINS_1
#define B_GPIO_PORT    GPIOB
#define C_PIN    GPIO_PINS_2
#define C_GPIO_PORT    GPIOB
#define EC_L1_PIN    GPIO_PINS_10
#define EC_L1_GPIO_PORT    GPIOB
#define EC_R1_PIN    GPIO_PINS_11
#define EC_R1_GPIO_PORT    GPIOB
#define EC_KEY1_PIN    GPIO_PINS_12
#define EC_KEY1_GPIO_PORT    GPIOB
#define EC_L2_PIN    GPIO_PINS_13
#define EC_L2_GPIO_PORT    GPIOB
#define EC_R2_PIN    GPIO_PINS_14
#define EC_R2_GPIO_PORT    GPIOB
#define EC_KEY2_PIN    GPIO_PINS_15
#define EC_KEY2_GPIO_PORT    GPIOB
#define AIR_PWM_PIN    GPIO_PINS_8
#define AIR_PWM_GPIO_PORT    GPIOA
#define DG6_PIN    GPIO_PINS_11
#define DG6_GPIO_PORT    GPIOA
#define DG5_PIN    GPIO_PINS_12
#define DG5_GPIO_PORT    GPIOA
#define DG4_PIN    GPIO_PINS_6
#define DG4_GPIO_PORT    GPIOF
#define LED_C_PIN    GPIO_PINS_7
#define LED_C_GPIO_PORT    GPIOF
#define LED_F_PIN    GPIO_PINS_15
#define LED_F_GPIO_PORT    GPIOA
#define D_PIN    GPIO_PINS_3
#define D_GPIO_PORT    GPIOB
#define E_PIN    GPIO_PINS_4
#define E_GPIO_PORT    GPIOB
#define F_PIN    GPIO_PINS_5
#define F_GPIO_PORT    GPIOB
#define G_PIN    GPIO_PINS_6
#define G_GPIO_PORT    GPIOB
#define DP_PIN    GPIO_PINS_7
#define DP_GPIO_PORT    GPIOB
#define DG3_PIN    GPIO_PINS_8
#define DG3_GPIO_PORT    GPIOB
#define DG2_PIN    GPIO_PINS_9
#define DG2_GPIO_PORT    GPIOB

/* exported functions ------------------------------------------------------- */
  /* system clock config. */
  void wk_system_clock_config(void);

  /* config periph clock. */
  void wk_periph_clock_config(void);

  /* nvic config. */
  void wk_nvic_config(void);

/* add user code begin exported functions */

/* add user code end exported functions */

#ifdef __cplusplus
}
#endif

#endif
