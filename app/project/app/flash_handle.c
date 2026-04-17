// #include "flash_handle.h"
// #include "led_handle.h"
// #include "DS01_handle.h"

// void get_data_from_b(void);
// void get_data_from_a(void);
// void get_reset_data(void);
// void check_data_all(void);
// uint16_t data_check(uint32_t address)
//{
//     static uint16_t data;
//     data = flash_wred_halfword(address);

//    if (data == 0xffff)
//    {
//        return FALSE;
//    }
//    else
//    {
//        return data;
//    }
//}

// uint16_t data_check_len(uint32_t address, uint8_t len)
//{
//     volatile uint16_t data;
//     uint8_t i;

//    for (i = 0; i < len; i += DATA_SIZE)
//    {
//        data = flash_wred_halfword(address + i);

//        if (data == 0xFFFF)
//        {
//            break;
//        }
//    }

//    return data;
//}

// void FlashProc(void)
//{
//     static flash_handle_t sflash;
//     static uint16_t last_set_temp;
//     static uint16_t last_set_wind;
//     static uint16_t last_ch1_set_temp;
//     static uint16_t last_ch1_set_wind;
//     static uint16_t last_ch2_set_temp;
//     static uint16_t last_ch2_set_wind;
//     static uint16_t last_ch3_set_temp;
//     static uint16_t last_ch3_set_wind;
//	static uint8_t last_temp_unit_mode;
//     static uint8_t last_speak_mode;
//	static uint16_t last_cal_data;
//     static uint8_t last_display_lock_mode;
//	static uint8_t last_sleep_mode;
//     static uint16_t flash_version = 0;
//     static uint8_t flash_count = 0;
//     static uint16_t a_ver;
//     static uint16_t b_ver;

//    switch (sflash.state)
//    {
//    case FLASH_START:
//		if (sFWDS01_t.init_flag == false)
//        {
//            /* 检测主存储区和备份存储区数据 */
//            if (data_check_len(A_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF
//                    && data_check_len(A_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
//            {
//                /* 检查哪个区的数据最新 */
//                a_ver = flash_wred_halfword(A_FLASH_VERSION_ADDRESS);
//                b_ver = flash_wred_halfword(B_FLASH_VERSION_ADDRESS);

//                if (a_ver > b_ver)
//                {
//					get_data_from_a();
//
//                }
//                else
//                {
//					get_data_from_b();
//                }
//            }
//            /* 检查主储区数据 */
//            else if (data_check_len(A_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
//            {
//                get_data_from_a();
//            }
//            /* 检查备份存储区数据 */
//            else if (data_check_len(B_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
//            {
//               get_data_from_b();
//            }
//            else
//            {
//				get_reset_data();
//            }
//
//			check_data_all();
//            sFWDS01_t.init_flag  = TRUE;
//        }
//        else
//        {
//            sflash.state = FLASH_HANDLE_DATA;
//            break;
//        }

//        break;

//    case FLASH_HANDLE_DATA:
//        if (last_set_temp != sFWDS01_t.system_parameter.set_temp             ||
//                last_set_wind != sFWDS01_t.system_parameter.air_data         ||
//                last_ch1_set_temp != sFWDS01_t.system_parameter.ch1_set_temp ||
//                last_ch1_set_wind != sFWDS01_t.system_parameter.ch1_set_air  ||
//                last_ch1_set_temp != sFWDS01_t.system_parameter.ch2_set_temp ||
//                last_ch1_set_wind != sFWDS01_t.system_parameter.ch2_set_air  ||
//                last_ch1_set_temp != sFWDS01_t.system_parameter.ch3_set_temp ||
//                last_ch1_set_wind != sFWDS01_t.system_parameter.ch3_set_air  ||
//				last_temp_unit_mode != sFWDS01_t.temp_unit                   ||
//                last_speak_mode != sFWDS01_t.speak_state                     ||
//				last_cal_data != sFWDS01_t.system_parameter.cal_data         ||
//                last_display_lock_mode != sFWDS01_t.display_lock_state       ||
//                last_sleep_mode != sFWDS01_t.sleep_state)
//        {
//            flash_unlock();

