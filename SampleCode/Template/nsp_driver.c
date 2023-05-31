#include "nsp_driver.h"


//Use this definition if the input mode is QUASI
#define QUASI_MODE //�������ģʽ����QUASI����ʹ�ô˶���
//===========================================================
//Systerm intial, please modify according to your MCU platform

#define NSP_TIMER		(TIMER0)

#define SDA_PORT		(PC)
#define SDA_PIN_MASK	(BIT0)
#define SDA_SET_LOW		(PC0=0)
#define SDA_SET_HIGH	(PC0=1)

#define BUSY_PORT		(PC)
#define BUSY_PIN_MASK	(BIT1)
#define BUSY_SET_LOW	(PC1=0)
#define BUSY_SET_HIGH	(PC1=1)

/*#define SCLK_PORT		BP0R
#define SCLK_PORT_BIT	BIT0
#define SCLK_PORT_D		BP0D
#define SCLK_PORT_M		BP0M

#define SDA_PORT		BP0R
#define SDA_PORT_BIT	BIT1
#define SDA_PORT_D		BP0D
#define SDA_PORT_M		BP0M*/

UINT8 	u8HOST_FLAG=0; 
UINT8	u8CMD_TX_BYTE	= 0;
UINT8  	CMD_TX_DATA[34]={0};					
//===========================================================
//Systerm intial, please modify according to your MCU platform
void GPIO1_SDA_OUTPUT(void) 
{
//����Ϊ���ģʽ,Set to output mode
    GPIO_SetMode(SDA_PORT, SDA_PIN_MASK, GPIO_MODE_OUTPUT);
}

//Systerm intial, please modify according to your MCU platform
void GPIO1_SDA_OUTPUT0(void) 
{
//����Ϊ����͵�ƽ,Set to output low level
    SDA_SET_LOW;
}

//Systerm intial, please modify according to your MCU platform
void GPIO1_SDA_OUTPUT1(void) 
{
//����Ϊ����ߵ�ƽ,Set to output High level
    SDA_SET_HIGH;
}
//----------------------------------
//Systerm intial, please modify according to your MCU platform
void GPIO2_SBUSY_OUTPUT(void) 
{
//����Ϊ���ģʽ,Set to output mode
    // GPIO_SetMode(BUSY_PORT, BUSY_PIN_MASK, GPIO_MODE_OUTPUT);
    GPIO_SetMode(BUSY_PORT, BUSY_PIN_MASK, GPIO_MODE_QUASI);
}

//Systerm intial, please modify according to your MCU platform
void GPIO2_SBUSY_OUTPUT1(void) 
{
//����Ϊ����ߵ�ƽ,Set to output High level
    BUSY_SET_HIGH;}

//Systerm intial, please modify according to your MCU platform
void GPIO2_SBUSY_INPUT(void) 
{
#ifdef QUASI_MODE
	GPIO2_SBUSY_OUTPUT();
    	GPIO2_SBUSY_OUTPUT1();
#endif
//����Ϊ����ģʽ,Set to input mode

    // GPIO_SetMode(BUSY_PORT, BUSY_PIN_MASK, GPIO_MODE_INPUT); 	
}
//Systerm intial, please modify according to your MCU platform
UINT8 GPIO2_SDA_INPUT_READ() 
{
	UINT8 u8Res = 0;
//��ȡ������ֵ,Read input values
    if ((GPIO_GET_IN_DATA(BUSY_PORT)&BUSY_PIN_MASK)!=0) {
        u8Res = 1;
    }
    // if ((GPIO_GET_IN_DATA(SDA_PORT)&SDA_PIN_MASK)!=0) {
    //     u8Res = 1;
    // }	
	return (u8Res);
}
//----------------------------------
//Systerm intial, please modify according to your MCU platform, during which you can perform other operations on host
void HOST_PIN_DURATION(void)
{
//�����ӳ�ʱ��200us;  Delay time 200 us
    // TIMER_Delay(NSP_TIMER, 200);
	CLK_SysTickDelay(200);
}

