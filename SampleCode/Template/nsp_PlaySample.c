#include "nsp_driver.h"
#include "nsp_PlaySample.h"

//===========================================================
#define MULTI_PLAY_MAX 32

UINT16 	u16PlayListIndex=0;			//play list index
UINT8 	u8SP_VOL=0;				//speech volume:0~0xF

/*----- Index Play Sample -----*/
void OneWire_IndexPlaySample(void)
{
    UINT16 temp = 0;                                    // ������ʼ��
                                                        // init parameter            	
    HOST_BUS_Init();                                    // IO�����ʼ��
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // æ�ߣ�SP_BUSY�����ܳ�ʼ��
                                                        // SP busy pin Initialization
    N_PLAY(1);                                          // ���� Index 1 ��Ч ,(1 <= Index < 478)
                                                        // Play Index 1 Resource,(1 <= Index < 478)
    OneWire_WaitPlayEND();                              // �ȴ��������
                                                        // Waiting for playback to finish
    N_PLAY(2);                                          // ���� Index 2 ��Ч 
                                                        // Play Index 2 Resource
    N_PLAY_REPEAT(0);                                   // ѭ������ Index 2 ��Ч
                                                        // loop Index 2
                                                        
    for (temp= 0; temp < 8000; temp++)                  // ѭ������ 8 ����
    {                                                   // Loop for 8 seconds
         HOST_Delay1MS();                               
    }
    
    N_STOP_REPEAT();                                    // ��鲥�Ž���֮��ֹͣ����
                                                        // stop playback after this time                                                     
                                                        
    OneWire_WaitPlayEND();                              // �ȴ��������
                                                        // Waiting for playback to finish
    while(1);
}


/*----- MultiPlay Sample -----*/
void OneWire_MultiPlaySample(void)
{
    UINT8 MultiPlayBuffer[10] = {1,1,2,2,3,3,4,4,5,5};  // �趨���ز���˳����� 
                                                        // Set the multiplay order parameters.
    
    HOST_BUS_Init();                                    // IO�����ʼ��
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // æ�ߣ�SP_BUSY�����ܳ�ʼ��
                                                        // SP busy pin Initialization
    N_MULTI_PLAY(6,&MultiPlayBuffer[0]);                // �趨6�ض��鲥�ţ�������ʼ�� Buffer[0]��ʼ 
                                                        // Set 6 groups of multi-play, starting playback starts with Buffer[0].
    
    OneWire_WaitPlayEND();                              // �ȴ�����Index 1 -> Index 1 -> Index2 -> Index 2 -> Index 3 -> Index 3 ������
                                                        // Waiting for playback Index 1 -> Index 1 -> Index2 -> Index 2 -> Index 3 -> Index 3 is finished
                                                        
    N_MULTI_PLAY(3,&MultiPlayBuffer[0]);                // �趨3�ض��鲥�ţ�������ʼ�� Buffer[0]��ʼ 
                                                        // Set 3 groups of multi-play, starting playback starts with Buffer[0].
                                                        
    N_PLAY_REPEAT(2);                                   // �ظ����� Index 1 -> Index 1 -> Index2 ��Ч 2��
                                                        // repeat 2 times
    
    OneWire_WaitPlayEND();                              // �ȴ�����Index 1 -> Index 1 -> Index2 ��Ч 2�������
                                                        // Waiting for playback (Index 1 -> Index 1 -> Index2)* 2times is finished                                                                 
    while(1);
}

/*----- Sleep Wake Up Sample -----*/
void OneWire_SleepWakeUpSample(void)
{
    UINT16 temp = 0;                                    // ������ʼ��				
                                                        // init parameter 
    HOST_BUS_Init();                                    // IO�����ʼ��
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // æ�ߣ�SP_BUSY�����ܳ�ʼ��
                                                        // SP busy pin Initialization
                                                        
    N_AUTO_SLEEP_DIS();                                 // GUI�趨sleep time�������£��ر��Զ����߹��ܣ�ֻ��N_PWR_DOWN������˯
                                                        // GUIû���趨sleep time��ָ����Ч    
                                                        // (If user sets the sleep time in the GUI), disable the automatic sleep function, only N_PWR_DOWN can enter sleep
                                                        // The command is invalid if user does not set the sleep time in the GUI
                                                         
                                                                                                            
    N_PLAY(1);                                          // ���� Index 1 ��Ч ,(1 <= Index < 478)
                                                        // Play Index 1 Resource,(1 <= Index < 478)
    for (temp= 0; temp < 0x4000; temp++)                // ���� 4 sec
    {                                                   // play for 4 sec
         HOST_Delay1MS();
    }
    N_STOP();                                           // ǿ�ƽ�������
                                                        // stop speech   
    N_PWR_DOWN();                                       // ǿ��ֱ�ӽ�������
                                                        // Go straight to sleep.                                                 
    for (temp= 0; temp < 0x4000; temp++)                // ���� 4 sec
    {                                                   // enter sleep for 4 sec
         HOST_Delay1MS();
    }
    
    N_AUTO_SLEEP_EN();                                  // ���Զ����߹��ܣ�GUI�趨sleep timeʱ����û�в��ţ�û����ָ�û��LVD��checksumִ�оͻ���˯
                                                        // GUIû���趨sleep time��ָ����Ч    
                                                        // Enable the automatic sleep function:if no play, no new CMD, no LVD and checksum execution 
                                                        // during the sleep time(setting in the GUI),then the NSP will enter sleep
    							// The command is invalid if user does not set the sleep time in the GUI
    
    N_PLAY(2);                                          // ���� Index 2 ��Ч 
                                                        // Play Index 2 Resource
    OneWire_WaitPlayEND();                              // �ȴ�������ϣ�GUI�趨sleep timeʱ����û�в��ţ�û����ָ�û��LVD��checksum,NSP�Զ���������
                                                        // Waiting for playback to finish
                                                        // If no play, no new CMD, no LVD and checksum execution 
                                                        // during the sleep time(setting in the GUI),then the NSP will enter sleep
    while(1);
}

