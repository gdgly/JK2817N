#include "test.h"
#include "LCD/lpc177x_8x_lcd.h"
#include "lpc177x_8x_uart.h"
#include "Globalvalue/GlobalValue.h"
#include "use_disp.h"
#include "EX_SDRAM/EX_SDRAM.H"
#include "stdint.h"
#include "key/key.h"
#include "uart.h"
#include "timer.h"
#include "lpc177x_8x_eeprom.h"
#include "math.h"
#include "LCD/AsciiLib.h"
#include  "stdio.h"
#include  "usbhost_fat.h"
#include 	"use_disp.h"
#include "lpc177x_8x_rtc.h"
#include "open.h"

const char *PASSWORD="12345789";
uint8_t  Avg_Test_Main[25][6];
uint8_t  Avg_Test_Second[25][6];
uint8_t  Avg_Count=0;
const uint8_t Disp_Unit1[]={'p','n','u','m',' ','k','M'};
const uint8_t Uart_Ordel[]={0x60,0x70,0x71,0x80,0x90,0xa0,0xb0,0xc0,0xe0};
const uint8_t READDATA[7]={0xAB,0x01,0x06,0x03,0x08,0xbf,'\0'};
//const uint8_t Disp_Main_Ord[][3]={
//	{1,1,0},
//	{1,1,1},
//	{1,1,2},
//	{1,1,3},//Cp
//	
//	{0,1,0},
//	{0,1,1},
//	{0,1,3},//Cs
//	{1,0,0},
//	{1,0,0},
//	{1,0,0},
//	{1,0,1},//Lp
//	{1,0,3},
//	{1,0,4},
//	{1,0,0},
//	{0,0,0},//Ls
//	{0,0,1},
//	{0,0,5},
//	{0,0,4},
//	{0,3,6},//Z
//	{0,3,7},
//	{0,4,6},//Y
//	{0,4,7},
//	{0,2,8},//R
//	{1,2,1},
//	{0,2,1},
//	{1,1,0},
//	{1,5,9},//GB
//	//{1,1,0},
//	};

const uint8_t Disp_Main_Ord[][3]={
	{1,1,6},
	{1,1,7},
	{1,1,5},
	{1,1,2},//Cp-D,CP-Q,CP-G,CP-RP,
	
	{0,1,6},
	{0,1,7},
	{0,1,2},//Cs-D,Cs-Q,Cs-Rs
	
	{1,0,7},
	{1,0,2},
	{1,0,2},
	{1,0,6},//Lp-Q,Lp-Rp,Lp-Rd,Lp-D
	
	{0,0,6},
	{0,0,7},
	{0,0,2},
	{0,0,2},//Ls-D,Ls-Q,Ls-Rs,Ls-Rd
	
	{0,3,6},
	{0,3,2},//Z-d,Z-r
	{0,4,6},
	{0,4,2},//Y-d,Y-r
	{0,2,8},//R-X
	{1,2,7},//Rp-Q
	{0,2,7},//Rs-Q
	{0,5,9},//G-B
	};	
//==========================================================
//函数名称：Power_Process
//函数功能：上电处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.26 08:53
//备注说明：开机长按SET进入校准调试模式
//==========================================================

void Power_Process(void)
{
	uint16_t i;
//	u8 buff[4096];
	u8 key;
    u8 keynum;
    Disp_Coordinates_Typedef Debug_Cood;
	HW_keyInt();
	GPIO_Led_Configuration();
	Turnon_backlight();
	Beep_on();
	HW_Sendvalueto164(0);
	Bais_LedOff();
	Lock_LedOff();
	Pass_Led();
	Power_Off_led();
	#ifdef DISP_JK
	lcd_image((uint8_t *)gImage_open);
	#endif
	InitGlobalValue();//初始化全局变量
	init_timer(0, 20);//定时器初始化
	
	enable_timer(0);
	
	ReadSavedata();
	Set_Compbcd_float();

	i=0;//显示延时
Delay(2000);
	while(GetSystemStatus()==SYS_STATUS_POWER)
	{    
        i++;
        Delay(10);
        if(i>10)
            SetSystemStatus(SYS_STATUS_TEST);//待测状态
         key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	
			//Disp_Flag=1;
			Key_Beep();
            switch(key)
			{
				case Key_FAST:
                    lcd_Clear(LCD_COLOR_TEST_BACK);
                    Debug_Cood.xpos=180;
                    Debug_Cood.ypos =120;
                    Debug_Cood.lenth=120;
                    input_password(&Debug_Cood);
                    break;//0xD0
                case Key_DOT:
                    Clear_Instr();
                    
                
                    break;
                default:
                    break;
                }
            }
//		i++;
//		if(i>POWERON_DISP_TIME)//延时20*100mS=2s后自动退出
			

//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);//等待按键(100*10ms/10=100ms)
//		switch(key)
//		{
////			case KEY_SET:		//设置键
//			case L_KEY_SET:		//长按设置键
//				//SetSystemStatus(SYS_STATUS_DEBUG);//调试状态
//                
//				Beep_One();//蜂鸣器响一声
//				break;
//	
//			case KEY_UP:		//上键
//			case L_KEY_UP:		//长按上键
//				break;
//	
//			case KEY_DOWN:		//下键
//			case L_KEY_DOWN:	//长按下键
//				break;
//	
//			case KEY_LEFT:		//左键
//			case L_KEY_LEFT:	//长按左键
//				break;

//			case KEY_RIGHT:		//右键
//			case L_KEY_RIGHT:	//长按右键
//				break;

//			case KEY_ENTER:		//确认键
//			case L_KEY_ENTER:	//长按确认键
//				break;
//	
//			case KEY_START:		//启动键
//			case L_KEY_START:	//长按启动键
//				break;
//	
//			case KEY_RESET:		//复位键
//			case L_KEY_RESET:	//长按复位键
//				i=POWERON_DISP_TIME;//显示延时结束
//				break;
//			
//			default:
//				break;
//		}
		
		//Delay_1ms(50);
		//Range_Control(3);
	}
}
void Send_T0_USB(void)
{
    const uint8_t DISP_UnitMain[]=
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,3,3,2,2,2};
	const uint8_t DISP_UnitSecond[]=
	{ 6,6,3,2,6,6,2,6,2,2,6,6,6,2,2,5,4,5,4,2,6,6};
    u8 i;
       // if(Button_Page.index==0)
    
//    if(usb_oenflag==1)
//        {
//            //Write_Usbdata ( UserBuffer,27);
//            Disp_Usbflag(1);
//            
//        }
//        else
//            Disp_Usbflag(2);
    UserBuffer[0]=0XAA;
    for(i=0;i<6;i++)
		{
			*(UserBuffer+i)=Test_Dispvalue.Main_valuebuff[i];
			*(UserBuffer+9+i)=Test_Dispvalue.Secondvaluebuff[i];
            ComBuf3.send.buf[i+1]=Test_Dispvalue.Main_valuebuff[i];
            ComBuf3.send.buf[i+9]=Test_Dispvalue.Secondvaluebuff[i];
            
		
		}
		*(UserBuffer+8)='	';
		*(UserBuffer+6)=(uint8_t)Disp_Unit1[Test_Dispvalue.Unit[0]];
		*(UserBuffer+7)=Disp_Range_Main_Disp[DISP_UnitMain[SaveData.Main_Func.Param.test]][0];
        ComBuf3.send.buf[7]=(uint8_t)Disp_Unit1[Test_Dispvalue.Unit[0]];
        ComBuf3.send.buf[8]=Disp_Range_Main_Disp[DISP_UnitMain[SaveData.Main_Func.Param.test]][0];;
        
		*(UserBuffer+8)=Disp_Range_Main_Disp[DISP_UnitMain[SaveData.Main_Func.Param.test]][1];
		
		*(UserBuffer+8+8)=(uint8_t)Disp_Unit1[Test_Dispvalue.Unit[1]];
        ComBuf3.send.buf[7+9]=Test_Dispvalue.Unit[1];
		
		*(UserBuffer+8+9)=Disp_Range_Main_Disp[DISP_UnitSecond[SaveData.Main_Func.Param.test]][0];
		*(UserBuffer+8+10)=Disp_Range_Main_Disp[DISP_UnitSecond[SaveData.Main_Func.Param.test]][1];
        ComBuf3.send.buf[7+10]=DISP_UnitSecond[SaveData.Main_Func.Param.test];
//		*(UserBuffer+8+8)=DISP_UnitMain[SaveData.Main_Func.Param.test];
		
        if(SaveData.Limit_Tab.Comp)
        {
            if(Comp_flag)
            {
                    *(UserBuffer+19)='F';
                    ComBuf3.send.buf[7+11]='F';
                
            }
                else
                    
                {
                    *(UserBuffer+19)='P';
                    ComBuf3.send.buf[7+11]='P';
                    
                }
        }
        else
           *(UserBuffer+19)='	'; 
		*(UserBuffer+20)='	';
		
		*(UserBuffer+21)='\r';
		*(UserBuffer+22)='\n';
        ComBuf3.send.buf[7+12]=0XBF;
}	
//测试程序
void Test_Process(void)
{
	uint32_t  numBlks, blkSize;
	uint8_t  inquiryResult[INQUIRY_LENGTH],rc;
	Button_Page_Typedef Button_Page;
	Main_Second_TypeDed Main_Second;//主参数和幅参赛的序号
	uint32_t keynum=0;
	uint8_t key,lock_flag=0;
    uint8_t page=1,i;
	uint8_t Disp_Flag=1;
    uint8_t timebuff[10];
	static uint8_t TrigFlag=0;
	
	Send_Ord_Typedef Uart;
	const uint8_t DISP_UnitMain[]=
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,3,3,2,2,2};
	const uint8_t DISP_UnitSecond[]=
	{ 6,6,3,2,6,6,2,6,2,2,6,6,6,2,2,5,4,5,4,2,6,6};
    UART_TxCmd(LPC_UART0, ENABLE);
	Button_Page.page=0;
	Button_Page.index=0;
	Button_Page.third=0xff;
	Disp_Test_Item();
	Main_Second.Main_flag=0;
	Main_Second.Second_falg=0;
        
	Delay_Key();
//    Uart.name=Disp_Main_Ord[SaveData.Main_Func.Param.test][0];
//    Uart.Ordel=0x60;
//    Send_Freq(&Uart);
//        Delay(300);
//    Uart.name=Disp_Main_Ord[SaveData.Main_Func.Param.test][1];
//    Uart.Ordel=0x70;
//    Send_Freq(&Uart);
//        Delay(300);
//    Uart.name=Disp_Main_Ord[SaveData.Main_Func.Param.test][2];
//    Uart.Ordel=0x71;
//    Send_Freq(&Uart);
//        Delay(300);
// //       Delay(1000);
// //       Uart_Process();
//    Uart.name=SaveData.Main_Func.Freq;
//    Uart.Ordel=Uart_Ordel[3];
//    Send_Freq(&Uart);
//    Delay(300);
//    Uart.name=SaveData.Main_Func.Level;
//    Uart.Ordel=Uart_Ordel[4];
//    Send_Freq(&Uart);
//    Delay(300);
//    Uart.name=SaveData.Main_Func.Speed;
//    Uart.Ordel=0xa0;
//    Send_Freq(&Uart);
//    Delay(300);
//    Uart.name=SaveData.Main_Func.Range.Range;
//    Uart.Ordel=0xb0;
//    Send_Freq(&Uart);
//    Delay(300);
//    Uart.name=SaveData.Main_Func.Avg;
//    Uart.Ordel=0xc0;
//    Send_Freq(&Uart);
//    Delay(300);
//    Uart.name=SaveData.Main_Func.Rsou;
//    Uart.Ordel=0xe0;
//    Send_Freq(&Uart);
//    Delay(300);
//    
        
	Send_UartStart();//开始时的串口发送数据
    GPIO_ClearInt(0, 1<<19);
    NVIC_EnableIRQ(GPIO_IRQn);
	while(GetSystemStatus()==SYS_STATUS_TEST)
	{
		Colour.Fword=LCD_COLOR_WHITE;
        Colour.black=LCD_COLOR_TEST_BACK;
        if(Rtc_intflag)
        {
            Rtc_intflag=0;
            Colour.Fword=LCD_COLOR_WHITE;
            Colour.black=LCD_COLOR_TEST_BACK;
            //sprintf((char *)timebuff,"%2d:%2d:%2d",RTC_TIME_DISP.HOUR,RTC_TIME_DISP.MIN,RTC_TIME_DISP.SEC);
            //Hex_Format(RTC_TIME_DISP.HOUR, 0, 2, 1);
            timebuff[0]=RTC_TIME_DISP.HOUR/10+'0';
            timebuff[1]=RTC_TIME_DISP.HOUR%10+'0';
            timebuff[2]=':';
            timebuff[3]=RTC_TIME_DISP.MIN/10+'0';
            timebuff[4]=RTC_TIME_DISP.MIN%10+'0';
            timebuff[5]=':';
            timebuff[6]=RTC_TIME_DISP.SEC/10+'0';
            timebuff[7]=RTC_TIME_DISP.SEC%10+'0';
            timebuff[8]=0;
            
            WriteString_16(LIST1+360, 2, timebuff,  0);
        }
//        sprintf((char *)DispBuf,"%2d-%2d-%2d",RTC_TIME_DISP.HOUR,RTC_TIME_DISP.MIN,RTC_TIME_DISP.SEC);
//        WriteString_16(LIST1+360, 2, DispBuf,  0);
		if(Disp_Flag==1)
		{
			Disp_Test_value(&Button_Page);
			
			Disp_Flag=0;
		
		}//
		//_printf("CoreClock: %s\n",READDATA); 
		
		Uart_Process();
//		Send_Uart3((uint8_t *)READDATA);
		Colour.black=LCD_COLOR_TEST_MID;
//	sprintf((char *)DispBuf,"%2d",SaveData.Main_Func.Param.test);
//	WriteString_16(210, 4, DispBuf,  0);
		switch(SaveData.Main_Func.Param.test)
		{
			case 0:
				Disp_Cp_D();
				
//			WriteString_Big((uint8_t)Set_Unit[Test_Dispvalue.Mainvalue.Unit]);
				//Disp_Big_MainUnit(Test_Dispvalue.Unit[0],0);
				break;
			case 1:
				Disp_Cp_Q();
				//Test_Dispvalue.Mainvalue.Unit=1;
				//Disp_Big_MainUnit(Test_Dispvalue.Unit[0],0);
				break;
			case 2:
				Disp_Cp_G();
				//Test_Dispvalue.Mainvalue.Unit=1;
				//Disp_Big_MainUnit(Test_Dispvalue.Unit[0],0);
			
				//Test_Dispvalue.Secondvalue.Dot=3;
				//Disp_Big_SecondUnit(Test_Dispvalue.Unit[1],3);
				break;
			case 3:
				Disp_Cp_Rp();
				//Test_Dispvalue.Mainvalue.Unit=1;
				//Disp_Big_MainUnit(Test_Dispvalue.Unit[0],0);
			
				
				break;
			case 4:
				//
				Disp_Cs_D();
				//Disp_Big_MainUnit(Test_Dispvalue.Unit[0],0);
			
				break;
			case 5:
				Disp_Cs_Q();
				break;
			case 6:
				Disp_Cs_Rs();
				break;
			case 7:
				Disp_Lp_Q();
				break;
			case 8:
				Disp_Lp_Rp();
				break;
			case 9:
				Disp_Lp_Rd();
				break;
			case 10:
				Disp_Lp_D();
				break;
			case 11:
//				Disp_Lp_G();
			Disp_Ls_D();
				break;
			case 12:
				Disp_Ls_Q();
				break;
			case 13:
				Disp_Ls_Rs();
				break;
			case 14:
				Disp_Ls_Rd();
				break;
			case 15:
				Disp_Z_d();
				break;
			case 16:
				Disp_Z_r();
				break;
			case 17:
				Disp_Y_d();
				break;
			case 18:
				Disp_Y_r();
				break;
			case 19:
				Disp_R_X();
				break;
			case 20:
				Disp_Rp_Q();
				break;
			case 21:
				
			Disp_Rs_Q();
				break;
			case 22:
				Disp_G_B();
				
				break;
			case 23:
				break;
			case 24:
				break;
			case 25:
				break;
			case 26:
				break;
			case 27:
				break;
			case 28:
				break;
			case 29:
				break;
			case 30:
				break;
			case 31:
				break;
			default:
				break;
		
		
		}
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
				//	Send_Request();
					break;
				case 1:
//					if(uart_count++>5)
//						Uart_Send_Flag=0;
					Send_Main_Ord();
//						SHORT_DELAY;
//						while(i)	
//						{
//								i--;
//							if(ComBuf.rec.buf[5]==0x00)
//								i=0;
//							else 
//									Send_Main_Ord();
//						SHORT_DELAY;
//						}				
				
					Uart_Send_Flag=0;
					break;
				case 2:
				Send_Freq(&Uart);
//				SHORT_DELAY;
//						i=3;
//						while(i)	
//						{
//								i--;
//							if(ComBuf.rec.buf[5]==0x00)
//								i=0;
//							else 
//									Send_Freq(&Uart);
//							SHORT_DELAY;
//						
//						}
					
				Uart_Send_Flag=0;
					break;
				default:
					//Send_Request();
					break;
			
			}
			
			
			timer0_counter=0;
		
		
		}
