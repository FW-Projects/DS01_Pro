#ifndef __OUTPUT_H
#define __OUTPUT_H
#include "flash.h"
#define FLASH_TASK_TIME            50
#define DATA_SIZE                  2
#define FLASH_MENBER               14



/* a area data address */
/* handle data */
#define A_SET_TEMP_ADDRESS               (0x08000000 + 1024 * 60)
#define A_SET_AIR_ADDRESS                A_SET_TEMP_ADDRESS + DATA_SIZE   
#define A_CH1_SET_TEMP_ADDRESS           A_SET_AIR_ADDRESS + DATA_SIZE   
#define A_CH1_SET_WIND_ADDRESS           A_CH1_SET_TEMP_ADDRESS + DATA_SIZE   
#define A_CH2_SET_TEMP_ADDRESS           A_CH1_SET_WIND_ADDRESS + DATA_SIZE   
#define A_CH2_SET_WIND_ADDRESS           A_CH2_SET_TEMP_ADDRESS + DATA_SIZE   
#define A_CH3_SET_TEMP_ADDRESS  		 A_CH2_SET_WIND_ADDRESS + DATA_SIZE  
#define A_CH3_SET_WIND_ADDRESS 			 A_CH3_SET_TEMP_ADDRESS + DATA_SIZE 
#define A_TEMP_UNIT_ADDRESS     		 A_CH3_SET_WIND_ADDRESS + DATA_SIZE   
#define A_SPEAK_MODE_ADDRESS   			 A_TEMP_UNIT_ADDRESS + DATA_SIZE  
#define A_CAL_DATA_ADDRESS     			 A_SPEAK_MODE_ADDRESS + DATA_SIZE 
#define A_DISPLAY_LOCK_MODE_ADDRESS      A_CAL_DATA_ADDRESS + DATA_SIZE   
#define A_SLEEP_MODE_ADDRESS             A_DISPLAY_LOCK_MODE_ADDRESS + DATA_SIZE  
#define A_FLASH_VERSION_ADDRESS          A_SLEEP_MODE_ADDRESS + DATA_SIZE                         

/* b area data address */
/* handle data */
#define B_SET_TEMP_ADDRESS               (0x08000000 + 1024 * 62)
#define B_SET_AIR_ADDRESS                B_SET_TEMP_ADDRESS + DATA_SIZE   
#define B_CH1_SET_TEMP_ADDRESS           B_SET_AIR_ADDRESS + DATA_SIZE   
#define B_CH1_SET_WIND_ADDRESS           B_CH1_SET_TEMP_ADDRESS + DATA_SIZE   
#define B_CH2_SET_TEMP_ADDRESS           B_CH1_SET_WIND_ADDRESS + DATA_SIZE   
#define B_CH2_SET_WIND_ADDRESS           B_CH2_SET_TEMP_ADDRESS + DATA_SIZE   
#define B_CH3_SET_TEMP_ADDRESS  		 B_CH2_SET_WIND_ADDRESS + DATA_SIZE  
#define B_CH3_SET_WIND_ADDRESS 			 B_CH3_SET_TEMP_ADDRESS + DATA_SIZE 
#define B_TEMP_UNIT_ADDRESS     		 B_CH3_SET_WIND_ADDRESS + DATA_SIZE   
#define B_SPEAK_MODE_ADDRESS   			 B_TEMP_UNIT_ADDRESS + DATA_SIZE  
#define B_CAL_DATA_ADDRESS     			 B_SPEAK_MODE_ADDRESS + DATA_SIZE 
#define B_DISPLAY_LOCK_MODE_ADDRESS      B_CAL_DATA_ADDRESS + DATA_SIZE   
#define B_SLEEP_MODE_ADDRESS             B_DISPLAY_LOCK_MODE_ADDRESS + DATA_SIZE  
#define B_FLASH_VERSION_ADDRESS          B_SLEEP_MODE_ADDRESS + DATA_SIZE    

typedef enum
{
    FLASH_START = 0,
    FLASH_HANDLE_DATA,
    FLASH_GENERAL_DATA,
	FLASH_VER,
    FLASH_FINSH,
} flash_state_t;


typedef struct
{
    flash_state_t state;
} flash_handle_t;

void FlashProc(void);


#endif