/*----- Volume Control Sample -----*/
void OneWire_VolumeCtrlSample(void)
{
    UINT16 temp = 0;                                    // ������ʼ��
    u8SP_VOL = 0xF;                                     // init parameter 

    HOST_BUS_Init();                                    // IO �����ʼ�� 
                                                        // IO Interface Initialization
    N_PLAY(1);                                          // ���� Index 1 ��Ч
                                                        // Play Index 1 Resource
    N_SET_VOL(u8SP_VOL);                                // �������� set volume		
    N_PLAY_REPEAT(0);                                   // ѭ������ Index 1 ��Ч
                                                        // Looping Index 1
							
    while ( u8SP_VOL >= 0x1 )	                        // ÿ��3����,�����ݼ�1,ֱ�������ݼ���1Ϊֹ
    {                                                   // the volume is reduced by 1 every 3 seconds until the volume is decremented to 1
          u8SP_VOL -- ;
          N_SET_VOL(u8SP_VOL);                          // �������� set volume					
          for (temp= 0; temp <3000; temp++)		
          {					
             HOST_Delay1MS();
          }	
    }  
     
    N_STOP_REPEAT();                                    // ��鲥�Ž���֮��ֹͣ����
                                                        // stop playback after this time 

    while(1);       
}

/*----- Mixed Sample 1 -----*/
/*----- ��ʱ�๤���������� MCU �οɿ��ƨ� -----*/
void OneWire_MixedSample1(void)
{
    HOST_BUS_Init();                                    // IO �����ʼ�� 
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // æ�ߣ�SP_BUSY�����ܳ�ʼ��
                                                        // SP busy pin Initialization
    N_PLAY(1);                                          // ���� Index 1 ��Ч
                                                        // Play Index 1 Resource
    while(1)
    {
        //.......
        //���� MCU �����ʽ
        //Host MCU Handle 
        if(OneWire_AskStatus())                         // ѯ�� NSP ״̬
                                                        // Ask NSP Status
        {
            break;                                      // NSP ������ϣ��뿪��ѭ���C
                                                        // After the NSP is finished playing , leave this loop.
        }
    }
    while(1);
}