//		for(i=0;i<6;i++)
//		{
//			*(UserBuffer+i)=Test_Dispvalue.Main_valuebuff[i];
//			*(UserBuffer+9+i)=Test_Dispvalue.Secondvaluebuff[i];
//		
//		}
//		*(UserBuffer+8)='	';
//		*(UserBuffer+6)=(uint8_t)Disp_Unit1[Test_Dispvalue.Unit[0]];
//		*(UserBuffer+7)=Disp_Range_Main_Disp[DISP_UnitMain[SaveData.Main_Func.Param.test]][0];
//		*(UserBuffer+8)=Disp_Range_Main_Disp[DISP_UnitMain[SaveData.Main_Func.Param.test]][1];
//		
//		*(UserBuffer+8+8)=(uint8_t)Disp_Unit1[Test_Dispvalue.Unit[1]];
//		
//		*(UserBuffer+8+9)=Disp_Range_Main_Disp[DISP_UnitSecond[SaveData.Main_Func.Param.test]][0];
//		*(UserBuffer+8+10)=Disp_Range_Main_Disp[DISP_UnitSecond[SaveData.Main_Func.Param.test]][1];
////		*(UserBuffer+8+8)=DISP_UnitMain[SaveData.Main_Func.Param.test];
//		*(UserBuffer+19)='	';
//		*(UserBuffer+20)='	';
//		
//		*(UserBuffer+21)='\r';
//		*(UserBuffer+22)='\n';
		//Test_Dispvalue.Mainvalue.Unit=1;timer0_counter
		if(TrigFlag==0)
		{
			if(SaveData.Main_Func.Trig!=0)
				TrigFlag=1;
		}	
		if(TrigFlag==1||SaveData.Main_Func.Trig==0||TrigFlag==0)
		{
			Disp_Big_MainUnit(Test_Dispvalue.Unit[0],DISP_UnitMain[SaveData.Main_Func.Param.test]);//显示单位
//			Test_Dispvalue.Secondvalue.Dot=3;
			Disp_Big_SecondUnit(Test_Dispvalue.Unit[1],DISP_UnitSecond[SaveData.Main_Func.Param.test]);//副参数单位
			Disp_Testvalue();			//显示测量值
			
		
            //分选比较打开
            if(SaveData.Limit_Tab.Comp)
            {
                Test_Comp(&Comp_Change);
                Test_Comp_Fmq();
    //			if(Comp_flag)
    //				*(UserBuffer+19)='F';
    //			else
    //				*(UserBuffer+19)='P';
                
             Send_T0_USB();//往U盘里面写数据  
             if(SaveData.Main_Func.buad)
                 Send_Uart3((uint8_t *) ComBuf3.send.buf);
                 
            if(Saveeeprom.Sys_set.U_flag)
                Write_Usbdata ( UserBuffer,23);                
                
            }
            else
            {
                All_LedOff1();
            
            
            }
            if(TrigFlag==1)
                TrigFlag=2;
        
        }
        
		//		_printf("CoreClock: %s\n","12345678"); 
//		Disp_NumKeyboard_Set();
//		Disp_Cp();
		
//		if(Save_Res.Sys_Setvalue.U_store)
		{
			if(Button_Page.index==0)
			{
				if(usb_oenflag==1)
					{
						//Write_Usbdata ( UserBuffer,27);
						Disp_Usbflag(1);
						
					}
					else
						Disp_Usbflag(2);
			}
//			if(usb_oenflag==1)
				{
                    
                    
                    //Delay(300);
					
					//Disp_Usbflag(1);
					
				}
		
		}
        Uart3_Process();
//		else
//			Disp_Usbflag(0);
		
	  key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	
			Disp_Flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
                    switch(Button_Page.index)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_SETUPTEST);
							break;
						case 1:
							switch(Button_Page.page)
							{
								case 0:
									Button_Page.force=0;
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
								(uint8_t *)Cp_Button_Tip,&Button_Page);
								Uart_Send_Flag=1;
								//Send_Main_Ord();
								//Button_Page.page=0;
								//	SaveData.Main_Func.Param.page=0;
									//SaveData.Main_Func.Param.test=0;
									break;
								case 1:
									Button_Page.force=4;
									
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Z_Button_Tip,&Button_Page);
									Uart_Send_Flag=1;
									//Send_Main_Ord();
									//Button_Page.page=1;
									
									break;
								case 2:
									SaveData.Main_Func.Param.page=8;
									//SaveData.Main_Func.Param.test=8;
									break;
								default:
									break;
							
							
							}
							
							break;
						case 2:
							if(SaveData.Main_Func.Freq>14)
								SaveData.Main_Func.Freq-=10;
							else
								SaveData.Main_Func.Freq=4;
							Uart.Ordel=Uart_Ordel[3];
							Uart.name=SaveData.Main_Func.Freq;
							Uart_Send_Flag=2;
					
							break;
						case 3:
							if(SaveData.Main_Func.Level>0)
								SaveData.Main_Func.Level--;
							Uart.Ordel=Uart_Ordel[4];
							Uart.name=SaveData.Main_Func.Level;
							//Send_Freq(&Uart);
							Uart_Send_Flag=2;
							//SaveData.Main_Func.Level=0;
							break;
						
						case 4:
							SaveData.Main_Func.Range.Range=0;
							Uart.Ordel=Uart_Ordel[6];
							Uart.name=SaveData.Main_Func.Range.Range;
							Uart_Send_Flag=2;
							break;
						case 5:
							SaveData.Main_Func.Speed=0;
							Uart.Ordel=Uart_Ordel[5];
							Uart.name=SaveData.Main_Func.Speed;
							Uart_Send_Flag=2;
							break;
//						case 6:
//							
//							if(SaveData.Main_Func.Bias>0)
//								SaveData.Main_Func.Bias--;
//							break;
						
						default:
							break;
					
					
					}
					Savetoeeprom();
				break;
				case Key_F2:
					
					switch(Button_Page.index)
					{
						case 0:
							if(page==1)
//							   SetSystemStatus(SYS_STATUS_SYSSET);
								SetSystemStatus(SYS_STATUS_RANGE);
							break;
						case 1:
							switch(Button_Page.page)
							{
								case 0:
									Button_Page.force=1;
									
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Cs_Button_Tip,&Button_Page);
									//	SaveData.Main_Func.Param.page=0;
									
									Uart_Send_Flag=1;
								break;
								case 1:
									
									Button_Page.force=5;
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Y_Button_Tip,&Button_Page);
									Uart_Send_Flag=1;
								//Button_Page.page=0;
									break;
								case 2:
//									SaveData.Main_Func.Param.page=2;
//									SaveData.Main_Func.Param.test=8;
									break;
								default:
									break;
							
							
							}
							
							break;
						case 2:
							if(SaveData.Main_Func.Freq>4)
								SaveData.Main_Func.Freq--;
							Uart.Ordel=Uart_Ordel[3];
							Uart.name=SaveData.Main_Func.Freq;
							Uart_Send_Flag=2;								
							break;
						case 3:
							if(SaveData.Main_Func.Level<2)
								SaveData.Main_Func.Level++;
							Uart.Ordel=Uart_Ordel[4];
							Uart.name=SaveData.Main_Func.Level;
							Uart_Send_Flag=2;
								
							break;
						
						case 4:
							SaveData.Main_Func.Range.Range=6;	//保持
							//SaveData.Main_Func.Range.Auto=0;
							Uart.Ordel=Uart_Ordel[6];
							Uart.name=SaveData.Main_Func.Range.Range;
							Uart_Send_Flag=2;
							break;
						case 5:
							SaveData.Main_Func.Speed=1;
							Uart.Ordel=Uart_Ordel[5];
							Uart.name=SaveData.Main_Func.Speed;
							Uart_Send_Flag=2;
							break;
//						case 6:
							
						
//							if(SaveData.Main_Func.Bias<5000)
//								SaveData.Main_Func.Bias++;
//							break;
						
						default:
							break;
					
					
					}
					Savetoeeprom();
				break;
				case Key_F3:
					
					switch(Button_Page.index)
					{
						case 0:
							if(page==1)
								SetSystemStatus(SYS_STATUS_RANGECOUNT);
							break;
						case 1:
							switch(Button_Page.page)
							{
								case 0:
									Button_Page.force=2;									
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Lp_Button_Tip1,&Button_Page);
								    Uart_Send_Flag=1;
								//	Send_Main_Ord();
								//Button_Page.page=0;
									break;
								case 1:
									Button_Page.force=6;
									
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)R_Button_Tip,&Button_Page);
									Uart_Send_Flag=1;
									break;
								case 2:
//									SaveData.Main_Func.Param.page=2;
//									SaveData.Main_Func.Param.test=8;
									break;
								default:
									break;
							
							
							}
							
							break;
						case 2:
							if(SaveData.Main_Func.Freq<NUM_FREQ)
								SaveData.Main_Func.Freq++;
							Uart.Ordel=Uart_Ordel[3];
							Uart.name=SaveData.Main_Func.Freq;
							Uart_Send_Flag=2;
				
							break;
						case 3:
							break;
						
						case 4://MAX_R_RANGE
							//SaveData.Main_Func.Range.Auto=0;
							if(SaveData.Main_Func.Range.Range>0)
								SaveData.Main_Func.Range.Range--;
							Uart.Ordel=Uart_Ordel[6];
							Uart.name=SaveData.Main_Func.Range.Range;
							Uart_Send_Flag=2;
							break;
						case 5:
							SaveData.Main_Func.Speed=2;
							Uart.Ordel=Uart_Ordel[5];
							Uart.name=SaveData.Main_Func.Speed;
							Uart_Send_Flag=2;
							break;
//						case 6:
//							
//							break;
						
						default:
							break;
					
					
					}
					Savetoeeprom();
				break;
				case Key_F4:
					
					switch(Button_Page.index)
					{
						case 0:
//							if(page==1)
//								SetSystemStatus(SYS_STATUS_ITEM);
							break;
						case 1:
							switch(Button_Page.page)
							{
								case 0:
									Button_Page.force=3;
									
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Ls_Button_Tip,&Button_Page);
									Uart_Send_Flag=1;
								//Button_Page.page=0;
									break;
								case 1:
									SaveData.Main_Func.Param.test=22;
									Uart_Send_Flag=1;
//									SaveData.Main_Func.Param.page=1;
//									SaveData.Main_Func.Param.test=7;
									break;
								case 2:
//									SaveData.Main_Func.Param.page=2;
//									SaveData.Main_Func.Param.test=8;
									break;
								default:
									break;
							
							
							}
							
							break;
						case 2:
							if(SaveData.Main_Func.Freq<NUM_FREQ-10)
								SaveData.Main_Func.Freq+=10;
							else
								SaveData.Main_Func.Freq=NUM_FREQ;
							Uart.Ordel=Uart_Ordel[3];
							Uart.name=SaveData.Main_Func.Freq;
							Uart_Send_Flag=2;

							break;
						case 3:
							break;

					
						case 4:
							//SaveData.Main_Func.Range.Auto=0;
							if(SaveData.Main_Func.Range.Range<MAX_R_RANGE)
								SaveData.Main_Func.Range.Range++;
							Uart.Ordel=Uart_Ordel[6];
							Uart.name=SaveData.Main_Func.Range.Range;
							Uart_Send_Flag=2;
							break;
						case 5:
							break;
//						case 6:
//							break;
						default:
							break;
					
					
					}	
					Savetoeeprom();
				break;
				case Key_F5:
					
					if(Button_Page.index==0)
					{
						if(usb_oenflag==0)	
//				if(Save_Res.Sys_Setvalue.U_store)
				{
//					Host_Init();               /* Initialize the lpc17xx host controller                                    */
					Host_Init();
					//rc = Host_EnumDev();       /* Enumerate the device connected                                            */
					
					rc = Host_EnumDev();
//					i=10;
//					while(rc!=0)
//					{
//						rc=Host_EnumDev();
//						i--;
//						if(i==0)
//							break;
//					}
					if (rc == OK) {
					/* Initialize the mass storage and scsi interfaces */
						rc = MS_Init( &blkSize, &numBlks, inquiryResult );
					if (rc == OK) {
						rc = FAT_Init();   /* Initialize the FAT16 file system */    
						
						if (rc == OK) {
							
							//Write_Usbdata ( "主参数		副参数		分选\r\n" ,22);	
							Write_Usbdata ( "Main P		Seco P		Part\r\n" ,22);	

						}							
			//            if (rc == OK) {
			//                Main_Copy();   /* Call the application                                                      */
			//            } 
						} 
					} 
					
				}
					}
					if(Button_Page.index==1)
					{
						if(Button_Page.page==0)
							Button_Page.page=1;
						else
							Button_Page.page=0;
					
					
					}
//					if(Button_Page.third!=0xff)
//					{
//						Button_Page.third=0xff;
//					
//					}else
//					{
//                    if(Button_Page.page==0)
//						Button_Page.page=1;
//					else
//						Button_Page.page=0;
//					}
                    //Disp_Button_value1(Button_Page.page);
				break;
				case Key_Disp:
					
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
					
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
					
					if(Button_Page.index==0)
						Button_Page.index=6;
					else
					if(Button_Page.index>3)
						Button_Page.index-=3;
					else
						Button_Page.index+=2;
					Button_Page.page=0;
				break;
				case Key_RIGHT:
					
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
					if(Button_Page.index<=3)
						Button_Page.index+=3;
					else
						Button_Page.index-=2;
					Button_Page.page=0;
						
				break;
				case Key_DOWN:
					
					if(Button_Page.index>4)
						Button_Page.index=0;
					else
						Button_Page.index++;
					Button_Page.page=0;
					
				break;
				case Key_UP:
					
					if(Button_Page.index<1)
						Button_Page.index=5;
					else
						Button_Page.index--;
					Button_Page.page=0;
				break;
				
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				//break;
				case Key_DOT:
					
					if(Button_Page.index==2)
					{ 	Disp_Coordinates_Typedef Coordinates;
						Coordinates.xpos=LIST1+88;
						Coordinates.ypos=FIRSTLINE+SPACE1*1;
						Coordinates.lenth=86;
						SaveData.Main_Func.Freq=Freq_Set_Num(&Coordinates);
						Uart.Ordel=Uart_Ordel[3];
							Uart.name=SaveData.Main_Func.Freq;
							Uart_Send_Flag=2;
					
					}
				break;
				case Key_BACK:
					
				break;
				case Key_LOCK:
					if(lock_flag)
						lock_flag=0;
					else
						lock_flag=1;
					if(lock_flag)
					{
						Lock_Control_On();
						Lock_LedOn();
						
					}
					else
					{
						Lock_LedOff();
						Lock_Control_Off();
					
					
					}
					
				break;
				case Key_BIAS:
					
				break;
				case Key_REST:
					
				break;
				case Key_TRIG:
					if(SaveData.Main_Func.Trig!=0)
							TrigFlag=1;	
				break;
				default:
					
				break;
					
			}
		
		
		}
	
	
	}
//    UART_TxCmd(LPC_UART0, DISABLE);

}
//文件管理程序
void File_Process(void)
{
	 uint32_t keynum=0;
	 uint8_t key;
	Delay_Key();
  	while(GetSystemStatus()==SYS_STATUS_FILE)
	{
	 key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{
			switch(key)
			{
				case Key_F1:
				break;
				case Key_F2:
				break;
				case Key_F3:
				break;
				case Key_F4:
				break;
				case Key_F5:
				break;
				case Key_Disp:
				break;
				case Key_SETUP:
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	
	}

}
//参数设置程序  测量设置
void Setup_Process(void)
{

	Button_Page_Typedef Button_Page;
	Disp_Coordinates_Typedef  Coordinates;
	Send_Ord_Typedef Uart;
	
	uint32_t keynum=0;
	uint8_t key;
//    uint8_t page=1;
	uint8_t Disp_Flag=1;
//	uint8_t index=0;
//	uint32_t *pt;
//	pt=(uint32_t *)&SaveData.Main_Func;
	Button_Page.index=0;
	Button_Page.page=0;
    Disp_Test_Set_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_SETUPTEST)
	{
	    
		if(Disp_Flag==1)
		{
			DispSet_value(&Button_Page);
			Disp_Flag=0;
		
		}
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
//					Send_Request();
					break;
				case 1:
					Send_Main_Ord();
					break;
				case 2:
					Send_Freq(&Uart);
					break;
				default:
//					Send_Request();
					break;
			
			}
			Uart_Send_Flag=0;
			
			timer0_counter=0;
		
		
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==7)
		{	Disp_Flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
					switch(Button_Page.index)
					{
						case 0:
							if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_TEST);//
							else
								SetSystemStatus(SYS_STATUS_FILE);
								
							break;
						case 1:
							switch(Button_Page.page)
							{
								case 0:
									Button_Page.force=0;
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
								(uint8_t *)Cp_Button_Tip,&Button_Page);
									
									//Send_Main_Ord();
								Uart_Send_Flag=1;
									break;
								case 1:
									Button_Page.force=4;

									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Z_Button_Tip,&Button_Page);
									Uart_Send_Flag=1;
									//Button_Page.page=1;
									
									break;
								case 2:
									SaveData.Main_Func.Param.page=8;
									//SaveData.Main_Func.Param.test=8;
									break;
								default:
									break;
							
							
							}
							
							break;
						case 2:
							if(SaveData.Main_Func.Freq>14)
								SaveData.Main_Func.Freq-=10;
							else
								SaveData.Main_Func.Freq=4;
							Uart.Ordel=Uart_Ordel[3];
							Uart.name=SaveData.Main_Func.Freq;
							
							Uart_Send_Flag=2;
								
							
							break;
						case 3:
							if(SaveData.Main_Func.Level>0)
								SaveData.Main_Func.Level--;
							Uart.Ordel=Uart_Ordel[4];
							Uart.name=SaveData.Main_Func.Level;
							Uart_Send_Flag=2;
							//SaveData.Main_Func.Level=0;
							break;
						case 4:
							SaveData.Main_Func.Trig=0;
							break;
