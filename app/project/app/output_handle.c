#include "output_handle.h"
#include "beep_handle.h"
#include "PID_handle.h"
#include "adc_filter.h"

static float atucl_air = 0x00;
static void get_handle_position(DS01_Handle *this);
static void get_handle_work_state(DS01_Handle *this);
static void get_handle_error_state(DS01_Handle *this);
static void relay_control(DS01_Handle *this);
static void error_state_recovery(void);
static void relay_control(DS01_Handle *this);

int8_t off_set_buff[41] = {22, 21, 21, 21, 21, 21, 21, 21, 20, 20,
						   21, 21, 21, 20, 19, 19, 19, 19, 18, 17,
						   17, 16, 16, 15, 14, 13, 12, 11, (11), (10 - 2),
						   (9), (9), (8), 12, 15, 17, 21, 24, 26, 28, 33};

int8_t wind_set_20_buff[41] = {22 - 10, 21 - 10, 21 - 10, 21 - 10, 21 - 10, 21 - 10, 21 - 10, 21 - 10, 20 - 10, 20 - 10,
							   21 - 10, 21 - 10, 21 - 10, 20 - 10, 19 - 10, 19 - 10, 19 - 10, 19 - 10, 18 - 10, 17 - 10,
							   17 - 10, 16 - 10, 16 - 10, 15 - 10, 14 - 10, 13 - 10, 12 - 10, 11 - 10, (11 - 30), (10 - 2 - 10),
							   (9 - 10), (9 - 10), (8 - 10), 12 - 10, 15 - 10, 17 - 10, 21 - 10, 24 - 10, 26 - 10, 28 - 10, 33 - 10};

void output_handle(void)
{
	static uint8_t in_first = false;

	get_handle_position(&sFWDS01_t);
	get_handle_work_state(&sFWDS01_t);
	get_handle_error_state(&sFWDS01_t);
	relay_control(&sFWDS01_t);
}


 

static void get_handle_position(DS01_Handle *this)
{
	if (gpio_input_data_bit_read(GPIOC, GPIO_PINS_15))
	{
		this->handle_position = NOT_IN_POSSITION;
	}
	else
	{
		this->handle_position = IN_POSSITION;
	}
}