//Systerm intial, please modify according to your MCU platform, during which you can perform other operations on host
void HOST_Delay1MS(void)
{
//�����ӳ�ʱ��1 ms;  Delay time 1ms
	// TIMER_Delay(NSP_TIMER, 1000);
	CLK_SysTickDelay(1000);
}
//===========================================================
//protocol
void HOST_BUS_Init(void)
{
	GPIO1_SDA_OUTPUT();
#ifdef ONEWIRE_MODE0
    	GPIO1_SDA_OUTPUT1();
#endif	
#ifdef ONEWIRE_MODE1
    	GPIO1_SDA_OUTPUT0();
#endif
}

void HOST_Init(UINT8* SP_VOL)
{
	*SP_VOL = 0x0F;	
	HOST_BUS_Init();
}

void ONEWIRE_HEADER(void)
{
	UINT8 i = 0;
#ifdef ONEWIRE_MODE0
    	GPIO1_SDA_OUTPUT0();
#endif	
#ifdef ONEWIRE_MODE1
    	GPIO1_SDA_OUTPUT1();
#endif
	for(i = 0; i < 5; i++)
	{
			HOST_Delay1MS();
	}
}

void ONEWIRE_TAIL(void)
{
	UINT8 i = 0;
#ifdef ONEWIRE_MODE0
    	GPIO1_SDA_OUTPUT1();
#endif	
#ifdef ONEWIRE_MODE1
    	GPIO1_SDA_OUTPUT0();
#endif
	for(i = 0; i < 5; i++)
	{
			HOST_Delay1MS();
	}
}