//						case 5:
//							SaveData.Main_Func.Alc=0;
//							break;
						case 5:
							
							SaveData.Main_Func.Rsou=0;
							Uart.Ordel=Uart_Ordel[8];
							Uart.name=SaveData.Main_Func.Rsou;
							Uart_Send_Flag=2;
							break;
//						case 6:
//							if(SaveData.Main_Func.Trig_time.Num>0)
//								SaveData.Main_Func.Trig_time.Num--;
//							break;
//						case 8:
//							if(SaveData.Main_Func.Temp_time.Num>0)
//								SaveData.Main_Func.Temp_time.Num--;
//							break;
						case 6:
							SaveData.Main_Func.beep=0;
							break;
						case 7:
							SaveData.Main_Func.Range.Range=0;						
							Uart.Ordel=Uart_Ordel[6];
							Uart.name=SaveData.Main_Func.Range.Range;
							Uart_Send_Flag=2;
							break;
						case 8:
//							if(SaveData.Main_Func.Bias>0)
//								SaveData.Main_Func.Bias--;
							SaveData.Main_Func.Speed=0;
							Uart.Ordel=Uart_Ordel[5];
							Uart.name=SaveData.Main_Func.Speed;
							Uart_Send_Flag=2;
							break;
//						case 12:
//							
//							break;
//						case 10:
//							if(SaveData.Main_Func.Avg>1)
//								SaveData.Main_Func.Avg--;
////							Uart.Ordel=Uart_Ordel[7];
////							Uart.name=SaveData.Main_Func.Avg;
////							Uart_Send_Flag=2;
//							break;
						case 9:
							SaveData.Main_Func.V_i=0;
							break;
//						case 15:
//							SaveData.Main_Func.Dcr=0;
//							break;
//						case 16:
//							SaveData.Main_Func.DC_Range.Auto=1;
//							Uart.Ordel=Uart_Ordel[6];
//							Uart.name=SaveData.Main_Func.Range.Auto;
//							Uart_Send_Flag=2;
//							break;
//						case 17:
//							if(SaveData.Main_Func.DC_Level>0)
//								SaveData.Main_Func.DC_Level--;
//							break;
						case 10://这里进行数据测量
							if(SaveData.Main_Func.buad>0)
								SaveData.Main_Func.buad--;
							break;
//						case 19:
//							break;
						default:
							break;
					
					
					}

				break;
				case Key_F2:
					

					switch(Button_Page.index)
					{
						case 0:
							if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_USERDEBUG);
							else
								SetSystemStatus(SYS_STATUS_SYSSET);
								
							break;
						case 1:
							switch(Button_Page.page)
							{
								case 0:
									Button_Page.force=1;

									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Cs_Button_Tip,&Button_Page);
									Uart_Send_Flag=1;
									//	SaveData.Main_Func.Param.page=0;
								
								//Button_Page.page=0;
									break;
								case 1:

									Button_Page.force=5;
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Y_Button_Tip,&Button_Page);
									Uart_Send_Flag=1;
								//Button_Page.page=0;
									break;
								case 2:
//									SaveData.Main_Func.Param.page=2;
//									SaveData.Main_Func.Param.test=8;
									break;
								default:
									break;
							
							
							}
							
							break;
						case 2:
							if(SaveData.Main_Func.Freq>4)
								SaveData.Main_Func.Freq--;
							Uart.Ordel=Uart_Ordel[3];
							Uart.name=SaveData.Main_Func.Freq;
							Uart_Send_Flag=2;
							
							
							break;
						case 3:
							if(SaveData.Main_Func.Level<2)
							SaveData.Main_Func.Level++;
							Uart.Ordel=Uart_Ordel[4];
							Uart.name=SaveData.Main_Func.Level;
							Uart_Send_Flag=2;
							break;
						case 4:
							SaveData.Main_Func.Trig=1;
							break;
//						case 5:
//							SaveData.Main_Func.Alc=1;
//							break;
						case 5:
							SaveData.Main_Func.Rsou=1;
							Uart.Ordel=Uart_Ordel[8];
							Uart.name=SaveData.Main_Func.Rsou;
							Uart_Send_Flag=2;
							break;
//						case 6:
//							if(SaveData.Main_Func.Trig_time.Num<60000)
//								SaveData.Main_Func.Trig_time.Num++;
//							break;
//						case 8:
//							if(SaveData.Main_Func.Temp_time.Num<60000)
//								SaveData.Main_Func.Temp_time.Num++;
//							break;
						case 6:
							SaveData.Main_Func.beep=1;
							break;
						case 7:
							SaveData.Main_Func.Range.Range=6;
							Uart.Ordel=Uart_Ordel[6];
							Uart.name=SaveData.Main_Func.Range.Range;
							Uart_Send_Flag=2;
							break;
						case 8:
							SaveData.Main_Func.Speed=1;
							Uart.Ordel=Uart_Ordel[5];
							Uart.name=SaveData.Main_Func.Speed;
							Uart_Send_Flag=2;
							break;
//						case 12:
////							if(SaveData.Main_Func.Bias<5000)
////								SaveData.Main_Func.Bias++;
//							
//							break;
//						case 9:
//							if(SaveData.Main_Func.Avg<25)
//								SaveData.Main_Func.Avg++;
////							Uart.Ordel=Uart_Ordel[7];
////							Uart.name=SaveData.Main_Func.Avg;
////							Uart_Send_Flag=2;
//							break;
						case 9:
							SaveData.Main_Func.V_i=1;
							break;
//						case 15:
//							SaveData.Main_Func.Dcr=1;
//							break;
//						case 16:
//							SaveData.Main_Func.DC_Range.Auto=0;
//							break;
//						case 17:
//							if(SaveData.Main_Func.DC_Level<5000)
//								SaveData.Main_Func.DC_Level++;
//							break;
						case 10:
							if(SaveData.Main_Func.buad<6)
								SaveData.Main_Func.buad++;
							break;
//						case 19:
//							break;
						default:
							break;
					
					
					}				
				

				break;
				case Key_F3:
					
					switch(Button_Page.index)
					{
						case 0:
							if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_LIMITSET);
							else
								SetSystemStatus(SYS_STATUS_TOOL);
							break;
						case 1:
							switch(Button_Page.page)
							{
								case 0:
									Button_Page.force=2;
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Lp_Button_Tip1,&Button_Page);
									Uart_Send_Flag=1;
								//Button_Page.page=0;
									break;
								case 1:
									Button_Page.force=6;
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)R_Button_Tip,&Button_Page);
									Uart_Send_Flag=1;
								//Button_Page.page=0;
									break;
								case 2:
//									SaveData.Main_Func.Param.page=2;
//									SaveData.Main_Func.Param.test=8;
									break;
								default:
									break;
							
							
							}
							
							break;
						case 2:
							if(SaveData.Main_Func.Freq<NUM_FREQ)
								SaveData.Main_Func.Freq++;
							Uart.Ordel=Uart_Ordel[3];
							Uart.name=SaveData.Main_Func.Freq;
							Uart_Send_Flag=2;
							break;
						case 3:
							break;
						case 4:
							SaveData.Main_Func.Trig=2;
							break;
//						case 5:
//							break;
						case 5:
							break;
//						case 6:
//							break;
//						case 8:
//							break;
						case 6:
							SaveData.Main_Func.beep=2;
							break;
						case 7://MAX_R_RANGE
							//SaveData.Main_Func.Range.Auto=0;
							if(SaveData.Main_Func.Range.Range>0)
								SaveData.Main_Func.Range.Range--;
							Uart.Ordel=Uart_Ordel[6];
							Uart.name=SaveData.Main_Func.Range.Range;
							Uart_Send_Flag=2;
							break;
						case 8:
							SaveData.Main_Func.Speed=2;
							break;
//						case 10:							
//							break;
						case 9:							
							break;
						case 10:
							break;
//						case 14:
//							break;
//						case 15:
//							break;
//						case 16:
//							if(SaveData.Main_Func.DC_Range.Range>0)
//								SaveData.Main_Func.DC_Range.Range--;
//							break;
//						case 17:
//							break;
//						case 18:
//							break;
//						case 19:
//							break;
						default:
							break;
					
					
					}	
					
				break;
				case Key_F4:
					
					switch(Button_Page.index)
					{
						case 0:
							if(Button_Page.page==0)				//SYS_STATUS_SYSSET
							    SetSystemStatus(SYS_STATUS_SYSSET);
//								if(Button_Page.page==0)
//								SetSystemStatus(SYS_STATUS_ITEMSET);
							break;
						case 1:
							switch(Button_Page.page)
							{
								case 0:
									Button_Page.force=3;
									Disp_Button_Fun_Set(LIST1+88, FIRSTLINE,
									(uint8_t *)Ls_Button_Tip,&Button_Page);
									Uart_Send_Flag=1;
								//Button_Page.page=0;
									break;
								case 1:
									SaveData.Main_Func.Param.test=22;
									Uart_Send_Flag=1;
//									SaveData.Main_Func.Param.page=1;
//									SaveData.Main_Func.Param.test=7;
									break;
								case 2:
//									SaveData.Main_Func.Param.page=2;
//									SaveData.Main_Func.Param.test=8;
									break;
								default:
									break;
							
							
							}
							
							break;
						case 2:
							if(SaveData.Main_Func.Freq<NUM_FREQ-10)
								SaveData.Main_Func.Freq+=10;
							else
								SaveData.Main_Func.Freq=NUM_FREQ;
							Uart.Ordel=Uart_Ordel[3];
							Uart.name=SaveData.Main_Func.Freq;
							Uart_Send_Flag=2;
							break;
						case 3:
							break;
						case 4:
							SaveData.Main_Func.Trig=3;
							break;
						case 5:
							break;
//						case 6:
//							break;
						case 6:
							break;
//						case 8:
//							break;
//						case 9:
//							break;
						case 7:
							//SaveData.Main_Func.Range.Auto=0;
							if(SaveData.Main_Func.Range.Range<MAX_R_RANGE)
								SaveData.Main_Func.Range.Range++;
							Uart.Ordel=Uart_Ordel[6];
							Uart.name=SaveData.Main_Func.Range.Range;
							Uart_Send_Flag=2;
							break;
						case 8:
							break;
//						case 10:
//							break;
						case 9:
							break;
						case 10:
							break;
//						case 15:
//							break;
//						case 16:
//							if(SaveData.Main_Func.DC_Range.Range<MAX_R_RANGE)
//								SaveData.Main_Func.DC_Range.Range++;
//							break;
//						case 17:
//							break;
//						case 18:
//							break;
//						case 19:
//							break;
						default:
							break;
					
					
					}	
				
				break;
				case Key_F5:
					
					switch(Button_Page.index)
					{
						case 0:
//							if(Button_Page.page==0)
//								Button_Page.page=1;
//							else
//								Button_Page.page=0;
////							page=2;
//							Disp_Button_TestSet(page);
						break;
						case 1:
							if(Button_Page.page<1)
								Button_Page.page++;
							else
								Button_Page.page=0;

						
						break;
					
					
					}
                    
					
				break;
				case Key_Disp:
						
                        SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
						
                        SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
					
					if(Button_Page.index==0)
						Button_Page.index=10;
					else
					if(Button_Page.index>5)
						Button_Page.index-=5;
					else
//						if(Button_Page.index==1)
//							Button_Page.index=0;
//						
//						else
							Button_Page.index+=4;
					Button_Page.page=0;
						
				break;
				case Key_RIGHT:
					
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
					if(Button_Page.index<=5)
						Button_Page.index+=5;
					else
						Button_Page.index-=4;
					Button_Page.page=0;
						
				break;
				case Key_DOWN:
					
					if(Button_Page.index>9)
						Button_Page.index=0;
					else
						Button_Page.index++;
					Button_Page.page=0;
					
				break;
				case Key_UP:
					
					if(Button_Page.index<1)
						Button_Page.index=10;
					else
						Button_Page.index--;
					Button_Page.page=0;
				break;
				case Key_NUM1:											
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				//break;
				case Key_DOT:
					
					if(Button_Page.index==2)
					{
						Coordinates.xpos=LIST1+88;
						Coordinates.ypos=FIRSTLINE+SPACE1*1;
						Coordinates.lenth=86;
						SaveData.Main_Func.Freq=Freq_Set_Num(&Coordinates);
					
					}
//					if(Button_Page.index==6)
//					{
//						//LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*6-2,SELECT_1END , FIRSTLINE+SPACE1*7-4 , Colour.black ) ;
//						Coordinates.xpos=LIST1+88;
//						Coordinates.ypos=FIRSTLINE+SPACE1*5;
//						Coordinates.lenth=86;
//						
//						SaveData.Main_Func.Trig_time=Disp_Set_Num(&Coordinates);
//					
//					
//					}
//					if(Button_Page.index==8)
//					{
//						//LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*6-2,SELECT_1END , FIRSTLINE+SPACE1*7-4 , Colour.black ) ;
//						Coordinates.xpos=LIST1+88;
//						Coordinates.ypos=FIRSTLINE+SPACE1*7;
//						Coordinates.lenth=86;
//						SaveData.Main_Func.Temp_time=Disp_Set_Num(&Coordinates);
//					
//					
//					}
//					if(Button_Page.index==10)
//					{
//						//LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*6-2,SELECT_1END , FIRSTLINE+SPACE1*7-4 , Colour.black ) ;
//						Coordinates.xpos=LIST2+88;
//						Coordinates.ypos=FIRSTLINE+SPACE1*3;
//						
//						SaveData.Main_Func.Avg=Avg_Set_Num(&Coordinates);
//						Uart.Ordel=Uart_Ordel[7];
//						Uart.name=SaveData.Main_Func.Avg;
//						Send_Freq(&Uart);
//					
//					
//					}
//					
//					if(Button_Page.index==12)//Sort_TypeDef Disp_Set_InputNum(Disp_Coordinates_Typedef *Coordinates)
//				
//					{
//						//LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*6-2,SELECT_1END , FIRSTLINE+SPACE1*7-4 , Colour.black ) ;
//						Coordinates.xpos=LIST2+88;
//						Coordinates.ypos=FIRSTLINE+SPACE1*5;
//						
//						SaveData.Main_Func.Ref_A=Disp_Set_InputNum(&Coordinates);
//					
//					
//					}
					
					break;
				case Key_BACK:
					
				break;
				case Key_LOCK:
					
				break;
				case Key_BIAS:
					
				break;
				case Key_REST:
					
				break;
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}
		
		
		}
	 	
	
	
	
	}
	Savetoeeprom();
}
//数据保存程序
void Data_StoreProcess(void)
{
	uint32_t keynum=0;
	uint8_t key;
//	Button_Page_Typedef Button_Page;
//	Button_Page.index=0;
//	Button_Page.page=0;
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_DATASTORE)
	{
	 key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{
			switch(key)
			{
				case Key_F1:
				break;
				case Key_F2:
				break;
				case Key_F3:
				break;
				case Key_F4:
				break;
				case Key_F5:
				break;
				case Key_Disp:
				break;
				case Key_SETUP:
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	 	
	
	}
}
//档号显示
void Range_Process(void)
{
	
	uint32_t keynum=0;
	uint8_t key,num,lock_flag=0;
	Send_Ord_Typedef Uart;
//    uint8_t page=1;
	uint8_t Disp_flag=1;
//	uint8_t index=0;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_Range_Item();
	Delay_Key();
//    Delay(300);
    UART_TxCmd(LPC_UART0, ENABLE);
    Uart_Send_Flag=0;
 	while(GetSystemStatus()==SYS_STATUS_RANGE)
	{
		if(Disp_flag==1)
		{
			Disp_RangeDispValue(&Button_Page);
			Disp_flag=0;	
		}
		Uart_Process();
		
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
//					Send_Request();
					break;
				case 1:
					Send_Main_Ord();
					break;
				case 2:
					Send_Freq(&Uart);
					break;
				default:
//					Send_Request();
					break;
			
			}
			Uart_Send_Flag=0;
			
			timer0_counter=0;
		
		
		}
		if(SaveData.Limit_Tab.Comp)
		{
			num=Test_Comp(&Comp_Change);
			Colour.black=LCD_COLOR_TEST_MID;
			Disp_Range_Num( num);
			Test_Comp_Fmq();
			
		}
        Send_T0_USB();
		Disp_RangeTestvalue();
	 	key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_TEST);
					}
					else
						SaveData.Limit_Tab.Comp=0;
				break;
				case Key_F2:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGE);
					}
					else
						SaveData.Limit_Tab.Comp=1;
				break;
				case Key_F3:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGECOUNT);
					}
				break;
				case Key_F4:
					
