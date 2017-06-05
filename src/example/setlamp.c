#include <stdlib.h>
#include "lamp.h"

int main(int argc, char ** argv)
{
    LampHandle * h = lamp_init();
    LampVolume reqVol = VOLUME_1;

    char key;

    while ((key = getchar()) != 'q')
    {
        if (key == 'r' || key == 'R') lamp_setColor(255, 0, 0, h);
        if (key == 'g' || key == 'G') lamp_setColor(0, 255, 0, h);
        if (key == 'b' || key == 'B') lamp_setColor(0, 0, 255, h);
        if (key == '1') lamp_setSound(SOUND_NONE, reqVol, h);
        if (key == '2') lamp_setSound(SOUND_OPENOFFICE, reqVol, h);
        if (key == '3') lamp_setSound(SOUND_QUIET, reqVol, h);
        if (key == '4') lamp_setSound(SOUND_FUNKY, reqVol, h);
        if (key == '5') lamp_setSound(SOUND_FAIRYTALE, reqVol, h);
        if (key == '6') lamp_setSound(SOUND_KUANDOTRAIN, reqVol, h);
        if (key == '+' || key == '=')
        {
            if (reqVol < VOLUME_7)
            {
                reqVol++;
            }
            else
            {
                reqVol = VOLUME_MUTE;
            }

            lamp_setVolume(reqVol, h);
        }
        if (key == '-')
        {
            if (reqVol > VOLUME_MUTE)
            {
                reqVol--;
            }

            lamp_setVolume(reqVol, h);
        }
    }

    lamp_free(h);
}