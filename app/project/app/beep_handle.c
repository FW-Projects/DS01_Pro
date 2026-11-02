#include "beep_handle.h"
#include "DS01_handle.h"
void beep_handle(void)
{
	static uint8_t ocp_time = 0;
	static uint8_t ocp_number = 0;
	static uint8_t beep_warning_time = 80;

	if(sFWDS01_t.handle_error_state != HANDLE_OK)
    {
        beep_warning_time--;

        if (!beep_warning_time)
        {
            beep_warning_time = 80;
            sbeep.cmd = BEEP_LONG;
        }
    }

	if (sFWDS01_t.speak_state == SPEAKER_OPEN)
		BeepProc(&sbeep);
	else if (sFWDS01_t.speak_state == SPEAKER_CLOSE)
		sbeep.off();
}