UINT8 ONEWIRE_TX_PROCESS()
{	
	UINT8 i = 0;
	UINT8 j = 0;
	UINT8 u8BitMap[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
	
	u8HOST_FLAG |= FLAG_TX_DOING;
	
	ONEWIRE_HEADER();
	
	//TX process
	for(i = 0; i < u8CMD_TX_BYTE; i++)
	{
		for(j = 0; j < 8; j++)
		{
			if ((CMD_TX_DATA[i] & u8BitMap[j]) !=0 )
			{
#ifdef ONEWIRE_MODE0
				GPIO1_SDA_OUTPUT1();
				HOST_PIN_DURATION();
				HOST_PIN_DURATION();
				HOST_PIN_DURATION();	
				GPIO1_SDA_OUTPUT0();
				HOST_PIN_DURATION();	
#endif	
#ifdef ONEWIRE_MODE1
				GPIO1_SDA_OUTPUT0();
				HOST_PIN_DURATION();	
				HOST_PIN_DURATION();	
				HOST_PIN_DURATION();	
				GPIO1_SDA_OUTPUT1();
				HOST_PIN_DURATION();
#endif
			}
			else
			{
#ifdef ONEWIRE_MODE0
				GPIO1_SDA_OUTPUT1();
				HOST_PIN_DURATION();
				GPIO1_SDA_OUTPUT0();
				HOST_PIN_DURATION();
				HOST_PIN_DURATION();
				HOST_PIN_DURATION();	
#endif	
#ifdef ONEWIRE_MODE1
				GPIO1_SDA_OUTPUT0();
				HOST_PIN_DURATION();		
				GPIO1_SDA_OUTPUT1();
				HOST_PIN_DURATION();
				HOST_PIN_DURATION();	
				HOST_PIN_DURATION();
#endif
			}
		}
	}
	
	u8HOST_FLAG &= ~FLAG_TX_DOING;
	
	ONEWIRE_TAIL();
	return 1;
}
//===========================================================
// function command
//----------------------------------
UINT8 N_PLAY_EXP(UINT8 PlayListIndexExp)
{
	UINT8 RTN = 0;
	if (PlayListIndexExp > 0xFF)
	{
		return 0;
	}
	else
	{
			CMD_TX_DATA[0] = CMD_PLAY_EXP;
			CMD_TX_DATA[1] = PlayListIndexExp;
			u8CMD_TX_BYTE = CMD_PLAY_EXP_TX_BYTE;
			RTN = ONEWIRE_TX_PROCESS();
	}
	return RTN;
}
//----------------------------------
UINT8 N_PLAY(UINT16 PlayListIndex)
{
	UINT8 RTN = 0;
	if (PlayListIndex > (CMD_PLAY_END+0xFF))
	{
		return 0;
	}
	else if (PlayListIndex > CMD_PLAY_END)
	{
			UINT8 PlayListIndexExp = PlayListIndex - CMD_PLAY_END;
		  RTN = N_PLAY_EXP(PlayListIndexExp);
	}
	else
	{
			CMD_TX_DATA[0] = PlayListIndex;
			u8CMD_TX_BYTE = CMD_PLAY_TX_BYTE;
			RTN = ONEWIRE_TX_PROCESS();
	}
	return RTN;
}
//----------------------------------
UINT8 N_MULTI_PLAY(UINT8 PlayListNum,PUINT8 DATA_BUFFER)
{
	UINT8 RTN = 0;
	UINT8 i = 0;
	if (PlayListNum > 32)
	{
		return 0;
	}	
	
	CMD_TX_DATA[0] = CMD_MULTI_PLAY;
	CMD_TX_DATA[1] = PlayListNum;
	for(i = 0; i < PlayListNum; i++)
	{
		CMD_TX_DATA[i+2] = *DATA_BUFFER;
		DATA_BUFFER++;
	}
	u8CMD_TX_BYTE = CMD_MULTI_PLAY_TX_BYTE+PlayListNum;
	RTN = ONEWIRE_TX_PROCESS();
	return RTN;
}
//----------------------------------
UINT8 N_PLAY_REPEAT(UINT8 PlayRepeat)
{
	UINT8 RTN = 0;
	CMD_TX_DATA[0] = CMD_REPEAT;
	CMD_TX_DATA[1] = PlayRepeat;
	u8CMD_TX_BYTE = CMD_REPEAT_TX_BYTE;	
	RTN = ONEWIRE_TX_PROCESS();
	return RTN;
}
//----------------------------------
UINT8 N_STOP_REPEAT()
{
	UINT8 RTN = 0;
	CMD_TX_DATA[0] = CMD_STOP_REPEAT;
	u8CMD_TX_BYTE = CMD_STOP_REPEAT_TX_BYTE;	
	RTN = ONEWIRE_TX_PROCESS();
	return RTN;
}
//----------------------------------
UINT8 N_SET_VOL(UINT8 SP_VOL)
{
	UINT8 RTN = 0;
	if (SP_VOL > 0xF)
	{
		return 0;
	}
	CMD_TX_DATA[0] = CMD_SET_VOL_NSP + SP_VOL;
	u8CMD_TX_BYTE = CMD_SET_VOL_TX_BYTE;
	RTN = ONEWIRE_TX_PROCESS();
	return RTN;
}
//----------------------------------
UINT8 N_STOP()
{
	UINT8 RTN = 0;
	CMD_TX_DATA[0] = CMD_STOP_EQU;
	u8CMD_TX_BYTE = CMD_STOP_TX_BYTE;
	RTN = ONEWIRE_TX_PROCESS();	
	return RTN;
}
//----------------------------------
UINT8 N_RESET()
{
	UINT8 RTN = 0;
	CMD_TX_DATA[0] = CMD_RESET;
	u8CMD_TX_BYTE = CMD_RESET_TX_BYTE;
	RTN = ONEWIRE_TX_PROCESS();	
	return RTN;
}
//----------------------------------
UINT8 N_PWR_DOWN()
{
	UINT8 RTN = 0;
	CMD_TX_DATA[0] = CMD_PWR_DOWN;
	u8CMD_TX_BYTE = CMD_PWR_DOWN_TX_BYTE;
	RTN = ONEWIRE_TX_PROCESS();	
	return RTN;
}
//----------------------------------
UINT8 N_AUTO_SLEEP_EN()
{
	UINT8 RTN = 0;
	CMD_TX_DATA[0] = CMD_AUTO_SLEEP_EN;
	u8CMD_TX_BYTE = CMD_AUTO_SLEEP_EN_TX_BYTE;
	RTN = ONEWIRE_TX_PROCESS();	
	return RTN;
}
//----------------------------------
UINT8 N_AUTO_SLEEP_DIS()
{
	UINT8 RTN = 0;
	CMD_TX_DATA[0] = CMD_AUTO_SLEEP_DIS;
	u8CMD_TX_BYTE = CMD_AUTO_SLEEP_DIS_TX_BYTE;
	RTN = ONEWIRE_TX_PROCESS();	
	return RTN;
}