static void get_handle_work_state(DS01_Handle *this)
{
	static uint8_t first_state = false;
	switch (this->Work_handle_state)
	{
	case HANDLE_SLEEP:
		if (this->sleep_state == SLEEP_OPEN)
		{
			if (this->handle_position == NOT_IN_POSSITION)
			{
				this->Work_handle_state = HANDLE_WORKING;
				sFWDS01_t.general_parameter.set_temp_time = SET_SHOW_TIMES;
				sFWDS01_t.general_parameter.set_air_time = SET_SHOW_TIMES;
				sFWDS01_t.system_parameter.last_set_temp = 0x00;
				sFWDS01_t.system_parameter.last_set_temp_f_display = 0x00;
				sFWDS01_t.system_parameter.last_air_data = 0x00;
			}
			else if (this->handle_position == IN_POSSITION)
			{
				this->Work_handle_state = HANDLE_SLEEP;
				if (sFWDS01_t.general_parameter.set_temp_time == 0x00)
					sFWDS01_t.display_temp_number = DISPLAY_SLP;
				if (sFWDS01_t.general_parameter.set_air_time == 0x00)
					sFWDS01_t.display_air_number = DISPLAY_SLP;
			}
		}
		else if (this->sleep_state == SLEEP_CLOSE)
		{
			this->Work_handle_state = HANDLE_WORKING;
			sFWDS01_t.general_parameter.set_temp_time = SET_SHOW_TIMES;
			sFWDS01_t.general_parameter.set_air_time = SET_SHOW_TIMES;
			sFWDS01_t.system_parameter.last_set_temp = 0x00;
			sFWDS01_t.system_parameter.last_set_temp_f_display = 0x00;
			sFWDS01_t.system_parameter.last_air_data = 0x00;
		}
		this->system_parameter.error_time = 0;
		break;

	case HANDLE_WORKING:

		if (this->handle_position == IN_POSSITION &&
			this->sleep_state == SLEEP_OPEN)
		{
			if (this->system_parameter.actual_temp <= 70)
			{
				this->system_parameter.sleep_time++;

				if (this->system_parameter.sleep_time > SLEEP_TIME)
				{
					this->Work_handle_state = HANDLE_SLEEP;

					this->system_parameter.sleep_time = 0;
					sFWDS01_t.fan_run_flag = false;
					//                    sbeep.cmd = BEEP_LONG;
					break;
				}
			}
		}
		else if (this->sleep_state == SLEEP_CLOSE || this->handle_position == NOT_IN_POSSITION)
		{
			this->Work_handle_state = HANDLE_WORKING;
			if (first_state == false)
			{
				first_state = true;
				sFWDS01_t.general_parameter.set_temp_time = SET_SHOW_TIMES;
				sFWDS01_t.general_parameter.set_air_time = SET_SHOW_TIMES;
				sFWDS01_t.system_parameter.last_set_temp = 0x00;
				sFWDS01_t.system_parameter.last_set_temp_f_display = 0x00;
				sFWDS01_t.system_parameter.last_air_data = 0x00;
			}
		}
		break;
	case HANDLE_WAKEN:
		if (sFWDS01_t.system_parameter.waken_time_count != 0)
		{
			if (this->handle_position == NOT_IN_POSSITION)
			{
				this->Work_handle_state = HANDLE_WORKING;
				sFWDS01_t.system_parameter.waken_time_count = 0;
			}
		}
		else
		{
			if (this->handle_position == NOT_IN_POSSITION)
				this->Work_handle_state = HANDLE_WORKING;
			else if (this->handle_position == IN_POSSITION)
				this->Work_handle_state = HANDLE_SLEEP;
		}
		break;
	default:
		break;
	}
}

static void get_handle_error_state(DS01_Handle *this)
{
	static uint8_t get_step = 0;
	uint8_t err_type = 0; // 0:ÎÞ´íÎó 1:³¬ÎÂ 2:µÍÎÂ 3:·ç»ú´íÎó
	switch (get_step)
	{
	// ==================== ×´Ì¬0£ºÕý³£¼à²â ====================
	case 0:
		// ÅÐ¶ÏÊÇ·ñÔÊÐí¼ì²â´íÎó£¨ÊÖ±ú²»ÔÚÎ» / ÔÚÎ»µ«Î´ÐÝÃß£©
		if ((this->handle_position == NOT_IN_POSSITION) ||
			(this->handle_position == IN_POSSITION && this->sleep_state == SLEEP_CLOSE))
		{
			// ============== ·Ö²½ÅÐ¶Ï´íÎóÀàÐÍ ==============
			if (this->system_parameter.actual_temp >= MAX_ACTUAL_TEMP)
			{
				err_type = 1; // ³¬ÎÂ´íÎó
			}
			else if (this->system_parameter.actual_air < MIN_ACTUAL_AIR)
			{
				err_type = 3; // ·ç»ú´íÎó
			}
			else if (this->system_parameter.actual_temp <= MIN_ACTUAL_TEMP)
			{
				err_type = 2; // µÍÎÂ´íÎó
			}
			

			// ============== ÓÐ´íÎó ¡ú ¼ÆÊ± ==============
			if (err_type != 0)
			{
				this->system_parameter.error_time++;

				// ³¬ÎÂ£º500ms ´¥·¢
				if (err_type == 1 && this->system_parameter.error_time > ERROR_TIME / 2)
				{
					this->handle_error_state = HANDLE_OVER_TEMP_ERR;
					sFWDS01_t.display_temp_number = DISPLAY_ERR;
					sFWDS01_t.display_air_number = DISPLAY_ERR;
					this->system_parameter.error_time = 0;
					get_step = 1;
				}
				// µÍÎÂ/·ç»ú£º1000ms ´¥·¢
				else if ((err_type == 2 || err_type == 3) &&
						 this->system_parameter.error_time > ERROR_TIME * 2)
				{
					if (err_type == 2)
						this->handle_error_state = HANDLE_LOW_TEMP_ERR;
					else
						this->handle_error_state = HANDLE_FAN_ERROR;

					sFWDS01_t.display_temp_number = DISPLAY_ERR;
					sFWDS01_t.display_air_number = DISPLAY_ERR;
					this->system_parameter.error_time = 0;
					get_step = 1;
				}
			}
			// ============== ÎÞ´íÎó ¡ú ÇåÁã¼ÆÊ± + »Ö¸´ÏÔÊ¾ ==============
			else
			{
				this->system_parameter.error_time = 0;
			}
		}
		// ============== ²»Âú×ã¼ì²âÌõ¼þ ¡ú Ö±½Ó»Ö¸´ ==============
		else
		{
			this->system_parameter.error_time = 0;
		}
		break;

	// ==================== ×´Ì¬1£º´íÎóËø¶¨£¬µÈ´ý»Ö¸´Ìõ¼þ ====================
	case 1:
		// ÊÖ±úÔÚÎ» + ÒÑÐÝÃß ¡ú ´íÎó½â³ý
		if (this->handle_position == IN_POSSITION)
		{
			this->handle_error_state = HANDLE_OK;
			this->system_parameter.error_time = 0;
			error_state_recovery();
			get_step = 0;
		}
		break;

	default:
		get_step = 0;
		break;
	}
}

