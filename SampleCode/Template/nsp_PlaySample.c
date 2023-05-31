#include "nsp_driver.h"
#include "nsp_PlaySample.h"

//===========================================================
#define MULTI_PLAY_MAX 32

UINT16 	u16PlayListIndex=0;			//play list index
UINT8 	u8SP_VOL=0;				//speech volume:0~0xF

/*----- Index Play Sample -----*/
void OneWire_IndexPlaySample(void)
{
    UINT16 temp = 0;                                    // 参数初始化
                                                        // init parameter            	
    HOST_BUS_Init();                                    // IO界面初始化
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // 忙线（SP_BUSY）功能初始化
                                                        // SP busy pin Initialization
    N_PLAY(1);                                          // 播放 Index 1 音效 ,(1 <= Index < 478)
                                                        // Play Index 1 Resource,(1 <= Index < 478)
    OneWire_WaitPlayEND();                              // 等待播放完毕
                                                        // Waiting for playback to finish
    N_PLAY(2);                                          // 播放 Index 2 音效 
                                                        // Play Index 2 Resource
    N_PLAY_REPEAT(0);                                   // 循环播放 Index 2 音效
                                                        // loop Index 2
                                                        
    for (temp= 0; temp < 8000; temp++)                  // 循环播放 8 秒钟
    {                                                   // Loop for 8 seconds
         HOST_Delay1MS();                               
    }
    
    N_STOP_REPEAT();                                    // 这遍播放结束之后，停止播放
                                                        // stop playback after this time                                                     
                                                        
    OneWire_WaitPlayEND();                              // 等待播放完毕
                                                        // Waiting for playback to finish
    while(1);
}


/*----- MultiPlay Sample -----*/
void OneWire_MultiPlaySample(void)
{
    UINT8 MultiPlayBuffer[10] = {1,1,2,2,3,3,4,4,5,5};  // 设定多重播放顺序参数 
                                                        // Set the multiplay order parameters.
    
    HOST_BUS_Init();                                    // IO界面初始化
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // 忙线（SP_BUSY）功能初始化
                                                        // SP busy pin Initialization
    N_MULTI_PLAY(6,&MultiPlayBuffer[0]);                // 设定6重多组播放，播放起始由 Buffer[0]开始 
                                                        // Set 6 groups of multi-play, starting playback starts with Buffer[0].
    
    OneWire_WaitPlayEND();                              // 等待播放Index 1 -> Index 1 -> Index2 -> Index 2 -> Index 3 -> Index 3 结束
                                                        // Waiting for playback Index 1 -> Index 1 -> Index2 -> Index 2 -> Index 3 -> Index 3 is finished
                                                        
    N_MULTI_PLAY(3,&MultiPlayBuffer[0]);                // 设定3重多组播放，播放起始由 Buffer[0]开始 
                                                        // Set 3 groups of multi-play, starting playback starts with Buffer[0].
                                                        
    N_PLAY_REPEAT(2);                                   // 重复播放 Index 1 -> Index 1 -> Index2 音效 2遍
                                                        // repeat 2 times
    
    OneWire_WaitPlayEND();                              // 等待播放Index 1 -> Index 1 -> Index2 音效 2遍结束
                                                        // Waiting for playback (Index 1 -> Index 1 -> Index2)* 2times is finished                                                                 
    while(1);
}

/*----- Sleep Wake Up Sample -----*/
void OneWire_SleepWakeUpSample(void)
{
    UINT16 temp = 0;                                    // 参数初始化				
                                                        // init parameter 
    HOST_BUS_Init();                                    // IO界面初始化
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // 忙线（SP_BUSY）功能初始化
                                                        // SP busy pin Initialization
                                                        
    N_AUTO_SLEEP_DIS();                                 // GUI设定sleep time的条件下，关闭自动休眠功能，只有N_PWR_DOWN才能入睡
                                                        // GUI没有设定sleep time则指令无效    
                                                        // (If user sets the sleep time in the GUI), disable the automatic sleep function, only N_PWR_DOWN can enter sleep
                                                        // The command is invalid if user does not set the sleep time in the GUI
                                                         
                                                                                                            
    N_PLAY(1);                                          // 播放 Index 1 音效 ,(1 <= Index < 478)
                                                        // Play Index 1 Resource,(1 <= Index < 478)
    for (temp= 0; temp < 0x4000; temp++)                // 播放 4 sec
    {                                                   // play for 4 sec
         HOST_Delay1MS();
    }
    N_STOP();                                           // 强制结束播放
                                                        // stop speech   
    N_PWR_DOWN();                                       // 强制直接进入休眠
                                                        // Go straight to sleep.                                                 
    for (temp= 0; temp < 0x4000; temp++)                // 休眠 4 sec
    {                                                   // enter sleep for 4 sec
         HOST_Delay1MS();
    }
    
    N_AUTO_SLEEP_EN();                                  // 打开自动休眠功能，GUI设定sleep time时间内没有播放，没有新指令，没有LVD和checksum执行就会入睡
                                                        // GUI没有设定sleep time则指令无效    
                                                        // Enable the automatic sleep function:if no play, no new CMD, no LVD and checksum execution 
                                                        // during the sleep time(setting in the GUI),then the NSP will enter sleep
    							// The command is invalid if user does not set the sleep time in the GUI
    
    N_PLAY(2);                                          // 播放 Index 2 音效 
                                                        // Play Index 2 Resource
    OneWire_WaitPlayEND();                              // 等待播放完毕，GUI设定sleep time时间内没有播放，没有新指令，没有LVD和checksum,NSP自动进入休眠
                                                        // Waiting for playback to finish
                                                        // If no play, no new CMD, no LVD and checksum execution 
                                                        // during the sleep time(setting in the GUI),then the NSP will enter sleep
    while(1);
}