//					if(Button_Page.index==0)
//					{
//						if(Button_Page.page==0)
//							SetSystemStatus(SYS_STATUS_ITEM);
//					}
				break;
				case Key_F5:
					
//					if(Button_Page.index==0)
//					{
//						if(Button_Page.page==1)
//							Button_Page.page=0;
//						else
//							Button_Page.page=1;
//						Disp_Button_value1(Button_Page.page);
//					}
				break;
				case Key_Disp:
					
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
					
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
//				break;
				case Key_LEFT:
//				break;
				case Key_RIGHT:
//				break;
				case Key_UP:
//				break;
				case Key_DOWN:
					
					if(Button_Page.index)
						Button_Page.index=0;
					else
						Button_Page.index=1;
				break;
				case Key_NUM1:
					
				break;
				case Key_NUM2:
					
				break;
				case Key_NUM3:
					
				break;
				case Key_NUM4:
					
				break;
				case Key_NUM5:
					
				break;
				case Key_NUM6:
					
				break;
				case Key_NUM7:
					
				break;
				case Key_NUM8:
					
				break;
				case Key_NUM9:
					
				break;
				case Key_NUM0:
					
				break;
				case Key_DOT:
					
				break;
				case Key_BACK:
					
				break;
				case Key_LOCK:
					if(lock_flag)
						lock_flag=0;
					else
						lock_flag=1;
					if(lock_flag)
					{
						Lock_Control_On();
						Lock_LedOn();
						
					}
					else
					{
						Lock_LedOff();
						Lock_Control_Off();
					
					
					}
					
				break;
				case Key_BIAS:
					
				break;
				case Key_REST:
					
				break;
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}
		
		
		}
	
	
	}
//    UART_TxCmd(LPC_UART0, DISABLE);
}
//档计数显示
void Range_CountProcess(void)
{
   	 uint32_t keynum=0;
	uint32_t uart_count=0;
	 uint8_t key,lock_flag=0;
	u8 i;
	uint8_t Disp_flag=1;
//	 uint8_t page=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
    UART_TxCmd(LPC_UART0, ENABLE);
	Disp_Range_Count_Item();
	Delay_Key();
	Set_Compbcd_float();
	for(i=0;i<=10;i++)
		Count_buff[i]=0;
 	while(GetSystemStatus()==SYS_STATUS_RANGECOUNT)
	{
		key=HW_KeyScsn();
		if(Disp_flag==1)
		{
			Disp_Range_ComDispValue(&Button_Page);
			Disp_flag=0;
			
		}
		uart_count=Uart_Process();
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
//					Send_Request();
					break;
				case 1:
					Send_Main_Ord();
					break;
				case 2:
					//Send_Freq(&Uart);
					break;
				default:
//					Send_Request();
					break;
			
			}
			Uart_Send_Flag=0;
			
			timer0_counter=0;
				
		}
		if(SaveData.Limit_Tab.Comp&&uart_count==1)
		{
			Test_Comp(&Comp_Change);
			Test_Comp_Fmq();
			
		}
        Send_T0_USB();
		//Disp_RangeTestvalue();
		if(SaveData.Limit_Tab.Param)//附属比较
			;
//		Disp_RangeCount();//增加分选结果计数
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	Disp_flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
                    if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_TEST);
					}
					else
						SaveData.Limit_Tab.count=0;
				break;
				case Key_F2:
					
					if(Button_Page.index==0)
					{
                    if(Button_Page.page==0)
                        SetSystemStatus(SYS_STATUS_RANGE);
					}
					else
						SaveData.Limit_Tab.count=1;
				break;
				case Key_F3:
					
					if(Button_Page.index==0)
                    if(Button_Page.page==0)
                        SetSystemStatus(SYS_STATUS_RANGECOUNT);
				break;
				case Key_F4:
					
//					if(Button_Page.index==0)
//                    if(Button_Page.page==0)
//                        SetSystemStatus(SYS_STATUS_ITEM);
				break;
				case Key_F5:
					
//                    if(Button_Page.page==1)
//						Button_Page.page=0;
//					else
//						Button_Page.page=1;
//                    Disp_Button_value1(Button_Page.page);
				break;
				case Key_Disp:
					
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
					
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
				//break;
				case Key_RIGHT:
				//break;
				case Key_UP:
				//break;
				case Key_DOWN:
					
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
						Button_Page.index=0;
				break;
				case Key_NUM1:
				case Key_NUM2:
				case Key_NUM3:
				case Key_NUM4:
				case Key_NUM5:
				case Key_NUM6:
				case Key_NUM7:
				case Key_NUM8:
				case Key_NUM9:
				case Key_NUM0:
				case Key_DOT:
				case Key_BACK:
					break ;
				case Key_LOCK:
					if(lock_flag)
						lock_flag=0;
					else
						lock_flag=1;
					if(lock_flag)
					{
						Lock_Control_On();
						Lock_LedOn();
						
					}
					else
					{
						Lock_LedOff();
						Lock_Control_Off();
					
					
					}
					break;
				case Key_BIAS:
				case Key_REST:
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}
		
		
		}
	
	
	}
//    UART_TxCmd(LPC_UART0, DISABLE);
}
// 列表显示
void ItemProcess(void)
{
	
	uint32_t keynum;
	uint8_t key;
	uint8_t Disp_flag=1;
//    uint8_t page=1;
	
	Button_Page_Typedef Button_Page;
	SaveData.Limit_ScanValue.num=0;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_List_Count_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_ITEM)
	{
		if(Disp_flag)
		{
			Disp_flag=0;
			Disp_LIMIT_ComDispValue(&Button_Page);
		}
		
		Disp_Scan_Compvalue(0);
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
                    if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_TEST);
					}
					else if(Button_Page.index==1)
						SaveData.Limit_ScanValue.fun=0;
					
				break;
				case Key_F2:
					 
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGE);
					
					}else if(Button_Page.index==1)
						SaveData.Limit_ScanValue.fun=1;
					
				break;
				case Key_F3:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGECOUNT);
					
					}
				break;
				case Key_F4:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_ITEM);
					}
					else if(Button_Page.index==2)
					{
						if(SaveData.Limit_ScanValue.num<1)
							SaveData.Limit_ScanValue.num=20;
						else
							SaveData.Limit_ScanValue.num--;
					
					}
				break;
				case Key_F5:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							Button_Page.page=1;
						else
							Button_Page.page=0;
						Disp_Button_value1(Button_Page.page);
					}
					else if(Button_Page.index==2)
					{
						if(SaveData.Limit_ScanValue.num>=20)
							SaveData.Limit_ScanValue.num=0;
						else
							SaveData.Limit_ScanValue.num++;
					
					}
				break;
				case Key_Disp:
					
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
					
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
					
				break;
				case Key_RIGHT:
					
				break;
				case Key_UP:
					
					if(Button_Page.index>0)
						Button_Page.index--;
				break;
				case Key_DOWN:
					
					if(Button_Page.index<2)
						Button_Page.index++;
				break;
				case Key_NUM1:
				case Key_NUM2:
				case Key_NUM3:
				case Key_NUM4:
				case Key_NUM5:
				case Key_NUM6:
				case Key_NUM7:
				case Key_NUM8:
				case Key_NUM9:
				case Key_NUM0:
				case Key_DOT:
				case Key_BACK:
				case Key_LOCK:
				case Key_BIAS:
				case Key_REST:
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}		
		}
	
	}
}
//列表扫描设置
void Use_ITEMSetProcess(void)
{
	Disp_Coordinates_Typedef  Coordinates;
	uint32_t keynum=0;
	uint8_t key;
//    uint8_t page=0;
	uint8_t Disp_flag=1;
	Button_Page_Typedef Button_Page;
	
	Button_Page.index=0;
	Button_Page.page=0;
	Button_Page.third=0;
	Button_Page.force=0;
	SaveData.Limit_ScanValue.num=0;
	Disp_ListScan_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_ITEMSET)
	{
	  	
		if(Disp_flag)
		{
			Disp_flag=0;
		
			Disp_Scan_SetCompvalue(&Button_Page);
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_SETUPTEST);
						else
							SetSystemStatus(SYS_STATUS_FILE);
					}
						
				break;
				case Key_F2:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_USERDEBUG);
						else
							SetSystemStatus(SYS_STATUS_SYSSET);
					}
				break;
				case Key_F3://
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_LIMITSET);
						else
							SetSystemStatus(SYS_STATUS_TOOL);
					}
				break;
				case Key_F4:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_ITEMSET);
					}else 
					if(Button_Page.index>1)
					{
						if(Button_Page.force>0)
							Button_Page.force--;
					
					}
					
				break;
				case Key_F5:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page)
							Button_Page.page=0;
						else
							Button_Page.page=1;
						Disp_Button_TestSet(Button_Page.page);
					}else
					if(Button_Page.force<20)
						Button_Page.force++;
				break;
				case Key_Disp:
					
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
					
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
					
					if(Button_Page.third>0)
						Button_Page.third--;
						
					
				break;
				case Key_RIGHT:
					
					if(Button_Page.third<5)
						Button_Page.third++;
					else
						Button_Page.third=5;
				break;
				case Key_UP:
					
					if(Button_Page.index>0)
						Button_Page.index--;
					else
						Button_Page.page=0;
						
				break;
				case Key_DOWN:
					
					if(Button_Page.index<12)
						Button_Page.index++;
					else
						Button_Page.index=12;
				break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				//break;
				case Key_DOT:
				//break;
					
					if(Button_Page.index>2)
					{
						if(Button_Page.third==0)
						{
							Coordinates.xpos=40;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=FIRSTLINE+SPACE1+3+(Button_Page.index-2)*16;
							
							SaveData.Limit_ScanValue.freq[Button_Page.force*10+
							(Button_Page.index-3)]=Freq_Set_Num(&Coordinates);
						}else 
						if(Button_Page.third==1)
						{
						
						}else
						if(Button_Page.third==2)
						{
						
						}else
						if(Button_Page.third==3)
						{
						
						}else
						if(Button_Page.third==4)
						{
							Coordinates.xpos=420;
							Coordinates.ypos=FIRSTLINE+SPACE1+3+(Button_Page.index-2)*16;
							Coordinates.lenth=60;
							SaveData.Limit_ScanValue.time[Button_Page.force*10+
							(Button_Page.index-3)]=Disp_Set_Num(&Coordinates);
						
						
						}
					
					}
				case Key_BACK:
					
				break;
				case Key_LOCK:
					
				break;
				case Key_BIAS:
					
				break;
				case Key_REST:
					
				break;
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}
		
		
		}
	
	
	}
}
//极限设置 
void Use_LimitSetProcess(void)
{
	
	uint32_t keynum=0;
	uint8_t key,i;
	Disp_Coordinates_Typedef Coordinates;
//    uint8_t page=1;
	uint8_t Disp_flag=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_LimitList_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_LIMITSET)
	{
	  	if(Disp_flag)
		{
			Disp_flag=0;
			Disp_LimitSEt_value(& Button_Page);
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_SETUPTEST);
						else
							SetSystemStatus(SYS_STATUS_FILE);
					
					}else if(Button_Page.index==1)
					;
					else if(Button_Page.index==2)
						;
					else if(Button_Page.index==3)
					{
						SaveData.Limit_Tab.Mode=0;
					
					}
					else if(Button_Page.index==4)
					{
						SaveData.Limit_Tab.Param=0;
					}
					else if(Button_Page.index==5)
					{
						SaveData.Limit_Tab.Comp=0;
					}else
					{
						if(Button_Page.index>15)
						{
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].high.Num=0;
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].low.Num=0;	
						}else if(Button_Page.index>5)
						{
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].high.Num=0;
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low.Num=0;	
						
						}					
					}
						
				break;
				case Key_F2:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_USERDEBUG);
						else
							SetSystemStatus(SYS_STATUS_SYSSET);
					}
					else if(Button_Page.index==1)
					;
					else if(Button_Page.index==2)
						;
					else if(Button_Page.index==3)
					{
						SaveData.Limit_Tab.Mode=1;
					
					}
					else if(Button_Page.index==4)
					{
						SaveData.Limit_Tab.Param=1;
					}
					else if(Button_Page.index==5)
					{
						SaveData.Limit_Tab.Comp=1;
					}
				break;
				case Key_F3:
					
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_LIMITSET);
						else
							SetSystemStatus(SYS_STATUS_TOOL);
					}
				break;
				case Key_F4:
					
				    if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_SYSSET);
					}
	
//					if(Button_Page.index==0)
//					{
//						if(Button_Page.page==0)
//							SetSystemStatus(SYS_STATUS_ITEMSET);
//					}
					
				break;
				case Key_F5:
					
					if(Button_Page.index==0)
					{
//						if(Button_Page.page)
//							Button_Page.page=0;
//						else
//						Button_Page.page=1;
//						Disp_Button_TestSet(Button_Page.page);
					}else if(Button_Page.index>5)
					{
						for(i=0;i<10;i++)
						{
							SaveData.Limit_Tab.Comp_Value[i].low.Num=0;
							SaveData.Limit_Tab.Comp_Value[i].high.Num=0;
							
						}
						Button_Page.index=0;
					
					
					}
				break;
				case Key_Disp:
					
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
					
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
					
					if(Button_Page.index!=0)
					{
						if(Button_Page.index<=5)
						{
							Button_Page.index--;
						
						}
						else
						{
							if(Button_Page.index>15)
								Button_Page.index-=10;
							else
								Button_Page.index+=9;
						}
					
					
					}
					
				break;
				case Key_RIGHT:
					
					if(Button_Page.index!=0)
					{
//						if(Button_Page.index<=5)
//						{
//							Button_Page.index++;
//						
//						}
//						else
						//if(Button_Page.index)
						{
							if(Button_Page.index>15)
								Button_Page.index-=9;
							else
								//if(Button_Page.index<)
								Button_Page.index+=1;
						}
					
					
					}
				break;
				case Key_UP:
					
					if(Button_Page.index>16)
						Button_Page.index--;
					else if(Button_Page.index==16)
						Button_Page.index=5;
					else
						
					if(Button_Page.index>0)
						Button_Page.index--;
				break;
				case Key_DOWN:
					
					if(Button_Page.index<15)
						Button_Page.index++;
					else if(Button_Page.index==15)
						Button_Page.index=0;
					else
						
					if(Button_Page.index<25)
						Button_Page.index++;
					else
						Button_Page.index=0;
						
				break;
				case Key_NUM1:
//				break;
				case Key_NUM2:
//				break;
				case Key_NUM3:
//				break;
				case Key_NUM4:
//				break;
				case Key_NUM5:
//				break;
				case Key_NUM6:
//				break;
				case Key_NUM7:
//				break;
				case Key_NUM8:
//				break;
				case Key_NUM9:
//				break;
				case Key_NUM0:
//				break;
				case Key_DOT:
					
					if(Button_Page.index==2)//LIST2-24, FIRSTLINE,
					{
							Coordinates.xpos=LIST2-24;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=FIRSTLINE-2;
							Coordinates.lenth=66;
							SaveData.Limit_Tab.Nom=Disp_Set_InputNum(&Coordinates);
					}else if(Button_Page.index>5&&Button_Page.index<=15)
						{
							//LIST2-90, 76+(i-6)*15
							Coordinates.xpos=LIST2-90;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=76+(Button_Page.index-6)*15;
							Coordinates.lenth=60;
							SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low=Disp_Set_InputpreNum(&Coordinates);
							SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].high=SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low;
							//Sort_TypeDef Disp_Set_InputpreNum(Disp_Coordinates_Typedef *Coordinates)
						}else if(Button_Page.index>15)
						{
							Coordinates.xpos=LIST2+70;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=76+(Button_Page.index-16)*15;
							Coordinates.lenth=60;
							SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].high=Disp_Set_InputpreNum(&Coordinates);
						}
						do{
							key=HW_KeyScsn();
						}
						while(key!=0xff);
						
				break;
				case Key_BACK:
					
				break;
				case Key_LOCK:
					
				break;
				case Key_BIAS:
					
				break;
				case Key_REST:
					
				break;
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}
		
		
		}
	
	
	}
	Set_Compbcd_float();
	Savetoeeprom();
}



