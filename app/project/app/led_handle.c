#include "led_handle.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "wk_system.h"
#include "at32f421.h"
#include "perf_counter.h"
#include "DS01_Handle.h"

// 引脚定义  功率数码管
#define PIN1 0
#define PIN2 1
#define PIN3 2
#define PIN4 3
#define PIN5 4

#define ON 1
#define OFF 0

static uint8_t buffer_disp_set[3] = {0};
uint8_t buffer_disp_temp[3] = {0};
uint8_t buffer_disp_air[3] = {0};

void display_temp_air_handle(void);

static void (*buff[10])() = {zero, one, two, three, four, five, six, seven, eight, nine};

static volatile bool buffer_update_flag = false; // 缓冲区更新标志

void zero(void)
{
    gpio_bits_set(GPIOB, LEDG_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN);
}

void one(void)
{
    gpio_bits_set(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDB_PIN | LEDC_PIN);
}

void two(void)
{
    gpio_bits_set(GPIOB, LEDC_PIN | LEDF_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDD_PIN | LEDE_PIN | LEDG_PIN);
}

void three(void)
{
    gpio_bits_set(GPIOB, LEDF_PIN | LEDDP_PIN | LEDE_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDG_PIN);
}

void four(void)
{
    gpio_bits_set(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDB_PIN | LEDC_PIN | LEDF_PIN | LEDG_PIN);
}

void five(void)
{
    gpio_bits_set(GPIOB, LEDB_PIN | LEDE_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDC_PIN | LEDD_PIN | LEDF_PIN | LEDG_PIN);
}