//            if (flash_count % 2 != FALSE)
//            {
//				flash_sector_erase(A_SET_TEMP_ADDRESS);
//				flash_halfword_program(A_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.set_temp);
//				flash_halfword_program(A_SET_AIR_ADDRESS, sFWDS01_t.system_parameter.air_data);
//				flash_halfword_program(A_CH1_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch1_set_temp);
//				flash_halfword_program(A_CH1_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch1_set_air);
//				flash_halfword_program(A_CH2_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch2_set_temp);
//				flash_halfword_program(A_CH2_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch2_set_air);
//				flash_halfword_program(A_CH3_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch3_set_temp);
//				flash_halfword_program(A_CH3_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch3_set_air);
//            }
//            else
//            {
//                flash_sector_erase(B_SET_TEMP_ADDRESS);
//				flash_halfword_program(B_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.set_temp);
//				flash_halfword_program(B_SET_AIR_ADDRESS, sFWDS01_t.system_parameter.air_data);
//				flash_halfword_program(B_CH1_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch1_set_temp);
//				flash_halfword_program(B_CH1_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch1_set_air);
//				flash_halfword_program(B_CH2_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch2_set_temp);
//				flash_halfword_program(B_CH2_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch2_set_air);
//				flash_halfword_program(B_CH3_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch3_set_temp);
//				flash_halfword_program(B_CH3_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch3_set_air);
//            }

//            last_set_temp = sFWDS01_t.system_parameter.set_temp;
//			last_set_wind = sFWDS01_t.system_parameter.air_data;
//			last_ch1_set_temp = sFWDS01_t.system_parameter.ch1_set_temp;
//			last_ch1_set_wind = sFWDS01_t.system_parameter.ch1_set_air;
//			last_ch1_set_temp = sFWDS01_t.system_parameter.ch2_set_temp;
//			last_ch1_set_wind = sFWDS01_t.system_parameter.ch2_set_air;
//			last_ch1_set_temp = sFWDS01_t.system_parameter.ch3_set_temp;
//			last_ch1_set_wind = sFWDS01_t.system_parameter.ch3_set_air;
//            sflash.state ++;
//            break;
//        }

//        break;

//    case FLASH_GENERAL_DATA:
//        if (flash_count % 2 != FALSE)
//        {
//			flash_halfword_program(A_TEMP_UNIT_ADDRESS, sFWDS01_t.temp_unit);
//			flash_halfword_program(A_SPEAK_MODE_ADDRESS, sFWDS01_t.speak_state);
//			flash_halfword_program(A_CAL_DATA_ADDRESS, sFWDS01_t.system_parameter.cal_data);
//			flash_halfword_program(A_DISPLAY_LOCK_MODE_ADDRESS, sFWDS01_t.display_lock_state);
//			flash_halfword_program(A_SLEEP_MODE_ADDRESS, sFWDS01_t.sleep_state);
//        }
//        else
//        {
//			flash_halfword_program(B_TEMP_UNIT_ADDRESS, sFWDS01_t.temp_unit);
//			flash_halfword_program(B_SPEAK_MODE_ADDRESS, sFWDS01_t.speak_state);
//			flash_halfword_program(B_CAL_DATA_ADDRESS, sFWDS01_t.system_parameter.cal_data);
//			flash_halfword_program(B_DISPLAY_LOCK_MODE_ADDRESS, sFWDS01_t.display_lock_state);
//			flash_halfword_program(B_SLEEP_MODE_ADDRESS, sFWDS01_t.sleep_state);
//        }
//		last_temp_unit_mode = sFWDS01_t.temp_unit;
//		last_speak_mode = sFWDS01_t.speak_state;
//		last_cal_data = sFWDS01_t.system_parameter.cal_data;
//		last_display_lock_mode = sFWDS01_t.display_lock_state;
//		last_sleep_mode = sFWDS01_t.sleep_state;
//        sflash.state ++;
//        break;

//    case FLASH_FINSH:
//        if (flash_count % 2 != FALSE)
//        {
//             flash_halfword_program(A_FLASH_VERSION_ADDRESS, flash_version);
//        }
//        else
//        {
//             flash_halfword_program(B_FLASH_VERSION_ADDRESS, flash_version);
//        }

//        flash_lock();
//        flash_version++;
//        flash_count++;
//        sflash.state = FLASH_HANDLE_DATA;
//        break;
//    }
//}