//系统设置
void Use_SysSetProcess(void)
{
	uint32_t  passwordnum=123769;
	uint32_t keynum=0;
	uint8_t key;	//,i=5
	uint8_t Disp_flag=1;
    RTC_TIME_Type Set_Time;
	Button_Page_Typedef Button_Page;
	SaveData.Sys_Setup.Cipher=0;
	Button_Page.index=0;
	Button_Page.page=0;
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_Sys_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_SYSSET)
	{
	  				
//           (u8 *) &RTC_TIME_Type=
			if(Disp_flag==1||Rtc_intflag)
            {
                Rtc_intflag=0;
                //if(Button_Page.page==0)
                Disp_Sys_value(&Button_Page);
    //			else if(Button_Page.page==1) 
    //			  Disp_Correction_Set(&Button_Page);
    //			else if(Button_Page.page==2)
    //				Disp_Correction_SetR(&Button_Page);
                Disp_flag=0;
            }
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==5)
		{
			Disp_flag=1;
			Key_Beep();
			Correc_successflag=0;
			switch(key)
			{
				case Key_F1:
					
					if(Button_Page.page==0)
					switch(Button_Page.index)
					{ 
						case 0:
//							if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_SETUPTEST);
//							else
//							{
//								SetSystemStatus(SYS_STATUS_SYSSET);
//								Button_Page.index=0;
//								Button_Page.page=0;								
//							}
							break;
						case 1:
							break;
						case 2:
							Saveeeprom.Sys_set.key_board=0;
							break;
						case 3:
							Saveeeprom.Sys_set.U_flag=0;
							break;
						case 4:
							SaveData.Sys_Setup.Language=0;
							break;
//						case 5:
//							SaveData.Sys_Setup.Password=1;
//							break;
						case 5:
							break;
						case 6:
							if(RTC_TIME_DISP.YEAR<1)
								RTC_TIME_DISP.YEAR=RTC_YEAR_MAX;
							else
								RTC_TIME_DISP.YEAR--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, RTC_TIME_DISP.YEAR);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 7:
							if(RTC_TIME_DISP.MONTH<=1)
								RTC_TIME_DISP.MONTH=RTC_MONTH_MAX;
							else
								RTC_TIME_DISP.MONTH--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, RTC_TIME_DISP.MONTH);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 8:
							if(RTC_TIME_DISP.DOM<=1)
								RTC_TIME_DISP.DOM=RTC_DAYOFMONTH_MAX;
							else
								RTC_TIME_DISP.DOM--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, RTC_TIME_DISP.DOM);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
//						case 10:
//							break ;
						case 9:
							if(Saveeeprom.Sys_set.buard<1)
								Saveeeprom.Sys_set.buard=5;
							else
								Saveeeprom.Sys_set.buard--;
							break;
						case 10:
							SaveData.Sys_Setup.Bias=0;
							break;
						case 11:
							SaveData.Sys_Setup.Bus_Mode=0;
							break;
						case 12:
							if(SaveData.Sys_Setup.GP_Addr<1)
								SaveData.Sys_Setup.GP_Addr=99;
							else
								SaveData.Sys_Setup.GP_Addr--;
							break;
						case 13:
							SaveData.Sys_Setup.Talk_Only=1;
							break;
						case 14:
							if(RTC_TIME_DISP.HOUR<1)
								RTC_TIME_DISP.HOUR=RTC_HOUR_MAX;
							else
								RTC_TIME_DISP.HOUR--;
                             RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, RTC_TIME_DISP.HOUR);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 15:
							if(RTC_TIME_DISP.MIN<1)
								RTC_TIME_DISP.MIN=RTC_MINUTE_MAX;
							else
								RTC_TIME_DISP.MIN--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, RTC_TIME_DISP.MIN);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 16:
							if(RTC_TIME_DISP.SEC<1)
								RTC_TIME_DISP.SEC=RTC_SECOND_MAX;
							else
								RTC_TIME_DISP.SEC--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, RTC_TIME_DISP.SEC);
                            
                            RTC_CalibConfig(LPC_RTC, 2, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						default:
							break;
					
					}
					
					else  if(Button_Page.page==1)
						switch(Button_Page.index)
					{ 
						case 0:
							break ;
						case 1:
						case 2:										  
						case 3:
						case 4:
						case 5:							
						case 6:					
						case 7:
						case 8:
//							Uart_Send_Flag=2;
							Correction_C(&Button_Page);		
							Correc_successflag=1;
							break;
						default:
							break;						
					}
					else  if(Button_Page.page==2)
						switch(Button_Page.index)
						{
							case 0:
								 break ;
							case 1:
							case 2:
							case 3:
							case 4:
							case 5:
	//							Uart_Send_Flag=4;
								Correction_R(&Button_Page);	
								Correc_successflag=1;
								break ;
								default :
									break ;
						}
					else
					{
						Button_Page.page=1;
						Button_Page.index=0;
						Disp_Serve_correction();
						Init_C();							
					}
						
                    
				break;
				case Key_F2:
					
					if(Button_Page.page==0)
					switch(Button_Page.index)
					{
						case 0:
						//	SetSystemStatus(SYS_STATUS_SETUPTEST);
							break;
						case 1:
							break;
						case 2:
							Saveeeprom.Sys_set.key_board=1;
							break;
						case 3:
							Saveeeprom.Sys_set.U_flag=1;
							break;
						case 4:
							SaveData.Sys_Setup.Language=1;
							break;
//						case 5:
//							SaveData.Sys_Setup.Password=0;
//							break;
						case 5:
							break;
						case 6:
							if(RTC_TIME_DISP.YEAR>RTC_YEAR_MAX)
								RTC_TIME_DISP.YEAR=0;
							else
								RTC_TIME_DISP.YEAR++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, RTC_TIME_DISP.YEAR);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 7:
							if(RTC_TIME_DISP.MONTH>=RTC_MONTH_MAX)
								RTC_TIME_DISP.MONTH=1;
							else
								RTC_TIME_DISP.MONTH++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, RTC_TIME_DISP.MONTH);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 8:
							if(RTC_TIME_DISP.DOM>=RTC_DAYOFMONTH_MAX)
								RTC_TIME_DISP.DOM=1;
							else
								RTC_TIME_DISP.DOM++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, RTC_TIME_DISP.DOM);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
//						case 9:
//							break ;							
						case 9:
							if(Saveeeprom.Sys_set.buard>4)
								Saveeeprom.Sys_set.buard=0;
							else
								Saveeeprom.Sys_set.buard++;
							break;
						case 10:
							SaveData.Sys_Setup.Bias=1;
							break;
						case 11:
							SaveData.Sys_Setup.Bus_Mode=1;
							break;
						case 12:
							if(SaveData.Sys_Setup.GP_Addr>99)
								SaveData.Sys_Setup.GP_Addr=0;
							else
								SaveData.Sys_Setup.GP_Addr++;
							break;
						case 13:
							SaveData.Sys_Setup.Talk_Only=0;
							break;
						case 14:
							if(RTC_TIME_DISP.HOUR>=RTC_HOUR_MAX)
								RTC_TIME_DISP.HOUR=0;
							else
								RTC_TIME_DISP.HOUR++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, RTC_TIME_DISP.HOUR);
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 15:
							if(RTC_TIME_DISP.MIN>=RTC_MINUTE_MAX)
								RTC_TIME_DISP.MIN=0;
							else
								RTC_TIME_DISP.MIN++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, RTC_TIME_DISP.MIN);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 16:
							if(RTC_TIME_DISP.SEC>=RTC_SECOND_MAX)
								RTC_TIME_DISP.SEC=0;
							else
								RTC_TIME_DISP.SEC++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, RTC_TIME_DISP.SEC);
                            
                            RTC_CalibConfig(LPC_RTC, 2, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						default:
							break;
					
					}
					else if(Button_Page.page==3)
					{
							Button_Page.page=2;
							Button_Page.index=0;
							Disp_Serve_correctionR();
							Init_R();	
					}
					else if(Button_Page.page==1)
						switch(Button_Page.index)
						{
							case 0:
								break ;
							case 1:
							case 2:
							case 3:
							case 4:
							case 5:
							case 6:					
						    case 7:
						    case 8:
								Save_C();
								Button_Page.page=3;
								Button_Page.index=0;
								Disp_Start_Corr();
								break ;
								default :
									break ;
						}
//						if(Button_Page.index!=0)
//						{
//							Save_C();
//							Button_Page.page=3;
//							Button_Page.index=0;
//							Disp_Start_Corr();
//						}
					else  if(Button_Page.page==2)
					switch(Button_Page.index)
					{ 
						case 0:
							break ;
						case 1:
						case 2:										  
						case 3:
						case 4:
						case 5:							
							Save_R();
							Button_Page.page=3;
							Button_Page.index=0;
							Disp_Start_Corr();
							break;
						default:
							break;						
					}
//						if(Button_Page.index!=0)
//						{
//							Save_R();
//							Button_Page.page=3;
//							Button_Page.index=0;
//							Disp_Start_Corr();
//						}					                   
				break;
				case Key_F3:
					
					if(Button_Page.page==0)
					switch(Button_Page.index)
					{
						case 11:
							SaveData.Sys_Setup.Bus_Mode=2;
							
						break;
						default:
						break;
					
					}
					else  if(Button_Page.page==3)
						switch(Button_Page.index)
					{
						case 0:
//						   Uart_Send_Flag=5;
//						Save_R();
//					INSTRUCT_TO_DEBUG;
						Exit_correction();
						INSTRUCT_TO_DEBUG
						Exit_correction();
						SetSystemStatus(SYS_STATUS_TEST);
						break;
						default:
						break;
					 
					}
						
						
				break;
				case Key_F4:
					
					if(Button_Page.page==0)
					switch(Button_Page.index)
					{
						case 0:
							break;//恢复默认设置
						case 11:
							SaveData.Sys_Setup.Bus_Mode=3;
							
						break;
						default:
						break;
					
					}
				break;
				case Key_F5:
					
					if(Button_Page.page==0)
					switch(Button_Page.index)
					{
						case 0:
							break;//恢复系统复位
						case 11:
							SaveData.Sys_Setup.Bus_Mode=4;
							
						break;
						default:
						break;
					
					}
					else if(Button_Page.page==3)
					{	
						Exit_correction();
						LONGDELAY;
						Clear_Instr();
					}
				break;
				case Key_Disp:
					
					if(Button_Page.page==3)	
//						Uart_Send_Flag=6;
					{	
						Exit_correction();
						INSTRUCT_TO_DEBUG
						Exit_correction();
						SetSystemStatus(SYS_STATUS_TEST);
					
					}
					else if(Button_Page.page==0)
						SetSystemStatus(SYS_STATUS_TEST);

				break;
				case Key_SETUP:					
					
					 if(Button_Page.page==3)
//						Uart_Send_Flag=5; 
					 {
						Exit_correction();
						INSTRUCT_TO_DEBUG 
						Exit_correction();
						SetSystemStatus(SYS_STATUS_SETUPTEST);
						 
					 }
					 else  if(Button_Page.page==0)
						SetSystemStatus(SYS_STATUS_SETUPTEST); 

				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
					
					if(Button_Page.page==0)
					{
						if(Button_Page.index>8&&Button_Page.index<14)
							Button_Page.index-=8;
						else if(Button_Page.index==15||Button_Page.index==16)
							Button_Page.index-=1;
						else if(Button_Page.index==7||Button_Page.index==8)
							Button_Page.index-=1;
						else if(Button_Page.index==14)
							Button_Page.index-=6;
                        else if(Button_Page.index==1)
                            Button_Page.index=16;
                            
                        else
                            Button_Page.index+=7;
                            
				    }
				break;
				case Key_RIGHT:
					
					if(Button_Page.page==0)
					{
						if(Button_Page.index<6)
							Button_Page.index+=8;
						else if(Button_Page.index<8)	//Button_Page.index==7||Button_Page.index==8
							Button_Page.index+=1;
						else if(Button_Page.index==14||Button_Page.index==15)
							Button_Page.index+=1;
						else if(Button_Page.index==8)
							Button_Page.index+=6;
                        else if(Button_Page.index==16)
                            Button_Page.index=1;
                        else
                            Button_Page.index-=7;
					}
				break;
				case Key_DOWN:
					
					if(Button_Page.page==0)
					{
						if(Button_Page.index>15)
							Button_Page.index=0;
						else
							Button_Page.index++;
					}
					else if(Button_Page.page==1)
					{
						if(Button_Page.index>7)
							Button_Page.index=0;
						else
							Button_Page.index++;					
					}
					else
					{
						if(Button_Page.index>4)
							Button_Page.index=0;
						else
							Button_Page.index++;	
					
					}
				break;
				case Key_UP:
					
					if(Button_Page.page==0)
					{
						if(Button_Page.index<1)
							Button_Page.index=16;
						else
							Button_Page.index--;
				    }
					else if(Button_Page.page==1)
					{
						if(Button_Page.index<1)
							Button_Page.index=8;
						else
							Button_Page.index--;
					
					}
					else
					{
					
						if(Button_Page.index<1)
							Button_Page.index=5;
						else
							Button_Page.index--;
					
					}
				break;
				case Key_NUM1:		//passwordnum
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
					    SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+1;
				break;
				case Key_NUM2:
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
						SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+2;
				break;
				case Key_NUM3:
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
					    SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+3;
				break;
				case Key_NUM4:
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
						SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+4;
				break;
				case Key_NUM5:
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
						SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+5;
				break;
				case Key_NUM6:
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
						SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+6;
				break;
				case Key_NUM7:
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
						SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+7;
				break;
				case Key_NUM8:
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
						SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+8;
				break;
				case Key_NUM9:
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
						SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+9;
					if(passwordnum==SaveData.Sys_Setup.Cipher)
					{
						
						Button_Page.index=0;
						Button_Page.page=3;
						Disp_Start_Corr();
//						Disp_Serve_correction();
//						Uart_Send_Flag=1;
						Start_Correction();
					}
				break;
				case Key_NUM0:
					
					if(Button_Page.index==10)
					if(SaveData.Sys_Setup.Cipher<100000)
						SaveData.Sys_Setup.Cipher=SaveData.Sys_Setup.Cipher*10+0;
				break;
				case Key_DOT:
					
				break;
				case Key_BACK:
					
				break;
				case Key_LOCK:
					
				break;
				case Key_BIAS:
					
				break;
				case Key_REST:
					
				break;
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}
		
		
		}
	
	
	}
    Savetoeeprom();
}

//用户校正
void Use_DebugProcess(void)
{
	
	uint32_t keynum=0;
	uint8_t key;
//    uint8_t page=1;
	uint8_t Disp_flag=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_UserCheck_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_USERDEBUG)
	{
		if(Disp_flag==1)
		{
			Disp_Debug_value(&Button_Page);
			Disp_flag=0;	
		}
	  	
			if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
					break;
				case 1:	
					 Open_Clear();	
					break;
				case 2:	
                    Open_Clear();
					//Short_Clear();
					break;
				case 3:	
					Close_Clear();
					break;
				default:
					break;
			
			}
			
			Uart_Send_Flag=0;
			timer0_counter=0;		
		
		}
		
		
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
                    if(Button_Page.page==0)
					 switch(Button_Page.index)
						{
							case 0:
								SetSystemStatus(SYS_STATUS_SETUPTEST);
								break;
							case 1:	
								SaveData.User_Correction.Open=0;
								Uart_Send_Flag=3;
								break;
							case 2:	
								SaveData.User_Correction.Short=0;
								Uart_Send_Flag=3;
								break;
							default:
								break;
						
						}
//                        SetSystemStatus(SYS_STATUS_SETUPTEST);
//					else
//						SetSystemStatus(SYS_STATUS_FILE);
						
				break;
				case Key_F2:
					
                    if(Button_Page.page==0)
						switch(Button_Page.index)
						{
							case 0:
								SetSystemStatus(SYS_STATUS_USERDEBUG);
								break;
							case 1:	
								SaveData.User_Correction.Open=1;
								Uart_Send_Flag=1;
								break;
							case 2:	
								SaveData.User_Correction.Short=1;
								Uart_Send_Flag=2;
								break;
							default:
								break;
						
						}
//                        SetSystemStatus(SYS_STATUS_USERDEBUG);
//					else
//						SetSystemStatus(SYS_STATUS_SYSSET);
				break;
				case Key_F3:
					
                    if(Button_Page.page==0)
                        SetSystemStatus(SYS_STATUS_LIMITSET);
					else
						SetSystemStatus(SYS_STATUS_TOOL);
				break;
				case Key_F4:
					
                    if(Button_Page.page==0)
                        SetSystemStatus(SYS_STATUS_SYSSET);					
//                    if(Button_Page.page==0)
//                        SetSystemStatus(SYS_STATUS_ITEMSET);
				break;
				case Key_F5:
					
