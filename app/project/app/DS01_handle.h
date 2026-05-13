#ifndef _DS01_HANDLE_H
#define _DS01_HANDLE_H
#include "at32f421.h"
#include "stdbool.h"

#define SOFTWARE_VERSION_BAI 1
#define SOFTWARE_VERSION_SHI 0
#define SOFTWARE_VERSION_GE 2
#define HARDWARE_VERSION_BAI 1
#define HARDWARE_VERSION_SHI 0
#define HARDWARE_VERSION_GE 1

#define CURVE_REFRESH_TIME 0
#define CURVE_BUFF_SIZE 358

#define RESET_TIME 300

#define SAVE_CH_TIME 300

#define RESET_VALUE 255

#define MAX_SET_TEMP 500
#define MIN_SET_TEMP 100
#define MAX_SET_TEMP_F 932
#define MIN_SET_TEMP_F 212

#define MAX_CAL_TEMP 100
#define MIN_CAL_TEMP -100
#define MAX_CAL_TEMP_F 212
#define MIN_CAL_TEMP_F -212

#define PID_RANGE 100

#define MAX_SET_AIR 200
#define MIN_SET_AIR 5

#define MAX_ACTUAL_AIR 534
#define MIN_ACTUAL_AIR 100

#define MAX_ACTUAL_TEMP 600
#define MIN_ACTUAL_TEMP 70

#define ERROR_TIME 200

#define MAX_PWM_OUTPUT (35000)
// #define MAX_PWM_OUTPUT (48000-1)
#define ACTUAL_TEMP_REFRESH_TIME 200

#define SET_SHOW_TIMES 1000

#define SET_TIME 10000;

#define LOCK_RANGE 20

#define SLEEP_TIME 50

#define SLEEP_FAN_DATA 100

#define WAKEN_TIMES 3000

#define POWER_TEMP 50

typedef enum
{
	HANDLE_SLEEP = 0,
	HANDLE_WORKING,
	HANDLE_WAKEN,
} handle_state_e;

typedef enum
{
	HANDLE_OK = 0,
	HANDLE_LOW_TEMP_ERR,
	HANDLE_OVER_TEMP_ERR,
	HANDLE_FAN_ERROR,
} handle_error_state_e;

typedef enum
{
	NOT_IN_POSSITION = 0,
	IN_POSSITION,
} handle_position_e;

typedef enum
{
	Standard_Mode = 0,
	Power_Mode,
} run_mode_e;

typedef enum
{
	CELSIUS = 0,
	FAHRENHEIT,
} temp_unit_e;

typedef enum
{
	SPEAKER_CLOSE = 0,
	SPEAKER_OPEN,
} speaker_state_e;

typedef enum
{
	UNLOCK = 0,
	LOCK,
} temp_lock_state_e;

typedef enum
{
	SLEEP_OPEN = 0,
	SLEEP_CLOSE,
} sleep_state_e;

typedef enum
{
	FULL_POWER = 0,
	ZERO_POWER,
	PID_POWER,
} hot_state_e;

typedef enum
{
	EXIT = 0,
	TEMP_CAL,
	TEMP_UNIT,
	SPEAK_STATE,
	DISPLAY_LOCK,
	SLEEP_STATE,
	SOFTWARE_VERSION,
	HARDWARE_VERSION,
	RESET_RUN,
} set_interface_number_e;

typedef enum
{
	DISPLAY_SET = 0,
	DISPLAY_REAL,
	DISPLAY_ERR,
	DISPLAY_SLP,
	DISPLAY_SAVE_CH,
} display_number_e;

typedef struct
{
	float actual_temp;
	float actual_temp_f_display;
	float last_actual_temp;
	float last_actual_temp_f_display;
	float last_curve_actual_temp;
	float last_curve_actual_temp_f_display;
	float set_temp;
	float last_set_temp;
	float set_temp_f_display;
	float last_set_temp_f_display;
	float cal_data;
	float cal_temp_c_display;
	float last_cal_temp_c_display;
	float cal_temp_f_display;
	float last_cal_temp_f_display;
	float current_data;
	float pwm_out;
	float last_pwm_out;
	float cpu_temp;
	float set_sleep_time;
	float last_set_sleep_time;
	float sleep_time_count;
	float waken_time_count;
	uint16_t error_time;
	bool run_disp_state;
	bool last_run_disp_state;
	float last_power_data;
	float ch1_set_temp;
	float ch2_set_temp;
	float ch3_set_temp;
	float ch1_set_temp_f_display;
	float ch2_set_temp_f_display;
	float ch3_set_temp_f_display;

	float ch1_set_air;
	float ch2_set_air;
	float ch3_set_air;
	float last_ch1_set_air;
	float last_ch2_set_air;
	float last_ch3_set_air;
	float air_data;
	float last_air_data;
	float last_curve_air_data;
	float last_curve_sleep_air_data;
	float actual_air;
	float sleep_air_data;
	float last_sleep_air_data;
	uint16_t sleep_time;
	int fan_icon_show_times;
	uint8_t fan_icon_show_step;
	float temp_linear_data;

	float last_ch1_set_temp;
	float last_ch2_set_temp;
	float last_ch3_set_temp;
	float last_ch1_set_temp_f_display;
	float last_ch2_set_temp_f_display;
	float last_ch3_set_temp_f_display;
	uint8_t temp_buff[CURVE_BUFF_SIZE];
	uint8_t air_buff[CURVE_BUFF_SIZE];
} handle_parameter_t;

typedef struct
{
	uint8_t ch;
	uint8_t last_ch;
	int set_temp_time;
	int set_air_time;
	bool save_ch_flag;
	bool dp_flash_flag;
} system_state_t;

typedef struct
{
	handle_parameter_t system_parameter;
	system_state_t general_parameter;
	handle_state_e Work_handle_state;
	handle_state_e last_Work_handle_state;
	handle_position_e handle_position;
	handle_position_e last_handle_position;
	handle_error_state_e handle_error_state;
	handle_error_state_e last_handle_error_state;

	run_mode_e run_mode;
	run_mode_e last_run_mode;

	temp_unit_e temp_unit;
	temp_unit_e last_temp_unit;

	speaker_state_e speak_state;
	speaker_state_e last_speak_state;

	temp_lock_state_e display_lock_state;
	temp_lock_state_e last_display_lock_state;

	sleep_state_e sleep_state;
	sleep_state_e last_sleep_state;

	display_number_e display_temp_number;
	display_number_e display_air_number;

	bool set_flag;
	set_interface_number_e set_interface_number;
	bool reset_flag;
	bool init_flag;
	bool fan_run_flag;
	bool relay_open_flag;
	hot_state_e hot_state;

} DS01_Handle;

extern DS01_Handle sFWDS01_t;

void FWDS01_Init(DS01_Handle *FWDS01);

#endif