// void get_data_from_a(void)
//{
//
//	sFWDS01_t.system_parameter.set_temp = flash_wred_halfword(A_SET_TEMP_ADDRESS);
//	sFWDS01_t.system_parameter.air_data= flash_wred_halfword(A_SET_AIR_ADDRESS);
//	sFWDS01_t.system_parameter.ch1_set_temp = flash_wred_halfword(A_CH1_SET_TEMP_ADDRESS);
//	sFWDS01_t.system_parameter.ch1_set_air = flash_wred_halfword(A_CH1_SET_WIND_ADDRESS);
//	sFWDS01_t.system_parameter.ch2_set_temp = flash_wred_halfword(A_CH2_SET_TEMP_ADDRESS);
//	sFWDS01_t.system_parameter.ch2_set_air = flash_wred_halfword(A_CH2_SET_WIND_ADDRESS);
//	sFWDS01_t.system_parameter.ch3_set_temp = flash_wred_halfword(A_CH3_SET_TEMP_ADDRESS);
//	sFWDS01_t.system_parameter.ch3_set_air = flash_wred_halfword(A_CH3_SET_WIND_ADDRESS);
//	sFWDS01_t.temp_unit = flash_wred_halfword(A_TEMP_UNIT_ADDRESS);
//	sFWDS01_t.speak_state = flash_wred_halfword(A_SPEAK_MODE_ADDRESS);
//	sFWDS01_t.system_parameter.cal_data = flash_wred_halfword(A_CAL_DATA_ADDRESS);
//	sFWDS01_t.display_lock_state = flash_wred_halfword(A_DISPLAY_LOCK_MODE_ADDRESS);
//	sFWDS01_t.sleep_state = flash_wred_halfword(A_SLEEP_MODE_ADDRESS);
// }
// void get_data_from_b(void)
//{
//
//	sFWDS01_t.system_parameter.set_temp = flash_wred_halfword(B_SET_TEMP_ADDRESS);
//	sFWDS01_t.system_parameter.air_data= flash_wred_halfword(B_SET_AIR_ADDRESS);
//	sFWDS01_t.system_parameter.ch1_set_temp = flash_wred_halfword(B_CH1_SET_TEMP_ADDRESS);
//	sFWDS01_t.system_parameter.ch1_set_air = flash_wred_halfword(B_CH1_SET_WIND_ADDRESS);
//	sFWDS01_t.system_parameter.ch2_set_temp = flash_wred_halfword(B_CH2_SET_TEMP_ADDRESS);
//	sFWDS01_t.system_parameter.ch2_set_air = flash_wred_halfword(B_CH2_SET_WIND_ADDRESS);
//	sFWDS01_t.system_parameter.ch3_set_temp = flash_wred_halfword(B_CH3_SET_TEMP_ADDRESS);
//	sFWDS01_t.system_parameter.ch3_set_air = flash_wred_halfword(B_CH3_SET_WIND_ADDRESS);
//	sFWDS01_t.temp_unit = flash_wred_halfword(B_TEMP_UNIT_ADDRESS);
//	sFWDS01_t.speak_state = flash_wred_halfword(B_SPEAK_MODE_ADDRESS);
//	sFWDS01_t.system_parameter.cal_data = flash_wred_halfword(B_CAL_DATA_ADDRESS);
//	sFWDS01_t.display_lock_state = flash_wred_halfword(B_DISPLAY_LOCK_MODE_ADDRESS);
//	sFWDS01_t.sleep_state = flash_wred_halfword(B_SLEEP_MODE_ADDRESS);
// }

// void get_reset_data(void)
//{
//	sFWDS01_t.system_parameter.set_temp = 380;
//	sFWDS01_t.system_parameter.air_data= 80;
//	sFWDS01_t.system_parameter.ch1_set_temp = 320;
//	sFWDS01_t.system_parameter.ch1_set_air = 50;
//	sFWDS01_t.system_parameter.ch2_set_temp = 350;
//	sFWDS01_t.system_parameter.ch2_set_air = 60;
//	sFWDS01_t.system_parameter.ch3_set_temp = 380;
//	sFWDS01_t.system_parameter.ch3_set_air = 80;
//	sFWDS01_t.temp_unit = CELSIUS;
//	sFWDS01_t.speak_state = SPEAKER_OPEN;
//	sFWDS01_t.system_parameter.cal_data = 0;
//	sFWDS01_t.display_lock_state = UNLOCK;
//	sFWDS01_t.sleep_state = SLEEP_OPEN;
// }

