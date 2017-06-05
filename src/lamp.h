#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <hidapi.h>

#define BLL_VENDOR_ID 0x04d8
#define BLL_PRODUCT_ID 0xf848

typedef enum _LAMP_ERROR {
    LAMP_ERROR_NOERROR = 0,
    LAMP_ERROR_INVALIDHANDLE = -1,
    LAMP_ERROR_BUFFERCONVERSIONFAILURE = -2,
    LAMP_ERROR_DEVICEWRITEFAILURE = -3
} LampError;

typedef enum _SOUND {
    SOUND_NONE = 128,
    SOUND_OPENOFFICE = 136,
    SOUND_QUIET = 144,
    SOUND_FUNKY = 152,
    SOUND_FAIRYTALE = 160,
    SOUND_KUANDOTRAIN = 168,
    SOUND_TELEPHONENORDIC = 176,
    SOUND_TELEPHONEORIGINAL = 184,
    SOUND_TELEPHONEPICKMEUP = 192,
    SOUND_BUZZ = 216
} LampSound;

typedef enum _VOLUME {
    VOLUME_MUTE = 0,
    VOLUME_1 = 1,
    VOLUME_2 = 2,
    VOLUME_3 = 3,
    VOLUME_4 = 4,
    VOLUME_5 = 5,
    VOLUME_6 = 6,
    VOLUME_7 = 7
} LampVolume;

typedef struct _LampHandle {
    int cRed;
    int cGreen;
    int cBlue;
    LampSound cSound;
    LampVolume cVolume;
    void * dHandle;
} LampHandle;

unsigned char * _lamp_getBuf(LampHandle* handle) 
{
    if (!handle || !handle->dHandle) return NULL;

    unsigned char * buf = (unsigned char*)calloc(9, sizeof(unsigned char));

    buf[3] = handle->cRed;
    buf[4] = handle->cGreen;
    buf[5] = handle->cBlue;
    buf[8] = handle->cSound + handle->cVolume;

    return buf;
}

LampError _lamp_updateHandleColor(int red, int green, int blue, LampHandle* handle)
{
    if (!handle || !handle->dHandle) return LAMP_ERROR_INVALIDHANDLE;

    handle->cRed = red;
    handle->cGreen = green;
    handle->cBlue = blue;

    return LAMP_ERROR_NOERROR;
}

LampError _lamp_updateHandleSound(LampSound sound, LampVolume volume, LampHandle* handle)
{
    if (!handle || !handle->dHandle) return LAMP_ERROR_INVALIDHANDLE;

    handle->cSound = sound;
    handle->cVolume = volume;

    return LAMP_ERROR_NOERROR;
}

LampError _lamp_updateDevice(LampHandle* handle)
{
    if (!handle || !handle->dHandle) return LAMP_ERROR_INVALIDHANDLE;

    unsigned char * buf = _lamp_getBuf(handle);

    if (buf == NULL) return LAMP_ERROR_BUFFERCONVERSIONFAILURE;

    int writeResult = hid_write(handle->dHandle, buf, 9);

    free (buf);

    if (!writeResult) return LAMP_ERROR_DEVICEWRITEFAILURE;

    return LAMP_ERROR_NOERROR;
}

LampError _lamp_mute(LampHandle* handle)
{
    if (!handle || !handle->dHandle) return LAMP_ERROR_INVALIDHANDLE;

    handle->cVolume = VOLUME_MUTE;

    return _lamp_updateDevice(handle);
}

LampHandle* lamp_init()
{
    LampHandle* h = (LampHandle*)malloc(sizeof(LampHandle));
    hid_device* handle = hid_open(BLL_VENDOR_ID, BLL_PRODUCT_ID, NULL);
    h->dHandle = handle;

    return h;
}

void lamp_free(LampHandle* handle)
{
    if (handle && handle->dHandle)
    {
        hid_close(handle->dHandle);
        free(handle);
    }
}

LampError lamp_setColor(int red, int green, int blue, LampHandle* handle)
{
    if (!handle || !handle->dHandle) return LAMP_ERROR_INVALIDHANDLE;

    LampError updateStatus = _lamp_updateHandleColor(red, green, blue, handle);

    if (LAMP_ERROR_NOERROR != updateStatus) return updateStatus;
    
    return _lamp_updateDevice(handle);
}

LampError lamp_setSound(LampSound sound, LampVolume volume, LampHandle* handle)
{
    if (!handle || !handle->dHandle) return LAMP_ERROR_INVALIDHANDLE;

    // First, mute to clear any currently set sound
    LampError muteStatus = _lamp_mute(handle);

    if (LAMP_ERROR_NOERROR != muteStatus) return muteStatus;

    // Second, update sound
    LampError updateStatus = _lamp_updateHandleSound(sound, volume, handle);

    if (LAMP_ERROR_NOERROR != updateStatus) return updateStatus;

    return _lamp_updateDevice(handle);
}

LampError lamp_setVolume(LampVolume volume, LampHandle* handle)
{
    return lamp_setSound(handle->cSound, volume, handle);
}