// ´íÎó×´Ì¬»Ö¸´¹«¹²º¯Êý
static void error_state_recovery(void)
{
	if (sFWDS01_t.Work_handle_state != HANDLE_SLEEP)
	{
		sFWDS01_t.general_parameter.set_temp_time = SET_SHOW_TIMES;
		sFWDS01_t.general_parameter.set_air_time = SET_SHOW_TIMES;
		sFWDS01_t.system_parameter.last_set_temp = 0x00;
		sFWDS01_t.system_parameter.last_set_temp_f_display = 0x00;
		sFWDS01_t.system_parameter.last_air_data = 0x00;
		
	}
}

void fan_control(DS01_Handle *this)
{
	static uint8_t control_step = 0;
	static bool fan_run_flag = false;

	switch (control_step)
	{
	case 0:
		if (this->handle_error_state != HANDLE_OK)
		{
			control_step = 1;
			break;
		}

		if (this->Work_handle_state == HANDLE_SLEEP)
		{
			/* keep fan output until the temp below 60 */
			if (this->system_parameter.actual_temp >= 60 && this->system_parameter.actual_temp < 70)
			{
				if (fan_run_flag == false)
				{
					tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 85);
				}
			}
			else
			{
				if (fan_run_flag == false)
				{
					fan_run_flag = true;
					/* close fan output */
					tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 0);
				}
			}
		}
		else if (this->Work_handle_state == HANDLE_WORKING &&
				 this->handle_position == IN_POSSITION &&
				 this->sleep_state == SLEEP_OPEN)
		{
			if(sFWDS01_t.general_parameter.set_air_time == 0x00)
				sFWDS01_t.display_air_number = DISPLAY_REAL;
			else
				sFWDS01_t.display_air_number = DISPLAY_SET;
			/* wait for next time open */
			if (fan_run_flag)
			{
				if (this->relay_open_flag)
				{
					fan_run_flag = false;
				}
			}
			if (this->system_parameter.actual_temp >= 250)
			{
				 
				if(this->system_parameter.sleep_air_data != SLEEP_FAN_DATA)
				{
				    this->system_parameter.sleep_air_data = SLEEP_FAN_DATA;
				}

				if (fan_run_flag == false)
				{
					/* open fan output with a half of max set val*/
					tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, this->system_parameter.sleep_air_data * 1.45 + 81);
				}
			}
			else if (this->system_parameter.actual_temp >= 70 && this->system_parameter.actual_temp < 250)
			{
				if (fan_run_flag == false)
				{
					/* open fan output with actual temp change*/
					this->system_parameter.sleep_air_data = this->system_parameter.actual_temp * 0.4;
					tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, this->system_parameter.sleep_air_data * 1.45 + 81);
				}
			}
			else
			{
				/* close fan output */
				tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 0);
			}
		}
		else if ((this->Work_handle_state == HANDLE_WORKING &&
				  this->handle_position == NOT_IN_POSSITION) ||
				 this->sleep_state == SLEEP_CLOSE)
		{
			if(sFWDS01_t.general_parameter.save_ch_flag == false)
			{
				sFWDS01_t.display_air_number = DISPLAY_SET;
				sFWDS01_t.system_parameter.last_air_data = 0x00;
			}
			/* wait for next time open */
			if (fan_run_flag == false)
			{
				if (this->relay_open_flag)
				{
					fan_run_flag = false;
				}
			}

			/* open fan output with user set val */
			tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, this->system_parameter.air_data * 1.45 + 81);
		}

		break;

	case 1:
		/* open fan output with max set val */
		tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, MAX_SET_AIR * 1.45 + 81);

		if (this->handle_error_state == HANDLE_OK)
		{
			control_step = 0;
			break;
		}

		break;

	default:
		break;
	}
}

