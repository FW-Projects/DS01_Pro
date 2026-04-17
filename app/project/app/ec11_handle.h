#ifndef __EC11_HANDLE_H
#define __EC11_HANDLE_H
#include <stdint.h>
#include <stdbool.h>

#include "EncoderEC11.h"

#define EC11_TASK_TIME 1

typedef enum
{
	TEMP_ADD,
	TEMP_REDUCE,
	TEMP_ADD_FIVE,
	TEMP_REDUCE_FIVE,
	TEMP_EC11_END_EVENT,
	EXIT_SET,
	RESET_DS01_VALUE,
} temp_ec11_event_e;

typedef enum
{
	AIR_ADD,
	AIR_REDUCE,
	AIR_ADD_FIVE,
	AIR_REDUCE_FIVE,
	TEMP_CAL_ADD,
	TEMP_CAL_ADD_FIVE,
	TEMP_CAL_REDUCE,
	TEMP_CAL_REDUCE_FIVE,
	SET_UNIT,
	SET_SPEAK,
	SET_DISPLAY_LOCK,
	SET_SLEEP_STATE,
	AIR_EC11_END_EVENT,
} air_ec11_event_e;

void ec11_handle(void);

#endif // __ENCODER_EC11_H
