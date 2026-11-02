/* add user code begin Header */
/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
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

/* Includes ------------------------------------------------------------------*/
#include "at32f421_wk_config.h"
#include "wk_adc.h"
#include "wk_crc.h"
#include "wk_tmr.h"
#include "wk_usart.h"
#include "wk_wdt.h"
#include "wk_dma.h"
#include "wk_gpio.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "iap.h"
#include "tmt.h"
#include "pc_comm_handle.h"
#include "perf_counter.h"
#include "EventRecorder.h"
#include "ec11_handle.h"
#include "key_handle.h"
#include "flash_handle.h"
#include "beep_handle.h"
#include "DS01_Handle.h"
#include "led_handle.h"
#include "output_handle.h"
#include "adc_filter.h"
#include "flash_handle.h"
#include "PID_handle.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */
#define FEED_DOG_TIME 100

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */
void iap_task(void);
void feed_dog_task(void);
void pc_task(void);
void key_task(void);
void ec11_task(void);
void led_task(void);
void beep_task(void);
void work_task(void);
void flash_task(void);
/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */
  nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x4000);
  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config. */
  wk_timebase_init();

  /* init gpio function. */
  wk_gpio_config();

  /* init dma1 channel1 */
  wk_dma1_channel1_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL1, 
                        DMA1_CHANNEL1_PERIPHERAL_BASE_ADDR, 
                        DMA1_CHANNEL1_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL1_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL1, FALSE);

  /* init usart1 function. */
  wk_usart1_init();

  /* init crc function. */
  wk_crc_init();

  /* init wdt function. */
  wk_wdt_init();

  /* init adc1 function. */
  wk_adc1_init();

  /* init tmr1 function. */
  wk_tmr1_init();

  /* init tmr3 function. */
  wk_tmr3_init();

  /* init tmr15 function. */
  wk_tmr15_init();

  /* add user code begin 2 */
  tmt_init();
  tmt.create(iap_task, IAP_HANDLE_TIME);
  tmt.create(feed_dog_task, FEED_DOG_TIME);
  tmt.create(pc_task, PC_HANDLE_TIME);
  tmt.create(led_task, DISP_HANDLE_TIME);
  tmt.create(ec11_task, EC11_TASK_TIME);
  tmt.create(beep_task, BEEP_TASK_TIME); 
  tmt.create(work_task, OUTPUT_HANDLE_TIME);
  tmt.create(key_task, KEY_TASK_TIME);
  tmt.create(flash_task, FLASH_TASK_TIME);
  init_cycle_counter(true);
  EventRecorderInitialize(0, 1);
  iap_init();
  FWDS01_Init(&sFWDS01_t);
  filter_init(&handle_temp, ADC_CHANNEL_1);
  BSP_UsartInit();
  PID_Init(&handle_pid,800,4,20000, MAX_PWM_OUTPUT);
  gpio_bits_set(GPIOF,GPIO_PINS_7);
  gpio_bits_set(GPIOA,GPIO_PINS_15);
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
    tmt.run();
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

/**
 * @brief  iap task function
 * @param  none
 * @retval none
 */
void iap_task(void)
{
  iap_command_handle();
}

/**
 * @brief  sensor task function(only for debug)
 * @param  none
 * @retval none
 */

void feed_dog_task(void)
{
  static bool first_in = false;
  if (first_in == false)
  {
    first_in = true;
    /* if enabled, please feed the dog through wdt_counter_reload() function */
    wdt_enable();
  }
  wdt_counter_reload();
}

void pc_task(void)
{
  pc_comm_handle();
}

/**
 * @brief  ec11 task function
 * @param  none
 * @retval none
 */
void ec11_task(void)
{
	ec11_handle();
}

/**
 * @brief  key task function
 * @param  none
 * @retval none
 */
void key_task(void)
{
	key_handle();
}

/**
 * @brief  beep task function
 * @param  none
 * @retval none
 */
void beep_task(void)
{
	beep_handle();
}

/**
 * @brief  led task function
 * @param  none
 * @retval none
 */
void led_task(void)
{
	if(sFWDS01_t.init_flag == true)
	{
		if(sFWDS01_t.set_flag == true)
		{
			Disp_Set_Handle(sFWDS01_t.set_interface_number);
		}
		else
		{
			Display_Handle(sFWDS01_t.display_temp_number,sFWDS01_t.display_air_number);
		}
		
		if(sFWDS01_t.temp_unit == CELSIUS)
		{
			gpio_bits_set(GPIOF,GPIO_PINS_7);
			gpio_bits_reset(GPIOA,GPIO_PINS_15);
			
		}
		else if(sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			gpio_bits_reset(GPIOF,GPIO_PINS_7);
			gpio_bits_set(GPIOA,GPIO_PINS_15);

		}
	}
	else
	{
		buffer_disp_temp[0] = 15;
		buffer_disp_temp[1] = 15;
		buffer_disp_temp[2] = 15;
		buffer_disp_air[0] = 15;
		buffer_disp_air[1] = 15;
		buffer_disp_air[2] = 15;
		disp_digit_all();
		gpio_bits_set(GPIOF,GPIO_PINS_7);
		gpio_bits_set(GPIOA,GPIO_PINS_15);
	}
}

/**
 * @brief  work task function
 * @param  none
 * @retval none
 */
void work_task(void)
{
	if(sFWDS01_t.init_flag == true)
		output_handle();
}

/**
 * @brief  flash task function
 * @param  none
 * @retval none
 */
void flash_task(void)
{
#if 1
	FlashProc();
#endif
}
/* add user code end 4 */