int8_t linear_correction(uint16_t user_set_temp)
{
	volatile uint8_t n = 0;
	volatile uint16_t check_start = 0;
	volatile uint8_t check_value = 0;

	if (sFWDS01_t.system_parameter.air_data > 20)
	{
		if (user_set_temp < 300)
		{
			check_value = 0;
			check_start = 100;

			for (n = 0; n < 20; n++)
			{
				if (user_set_temp <= (check_start + check_value))
				{
					return (off_set_buff[n]);
				}
				else
				{
					check_start += 10;
				}
			}
		}
		else
		{
			check_value = 0;
			check_start = 300;

			for (n = 20; n < 40; n++)
			{
				if (user_set_temp <= (check_start + check_value))
				{
					return (off_set_buff[n]);
				}
				else
				{
					check_start += 10;
				}
			}
		}

		return (off_set_buff[n]);
	}
	else if (sFWDS01_t.system_parameter.air_data <= 20 && sFWDS01_t.system_parameter.air_data > 0)
	{
		if (user_set_temp < 300)
		{
			check_value = 0;
			check_start = 100;

			for (n = 0; n < 20; n++)
			{
				if (user_set_temp <= (check_start + check_value))
				{
					return (wind_set_20_buff[n]);
				}
				else
				{
					check_start += 10;
				}
			}
		}
		else
		{
			check_value = 0;
			check_start = 300;

			for (n = 20; n < 40; n++)
			{
				if (user_set_temp <= (check_start + check_value))
				{
					return (wind_set_20_buff[n]);
				}
				else
				{
					check_start += 10;
				}
			}

			return (wind_set_20_buff[n]);
		}
	}

	return 0;
}

static void relay_control(DS01_Handle *this)
{
	static uint8_t control_step = 0;
	static uint16_t open_relay_delay_time = RELAY_OPEN_TIME;

	if ((this->Work_handle_state == HANDLE_SLEEP && this->sleep_state == SLEEP_OPEN) || this->handle_error_state != HANDLE_OK)
	{
		/* close relay */
		gpio_bits_reset(GPIOA, GPIO_PINS_7);
		/* reset relay open flag */
		this->relay_open_flag = FALSE;
		open_relay_delay_time = RELAY_OPEN_TIME;
	}
	else if (this->Work_handle_state == HANDLE_WORKING &&
			 this->handle_error_state == HANDLE_OK &&
			 this->handle_position == NOT_IN_POSSITION ||
			this->sleep_state == SLEEP_CLOSE)
	{
		/* open relay */
		gpio_bits_set(GPIOA, GPIO_PINS_7);

		/* wait 300ms*/
		if (this->relay_open_flag == false)
		{
			open_relay_delay_time--;
		}

		/* relay opened done */
		if (0 == open_relay_delay_time)
		{
			this->relay_open_flag = true;
			open_relay_delay_time = RELAY_OPEN_TIME;
		}
	}
}