// void check_data_all(void)
//{
//	if(sFWDS01_t.system_parameter.set_temp > MAX_SET_TEMP || sFWDS01_t.system_parameter.set_temp < MIN_SET_TEMP)
//	{
//		sFWDS01_t.system_parameter.set_temp = 380;
//	}
//
//	if(sFWDS01_t.system_parameter.air_data > MAX_SET_AIR || sFWDS01_t.system_parameter.air_data < MIN_SET_AIR)
//	{
//		sFWDS01_t.system_parameter.air_data = 80;
//	}
//
//	if(sFWDS01_t.system_parameter.ch1_set_temp > MAX_SET_TEMP || sFWDS01_t.system_parameter.ch1_set_temp < MIN_SET_TEMP)
//	{
//		sFWDS01_t.system_parameter.ch1_set_temp = 320;
//	}
//	if(sFWDS01_t.system_parameter.ch2_set_temp > MAX_SET_TEMP || sFWDS01_t.system_parameter.ch2_set_temp < MIN_SET_TEMP)
//	{
//		sFWDS01_t.system_parameter.ch2_set_temp = 350;
//	}
//	if(sFWDS01_t.system_parameter.ch3_set_temp > MAX_SET_TEMP || sFWDS01_t.system_parameter.ch3_set_temp < MIN_SET_TEMP)
//	{
//		sFWDS01_t.system_parameter.ch3_set_temp = 380;
//	}
//
//	if(sFWDS01_t.system_parameter.ch1_set_air > MAX_SET_AIR || sFWDS01_t.system_parameter.ch1_set_air < MIN_SET_AIR)
//	{
//		sFWDS01_t.system_parameter.ch1_set_air = 50;
//	}
//	if(sFWDS01_t.system_parameter.ch2_set_air > MAX_SET_AIR || sFWDS01_t.system_parameter.ch2_set_air < MIN_SET_AIR)
//	{
//		sFWDS01_t.system_parameter.ch2_set_air = 60;
//	}
//	if(sFWDS01_t.system_parameter.ch3_set_air > MAX_SET_AIR || sFWDS01_t.system_parameter.ch3_set_air < MIN_SET_AIR)
//	{
//		sFWDS01_t.system_parameter.ch3_set_air = 80;
//	}
//
//	if(sFWDS01_t.temp_unit != CELSIUS && sFWDS01_t.temp_unit != FAHRENHEIT)
//	{
//		sFWDS01_t.temp_unit = CELSIUS;
//	}
//
//	if(sFWDS01_t.speak_state != SPEAKER_CLOSE && sFWDS01_t.speak_state != SPEAKER_OPEN)
//	{
//		sFWDS01_t.speak_state = SPEAKER_OPEN;
//	}
//
//	if(sFWDS01_t.system_parameter.cal_data > MAX_CAL_TEMP || sFWDS01_t.system_parameter.cal_data < MIN_CAL_TEMP)
//	{
//		sFWDS01_t.system_parameter.cal_data = 0;
//	}
//
//	if(sFWDS01_t.display_lock_state != LOCK && sFWDS01_t.display_lock_state != UNLOCK)
//	{
//		sFWDS01_t.display_lock_state = UNLOCK;
//	}
//
//	if(sFWDS01_t.sleep_state != SLEEP_CLOSE && sFWDS01_t.sleep_state != SLEEP_OPEN)
//	{
//		sFWDS01_t.sleep_state = SLEEP_OPEN;
//	}
// }
#include "flash_handle.h"
#include "led_handle.h"
#include "DS01_handle.h"

// ===================== 新增：Flash 忙标志 =====================
uint8_t flash_busy_flag = 0;

void get_data_from_b(void);
void get_data_from_a(void);
void get_reset_data(void);
void check_data_all(void);

uint16_t data_check(uint32_t address)
{
    static uint16_t data;
    data = flash_wred_halfword(address);

    if (data == 0xffff)
    {
        return FALSE;
    }
    else
    {
        return data;
    }
}

