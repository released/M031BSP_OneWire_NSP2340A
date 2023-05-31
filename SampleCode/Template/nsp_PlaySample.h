#ifndef _NSP_PLAY_SAMPLE_H_
#define _NSP_PLAY_SAMPLE_H_

/* exact-width unsigned integer types */
typedef unsigned          char UINT8;
typedef unsigned short     int UINT16;
typedef unsigned           int UINT32;
typedef unsigned char *		PUINT8;

/*----- Index Play Sample -----*/
void OneWire_IndexPlaySample(void);
/*----- MultiPlay Sample -----*/
void OneWire_MultiPlaySample(void);
/*----- Sleep Wake Up Sample -----*/
void OneWire_SleepWakeUpSample(void);
/*----- Volume Control Sample -----*/
void OneWire_VolumeCtrlSample(void);
/*----- Mixed Sample 1 -----*/
void OneWire_MixedSample1(void);
/*----- Mixed Sample 2 -----*/
void OneWire_MixedSample2(void);
/*----- Waiting for playback to finish -----*/
void OneWire_WaitPlayEND(void);
/*----- Ask for status -----*/
UINT8 OneWire_AskStatus(void);

#endif