void six(void)
{
    gpio_bits_set(GPIOB, LEDB_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

void seven(void)
{
    gpio_bits_set(GPIOB, LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN);
}

void eight(void)
{
    gpio_bits_set(GPIOB, LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

void nine(void)
{
    gpio_bits_set(GPIOB, LEDE_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDF_PIN | LEDG_PIN);
}

static void r(void) // 10
{
    gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDF_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDE_PIN | LEDG_PIN);
}

static void C(void) // 12
{
    gpio_bits_set(GPIOB, LEDB_PIN | LEDC_PIN | LEDG_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN);
}

static void E(void) // 14
{
    gpio_bits_set(GPIOB, LEDB_PIN | LEDC_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

static void F(void) // 15
{
    gpio_bits_set(GPIOB, LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

static void H(void) // 16
{
    gpio_bits_set(GPIOB, LEDA_PIN | LEDD_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDB_PIN | LEDC_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

static void n(void) // 17
{
    gpio_bits_set(GPIOB, LEDD_PIN | LEDG_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDE_PIN | LEDF_PIN);
}

void sleep_G(void) // 19
{
    gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDG_PIN);
}

static void Clear(void)
{
    gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
}

static void P(void) // 20
{
    gpio_bits_set(GPIOB, LEDC_PIN | LEDD_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

static void celsius(void) // ℃
{
    gpio_bits_set(GPIOB, LEDB_PIN | LEDC_PIN | LEDDP_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

static void fahrenheit(void) // ℉
{
    gpio_bits_set(GPIOB, LEDD_PIN | LEDB_PIN | LEDC_PIN);
    gpio_bits_reset(GPIOB, LEDA_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
}

static void nothing(void) // 全灭
{
    gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
}

// 统一扫描：温度3位 + 风量3位
void disp_digit_all(void)
{
    static uint8_t step_n = 0; // 全局只有一个步数
    static void (*TAST[21])() = {zero, one, two, three, four, five, six, seven, eight, nine,
                                 //   0     1    2    3      4     5        6    7        8           9
                                 C, E, F, H, n, sleep_G, P, celsius, fahrenheit, nothing, r
                                };
    //   10    11   12   13     14    15       16   17       18          19        20
    // ===================== 统一消隐（必须）=====================
    gpio_bits_set(TAMP_BAI_PORT, TAMP_BAI_PIN);
    gpio_bits_set(TAMP_SHI_PORT, TAMP_SHI_PIN);
    gpio_bits_set(TAMP_GE_PORT, TAMP_GE_PIN);
    gpio_bits_set(AIR_BAI_PORT, AIR_BAI_PIN);
    gpio_bits_set(AIR_SHI_PORT, AIR_SHI_PIN);
    gpio_bits_set(AIR_GE_PORT, AIR_GE_PIN);
    gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);

    // ===================== 6位轮流显示 =====================
    switch (step_n)
    {
    // 风量 百位
    case 0:
    {
        TAST[buffer_disp_air[0]]();
        gpio_bits_reset(AIR_BAI_PORT, AIR_BAI_PIN);
        break;
    }

    // 风量 十位
    case 1:
    {
        TAST[buffer_disp_air[1]]();
        gpio_bits_reset(AIR_SHI_PORT, AIR_SHI_PIN);
        break;
    }

    // 风量 个位
    case 2:
    {
        TAST[buffer_disp_air[2]]();
        gpio_bits_reset(AIR_GE_PORT, AIR_GE_PIN);
        break;
    }

    // 温度 百位
    case 3:
    {
        TAST[buffer_disp_temp[0]]();
        gpio_bits_reset(TAMP_BAI_PORT, TAMP_BAI_PIN);
        break;
    }

    // 温度 十位
    case 4:
    {
        TAST[buffer_disp_temp[1]]();
        gpio_bits_reset(TAMP_SHI_PORT, TAMP_SHI_PIN);
        break;
    }

    // 温度 个位
    case 5:
    {
        TAST[buffer_disp_temp[2]]();

        if (sFWDS01_t.handle_position == NOT_IN_POSSITION &&
                sFWDS01_t.Work_handle_state == HANDLE_WORKING &&
                sFWDS01_t.handle_error_state == HANDLE_OK ||
                sFWDS01_t.sleep_state == SLEEP_CLOSE)
        {
#if 0

            if (sFWDS01_t.hot_state == FULL_POWER)
            {
                gpio_bits_reset(GPIOB, LEDDP_PIN);
            }
            else if (sFWDS01_t.hot_state == ZERO_POWER)
            {
                gpio_bits_set(GPIOB, LEDDP_PIN);
            }
            else if (sFWDS01_t.hot_state == PID_POWER)
            {
                if (sFWDS01_t.system_parameter.actual_temp <= (sFWDS01_t.system_parameter.set_temp + LOCK_RANGE)
                        && sFWDS01_t.system_parameter.actual_temp >= (sFWDS01_t.system_parameter.set_temp - LOCK_RANGE))
                {
                    if (sFWDS01_t.general_parameter.dp_flash_flag)
                    {
                        gpio_bits_set(GPIOB, LEDDP_PIN);
                    }
                    else
                    {
                        gpio_bits_reset(GPIOB, LEDDP_PIN);
                    }
                }
                else if (sFWDS01_t.system_parameter.actual_temp > (sFWDS01_t.system_parameter.set_temp + LOCK_RANGE))
                {
                    gpio_bits_set(GPIOB, LEDDP_PIN);
                }
                else if (sFWDS01_t.system_parameter.actual_temp < (sFWDS01_t.system_parameter.set_temp - LOCK_RANGE))
                {
                    gpio_bits_reset(GPIOB, LEDDP_PIN);
                }
            }

#endif

            if (sFWDS01_t.system_parameter.actual_temp <= (sFWDS01_t.system_parameter.set_temp + LOCK_RANGE)
                    && sFWDS01_t.system_parameter.actual_temp >= (sFWDS01_t.system_parameter.set_temp - LOCK_RANGE))
            {
                if (sFWDS01_t.general_parameter.dp_flash_flag)
                {
                    gpio_bits_set(GPIOB, LEDDP_PIN);
                }
                else
                {
                    gpio_bits_reset(GPIOB, LEDDP_PIN);
                }
            }
            else if (sFWDS01_t.system_parameter.actual_temp > (sFWDS01_t.system_parameter.set_temp + LOCK_RANGE))
            {
                gpio_bits_set(GPIOB, LEDDP_PIN);
            }
            else if (sFWDS01_t.system_parameter.actual_temp < (sFWDS01_t.system_parameter.set_temp - LOCK_RANGE))
            {
                gpio_bits_reset(GPIOB, LEDDP_PIN);
            }
        }
        else
        {
            gpio_bits_set(GPIOB, LEDDP_PIN);
        }

        gpio_bits_reset(TAMP_GE_PORT, TAMP_GE_PIN);
        break;
    }
    }

    step_n = (step_n + 1) % 6;
}

// void Disp_Clean(void)
//{
//	static uint8_t n_5 = 0;
//	gpio_bits_set(GPIOA, GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_7);
//	gpio_bits_set(GPIOB, GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2);
//	gpio_bits_set(GPIOC, GPIO_PINS_14 | GPIO_PINS_13);
//	gpio_bits_set(GPIOB, GPIO_PINS_9 );
//	switch (n_5)
//	{
//	case 0:
//		gpio_bits_reset(GPIOC, GPIO_PINS_14); // 个位
//		Clear();
//		n_5++;
//		break;
//	case 1:
//		gpio_bits_reset(GPIOC, GPIO_PINS_13); // 十位
//		Clear();
//		n_5++;
//		break;
//	case 2:
//		gpio_bits_reset(GPIOB, GPIO_PINS_9); // 百位
//		Clear();
//		n_5 = 0;
//		break;
//	}
// }

// void Disp_Error(void)
//{
//	static uint8_t n_4 = 0;
//	static void (*TAST[10])() = {zero, one, two, three, four, five, six, seven, eight, nine};
//	gpio_bits_set(GPIOA, GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_7);
//	gpio_bits_set(GPIOB, GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2);
//	gpio_bits_set(GPIOC, GPIO_PINS_14 | GPIO_PINS_13);
//	gpio_bits_set(GPIOB, GPIO_PINS_9 );

//	switch (n_4)
//	{
//	case 0:
//		gpio_bits_reset(GPIOC, GPIO_PINS_14); // 个位
//		E();
//		n_4++;
//		break;
//	case 1:
//		gpio_bits_reset(GPIOC, GPIO_PINS_13); // 十位
//		r();
//		n_4++;
//		break;
//	case 2:
//		gpio_bits_reset(GPIOB, GPIO_PINS_9); // 百位
//		TAST[sFWDS01_t.Direct_handle_error_state]();
//		n_4 = 0;
//		break;
//
//	}
//}

void Display_Handle(uint8_t in_temp_num, uint8_t in_air_num)
{
    static char disp_times = 100;
    static char disp_times_air = 100;
    static float last_set_temp = 0, last_actual_temp = 0;
    static uint16_t temp_val = 0;
    static uint16_t show_temp = 0, show_air = 0;
    static void (*TAST[21])() = {zero, one, two, three, four, five, six, seven, eight, nine,
                                 //   0     1    2    3      4     5        6    7        8           9
                                 C, E, F, H, n, sleep_G, P, celsius, fahrenheit, nothing, r
                                };
    //   10    11   12   13     14    15       16   17       18          19        20

    switch (in_temp_num)
    {
    case DISPLAY_SET:
    {
        // 显示设置温度
        if (sFWDS01_t.system_parameter.last_set_temp != sFWDS01_t.system_parameter.set_temp ||
                sFWDS01_t.system_parameter.last_set_temp_f_display != sFWDS01_t.system_parameter.set_temp_f_display)
        {
            if (sFWDS01_t.temp_unit == CELSIUS)
            {
                show_temp = (uint16_t)sFWDS01_t.system_parameter.set_temp;
            }
            else if (sFWDS01_t.temp_unit == FAHRENHEIT) // 华氏度
            {
                show_temp = (uint16_t)sFWDS01_t.system_parameter.set_temp_f_display;
            }

            sFWDS01_t.system_parameter.last_set_temp = sFWDS01_t.system_parameter.set_temp;
            sFWDS01_t.system_parameter.last_set_temp_f_display = sFWDS01_t.system_parameter.set_temp_f_display;
            buffer_disp_temp[0] = show_temp / 100;
            buffer_disp_temp[1] = (show_temp / 10) % 10;
            buffer_disp_temp[2] = show_temp % 10;
        }
    }
    break;

    case DISPLAY_REAL:
    {
        disp_times--;

        if (disp_times <= 0)
        {
            disp_times = 100;

            if (sFWDS01_t.temp_unit == CELSIUS)
            {
                show_temp = sFWDS01_t.system_parameter.actual_temp - sFWDS01_t.system_parameter.cal_data;
            }
            else if (sFWDS01_t.temp_unit == FAHRENHEIT)
            {
                show_temp = ((sFWDS01_t.system_parameter.actual_temp - sFWDS01_t.system_parameter.cal_data) * 9 / 5) + 32;
            }

            buffer_disp_temp[0] = show_temp / 100;
            buffer_disp_temp[1] = (show_temp / 10) % 10;
            buffer_disp_temp[2] = show_temp % 10;
        }
    }
    break;

    case DISPLAY_ERR:
    {
        buffer_disp_temp[0] = 11;
        buffer_disp_temp[1] = 20;
        buffer_disp_temp[2] = sFWDS01_t.handle_error_state;
    }
    break;

    case DISPLAY_SLP:
    {
        buffer_disp_temp[0] = 15;
        buffer_disp_temp[1] = 15;
        buffer_disp_temp[2] = 15;
    }
    break;

    case DISPLAY_SAVE_CH:
    {
        if (sFWDS01_t.temp_unit == CELSIUS)
        {
            show_temp = (uint16_t)sFWDS01_t.system_parameter.set_temp;
        }
        else if (sFWDS01_t.temp_unit == FAHRENHEIT) // 华氏度
        {
            show_temp = (uint16_t)sFWDS01_t.system_parameter.set_temp_f_display;
        }

        buffer_disp_temp[0] = show_temp / 100;
        buffer_disp_temp[1] = (show_temp / 10) % 10;
        buffer_disp_temp[2] = show_temp % 10;
    }
    break;

    default:
        break;
    }

    switch (in_air_num)
    {
    case DISPLAY_SET:
        if (sFWDS01_t.system_parameter.last_air_data != sFWDS01_t.system_parameter.air_data)
        {
            sFWDS01_t.system_parameter.last_air_data = sFWDS01_t.system_parameter.air_data;
            show_air = sFWDS01_t.system_parameter.air_data;
            buffer_disp_air[0] = show_air / 100;
            buffer_disp_air[1] = (show_air / 10) % 10;
            buffer_disp_air[2] = show_air % 10;
        }

        break;

    case DISPLAY_REAL:
        disp_times_air--;

        if (disp_times_air <= 0x00)
        {
            disp_times_air = 100;

			//防止手柄刚休眠时风量不显示线性降温值
			if (sFWDS01_t.system_parameter.actual_temp >= 250)
			{
			    sFWDS01_t.system_parameter.last_sleep_air_data = sFWDS01_t.system_parameter.last_sleep_air_data+1;
			}
			
            if (sFWDS01_t.system_parameter.last_sleep_air_data != sFWDS01_t.system_parameter.sleep_air_data)
            {
                sFWDS01_t.system_parameter.last_sleep_air_data = sFWDS01_t.system_parameter.sleep_air_data;
                show_air = sFWDS01_t.system_parameter.sleep_air_data;
                buffer_disp_air[0] = show_air / 100;
                buffer_disp_air[1] = (show_air / 10) % 10;
                buffer_disp_air[2] = show_air % 10;
            }
        }

        break;

    case DISPLAY_ERR:
        buffer_disp_air[0] = 11;
        buffer_disp_air[1] = 20;
        buffer_disp_air[2] = sFWDS01_t.handle_error_state;
        break;

    case DISPLAY_SLP:
        buffer_disp_air[0] = 15;
        buffer_disp_air[1] = 15;
        buffer_disp_air[2] = 15;
        break;

    case DISPLAY_SAVE_CH:
        buffer_disp_air[0] = 10;
        buffer_disp_air[1] = 13;
        buffer_disp_air[2] = sFWDS01_t.general_parameter.ch;
        break;

    default:
        break;
    }

    disp_digit_all();
    display_temp_air_handle();
}


void display_temp_air_handle(void)
{
    static bool first_time = false;
    static int save_time = SAVE_CH_TIME;
    static uint8_t in_first = false;

    if (in_first == false)
    {
        in_first = true;

        if (sFWDS01_t.temp_unit == FAHRENHEIT)
        {
            sFWDS01_t.system_parameter.set_temp_f_display = (sFWDS01_t.system_parameter.set_temp * 9 / 5) + 32;
        }
    }

    /* temp display */
    if (sFWDS01_t.general_parameter.save_ch_flag == false)
    {
        if (sFWDS01_t.general_parameter.set_temp_time != 0x00)
        {
            first_time = false;
            sFWDS01_t.general_parameter.set_temp_time--;

            if (sFWDS01_t.general_parameter.save_ch_flag == false)
            {
                sFWDS01_t.display_temp_number = DISPLAY_SET;
            }
        }
        else
        {
            if (sFWDS01_t.Work_handle_state != HANDLE_SLEEP && sFWDS01_t.handle_error_state == HANDLE_OK)
            {
                if (sFWDS01_t.display_lock_state == LOCK)
                {
                    sFWDS01_t.general_parameter.set_temp_time = 0x00;

                    if (sFWDS01_t.system_parameter.actual_temp <= (sFWDS01_t.system_parameter.set_temp + LOCK_RANGE) &&
                            sFWDS01_t.system_parameter.actual_temp >= (sFWDS01_t.system_parameter.set_temp - LOCK_RANGE))
                    {
                        sFWDS01_t.system_parameter.last_set_temp = 0x00;
                        sFWDS01_t.system_parameter.last_set_temp_f_display = 0x00;
                        first_time = true;
                        sFWDS01_t.display_temp_number = DISPLAY_SET;
                    }
                    else
                    {
                        first_time = false;
                        sFWDS01_t.display_temp_number = DISPLAY_REAL;
                    }
                }
                else
                {
                    sFWDS01_t.display_temp_number = DISPLAY_REAL;
                }
            }
            else if (sFWDS01_t.handle_error_state != HANDLE_OK)
            {
                sFWDS01_t.display_temp_number = DISPLAY_ERR;
                sFWDS01_t.display_air_number = DISPLAY_ERR;
            }
            else
            {
                first_time = false;
            }
        }

        /* air display */
        if (sFWDS01_t.general_parameter.set_air_time != 0x00)
        {
            sFWDS01_t.general_parameter.set_air_time--;
            sFWDS01_t.display_air_number = DISPLAY_SET;
        }
        else
        {
            if (sFWDS01_t.handle_error_state != HANDLE_OK)
            {
                sFWDS01_t.display_air_number = DISPLAY_ERR;
            }
            else
            {
                sFWDS01_t.general_parameter.set_air_time = 0x00;
                sFWDS01_t.display_air_number = DISPLAY_SET;
            }
        }
    }
    else
    {
        if (sFWDS01_t.general_parameter.save_ch_flag == true)
        {
            save_time--;

            if (save_time <= 0x00)
            {
                save_time = SAVE_CH_TIME;
                sFWDS01_t.general_parameter.save_ch_flag = false;
            }
            else
            {
                sFWDS01_t.display_temp_number = DISPLAY_SAVE_CH;
                sFWDS01_t.display_air_number = DISPLAY_SAVE_CH;
            }
        }
    }
}



/* 设置界面显示 */
void dispmenu_pageoption(uint8_t in_number)
{
    static uint16_t flash_time = 0;
    static bool flash_flag = false;
    static uint8_t step_n = 0; // 全局只有一个步数
    static void (*TAST[21])() = {zero, one, two, three, four, five, six, seven, eight, nine,
                                 //   0     1    2    3      4     5        6    7        8           9
                                 C, E, F, H, n, sleep_G, P, celsius, fahrenheit, nothing, r
                                };
    //   10    11   12   13     14    15       16   17       18          19        20
    // ===================== 统一消隐（必须）=====================
    gpio_bits_set(TAMP_BAI_PORT, TAMP_BAI_PIN);
    gpio_bits_set(TAMP_SHI_PORT, TAMP_SHI_PIN);
    gpio_bits_set(TAMP_GE_PORT, TAMP_GE_PIN);
    gpio_bits_set(AIR_BAI_PORT, AIR_BAI_PIN);
    gpio_bits_set(AIR_SHI_PORT, AIR_SHI_PIN);
    gpio_bits_set(AIR_GE_PORT, AIR_GE_PIN);
    gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);


    if (sFWDS01_t.reset_flag)
    {
        flash_time++;

        if (flash_time >= 300)
        {
            flash_time = 0;
            flash_flag = !flash_flag;
        }

        // ===================== 6位轮流显示 =====================
        switch (step_n)
        {
        case 0:
        {
            TAST[15]();

            if (flash_flag)
            {
                gpio_bits_set(AIR_BAI_PORT, AIR_BAI_PIN);
            }
            else
            {
                gpio_bits_reset(AIR_BAI_PORT, AIR_BAI_PIN);
            }

            break;
        }

        case 1:
        {
            TAST[15]();

            if (flash_flag)
            {
                gpio_bits_set(AIR_SHI_PORT, AIR_SHI_PIN);
            }
            else
            {
                gpio_bits_reset(AIR_SHI_PORT, AIR_SHI_PIN);
            }

            break;
        }

        case 2:
        {
            TAST[15]();

            if (flash_flag)
            {
                gpio_bits_set(AIR_GE_PORT, AIR_GE_PIN);
            }
            else
            {
                gpio_bits_reset(AIR_GE_PORT, AIR_GE_PIN);
            }

            break;
        }

        case 3:
        {
            TAST[15]();

            if (sFWDS01_t.set_interface_number == SOFTWARE_VERSION || sFWDS01_t.set_interface_number == HARDWARE_VERSION)
            {
                gpio_bits_reset(GPIOB, LEDDP_PIN);
            }

            if (flash_flag)
            {
                gpio_bits_set(TAMP_BAI_PORT, TAMP_BAI_PIN);
            }
            else
            {
                gpio_bits_reset(TAMP_BAI_PORT, TAMP_BAI_PIN);
            }

            break;
        }

        case 4:
        {
            TAST[15]();

            if (sFWDS01_t.set_interface_number == SOFTWARE_VERSION || sFWDS01_t.set_interface_number == HARDWARE_VERSION)
            {
                gpio_bits_reset(GPIOB, LEDDP_PIN);
            }

            if (flash_flag)
            {
                gpio_bits_set(TAMP_SHI_PORT, TAMP_SHI_PIN);
            }
            else
            {
                gpio_bits_reset(TAMP_SHI_PORT, TAMP_SHI_PIN);
            }

            break;
        }

        case 5:
        {
            TAST[15]();

            if (flash_flag)
            {
                gpio_bits_set(TAMP_GE_PORT, TAMP_GE_PIN);
            }
            else
            {
                gpio_bits_reset(TAMP_GE_PORT, TAMP_GE_PIN);
            }

            break;
        }
        }

        step_n = (step_n + 1) % 6;
		
		
        /* or you can do this */							
//		buffer_disp_temp[0] = 15;
//		buffer_disp_temp[1] = 15;
//		buffer_disp_temp[2] = 15;
//		buffer_disp_air[0] = 15;
//		buffer_disp_air[1] = 15;
//		buffer_disp_air[2] = 15;
//		disp_digit_all();
//		gpio_bits_set(GPIOF,GPIO_PINS_7);
//		gpio_bits_set(GPIOA,GPIO_PINS_15);		
								
    }
    else
    {
        flash_time = 0;
        flash_flag = false;

        // ===================== 6位轮流显示 =====================
        switch (step_n)
        {
        case 0:
        {
            TAST[16]();
            gpio_bits_reset(AIR_BAI_PORT, AIR_BAI_PIN);
            break;
        }

        case 1:
        {
            TAST[0]();
            gpio_bits_reset(AIR_SHI_PORT, AIR_SHI_PIN);
            break;
        }

        case 2:
        {
            TAST[in_number]();
            gpio_bits_reset(AIR_GE_PORT, AIR_GE_PIN);
            break;
        }

        case 3:
        {
            TAST[buffer_disp_set[0]]();

            if (sFWDS01_t.set_interface_number == SOFTWARE_VERSION || sFWDS01_t.set_interface_number == HARDWARE_VERSION)
            {
                gpio_bits_reset(GPIOB, LEDDP_PIN);
            }

            gpio_bits_reset(TAMP_BAI_PORT, TAMP_BAI_PIN);
            break;
        }

        case 4:
        {
            TAST[buffer_disp_set[1]]();

            if (sFWDS01_t.set_interface_number == SOFTWARE_VERSION || sFWDS01_t.set_interface_number == HARDWARE_VERSION)
            {
                gpio_bits_reset(GPIOB, LEDDP_PIN);
            }

            gpio_bits_reset(TAMP_SHI_PORT, TAMP_SHI_PIN);
            break;
        }

        case 5:
        {
            TAST[buffer_disp_set[2]]();
            gpio_bits_reset(TAMP_GE_PORT, TAMP_GE_PIN);
            break;
        }
        }

        step_n = (step_n + 1) % 6;
    }
}

void Disp_Set_Handle(uint8_t menu_number)
{
    static float last_cal_data = 0;
    static uint16_t temp_val = 0;

    switch (menu_number)
    {
    case EXIT: // 退出设置
        buffer_disp_set[0] = 15;
        buffer_disp_set[1] = 15;
        buffer_disp_set[2] = 15;
        break;

    case TEMP_CAL: // 温度校准
        if (sFWDS01_t.temp_unit == CELSIUS)
        {
            temp_val = (uint16_t)sFWDS01_t.system_parameter.cal_temp_c_display;
        }
        else if (sFWDS01_t.temp_unit == FAHRENHEIT)
        {
            temp_val = (uint16_t)sFWDS01_t.system_parameter.cal_temp_f_display;
        }

        buffer_disp_set[0] = temp_val / 100;	   // 百位
        buffer_disp_set[1] = (temp_val / 10) % 10; // 十位
        buffer_disp_set[2] = temp_val % 10;		   // 个位
        break;

    case TEMP_UNIT: // 华氏度摄氏度
        if (sFWDS01_t.temp_unit == CELSIUS)
        {
            buffer_disp_set[1] = 10;    // C
        }
        else if (sFWDS01_t.temp_unit == FAHRENHEIT)
        {
            buffer_disp_set[1] = 12;    // F
        }

        buffer_disp_set[0] = 15; //-
        buffer_disp_set[2] = 15; //-
        break;

    case SPEAK_STATE:  // 声音开关
        if (sFWDS01_t.speak_state == SPEAKER_OPEN)
        {
            buffer_disp_set[0] = 19;
            buffer_disp_set[1] = 0;	 // O
            buffer_disp_set[2] = 14; // N
        }
        else if (sFWDS01_t.speak_state == SPEAKER_CLOSE)
        {
            buffer_disp_set[0] = 0;	 // O
            buffer_disp_set[1] = 12; // F
            buffer_disp_set[2] = 12; // F
        }

        break;

    case DISPLAY_LOCK:// 显示锁定
        if (sFWDS01_t.display_lock_state == LOCK)
        {
            buffer_disp_set[0] = 19;
            buffer_disp_set[1] = 0;	 // O
            buffer_disp_set[2] = 14; // N
        }
        else if (sFWDS01_t.display_lock_state == UNLOCK)
        {
            buffer_disp_set[0] = 0;	 // O
            buffer_disp_set[1] = 12; // F
            buffer_disp_set[2] = 12; // F
        }

        break;

    case SLEEP_STATE:
        if (sFWDS01_t.sleep_state == SLEEP_OPEN)
        {
            buffer_disp_set[0] = 19;
            buffer_disp_set[1] = 0;	 // O
            buffer_disp_set[2] = 14; // N
        }
        else if (sFWDS01_t.sleep_state == SLEEP_CLOSE)
        {
            buffer_disp_set[0] = 0;	 // O
            buffer_disp_set[1] = 12; // F
            buffer_disp_set[2] = 12; // F
        }

        break;

    case SOFTWARE_VERSION:
        buffer_disp_set[0] = SOFTWARE_VERSION_BAI;
        buffer_disp_set[1] = SOFTWARE_VERSION_SHI;
        buffer_disp_set[2] = SOFTWARE_VERSION_GE;
        break;

    case HARDWARE_VERSION:
        buffer_disp_set[0] = HARDWARE_VERSION_BAI;
        buffer_disp_set[1] = HARDWARE_VERSION_SHI;
        buffer_disp_set[2] = HARDWARE_VERSION_GE;
        break;

    case RESET_RUN:
        if (sFWDS01_t.reset_flag)
        {
            buffer_disp_set[0] = 15;
            buffer_disp_set[1] = 15;
            buffer_disp_set[2] = 15;
        }
        else
        {
            buffer_disp_set[0] = 15;
            buffer_disp_set[1] = 20;
            buffer_disp_set[2] = 15;
        }

        break;

    default:
        break;
    }

    dispmenu_pageoption(sFWDS01_t.set_interface_number);
}