//					if(Button_Page.page)
//						Button_Page.page=0;
//					else
//						Button_Page.page=1;
//                    Disp_Button_TestSet(Button_Page.page);
				break;
				case Key_Disp:
					
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
					
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
					
				break;
				case Key_RIGHT:
					
				break;
				case Key_UP:
					    
						if(Button_Page.index>0)
							Button_Page.index--;
						else
							Button_Page.index=1;
							
				break;
				case Key_DOWN:
					
					if(Button_Page.index>1)
						Button_Page.index=0;
					else
						Button_Page.index++;
				break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				//break;
				case Key_DOT:
					
//					if(Button_Page.index==5)
//					{ 	Disp_Coordinates_Typedef Coordinates;
//						Coordinates.xpos=LIST1+92;
//						Coordinates.ypos=FIRSTLINE+SPACE1*4;
//						Coordinates.lenth=86;
//						
//						SaveData.Main_Func.Freq=Freq_Set_Num(&Coordinates);
//					
//					}
				break;
				case Key_BACK:
					
				break;
				case Key_LOCK:
					
				break;
				case Key_BIAS:
					
				break;
				case Key_REST:
					
				break;
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}
		
		
		}
	
	
	}
}

//公司校正
void Fac_DebugProcess(void)
{
	
 	
	uint32_t keynum=0;
	uint8_t key;
    uint8_t set_num;
    uint8_t num_flag=0;
    uint8_t debugbuff[20];
    uint8_t setupflag=0,sendnum;
    Disp_Coordinates_Typedef Disp_pos;
//    uint8_t page=1;
	uint8_t Disp_flag=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
    debug_over=0;
    debugbuff[0]=0xAB ;// 08 06 E0 00 00 BF
    debugbuff[1]=01;
    debugbuff[2]=8;
    debugbuff[3]=6;
    debugbuff[4]=0xc0;
    debugbuff[5]=0x01;
    debugbuff[6]=0x08;
    debugbuff[7]=0xbf;
    Delay(1000);
    UARTPuts( LPC_UART0, debugbuff);//发送校正命令
    Delay(200);
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_FacrCheck_Item();
	Delay_Key();
    debugbuff[0]=0xAB ;// 08 06 E0 00 00 BF
    debugbuff[1]=01;
    debugbuff[2]=8;
    debugbuff[3]=6;
    debugbuff[4]=0xe0;
    debugbuff[5]=0;
    debugbuff[6]=0;
    debugbuff[7]=0xbf;
 	while(GetSystemStatus()==SYS_STATUS_FACRDEBUG)
	{
        Uart_Process();
        switch(setupflag)
        {
            case 1://电容校正
                set_num=7;
                
            break;
            case 2://电阻校正
                set_num=4;
                
            break;
            case 3://退出保存
                
            break;
            case 4://清除校正
                
            break ;
        
        }
        if(debug_over)
        {
            debug_over=0;
            Start_Debugled(0);
        
        }
        
		if(Disp_flag==1)
		{
            switch(setupflag)
            {
                case 0:
                    if(num_flag)
                    {
                        Disp_pos.xpos=80;
                        Disp_pos.ypos=120;
                        Disp_pos.lenth=120;
                        input_num(&Disp_pos );
                        num_flag=0;
                        
                    }
                    
                break;
                case 1:
                    Disp_Fac_Debug_C(&Button_Page);
                    
                break;
                case 2:
                    Disp_Fac_Debug_R(& Button_Page);//电阻校正界面;
                    
                break;
                case 3:
                    
                break;
                case 4:
                    
                break;
                default:
                    break;
                
            
            
            }
            
			Disp_flag=0;	
		}	
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
        if(debug_over==0)	
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1://AB 01 08 06 E0 00 00 BF
                    if(Button_Page.index==0)
                    {
                        setupflag=1;
//                        debugbuff[4]=0xe0;
//                        debugbuff[5]=0;
//                        UARTPuts( LPC_UART0, debugbuff);//电容清零命令
                        
                    }
                    else
                    {
                        debugbuff[5]=Button_Page.index;
                        if(setupflag==1)
                        {
                            debugbuff[4]=0xe0;
                            UARTPuts( LPC_UART0, debugbuff);//每个容值的校正命令
                            Start_Debugled(1);
                            
                        }
                        else if(setupflag==2)
                        {
                            debugbuff[4]=0xe1;
                            UARTPuts( LPC_UART0, debugbuff);//每个电阻的校正命令
                            Start_Debugled(1);
                            
                        }
                        
                        
                        
                    }
                    //Colour.black=
					LCD_DrawRect( 0, 22, 479,271-40  , LCD_COLOR_TEST_BACK ) ;
						
				break;
				case Key_F2:
                    if(Button_Page.index==0)
                    {
                        setupflag=2;
                        
                    
                    }
//                    else
//                    {
//                        debugbuff[5]=0x0f;
//                        if(setupflag==1)
//                        {
//                            debugbuff[4]=0xe0;
//                            UARTPuts( LPC_UART0, debugbuff);
//                            
//                        }
//                        else if(setupflag==2)
//                        {
//                            debugbuff[4]=0xe1;
//                            UARTPuts( LPC_UART0, debugbuff);
//                            
//                        }
//                    
//                    
//                    }
//                        sendnum=Button_Page.index;
                   LCD_DrawRect( 0, 22, 479,271-40  , LCD_COLOR_TEST_BACK ) ;
					
                    
				break;
				case Key_F3:
                    if(Button_Page.index==0)
                    {
                        debugbuff[5]=0x0f;
                        if(setupflag==1)
                        {
                            debugbuff[4]=0xe0;
                            UARTPuts( LPC_UART0, debugbuff);
                            
                        }
                        else if(setupflag==2)
                        {
                            debugbuff[4]=0xe1;
                            UARTPuts( LPC_UART0, debugbuff);
                            
                        }
                        
                    
                    }  
                    LCD_DrawRect( 0, 22, 479,271-40  , LCD_COLOR_TEST_BACK ) ;
                    

				break;
				case Key_F4://校正清零命令
                    if(Button_Page.index==0)
                    {
                        debugbuff[5]=0;
                        if(setupflag==1)
                        {
                            debugbuff[4]=0xe0;
                            UARTPuts( LPC_UART0, debugbuff);
                            
                        }
                        else if(setupflag==2)
                        {
                            debugbuff[4]=0xe1;
                            UARTPuts( LPC_UART0, debugbuff);
                            
                        }
                    
                    }
					LCD_DrawRect( 0, 22, 479,271-40  , LCD_COLOR_TEST_BACK ) ;
                 
				break;
				case Key_F5:
					

				break;
				case Key_Disp:
                    num_flag=1;
					
				break;
				case Key_SETUP:
					
				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
					
				break;
				case Key_RIGHT:
					
				break;
				case Key_UP:
					    if(setupflag==1||setupflag==2)
                        {
						if(Button_Page.index>0)
							Button_Page.index--;
						else
							Button_Page.index=set_num;
                        }
							
				break;
				case Key_DOWN:
					if(setupflag==2||setupflag==1)
                    {
                        if(Button_Page.index>set_num)
                            Button_Page.index=0;
                        else
                            Button_Page.index++;
                    }
				break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				//break;
				case Key_DOT:
					
//					if(Button_Page.index==5)
//					{ 	Disp_Coordinates_Typedef Coordinates;
//						Coordinates.xpos=LIST1+92;
//						Coordinates.ypos=FIRSTLINE+SPACE1*4;
//						Coordinates.lenth=86;
//						
//						SaveData.Main_Func.Freq=Freq_Set_Num(&Coordinates);
//					
//					}
				break;
				case Key_BACK:
					
				break;
				case Key_LOCK:
					
				break;
				case Key_BIAS:
					
				break;
				case Key_REST:
					
				break;
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}
		
		
		}
	
	
	}
}	
//==========================================================
//函数名称：Uart_Process
//函数功能：串口处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26 
//修改日期：2015.10.26 10:02
//备注说明：无
//==========================================================
u8 Uart_Process(void)
{
	uint8_t i;
#if HW_UART_SUPPORT
	u8 kind=0xff;
	u8 data=0;
//#if DEBUG_SUPPORT
	u8 str[(FRAME_LEN_MAX-FRAME_LEN_MIN)+1];//收发数据缓冲
//#else
//	u8 str[(FRAME_LEN_MAX-FRAME_LEN_MIN)+1];//收发数据缓冲
//#endif

//	if(SaveData.Sys_Setup.Bus_Mode==0)//串口有效
	{
		if (ComBuf.rec.end)//接收数据结束
		{data=1;
			//memset(str,'\0',(FRAME_LEN_MAX-FRAME_LEN_MIN+1));//清空缓冲
            kind=ComBuf.rec.buf[PFRAMEKIND];//命令字
            ComBuf.rec.buf[PFRAMEKIND]=0;
            switch(kind)
            {
                
                case 0x03:
//                    Debug_over=1;
//                    break;
                case 0x01:
                    if(ComBuf.rec.buf[2]==0x26)//27)
                    {
                        for(i=0;i<5;i++)
                        {
                            if(ComBuf.rec.buf[i+5]==' ')
                                Test_Dispvalue.Main_valuebuff[i]=ComBuf.rec.buf[i+5];
                            else
                                Test_Dispvalue.Main_valuebuff[i]=ComBuf.rec.buf[i+5]+'0';
                                
                            Comp_Change.all[0].buff[i]=ComBuf.rec.buf[i+5];
                            if(ComBuf.rec.buf[i+13]==' ')
                                Test_Dispvalue.Secondvaluebuff[i]=ComBuf.rec.buf[i+13];
                            else
                                Test_Dispvalue.Secondvaluebuff[i]=ComBuf.rec.buf[i+13]+'0';
                            Comp_Change.all[1].buff[i]=ComBuf.rec.buf[i+13];
                            if(ComBuf.rec.buf[i+21]==' ')
                                Test_Dispvalue.Vmvaluebuff[i]=ComBuf.rec.buf[i+21];
                            else
                                Test_Dispvalue.Vmvaluebuff[i]=ComBuf.rec.buf[i+21]+'0';
                            Comp_Change.all[2].buff[i]=ComBuf.rec.buf[i+21];
                            if(ComBuf.rec.buf[i+29]==' ')
                            Test_Dispvalue.Imvaluebuff[i]=ComBuf.rec.buf[i+29];
                            else
                                Test_Dispvalue.Imvaluebuff[i]=ComBuf.rec.buf[i+29]+'0';
                            Comp_Change.all[3].buff[i]=ComBuf.rec.buf[i+29];
                                        
                        }
                        Test_Dispvalue.Dot[0]=ComBuf.rec.buf[10];				
                        Test_Dispvalue.Dot[1]=ComBuf.rec.buf[18];
                        Test_Dispvalue.Dot[2]=ComBuf.rec.buf[26];
                        Test_Dispvalue.Dot[3]=ComBuf.rec.buf[34];
                        Set_daot(Test_Dispvalue.Main_valuebuff,Test_Dispvalue.Dot[0]);
                        Set_daot(Test_Dispvalue.Secondvaluebuff,Test_Dispvalue.Dot[1]);
                        Set_daot(Test_Dispvalue.Vmvaluebuff,Test_Dispvalue.Dot[2]);
                        Set_daot(Test_Dispvalue.Imvaluebuff,Test_Dispvalue.Dot[3]);
                        Test_Dispvalue.Unit[0]=ComBuf.rec.buf[11];
                        Test_Dispvalue.Unit[1]=ComBuf.rec.buf[19];
                        Test_Dispvalue.Unit[2]=ComBuf.rec.buf[27];
                        Test_Dispvalue.Unit[3]=ComBuf.rec.buf[35];
                        for(i=0;i<4;i++)
                        {
                            Comp_Change.all[i].Dot=Test_Dispvalue.Dot[i];
                            Comp_Change.all[i].Unit=Test_Dispvalue.Unit[i];
                        
                        }
                    }
                    else if(ComBuf.rec.buf[5]==3)
                        debug_over=1;
                        //校正完成标志
                    
                
                    break;
            
            
            
            }
                
//            if(ComBuf.rec.buf[11]<7&&ComBuf.rec.buf[19]<7&&ComBuf.rec.buf[27]<7&&ComBuf.rec.buf[35]<7&&ComBuf.rec.buf[2]==38)
//                
//			{
//				//memcpy(str,&ComBuf.rec.buf[PDATASTART],ComBuf.send.len-FRAME_LEN_MIN);//数据包
//				
//				for(i=0;i<5;i++)
//				{
//					if(ComBuf.rec.buf[i+5]==' ')
//						Test_Dispvalue.Main_valuebuff[i]=ComBuf.rec.buf[i+5];
//					else
//						Test_Dispvalue.Main_valuebuff[i]=ComBuf.rec.buf[i+5]+'0';
//						
//					Comp_Change.all[0].buff[i]=ComBuf.rec.buf[i+5];
//					if(ComBuf.rec.buf[i+13]==' ')
//						Test_Dispvalue.Secondvaluebuff[i]=ComBuf.rec.buf[i+13];
//					else
//						Test_Dispvalue.Secondvaluebuff[i]=ComBuf.rec.buf[i+13]+'0';
//					Comp_Change.all[1].buff[i]=ComBuf.rec.buf[i+13];
//					if(ComBuf.rec.buf[i+21]==' ')
//						Test_Dispvalue.Vmvaluebuff[i]=ComBuf.rec.buf[i+21];
//					else
//						Test_Dispvalue.Vmvaluebuff[i]=ComBuf.rec.buf[i+21]+'0';
//					Comp_Change.all[2].buff[i]=ComBuf.rec.buf[i+21];
//					if(ComBuf.rec.buf[i+29]==' ')
//					Test_Dispvalue.Imvaluebuff[i]=ComBuf.rec.buf[i+29];
//					else
//						Test_Dispvalue.Imvaluebuff[i]=ComBuf.rec.buf[i+29]+'0';
//					Comp_Change.all[3].buff[i]=ComBuf.rec.buf[i+29];
//								
//				}
//				Test_Dispvalue.Dot[0]=ComBuf.rec.buf[10];				
//				Test_Dispvalue.Dot[1]=ComBuf.rec.buf[18];
//				Test_Dispvalue.Dot[2]=ComBuf.rec.buf[26];
//				Test_Dispvalue.Dot[3]=ComBuf.rec.buf[34];
//                Set_daot(Test_Dispvalue.Main_valuebuff,Test_Dispvalue.Dot[0]);
//                Set_daot(Test_Dispvalue.Secondvaluebuff,Test_Dispvalue.Dot[1]);
//				Set_daot(Test_Dispvalue.Vmvaluebuff,Test_Dispvalue.Dot[2]);
//				Set_daot(Test_Dispvalue.Imvaluebuff,Test_Dispvalue.Dot[3]);
//				Test_Dispvalue.Unit[0]=ComBuf.rec.buf[11];
//				Test_Dispvalue.Unit[1]=ComBuf.rec.buf[19];
//				Test_Dispvalue.Unit[2]=ComBuf.rec.buf[27];
//				Test_Dispvalue.Unit[3]=ComBuf.rec.buf[35];
//				for(i=0;i<4;i++)
//				{
//					Comp_Change.all[i].Dot=Test_Dispvalue.Dot[i];
//					Comp_Change.all[i].Unit=Test_Dispvalue.Unit[i];
//				
//				}

//			}
			//准备接收下一帧数据sprintf
			ComBuf.rec.end=0;//接收缓冲可读标志复位
			ComBuf.rec.ptr=0;//接收指针清零
		}
	}
//	WriteString_Big(0, 150, (uint8_t *)&ComBuf.rec.buf[1]);
	ComBuf.rec.end=0;

//	switch(kind)
//	{
//		case FRAME_READ_RESULT://读取结果
//			//串口发送测试数据:电压(5)+电阻(6)+时间(4)+分选(1)=16字节
//			switch (GetSystemMessage())//系统信息
//			{
//				case MSG_ABORT:
//					kind=0x9B;//测试中止
//					break;
//				case MSG_PASS:
//					kind=0x91;//测试通过
//					break;
//				case MSG_HIGH:
//					kind=0x92;//上限报警
//					break;
//				case MSG_LOW:
//					kind=0x92;//下限报警
//					break;
//				default:
//					kind=0x90;//正常测试
//					break;
//			}		
//			ComBuf.send.buf[1+5+6+4]=kind;
//			ComBuf.send.begin=0;
//			ComBuf.send.len=PackStandFrame(ComBuf.send.buf , &ComBuf.send.buf[1] , 16  );
////			if(SendDataToCom()==0)//发送成功判别
////			{
////			//	Delay_1ms(100);//延时
////			//	SendDataToCom();//发送
////			}
//			break;
//		
//		case FRAME_START://启动
//			SetSystemStatus(SYS_STATUS_TEST);//系统状态-启动测试
//			break;

//		case FRAME_RESET://复位
//			//SetSystemStatus(SYS_STATUS_IDLE);//系统状态-待机
//			break;

//		case FRAME_WRITE_SN://写序列号
//			break;
//		
//		case FRAME_CLR_BOOT_NUM://清开机次数
//			break;
//		
//		case FRAME_DATA://数据帧
//			break;

//		default:
//			break;
//	}
	return data;
#endif
}


