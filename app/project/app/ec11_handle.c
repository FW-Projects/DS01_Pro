#include "ec11_handle.h"
#include "beep_handle.h"
#include "DS01_handle.h"

// 事件处理结束标记
#define TEMP_EC11_EVENT_END TEMP_EC11_END_EVENT
#define AIR_EC11_EVENT_END AIR_EC11_END_EVENT

void temp_ec11_event_handle(void);
void air_ec11_event_handle(void);
void temp_ec11_get_event(EC11_AnalyzeResult state);
void air_ec11_get_event(EC11_AnalyzeResult state_current);

static temp_ec11_event_e g_temp_ec11_event = TEMP_EC11_END_EVENT;
static air_ec11_event_e g_air_ec11_event = AIR_EC11_END_EVENT;

static bool g_temp_long_press_handled = false;

EC11_AnalyzeResult g_temp_ec11_analyze_result;
EC11_AnalyzeResult g_air_ec11_analyze_result;

/************************** EC11总处理 **************************/
/**
 * @brief EC11编码器总处理函数
 * @note  依次处理电压EC11、电流EC11的扫描→解析→事件处理
 */
void ec11_handle(void)
{
	// 电压EC11处理：扫描→解析→事件分发

	EC11_ScanResult temp_scan_result = EC11_Scan(&temp_ec11);
	g_temp_ec11_analyze_result = EC11_Analyze(&temp_ec11, temp_scan_result);
	temp_ec11_get_event(g_temp_ec11_analyze_result);
	temp_ec11_event_handle();

	// 电流EC11处理：扫描→解析→事件分发
	EC11_ScanResult air_scan_result = EC11_Scan(&air_ec11);
	g_air_ec11_analyze_result = EC11_Analyze(&air_ec11, air_scan_result);
	air_ec11_get_event(g_air_ec11_analyze_result);
	air_ec11_event_handle();
}

/************************** 电流EC11事件解析 **************************/
/**
 * @brief 电流EC11事件解析（根据编码器动作设置调节事件）
 * @param state: EC11解析结果（正转/反转/短按/长按等）
 */