/*----- Volume Control Sample -----*/
void OneWire_VolumeCtrlSample(void)
{
    UINT16 temp = 0;                                    // 参数初始化
    u8SP_VOL = 0xF;                                     // init parameter 

    HOST_BUS_Init();                                    // IO 界面初始化 
                                                        // IO Interface Initialization
    N_PLAY(1);                                          // 播放 Index 1 音效
                                                        // Play Index 1 Resource
    N_SET_VOL(u8SP_VOL);                                // 设置音量 set volume		
    N_PLAY_REPEAT(0);                                   // 循环播放 Index 1 音效
                                                        // Looping Index 1
							
    while ( u8SP_VOL >= 0x1 )	                        // 每隔3秒钟,音量递减1,直到音量递减至1为止
    {                                                   // the volume is reduced by 1 every 3 seconds until the volume is decremented to 1
          u8SP_VOL -- ;
          N_SET_VOL(u8SP_VOL);                          // 设置音量 set volume					
          for (temp= 0; temp <3000; temp++)		
          {					
             HOST_Delay1MS();
          }	
    }  
     
    N_STOP_REPEAT();                                    // 这遍播放结束之后，停止播放
                                                        // stop playback after this time 

    while(1);       
}

/*----- Mixed Sample 1 -----*/
/*----- 分时多工处理，播放中 MCU 任可控制 -----*/
void OneWire_MixedSample1(void)
{
    HOST_BUS_Init();                                    // IO 界面初始化 
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // 忙线（SP_BUSY）功能初始化
                                                        // SP busy pin Initialization
    N_PLAY(1);                                          // 播放 Index 1 音效
                                                        // Play Index 1 Resource
    while(1)
    {
        //.......
        //主控 MCU 处理程式
        //Host MCU Handle 
        if(OneWire_AskStatus())                         // 询问 NSP 状态
                                                        // Ask NSP Status
        {
            break;                                      // NSP 播放完毕，离开此循环
                                                        // After the NSP is finished playing , leave this loop.
        }
    }
    while(1);
}