/*----- Mixed Sample 2 -----*/
void OneWire_MixedSample2(void)
{
    UINT16 temp = 0;                                    // ������ʼ��	
    PUINT8 PLAY_BUFFER = 0;                             // init parameter 
    UINT8 g_au8MultiPlayBuf[MULTI_PLAY_MAX]={3,1,2,4,0,0,0,0,0,0};// �趨���ز���˳����� 
                                                        // Set the multiplay order parameters.
    u8SP_VOL = 0x0F;
   
    HOST_BUS_Init();                                    // IO �����ʼ�� 
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // æ�ߣ�SP_BUSY�����ܳ�ʼ��
                                                        // SP busy pin Initialization
    N_SET_VOL(u8SP_VOL);                                // �������� set volume		
		
    for (u16PlayListIndex= 1; u16PlayListIndex <= MAX_EQU_LIST; u16PlayListIndex++)//������Ч��ͷ��β����һ��
    {                                                   // Play all the resource from index 1 to MAX_EQU_LIST
         N_PLAY(u16PlayListIndex);                      // ���� ��Ч 
                                                        // Play Resource
         OneWire_WaitPlayEND();                         // �ȴ��������
                                                        // Waiting for playback to finish
    }
	
    N_RESET();                                          // NSP����,NSP reset
	
    for (u16PlayListIndex= 1; u16PlayListIndex <= MAX_EQU_LIST; u16PlayListIndex++)//�� index1 ��ʼÿ����10 sec ����һ����Ч
    {                                                   // Play the next index every 10 seconds from index1
         N_PLAY(u16PlayListIndex);                      // ���� ��Ч 
                                                        // Play Resource
         for (temp= 0; temp < 10000; temp++)
         {
              HOST_Delay1MS();
         }
         N_STOP();                                      // ǿ�ƽ�������
                                                        // stop speech   				
    }
			
    N_PWR_DOWN();                                       // ǿ��ֱ�ӽ�������	
                                                        // Go straight to sleep	
    for (temp= 0; temp < 2000; temp++)                  // ʱ����� 2 ����
    {                                                   // 2 sec
         HOST_Delay1MS(); 
    }
			
    N_PLAY(2);                                          // ���� Index 2 ��Ч 
                                                        // Play Index 2 Resource
    N_PLAY_REPEAT(5);                                   // �ظ����� Index 2 ��Ч 5��
                                                        // repeat 5 times
    OneWire_WaitPlayEND();                              // �ȴ��������
                                                        // Waiting for playback to finish
		
    N_PLAY(3);                                          // ���� Index 3 ��Ч 
                                                        // Play Index 3 Resource
    N_PLAY_REPEAT(0);                                   // ѭ������ Index 3 ��Ч
                                                        // Looping Index 3
    for (temp= 0; temp < 2000; temp++)
    {
         HOST_Delay1MS();
    }
    
    N_STOP_REPEAT();                                    // ��鲥�Ž���֮��ֹͣ����
                                                        // stop playback after this time 
    OneWire_WaitPlayEND();                              // �ȴ��������
                                                        // Waiting for playback to finish                                                    
                                                        
		
    PLAY_BUFFER = g_au8MultiPlayBuf;
    N_MULTI_PLAY(4,PLAY_BUFFER);                        // �趨4�ض��鲥�ţ�������ʼ�� PLAY_BUFFER[0]��ʼ 
                                                        // Set 4 groups of multi-play, starting playback starts with PLAY_BUFFER[0].
    OneWire_WaitPlayEND();                              // �ȴ�����Index 3 -> Index 1 -> Index2 -> Index 4 ������
                                                        // Waiting for playback Index 3 -> Index 1 -> Index2 -> Index 4 is finished

    g_au8MultiPlayBuf[0] = 7;                           // �趨���ز���˳����� 
    g_au8MultiPlayBuf[1] = 5;                           // Set the multiplay order parameters.
    g_au8MultiPlayBuf[2] = 3;
    g_au8MultiPlayBuf[3] = 1;
    g_au8MultiPlayBuf[4] = 6;
    PLAY_BUFFER = g_au8MultiPlayBuf;
    N_MULTI_PLAY(5,PLAY_BUFFER);                        // �趨5�ض��鲥�ţ�������ʼ�� PLAY_BUFFER[0]��ʼ 
                                                        // Set 5 groups of multi-play, starting playback starts with PLAY_BUFFER[0].
    N_PLAY_REPEAT(2);                                   // �ظ����� Index 7 -> Index 5 -> Index3 -> Index 1 -> Index6 ��Ч 2��
                                                        // repeat 2 times
    
    OneWire_WaitPlayEND();                              // �ȴ����� Index 7 -> Index 5 -> Index3 -> Index 1 -> Index6 ��Ч 2�������
                                                        // Waiting for playback ( Index 7 -> Index 5 -> Index3 -> Index 1 -> Index6 )* 2times is finished
}


/*----- �ȴ����Ž��� -----*/ 
/*----- Waiting for playback to finish -----*/
void OneWire_WaitPlayEND(void)
{
#ifdef BUSYPIN_MODE0
    while(GPIO2_SDA_INPUT_READ() == 1)                //�ߵ�ƽ��Ч,��������ʱGPIO2 ����ߵ�ƽ,������ʱGPIO2 ����͵�ƽ
                                                      //high active��GPIO2 output high during speech is speech,GPIO2 output low when speech is finish
#endif			
#ifdef BUSYPIN_MODE1
    while(GPIO2_SDA_INPUT_READ() == 0)                //�͵�ƽ��Ч,��������ʱGPIO2 ����͵�ƽ,������ʱGPIO2 ����ߵ�ƽ
                                                      //low active��GPIO2 output low during speech is speech,GPIO2 output high when speech is finish
#endif		
    {
         HOST_Delay1MS();
    }
}
                                                         
/*----- ѯ��״̬ -----*/
/*----- Ask for status -----*/
UINT8 OneWire_AskStatus(void)
{
#ifdef BUSYPIN_MODE0
    if(GPIO2_SDA_INPUT_READ() == 1)                
#endif			
#ifdef BUSYPIN_MODE1
    if(GPIO2_SDA_INPUT_READ() == 0)                                                 //low active��GPIO2 output low during speech is speech,GPIO2 output high when speech is finish
#endif	
          return 0;                                // �ش�ֵ 0 ���� NSP �ڲ�����
                                                   // The return value of 0 means that the NSP is playing,
    else
          return 1;                                // �ش�ֵ 1 ���� NSP ���ڲ�����
                                                   // The return value 1 means that the NSP is not playing
}