void  Avg_Test(uint8_t *buff,uint8_t dot,uint8_t *buff1,uint8_t dot1)		//平均测试值
{
	uint8_t i;
//uint8_t  Avg_Test_Main[25][6];
//uint8_t  Avg_Test_Second[25][6];
//uint8_t  Avg_Count=0;
//	if(SaveData.Main_Func.Avg>1)
       for(i=0;i<5;i++)
		{
			Avg_Test_Main[Avg_Count-1][i]=buff[i];	
			Avg_Test_Second[Avg_Count-1][i]=buff1[i];
		}
			Avg_Test_Main[Avg_Count-1][5]=dot;
			Avg_Test_Second[Avg_Count-1][5]=dot;
}


//void  Avg_Test_All(void)
//{
//	uint32_t  Total_data_Main,Total_data_Second;
//	uint8_t i,j,dot_num_main,dot_num_second;
//	uint32_t Multiple_Main,Multiple_Second;	//倍数
//	
//	
//	for(i=0;i<SaveData.Main_Func.Avg-1;i++)
//	{
//		if(Avg_Test_Main[i][5]>Avg_Test_Main[i+1][5])
//			dot_num_main=Avg_Test_Main[i][5];
//		else
//			dot_num_main=Avg_Test_Main[i+1][5];
//		
//		if(Avg_Test_Second[i][5]>Avg_Test_Second[i+1][5])
//			dot_num_second=Avg_Test_Second[i][5];
//		else
//			dot_num_second=Avg_Test_Second[i+1][5];	
//	}
//	
////	if(dot_num_main==5)
////		 Multiple_Main=100000;
////	else if(dot_num_main==4)
////		 Multiple_Main=10000;
////	else if(dot_num_main==3)
////		 Multiple_Main=1000;
////	else if(dot_num_main==2)
////		 Multiple_Main=100;
////	else if(dot_num_main==1)
////		 Multiple_Main=10;
////	else if(dot_num_main==0)
////		 Multiple_Main=1;
////	
////	if(dot_num_second==5)
////		 Multiple_Second=100000;
////	else if(dot_num_second==4)
////		 Multiple_Second=10000;
////	else if(dot_num_second==3)
////		 Multiple_Second=1000;
////	else if(dot_num_second==2)
////		 Multiple_Second=100;
////	else if(dot_num_second==1)
////		 Multiple_Second=10;
////	else if(dot_num_second==0)
////		 Multiple_Second=1;
//	
//	
//	for(i=0;i<SaveData.Main_Func.Avg;i++)
//	{
//		for(j=0;j<5;j++)
//		{
//			Multiple_Main=Avg_Test_Main[i][j]-'0';
////			Avg_Test_Main[i][j]=-'0';
//			
//		
//		}	
//	}


//}


//void Set_daot(uint8_t *buff,uint8_t dot)
//{
//	uint8_t i;
//	for(i=0;i<dot;i++)
//	{
//		buff[5-i]=buff[5-i-1];		
//	}
//	if(dot==0)
//		buff[5]=' ';
//	else
//	buff[5-dot]='.';
//	for(i=0;i<6;i++)
//	{
//		if(buff[i]>0x80)
//			buff[i]='0';
//	}
//	buff[6]=0;


//}


////全局变量
//u8 WaitRecTimeOver;

//==========================================================
//函数名称：PackStandFrame
//函数功能：将数据打包成帧
//入口参数：*framebuf:帧数据缓冲
//			*datbuf:数据缓冲
//			len:数据长度
//出口参数：一帧数据长度
//创建日期：2014.04.11
//修改日期：2014.04.11 10:28
//备注说明：旧的发送协议
//开始(0xAB)，电压(5) 电阻(6) 时间(4)，分选(1)，结束(0xAF)=18字节
//==========================================================
int8_t PackStandFrame(int8_t * framebuf, int8_t * datbuf, int8_t len)
{
	if(len>(SEND_LEN_MAX-2))//数据的最大长度
		len=(SEND_LEN_MAX-2);
	framebuf[0]=UART_SEND_BEGIN;//帧头
	memcpy(&framebuf[1], datbuf, len);//数据
	framebuf[len+1]=UART_SEND_END;//帧尾
	return (len+1);//返回一帧数据长度
}

const u32 UNIT_VALUE[15]=
{
	1,1E3,1E6

};
Sort_TypeDef Input_compvalue(Disp_Coordinates_Typedef *Coordinates)
{
	u8 key;		//,i
	u8 page=0;
	u8 disp_flag=1;
//	u8 index=0;
	u8 disp_cound=0;
	u8 disp_pow=0;
//	u8 input_flag=0;
	u8 dot=5;//小数点
	u8 dot_num=0;
	uint8_t While_flag=1;
	uint32_t keynum=0;
	uint8_t Disp_buff[10];
	uint8_t key_count;
	uint8_t dot_num1;
	uint8_t del_flag=0;
	Sort_TypeDef   Sort_set;
	
//	u8 unit=0;//单位
	u32 Word;
	
	float conp_value=0;
//	float old_value;
	key_count=0;
//	old_value=SaveData.SetParameter.Nominal_value.comp_value;
//	if(SaveData.SysParameter.language==0)
//		Disp_Inputback("输入数值");//背景色
//	else
//		Disp_Inputback("INPUTNUM");//背景色
	LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , 
	Coordinates->ypos+16 , Red );
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			disp_flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					//unit_c=UNIT_VALUE[DispBuf[disp_cound]-'0'];
						conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						if(page==0)
							Sort_set.Unit=0;
						else
							Sort_set.Unit=4;
							
						While_flag=0;
					break;
				case Key_F2:
						conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						if(page==0)
							Sort_set.Unit=1;
						else
							Sort_set.Unit=5;
						While_flag=0;
					break;
				case Key_F3:
					conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						if(page==0)
							Sort_set.Unit=2;
						else
							Sort_set.Unit=6;
						While_flag=0;
					break;
				case Key_F4:
						if(page==0)
						{
						conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						Sort_set.Unit=3;
						While_flag=0;
						}
					break;
				case Key_F5:
					if(page)
						page=0;
					else
						page=1;
				
					break;
				case Key_Disp:
					SetSystemStatus(SYS_STATUS_TEST);
					While_flag=0;
					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
					While_flag=0;
					Sort_set.Updata_flag=0;
				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='1';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
							
					}
				break;
				case Key_NUM2:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM3:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM4:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
					
				break;
				case Key_NUM5:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM6:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM7:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='7';
						
						Word=Disp_buff[disp_cound]-'0';
						key_count++;
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM8:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM9:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM0:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_DOT:
					if(dot_num==0)
					{
						dot_num1=key_count;
						Disp_buff[key_count]='.';
						dot_num=1;
						key_count++;
						disp_cound++;
					}
					break;
				case Key_BACK:
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;
							
						}
						del_flag=1;
					
					}
//					else
//					{
//						if(Disp_buff[key_count]==0)
//								Disp_buff[key_count]='-';
//							else if(Disp_buff[key_count]=='-')
//								Disp_buff[key_count]='+';
//							else
//								Disp_buff[key_count]='-';
//							key_count++;
//					}
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
			if(disp_flag)
			{
				disp_flag=0;
				Disp_button_Num_Input(page);
				Colour.Fword=White;
				Colour.black=Red;
				if(del_flag)
				{
					PutChar( Coordinates->xpos+(key_count+1)*10, Coordinates->ypos, 
					Disp_buff[key_count], Colour.Fword, Colour.black ); 
					del_flag=0;
					
				}
				else if(key_count>0)				
					PutChar( Coordinates->xpos+key_count*10, Coordinates->ypos, 
					Disp_buff[key_count-1], Colour.Fword, Colour.black ); 
			
			}
				
				
			
			
		}
			
	}
		
		
	return Sort_set;
}
	
	

//数字键输入显示
Sort_TypeDef Disp_NumKeyboard_Set(Disp_Coordinates_Typedef *Coordinates )
{
	uint8_t While_flag=1;
	uint8_t Disp_buff[10]={"         ",};
	uint8_t key,i;
	uint8_t dispflag=1;
	uint8_t dot_num=0,dot_num1=0;
	uint8_t page=0;
	uint32_t keynum=0;
	uint8_t key_count=0;
//	uint32_t Num[6]={1,10,100,1e3,1e4,1e5};
	Sort_TypeDef   Sort_set;
	Sort_set.Dot=0;
	Sort_set.Num=0;
	Sort_set.Unit=0;
	Sort_set.Num=0;
//	for(i=0;i<6;i++)
//	Disp_buff[i]=' ';
	Disp_buff[7]=0;
	
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==5)
		{
			dispflag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
					if(page==0)
						Sort_set.Unit=0;
					else
						Sort_set.Unit=5;
					While_flag=0;
					if(key_count<NUM_LENTH)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
						
					
					Sort_set.Updata_flag=1;
					
				break;
				case Key_F2:
					
					if(page==0)
						Sort_set.Unit=1;
					else
						Sort_set.Unit=6;
					While_flag=0;
					if(key_count<NUM_LENTH)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
					Sort_set.Updata_flag=1;
				break;
				case Key_F3:
					
					if(page==0)
						Sort_set.Unit=2;
					else
						Sort_set.Unit=8;
					While_flag=0;
					if(key_count<NUM_LENTH)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
					Sort_set.Updata_flag=1;
				break;
				case Key_F4:
					
					if(page==0)
						Sort_set.Unit=3;
					else
						Sort_set.Unit=7;
					While_flag=0;
					if(key_count<NUM_LENTH)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
					Sort_set.Updata_flag=1;
				break;
				case Key_F5:
					
					if(page==0)
						page=1;
					else
					page=0;
				break;
				case Key_Disp:
					
					SetSystemStatus(SYS_STATUS_TEST);
					While_flag=0;
					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
					
					While_flag=0;
					Sort_set.Updata_flag=0;
				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
				break;
				case Key_LEFT:
					
				break;
				case Key_RIGHT:
					
				break;
				case Key_UP:
					
				break;
				case Key_DOWN:
					
				break;
				case Key_NUM1:
					
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='1';
						
						
							
						
						key_count++;
							
					}
				break;
				case Key_NUM2:
					
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					 
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					 
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
					
					if(key_count<NUM_LENTH&&key_count>0)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					}
//					else 
//					{
//						if(Disp_buff[key_count]==0)
//							Disp_buff[key_count]='-';
//						else if(Disp_buff[key_count]=='-')
//							Disp_buff[key_count]='+';
//						else
//							Disp_buff[key_count]='-';
//						key_count++;
//							
//					
//					
//					
//					
//					}
				break;
				case Key_BACK:
					
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;							
						}
					
					}
					else
					{
						if(Disp_buff[key_count]==0)
								Disp_buff[key_count]='-';
							else if(Disp_buff[key_count]=='-')
								Disp_buff[key_count]='+';
							else
								Disp_buff[key_count]='-';
							key_count++;
					}
				break;
				case Key_LOCK:
					
				break;
				case Key_BIAS:
					
				break;
				case Key_REST:
					
				break;
				case Key_TRIG:
					
				break;
				default:
					
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
			LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth+5 , Coordinates->ypos+16 , Red );
			Colour.Fword=White;
			Colour.black=Red;
			WriteString_16(Coordinates->xpos, Coordinates->ypos, Disp_buff,  0);
			//dispflag=0;
		}
	
	}
	for(i=key_count;i<NUM_LENTH;i++)
		Disp_buff[i]='0';
	for(i=0;i<NUM_LENTH;i++)
	{
		if(Disp_buff[0]>='0'&&(Disp_buff[0]<='9'))
		{
			if(Disp_buff[i]>='0'&&(Disp_buff[i]<='9'))
			{
			
				if(dot_num1>i)
				{
					Sort_set.Num*=10;
					Sort_set.Num+=Disp_buff[i]-'0';
				
				}
				else
				{
					Sort_set.Num*=10;
					Sort_set.Num+=Disp_buff[i]-'0';				
				
				}
			}
			
			
			//Sort_set.Num+=(Disp_buff[key_count-1]-'0');
		
		
		}
	
	
	
	}
	Sort_set.Dot=dot_num1;
//	if(Disp_buff[0]>='0'&&(Disp_buff[0]<'9'))
//		{
//			if(Disp_buff[key_count-1]!='.')		
//			{
//				Sort_set.Num*=Num[key_count-dot_num-1];
//				Sort_set.Num+=(Disp_buff[key_count-1]-'0');//*Num[key_count-dot_num-1];
//				
//			}				
//			//*(Disp_buff[key_count-1]-'0'))+=Num[key_count-dot_num-1];
//			else
//			{
//			
//			}
//		
//		
//		}
//			
//		else 
//			;//(Disp_buff[key_count-1]-'0')*Sort_set.Num+=Num[key_count-dot_num-2];
	return Sort_set;

}

Sort_TypeDef Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_Num_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Time_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;

}
Sort_TypeDef Disp_Set_CompNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_Num_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Time_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;

}
Sort_TypeDef Disp_Set_InputNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num1;		//Sort_num,
	Disp_button_Num_Input(0);
	Sort_num1=Input_compvalue(Coordinates);
	//if(SaveData.Limit_Tab.Mode==0)
	Sort_num1=Input_Set_Cov(&Sort_num1);
	//else
	//Sort_num1=Input_Set_CovPre(&Sort_num1);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;
	
	

}
Sort_TypeDef Disp_Set_InputpreNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num1;
	Disp_button_Num_Input(0);
	Sort_num1=Input_compvalue(Coordinates);//Input_Set_Cov
	if(SaveData.Limit_Tab.Mode==0)
		Sort_num1=Input_Set_Cov(&Sort_num1);
	else
		Sort_num1=Input_Set_CovPre(&Sort_num1);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;	

}


uint8_t Freq_Set_Num(Disp_Coordinates_Typedef *Coordinates)//频率设置
{
	Sort_TypeDef Sort_num,Sort_num1;
	uint8_t i;
	uint8_t num;
	Disp_button_Num_Freq();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Freq_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
	for(i=4;i<NUM_FREQ+1;i++)
	{
		if(Sort_num1.Num>=FreqNum[i]&&Sort_num1.Num<FreqNum[i+1])
		{
			num=i;
			break;
			
		}
		else
			num=4;
					
	}
		
	return num;
	
}
uint8_t Avg_Set_Num(Disp_Coordinates_Typedef *Coordinates)//平均数设置
{
	Sort_TypeDef Sort_num,Sort_num1;
	uint8_t i;
	uint8_t num;
	Disp_button_Num_Avg();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Freq_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
	if(Sort_num1.Dot==0)
	{
		if(Sort_num1.Num>32)
			Sort_num1.Num=32;
	
	} else
	if(Sort_num1.Dot==1)
	{
		for(i=0;i<5;i++)
		{
			if(Sort_num1.Num>0&&Sort_num1.Num<=9)
				break;
			else
				Sort_num1.Num/=10;
		
		
		}
	
	}else if(Sort_num1.Dot==2)
	{
		for(i=0;i<5;i++)
		{
			if(Sort_num1.Num>10&&Sort_num1.Num<=99)
				break;
			else
				Sort_num1.Num/=10;
		
		
		}
		if(Sort_num1.Num>32)
			Sort_num1.Num=32;
	
	
	
	}else
	{
		Sort_num1.Num=32;
	
	
	}
	num=Sort_num1.Num;
	if(num==0)
		num=1;
		
	return num;
	
}
void Set_daot(uint8_t *buff,uint8_t dot)
{
	uint8_t i;
    if(dot>5)
        dot=0;
	for(i=0;i<dot;i++)
	{
		buff[5-i]=buff[5-i-1];		
	}
	if(dot==0)
		buff[5]=' ';
	else
	buff[5-dot]='.';
	for(i=0;i<6;i++)
	{
		if(buff[i]>0x80)
			buff[i]='0';
	}
	buff[6]=0;


}