/*----- Mixed Sample 2 -----*/
void OneWire_MixedSample2(void)
{
    UINT16 temp = 0;                                    // 参数初始化	
    PUINT8 PLAY_BUFFER = 0;                             // init parameter 
    UINT8 g_au8MultiPlayBuf[MULTI_PLAY_MAX]={3,1,2,4,0,0,0,0,0,0};// 设定多重播放顺序参数 
                                                        // Set the multiplay order parameters.
    u8SP_VOL = 0x0F;
   
    HOST_BUS_Init();                                    // IO 界面初始化 
                                                        // IO Interface Initialization
    GPIO2_SBUSY_INPUT();                                // 忙线（SP_BUSY）功能初始化
                                                        // SP busy pin Initialization
    N_SET_VOL(u8SP_VOL);                                // 设置音量 set volume		
		
    for (u16PlayListIndex= 1; u16PlayListIndex <= MAX_EQU_LIST; u16PlayListIndex++)//所有音效从头到尾播放一遍
    {                                                   // Play all the resource from index 1 to MAX_EQU_LIST
         N_PLAY(u16PlayListIndex);                      // 播放 音效 
                                                        // Play Resource
         OneWire_WaitPlayEND();                         // 等待播放完毕
                                                        // Waiting for playback to finish
    }
	
    N_RESET();                                          // NSP重启,NSP reset
	
    for (u16PlayListIndex= 1; u16PlayListIndex <= MAX_EQU_LIST; u16PlayListIndex++)//从 index1 开始每播放10 sec 换下一个音效
    {                                                   // Play the next index every 10 seconds from index1
         N_PLAY(u16PlayListIndex);                      // 播放 音效 
                                                        // Play Resource
         for (temp= 0; temp < 10000; temp++)
         {
              HOST_Delay1MS();
         }
         N_STOP();                                      // 强制结束播放
                                                        // stop speech   				
    }
			
    N_PWR_DOWN();                                       // 强制直接进入休眠	
                                                        // Go straight to sleep	
    for (temp= 0; temp < 2000; temp++)                  // 时间持续 2 秒钟
    {                                                   // 2 sec
         HOST_Delay1MS(); 
    }
			
    N_PLAY(2);                                          // 播放 Index 2 音效 
                                                        // Play Index 2 Resource
    N_PLAY_REPEAT(5);                                   // 重复播放 Index 2 音效 5遍
                                                        // repeat 5 times
    OneWire_WaitPlayEND();                              // 等待播放完毕
                                                        // Waiting for playback to finish
		
    N_PLAY(3);                                          // 播放 Index 3 音效 
                                                        // Play Index 3 Resource
    N_PLAY_REPEAT(0);                                   // 循环播放 Index 3 音效
                                                        // Looping Index 3
    for (temp= 0; temp < 2000; temp++)
    {
         HOST_Delay1MS();
    }
    
    N_STOP_REPEAT();                                    // 这遍播放结束之后，停止播放
                                                        // stop playback after this time 
    OneWire_WaitPlayEND();                              // 等待播放完毕
                                                        // Waiting for playback to finish                                                    
                                                        
		
    PLAY_BUFFER = g_au8MultiPlayBuf;
    N_MULTI_PLAY(4,PLAY_BUFFER);                        // 设定4重多组播放，播放起始由 PLAY_BUFFER[0]开始 
                                                        // Set 4 groups of multi-play, starting playback starts with PLAY_BUFFER[0].
    OneWire_WaitPlayEND();                              // 等待播放Index 3 -> Index 1 -> Index2 -> Index 4 结束
                                                        // Waiting for playback Index 3 -> Index 1 -> Index2 -> Index 4 is finished

    g_au8MultiPlayBuf[0] = 7;                           // 设定多重播放顺序参数 
    g_au8MultiPlayBuf[1] = 5;                           // Set the multiplay order parameters.
    g_au8MultiPlayBuf[2] = 3;
    g_au8MultiPlayBuf[3] = 1;
    g_au8MultiPlayBuf[4] = 6;
    PLAY_BUFFER = g_au8MultiPlayBuf;
    N_MULTI_PLAY(5,PLAY_BUFFER);                        // 设定5重多组播放，播放起始由 PLAY_BUFFER[0]开始 
                                                        // Set 5 groups of multi-play, starting playback starts with PLAY_BUFFER[0].
    N_PLAY_REPEAT(2);                                   // 重复播放 Index 7 -> Index 5 -> Index3 -> Index 1 -> Index6 音效 2遍
                                                        // repeat 2 times
    
    OneWire_WaitPlayEND();                              // 等待播放 Index 7 -> Index 5 -> Index3 -> Index 1 -> Index6 音效 2遍结束
                                                        // Waiting for playback ( Index 7 -> Index 5 -> Index3 -> Index 1 -> Index6 )* 2times is finished
}


/*----- 等待播放结束 -----*/ 
/*----- Waiting for playback to finish -----*/
void OneWire_WaitPlayEND(void)
{
#ifdef BUSYPIN_MODE0
    while(GPIO2_SDA_INPUT_READ() == 1)                //高电平有效,语音播放时GPIO2 输出高电平,语句结束时GPIO2 输出低电平
                                                      //high active：GPIO2 output high during speech is speech,GPIO2 output low when speech is finish
#endif			
#ifdef BUSYPIN_MODE1
    while(GPIO2_SDA_INPUT_READ() == 0)                //低电平有效,语音播放时GPIO2 输出低电平,语句结束时GPIO2 输出高电平
                                                      //low active：GPIO2 output low during speech is speech,GPIO2 output high when speech is finish
#endif		
    {
         HOST_Delay1MS();
    }
}
                                                         
/*----- 询问状态 -----*/
/*----- Ask for status -----*/
UINT8 OneWire_AskStatus(void)
{
#ifdef BUSYPIN_MODE0
    if(GPIO2_SDA_INPUT_READ() == 1)                
#endif			
#ifdef BUSYPIN_MODE1
    if(GPIO2_SDA_INPUT_READ() == 0)                                                 //low active：GPIO2 output low during speech is speech,GPIO2 output high when speech is finish
#endif	
          return 0;                                // 回传值 0 代表 NSP 在播放中
                                                   // The return value of 0 means that the NSP is playing,
    else
          return 1;                                // 回传值 1 代表 NSP 不在播放中
                                                   // The return value 1 means that the NSP is not playing
}