void air_ec11_get_event(EC11_AnalyzeResult state)
{
	static int disp_vision_time = 0x00;
	switch (state)
	{
	case EC11_ANALYZE_CW:
		if (sFWDS01_t.set_flag == false)
		{
			g_air_ec11_event = AIR_ADD;
		}
		else
		{
			switch (sFWDS01_t.set_interface_number)
			{
			case EXIT:
				break;
			case TEMP_CAL:
				g_air_ec11_event = TEMP_CAL_ADD;
				break;
			case TEMP_UNIT:
				g_air_ec11_event = SET_UNIT;
				break;
			case SPEAK_STATE:
				g_air_ec11_event = SET_SPEAK;
				break;
			case DISPLAY_LOCK:
				g_air_ec11_event = SET_DISPLAY_LOCK;
				break;
			case SLEEP_STATE:
				g_air_ec11_event = SET_SLEEP_STATE;
				break;
			case VISION:
				break;
			case RESET_RUN:
				break;
			}
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_CCW:
		if (sFWDS01_t.set_flag == false)
		{
			g_air_ec11_event = AIR_REDUCE;
		}
		else
		{
			switch (sFWDS01_t.set_interface_number)
			{
			case EXIT:
				break;
			case TEMP_CAL:
				g_air_ec11_event = TEMP_CAL_REDUCE;
				break;
			case TEMP_UNIT:
				g_air_ec11_event = SET_UNIT;
				break;
			case SPEAK_STATE:
				g_air_ec11_event = SET_SPEAK;
				break;
			case DISPLAY_LOCK:
				g_air_ec11_event = SET_DISPLAY_LOCK;
				break;
			case SLEEP_STATE:
				g_air_ec11_event = SET_SLEEP_STATE;
				break;
			case VISION:
				break;
			case RESET_RUN:
				break;
			}
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CW:
		if (sFWDS01_t.set_flag == false)
		{
			g_air_ec11_event = AIR_ADD_FIVE;
		}
		else
		{
			switch (sFWDS01_t.set_interface_number)
			{
			case EXIT:
				break;
			case TEMP_CAL:
				g_air_ec11_event = TEMP_CAL_REDUCE_FIVE;
				break;
			case TEMP_UNIT:
				g_air_ec11_event = SET_UNIT;
				break;
			case SPEAK_STATE:
				g_air_ec11_event = SET_SPEAK;
				break;
			case DISPLAY_LOCK:
				g_air_ec11_event = SET_DISPLAY_LOCK;
				break;
			case SLEEP_STATE:
				g_air_ec11_event = SET_SLEEP_STATE;
				break;
			case VISION:
				break;
			case RESET_RUN:
				break;
			}
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CCW:
		if (sFWDS01_t.set_flag == false)
		{
			g_air_ec11_event = AIR_REDUCE_FIVE;
		}
		else
		{
			switch (sFWDS01_t.set_interface_number)
			{
			case EXIT:
				break;
			case TEMP_CAL:
				g_air_ec11_event = TEMP_CAL_ADD_FIVE;
				break;
			case TEMP_UNIT:
				g_air_ec11_event = SET_UNIT;
				break;
			case SPEAK_STATE:
				g_air_ec11_event = SET_SPEAK;
				break;
			case DISPLAY_LOCK:
				g_air_ec11_event = SET_DISPLAY_LOCK;
				break;
			case SLEEP_STATE:
				g_air_ec11_event = SET_SLEEP_STATE;
				break;
			case VISION:
				break;
			case RESET_RUN:
				break;
			}
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_SHORT_CLICK:
		break;
	case EC11_ANALYZE_DOUBLE_CLICK:
		break;
	case EC11_ANALYZE_LONG_PRESS:
		break;
	case EC11_ANALYZE_LONG_RELEASE:
		break;
	case EC11_ANALYZE_KEY_CW:
		break;
	case EC11_ANALYZE_KEY_CCW:
		break;
	default:
		break;
	}
}

/************************** 电压EC11事件解析 **************************/
/**
 * @brief 电压EC11事件解析（根据编码器动作设置调节事件）
 * @param state: EC11解析结果（正转/反转/短按/长按等）
 */
void temp_ec11_get_event(EC11_AnalyzeResult state)
{
	static int set_time = SET_TIME;
	static int reset_time = RESET_TIME;
	switch (state)
	{
	case EC11_ANALYZE_CW:
		if (sFWDS01_t.set_flag == true)
		{
			sFWDS01_t.set_interface_number++;
			if (sFWDS01_t.set_interface_number > 7)
				sFWDS01_t.set_interface_number = 0;
		}
		else
		{
			g_temp_ec11_event = TEMP_ADD;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_CCW:
		if (sFWDS01_t.set_flag == true)
		{
			if (sFWDS01_t.set_interface_number == 0)
			{
				sFWDS01_t.set_interface_number = 7;
			}
			else
				sFWDS01_t.set_interface_number--;
		}
		else
		{
			g_temp_ec11_event = TEMP_REDUCE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CW:
		if (sFWDS01_t.set_flag == false)
		{
			g_temp_ec11_event = TEMP_ADD_FIVE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CCW:
		if (sFWDS01_t.set_flag == false)
		{
			g_temp_ec11_event = TEMP_REDUCE_FIVE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_SHORT_CLICK:
		if (sFWDS01_t.set_interface_number == EXIT)
		{
			g_temp_ec11_event = EXIT_SET;
		}
		else if (sFWDS01_t.set_interface_number == RESET_RUN)
		{
			g_temp_ec11_event = RESET_DS01_VALUE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_DOUBLE_CLICK:

		break;
	case EC11_ANALYZE_LONG_PRESS:
		// 长按进入设置界面
		if (!g_temp_long_press_handled)
		{
			g_temp_long_press_handled = true;
			if (sFWDS01_t.set_flag == false)
			{
				sFWDS01_t.set_flag = true;
				sFWDS01_t.set_interface_number = EXIT;
			}
			else
			{
				sFWDS01_t.set_interface_number = EXIT;
				g_temp_ec11_event = EXIT_SET;
			}
		}
		sbeep.cmd = BEEP_LONG;
		break;
	case EC11_ANALYZE_LONG_RELEASE:
		g_temp_long_press_handled = false;
		break;
	case EC11_ANALYZE_KEY_CW:
		break;
	case EC11_ANALYZE_KEY_CCW:
		break;
	default:
		break;
	}
	if (sFWDS01_t.set_flag == false)
	{
		sFWDS01_t.system_parameter.cal_temp_c_display = sFWDS01_t.system_parameter.set_temp;
		sFWDS01_t.system_parameter.cal_temp_f_display = sFWDS01_t.system_parameter.set_temp_f_display;
	}
	else
	{
		if (state != EC11_ANALYZE_NO_ACTION)
		{
			set_time--;
			if (set_time < 0)
			{
				set_time = SET_TIME;
				g_temp_ec11_event = EXIT_SET;
			}
		}
		if (sFWDS01_t.reset_flag == true)
		{
			reset_time--;
			if (reset_time < 0)
			{
				reset_time = RESET_TIME;
				g_temp_ec11_event = EXIT_SET;
				sFWDS01_t.reset_flag = false;
			}
		}
	}
}

/************************** 电压EC11事件处理（调节电压值） **************************/
void temp_ec11_event_handle(void)
{
	switch (g_temp_ec11_event)
	{
	case TEMP_ADD:
		sbeep.cmd = BEEP_SHORT;
		sFWDS01_t.general_parameter.set_temp_time = SET_SHOW_TIMES;
		sFWDS01_t.display_temp_number = DISPLAY_SET;
		if (sFWDS01_t.temp_unit == CELSIUS)
		{
			if (sFWDS01_t.system_parameter.set_temp >= MAX_SET_TEMP)
			{
				sFWDS01_t.system_parameter.last_set_temp = 0;
				sFWDS01_t.system_parameter.set_temp = MAX_SET_TEMP;
			}
			else
			{
				sFWDS01_t.system_parameter.set_temp++;
			}

			sFWDS01_t.system_parameter.set_temp_f_display = 9 * sFWDS01_t.system_parameter.set_temp / 5 + 32;
		}
		else if (sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS01_t.system_parameter.set_temp_f_display >= MAX_SET_TEMP_F)
			{
				sFWDS01_t.system_parameter.last_set_temp_f_display = 0;
				sFWDS01_t.system_parameter.set_temp_f_display = MAX_SET_TEMP_F;
			}
			else
			{
				sFWDS01_t.system_parameter.set_temp_f_display++;
			}

			sFWDS01_t.system_parameter.set_temp = (sFWDS01_t.system_parameter.set_temp_f_display - 32) * 5 / 9;
		}
		sFWDS01_t.general_parameter.ch = 0;
		break;
	case TEMP_ADD_FIVE:
		sbeep.cmd = BEEP_SHORT;
		sFWDS01_t.general_parameter.set_temp_time = SET_SHOW_TIMES;
		sFWDS01_t.display_temp_number = DISPLAY_SET;
		if (sFWDS01_t.temp_unit == CELSIUS)
		{
			if (sFWDS01_t.system_parameter.set_temp + 10 >= MAX_SET_TEMP)
			{
				sFWDS01_t.system_parameter.last_set_temp = 0;
				sFWDS01_t.system_parameter.set_temp = MAX_SET_TEMP;
			}
			else
			{
				sFWDS01_t.system_parameter.set_temp += 10;
			}

			sFWDS01_t.system_parameter.set_temp_f_display = 9 * sFWDS01_t.system_parameter.set_temp / 5 + 32;
		}
		else if (sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS01_t.system_parameter.set_temp_f_display + 50 >= MAX_SET_TEMP_F)
			{
				sFWDS01_t.system_parameter.last_set_temp_f_display = 0;
				sFWDS01_t.system_parameter.set_temp_f_display = MAX_SET_TEMP_F;
			}
			else
			{
				sFWDS01_t.system_parameter.set_temp_f_display += 50;
			}

			sFWDS01_t.system_parameter.set_temp = (sFWDS01_t.system_parameter.set_temp_f_display - 32) * 5 / 9;
		}
		sFWDS01_t.general_parameter.ch = 0;
		break;
	case TEMP_REDUCE:
		sbeep.cmd = BEEP_SHORT;
		sFWDS01_t.general_parameter.set_temp_time = SET_SHOW_TIMES;
		sFWDS01_t.display_temp_number = DISPLAY_SET;
		if (sFWDS01_t.temp_unit == CELSIUS)
		{
			if (sFWDS01_t.system_parameter.set_temp <= MIN_SET_TEMP)
			{
				sFWDS01_t.system_parameter.last_set_temp = 0;
				sFWDS01_t.system_parameter.set_temp = MIN_SET_TEMP;
			}
			else
			{
				sFWDS01_t.system_parameter.set_temp--;
			}

			sFWDS01_t.system_parameter.set_temp_f_display = 9 * sFWDS01_t.system_parameter.set_temp / 5 + 32;
		}
		else if (sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS01_t.system_parameter.set_temp_f_display <= MIN_SET_TEMP_F)
			{
				sFWDS01_t.system_parameter.last_set_temp_f_display = 0;
				sFWDS01_t.system_parameter.set_temp_f_display = MIN_SET_TEMP_F;
			}
			else
			{
				sFWDS01_t.system_parameter.set_temp_f_display--;
			}

			sFWDS01_t.system_parameter.set_temp = (sFWDS01_t.system_parameter.set_temp_f_display - 32) * 5 / 9;
		}
		sFWDS01_t.general_parameter.ch = 0;
		break;
	case TEMP_REDUCE_FIVE:
		sbeep.cmd = BEEP_SHORT;
		sFWDS01_t.general_parameter.set_temp_time = SET_SHOW_TIMES;
		sFWDS01_t.display_temp_number = DISPLAY_SET;
		if (sFWDS01_t.temp_unit == CELSIUS)
		{
			if (sFWDS01_t.system_parameter.set_temp - 10 <= MIN_SET_TEMP)
			{
				sFWDS01_t.system_parameter.last_set_temp = 0;
				sFWDS01_t.system_parameter.set_temp = MIN_SET_TEMP;
			}
			else
			{
				sFWDS01_t.system_parameter.set_temp -= 10;
			}

			sFWDS01_t.system_parameter.set_temp_f_display = 9 * sFWDS01_t.system_parameter.set_temp / 5 + 32;
		}
		else if (sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS01_t.system_parameter.set_temp_f_display <= MIN_SET_TEMP_F + 41)
			{
				sFWDS01_t.system_parameter.last_set_temp_f_display = 0;
				sFWDS01_t.system_parameter.set_temp_f_display = MIN_SET_TEMP_F;
			}
			else
			{
				sFWDS01_t.system_parameter.set_temp_f_display -= 41;
			}

			sFWDS01_t.system_parameter.set_temp = (sFWDS01_t.system_parameter.set_temp_f_display - 32) * 5 / 9;
		}
		sFWDS01_t.general_parameter.ch = 0;
		break;
	case EXIT_SET:
	{
		sFWDS01_t.set_flag = false;

		sFWDS01_t.system_parameter.cal_data += sFWDS01_t.system_parameter.set_temp - sFWDS01_t.system_parameter.cal_temp_c_display;

		if (sFWDS01_t.system_parameter.cal_data >= MAX_CAL_TEMP)
		{
			sFWDS01_t.system_parameter.cal_data = MAX_CAL_TEMP;
		}
		if (sFWDS01_t.system_parameter.cal_data <= MIN_CAL_TEMP)
		{
			sFWDS01_t.system_parameter.cal_data = MIN_CAL_TEMP;
		}

		sbeep.cmd = BEEP_LONG;
	}
	break;
	case RESET_DS01_VALUE:
		sFWDS01_t.reset_flag = true;
		/* reset value */
		/* reset handle data */
		sFWDS01_t.system_parameter.set_temp = 380;
		sFWDS01_t.system_parameter.air_data = 40;
		sFWDS01_t.system_parameter.cal_data = 0;
		sFWDS01_t.system_parameter.ch1_set_temp = 380;
		sFWDS01_t.system_parameter.ch2_set_temp = 400;
		sFWDS01_t.system_parameter.ch3_set_temp = 420;
		sFWDS01_t.system_parameter.ch1_set_air = 80;
		sFWDS01_t.system_parameter.ch2_set_air = 100;
		sFWDS01_t.system_parameter.ch3_set_air = 120;
		sFWDS01_t.general_parameter.ch = 0;
		sFWDS01_t.system_parameter.set_sleep_time = 0;
		/* reset function data */

		sFWDS01_t.temp_unit = CELSIUS;

		sFWDS01_t.speak_state = SPEAKER_OPEN;

		sFWDS01_t.display_lock_state = LOCK;

		sFWDS01_t.sleep_state = SLEEP_OPEN;

		sFWDS01_t.general_parameter.set_temp_time = SET_SHOW_TIMES;
	case TEMP_EC11_EVENT_END: // 无事件
	default:
		break;
	}

	// 事件处理完成，复位事件标记
	g_temp_ec11_event = TEMP_EC11_EVENT_END;
}

/************************** 电流EC11事件处理（调节电流值） **************************/
void air_ec11_event_handle(void)
{
	switch (g_air_ec11_event)
	{
	case AIR_ADD:
		sFWDS01_t.general_parameter.set_air_time = SET_SHOW_TIMES;
		sFWDS01_t.display_air_number = DISPLAY_SET;
		if (sFWDS01_t.system_parameter.air_data + 1 > MAX_SET_AIR)
		{
			sFWDS01_t.system_parameter.air_data = MAX_SET_AIR;
			sFWDS01_t.system_parameter.last_air_data = 0;
		}
		else
		{
			sFWDS01_t.system_parameter.air_data++;
		}
		sFWDS01_t.general_parameter.ch = 0;
		break;
	case AIR_ADD_FIVE:
		sFWDS01_t.general_parameter.set_air_time = SET_SHOW_TIMES;
		sFWDS01_t.display_air_number = DISPLAY_SET;
		if (sFWDS01_t.system_parameter.air_data + 5 > MAX_SET_AIR)
		{
			sFWDS01_t.system_parameter.air_data = MAX_SET_AIR;
			sFWDS01_t.system_parameter.last_air_data = 0;
		}
		else
		{
			sFWDS01_t.system_parameter.air_data += 5;
		}
		sFWDS01_t.general_parameter.ch = 0;
		break;
	case AIR_REDUCE:
		sFWDS01_t.general_parameter.set_air_time = SET_SHOW_TIMES;
		sFWDS01_t.display_air_number = DISPLAY_SET;
		if (sFWDS01_t.system_parameter.air_data - 1 < MIN_SET_AIR)
		{
			sFWDS01_t.system_parameter.air_data = MIN_SET_AIR;
			sFWDS01_t.system_parameter.last_air_data = 0;
		}
		else
		{
			sFWDS01_t.system_parameter.air_data--;
		}
		sFWDS01_t.general_parameter.ch = 0;
		break;
	case AIR_REDUCE_FIVE:
		sFWDS01_t.general_parameter.set_air_time = SET_SHOW_TIMES;
		sFWDS01_t.display_air_number = DISPLAY_SET;
		if (sFWDS01_t.system_parameter.air_data - 5 < MIN_SET_AIR)
		{
			sFWDS01_t.system_parameter.air_data = MIN_SET_AIR;
			sFWDS01_t.system_parameter.last_air_data = 0;
		}
		else
		{
			sFWDS01_t.system_parameter.air_data -= 5;
		}

		sFWDS01_t.general_parameter.ch = 0;
		break;
	case TEMP_CAL_ADD:
		if (sFWDS01_t.temp_unit == CELSIUS)
		{
			if (sFWDS01_t.system_parameter.cal_temp_c_display >= sFWDS01_t.system_parameter.set_temp + MAX_CAL_TEMP)
			{
				sFWDS01_t.system_parameter.cal_temp_c_display = sFWDS01_t.system_parameter.set_temp + MAX_CAL_TEMP;
			}
			else
			{
				sFWDS01_t.system_parameter.cal_temp_c_display++;
			}

			sFWDS01_t.system_parameter.cal_temp_f_display = 9 * sFWDS01_t.system_parameter.set_temp / 5 + 32;
		}
		else if (sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS01_t.system_parameter.cal_temp_f_display >= sFWDS01_t.system_parameter.set_temp_f_display + MAX_CAL_TEMP_F)
			{
				sFWDS01_t.system_parameter.cal_temp_f_display = sFWDS01_t.system_parameter.set_temp_f_display + MAX_CAL_TEMP_F;
			}
			else
			{
				sFWDS01_t.system_parameter.cal_temp_f_display++;
			}

			sFWDS01_t.system_parameter.cal_temp_c_display = (sFWDS01_t.system_parameter.cal_temp_f_display - 32) * 5 / 9;
		}
		break;
	case TEMP_CAL_ADD_FIVE:
		if (sFWDS01_t.temp_unit == CELSIUS)
		{
			if (sFWDS01_t.system_parameter.cal_temp_c_display + 10 >= sFWDS01_t.system_parameter.set_temp + MAX_CAL_TEMP)
			{
				sFWDS01_t.system_parameter.cal_temp_c_display = sFWDS01_t.system_parameter.set_temp + MAX_CAL_TEMP;
			}
			else
			{
				sFWDS01_t.system_parameter.cal_temp_c_display += 10;
			}

			sFWDS01_t.system_parameter.cal_temp_f_display = 9 * sFWDS01_t.system_parameter.set_temp / 5 + 32;
		}
		else if (sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS01_t.system_parameter.cal_temp_f_display + 10 >= sFWDS01_t.system_parameter.set_temp_f_display + MAX_CAL_TEMP_F)
			{
				sFWDS01_t.system_parameter.cal_temp_f_display = sFWDS01_t.system_parameter.set_temp_f_display + MAX_CAL_TEMP_F;
			}
			else
			{
				sFWDS01_t.system_parameter.cal_temp_f_display += 10;
			}

			sFWDS01_t.system_parameter.cal_temp_c_display = (sFWDS01_t.system_parameter.cal_temp_f_display - 32) * 5 / 9;
		}
		break;
	case TEMP_CAL_REDUCE:
		if (sFWDS01_t.temp_unit == CELSIUS)
		{
			if (sFWDS01_t.system_parameter.cal_temp_c_display <= sFWDS01_t.system_parameter.set_temp + MIN_CAL_TEMP)
			{
				sFWDS01_t.system_parameter.cal_temp_c_display = sFWDS01_t.system_parameter.set_temp + MIN_CAL_TEMP;
			}
			else
			{
				sFWDS01_t.system_parameter.cal_temp_c_display--;
			}

			sFWDS01_t.system_parameter.cal_temp_f_display = 9 * sFWDS01_t.system_parameter.set_temp / 5 + 32;
		}
		else if (sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS01_t.system_parameter.cal_temp_f_display <= sFWDS01_t.system_parameter.set_temp_f_display + MIN_CAL_TEMP_F)
			{
				sFWDS01_t.system_parameter.cal_temp_f_display = sFWDS01_t.system_parameter.set_temp_f_display + MIN_CAL_TEMP_F;
			}
			else
			{
				sFWDS01_t.system_parameter.cal_temp_f_display--;
			}

			sFWDS01_t.system_parameter.cal_temp_c_display = (sFWDS01_t.system_parameter.cal_temp_f_display - 32) * 5 / 9;
		}
		break;
	case TEMP_CAL_REDUCE_FIVE:
		if (sFWDS01_t.temp_unit == CELSIUS)
		{
			if (sFWDS01_t.system_parameter.cal_temp_c_display - 10 <= sFWDS01_t.system_parameter.set_temp + MIN_CAL_TEMP)
			{
				sFWDS01_t.system_parameter.cal_temp_c_display = sFWDS01_t.system_parameter.set_temp + MIN_CAL_TEMP;
			}
			else
			{
				sFWDS01_t.system_parameter.cal_temp_c_display -= 10;
			}

			sFWDS01_t.system_parameter.cal_temp_f_display = 9 * sFWDS01_t.system_parameter.set_temp / 5 + 32;
		}
		else if (sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS01_t.system_parameter.cal_temp_f_display - 10 <= sFWDS01_t.system_parameter.set_temp_f_display + MIN_CAL_TEMP_F)
			{
				sFWDS01_t.system_parameter.cal_temp_f_display = sFWDS01_t.system_parameter.set_temp_f_display + MIN_CAL_TEMP_F;
			}
			else
			{
				sFWDS01_t.system_parameter.cal_temp_f_display -= 10;
			}

			sFWDS01_t.system_parameter.cal_temp_c_display = (sFWDS01_t.system_parameter.cal_temp_f_display - 32) * 5 / 9;
		}
		break;
	case SET_UNIT:
		if (sFWDS01_t.temp_unit == CELSIUS)
		{
			sFWDS01_t.temp_unit = FAHRENHEIT;
		}
		else if (sFWDS01_t.temp_unit == FAHRENHEIT)
		{
			sFWDS01_t.temp_unit = CELSIUS;
		}
		break;
	case SET_SPEAK:
		if (sFWDS01_t.speak_state == SPEAKER_CLOSE)
		{
			sFWDS01_t.speak_state = SPEAKER_OPEN;
		}
		else if (sFWDS01_t.speak_state == SPEAKER_OPEN)
		{
			sFWDS01_t.speak_state = SPEAKER_CLOSE;
		}
		break;
	case SET_DISPLAY_LOCK:
		if (sFWDS01_t.display_lock_state == LOCK)
		{
			sFWDS01_t.display_lock_state = UNLOCK;
		}
		else if (sFWDS01_t.display_lock_state == UNLOCK)
		{
			sFWDS01_t.display_lock_state = LOCK;
		}
		break;
	case SET_SLEEP_STATE:
		if (sFWDS01_t.sleep_state == SLEEP_CLOSE)
		{
			sFWDS01_t.sleep_state = SLEEP_OPEN;
		}
		else if (sFWDS01_t.sleep_state == SLEEP_OPEN)
		{
			sFWDS01_t.sleep_state = SLEEP_CLOSE;
		}
		break;
	case AIR_EC11_EVENT_END: // 无事件
	default:
		break;
	}

	// 事件处理完成，复位事件标记
	g_air_ec11_event = AIR_EC11_EVENT_END;
}
