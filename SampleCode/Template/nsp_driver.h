
#include <stdio.h>
#include <string.h>
#include "NuMicro.h"

//Use this definition if the OneWire mode is ONEWIRE_MODE0(Idle high)or ONEWIRE_MODE1(Idle low)
#define ONEWIRE_MODE0
//Use this definition if the BUSY PIN is BUSYPIN_MODE0(high active)or BUSYPIN_MODE1(low active)
#define BUSYPIN_MODE1
//-----------------------------------------
#define CMD_PLAY_START      0x01
#define CMD_PLAY_END        0xDF
#define CMD_SET_VOL_NSP     0xE0
#define CMD_PLAY_EXP        0xF0	
#define CMD_MULTI_PLAY      0xF1
#define CMD_REPEAT          0xF2
#define CMD_STOP_REPEAT     0xF3
#define CMD_STOP_EQU        0xF5
#define CMD_RESET           0xF6
#define CMD_PWR_DOWN        0xF7
#define CMD_AUTO_SLEEP_EN   0xF8
#define CMD_AUTO_SLEEP_DIS  0xF9

#define CMD_PLAY_TX_BYTE           0x1
#define CMD_SET_VOL_TX_BYTE        0x1
#define CMD_PLAY_EXP_TX_BYTE       0x2
#define CMD_MULTI_PLAY_TX_BYTE     0x2
#define CMD_REPEAT_TX_BYTE         0x2
#define CMD_STOP_REPEAT_TX_BYTE    0x1
#define CMD_STOP_TX_BYTE           0x1
#define CMD_RESET_TX_BYTE          0x1
#define CMD_PWR_DOWN_TX_BYTE       0x1
#define CMD_AUTO_SLEEP_EN_TX_BYTE  0x1
#define CMD_AUTO_SLEEP_DIS_TX_BYTE 0x1
//-----------------------------------------
// #define BIT0	0x1
// #define BIT1	0x2
// #define BIT2	0x4
// #define BIT3	0x8
// #define BIT4	0x10
// #define BIT5	0x20
// #define BIT6	0x40
// #define BIT7	0x80
//-----------------------------------------
#define FLAG_TX_DOING		BIT7
//-----------------------------------------
#define MAX_EQU_LIST 		0x000B
//-----------------------------------------
    /* exact-width unsigned integer types */
typedef unsigned          char UINT8;
typedef unsigned short     int UINT16;
typedef unsigned           int UINT32;
typedef unsigned char *		PUINT8;

void HOST_BUS_Init(void);
void HOST_Init(UINT8* SP_VOL);
void GPIO2_SBUSY_INPUT(void);
UINT8 GPIO2_SDA_INPUT_READ(void);
void HOST_PIN_DURATION(void);
void HOST_Delay1MS(void);

UINT8 N_PLAY(UINT16 PlayListIndex);
UINT8 N_PLAY_EXP(UINT8 PlayListIndexExp);
UINT8 N_PLAY_REPEAT(UINT8 PlayRepeat);
UINT8 N_STOP_REPEAT(void);
UINT8 N_MULTI_PLAY(UINT8 PlayListNum,PUINT8 DATA_BUFFER);
UINT8 N_SET_VOL(UINT8 SP_VOL);
UINT8 N_STOP(void);
UINT8 N_RESET(void);
UINT8 N_PWR_DOWN(void);
UINT8 N_AUTO_SLEEP_EN(void);
UINT8 N_AUTO_SLEEP_DIS(void);

