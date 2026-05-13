#include "DS01_Handle.h"
#include <string.h>

DS01_Handle sFWDS01_t;

void FWDS01_Init(DS01_Handle *FWDS01)
{

	/* start the Direct handle init of  */
	FWDS01->system_parameter.actual_temp = 0,
	FWDS01->system_parameter.set_temp = 380,
	FWDS01->system_parameter.set_temp_f_display = 716;

	FWDS01->system_parameter.error_time = 0,
	FWDS01->system_parameter.cal_data = 0,
	FWDS01->system_parameter.cal_temp_c_display = 0,
	FWDS01->system_parameter.cal_temp_f_display = 0,
	FWDS01->handle_position = IN_POSSITION,
	FWDS01->handle_error_state = HANDLE_OK,
	FWDS01->Work_handle_state = HANDLE_SLEEP;

	memset(FWDS01->system_parameter.temp_buff, 250, CURVE_BUFF_SIZE);
	memset(FWDS01->system_parameter.air_buff, 250, CURVE_BUFF_SIZE);

	/* end the  Direct handle init of*/

	/* start the general init  of  */
	FWDS01->temp_unit = CELSIUS,
	FWDS01->speak_state = SPEAKER_OPEN,
	FWDS01->display_lock_state = UNLOCK,
	FWDS01->sleep_state = SLEEP_OPEN,
	FWDS01->display_temp_number = DISPLAY_SET;
	FWDS01->display_air_number = DISPLAY_SET;
	FWDS01->general_parameter.ch = 0,
	FWDS01->system_parameter.last_pwm_out = 1000;
	/* ch set */
	FWDS01->system_parameter.ch1_set_temp = 320,
	FWDS01->system_parameter.ch2_set_temp = 350,
	FWDS01->system_parameter.ch3_set_temp = 380,
	FWDS01->system_parameter.ch1_set_air = 40,
	FWDS01->system_parameter.ch2_set_air = 50,
	FWDS01->system_parameter.ch3_set_air = 60,
	FWDS01->system_parameter.last_sleep_air_data = 0,
	FWDS01->system_parameter.sleep_air_data = 0,

	/* en working set */
		/* end the general init of*/

		FWDS01->reset_flag = 0;
	FWDS01->system_parameter.pwm_out = 0x00;
	FWDS01->system_parameter.cpu_temp = 0x00;

	FWDS01->system_parameter.air_data = 0x32;
	FWDS01->system_parameter.last_air_data = 0x00;
	FWDS01->system_parameter.actual_air = 0x00;
	FWDS01->system_parameter.last_curve_sleep_air_data = 0x00;
	FWDS01->system_parameter.last_curve_air_data = 0x00;
	//	FWDS01->general_parameter.set_temp_time = SET_SHOW_TIMES;
	FWDS01->general_parameter.set_temp_time = 0x00;
	FWDS01->general_parameter.set_air_time = 0x00;
	FWDS01->general_parameter.save_ch_flag = false;
	FWDS01->general_parameter.dp_flash_flag = false;
	FWDS01->set_flag = false;
	FWDS01->set_interface_number = EXIT;
	FWDS01->fan_run_flag = true;
	FWDS01->relay_open_flag = false;
	FWDS01->hot_state = FULL_POWER;
	FWDS01->init_flag = false;
}