void pwm_control(DS01_Handle *this)
{
	static float temp = 0;
	static float set_temp = 0;
	static float last_set_temp = 0;
	static float last_set_wind = 0;
	static uint16_t delay_time = 0;
	static bool change_flag = false;
	//    static uint32_t hight_kd = 35000;
	static uint32_t hight_kd = 35000;
	static uint32_t low_kd = 5000;

	if (change_flag)
	{
		if (handle_pid.Kd <= low_kd)
			handle_pid.Kd = low_kd;
	}
	else
	{
		handle_pid.Kd = hight_kd;
	}

	set_temp = this->system_parameter.set_temp;

	#if 0
	if (last_set_temp != set_temp)
	{
		handle_pid.SumError = handle_pid.SumError / 2;
		this->system_parameter.temp_linear_data = linear_correction(set_temp);
		last_set_temp = set_temp;
	}
    #endif
	if (last_set_temp != set_temp || last_set_wind != this->system_parameter.air_data)
	{
		 if (set_temp < this->system_parameter.actual_temp &&
                last_set_temp != set_temp)
        {
            handle_pid.SumError = handle_pid.SumError / 2;
        }
		
		handle_pid.SumError = handle_pid.SumError / 2;
		this->system_parameter.temp_linear_data = linear_correction(set_temp);
		last_set_temp = set_temp;
		last_set_wind = this->system_parameter.air_data;
	}
	this->system_parameter.actual_temp = move_average_filter(&handle_temp) >> 2;
	//    temp = this->system_parameter.actual_temp + this->system_parameter.temp_linear_data - this->system_parameter.cal_data;
	temp = this->system_parameter.actual_temp - this->system_parameter.cal_data;
	if ((this->handle_position == NOT_IN_POSSITION || this->sleep_state == SLEEP_CLOSE))
	{
		if (this->relay_open_flag == true)
		{
			if (temp <= (set_temp + PID_RANGE) && temp >= (set_temp - PID_RANGE))
			{
				if (temp <= (set_temp + 5) && temp >= (set_temp - 5))
				{
					sFWDS01_t.hot_state = PID_POWER;
					delay_time++;
					if (delay_time >= 15)
					{
						if (handle_pid.Kd - 5000 <= low_kd)
						{
							handle_pid.Kd = low_kd;
						}
						else
						{
							handle_pid.Kd -= 5000;
						}

						delay_time = 0;
						change_flag = true;
					}
				}
				else
				{
					change_flag = false;
					delay_time = 0;
					sFWDS01_t.hot_state = FULL_POWER;
				}
				this->system_parameter.pwm_out = PID_Position_Calc(&handle_pid, set_temp, temp);
			}
			else if (temp > (set_temp + PID_RANGE))
			{
				this->system_parameter.pwm_out = 0;
				sFWDS01_t.hot_state = ZERO_POWER;
			}
			else if (temp < (set_temp - PID_RANGE))
			{
				this->system_parameter.pwm_out = MAX_PWM_OUTPUT;
				sFWDS01_t.hot_state = FULL_POWER;
			}
		}
	}
	else if ((this->handle_position == IN_POSSITION && this->sleep_state == SLEEP_OPEN))
	{
		
		change_flag = false;
		delay_time = 0;
		this->system_parameter.pwm_out = 0;
		PID_Clear(&handle_pid);
	}

	if (this->system_parameter.pwm_out <= 0)
	{
		this->system_parameter.pwm_out = 0;
	}

	if (this->handle_error_state != HANDLE_OK)
	{
		change_flag = false;
		delay_time = 0;
		this->system_parameter.pwm_out = 0;
		PID_Clear(&handle_pid);
	}
}

 