uint8_t Test_Comp(All_Compvalue_Typedef *pt)
{
	float value;
	float data;
	uint8_t i,j=0;
	uint8_t num=9;
	for(i=0;i<5;i++)
	{
		if(pt->all[0].buff[i]==' ')
			data=0;
		else
			data=pt->all[0].buff[i];
		value+=(pow(10,4-i)*data);//从下位机接收来的数据
	}
	value=value*pow(1000,pt->all[0].Unit);
	value/=(pow(10,pt->all[0].Dot));
	
//	if(SaveData.Limit_Tab.Mode)//百分比比较
	{
		for(i=0;i<9;i++)
		{
			if(SaveData.Limit_Tab.Comp_Value[i].low.Num!=0)
			{
				if(value>Comp_Testvalue.comp_highvalue[i])//大于上限
				{
					//Count_buff[10]++;
					j=1;
					
					//break;
				}
				else if(value<Comp_Testvalue.comp_lowvalue[i])//小于下限 加合格标志位
									//计数
				{
					//Count_buff[10]++;
					//break;
					j=1;
					
				}
				else 
				{
					Count_buff[i]++;
					num=i;
					i=10;
					
					j=0;
				}
			
			
			}
		
		
		}
		if(j==1)
		{
			Count_buff[10]++;
			Comp_flag=1;
			
		}
		else
			Comp_flag=0;
	
	
	
	}
//	else
//	{
//		
//	
//	
//	}//ABS比较
return num;
}//Comp_Testvalue  Comp_Testvalue
float Inttofloat(Sort_TypeDef *pt)//int转换为float  INT包含小数点和单位
{
	float value;
//	uint8_t i;
	value=pt->Num;
	value=value*pow(1000,pt->Unit);
	value/=pow(10,pt->Dot);
	return value;
}
void Set_Compbcd_float(void)//把设置比较数据转换为float数据  把这个数据与标称值进行运算，得到
	//可以直接比较的值  这个分为2种，一种是ABS一种是%
{
	uint8_t i;
	float value;
	for(i=0;i<9;i++)
	{
		Comp_Testvalue.comp_highvalue[i]=Inttofloat( &SaveData.Limit_Tab.Comp_Value[i].high);
		Comp_Testvalue.comp_lowvalue[i]=Inttofloat( &SaveData.Limit_Tab.Comp_Value[i].low);
	
	
	}
	Comp_Testvalue.comp_highvalue[9]=Inttofloat( &SaveData.Limit_Tab.Comp_Value_2nd.high);
	Comp_Testvalue.comp_lowvalue[9]=Inttofloat( &SaveData.Limit_Tab.Comp_Value_2nd.low);
	value=Inttofloat(&SaveData.Limit_Tab.Nom);
	if(SaveData.Limit_Tab.Mode)//百分比比较
	{
		for(i=0;i<9;i++)
		{
			Comp_Testvalue.comp_highvalue[i]=(100000+Comp_Testvalue.comp_highvalue[i] * 1000)*value;
			Comp_Testvalue.comp_highvalue[i]/=100000;
			Comp_Testvalue.comp_lowvalue[i]=(100000-Comp_Testvalue.comp_lowvalue[i] * 1000)*value;
			Comp_Testvalue.comp_lowvalue[i]/=100000;
			
		}
	
	
	
	}
	else
	{
		
		for(i=0;i<9;i++)
		{
			Comp_Testvalue.comp_highvalue[i]+=value;
			Comp_Testvalue.comp_lowvalue[i]=fabs(value-Comp_Testvalue.comp_lowvalue[i]);
			
		}
		//Comp_Testvalue.comp_highvalue[9]=
		
	
	
	}//ABS比较
	



}
void  Write_Usbdata (volatile uint8_t  *buffer,uint32_t num)
{
    int32_t  fdw;
//    int32_t  fdr;
//    uint32_t  tot_bytes_written;
    uint32_t  bytes_written;



        fdw = FILE_Open(FILENAME_W, RDWR);
        if (fdw > 0) 
		{
			usb_oenflag=1;

			bytes_written = FILE_Write(fdw, buffer, num);//MAX_BUFFER_SIZE);

			FILE_Close(fdw);
            
        } 
		else
			usb_oenflag=0;

    } 

//软件关机程序，放在工具状态里面
void Soft_Turnon(void)
{
	uint32_t i;
	Set_Compbcd_float();
//	ReadSavedata();
//	SetData_High_Low_Comp();
//	Savetoeeprom();
	
	Power_On_led();

	All_LedOff();
	Beep_Off();
    for(i=0xffffff;i>0;i--)
		;
	GPIO_ClearInt(0, 1<<19);	
	while(GetSystemStatus()==SYS_STATUS_TOOL)
	{
		NVIC_EnableIRQ(GPIO_IRQn);
		
//		if(Save_Res.softswitch)
//		{
//			SetSystemStatus(SYS_STATUS_POWER);
//		
//		
//		}
	
	
	}
    NVIC_DisableIRQ(GPIO_IRQn);
	Power_Off_led();
    GPIO_ClearInt(0, 1<<19);
		

}
void Test_Comp_Fmq(void)//Comp_flag==1不合格
{
	if(Comp_flag==0)
		Pass_Led();
	else
		Fail_led();
	if(SaveData.Main_Func.beep==1)//合格讯响
	{
		if(Comp_flag==0)//合格
			Beep_on();
		else
			Beep_Off();
	
	}
	else if(SaveData.Main_Func.beep==2)
	{
		if(Comp_flag==1)//不合格
			Beep_on();
		else
			Beep_Off();
	
	}
	else
		Beep_Off();
		
	


}

uint8_t Uart3_Process(void)
{
//	u8 i;

	u8 kind=0xff;
	u8 data=0;
	u8 str[(FRAME_LEN_MAX-FRAME_LEN_MIN)+1];//收发数据缓冲
//	if(Save_Res.Sys_Setvalue.uart==1)//串口有效
	{
		if (ComBuf3.rec.end)//接收数据结束
		{
            data=1;
			memset(str,'\0',(FRAME_LEN_MAX-FRAME_LEN_MIN+1));//清空缓冲
			{
				//memcpy(str,&ComBuf3.rec.buf[PDATASTART],ComBuf3.send.len-FRAME_LEN_MIN);//数据包
				kind=ComBuf3.rec.buf[PFRAMEKIND];//命令字

			}
			//准备接收下一帧数据sprintf
			ComBuf3.rec.end=0;//接收缓冲可读标志复位
			ComBuf3.rec.ptr=0;//接收指针清零
		}
	}
//	WriteString_Big(0, 150, (uint8_t *)&ComBuf.rec.buf[1]);
	ComBuf3.rec.end=0;

	switch(kind)
	{
		case FRAME_READ_RESULT://读取结果
			//串口发送测试数据:电压(5)+电阻(6)+时间(4)+分选(1)=16字节
	
			ComBuf3.send.buf[1+5+6+4]=kind;
			ComBuf3.send.begin=0;
			ComBuf3.send.len=PackStandFrame(ComBuf3.send.buf , &ComBuf3.send.buf[1] , 16  );
//			if(SendDataToCom()==0)//发送成功判别
//			{
//			//	Delay_1ms(100);//延时
//			//	SendDataToCom();//发送
//			}
			break;
		
		case FRAME_START://启动
			SetSystemStatus(SYS_STATUS_TEST);//系统状态-启动测试
			break;

		case FRAME_RESET://复位
			//SetSystemStatus(SYS_STATUS_IDLE);//系统状态-待机
			break;
        case 0x10://设置参数
            
            break;
        case 0x11://设置频率
            break;
        case 0x12://设置电平
            break;
        case 0x13://设置显示  读
            break;
        case 0x14://设置量程
            break;
        case 0x15://设置清零
            break;
        case 0x16://设置讯响
            break;
        case 0x17://设置内阻
            break;
        case 0x18://设置分选
            break;
        case 0x19://设置速度
            break;
        case 0x1a://设置触发方式
            break;
        case 0x1b://设置DQ上下限
            break;
        case 0x2c://设置档1
            break;
        case 0x2d://设置档2
            break;
        case 0x2e://设置档3
            break;
        case 0x2f://设置档4
            break;
        case 0x30://设置档5
            break;
        case 0x31://设置档6
            break;
        case 0x32://设置档7
            break;
        case 0x33://设置档8
            break;
        case 0x34://设置档9
            break;
        case 0x20://设置标称值
            break;
        
        
        //读取设置值
        case 0x51://读取参数
            break;
        case 0x52://读取频率
            break;
        case 0x53://读取电平
            break;
        case 0x54://读取显示
            break;
        case 0x55://读取量程
            break;
        case 0x56://读取清零
            break;
        case 0x57://读取讯响
            break;
        case 0x58://读取内阻
            break;
        case 0x59://读取分选
            break;
        case 0x5a://读取速度
            break;
        case 0x5b://读取触发方式
            break;
        case 0x5c://读取D/Q值
            break;
        case 0x5d://读取档1值
            break;
        case 0x5e://读取档2值
            break;
        case 0x5f://读取档3值
            break;
        case 0x61://读取标称值
            break;
        case 0x62://读取档4值
            break;
        case 0x63://读取档5值
            break;
        case 0x64://读取档6值
            break;
        case 0x65://读取档7值
            break;
        case 0x66://读取档8值
            break;
        case 0x67://读取档9值
            break;
        
        
//        case 0x2d://设置档2
//            break;
        
		case FRAME_WRITE_SN://写序列号
			break;
		
		case FRAME_CLR_BOOT_NUM://清开机次数
			break;
		
		case FRAME_DATA://数据帧
			break;

		default:
			break;
	}
	return data;

}


void input_password(Disp_Coordinates_Typedef *Coordinates )
{
  	char While_flag=1;
	char Disp_buff[10];
	u8 key,i;
	char dispflag=1;
	char dot_num=0,dot_num1=0;
    char password_flag=0;
//	vu8 page=0;
	uint32_t keynum=0;
	char  key_count=0;
//	vu32 Num[6]={1,10,100,1e3,1e4,1e5};
//	Sort_TypeDef   Sort_set;
//	Sort_set.Dot=0;
//	Sort_set.Num=0;
//	Sort_set.Unit=0;
//	Sort_set.Num=0;
	for(i=0;i<9;i++)
	Disp_buff[i]=0;
	//Disp_buff[8]=0;
	Disp_Button_value1(2);
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
            password_flag=0;
            Key_Beep();
			dispflag=1;
			switch(key)
			{
				case Key_F1://退出
					 While_flag=0;
                       SetSystemStatus(SYS_STATUS_TEST);

					
				break;
				case Key_F3://取消
                    
					
				break;
				case Key_F2://确认
                   if(strcmp(PASSWORD,Disp_buff))//比较函数  当相等时  返回0
                   {//密码错误
                       key_count=0;
                       for(i=0;i<8;i++)
                        Disp_buff[i]=' ';
                       password_flag=1;
                       
                   
                   
                   }
                   else //密码正确
                   {
                        While_flag=0;
                       SetSystemStatus(SYS_STATUS_FACRDEBUG);
                   
                   }
                       
					
				break;
				case Key_F4:
					
				break;
				case Key_F5:
					
				break;
				case Key_Disp:
//					SetSystemStatus(SYS_STATUS_TEST);
//					While_flag=0;
//					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
//					While_flag=0;
//					Sort_set.Updata_flag=0;
//				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='1';
						
						
							
						
						key_count++;
							
					}
				break;
				case Key_NUM2:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
					
					if(key_count<PASSWORD_LENTH&&key_count>0)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					}
//					else 
//					{
//						if(Disp_buff[key_count]==0)
//							Disp_buff[key_count]='-';
//						else if(Disp_buff[key_count]=='-')
//							Disp_buff[key_count]='+';
//						else
//							Disp_buff[key_count]='-';
//						key_count++;
//							
//					
//					
//					
//					
//					}
				break;
				case Key_BACK:
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;
							
						}
					
					}
					else
					{
						if(Disp_buff[key_count]==0)
								Disp_buff[key_count]='-';
							else if(Disp_buff[key_count]=='-')
								Disp_buff[key_count]='+';
							else
								Disp_buff[key_count]='-';
							key_count++;
					}
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
            LCD_DrawLine( Coordinates->xpos+13, Coordinates->ypos-2, Coordinates->xpos+13+80, Coordinates->ypos-2 , Red );
            LCD_DrawLine( Coordinates->xpos+13, Coordinates->ypos+18, Coordinates->xpos+13+80, Coordinates->ypos+18 , Red );
            for(i=0;i<9;i++)
            LCD_DrawLine( Coordinates->xpos+13+i*10, Coordinates->ypos-2, Coordinates->xpos+13+i*10, Coordinates->ypos+18 , Red );
            
			//LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , Coordinates->ypos+16 , Red );
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BACK;
          //  WriteString_16(Coordinates->xpos-38, Coordinates->ypos, "密码:",  0);
			WriteString_16(Coordinates->xpos-38, Coordinates->ypos, " PWD:",  0);
			WriteString_16(Coordinates->xpos+15, Coordinates->ypos, ( uint8_t *)Disp_buff,  0);
            if(password_flag)
                WriteString_16(Coordinates->xpos, Coordinates->ypos+20, "PwdERR",  0);
            else
               LCD_DrawRect( Coordinates->xpos, Coordinates->ypos+20,Coordinates->xpos+100 , Coordinates->ypos+40, LCD_COLOR_TEST_BACK );
               // WriteString_16(Coordinates->xpos, Coordinates->ypos, "         ",  0);
           // WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "请输入8位密码",  0);
						WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "8bit Password",  0);
			//dispflag=0;
		}
	
	}

}
void input_num(Disp_Coordinates_Typedef *Coordinates )
{
  	char While_flag=1;
	uint8_t Disp_buff[12];
	uint8_t  key,i;
	char dispflag=1;
//	char dot_num=0,dot_num1=0;
    char password_flag=0;
	char keynum=0;
	char key_count=0;
//	Sort_TypeDef   Sort_set;
	
	for(i=0;i<8;i++)
	Disp_buff[i]=' ';
	Disp_buff[8]=0;
	
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
            password_flag=0;
            Key_Beep();
			dispflag=1;
			switch(key)
			{
				case Key_F1://退出
					 
                       //SetSystemStatus(SYS_STATUS_USERDEBUG);

					
				break;
				case Key_F2://取消
                    
					
				break;
				case Key_F3://确认保存机号和日期
//                   if(strcmp(PASSWORD,Disp_buff))//比较函数  当相等时  返回0
//                   {//密码错误
//                       key_count=0;
//                       for(i=0;i<8;i++)
//                        Disp_buff[i]=' ';
//                       password_flag=1;
//                       
//                   
//                   
//                   }
//                   else //密码正确
//                   {
//                        While_flag=0;
//                       SetSystemStatus(SYS_STATUS_USERDEBUG);
//                   
//                   }//复制到内存
                       
					
				break;
				case Key_F4:
					
				break;
				case Key_F5:
                   //return  &Disp_buff[0];
					
				break;
				case Key_Disp:
//					SetSystemStatus(SYS_STATUS_TEST);
//					While_flag=0;
//					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
//					While_flag=0;
//					Sort_set.Updata_flag=0;
//				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
                    While_flag=0;//保存
                    dispflag=0;
                    for(i=0;i<8;i++)
                    {
                        Saveeeprom.fac_num[i]=Disp_buff[i];
                    
                    }
                    Savetoeeprom();
                     LCD_DrawRect( 0, Coordinates->ypos-20,Coordinates->xpos+200 , Coordinates->ypos+20, LCD_COLOR_TEST_BACK );
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='1';
						key_count++;
							
					}
				break;
				case Key_NUM2:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
                    if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='/';
						key_count++;
					}
					
//					if(key_count<NUM_LENTH&&key_count>0)
//					{
//						if(dot_num==0)
//						{
//							if(key_count>0)
//							{
//								Disp_buff[key_count]='.';
//								dot_num1=key_count;
//								key_count++;
//							
//							
//							}
//							dot_num++;
//						}
//					}
//					else 
//					{
//						if(Disp_buff[key_count]==0)
//							Disp_buff[key_count]='-';
//						else if(Disp_buff[key_count]=='-')
//							Disp_buff[key_count]='+';
//						else
//							Disp_buff[key_count]='-';
//						key_count++;
//							
//					
//					
//					
//					
//					}
				break;
				case Key_BACK:
					if(key_count)
					{	key_count--;
						Disp_buff[key_count]=' ';
//						if(dot_num1==key_count)
//						{
//							dot_num=0;
//							dot_num1=0;
//							
//						}
					
					}
//					else
//					{
//						if(Disp_buff[key_count]==0)
//								Disp_buff[key_count]='-';
//							else if(Disp_buff[key_count]=='-')
//								Disp_buff[key_count]='+';
//							else
//								Disp_buff[key_count]='-';
//							key_count++;
//					}
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
            LCD_DrawLine( Coordinates->xpos+13, Coordinates->ypos-2, Coordinates->xpos+13+80, Coordinates->ypos-2 , Red );
            LCD_DrawLine( Coordinates->xpos+13, Coordinates->ypos+18, Coordinates->xpos+13+80, Coordinates->ypos+18 , Red );
            for(i=0;i<9;i++)
            LCD_DrawLine( Coordinates->xpos+13+i*10, Coordinates->ypos-2, Coordinates->xpos+13+i*10, Coordinates->ypos+18 , Red );
            
			//LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , Coordinates->ypos+16 , Red );
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BACK;
            WriteString_16(Coordinates->xpos-70, Coordinates->ypos, "Fact No.:",  0);
			WriteString_16(Coordinates->xpos+15, Coordinates->ypos, ( uint8_t *)Disp_buff,  0);
//            if(password_flag)
//                WriteString_16(Coordinates->xpos, Coordinates->ypos+20, "密码错误",  0);
//            else
//               LCD_DrawRect( Coordinates->xpos, Coordinates->ypos+20,Coordinates->xpos+100 , Coordinates->ypos+40, LCD_COLOR_TEST_BACK );
               // WriteString_16(Coordinates->xpos, Coordinates->ypos, "         ",  0);
          //  WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "请输入8位出厂编号",  0);
			WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "Enter 8 bits No. ",  0);
		}
	
	}

}