uint16_t data_check_len(uint32_t address, uint8_t len)
{
    volatile uint16_t data;
    uint8_t i;

    for (i = 0; i < len; i += DATA_SIZE)
    {
        data = flash_wred_halfword(address + i);

        if (data == 0xFFFF)
        {
            break;
        }
    }

    return data;
}

void FlashProc(void)
{
    static flash_handle_t sflash;
    static uint16_t last_set_temp;
    static uint16_t last_set_wind;
    static uint16_t last_ch1_set_temp;
    static uint16_t last_ch1_set_wind;
    static uint16_t last_ch2_set_temp; // 修复：独立变量
    static uint16_t last_ch2_set_wind; // 修复：独立变量
    static uint16_t last_ch3_set_temp; // 修复：独立变量
    static uint16_t last_ch3_set_wind; // 修复：独立变量
    static uint8_t last_temp_unit_mode;
    static uint8_t last_speak_mode;
    static uint16_t last_cal_data;
    static uint8_t last_display_lock_mode;
    static uint8_t last_sleep_mode;
    static uint16_t flash_version = 0;
    static uint8_t flash_count = 0;
    static uint16_t a_ver;
    static uint16_t b_ver;

    // ===================== 防抖延时 =====================
    static uint16_t data_change_delay = 0;
    static uint8_t need_save_flash = 0;

    switch (sflash.state)
    {
    case FLASH_START:
        if (sFWDS01_t.init_flag == false)
        {
            if (data_check_len(A_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF && data_check_len(B_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
            {
                a_ver = flash_wred_halfword(A_FLASH_VERSION_ADDRESS);
                b_ver = flash_wred_halfword(B_FLASH_VERSION_ADDRESS);

                if (a_ver > b_ver)
                {
                    get_data_from_a();
                }
                else
                {
                    get_data_from_b();
                }
            }
            else if (data_check_len(A_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
            {
                get_data_from_a();
            }
            else if (data_check_len(B_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
            {
                get_data_from_b();
            }
            else
            {
                get_reset_data();
            }

            check_data_all();
            sFWDS01_t.init_flag = TRUE;

            // 初始化上次值
            last_set_temp = sFWDS01_t.system_parameter.set_temp;
            last_set_wind = sFWDS01_t.system_parameter.air_data;
            last_ch1_set_temp = sFWDS01_t.system_parameter.ch1_set_temp;
            last_ch1_set_wind = sFWDS01_t.system_parameter.ch1_set_air;
            last_ch2_set_temp = sFWDS01_t.system_parameter.ch2_set_temp;
            last_ch2_set_wind = sFWDS01_t.system_parameter.ch2_set_air;
            last_ch3_set_temp = sFWDS01_t.system_parameter.ch3_set_temp;
            last_ch3_set_wind = sFWDS01_t.system_parameter.ch3_set_air;
            last_temp_unit_mode = sFWDS01_t.temp_unit;
            last_speak_mode = sFWDS01_t.speak_state;
            last_cal_data = sFWDS01_t.system_parameter.cal_data;
            last_display_lock_mode = sFWDS01_t.display_lock_state;
            last_sleep_mode = sFWDS01_t.sleep_state;
        }
        else
        {
            sflash.state = FLASH_HANDLE_DATA;
        }
        break;

    case FLASH_HANDLE_DATA:
        // ===================== 检测数据是否变化 =====================
        if (last_set_temp != sFWDS01_t.system_parameter.set_temp ||
            last_set_wind != sFWDS01_t.system_parameter.air_data ||
            last_ch1_set_temp != sFWDS01_t.system_parameter.ch1_set_temp ||
            last_ch1_set_wind != sFWDS01_t.system_parameter.ch1_set_air ||
            last_ch2_set_temp != sFWDS01_t.system_parameter.ch2_set_temp ||
            last_ch2_set_wind != sFWDS01_t.system_parameter.ch2_set_air ||
            last_ch3_set_temp != sFWDS01_t.system_parameter.ch3_set_temp ||
            last_ch3_set_wind != sFWDS01_t.system_parameter.ch3_set_air ||
            last_temp_unit_mode != sFWDS01_t.temp_unit ||
            last_speak_mode != sFWDS01_t.speak_state ||
            last_cal_data != sFWDS01_t.system_parameter.cal_data ||
            last_display_lock_mode != sFWDS01_t.display_lock_state ||
            last_sleep_mode != sFWDS01_t.sleep_state)
        {
            data_change_delay++;
            if (data_change_delay >= 20) // 1秒防抖
            {
                need_save_flash = 1;
                data_change_delay = 0;
            }
        }
        else
        {
            data_change_delay = 0;
        }

        // ===================== 需要保存，进入擦写流程 =====================
        if (need_save_flash == 1)
        {
            need_save_flash = 0;
            flash_busy_flag = 1; // 【关键】标记Flash忙，数码管暂停刷新
            flash_unlock();

            if (flash_count % 2 != FALSE)
            {
                flash_sector_erase(A_SET_TEMP_ADDRESS);
            }
            else
            {
                flash_sector_erase(B_SET_TEMP_ADDRESS);
            }
            sflash.state = FLASH_GENERAL_DATA;
        }
        break;

    case FLASH_GENERAL_DATA:
        if (flash_count % 2 != FALSE)
        {
            flash_halfword_program(A_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.set_temp);
            flash_halfword_program(A_SET_AIR_ADDRESS, sFWDS01_t.system_parameter.air_data);
            flash_halfword_program(A_CH1_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch1_set_temp);
            flash_halfword_program(A_CH1_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch1_set_air);
            flash_halfword_program(A_CH2_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch2_set_temp);
            flash_halfword_program(A_CH2_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch2_set_air);
            flash_halfword_program(A_CH3_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch3_set_temp);
            flash_halfword_program(A_CH3_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch3_set_air);

            flash_halfword_program(A_TEMP_UNIT_ADDRESS, sFWDS01_t.temp_unit);
            flash_halfword_program(A_SPEAK_MODE_ADDRESS, sFWDS01_t.speak_state);
            flash_halfword_program(A_CAL_DATA_ADDRESS, sFWDS01_t.system_parameter.cal_data);
            flash_halfword_program(A_DISPLAY_LOCK_MODE_ADDRESS, sFWDS01_t.display_lock_state);
            flash_halfword_program(A_SLEEP_MODE_ADDRESS, sFWDS01_t.sleep_state);
        }
        else
        {
            flash_halfword_program(B_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.set_temp);
            flash_halfword_program(B_SET_AIR_ADDRESS, sFWDS01_t.system_parameter.air_data);
            flash_halfword_program(B_CH1_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch1_set_temp);
            flash_halfword_program(B_CH1_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch1_set_air);
            flash_halfword_program(B_CH2_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch2_set_temp);
            flash_halfword_program(B_CH2_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch2_set_air);
            flash_halfword_program(B_CH3_SET_TEMP_ADDRESS, sFWDS01_t.system_parameter.ch3_set_temp);
            flash_halfword_program(B_CH3_SET_WIND_ADDRESS, sFWDS01_t.system_parameter.ch3_set_air);

            flash_halfword_program(B_TEMP_UNIT_ADDRESS, sFWDS01_t.temp_unit);
            flash_halfword_program(B_SPEAK_MODE_ADDRESS, sFWDS01_t.speak_state);
            flash_halfword_program(B_CAL_DATA_ADDRESS, sFWDS01_t.system_parameter.cal_data);
            flash_halfword_program(B_DISPLAY_LOCK_MODE_ADDRESS, sFWDS01_t.display_lock_state);
            flash_halfword_program(B_SLEEP_MODE_ADDRESS, sFWDS01_t.sleep_state);
        }

        // 更新缓存
        last_set_temp = sFWDS01_t.system_parameter.set_temp;
        last_set_wind = sFWDS01_t.system_parameter.air_data;
        last_ch1_set_temp = sFWDS01_t.system_parameter.ch1_set_temp;
        last_ch1_set_wind = sFWDS01_t.system_parameter.ch1_set_air;
        last_ch2_set_temp = sFWDS01_t.system_parameter.ch2_set_temp;
        last_ch2_set_wind = sFWDS01_t.system_parameter.ch2_set_air;
        last_ch3_set_temp = sFWDS01_t.system_parameter.ch3_set_temp;
        last_ch3_set_wind = sFWDS01_t.system_parameter.ch3_set_air;
        last_temp_unit_mode = sFWDS01_t.temp_unit;
        last_speak_mode = sFWDS01_t.speak_state;
        last_cal_data = sFWDS01_t.system_parameter.cal_data;
        last_display_lock_mode = sFWDS01_t.display_lock_state;
        last_sleep_mode = sFWDS01_t.sleep_state;

        sflash.state = FLASH_FINSH;
        break;

    case FLASH_FINSH:
        if (flash_count % 2 != FALSE)
        {
            flash_halfword_program(A_FLASH_VERSION_ADDRESS, flash_version);
        }
        else
        {
            flash_halfword_program(B_FLASH_VERSION_ADDRESS, flash_version);
        }

        flash_lock();
        flash_version++;
        flash_count++;
        flash_busy_flag = 0; // 【关键】解除忙标志
        sflash.state = FLASH_HANDLE_DATA;
        break;
    }
}

void get_data_from_a(void)
{
    sFWDS01_t.system_parameter.set_temp = flash_wred_halfword(A_SET_TEMP_ADDRESS);
    sFWDS01_t.system_parameter.air_data = flash_wred_halfword(A_SET_AIR_ADDRESS);
    sFWDS01_t.system_parameter.ch1_set_temp = flash_wred_halfword(A_CH1_SET_TEMP_ADDRESS);
    sFWDS01_t.system_parameter.ch1_set_air = flash_wred_halfword(A_CH1_SET_WIND_ADDRESS);
    sFWDS01_t.system_parameter.ch2_set_temp = flash_wred_halfword(A_CH2_SET_TEMP_ADDRESS);
    sFWDS01_t.system_parameter.ch2_set_air = flash_wred_halfword(A_CH2_SET_WIND_ADDRESS);
    sFWDS01_t.system_parameter.ch3_set_temp = flash_wred_halfword(A_CH3_SET_TEMP_ADDRESS);
    sFWDS01_t.system_parameter.ch3_set_air = flash_wred_halfword(A_CH3_SET_WIND_ADDRESS);
    sFWDS01_t.temp_unit = flash_wred_halfword(A_TEMP_UNIT_ADDRESS);
    sFWDS01_t.speak_state = flash_wred_halfword(A_SPEAK_MODE_ADDRESS);
    sFWDS01_t.system_parameter.cal_data = flash_wred_halfword(A_CAL_DATA_ADDRESS);
    sFWDS01_t.display_lock_state = flash_wred_halfword(A_DISPLAY_LOCK_MODE_ADDRESS);
    sFWDS01_t.sleep_state = flash_wred_halfword(A_SLEEP_MODE_ADDRESS);
}

void get_data_from_b(void)
{
    sFWDS01_t.system_parameter.set_temp = flash_wred_halfword(B_SET_TEMP_ADDRESS);
    sFWDS01_t.system_parameter.air_data = flash_wred_halfword(B_SET_AIR_ADDRESS);
    sFWDS01_t.system_parameter.ch1_set_temp = flash_wred_halfword(B_CH1_SET_TEMP_ADDRESS);
    sFWDS01_t.system_parameter.ch1_set_air = flash_wred_halfword(B_CH1_SET_WIND_ADDRESS);
    sFWDS01_t.system_parameter.ch2_set_temp = flash_wred_halfword(B_CH2_SET_TEMP_ADDRESS);
    sFWDS01_t.system_parameter.ch2_set_air = flash_wred_halfword(B_CH2_SET_WIND_ADDRESS);
    sFWDS01_t.system_parameter.ch3_set_temp = flash_wred_halfword(B_CH3_SET_TEMP_ADDRESS);
    sFWDS01_t.system_parameter.ch3_set_air = flash_wred_halfword(B_CH3_SET_WIND_ADDRESS);
    sFWDS01_t.temp_unit = flash_wred_halfword(B_TEMP_UNIT_ADDRESS);
    sFWDS01_t.speak_state = flash_wred_halfword(B_SPEAK_MODE_ADDRESS);
    sFWDS01_t.system_parameter.cal_data = flash_wred_halfword(B_CAL_DATA_ADDRESS);
    sFWDS01_t.display_lock_state = flash_wred_halfword(B_DISPLAY_LOCK_MODE_ADDRESS);
    sFWDS01_t.sleep_state = flash_wred_halfword(B_SLEEP_MODE_ADDRESS);
}

void get_reset_data(void)
{
    sFWDS01_t.system_parameter.set_temp = 380;
    sFWDS01_t.system_parameter.air_data = 80;
    sFWDS01_t.system_parameter.ch1_set_temp = 320;
    sFWDS01_t.system_parameter.ch1_set_air = 50;
    sFWDS01_t.system_parameter.ch2_set_temp = 350;
    sFWDS01_t.system_parameter.ch2_set_air = 60;
    sFWDS01_t.system_parameter.ch3_set_temp = 380;
    sFWDS01_t.system_parameter.ch3_set_air = 80;
    sFWDS01_t.temp_unit = CELSIUS;
    sFWDS01_t.speak_state = SPEAKER_OPEN;
    sFWDS01_t.system_parameter.cal_data = 0;
    sFWDS01_t.display_lock_state = UNLOCK;
    sFWDS01_t.sleep_state = SLEEP_OPEN;
}

void check_data_all(void)
{
    if (sFWDS01_t.system_parameter.set_temp > MAX_SET_TEMP || sFWDS01_t.system_parameter.set_temp < MIN_SET_TEMP)
        sFWDS01_t.system_parameter.set_temp = 380;

    if (sFWDS01_t.system_parameter.air_data > MAX_SET_AIR || sFWDS01_t.system_parameter.air_data < MIN_SET_AIR)
        sFWDS01_t.system_parameter.air_data = 80;

    if (sFWDS01_t.system_parameter.ch1_set_temp > MAX_SET_TEMP || sFWDS01_t.system_parameter.ch1_set_temp < MIN_SET_TEMP)
        sFWDS01_t.system_parameter.ch1_set_temp = 320;
    if (sFWDS01_t.system_parameter.ch2_set_temp > MAX_SET_TEMP || sFWDS01_t.system_parameter.ch2_set_temp < MIN_SET_TEMP)
        sFWDS01_t.system_parameter.ch2_set_temp = 350;
    if (sFWDS01_t.system_parameter.ch3_set_temp > MAX_SET_TEMP || sFWDS01_t.system_parameter.ch3_set_temp < MIN_SET_TEMP)
        sFWDS01_t.system_parameter.ch3_set_temp = 380;

    if (sFWDS01_t.system_parameter.ch1_set_air > MAX_SET_AIR || sFWDS01_t.system_parameter.ch1_set_air < MIN_SET_AIR)
        sFWDS01_t.system_parameter.ch1_set_air = 50;
    if (sFWDS01_t.system_parameter.ch2_set_air > MAX_SET_AIR || sFWDS01_t.system_parameter.ch2_set_air < MIN_SET_AIR)
        sFWDS01_t.system_parameter.ch2_set_air = 60;
    if (sFWDS01_t.system_parameter.ch3_set_air > MAX_SET_AIR || sFWDS01_t.system_parameter.ch3_set_air < MIN_SET_AIR)
        sFWDS01_t.system_parameter.ch3_set_air = 80;

    if (sFWDS01_t.temp_unit != CELSIUS && sFWDS01_t.temp_unit != FAHRENHEIT)
        sFWDS01_t.temp_unit = CELSIUS;

    if (sFWDS01_t.speak_state != SPEAKER_CLOSE && sFWDS01_t.speak_state != SPEAKER_OPEN)
        sFWDS01_t.speak_state = SPEAKER_OPEN;

    if (sFWDS01_t.system_parameter.cal_data > MAX_CAL_TEMP || sFWDS01_t.system_parameter.cal_data < MIN_CAL_TEMP)
        sFWDS01_t.system_parameter.cal_data = 0;

    if (sFWDS01_t.display_lock_state != LOCK && sFWDS01_t.display_lock_state != UNLOCK)
        sFWDS01_t.display_lock_state = UNLOCK;

    if (sFWDS01_t.sleep_state != SLEEP_CLOSE && sFWDS01_t.sleep_state != SLEEP_OPEN)
        sFWDS01_t.sleep_state = SLEEP_OPEN;
}