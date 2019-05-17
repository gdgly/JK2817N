#include "LCD/lpc177x_8x_lcd.h"
#include"EX_SDRAM/EX_SDRAM.H"
#include "LCD/AsciiLib.h"
#include "use_disp.h"
#include  "stdint.h"
#include  "stdio.h"
#include "Key/key.h"
#include "test/Test.h"
#include  "Globalvalue/globalvalue.h"
#include "lpc177x_8x_eeprom.h"
#include "timer.h"
#include <math.h>
#include "debug_frmwrk.h"
#include "rtc.h"
#include "lpc177x_8x_rtc.h"


uint8_t CorrectionflagC=0,CorrectionflagR=0,Correc_successflag=0;
const uint8_t Num_1[][9]=
{"1","2","3","4","5","6","7","8","9"};
const uint8_t Test_Setitem_E[][9+1]=
{
	{"Func    :"},
	{"Freq    :"},
	{"ELevel  :"},
	{"MRange  :"},
	{"Speed:"},
//    {"ƫ��    :"},
	
	{"Vm:"},
	{"Im:"},
	{"Corr:"}
	
};

const uint8_t Test_Setitem[][9+1]=
{
	{"����    :"},
	{"Ƶ��    :"},
	{"��ƽ    :"},
	{"����    :"},
	{"�ٶ�    :"},
//    {"ƫ��    :"},
	
	{"Vm:"},
	{"Im:"},
	{"У��:"}
	
};

const uint8_t Button_TipPage1[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"Cp-.."},
    {"Cs-.."},
    {"Lp-.."},
    {"Ls-.."},
    {" "},


};
const uint8_t Button_TipPage2[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"Z-.."},
    {"Y-.."},
    {"R-.."},
    {"G-B"},
    {" "},


};
const uint8_t Button_TipPage3[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"DCR"},
    {" "},
    {" "},
    {""},
    {" "},


};
const uint8_t Cp_Button_Tip[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"Cp-D"},
    {"Cp-Q"},
    {"Cp-G"},
    {"Cp-Rp"},
    {"RETURN"},


};
const uint8_t Cs_Button_Tip[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"Cs-D"},
    {"Cs-Q"},
    {"Cs-Rs"},
    {"    "},
    {"RETURN"},


};
const uint8_t Lp_Button_Tip1[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"Lp-Q"},
	{"Lp-Rp"},
    {"Lp-Rd"}, 
	{"Lp-D"},
    {"RETURN"},


};
const uint8_t Lp_Button_Tip2[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"Lp-D"},
    {"Lp-G"},
    {"    "},
    {"    "},
    {"RETURN"},


};
const uint8_t Ls_Button_Tip[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"Ls-D"},
    {"Ls-Q"},	
    {"Ls-Rs"},
    {"Ls-Rd"},
    {"RETURN"},


};
const uint8_t Z_Button_Tip[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"Z-d"},
    {"Z-r"},
    {" "},
    {" "},
    {"RETURN"},


};

const uint8_t Y_Button_Tip[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"Y-d"},
    {"Y-r"},
    {" "},
    {" "},
    {"RETURN"},


};
const uint8_t R_Button_Tip[][7+1]=  //���Բ���ѡ��ʱ����������ʾ����
{
    {"R-X"},
    {"Rp-Q"},
    {"Rs-Q"},
    {" "},
    {"RETURN"},


};
const uint8_t User_FUNC[][8+1]=
{
	{"Cp-D"},
	{"Cp-Q"},
    {"Cp-G"},
	{"Cp-Rp"},
	{"Cs-D"},
	{"Cs-Q"},
    {"Cs-Rs"},
    {"Lp-Q"},//7
    {"Lp-Rp"},
    {"Lp-Rd"},
    {"Lp-D"},
//    {"Lp-G"},//10
    {"Ls-D"},
    {"Ls-Q"},
    {"Ls-Rs"},
    {"Ls_Rd"},//14
    {"Z-d"},
    {"Z-r"},
    {"Y-d"},
    {"Y-r"},//18
	{"R-X"},
	{"Rp-Q"},
	{"Rs-Q"},
    {"G-B"},
    {"DCR"},
};
const uint8_t RangeDisp_Main[][3+1]=
{
	{"Cp:"},{"Cp:"},{"Cp:"},{"Cp:"},
	{"Cs:"},{"Cs:"},{"Cs:"},
	{"Lp:"},{"Lp:"},{"Lp:"},{"Lp:"},
	{"Ls:"},{"Ls:"},{"Ls:"},{"Ls:"},
	{"Z :"},{"Z :"},
	{"Y :"},{"Y :"},
	{"R :"},{"Rp:"},{"Rs:"},
	{"G :"}
	
};
const uint8_t RangeDisp_Second[][3+1]=
{
	{"D :"},{"Q :"},{"G :"},{"Rp:"},
	{"D :"},{"Q :"},{"Rs:"},
	{"Q :"},{"Rp:"},{"Rd:"},{"D :"},
	{"D :"},{"Q :"},{"Rs:"},{"Rd:"},
	{"r :"},{"r :"},
	{"r :"},{"r :"},
	{"X :"},{"Q :"},{"Q :"},
	{"B :"}
};
const uint8_t Fun_SelectValue[4][7]=
{
	{0,4,	7,	11,	15,	17,	19},
	{1,5,	8,	12,	16,	18,	20},
	{2,6,	9,	13,	0xff,0xff,21},
	{3,0xff,10,	14,	0xff,0xff,0xff}


};
const uint32_t FreqNum[]=
	{20,25,30,40,50,60,75,100,120,150,200,250,300,400,500,600,750,
	1e3,1200,1500,2e3,2500,3e3,4e3,5e3,6e3,7500,
	1e4,12e3,15e3,2e4,25e3,3e4,4e4,5e4,6e4,75e3,
	1e5};
//const uint32_t FreqNum[]=
//	{20,25,30,40,50,60,75,100,120,150,200,250,300,400,500,600,750,
//	1000,1200,1500,2000,2500,3000,4000,5000,6000,7500,
//	10000,12000,15000,20000,25000,30000,40000,50000,60000,75000,
//	100000,120000};
const uint8_t User_Freq[][10+1]=
{
	{"20.000 Hz"},
	{"25.000 Hz"},
	{"30.000 Hz"},
	{"40.000 Hz"},
    {"50.000 Hz"},
    {"60.000 Hz"},
    {"75.000 Hz"},
    {"100.00 Hz"},
    {"120.00 Hz"},
    {"150.00 Hz"},
    {"200.00 Hz"},
    {"250.00 Hz"},
    {"300.00 Hz"},
    {"400.00 Hz"},
    {"500.00 Hz"},
    {"600.00 Hz"},
    {"750.00 Hz"},
    {"1.0000KHz"},
    {"1.2000KHz"},
    {"1.5000KHz"},
    {"2.0000KHz"},
    {"2.5000KHz"},
    {"3.0000KHz"},
    {"4.0000KHz"},
    {"5.0000KHz"},
    {"6.0000KHz"},
    {"7.5000KHz"},
    {"10.000KHz"},
    {"12.000KHz"},
    {"15.000KHz"},
    {"20.000KHz"},
    {"25.000KHz"},
    {"30.000KHz"},
    {"40.000KHz"},
    {"50.000KHz"},
    {"60.000KHz"},
    {"75.000KHz"},
    {"100.00KHz"},
	{"120.00KHz"},
	{"150.00KHz"},
	{"200.00KHz"},


};
const uint8_t Setup_Beep_E[][6+1]=
{
	"Shut",
	"Good",
	"NoGood",

};
const uint8_t Setup_Beep[][6+1]=
{
	"�ر�",
	"�ϸ�",
	"���ϸ�",

};
const uint8_t *Setup_Valueall[]=
{
	*User_FUNC,
	*User_Freq,



};
const uint8_t FreqButton_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"DECR --"},
    {"DECR -"},
    {"INCR +"},
    {"INCR ++"},
	{" "}

};
const uint8_t User_Level[][10+1]=
{
    {"0.100 V"},
    {"0.300 V"},
	{"1.000 V"},

};
const uint8_t LevleButton_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"DECR -"},
    {"INCR +"},

};

const uint8_t User_Range[][10+1]=
{
    {"AUTO "},
    {"10 �� "},
    {"100�� "},
    {"1 k�� "},
    {"10k�� "},
    {"100k��"},
	{"HOLD"},

};
const uint8_t RangeButton_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"AUTO"},
    {"HOLD"},
    {"DECR -"},
    {"INCR +"},

};
const uint8_t User_Biad[][10+1]=
{
    {"---- "},
   

};
const uint8_t BiasButton_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"DECR -"},
    {"INCR +"},

};
const uint8_t User_Speed[][10+1]=
{
    {"FAST "},
    {"MED "},
    {"SLOW "},
   
};
const uint8_t SpeedButton_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"FAST "},
    {"MED "},
    {"SLOW "},
};
const uint8_t User_Comp[][10+1]=
{
    
    {"OFF"},
	{"ON"},
   
};
const uint8_t Limit_Scan_Fun[][4+1]=
{
	{"SEQ"},
	{"STEP"},

};
const uint8_t Limit_Scan_Page[][6+1]=
{
	{"Last P"},
	{"Next P"},

};
	
const uint8_t Sys_Bais_Value[][6+1]=
{
	{"INT"},
	{"OUT"}


};
const uint8_t Sys_Addr_value[][6+1]=
{
	{"RS232C"},
	{"GPIB"},
	{"USBTMC"},
	{"USBCDC"},
	{"RS485"},


};
const uint8_t Switch_Value[][4+1]=
{
	{"OFF"},
	{"ON"}

};
const uint8_t CompButton_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"ON"},
	{"OFF "},
};
const uint8_t User_ListMode[][10+1]=
{
    {"SEQ"},
    {"STEP "},
   
};
const uint8_t ListModeButton_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"SEQ "},
    {"SETP "}
};
const uint8_t Setup_Trig[][10+1]=
{
    {"INT "},
    {"MAN "},
    {"EXT "},
    {"BUS "},
   
};
const uint8_t SetupTrig_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"INT "},
    {"MAN "},
    {"EXT "},
    {"BUS "},
};
const uint8_t Setup_Alc[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t SetupAlc_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Setup_Rsou[][10+1]=
{
    {"100��"},
    {"30��"},
   
};
const uint8_t SetupRsou_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"100��"},
    {"30��"},
};
const uint8_t SetupTrigDly_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"DECR -"},
    {"INCR +"},
};
const uint8_t Setup_Rev_A[][10+1]=
{
    {"ABS  "},
    {"%   "},
    {"OFF   "},
   
};
const uint8_t SetupRev_A_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"ABS"},
    {"%  "},
    {"OFF"},
};
const uint8_t Setup_Rev_B[][10+1]=
{
    {"ABS  "},
    {"%   "},
    {"OFF   "},
   
};
const uint8_t SetupRev_B_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"ABS"},
    {"%  "},
    {"OFF"},
};
const uint8_t SetupAvg_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"DECR -"},
    {"INCR +"},
};
const uint8_t Setup_Vm[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t SetupVm_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Setup_Dcr[][10+1]=
{
    {"FIX  "},
    {"ALT "},
   
};
const uint8_t SetupDcr_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"FIX  "},
    {"ALT "},
};
const uint8_t SetupRefa_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"MEASURE"},
};
const uint8_t SetupRefB_Button_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
{
    {"MEASURE"},
};

const uint8_t Correct_Open[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t CorrectOpen_Button_Tip[][7+1]=  //�ȽϽ���ѡ��ʱ����������ʾ����
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Correct_Short[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t CorrectShort_Button_Tip[][7+1]=  //�ȽϽ���ѡ��ʱ����������ʾ����
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Correct_Load[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t CorrectLoad_Button_Tip[][7+1]=  //�ȽϽ���ѡ��ʱ����������ʾ����
{
    {"ON  "},
    {"OFF "},
};
const uint8_t CorrectSpot_Button_Tip[][7+1]=  //�ȽϽ���ѡ��ʱ����������ʾ����
{
    {"DECR --"},
    {"DECR -"},
    {"INCR --"},
    {"INCR ++"}
};
const uint8_t Correct_Freq[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t CorrectFreq_Button_Tip[][7+1]=  //�ȽϽ���ѡ��ʱ����������ʾ����
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Correct_Cable[][10+1]=
{
    {"0m  "},
    {"1m "},
    {"2m  "},
    {"4m "},
   
};
const uint8_t CorrectCable_Button_Tip[][7+1]=  //�ȽϽ���ѡ��ʱ����������ʾ����
{
    {"0m "},
    {"1m "},
    {"2m "},
    {"4m "},
};
//const uint8_t User_ListNo[][10+1]=
//{
//    {"SEQ"},
//    {"STEP "},
//   
//};
//const uint8_t ListNoButton_Tip[][7+1]=  //Ƶ��ѡ��ʱ����������ʾ����
//{
//    {"SEQ "},
//    {"SETP "}
//};
//const uint8_t Test_Setitem_E[][9+1]=
//{
//	{"FUNC    :"},
//	{"FREQ    :"},
//	{"LEVEL   :"},
//	{"RANGE   :"},
//    {"BIAS    :"},
//	{"SPEED   :"},
//	
//	{"Vm:"},
//	{"Im:"},
//	{"CORR:"}
//	
//};
const uint8_t  Range_Item[][7+1]=
{
	{"����  :"},
	{"Ƶ��  :"},
	{"��ƽ  :"},
	{"����  :"},
//	{"ƫ��  :"},
    {"�ٶ�  :"},
    {"�Ƚ�  :"},
};
const uint8_t  Range_Item_E[][7+1]=
{
	{"FUNC  :"},
	{"FREQ  :"},
	{"LEVEL :"},
	{"RANGE :"},
//	{"BIAS  :"},
	{"SPEED :"},
	{"COMP  :"}
};
//const uint8_t	Set_testitem[][9+1]=
//{
//	{"����    :"},
//	{"Ƶ��    :"},
//	{"��ƽ    :"},
//	{"����    :"},
//	{"���ƽ  :"},
//	{"����    :"},
//	{"������ʱ:"},
//	{"������ʱ:"},
//	{"Ѷ�칦��:"},
//	{"����    :"},
//	{"�ٶ�    :"},
//	{"ƫ��    :"},
//	{"ƽ��    :"},
//	{"V/I���� :"},
//	
//	{"DCR���� :"},
//	{"DC����  :"},
//	{"DC��ƽ  :"},
//    {"������  :"},
//};

const uint8_t	Set_testitem[][9+1]=
{
	{"����    "},
	{"Ƶ��    "},
	{"��ƽ    "},
	{"����    "},
//	{"���ƽ  "},
	{"����    "},
//	{"������ʱ"},
//	{"������ʱ"},
	{"Ѷ�칦��"},
	{"����    "},
	{"�ٶ�    "},
//	{"ƫ��    "},
//	{"ƽ��    "},
	{"V/I���� "},
	
//	{"DCR���� "},
//	{"DC����  "},
//	{"DC��ƽ  "},
    {"������  "},
};

const uint8_t	Set_testitem_E[][9+1]=
{
	{"FUNC    :"},
	{"FREQ    :"},
	{"LEVEL   :"},
	{"TRIG    :"},
//	{"ALC     :"},
	{"Rsou    :"},
//	{"TRIG DLY:"},
	//{"STEP DLY:"},
	{"DEV A   :"},
	{"RANGE   :"},
 //   {"BIAS    :"},
	{"SPEED   :"},
	
	//{"AVG     :"},
	{"Vm/Im   :"},
//	{"DCR POL :"},
//	{"DCR RNG :"},
//	{"DC LEV  :"},
	{"REF A   :"}
};
//const uint8_t Sys_Setitem_E[][10+1]=
//{
//	{"�Ǳ����� :"},
//	{"�ϸ�Ѷ�� :"},
//	{"����Ѷ�� :"},
//	{"��ʾ���� :"},
//	{"����     :"},
//	{"�˵����� :"},
//	{"����     :"},
//	{"������   :"},
//	{"ƫ��Դ   :"},
//	{"���߷�ʽ :"},
//	{"GPIB��ַ :"},
//	{"ֻ��     :"},
//	{"ʱ��     :"}
//};


const uint8_t Sys_Setitem[][10+1]=
{
	{"�Ǳ����� "},
	{"�������� "},
	{"U�̿��� "},
	{"��ʾ���� "},
//	{"����     "},
	{"�˵����� "},
	{"����     "},
//	{"����     "},
	{"������   "},
	{"ƫ��Դ   "},
	{"���߷�ʽ "},
	{"GPIB��ַ "},
	{"ֻ��     "},
	{"ʱ��     "}
};
const uint8_t Sys_Setitem_E[][10+1]=
{
	{"MAIN FUNC:"},
	{"PASS BEEP:"},
//	{"FAIL BEEP:"},
	{"U  SWITCH:"},
	{"LANGUAGE :"},
	//{"PASS WORD:"},
	{"MENU DISP:"},
	{"DATA     :"},
	{"BAUD RATE:"},
	{"BIAS SRC :"},
	{"BUS MODE :"},
	{"GPIBADDR :"},
	{"TALK ONLY:"},
	{"TIME     :"}
};
const uint8_t All_TopName[][21+1]=
{
	{"< ������ʾ >"},
	{"< ������ʾ >"},
	{"< ��������ʾ >"},
	{"< �б�ɨ����ʾ >"},
	{"< �������� >"},
	{"< �û�У�� >"},
	{"< �����б����� >"},
	{"< �б�ɨ������ >"},
	{"< ϵͳ���� >"},
	{"[ LCR�ļ��б� ]"},
	{"< У������ >"},

};
const uint8_t All_TopName_E[][21+1]=
{
	{"< MEAS DISPLAY >"},
	{"< BIN No. DISP >"},
	{"< BIN COUNT DISP >"},
	{"< LIST SWEEP DISP >"},
	{"< MEASURE SETUP >"},
	{"< CORRECTION >"},
	{"< LIMIT TABLE SETUP >"},
	{"< LIST SWEEP SETUP >"},
	{"< SYSTEM SETUP >"},
	{"[ LCR FILE LIST ]"},
	{"< CORRECTION SET >"},

};
const uint8_t Range_Count_Item[][6+1]=
{
	{"���� :"},
	{"��� :"},
	{"���� :"}

};
const uint8_t Range_Count_Item_E[][6+1]=
{
	{"PARAM :"},
	{"NOM.  :"},
	{"COUNT :"}

};

const uint8_t User_Check_Item[][12+1]=
{
	{""},
	{""},
//	{"��·  :"},
//	{"��·  :"},
//	{"����  :"},
//	{"У����   :"},
//	{"Ƶ��     :"},
//	{" �ο�   A:"},
//	{" ��·   A:"},
//	{" ��·   A:"},
//	{" ����   A:"},
//	{"����  :"},
//	{"��ʽ  :"},
//	{"����  :"},
//	{"       "},
//	{"  B:"},
//	{"  B:"},
//	{"       "},
//	{"  B:"},
//	{"  B:"}

};


const uint8_t User_Check_Opencircuit[][7+1]=	//��·
{
	{"OFF"},
	{"ON"}
};
const uint8_t DEBUG_Check_Opencircuit[][7+1]=	//��·
{
	{"��ʼ"},
	{"     "}
};
const uint8_t DEBUG_Check_Opencircuit_E[][7+1]=	//��·
{
	{"STAR"},
	{"     "}
};
const uint8_t User_Check_Shortcircuit[][7+1]=	//��·
{
	{"OFF"},
	{"ON"}
};

const uint8_t User_Check_Item_E[][12+1]=
{
	{"OPEN  :"},
	{"SHORT :"},
//	{"LOAD  :"},
//	{"SPOT No.  :"},
//	{"FREQ      :"},
//	{" REF     A:"},
//	{" OPEN    A:"},
//	{" SHORT   A:"},
//	{" LOAD:   A"},
//	{"CABLE :"},
//	{"MODE  :"},
//	{"FUNC  :"},
//	{"       "},
//	{"  B:"},
//	{"  B:"},
//	{"       "},
//	{"  B:"},
//	{"  B:"}



};
const uint8_t User_LimitList_Item[][7+1]=
{
 	{"����  :"},
	{"���  :"},
	{"��ʽ  :"},
	{"����  :"},
	{"�Ƚ�  :"},
	{""},


};
const uint8_t User_LimitList_Value[][3+1]=
{
	
	"ABS",
	"%",
};
const uint8_t User_LimitList_Item_E[][7+1]=
{
 	{"PARAM :"},
	{"NOM   :"},
	{"MODE  :"},
	{"AUX   :"},
	{"COMP  :"},
	{""},


};
const uint8_t User_LimitList_Item2[][3+1]=
{
	{"BIN"},
	{" 1"},
	{" 2"},
	{" 3"},
	{" 4"},
	{" 5"},
	{" 6"},
	{" 7"},
	{" 8"},
	{" 9"},
	{"2nd"}


};
const uint8_t User_LimitScan_Item2[][3+1]=
{
	{"No."},
	{"001"},
	{"002"},
	{"003"},
	{"004"},
	{"005"},
	{"006"},
	{"007"},
	{"008"},
	{"009"},
	{"010"}


};
const uint8_t User_ListScan_Item[][8+1]=
{
	{"ģʽ :"},
	{"Ƶ��[Hz]"},
    {"��ƽ[V]"},
	{"LMT"},
	{"����"},
	{"����"},
    {"��ʱ[s]"},

};
const uint8_t User_ListScan_Item_E[][8+1]=
{
	{"MODE :"},
	{"FREQ[Hz]"},
    {"LEVEL[V]"},
	{"LMT"},
	{"LOW "},
	{"HIGH"},
    {"DELY[s]"},

};
const uint8_t Sys_Language_Value[][7+1]=
{
	{"����"},
	{"ENGLISH"}

};
const uint8_t Sys_Buard_Value[][6+1]=
{
	{"SHUT"},
	{"4800"},
	{"9600"},
	{"19200"},
	{"38400"},
	{"56000"},
	{"115200"},

};
const uint8_t Set_Unit[][5+1]=
{
	{"p"},
	{"n"},
	{"u"},
	{"m"},
	{"next"},
	{"*1"},
	{"k"},
	{"M"},
	
	{""},
	{"back"},



};
const uint8_t Disp_Unit[][2+1]=
{"p","n","u","m"," ","k","M"};

const uint8_t Disp_Range_Main_NUm[]={0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,3,3,3,2,2,1,1};
const uint8_t Disp_Range_Main_Disp[][2+1]=
{	"F ",
	"H ",
	"��",
	"S ",
	"r ",
	"��",
	"  "
};//F,H,����S r ��ո�
uint32_t const Main_Func_Comp[][2]=                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
{
	{0,21},
	{0,33},
	{0,1},
	{0,3},
	{0,0},
	{0,0},
	{0,60000},
	{0,60000},
	{0,2},
	{0,2},
	{0,7},//����
	{0,0},
	{0,2},
	{0,16},
	{0,1},
	{0,0},
	{0,0},
	{0,0},
	{0,999999},
	{0,5},
	{0,7},
	{0,999999},
	{0,5},
	{0,7}


};

const uint8_t Correction_Capacitance[][7+1]=			//У������
{
	{"100uF"},
	{"10uF"},
	{"1uF"},
	{"100nF"},
	{"10nF"},
	{"1nF"},
	{"100pF"},
	{"10pF"},
};

const uint8_t Correction_Resist[][7+1]=			//У������
{
	{"10��"},
	{"100��"},
	{"1K��"},
	{"10K��"},
	{"100K��"},
};

//��ʾ ����Ŀ�ݼ�����
void Disp_Fastbutton(void)
{
	uint32_t i;
	for(i=0;i<6;i++)
	{
		LCD_DrawRect( 4*i+i*76, 228, 76+i*80, 271 , LCD_COLOR_TEST_BUTON ) ;
		LCD_DrawLine( 80*i+1, 227, 76+i*80, 227 , LCD_COLOR_TEST_LINE );
		//if(i<5)
		LCD_DrawLine( 76+i*80, 227, 76+i*80, 271 , LCD_COLOR_TEST_LINE );
		LCD_DrawLine( 76+i*80+1, 227, 76+i*80+1, 271 , Black );
		LCD_DrawLine( 76+i*80+2, 227, 76+i*80+2, 271 , Black );
		LCD_DrawLine( 76+i*80+3, 227, 76+i*80+3, 271 , Black );
		
	}

}//118





void Disp_button_Num_time(void)
	
{
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(84, 271-30, "msec",  0);
	WriteString_16(84+80, 271-30, "sec",  0);


}

//������������n,u,p,m,next,
void Disp_button_Num_Input(uint8_t page)    
	
{
	uint8_t i;
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;//
	if(page==0)
	{
		for(i=0;i<5;i++)
		{ 
			if(i==4)
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, Set_Unit[i],  0);
			else 
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+29, BUTTOM_Y_VALUE, Set_Unit[i],  0);
		}
	}
	else
	{
		for(i=0;i<5;i++)
		{
			if(i==4)
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, Set_Unit[i+5],  0);
			else if(i==0)
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+24, BUTTOM_Y_VALUE, Set_Unit[i+5],  0);
			else
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+29, BUTTOM_Y_VALUE, Set_Unit[i+5],  0);
		}
	}
	
//	WriteString_16(84+80, 271-30, "sec",  0);


}
void Disp_button_Num_Freq(void)
	
{
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(84, 271-30, "Hz",  0);
	WriteString_16(84+80, 271-30, "KHz",  0);


}
void Disp_button_Num_Avg(void)
	
{
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(84, 271-30, "*1",  0);
	//WriteString_16(84+80, 271-30, "KHz",  0);


}

void  Disp_Button_Test(void)
{
		Disp_Fastbutton();
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
	if(SaveData.Sys_Setup.Language == 1)
	{
		WriteString_16(83, 271-29, "MEASSET",  0);
		WriteString_16(83+80,271-29, "No.DISP",  0);
		WriteString_16(92+80+80,271-29, "COUNT ",  0);
	}else if(SaveData.Sys_Setup.Language == 0){
		WriteString_16(83, 271-29, "��������",  0);
		WriteString_16(83+80,271-29, "������ʾ",  0);
		WriteString_16(92+80+80,271-29, "������",  0);
	}
	//	WriteString_16(83, 271-29, "��������",  0);
	  
	//	WriteString_16(83+80,271-29, "������ʾ",  0);
	  
	//	WriteString_16(92+80+80,271-29, "������",  0);
	

}



void Disp_Button_value1(uint32_t value)
{
	Disp_Fastbutton();
	if(value==0)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		if(SaveData.Sys_Setup.Language == 1)
		{
			WriteString_16(83, 271-29, "MEASSET",  0);
			WriteString_16(83+80,271-29, "No.DISP",  0);
			WriteString_16(92+80+80,271-29, "COUNT ",  0);
		}else if(SaveData.Sys_Setup.Language == 0){
			WriteString_16(83, 271-29, "������ʾ",  0);
			WriteString_16(83+80,271-29, "������ʾ",  0);
			WriteString_16(92+80+80,271-29, "������",  0);
		}
//	    WriteString_16(64, 100, "����",  0);
//		WriteString_16(84, 271-40, "����",  0);
//		WriteString_16(84, 271-20, "��ʾ",  0);
//		WriteString_16(84+80, 271-40, "����",  0);
//		WriteString_16(84+80, 271-20, "��ʾ",  0);
//		WriteString_16(84+80+80, 271-40, "��",  0);
//		WriteString_16(84+80+80, 271-20, "����",  0);
//		WriteString_16(83, 271-29, "������ʾ",  0);
//		WriteString_16(83+80,271-29, "������ʾ",  0);
//		WriteString_16(92+80+80,271-29, "������",  0);
//		WriteString_16(83, 271-29, "MEASSET",  0);
//		WriteString_16(83+80,271-29, "No.DISP",  0);
//		WriteString_16(92+80+80,271-29, "COUNT ",  0);
//		WriteString_16(84+80+80+80, 271-40, "�б�",  0);
//		WriteString_16(84+80+80+80, 271-20, "ɨ��",  0);
//		WriteString_16(84+80+80+80+80, 271-40, "����",  0);
//		WriteString_16(84+80+80+80+80, 271-20, " 1/2",  0);
	}
    else if(value==1)
    {
        Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
//		WriteString_16(84, 271-40, "�ļ�",  0);
//		WriteString_16(84, 271-20, "����",  0);
//		WriteString_16(84+80, 271-40, "����",  0);
//		WriteString_16(84+80, 271-20, "����",  0);
//        WriteString_16(84+80+80+80+80, 271-40, "����",  0);
//		WriteString_16(84+80+80+80+80, 271-20, "2/2",  0);
		
//			WriteString_16(83, 271-29, "�ļ�����",  0);
//			WriteString_16(83+80, 271-29, "��������",  0);
//			WriteString_16(84+80+80+80+80-1, 271-29, "����",  0);
		if(SaveData.Sys_Setup.Language == 1)
		{
			WriteString_16(83, 271-29, "FILE SET",  0);
			WriteString_16(83+80, 271-29, "SaveData",  0);
			WriteString_16(84+80+80+80+80-1, 271-29, "MORE",  0);
		}else if(SaveData.Sys_Setup.Language == 0){
			WriteString_16(83, 271-29, "�ļ�����",  0);
			WriteString_16(83+80, 271-29, "��������",  0);
			WriteString_16(84+80+80+80+80-1, 271-29, "����",  0);
		}
			WriteString_16(84+80+80+80+80+41, 271-29, "2/2",  0);
    }
    else
    {
        Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
//        WriteString_16(83, 271-29, "  ȡ�� ",  0);
//		WriteString_16(83+80,271-29, "  ȷ�� ",  0);
		if(SaveData.Sys_Setup.Language == 1)
		{
			 WriteString_16(83, 271-29, "CANCEL ",  0);
			WriteString_16(83+80,271-29, "CONFIRM",  0);
		}else if(SaveData.Sys_Setup.Language == 0){
			WriteString_16(83, 271-29, "  ȡ�� ",  0);
			WriteString_16(83+80,271-29, "  ȷ�� ",  0);
		}
		//WriteString_16(92+80+80,271-29, "������",  0);
    
    
    }

}
void Disp_Button_ItemScan_no(void)
{
	Disp_Fastbutton();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BUTON;

	if(SaveData.Sys_Setup.Language == 1)
	{
		WriteString_16(84, 271-40, "CLEA",  0);
		WriteString_16(84, 271-20, "LIST",  0);
	}else if(SaveData.Sys_Setup.Language == 0){
		WriteString_16(84, 271-40, "���",  0);
		WriteString_16(84, 271-20, "����",  0);
	}
	
//				WriteString_16(84+80, 271-40, "�Զ�",  0);
//				WriteString_16(84+80, 271-20, "����",  0);
//        WriteString_16(84+80+80, 271-30, "����",  0);
//		WriteString_16(84+80+80, 271-20, "����",  0);
//	WriteString_16(84+80+80+80, 271-30, "��һҳ",  0);
//	WriteString_16(84+80+80+80+80, 271-30, "��һҳ",  0);
	if(SaveData.Sys_Setup.Language == 1)
	{
		WriteString_16(84+80+80+80, 271-30, "Last P",  0);
		WriteString_16(84+80+80+80+80, 271-30, "Next P",  0);
	}else if(SaveData.Sys_Setup.Language == 0){
		WriteString_16(84+80+80+80, 271-30, "��һҳ",  0);
		WriteString_16(84+80+80+80+80, 271-30, "��һҳ",  0);
	}
	
}
void Disp_Button_ItemScanSet(uint32_t value)
{
	Disp_Fastbutton();
 	if(value==0||value==1)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		if(SaveData.Sys_Setup.Language == 1)
		{
			WriteString_16(84, 271-30, "DelRow",  0);
			WriteString_16(84+80, 271-40, "LINE",  0);
			WriteString_16(84+80, 271-20, "ATOU",  0);
			WriteString_16(84+80+80, 271-40, "LOG ",  0);
			WriteString_16(84+80+80, 271-20, "ATOU",  0);
			WriteString_16(84+80+80+80, 271-30, "Last P",  0);
			WriteString_16(84+80+80+80+80, 271-30, "Next P",  0);
			
		}else if(SaveData.Sys_Setup.Language == 0){
			WriteString_16(84, 271-30, "ɾ����",  0);
	//		WriteString_16(84, 271-20, "����",  0);
			WriteString_16(84+80, 271-40, "����",  0);
			WriteString_16(84+80, 271-20, "�Զ�",  0);
			WriteString_16(84+80+80, 271-40, "����",  0);
			WriteString_16(84+80+80, 271-20, "�Զ�",  0);
			WriteString_16(84+80+80+80, 271-30, "��һҳ",  0);
	//		WriteString_16(84+80+80+80, 271-20, "����",  0);
			WriteString_16(84+80+80+80+80, 271-30, "��һҳ",  0);
		}
		//WriteString_16(84, 271-30, "ɾ����",  0);
//		WriteString_16(84, 271-20, "����",  0);
		//WriteString_16(84+80, 271-40, "����",  0);		
		//WriteString_16(84+80, 271-20, "�Զ�",  0);
		
		
	//	WriteString_16(84+80+80, 271-20, "�Զ�",  0);
		
//		WriteString_16(84+80+80+80, 271-20, "����",  0);
		
//		WriteString_16(84+80+80+80+80, 271-20, " 1/2",  0);
	}
    else 
//		if(value==2)
//		{
//			Colour.Fword=White;
//				Colour.black=LCD_COLOR_TEST_BUTON;
//				WriteString_16(84, 271-40, "���",  0);
//				WriteString_16(84, 271-20, "����",  0);
////				WriteString_16(84+80, 271-40, "�Զ�",  0);
////				WriteString_16(84+80, 271-20, "����",  0);
//		//        WriteString_16(84+80+80, 271-30, "����",  0);
//		//		WriteString_16(84+80+80, 271-20, "����",  0);
//				WriteString_16(84+80+80+80, 271-30, "��һҳ",  0);
//				WriteString_16(84+80+80+80+80, 271-30, "��һҳ",  0);
//			
//		
//		}else
				
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				if(SaveData.Sys_Setup.Language == 1)
				{
					WriteString_16(84, 271-30, "DelRow",  0);
					WriteString_16(84+80, 271-40, "ATOU",  0);
					WriteString_16(84+80, 271-20, "COPY",  0);
					WriteString_16(84+80+80+80, 271-30, "Last P",  0);
					WriteString_16(84+80+80+80+80, 271-30, "Next P",  0);
					
				}else if(SaveData.Sys_Setup.Language == 0){
					WriteString_16(84, 271-30, "ɾ����",  0);
			//		WriteString_16(84, 271-20, "����",  0);
					WriteString_16(84+80, 271-40, "�Զ�",  0);
					WriteString_16(84+80, 271-20, "����",  0);
			//        WriteString_16(84+80+80, 271-30, "����",  0);
			//		WriteString_16(84+80+80, 271-20, "����",  0);
					WriteString_16(84+80+80+80, 271-30, "��һҳ",  0);
					WriteString_16(84+80+80+80+80, 271-30, "��һҳ",  0);
				}
			//	WriteString_16(84, 271-30, "ɾ����",  0);
		//		WriteString_16(84, 271-20, "����",  0);
			//	WriteString_16(84+80, 271-40, "�Զ�",  0);
				
			//	WriteString_16(84+80, 271-20, "����",  0);
				
		//        WriteString_16(84+80+80, 271-30, "����",  0);
		//		WriteString_16(84+80+80, 271-20, "����",  0);
				
			
			
			}


}
void Disp_Button_TestSet(uint32_t value)
{
	Disp_Fastbutton();
 	if(value==0)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
//		WriteString_16(84, 271-40, "����",  0);
//		WriteString_16(84, 271-20, "����",  0);
//		WriteString_16(84+80, 271-40, "�û�",  0);
//		WriteString_16(84+80, 271-20, "У��",  0);
//		WriteString_16(84+80+80, 271-40, "����",  0);
//		WriteString_16(84+80+80, 271-20, "����",  0);
		if(SaveData.Sys_Setup.Language == 1)
		{
			WriteString_16(83, 271-29, "MEADISP",  0);
			WriteString_16(83+80, 271-29, "CORRECT",  0);
			WriteString_16(83+80+80, 271-29, "LMTSET",  0);
			WriteString_16(83+80+80+80, 271-29, "SYSSET",  0);
			
		}else if(SaveData.Sys_Setup.Language == 0){
			WriteString_16(83, 271-29, "������ʾ",  0);
			WriteString_16(83+80, 271-29, "�û�У��",  0);
			WriteString_16(83+80+80, 271-29, "��������",  0);
			WriteString_16(83+80+80+80, 271-29, "ϵͳ����",  0);
		}
		
		
		
//		WriteString_16(84+80+80+80, 271-40, "�б�",  0);
//		WriteString_16(84+80+80+80, 271-20, "����",  0);
//		WriteString_16(84+80+80+80+80, 271-40, "����",  0);
//		WriteString_16(84+80+80+80+80, 271-20, " 1/2",  0);
	}
    else if(value==1)
    {
        Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
//		WriteString_16(84, 271-40, "�ļ�",  0);
//		WriteString_16(84, 271-20, "����",  0);
//		WriteString_16(84+80, 271-40, "ϵͳ",  0);
//		WriteString_16(84+80, 271-20, "����",  0);
//        WriteString_16(84+80+80, 271-30, "����",  0);
//		WriteString_16(84+80+80, 271-20, "����",  0);
//        WriteString_16(84+80+80+80+80, 271-40, "����",  0);
		if(SaveData.Sys_Setup.Language == 1)
		{
			WriteString_16(84, 271-40, "FILE",  0);
			WriteString_16(84, 271-20, "MANE",  0);
			WriteString_16(84+80, 271-40, "SYS ",  0);
			WriteString_16(84+80, 271-20, "SET ",  0);
			WriteString_16(84+80+80, 271-30, "TOOL",  0);
			WriteString_16(84+80+80, 271-20, "SET ",  0);
			WriteString_16(84+80+80+80+80, 271-40, "MORE",  0);
			WriteString_16(84+80+80+80+80, 271-20, " 2/2",  0);  
			
		}else if(SaveData.Sys_Setup.Language == 0){
			WriteString_16(84, 271-40, "�ļ�",  0);
			WriteString_16(84, 271-20, "����",  0);
			WriteString_16(84+80, 271-40, "ϵͳ",  0);
			WriteString_16(84+80, 271-20, "����",  0);
			WriteString_16(84+80+80, 271-30, "����",  0);
			WriteString_16(84+80+80, 271-20, "����",  0);
			WriteString_16(84+80+80+80+80, 271-40, "����",  0);
			WriteString_16(84+80+80+80+80, 271-20, " 2/2",  0);  
		}
		  
    
    }
    else
    {
        Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
//        WriteString_16(83, 271-29, "����У��",  0);
//		WriteString_16(83+80, 271-29, "����У��",  0);
//		WriteString_16(83+80+80, 271-29, "�˳�����",  0);
//		WriteString_16(83+80+80+80, 271-29, "���У��",  0);
		if(SaveData.Sys_Setup.Language == 1)
		{
			WriteString_16(83, 271-29, "C Corret",  0);
			WriteString_16(83+80, 271-29, "R Corret",  0);
			WriteString_16(83+80+80, 271-29, "ExitSave",  0);
			WriteString_16(83+80+80+80, 271-29, "Cle Corr",  0); 
			
		}else if(SaveData.Sys_Setup.Language == 0){
			WriteString_16(83, 271-29, "����У��",  0);
			WriteString_16(83+80, 271-29, "����У��",  0);
			WriteString_16(83+80+80, 271-29, "�˳�����",  0);
			WriteString_16(83+80+80+80, 271-29, "���У��",  0); 
		}
     
    }

}

void Disp_Button_SysSet(void)
{

	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BUTON;
		
//		WriteString_16(83, 271-29, "��������",  0);
	//	WriteString_16(83, 271-29, "��������",  0);
	if(SaveData.Sys_Setup.Language == 1)
	{
		WriteString_16(83, 271-29, "MeasSet",  0);
		
	}else if(SaveData.Sys_Setup.Language == 0){
		WriteString_16(83, 271-29, "��������",  0); 
	}
	
//		WriteString_16(83, 271-29, "ϵͳ",  0);
//		WriteString_16(83, 271-29, "����",  0);
		
//		WriteString_16(83+80, 271-29, "����",  0);
//		WriteString_16(84+80+80, 271-40, "����",  0);
//		WriteString_16(84+80+80, 271-20, "����",  0);
//		WriteString_16(84+80+80+80, 271-40, "Ĭ��",  0);
//		WriteString_16(84+80+80+80, 271-20, "����",  0);
//		WriteString_16(84+80+80+80+80, 271-40, "ϵͳ",  0);
//		WriteString_16(84+80+80+80+80, 271-20, "��λ",  0);


}

void Disp_Button_Correction(void)
{

		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;	
//		WriteString_16(83, 271-29, "����У��",  0);
//		WriteString_16(83+80, 271-29, "����У��",  0);
//		WriteString_16(83+160+19, 271-29, "�˳�",  0);
//		WriteString_16(83+160+160, 271-29, "���У��",  0);
	if(SaveData.Sys_Setup.Language == 1)
	{
		WriteString_16(83, 271-29, "C Corret",  0);
		WriteString_16(83+80, 271-29, "R Corret",  0);
		WriteString_16(83+160+19, 271-29, "Exit",  0);
		WriteString_16(83+160+160, 271-29, "Cle Corr",  0);
		
	}else if(SaveData.Sys_Setup.Language == 0){
		WriteString_16(83, 271-29, "����У��",  0);
		WriteString_16(83+80, 271-29, "����У��",  0);
		WriteString_16(83+160+19, 271-29, "�˳�",  0);
		WriteString_16(83+160+160, 271-29, "���У��",  0);
	}
	
//		WriteString_16(83+80+19, 271-29, "ȷ��",  0);
}
void Disp_TopBar_Color(void)	//���˵��ĵ�ɫ��ɫ
{
	LCD_DrawRect( 0, 0, 210, 22 , LCD_COLOR_TEST_BAR);

}
void Disp_MidRect(void)
{
	LCD_DrawRect(0,92,479 ,202 , LCD_COLOR_TEST_MID ) ;
}
void Disp_TestScreen(void)
{
//	Disp_TopBar_Color();
	LCD_DrawRect(0, 0, 160,22 , LCD_COLOR_TEST_BAR);
	Disp_Fastbutton();//��ʾ���ٰ���
	Disp_MidRect();//��ʾ�м�ɫ��
}
//
//������ʾ���˵�����Ŀ��ʾ
//
void Disp_Test_Item(void)
{
	uint32_t i;
	Disp_TestScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language == 1)
	{
		WriteString_16(0,4, All_TopName_E[0],  0);
		
	}else if(SaveData.Sys_Setup.Language == 0){
		WriteString_16(0,4, All_TopName[0],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<5;i++)
	{
		if(i<3)
		{
			if(SaveData.Sys_Setup.Language == 1)
			{
				WriteString_16(0, 26+i*22, Test_Setitem_E[i],  0);
				
			}else if(SaveData.Sys_Setup.Language == 0){
				WriteString_16(0, 26+i*22, Test_Setitem[i],  0);
			}
		}else{
			if(SaveData.Sys_Setup.Language == 1)
			{
				WriteString_16(250,26+(i-3)*22, Test_Setitem_E[i],  0);
				
			}else if(SaveData.Sys_Setup.Language == 0){
				WriteString_16(250,26+(i-3)*22, Test_Setitem[i],  0);
			}
			
		}
		if(SaveData.Sys_Setup.Language == 1)
		{
			WriteString_16(0,210, Test_Setitem_E[5],  0);
			WriteString_16(120,210, Test_Setitem_E[6],  0);
			WriteString_16(250, 210, Test_Setitem_E[7],  0);
			
		}else if(SaveData.Sys_Setup.Language == 0){
			WriteString_16(0,210, Test_Setitem[5],  0);
			WriteString_16(120,210, Test_Setitem[6],  0);
			WriteString_16(250, 210, Test_Setitem[7],  0);
		}	
		
	}
	Disp_Button_Test();
	

}
//������ʾ�Ӻ���
void Disp_Range_MidRect(void)
{

	LCD_DrawRect( 0, 160,479 , 190 , LCD_COLOR_TEST_MID ) ;
}
void Disp_RangeScreen(void)
{
//	Disp_TopBar_Color();
	LCD_DrawRect( 0, 0, 160, 22 , LCD_COLOR_TEST_BAR);
	Disp_Fastbutton();//��ʾ���ٰ���
	Disp_Range_MidRect();//��ʾ�м�ɫ��
}
//������ʾ���˵���Ŀ��ʾ
void Disp_Range_Item(void)
{
	uint32_t i;
	Disp_RangeScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language == 1)
	{
		WriteString_16(0, 4, All_TopName_E[1],  0);
		
	}else if(SaveData.Sys_Setup.Language == 0){
		WriteString_16(0, 4, All_TopName[1],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<6;i++)
	{
		if(i<3)
		{
			if(SaveData.Sys_Setup.Language == 1)
			{
				WriteString_16(0, 26+i*22, Range_Item_E[i],  0);
				
			}else if(SaveData.Sys_Setup.Language == 0){
				WriteString_16(0, 26+i*22, Range_Item[i],  0);
			}
			
		}else{
			if(SaveData.Sys_Setup.Language == 1)
			{
				WriteString_16(250, 26+(i-3)*22, Range_Item_E[i],  0);
				
			}else if(SaveData.Sys_Setup.Language == 0){
				WriteString_16(250, 26+(i-3)*22, Range_Item[i],  0);
			}
			
		}
//		WriteString_16(0, 210, "R :",  0);
//		WriteString_16(120, 210, Test_Setitem[7],  0);
		
	
	}
	Disp_RangeDispvalue(SaveData.Main_Func.Param.test);
	
	Disp_Button_value1(0);
	

}
//��������ʾ�Ӻ���

void Disp_Rang_Count_MidRect(void)
{

	LCD_DrawRect( 0, 86,479 , 160 , LCD_COLOR_TEST_MID ) ;		//��ɫ
	LCD_DrawRect( 0, 193,479 , 227 , LCD_COLOR_TEST_MID ) ;
}

void Disp_Range_CountScreen(void)
{
//	Disp_TopBar_Color();
	LCD_DrawRect( 0, 0, 178, 22 , LCD_COLOR_TEST_BAR);
	Disp_Fastbutton();//��ʾ���ٰ���
	Disp_Rang_Count_MidRect();//��ʾ�м�ɫ��
}
//��������ʾ����
void Disp_Range_Count_Item(void)
{
	uint32_t i;
	Disp_Range_CountScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language == 1)
	{
		WriteString_16(0, 4, All_TopName_E[2],  0);	
	}else if(SaveData.Sys_Setup.Language == 0){
		WriteString_16(0, 4, All_TopName[2],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<3;i++)
	{
		if(i==1)
		{
			if(SaveData.Sys_Setup.Language == 1)
			{
				WriteString_16(i*160+30, FIRSTLINE-2, Range_Count_Item_E[i],  0);	
			}else if(SaveData.Sys_Setup.Language == 0){
				WriteString_16(i*160+30, FIRSTLINE-2, Range_Count_Item[i],  0);
			}		
		}else if(i==2){
			if(SaveData.Sys_Setup.Language == 1)
			{
				WriteString_16(i*160+40, FIRSTLINE-2, Range_Count_Item_E[i],  0);	
			}else if(SaveData.Sys_Setup.Language == 0){
				WriteString_16(i*160+40, FIRSTLINE-2, Range_Count_Item[i],  0);
			}			
		}else{
			if(SaveData.Sys_Setup.Language == 1)
			{
				WriteString_16(i*160, FIRSTLINE-2, Range_Count_Item_E[i],  0);	
			}else if(SaveData.Sys_Setup.Language == 0){
				WriteString_16(i*160, FIRSTLINE-2, Range_Count_Item[i],  0);
			}			
		}
//		WriteString_16(0, 210, "R :",  0);
//		WriteString_16(120, 210, Test_Setitem[7],  0);//WriteString_12
		
	
	}
	WriteString_16(LIST1+60, FIRSTLINE-2, User_Freq[SaveData.Main_Func.Freq],  1);//��ʾ����
	Hex_Format(SaveData.Limit_Tab.Nom.Num , SaveData.Limit_Tab.Nom.Dot , 6 , 0);
	WriteString_16(LIST2, FIRSTLINE-2, DispBuf,  1);//��ʾ���ֵ  ����Ҫ������ʾ��λ
	WriteString_16(LIST2+70, FIRSTLINE-2, Disp_Unit[SaveData.Limit_Tab.Nom.Unit],  1);
	Disp_Set_Unit_16(SaveData.Main_Func.Param.test,SELECT_2END-112,FIRSTLINE-2 );//��ʾ��λ
//	WriteString_16(LIST2-30, FIRSTLINE, DispBuf,  1);
	
	WriteString_12(0,FIRSTLINE+20-2,"BIN",0);//Num_1
	WriteString_12(120,FIRSTLINE+20-2,"LOW [F]",0);
	WriteString_12(250,FIRSTLINE+20-2,"HIGH[F]",0);
	WriteString_12(413,FIRSTLINE+20-2,"COUNT",0);
	
	for(i=6;i<15;i++)
	{
		if((i-6>1&&i-6<7)||((i-6>8)))
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;

		
		if(SaveData.Limit_Tab.Comp_Value[i-6].low.Num!=0)//SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[i-6].low.Num ,
			SaveData.Limit_Tab.Comp_Value[i-6].low.Dot , 6 , 1);//�ӵ�λ
			//SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].high
			WriteString_12(LIST2-130, 58+(i-6)*15, DispBuf,  1);//��ʾ���ֵ  ����Ҫ������ʾ��λ
		}
		else
		WriteString_12(LIST2-130, 58+(i-6)*15, "------",  1);	
	}
	Colour.black=LCD_COLOR_TEST_MID;
	if(SaveData.Limit_Tab.Comp_Value[9].high.Num!=0)
	{
		Hex_Format(SaveData.Limit_Tab.Comp_Value[9].high.Num, 
			SaveData.Limit_Tab.Comp_Value[9].high.Dot , 6 , 1);//�ӵ�λ//�ӵ�λ
		
		WriteString_12(LIST2-130, 196, DispBuf,  1);//��ʾ���ֵ  ����Ҫ������ʾ��λ
	}
	else
	WriteString_12(LIST2-130, 196, "------",  1);
	
	for(i=16;i<25;i++)
	{
		if((i-16>1&&i-16<7)||(i-16>8))
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
			
		if(SaveData.Limit_Tab.Comp_Value[i-16].high.Num!=0)
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[i-16].high.Num, 
			SaveData.Limit_Tab.Comp_Value[i-16].high.Dot , 6 , 1);//�ӵ�λ
			
			WriteString_12(LIST2, 58+(i-16)*15, DispBuf,  1);//��ʾ���ֵ  ����Ҫ������ʾ��λ
		}
		else
			WriteString_12(LIST2, 58+(i-16)*15, "------",  1);
	}
	Colour.black=LCD_COLOR_TEST_MID;
	if(SaveData.Limit_Tab.Comp_Value[9].high.Num!=0)
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[9].low.Num, 
			SaveData.Limit_Tab.Comp_Value[9].low.Dot , 6 , 1);//�ӵ�λ
		
		WriteString_12(LIST2, 196, DispBuf,  1);//��ʾ���ֵ  ����Ҫ������ʾ��λ
		}
		else
			WriteString_12(LIST2, 196, "------",  1);
	
	
	for(i=1;i<10;i++)
	{
		if(i>2&&i<8)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
			
		WriteString_12(8,FIRSTLINE+17+15*i,Num_1[i-1],0);
		
	}
	Colour.black=LCD_COLOR_TEST_MID;
	WriteString_12(0,196,"2nd",0);
	WriteString_12(0,212,"AUX:",0);
	WriteString_12(248,212,"OUT:",0);
	
	
	
	Disp_Button_value1(0);


}
//�б���ʾ�Ӻ���
void Disp_List_Count_MidRect(void)
{

	LCD_DrawRect( 0, 175,479 , 160 , LCD_COLOR_TEST_MID ) ;
//	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
}

void Disp_List_CountScreen(void)
{
	Disp_TopBar_Color();
	Disp_Fastbutton();//��ʾ���ٰ���
	Disp_List_Count_MidRect();//��ʾ�м�ɫ��
}
//�б���ʾ����
void Disp_List_Count_Item(void)
{
	Disp_List_CountScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	WriteString_16(0, 4, All_TopName_E[3],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;

//	WriteString_16(LIST1, FIRSTLINE, "��ʽ :",  0);
//	WriteString_16(LIST1+40, FIRSTLINE+SPACE1, "Ƶ��[Hz]",  0);	
//	WriteString_16(LIST1+140, FIRSTLINE+SPACE1, "��ƽ[V]",  0);	
	if(SaveData.Sys_Setup.Language == 1)
	{
		WriteString_16(LIST1, FIRSTLINE, "Meth :",  0);
		WriteString_16(LIST1+40, FIRSTLINE+SPACE1, "Freq[Hz]",  0);	
		WriteString_16(LIST1+140, FIRSTLINE+SPACE1, "Levl[V]",  0);		
	}else if(SaveData.Sys_Setup.Language == 0){
		WriteString_16(LIST1, FIRSTLINE, "��ʽ :",  0);
		WriteString_16(LIST1+40, FIRSTLINE+SPACE1, "Ƶ��[Hz]",  0);	
		WriteString_16(LIST1+140, FIRSTLINE+SPACE1, "��ƽ[V]",  0);	
	}	
	
	WriteString_16(LIST1+140+100, FIRSTLINE+SPACE1, "Cp[F]",  0);
	WriteString_16(LIST1+140+100+100, FIRSTLINE+SPACE1, "D[ ]",  0);
	WriteString_16(LIST1+440, FIRSTLINE+SPACE1, "CMP",  0);
	Disp_Button_value1(0);

}

//�б���ʾ�Ӻ���
void Disp_TestSet_MidRect(void)
{

	LCD_DrawRect( 0, 90,479 , 154 , LCD_COLOR_TEST_MID ) ;
//	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
}

void Disp_TestSetScreen(void)
{
	LCD_DrawRect( 0, 0, 168, 22 , LCD_COLOR_TEST_BAR);
//	Disp_TopBar_Color();
	Disp_Fastbutton();//��ʾ���ٰ���
	Disp_TestSet_MidRect();//��ʾ�м�ɫ��
}

static  void  Dis_Dot(void)
{
   uint8_t  i;
	
	for(i=0;i<10;i++)
	{
		if(i<5)
		{
			if(i==3||i==4)		Colour.black=LCD_COLOR_TEST_MID;
			else                    Colour.black=LCD_COLOR_TEST_BACK;
			WriteString_16(LIST1+72, FIRSTLINE+i*SPACE1+1, ":",  0);
		}
		else 
		{
			if(i==8||i==9)	  Colour.black=LCD_COLOR_TEST_MID;
			else 					  Colour.black=LCD_COLOR_TEST_BACK;
			WriteString_16(LIST2+72, FIRSTLINE+(i-5)*SPACE1+1, ":",  0);
		}
	}

}



//�б���ʾ����	   //  �������ý�����ʾ�˵�����
void Disp_Test_Set_Item(void)
{
	uint32_t i;
	Disp_TestSetScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language ==1)
	{
		WriteString_16(0, 4, All_TopName_E[4],  0);
	}else if(SaveData.Sys_Setup.Language ==0){
		WriteString_16(0, 4, All_TopName[4],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<(sizeof(Set_testitem_E)/(sizeof(Set_testitem_E[0])));i++)
	if(SaveData.Sys_Setup.Language == 1)
	{
		if(i<sizeof(Set_testitem_E)/(sizeof(Set_testitem_E[0]))/2)
		{
			if(i>2&&i<5)
				Colour.black=LCD_COLOR_TEST_MID;
			else
				Colour.black=LCD_COLOR_TEST_BACK;	
			
			WriteString_16(LIST1, FIRSTLINE+SPACE1*i, Set_testitem_E[i],  0);

		}
		else
		{
			if((i-sizeof(Set_testitem_E)/(sizeof(Set_testitem_E[0]))/2)>2&&(i-sizeof(Set_testitem_E)/(sizeof(Set_testitem_E[0]))/2)<5)
				Colour.black=LCD_COLOR_TEST_MID;
			else
				Colour.black=LCD_COLOR_TEST_BACK;

			WriteString_16(LIST2, FIRSTLINE+SPACE1*(i-sizeof(Set_testitem_E)/(sizeof(Set_testitem_E[0]))/2), Set_testitem_E[i],  0);
			
			
		}		
	}else if(SaveData.Sys_Setup.Language == 0){
		if(i<sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)
		{
			if(i>2&&i<5)
				Colour.black=LCD_COLOR_TEST_MID;
			else
				Colour.black=LCD_COLOR_TEST_BACK;	
			
			WriteString_16(LIST1, FIRSTLINE+SPACE1*i, Set_testitem[i],  0);

		}
		else
		{
			if((i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)>2&&(i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)<5)
				Colour.black=LCD_COLOR_TEST_MID;
			else
				Colour.black=LCD_COLOR_TEST_BACK;

			WriteString_16(LIST2, FIRSTLINE+SPACE1*(i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2), Set_testitem[i],  0);
			
			
		}	
	}	
	
	Dis_Dot();
	Disp_Button_TestSet(0);

}
//����ʱ������ʾ�������ʾ
void Disp_RangeDispValue(Button_Page_Typedef *Button_Page)
{
//	uint32_t flag;
	uint32_t i;
//	uint32_t xpose;
	uint32_t Black_Select;
//	uint32_t Select_color;

//����	
	
	Colour.black=LCD_COLOR_TEST_BACK;	
	WriteString_16(LIST1+73, FIRSTLINE, User_FUNC[SaveData.Main_Func.Param.test],  1);//�����㷨  ��˳��Ĺ���
	
//Ƶ��	
	WriteString_16(LIST1+73, FIRSTLINE+SPACE1, User_Freq[SaveData.Main_Func.Freq],  1);
	
	
//��ƽ	

	WriteString_16(LIST1+73, FIRSTLINE+SPACE1*2, User_Level[SaveData.Main_Func.Level],  1);
//����

//	if(SaveData.Main_Func.Range.Auto)
//		WriteString_16(LIST2+88, FIRSTLINE, User_Range[0],  0);
//	else
//	{
		WriteString_16(LIST2+73, FIRSTLINE, User_Range[SaveData.Main_Func.Range.Range],  0);
	
//	}
	
//�ٶ�
	
	WriteString_16(LIST2+73, FIRSTLINE+SPACE1, User_Speed[SaveData.Main_Func.Speed],  1);
	
//ƫ��

//	WriteString_16(LIST2+73, FIRSTLINE+SPACE1, User_Comp[SaveData.Main_Func.Bias],  1);
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+72, FIRSTLINE+SPACE1*2-2,SELECT_2END-50 , FIRSTLINE+SPACE1*3-4 , Colour.black ) ;//SPACE1//SPACE1
	WriteString_16(LIST2+73, FIRSTLINE+SPACE1*2, User_Comp[SaveData.Limit_Tab.Comp],  1);
	Disp_Fastbutton();
	if(Button_Page->index==1)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		for(i=0;i<2;i++)
		 {
			 if(i==0)
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, User_Comp[i],  0);
			 else
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+25, BUTTOM_Y_VALUE, User_Comp[i],  0); 
		 }
	}
	else
	
	Disp_Button_value1(0);
	

}
//����ʱ������ʾ�������ʾ        ��������ʾON �� OFF
void Disp_Range_ComDispValue(Button_Page_Typedef *Button_Page)
{
//	uint32_t flag;
	uint32_t i;
//	uint32_t xpose;
	uint32_t Black_Select;
//	uint32_t Select_color;

	Colour.black=LCD_COLOR_TEST_BACK;	
	
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawRect( 320+99, 26-2,320+140 , 26+16-2 , Colour.black ) ;//SPACE1//SPACE1
	WriteString_16(320+100, 26-2, User_Comp[SaveData.Limit_Tab.count],  1);
	Disp_Fastbutton();
	if(Button_Page->index==1)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		for(i=0;i<2;i++)
		  {
			  if(i==0)
				  WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, User_Comp[i],  0);
			  else
				  WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+25, BUTTOM_Y_VALUE, User_Comp[i],  0); 
		  }
	}
	else
	
	Disp_Button_value1(0);
	

}

//����ʱ����ʾ�б�ɨ��Ĳ���ֵ��Ҳ���ǴӴ��ڽ��չ�����ֵ
void Disp_Scan_Compvalue(uint8_t set)
{
	uint8_t i;
//	uint32_t  Black_Select;
//	static uint32_t pos[2];
	
//	Black_Select=(SaveData.Limit_ScanValue.xpos)?1:0;//count
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//	pos[0]
//	if(set)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//		LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;
//	}
//	Colour.black=LCD_COLOR_TEST_BACK;
//	if(pos[0]!=SaveData.Limit_ScanValue.xpos||pos[1]!=SaveData.Limit_ScanValue.ypos)
//	{
//		LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;
//	
//	}
	for(i=1;i<11;i++)
	{
		
		if(i>2&&i<7)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
		
		if(SaveData.Limit_ScanValue.Range_Set_main[SaveData.Limit_ScanValue.num*10+(i-1)].Num==0xffffffff)
			WriteString_12(8+30+108+100,FIRSTLINE+SPACE1+3+ (i)*16, "----",  0);
		else
		{
			Hex_Format(SaveData.Limit_ScanValue.Range_Set_main[SaveData.Limit_ScanValue.num*10+(i-1)].Num ,
			 SaveData.Limit_ScanValue.Range_Set_main[SaveData.Limit_ScanValue.num*10+(i-1)].Dot , 5 , 1);
				WriteString_12(8+30+108+100,FIRSTLINE+SPACE1+3+ (i)*16, DispBuf,  0);
			
		}
		if(SaveData.Limit_ScanValue.Range_Sed_Second[SaveData.Limit_ScanValue.num*10+(i-1)].Num==0xffffffff)
			WriteString_12(8+30+108+100+100,FIRSTLINE+SPACE1+3+ (i)*16, "----",  0);
		else
		{
			
			Hex_Format(SaveData.Limit_ScanValue.Range_Sed_Second[SaveData.Limit_ScanValue.num*10+(i-1)].Num ,
				SaveData.Limit_ScanValue.Range_Sed_Second[SaveData.Limit_ScanValue.num*10+(i-1)].Dot , 5 , 1);
			WriteString_12(8+30+108+100+100,FIRSTLINE+SPACE1+3+ (i)*16, DispBuf,  0);
		
		}
		if(SaveData.Limit_ScanValue.comp[SaveData.Limit_ScanValue.num*10+(i-1)]==0xff)
			WriteString_12(8+30+108+100+100+100,FIRSTLINE+SPACE1+3+ (i)*16, "-",  0);
		else
		{
			
			if(SaveData.Limit_ScanValue.comp[SaveData.Limit_ScanValue.num*10+(i-1)])
				WriteString_12(8+30+108+100+100+100,FIRSTLINE+SPACE1+3+ (i)*16, "P",  0);
			else
				WriteString_12(8+30+108+100+100+100,FIRSTLINE+SPACE1+3+ (i)*16, "F",  0);
		
		}
		//for(i=1;i<11;i++)
		{
			
			if(i>2&&i<7)
				Colour.black=LCD_COLOR_TEST_MID;
			else
				Colour.black=LCD_COLOR_TEST_BACK;	
			if(SaveData.Limit_ScanValue.freq[SaveData.Limit_ScanValue.num*10+(i-1)]>30)
				WriteString_12(8+30,FIRSTLINE+SPACE1+3+ (i)*16, "----",  0);
			else
				{
					WriteString_12(8+30,FIRSTLINE+SPACE1+3+ (i)*16, User_Freq[SaveData.Limit_ScanValue.freq[SaveData.Limit_ScanValue.num*10+(i-1)]],  0);
				
				}
			if(SaveData.Limit_ScanValue.ac_leave[SaveData.Limit_ScanValue.num*10+(i-1)]>2)
				WriteString_12(8+30+108,FIRSTLINE+SPACE1+3+ (i)*16, "----",  0);
			else
				{
					WriteString_12(8+30+108,FIRSTLINE+SPACE1+3+ (i)*16, User_Level[SaveData.Limit_ScanValue.ac_leave[SaveData.Limit_ScanValue.num*10+(i-1)]],  0);
				
				}
				
			Hex_Format(SaveData.Limit_ScanValue.num*10+i , 0 , 3 , 1);
			
			WriteString_12(8,FIRSTLINE+SPACE1+3+ (i)*16, DispBuf,  0);
		}
				
		
	}

}
//const List_Scan[][6][2]=
//{
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{},
//	{},
//	{},
//	{}


//};

//void Disp_List_Select(Button_Page_Typedef* Button_Page,uint8_t i)
//{
//	uint8_t Black_Select;
////	uint8_t i;
////	for(i=0;i<10;i++)
//	{
//		Black_Select=(Button_Page->index==i)?1:0;
//		if(Black_Select)
//		{
//			Colour.black=LCD_COLOR_SELECT;
//		
//		}
//		else
//		{
//			Colour.black=LCD_COLOR_TEST_BACK;
//		}
//		//if()	
//		LCD_DrawRect( 32+Button_Page->page*60, FIRSTLINE+SPACE1+3+16*(Button_Page->index-1),88+Button_Page->page*60 ,
//			FIRSTLINE+SPACE1+3+16*Button_Page->index+16 , Colour.black ) ;
//		//LCD_DrawRect( LIST1+88, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
////		WriteString_16(LIST1+88, FIRSTLINE, User_FUNC[SaveData.Main_Func.Param.test],  1);//�����㷨  ��˳��Ĺ���
//	}
//}
//void Disp_List_Select(Button_Page_Typedef* Button_Page)
//{
//	static uint32_t pos[2];
//	if(Button_Page->index==0&&Button_Page->page==0)
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	
//	}
//	else
//		if(Button_Page->index==1&&Button_Page->page==0)//
//		{
//			Colour.black=LCD_COLOR_SELECT;
//			LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;
//			Colour.black=LCD_COLOR_TEST_BACK;
////			pos[0]=Button_Page->index;
////			pos[1]=Button_Page->page;
//		
//		}
//		else
//		{
//			Colour.black=LCD_COLOR_SELECT;
//			LCD_DrawRect( 32+Button_Page->page*60, FIRSTLINE+SPACE1+3+16*(Button_Page->index-1),88+Button_Page->page*60 ,
//			FIRSTLINE+SPACE1+3+16*Button_Page->index+16 , Colour.black ) ;
//			
//			Colour.black=LCD_COLOR_TEST_BACK;
////			if(pos[0]==1&&pos[1]==0)
////			{
////				LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;
////			}
////			else
////			{
////			
////				LCD_DrawRect( 32+pos[1]*60, FIRSTLINE+SPACE1+3+16*pos[0],88+pos[1]*60 ,
////			FIRSTLINE+SPACE1+3+16*pos[0]+16 , Colour.black ) ;
////			}
////			pos[0]=Button_Page->index;
////			pos[1]=Button_Page->page;
//		
//		}



//}
//����ʱ����ʾ�б�ɨ��Ĳ���ֵ��Ҳ���ǴӴ��ڽ��չ�����ֵ
void Disp_Scan_SetCompvalue(Button_Page_Typedef* Button_Page)
{
	uint8_t i;
	if(Button_Page->index==2)
		Disp_Button_ItemScan_no();
	else
		if(Button_Page->index>2)
		Disp_Button_ItemScanSet(Button_Page->third);
	if(Button_Page->index==1&&Button_Page->third==0)//��һ��ֵ
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawRect( 54, FIRSTLINE,54+40 , FIRSTLINE+16 , Colour.black ) ;	
	WriteString_16(54,FIRSTLINE, User_FUNC[SaveData.Sys_Setup.Main_Func],  0);
	
	if(Button_Page->index==2&&Button_Page->third==0)//�ڶ���ֵ
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawRect( 0, FIRSTLINE+24,30 , FIRSTLINE+24+16 , Colour.black ) ;	
	WriteString_16(0,FIRSTLINE+24, "No.",  0);
	
	for(i=3;i<13;i++)
	{
		
		if(Button_Page->index==i&&Button_Page->third==2)//������
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect( 250, FIRSTLINE+SPACE1+3+ (i-2)*16,250+56 ,
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;	
		if(SaveData.Limit_ScanValue.Range_Set_main[Button_Page->force*10+(i-3)].Num==0xffffffff)
			WriteString_12(250,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
		else
		{
			Hex_Format(SaveData.Limit_ScanValue.Range_Set_main[Button_Page->force*10+(i-3)].Num ,
			 SaveData.Limit_ScanValue.Range_Set_main[Button_Page->force*10+(i-3)].Dot , 5 , 1);
				WriteString_12(250,FIRSTLINE+SPACE1+3+ (i-2)*16, DispBuf,  0);
			
		}
		if(Button_Page->index==i&&Button_Page->third==3)//������
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect( 250+80, FIRSTLINE+SPACE1+3+ (i-2)*16,250+80+56 ,
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;	
		if(SaveData.Limit_ScanValue.Range_Sed_Second[Button_Page->force*10+(i-3)].Num==0xffffffff)
			WriteString_12(250+80,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
		else
		{
			
			Hex_Format(SaveData.Limit_ScanValue.Range_Sed_Second[Button_Page->force*10+(i-3)].Num ,
				SaveData.Limit_ScanValue.Range_Sed_Second[Button_Page->force*10+(i-3)].Dot , 5 , 1);
			WriteString_12(250+80,FIRSTLINE+SPACE1+3+ (i-2)*16, DispBuf,  0);
		
		}
		if(Button_Page->index==i&&Button_Page->third==4)//������
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect( 420, FIRSTLINE+SPACE1+3+ (i-2)*16,479 ,
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;	
		if(SaveData.Limit_ScanValue.comp[Button_Page->force*10+(i-3)]==0xff)
			WriteString_12(420,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
		else
		{
			
			//if(SaveData.Limit_ScanValue.comp[SaveData.Limit_ScanValue.num*10+(i-2)])
				WriteString_12(420,FIRSTLINE+SPACE1+3+ (i-2)*16, "P",  0);
			//else
				//WriteString_12(8+30+108+100+100+100,FIRSTLINE+SPACE1+3+ (i-1)*16, "F",  0);
		
		}
		{
			
			if(Button_Page->index==i&&Button_Page->third==0)//��һ��
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect(40, FIRSTLINE+SPACE1+3+ (i-2)*16,126 , 
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;
			if(SaveData.Limit_ScanValue.freq[Button_Page->force*10+(i-3)]>0x30)
				WriteString_12(40,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
			else
				{
					WriteString_12(40,FIRSTLINE+SPACE1+3+ (i-2)*16, 
					User_Freq[SaveData.Limit_ScanValue.freq[Button_Page->force*10+(i-3)]],  0);
				
				}
			if(Button_Page->index==i&&Button_Page->third==1)//�ڶ���
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect( 38+88, FIRSTLINE+SPACE1+3+ (i-2)*16,38+88+70 , 
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;	
			if(SaveData.Limit_ScanValue.ac_leave[Button_Page->force*10+(i-3)]>2)
				WriteString_12(8+30+88,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
			else
				{
					WriteString_12(8+30+88,FIRSTLINE+SPACE1+3+ (i-2)*16, 
					User_Level[SaveData.Limit_ScanValue.ac_leave[Button_Page->force*10+(i-3)]],  0);
				
				}
			Colour.black=LCD_COLOR_TEST_BACK;	
			Colour.black=LCD_COLOR_TEST_BACK;	
			Hex_Format(Button_Page->force*10+i-2 , 0 , 3 , 1);
			
			WriteString_12(8,FIRSTLINE+SPACE1+3+ (i-2)*16, DispBuf,  0);
		}
				
		
	}
	



}
//����ʱ���б�ɨ����ʾֵ
void Disp_LIMIT_ComDispValue(Button_Page_Typedef *Button_Page)
{
//	uint32_t flag;
	uint32_t i;
//	uint32_t xpose;
	uint32_t Black_Select;
//	uint32_t Select_color;

	Colour.black=LCD_COLOR_TEST_BACK;	
	
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;//SPACE1//SPACE1
	WriteString_16(48, FIRSTLINE, Limit_Scan_Fun[SaveData.Limit_ScanValue.fun],  1);
	
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawRect( 8, FIRSTLINE+SPACE1,30 , FIRSTLINE+SPACE1+16 , Colour.black ) ;//SPACE1//SPACE1
	WriteString_16(8, FIRSTLINE+SPACE1, "No.",  1);
	Disp_Fastbutton();
	if(Button_Page->index==1)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		for(i=0;i<2;i++)
			WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Limit_Scan_Fun[i],  0);
	}
	else if(Button_Page->index==2)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		for(i=0;i<2;i++)
			WriteString_16(BUTTOM_X_VALUE+(i+3)*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Limit_Scan_Page[i],  0);
	}else
		Disp_Button_value1(Button_Page->page);
	

}
//���Ե�ʱ����ʾ������ֵ     // ������ʾ�е�����ֵ
void Disp_Test_value(Button_Page_Typedef* Button_Page)
{
//	uint32_t flag;
	uint32_t i;
//	uint32_t xpose;
	uint32_t Black_Select;
//	uint32_t Select_color;
	if(SaveData.Main_Func.V_i==1) 
	{
		Colour.black=LCD_COLOR_TEST_BACK;
		WriteString_16(30,210, User_Comp[1],  1);
		WriteString_16(30+120, 210, User_Comp[1],  1);
		
	}
		
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//if()	
	LCD_DrawRect( LIST1+87,FIRSTLINE-2,SELECT_1END ,FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88,FIRSTLINE, User_FUNC[SaveData.Main_Func.Param.test],  1);//�����㷨  ��˳��Ĺ���
//	WriteString_16(LIST2-88, SCREENHIGH-FIRSTLINE, User_Range[SaveData.Main_Func.Range.Range],  0);
//Ƶ��	
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect(LIST1+87, FIRSTLINE+SPACE1-2,SELECT_1END ,FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88,FIRSTLINE+SPACE1, User_Freq[SaveData.Main_Func.Freq],  1);
	
	
//��ƽ	
	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect(LIST1+87, FIRSTLINE+SPACE1*2-2,SELECT_1END ,FIRSTLINE+SPACE1*3 -4, Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88,FIRSTLINE+SPACE1*2, User_Level[SaveData.Main_Func.Level],  1);
//����
	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+87, FIRSTLINE-2,SELECT_2END , FIRSTLINE+SPACE1*1-4 , Colour.black ) ;//SPACE1
//	if(SaveData.Main_Func.Range.Auto)
//		WriteString_16(LIST2+88, FIRSTLINE, User_Range[0],  0);
//	else
	{
		WriteString_16(LIST2+88, FIRSTLINE, User_Range[SaveData.Main_Func.Range.Range],  0);
	
	}
	
//�ٶ�
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1-2,SELECT_2END ,FIRSTLINE+SPACE1*2-4, Colour.black ) ;//SPACE1
	WriteString_16(LIST2+88,FIRSTLINE+SPACE1, User_Speed[SaveData.Main_Func.Speed],  1);
	
//ƫ��
//	if(index==12)
//		index=13;
//	Black_Select=(Button_Page->index==6)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST2+87,FIRSTLINE+SPACE1*2-2,SELECT_2END ,FIRSTLINE+SPACE1*3-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST2+88,FIRSTLINE+SPACE1*2, User_Comp[1],  1);//SaveData.Main_Func.Bias
	Disp_Fastbutton();
	
	switch(Button_Page->index)
	{
		case 0:
			Disp_Button_Test();
			//	Disp_Button_value1(Button_Page->page);
			break;
		case 1:				//��ʾ������İ���ֵ
			if(Button_Page->page==0)
			{
				
					
				Colour.Fword=White;//
				Colour.black=LCD_COLOR_TEST_BUTON;
				if(Button_Page->third==0)
				{				
					
					for(i=0;i<5;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Cp_Button_Tip[i],  0);
				
				}
				else if(Button_Page->third==1)
				{
					for(i=0;i<5;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Cs_Button_Tip[i],  0);
				
				}
				
				else if(Button_Page->third==2)
				{
					for(i=0;i<5;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Lp_Button_Tip1[i],  0);
//					WriteString_16(BUTTOM_X_VALUE+3*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE-8, "MORE",  0);
//					WriteString_16(BUTTOM_X_VALUE+3*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+8, "1/2",  0);
				
				}
				else if(Button_Page->third==3)
				{
					for(i=0;i<5;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Ls_Button_Tip[i],  0);
//					WriteString_16(BUTTOM_X_VALUE+3*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE-8, "MORE",  0);
//					WriteString_16(BUTTOM_X_VALUE+3*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+8, "1/2",  0);
				
				}
				else
				{						
					for(i=0;i<4;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+15, BUTTOM_Y_VALUE, Button_TipPage1[i],  0);
					WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
					WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "1/2",  0);
				}
			}
			else if(Button_Page->page==1)
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				if(Button_Page->third==0)
				{
					for(i=0;i<5;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Z_Button_Tip[i],  0);
				
				}
				else if(Button_Page->third==1)
				{
					for(i=0;i<5;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Y_Button_Tip[i],  0);
				
				}
				else if(Button_Page->third==2)
				{
					for(i=0;i<5;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, R_Button_Tip[i],  0);
				
				}
				else if(Button_Page->third==3)
				{
					for(i=0;i<5;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, R_Button_Tip[i],  0);
				
				}
				else
				
				{
					for(i=0;i<4;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, Button_TipPage2[i],  0);
					WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
					WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "2/2",  0);
				}
			
			
			}
			else
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE,Button_TipPage3[0],  0);
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "3/3",  0);
			
			
			}
		break;
		case 2:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<4;i++)
			{
				if(i==1||i==2)
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+5, BUTTOM_Y_VALUE, FreqButton_Tip[i],  0);
				else  
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, FreqButton_Tip[i],  0);	
			}
		break;
		case 3:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, LevleButton_Tip[i],  0);
			}
			break;

		case 4:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<4;i++)
			{
				if(i<2)
				  WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+16, BUTTOM_Y_VALUE, RangeButton_Tip[i],  0);
				else 
				  WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, RangeButton_Tip[i],  0);
			}
			break;
		case 5:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<3;i++)
			{
				if(i==1)
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, SpeedButton_Tip[i],  0);
				else
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+16, BUTTOM_Y_VALUE, SpeedButton_Tip[i],  0);
			}
			break;
//		case 6:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				if(i==0)
//					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, User_Comp[i],  0);
//				else 
//					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+25, BUTTOM_Y_VALUE, User_Comp[i],  0);
//			}
//			break;
		default:
			
		break;
			
	}

}
//��ʾ���ò�����ֵSetup_Valueall
void DispSet_value(Button_Page_Typedef* Button_Page)
{
	uint32_t i;
	uint32_t Black_Select;
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+87, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1*1-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88, FIRSTLINE, User_FUNC[SaveData.Main_Func.Param.test],  1);//�����㷨  ��˳��Ĺ���
	
//Ƶ��	
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+87, FIRSTLINE+SPACE1*1-2,SELECT_1END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*1, User_Freq[SaveData.Main_Func.Freq],  1);
	
	
//��ƽ	
	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+87, FIRSTLINE+SPACE1*2-2,SELECT_1END , FIRSTLINE+SPACE1*3 -4, Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*2, User_Level[SaveData.Main_Func.Level],  1);
	
//����	
	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_MID;
	}
		
	LCD_DrawRect( LIST1+87, FIRSTLINE+SPACE1*3-2,SELECT_1END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*3, Setup_Trig[SaveData.Main_Func.Trig],  1);	
	
//���ƽ
//	if(index==5)
//		index=6;
//	Black_Select=(Button_Page->index==5)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_MID;
//	}
//		
//	LCD_DrawRect( LIST1+87, FIRSTLINE+SPACE1*4-2,SELECT_1END , FIRSTLINE+SPACE1*5-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*4, Setup_Alc[1],  1);//SaveData.Main_Func.Alc
//����	
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_MID;
	}
		
	LCD_DrawRect( LIST1+87, FIRSTLINE+SPACE1*4-2,SELECT_1END , FIRSTLINE+SPACE1*5-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*4, Setup_Rsou[SaveData.Main_Func.Rsou],  0);
	
//������ʱ
//	Black_Select=(Button_Page->index==6)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_MID;
//	}
//	//	SaveData.Main_Func.Trig_time=100000;
//	LCD_DrawRect( LIST1+87, FIRSTLINE+SPACE1*5-2,SELECT_1END , FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1
//	
////	sprintf((char *)DispBuf,"%d",SaveData.Main_Func.Trig_time.Num);
//	if(SaveData.Main_Func.Trig_time.Num<999)
//	{
//		sprintf((char *)DispBuf,"%d",SaveData.Main_Func.Trig_time.Num);
//		WriteString_16(LIST1+88+28, FIRSTLINE+SPACE1*5, "ms",  0);
//	}
//	else
//	{
//		Hex_Format(SaveData.Main_Func.Trig_time.Num , 3 , 6 , 0);
//		WriteString_16(LIST1+88+56+16, FIRSTLINE+SPACE1*5, "s",  0);
//	
//	
//	}

//		
//	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*5, DispBuf,  0);
	
	
//������ʱ
//	Black_Select=(Button_Page->index==8)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//	LCD_DrawRect( LIST1+87, FIRSTLINE+SPACE1*7-2,SELECT_1END , FIRSTLINE+SPACE1*8-4 , Colour.black ) ;
//	if(SaveData.Main_Func.Temp_time.Num<999)
//	{
//		sprintf((char *)DispBuf,"%d",SaveData.Main_Func.Temp_time.Num);
//		WriteString_16(LIST1+88+28, FIRSTLINE+SPACE1*7, "ms",  0);
//		
//	}
//	else
//	{
//		//sprintf((char *)DispBuf,"%6.3d",SaveData.Main_Func.Temp_time);
//		Hex_Format(SaveData.Main_Func.Temp_time.Num , 3 , 6 , 0);
//		WriteString_16(LIST1+88+56+16, FIRSTLINE+SPACE1*7, "s",  0);
//		
//	}
	
		
//	LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*7-2,SELECT_1END , FIRSTLINE+SPACE1*8-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*7, DispBuf,  0);
	
//Ѷ�칦��
	Black_Select=(Button_Page->index==6)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+87, FIRSTLINE-2,SELECT_2END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
	if(SaveData.Sys_Setup.Language==1)
	{
		WriteString_16(LIST2+88, FIRSTLINE, Setup_Beep_E[SaveData.Main_Func.beep],  0);
	}else if(SaveData.Sys_Setup.Language==0){
		WriteString_16(LIST2+88, FIRSTLINE, Setup_Beep[SaveData.Main_Func.beep],  0);
	}
	
	
//����
	Black_Select=(Button_Page->index==7)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1-2,SELECT_2END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
//	if(SaveData.Main_Func.Range.Auto)
//		WriteString_16(LIST2+88, FIRSTLINE, User_Range[0],  0);
//	else
	{
		
		WriteString_16(LIST2+88, FIRSTLINE+SPACE1, User_Range[SaveData.Main_Func.Range.Range],  0);
	
	}
	
//�ٶ�
	Black_Select=(Button_Page->index==8)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1*2-2,SELECT_2END , FIRSTLINE+SPACE1*3-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*2, User_Speed[SaveData.Main_Func.Speed],  1);
	
//ƫ��
//	if(index==12)
//		index=13;
//	Black_Select=(Button_Page->index==12)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1*2-2,SELECT_2END , FIRSTLINE+SPACE1*3-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*2, "----",  1);	

//ƽ��
	
//	Black_Select=(Button_Page->index==10)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_MID;
//	}
//		
//	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1*3-2,SELECT_2END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
//	sprintf((char *)DispBuf,"%d",SaveData.Main_Func.Avg);
//	//WriteString_16(LIST1+88+28, FIRSTLINE+SPACE1*7, "ms",  0);
//	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*3, DispBuf,  1);	
	
//V/I����
	
	Black_Select=(Button_Page->index==9)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_MID;
	}
		
	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1*3-2,SELECT_2END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*3, Setup_Vm[SaveData.Main_Func.V_i],  1);	
	
//DCR����
	
//	Black_Select=(Button_Page->index==15)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_MID;
//	}
//		
//	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1*5-2,SELECT_2END , FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*5, Setup_Dcr[SaveData.Main_Func.Dcr],  1);
////DC����
//	
//	Black_Select=(Button_Page->index==16)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1*6-2,SELECT_2END , FIRSTLINE+SPACE1*7-4 , Colour.black ) ;//SPACE1
//	
//	if(SaveData.Main_Func.DC_Range.Auto)
//		WriteString_16(LIST2+88, FIRSTLINE+SPACE1*6, User_Range[0],  0);
//	else
//	{
//		WriteString_16(LIST2+88, FIRSTLINE+SPACE1*6, User_Range[SaveData.Main_Func.DC_Range.Range+1],  0);
//	
//	}
//	
////DC��ƽ
//	
//	Black_Select=(Button_Page->index==17)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1*7-2,SELECT_2END , FIRSTLINE+SPACE1*8-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*7, "-----",  1);//SaveData.Main_Func.DC_Level
	
	//������A
	Black_Select=(Button_Page->index==10)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_MID;
	}
//	
//	sprintf((char *)DispBuf,"%d",SaveData.Main_Func.Temp_time.Num);
//	WriteString_16(LIST1+88+28, FIRSTLINE+SPACE1*7, "ms",  0);
			
	LCD_DrawRect( LIST2+87, FIRSTLINE+SPACE1*4-2,SELECT_2END , FIRSTLINE+SPACE1*5-4 , Colour.black ) ;//SPACE1
	//sprintf((char *)DispBuf,"%d",SaveData.Main_Func.Ref_A.Num);
	//Hex_Format(SaveData.Main_Func.Ref_A.Num , SaveData.Main_Func.Ref_A.Dot , 6 , 0);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*4, Sys_Buard_Value[SaveData.Main_Func.buad],  0);
	
	Disp_Fastbutton();
	switch(Button_Page->index)
	{
		case 0:
				Disp_Button_TestSet(Button_Page->page);
			break;
		case 1:
			if(Button_Page->page==0)
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				for(i=0;i<4;i++)
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, Button_TipPage1[i],  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE-10, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE+10, " 2/3",  0);
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "1/2",  0);
//				
			}
			else if(Button_Page->page==1)
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				for(i=0;i<4;i++)
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, Button_TipPage2[i],  0);
//				WriteString_16(84, 271-30, "Z-..",  0);
//				WriteString_16(84+80, 271-30, "Y-..",  0);
//				WriteString_16(84+80+80, 271-30, "R-..",  0);
//				WriteString_16(84+80+80+80, 271-30, "G_B",  0);
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "2/2",  0);
				
			
			
			}
//			else
//			{
//				Colour.Fword=White;
//				Colour.black=LCD_COLOR_TEST_BUTON;
//				WriteString_16(BUTTOM_X_VALUE+35, BUTTOM_Y_VALUE,Button_TipPage3[0],  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "3/3",  0);
//			
//			
//			}
		break;
		case 2:		//Ƶ��
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<4;i++)
			{
				if(i==1||i==2)
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, FreqButton_Tip[i],  0);
				else
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, FreqButton_Tip[i],  0);
			}
		break;
		case 3:		//��ƽ
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, LevleButton_Tip[i],  0);
			}
			break;
		case 4:		//����
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<4;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, Setup_Trig[i],  0);
			}
			break;
//		case 5:		//���ƽ
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				if(i==0)
//					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+25, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
//				else
//					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
//			}
//			break;
		case 5:		//����
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				if(i==0)
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, Setup_Rsou[i],  0);
				else
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, Setup_Rsou[i],  0);
			}
			break;
//		case 6:		//������ʱ
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
//		case 7:		//������ʱ
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
		case 6:		//Ѷ�칦��
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<3;i++)
			{
				if(i==2)
				     WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+10, BUTTOM_Y_VALUE, Setup_Beep[i],  0);
				else
					 WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+18, BUTTOM_Y_VALUE, Setup_Beep[i],  0);
			}
			break;
		case 7:	//����
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<4;i++)
			{
				if(i<2)
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+16, BUTTOM_Y_VALUE, RangeButton_Tip[i],  0);
				else 
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, RangeButton_Tip[i],  0);
			}
			break;
		case 8:	//�ٶ�
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<3;i++)
			{
				if(i==1)
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, SpeedButton_Tip[i],  0);
				else 
				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+16, BUTTOM_Y_VALUE, SpeedButton_Tip[i],  0);
			}
			break;
//		case 10:	//ƫ��
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				if(i==1)
//					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, User_Comp[i],  0);
//				else 
//					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+25, BUTTOM_Y_VALUE, User_Comp[i],  0);
//			}
//			break;
//		case 10:	//ƽ��
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
		case 9:	//V/I����
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				if(i==0)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+25, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
			}
			break;
//		case 15:	//DCR����
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, Setup_Dcr[i],  0);
//			}
//			break;
//		case 16:		//DC����
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<4;i++)
//			{
//				if(i<2)
//					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+16, BUTTOM_Y_VALUE, RangeButton_Tip[i],  0);
//				else
//				    WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, RangeButton_Tip[i],  0);
//			}
//			break;
//		case 17:		//DC��ƽ
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, SetupAvg_Button_Tip[i],  0);
//			}
//			break;
		case 10:	//������
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
//		case 19:
//			break;
//		case 20:
//			break;
		default:
			break;
		
			
	
	
	}
//	WriteString(SETNUM_POSXVALUE, SETNUM_POSYHIGH, p[SaveData.SetParameter.mainparameter], (index==1)?1:0);
//	WriteString(SETNUM_POSXVALUE, SETNUM_POSYHIGH*2, TESTPAGE_HZset[SaveData.SetParameter.frequency], (index==2)?1:0);
//	if(SaveData.SysParameter.language==0)
//		p=TESTPAGE_SPEEDset;
//	else
//		p=ENG_TESTPAGE_SPEEDset;
//	WriteString(SETNUM_POSXVALUE, SETNUM_POSYHIGH*3, p[SaveData.SetParameter.speed], (index==3)?1:0);
//	if(SaveData.SysParameter.language==0)
//		p=SETCOM_ITEMset;
//	else
//		p=ENG_SETCOM_ITEMset;
//	WriteString(SETNUM_POSXVALUE, SETNUM_POSYHIGH*5, p[SaveData.SetParameter.compare], (index==4)?1:0);
//	
//	
//	if(SaveData.SetParameter.Nominal_value.comp_value <0.001)
//		SaveData.SetParameter.Nominal_value.comp_value=0.001;

//	
//	if(SaveData.SetParameter.mainparameter>1)
//	{
//		if(SaveData.SetParameter.Nominal_value.comp_value>=10)
//		{
//			DispF(SaveData.SetParameter.Nominal_value.comp_value );//
//			Hex_Format(Float.Digit/100 , RANGE_DOT_R[Float.Exponent] , 5 , 1);  //u32 Digit;//β��
//			
//		}
//		else
//		{
//			
//			sprintf((char *)DispBuf,"%6.3f",SaveData.SetParameter.Nominal_value.comp_value);	
//			Float.Exponent=0;
//		
//		}
//	}
//	else 
//	{
//		if(SaveData.SetParameter.Nominal_value.comp_value>=10)
//		{
//		
//			DispF(SaveData.SetParameter.Nominal_value.comp_value );
//			Hex_Format((Float.Digit)/100 , RANGE_DOTSET[SaveData.SetParameter.mainparameter][Float.Exponent], \
//			RANGE_LENTHSET[Float.Exponent] , 1);		
//		}
//		else
//		{
//			sprintf((char *)DispBuf,"%6.2f",SaveData.SetParameter.Nominal_value.comp_value);	
//			Float.Exponent=0;
//		
//		}
//	}
//	
//	WriteString(SETNUM_POSXVALUE, SETNUM_POSYHIGH*6, DispBuf, (index==5)?1:0);
//	
//	WriteString(SETNUM_POSXVALUE+48, SETNUM_POSYHIGH*6,	\
//	 COM_UNIT[SaveData.SetParameter.mainparameter][RANGE_UNIT[Float.Exponent]], 0);

//	
//	
//	if(SaveData.SysParameter.language==0)
//			p=SETPAGE_SECONDARYPARAMETERset;
//		else
//		p=ENG_SETPAGE_SECONDARYPARAMETERset;
//	WriteString(SETNUM_POSXVALUE2, SETNUM_POSYHIGH*1, \
//	p[SaveData.SetParameter.Secondary_parameter.vlaue], (index==6)?1:0);
//	if(SaveData.SysParameter.language==0)
//		p=TESTPAGE_RANGEITEMset;
//	else
//		p=ENG_TESTPAGE_RANGEITEMset;
//	if(SaveData.SetParameter.ranger.flag==0)
//		WriteString(SETNUM_POSXVALUE2, SETNUM_POSYHIGH*2, p[0], (index==7)?1:0);
//	else
//	WriteString(SETNUM_POSXVALUE2, SETNUM_POSYHIGH*2, p[SaveData.SetParameter.ranger.range], (index==7)?1:0);
//	if(SaveData.SysParameter.language==0)
//		p=TESTmanue_ITEMset;
//	else
//		p=ENG_TESTmanue_ITEMset;
//	WriteString(SETNUM_POSXVALUE2, SETNUM_POSYHIGH*3, p[SaveData.SetParameter.Equivalent_mode], (index==8)?1:0);
//	if(SaveData.SysParameter.language==0)
//		p=SETBEEP_SPEEDset;
//	else
//		p=ENG_SETBEEP_SPEEDset;
//	WriteString(SETNUM_POSXVALUE2, SETNUM_POSYHIGH*5, p[SaveData.SetParameter.beep], (index==9)?1:0);//��λ��û��Ū
//	Hex_Format(SaveData.SetParameter.Relative_deviation.value, 1 , 3 , 0);
//	WriteString(SETNUM_POSXVALUE2, SETNUM_POSYHIGH*6, DispBuf, (index==10)?1:0);
//	WriteString(SETNUM_POSXVALUE2+24,SETNUM_POSYHIGH*6,"%",0);
//	POINT_COLOR=Grey;
//	if(SaveData.SysParameter.language==0)
//		p1=SETUPTIP_DISP;
//	else
//		p1=ENG_SETUPTIP_DISP;
//	WriteString(0, 180, p1[index], 0);
//	BACK_COLOR=blue;
//	flag=0;
//	//��F��������ʾ����
//	switch(index)
//	{
//		case 0:	
//		if(SaveData.SysParameter.language==0)
//			p=SETPAGEone_ITEM;
//		else
//			p=ENG_SETPAGEone_ITEM;		
//		POINT_COLOR=red;
//			for(i=0;i<=SetcomParameter[index][1];i++)
//			{
//				xpose=Disp_position(i);
//				WriteString(xpose, 215,p[i], flag);
//			}
//			if(SaveData.SysParameter.language==0)
//			{
//			WriteString(155, 205,"��·", flag);
//			WriteString(155, 221,"У׼", flag);
//			WriteString(230, 205,"��·", flag);
//			WriteString(230, 221,"У׼", flag);
//			}
//			else
//			{
//			WriteString(20, 205,"MEAS", flag);
//			WriteString(6, 221,"DISPLAY", flag);
//			WriteString(155, 205,"OPEN", flag);
//			WriteString(155, 221,"CORR", flag);
//			WriteString(230, 205,"SHORT", flag);
//			WriteString(230, 221,"CORR", flag);
//			}
//			break;
//			
//		case 1:	
//			if(SaveData.SysParameter.language==0)
//				p=TESTPAGE_ITEM;
//			else
//				p=ENG_TESTPAGE_ITEM;	
//			for(i=0;i<=SetcomParameter[index][1];i++)
//			{
//				POINT_COLOR=(SaveData.SetParameter.mainparameter==i)?yellow:red;
//				WriteString(Disp_position(i), 215,p[i], flag);
//			}
//			break;
//		case 2:
//			
//		for(i=0;i<=SetcomParameter[index][1];i++)
//			{
//				POINT_COLOR=(SaveData.SetParameter.frequency==i)?yellow:red;
//				WriteString(Disp_position(i), 215,TESTPAGE_HZ[i], flag);
//			}
//			
//			break;
//		case 3:
//			if(SaveData.SysParameter.language==0)
//				p=TESTPAGE_SPEED;
//			else
//				p=ENG_TESTPAGE_SPEED;
//			for(i=0;i<=SetcomParameter[index][1];i++)
//			{
//				POINT_COLOR=(SaveData.SetParameter.speed==i)?yellow:red;
//				WriteString(Disp_position(i), 215,p[i], flag);
//			}
//			break;
//		case 4:
//			if(SaveData.SysParameter.language==0)
//				p=SETCOM_ITEM;
//			else
//				p=ENG_SETCOM_ITEM;	
//			for(i=0;i<=SetcomParameter[index][1];i++)
//			{
//				POINT_COLOR=(SaveData.SetParameter.compare==i)?yellow:red;
//				WriteString(Disp_position(i), 215,p[i], flag);
//			}
//			
//			break;
//		case 5:
//			
//		//for(i=0;i<=SetcomParameter[index][1];i++)
//			{
//				POINT_COLOR=red;//(SaveData.SetParameter.Nominal_value.flag==0)?yellow:red;
//				if(SaveData.SysParameter.language==0)
//				{
//					WriteString(Disp_position(0)+5, 209,"����", 0);
//					WriteString(Disp_position(0)+5, 221,"����", 0);
//					//POINT_COLOR=(SaveData.SetParameter.Nominal_value.flag==1)?yellow:red;
//					WriteString(Disp_position(1), 209,"����", flag);
//					WriteString(Disp_position(1), 221,"����", flag);
//				}
//				else
//				{
//					 WriteString(Disp_position(0), 209,"KEYPAD", 0);
//					WriteString(Disp_position(0)+5, 221,"INPUT", 0);
//					//POINT_COLOR=(SaveData.SetParameter.Nominal_value.flag==1)?yellow:red;
//					WriteString(Disp_position(1), 209,"MEAS", flag);
//					WriteString(Disp_position(1), 221,"INPUT", flag);
//				
//				}
//			}
//			break;
//		case 6:
//			if(SaveData.SysParameter.language==0)
//			{
//					p=SETPAGE_SECONDARYPARAMETER;
//					WriteString(290, 205,"����", flag);
//				}
//				else
//				{
//					p=ENG_SETPAGE_SECONDARYPARAMETER;
//					WriteString(290, 205,"MORE", flag);
//					} 
//			if(SaveData.SetParameter.Secondary_parameter.flag==0)
//			{
//			
//			for(i=0;i<=4;i++)//SetcomParameter[index+2][1]
//				{
//					POINT_COLOR=(SaveData.SetParameter.Secondary_parameter.vlaue==i)?yellow:red;
//					WriteString(Disp_position(i),215,p[i], flag);
//						
//				}
//			}
//			else
//				for(i=5;i<=9-1;i++)
//			{
//				
//				POINT_COLOR=(SaveData.SetParameter.Secondary_parameter.vlaue==i)?yellow:red;								
//				WriteString(Disp_position(i-5), 215,p[i], flag);
//			}
//		
//		
//			POINT_COLOR=(SaveData.SetParameter.Secondary_parameter.vlaue==4)?yellow:red;	
//			if(SaveData.SetParameter.Secondary_parameter.flag==0)
//			{
//				if(SaveData.SysParameter.language==0)
//				{
//				WriteString(290, 205,"����", flag);
//				}
//				else
//				{
//					WriteString(290, 205,"MORE", flag);
//				
//				}
//				WriteString(290, 223,"1/2", flag);
//			}
//			POINT_COLOR=(SaveData.SetParameter.Secondary_parameter.vlaue==9)?yellow:red;	
//			if(SaveData.SetParameter.Secondary_parameter.flag!=0)
//			{
//			
//			WriteString(290, 223,"2/2", flag);
//			}
////		 if(SaveData.SysParameter.language==0)
////				p=SETPAGE_SECONDARYPARAMETER;
////			else
////				p=ENG_SETPAGE_SECONDARYPARAMETER;
////		if(SaveData.SetParameter.Secondary_parameter.flag==0)
////		{
////		for(i=0;i<=SetcomParameter[index][1];i++)
////			{
////				POINT_COLOR=(SaveData.SetParameter.Secondary_parameter.vlaue==i)?yellow:red;								
////				WriteString(Disp_position(i), 215,p[i], flag);
////			}
////		}
////		else
////		{
////		for(i=5;i<=9-1;i++)
////			{
////				POINT_COLOR=(SaveData.SetParameter.Secondary_parameter.vlaue==i)?yellow:red;								
////				WriteString(Disp_position(i-5), 215,p[i], flag);
////			}
////		
////		}
////			POINT_COLOR=(SaveData.SetParameter.Secondary_parameter.vlaue==4)?yellow:red;
////			if(SaveData.SysParameter.language==0)
////			{
////				WriteString(290, 205,"����", flag);
////			}
////			else
////			{
////			 WriteString(290, 205,"MORE", flag);
////			
////			}	
////			if(SaveData.SetParameter.Secondary_parameter.flag==0)
////			{
////				
////			
////			WriteString(290, 223,"1/2", flag);
////			}
////			else
////			{
////			WriteString(290, 223,"2/2", flag);
////			}
//			
//			break;
//		case 7:
//			if(SaveData.SysParameter.language==0)
//				p=TESTPAGE_RANGEITEM;
//			else
//				p=ENG_TESTPAGE_RANGEITEM;	
//		for(i=0;i<=SetcomParameter[index][1];i++)
//			{
//				
//				//if(SaveData.SetParameter.ranger.flag==1)
//				//	POINT_COLOR=red;
//				//else					
//				POINT_COLOR=(SaveData.SetParameter.ranger.flag==i)?yellow:red;		
//				WriteString(Disp_position(i), 215,p[i], flag);
//			}
//			break;
//			case 8:
//			 if(SaveData.SysParameter.language==0)
//				p=TESTmanue_ITEM;
//			else
//				p=ENG_TESTmanue_ITEM;
//		for(i=0;i<=SetcomParameter[index][1];i++)
//			{
//				POINT_COLOR=(SaveData.SetParameter.Equivalent_mode==i)?yellow:red;	
//				WriteString(Disp_position(i), 215,p[i], flag);
//			}
//			break;
//		case 9:
//		if(SaveData.SysParameter.language==0)
//				p=SETBEEP_SPEED;
//			else
//				p=ENG_SETBEEP_SPEED;	
//		for(i=0;i<=SetcomParameter[index][1];i++)
//			{
//				POINT_COLOR=(SaveData.SetParameter.beep==i)?yellow:red;					
//				WriteString(Disp_position(i), 215,p[i], flag);
//			}
//			break;
//		case 10:
//		if(SaveData.SysParameter.language==0)
//				
//				WriteString(Disp_position(0), 215,SETBASE_ITEM[0], flag);
//			else
//			{
//				WriteString(Disp_position(0), 209,"KEYPAD", 0);
//				WriteString(Disp_position(0)+5, 221,"INPUT", 0);
//				
//			}		
//		
//		POINT_COLOR=yellow;		
//		if(SaveData.SetParameter.Relative_deviation.flag)
//		{	
//			POINT_COLOR=red;
//			for(i=1;i<=4;i++)
//				WriteString(Disp_position(i), 215,SETBASE_ITEM[i], flag);
//			
//		
//		}
//		else
//		{
//			POINT_COLOR=(SaveData.SetParameter.Relative_deviation.value==1)?yellow:red;
//			WriteString(Disp_position(1), 215,SETBASE_ITEM[1], flag);
//			POINT_COLOR=(SaveData.SetParameter.Relative_deviation.value==5)?yellow:red;
//			WriteString(Disp_position(2), 215,SETBASE_ITEM[2], flag);
//			POINT_COLOR=(SaveData.SetParameter.Relative_deviation.value==10)?yellow:red;
//			WriteString(Disp_position(3), 215,SETBASE_ITEM[3], flag);
//			POINT_COLOR=(SaveData.SetParameter.Relative_deviation.value==25)?yellow:red;
//			WriteString(Disp_position(4), 215,SETBASE_ITEM[4], flag);
//		
//		
//		}
////		for(i=1;i<=SetcomParameter[index][1];i++)
////			{
////				if(SaveData.SetParameter.Relative_deviation.flag)
////					POINT_COLOR=red;
////				else
////					POINT_COLOR=(SaveData.SetParameter.Relative_deviation.value==i)?yellow:red;
////				WriteString(Disp_position(i), 215,SETBASE_ITEM[i], flag);
////			}
//			break;	
//	}
//	while(0xff!=scan_key());
}




//�������ð���ֵ
void Disp_LimitSEt_value(Button_Page_Typedef* Button_Page)
{
//	uint32_t flag;
	uint32_t i;
//	uint32_t xpose;
	uint32_t Black_Select;
//	uint32_t Select_color;
//	if(SaveData.Main_Func.V_i==1)
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//		WriteString_16(30, 210, User_Comp[1],  1);
//		WriteString_16(30+120, 210, User_Comp[1],  1);
//		
//	}
//	
	Colour.black=LCD_COLOR_TEST_BACK;

	if(SaveData.Limit_Tab.Mode==0)//������
	{
		Disp_Set_Unit_12(SaveData.Main_Func.Param.test,330+40,65-2);
		Disp_Set_Unit_12(SaveData.Main_Func.Param.test,160+40,65-2);
		
	}
	else
	{
		WriteString_12(330+40, 65-2, "%",  0);	
		WriteString_12(160+40, 65-2, "%",  0);
		
	}
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+69, FIRSTLINE-2,SELECT_1END-39 , FIRSTLINE+SPACE1-6 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+70, FIRSTLINE-2, User_FUNC[SaveData.Main_Func.Param.test],  1);//�����㷨  ��˳��Ĺ���
	
//���
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	//WriteString_16(LIST1+60, FIRSTLINE, User_Freq[SaveData.Main_Func.Freq],  1);//��ʾ����
	Hex_Format(SaveData.Limit_Tab.Nom.Num , SaveData.Limit_Tab.Nom.Dot , 5 , 0);//�ӵ�λ
	LCD_DrawRect( LIST2-24, FIRSTLINE-2,SELECT_2END-123+6 , FIRSTLINE+16, Colour.black ) ;
	WriteString_16(LIST2-21, FIRSTLINE-2, DispBuf,  1);//��ʾ���ֵ  ����Ҫ������ʾ��λ
	WriteString_16(LIST2+47, FIRSTLINE-2, Disp_Unit[SaveData.Limit_Tab.Nom.Unit],  1);
	Disp_Set_Unit_16(SaveData.Main_Func.Param.test,SELECT_2END-135,FIRSTLINE-2 );
	
	
//��ʽ
	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+149, FIRSTLINE-2,SELECT_2END+35 , FIRSTLINE+SPACE1*1 -6, Colour.black ) ;//SPACE1
	WriteString_16(LIST2+150, FIRSTLINE-2, User_LimitList_Value[SaveData.Limit_Tab.Mode],  1);
//����
	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+69, FIRSTLINE+SPACE1-4,SELECT_1END-39 , FIRSTLINE+SPACE1*2-8 , Colour.black ) ;//SPACE1
//	if(SaveData.Main_Func.Range.Auto)
		WriteString_16(LIST1+70, FIRSTLINE+SPACE1-4, Switch_Value[SaveData.Limit_Tab.Param],  0);
//	else
//	{
//		WriteString_16(LIST1+88, FIRSTLINE, User_Range[SaveData.Main_Func.Range.Range+1],  0);
//	
//	}
	
//�Ƚ�
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2-22, FIRSTLINE+SPACE1-4,SELECT_2END-130 , FIRSTLINE+SPACE1*2-8 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2-21, FIRSTLINE+SPACE1-4, Switch_Value[SaveData.Limit_Tab.Comp],  1);
	for(i=6;i<16;i++)
	{
		//SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low
		if(i==Button_Page->index)
			Colour.black=LCD_COLOR_SELECT;
		else
			if(i-6>2&&i-6<7)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
		LCD_DrawRect( LIST2-90-12, 76+(i-6)*15,LIST2-20 , 77+(i-6)*15+15 , Colour.black ) ;
		if(SaveData.Limit_Tab.Comp_Value[i-6].low.Num!=0)
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[i-6].low.Num , 
			SaveData.Limit_Tab.Comp_Value[i-6].low.Dot , 5 , 0);//�ӵ�λ
			
//			WriteString_12(LIST2-90-8, 76+(i-6)*15, "-",  1);
			WriteString_12(LIST2-90, 76+(i-6)*15, DispBuf,  1);//��ʾ���ֵ  ����Ҫ������ʾ��λ
			if(SaveData.Limit_Tab.Mode==0)
			WriteString_12(LIST2-42, 76+(i-6)*15, 
			Disp_Unit[SaveData.Limit_Tab.Comp_Value[i-6].low.Unit],  1);
			//WriteString_16(LIST1+70, FIRSTLINE+SPACE1, Switch_Value[SaveData.Limit_Tab.Param],  0);
			//WriteString_16(LIST2+34, FIRSTLINE, Disp_Unit[SaveData.Limit_Tab.Nom.Unit],  1);
		}
		else
		WriteString_12(LIST2-90, 76+(i-6)*15, "------",  1);	
	}
	for(i=16;i<26;i++)
	{
		
		if(i==Button_Page->index)
			Colour.black=LCD_COLOR_SELECT;
		else
			if(i-16>2&&i-16<7)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
			
		LCD_DrawRect( LIST2+58, 76+(i-16)*15,LIST2+70+70 , 77+(i-16)*15+15, Colour.black ) ;
		if(SaveData.Limit_Tab.Comp_Value[i-16].high.Num !=0)
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[i-16].high.Num ,
				SaveData.Limit_Tab.Comp_Value[i-16].high.Dot , 5 , 0);//�ӵ�λ
			
			WriteString_12(LIST2+80, 76+(i-16)*15, DispBuf,  0);//��ʾ���ֵ  ����Ҫ������ʾ��λ
			if(SaveData.Limit_Tab.Mode==0)
			WriteString_12(LIST2+70+58, 76+(i-16)*15, 
			Disp_Unit[SaveData.Limit_Tab.Comp_Value[i-16].high.Unit],  1);
		}
		else
			WriteString_12(LIST2+80, 76+(i-16)*15, "------",  1);
	}

		
//	LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1*2-2,SELECT_2END , FIRSTLINE+SPACE1*3-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*2, User_Comp[SaveData.Main_Func.Bias],  1);
	Disp_Fastbutton();
	
	switch(Button_Page->index)
	{
		case 0:
				Disp_Button_TestSet(Button_Page->page);
			break;
		case 1:

		break;
		case 2:

		break;
		case 3:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				if(i==0)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, User_LimitList_Value[i],  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+30, BUTTOM_Y_VALUE, User_LimitList_Value[i],  0);
			}
			break;

		case 4:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				if(i==0)
				    WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, Switch_Value[i],  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+25, BUTTOM_Y_VALUE, Switch_Value[i],  0);
			}
			break;
		case 5:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				if(i==0)
				    WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+20, BUTTOM_Y_VALUE, Switch_Value[i],  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+25, BUTTOM_Y_VALUE, Switch_Value[i],  0);
			}
			break;
//		case 6:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, User_Comp[i],  0);
//			}
//			break;
		default:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			if(SaveData.Sys_Setup.Language==1)
			{
				WriteString_16(BUTTOM_X_VALUE+9, BUTTOM_Y_VALUE, "Cel R",  0);
				WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+1, "CelList",  0);
			}else if(SaveData.Sys_Setup.Language==0){
				WriteString_16(BUTTOM_X_VALUE+9, BUTTOM_Y_VALUE, "�����",  0);
				WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+1, "�������",  0);
			}
			
//			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+10, "����",  0);
		break;
		
			

		
			
	
	
	}

}
//��ʾ���ò�����ֵSetup_Valueall   //�û�У���еİ���ֵ����
void Disp_Debug_value(Button_Page_Typedef* Button_Page)
{
//	uint32_t flag;
	uint32_t i;
//	uint32_t xpose;
	uint32_t Black_Select;
//	uint32_t Select_color;

	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88, FIRSTLINE, Switch_Value[SaveData.User_Correction.Open],  1);//��·
	
//��·
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1-2,SELECT_1END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1, Switch_Value[SaveData.User_Correction.Short],  1);
	
	
////����
//	Black_Select=(Button_Page->index==3)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*2-2,SELECT_1END , FIRSTLINE+SPACE1*3 -4, Colour.black ) ;//SPACE1
//	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*2, Switch_Value[SaveData.User_Correction.Load],  1);
//	
////У����
//	Black_Select=(Button_Page->index==4)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*3-2,SELECT_1END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
//	sprintf((char *)DispBuf,"%3d",SaveData.User_Correction.Short);
//	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*3, DispBuf,  1);	
//	
////Ƶ��
////	if(index==5)
////		index=6;
//	Black_Select=(Button_Page->index==5)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*4-2,SELECT_1END , FIRSTLINE+SPACE1*5-4 , Colour.black ) ;//SPACE1
//	if(SaveData.User_Correction.Freq==1)//��
//		WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*4, "1.00000KHz",  1);
//	else
//		WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*4, "----------",  1);
//		
////�ο�A
//	Black_Select=(Button_Page->index==6)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*5-2,SELECT_1END , FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1
//	if(SaveData.User_Correction.Freq==0)
//	{
//		sprintf((char *)DispBuf,"%3d",SaveData.User_Correction.Ref_A.Num);
//		WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*5,DispBuf,  0);
//		//��ʾ��λ
//	}
//	else
//	{
//		WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*5,"----------",  0);
//	}
//	
////��·A
//	Black_Select=(Button_Page->index==7)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		SaveData.Main_Func.Trig_time.Num=100000;
//	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*6-2,SELECT_1END , FIRSTLINE+SPACE1*7-4 , Colour.black ) ;//SPACE1
////	if(SaveData.Main_Func.Trig_time<999)
//	{
////		sprintf((char *)DispBuf,"%d",SaveData.Main_Func.Trig_time);
//		WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*6, "0.00000uS",  0);
//		
//	}
////	else
////	{
////		//sprintf((char *)DispBuf,"%6.3d",SaveData.Main_Func.Trig_time);
////		Hex_Format(SaveData.Main_Func.Trig_time , 3 , 6 , 0);
////		WriteString_16(LIST1+88+56+16, FIRSTLINE+SPACE1*6, "s",  0);
////		
////	}
//		
//	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*6, DispBuf,  0);
//	
//	
////��·A
//	Black_Select=(Button_Page->index==8)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
////	if(SaveData.Main_Func.Trig_time<999)
//	{
//		sprintf((char *)DispBuf,"%d",SaveData.User_Correction.Short_A.Num);
//		//WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*7, "ms",  0);
//		
//	}
////	else
////	{
////		//sprintf((char *)DispBuf,"%6.3d",SaveData.Main_Func.Temp_time);
////		Hex_Format(SaveData.Main_Func.Trig_time , 3 , 6 , 0);
////		WriteString_16(LIST1+88+56+16, FIRSTLINE+SPACE1*7, "s",  0);
////		
////	}
//	
//		
//	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*7-2,SELECT_1END , FIRSTLINE+SPACE1*8-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*7, DispBuf,  0);
//	
////����A
//	Black_Select=(Button_Page->index==9)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*8-2,SELECT_1END , FIRSTLINE+SPACE1*9-4 , Colour.black ) ;//SPACE1
//	sprintf((char *)DispBuf,"%d",SaveData.User_Correction.Load_A.Num);
//	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*8, DispBuf,  1);
//	
////����
//	Black_Select=(Button_Page->index==10)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST2+88, FIRSTLINE-2,SELECT_2END , FIRSTLINE+SPACE1*1-4 , Colour.black ) ;//SPACE1

//	WriteString_16(LIST2+88, FIRSTLINE, "0m",  0);
//	
//	
////��ʽ
////	Black_Select=(Button_Page->index==11)?1:0;
////	if(Black_Select)
////	{
////		Colour.black=LCD_COLOR_SELECT;
////	
////	}
////	else
////	{
//		Colour.black=LCD_COLOR_TEST_BACK;
////	}
//		
////	LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1-2,SELECT_2END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST2+88, FIRSTLINE+SPACE1, "Single",  1);
//	
////����
////	if(index==12)
////		index=13;
//	Black_Select=(Button_Page->index==11)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1*2-2,SELECT_2END , FIRSTLINE+SPACE1*3-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*2, User_Comp[SaveData.Main_Func.Bias],  1);	
	Disp_Fastbutton();
	switch(Button_Page->index)
	{
		case 0:
				Disp_Button_TestSet(Button_Page->page);
			break;
		case 1:
			if(Button_Page->page==0)
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				for(i=0;i<2;i++)
				{
					if(i==0)
				      WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, User_Check_Opencircuit[i],  0);
					else 
					  WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+24, BUTTOM_Y_VALUE, User_Check_Opencircuit[i],  0);				
				}
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+13, BUTTOM_Y_VALUE, Button_TipPage1[i],  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "1/3",  0);
//				
			}
			else if(Button_Page->page==1)
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				for(i=0;i<4;i++)
				{
				  if(i==3)
					 WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, Button_TipPage2[i],  0); 
				  else
				     WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, Button_TipPage2[i],  0);
				}
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "2/3",  0);
				
			
			
			}
			else
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE+19,Button_TipPage3[0],  0);
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "3/3",  0);
			
			
			}
		break;
		case 2:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
					if(i==0)
				      WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, User_Check_Shortcircuit[i],  0);
					else 
					  WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+24, BUTTOM_Y_VALUE, User_Check_Shortcircuit[i],  0);	
//				if(i==1||i==2)
//				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, FreqButton_Tip[i],  0);
//				else
//				   WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, FreqButton_Tip[i],  0);
			}
		break;
		case 3:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, LevleButton_Tip[i],  0);
			}
			break;
		case 4:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<4;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Setup_Trig[i],  0);
			}
			break;
		case 5:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
			}
			break;
		case 6:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Setup_Rsou[i],  0);
			}
			break;
		case 7:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 8:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 9:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<3;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, SetupRev_A_Button_Tip[i],  0);
			}
			break;
		case 10:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<4;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, RangeButton_Tip[i],  0);
			}
			break;
		case 11:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<3;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, SpeedButton_Tip[i],  0);
			}
			break;
		case 12:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, User_Comp[i],  0);
			}
			break;
		case 13:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 14://
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
			}
			break;
		case 15:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Setup_Dcr[i],  0);
			}
			break;
		case 16:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<4;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, RangeButton_Tip[i],  0);
			}
			break;
		case 17:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<4;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Setup_Dcr[i],  0);
			}
			break;
		case 18:
			break;
		case 19:
			break;
		case 20:
			break;
		default:
			break;
		
			
	
	
	}

}
//��ʾ���ò�����ֵSetup_Valueall		//ϵͳ�����е�������
void Disp_Sys_value(Button_Page_Typedef* Button_Page)
{
//	uint32_t flag;
	uint32_t i;
//	uint32_t xpose;
	uint32_t Black_Select;
//	uint32_t Select_color;

	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+94, FIRSTLINE,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
	if(SaveData.Sys_Setup.Language==1)
	{
		WriteString_16(LIST1+95, FIRSTLINE+2, "Brid",  0);//
	}else if(SaveData.Sys_Setup.Language==0){
		WriteString_16(LIST1+95, FIRSTLINE+2, "����",  0);//
	}
	
	
//�ϸ�Ѷ��
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+94, FIRSTLINE+SPACE1,SELECT_1END , FIRSTLINE+SPACE1*2-4 , Colour.black );//SPACE1
	WriteString_16(LIST1+95, FIRSTLINE+SPACE1+2, Switch_Value[Saveeeprom.Sys_set.key_board],  1);
	
	
//���ϸ�Ѷ��
	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+94, FIRSTLINE+SPACE1*2,SELECT_1END , FIRSTLINE+SPACE1*3 -4, Colour.black ) ;//SPACE1
	WriteString_16(LIST1+95, FIRSTLINE+SPACE1*2+2, Switch_Value[Saveeeprom.Sys_set.U_flag],  1);
	
//��ʾ����
	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+94, FIRSTLINE+SPACE1*3,SELECT_1END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
//	sprintf((char *)DispBuf,"%3d",SaveData.User_Correction.Short);
	WriteString_16(LIST1+95, FIRSTLINE+SPACE1*3+2, Sys_Language_Value[SaveData.Sys_Setup.Language],  0);	
	
//����
//	if(index==5)
//		index=6;
//	Black_Select=(Button_Page->index==5)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST1+94, FIRSTLINE+SPACE1*4,SELECT_1END +3, FIRSTLINE+SPACE1*5-4 , Colour.black ) ;//SPACE1
//	if(SaveData.Sys_Setup.Password==1)//��
//		WriteString_16(LIST1+95, FIRSTLINE+SPACE1*4+2, "�ر�",  0);
//	else
//		WriteString_16(LIST1+95, FIRSTLINE+SPACE1*4, "----------",  1);
		
//�˵�����
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+94, FIRSTLINE+SPACE1*4,SELECT_1END , FIRSTLINE+SPACE1*5-4 , Colour.black ) ;//SPACE1
	//if(SaveData.Sys_Setup.Menu_Disp==0)
	//{
		//sprintf((char *)DispBuf,"%3d",SaveData.User_Correction.Ref_A);
		WriteString_16(LIST1+95, FIRSTLINE+SPACE1*4+2,"HOLD",  0);
		//��ʾ��λ
//	}
//	else
//	{
//		WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*5,"----------",  0);
//	}
	
//��
	Black_Select=(Button_Page->index==6)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Data_Value.Year=17;
	LCD_DrawRect( LIST1+94, FIRSTLINE+SPACE1*5,LIST1+90+25 , FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1

	Hex_Format(RTC_TIME_DISP.YEAR , 0 , 2 , 1);
//	WriteString_16(LIST1+90, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
	WriteString_16(LIST1+95, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(LIST1+95+20, FIRSTLINE+SPACE1*5+2, "-",  0);
//��
	
	Black_Select=(Button_Page->index==7)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Data_Value.Mon=17;
	LCD_DrawRect( LIST1+94+30, FIRSTLINE+SPACE1*5,LIST1+90+25+30 , FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1

	Hex_Format(RTC_TIME_DISP.MONTH , 0 , 2 , 1);
	WriteString_16(LIST1+95+30, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(LIST1+95+20+30, FIRSTLINE+SPACE1*5+2, "-",  0);
	
	
	//��
	
	Black_Select=(Button_Page->index==8)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Data_Value.data=17;
	LCD_DrawRect( LIST1+94+30+32, FIRSTLINE+SPACE1*5,LIST1+90+25+30+32 , FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1

	Hex_Format(RTC_TIME_DISP.DOM , 0 , 2 , 1);
    DispBuf[2]=0;
	WriteString_16(LIST1+95+30+32, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
    Colour.black=LCD_COLOR_TEST_BACK;
    
    WriteString_16(LIST1, FIRSTLINE+SPACE1*6+2, "Fact No.:",  0);
    WriteString_16(LIST1+94, FIRSTLINE+SPACE1*6+2, Saveeeprom.fac_num,  0);
    //Saveeeprom.fac_num;
	
//����
//	Black_Select=(Button_Page->index==10)?1:0;
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//		
//	LCD_DrawRect( LIST2+94, FIRSTLINE,SELECT_2END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
//	if(SaveData.Sys_Setup.Cipher==0)
//		WriteString_16(LIST2+95, FIRSTLINE+2, "------",  0);	
//	else
//	{
//		sprintf((char *)DispBuf,"%2d",SaveData.Sys_Setup.Cipher);
//		WriteString_16(LIST2+95, FIRSTLINE+2,DispBuf,0);
//	}

	

	
//������
	Black_Select=(Button_Page->index==9)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+94, FIRSTLINE,SELECT_2END , FIRSTLINE+SPACE1*1-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2+95, FIRSTLINE+2, Sys_Buard_Value[Saveeeprom.Sys_set.buard],  0);
	
//ƫ��Դ
	
	Black_Select=(Button_Page->index==10)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+94, FIRSTLINE+SPACE1*1,SELECT_2END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2+95, FIRSTLINE+SPACE1*1+2, Sys_Bais_Value[SaveData.Sys_Setup.Bias],  1);	
	
	//���߷�ʽ
	
	Black_Select=(Button_Page->index==11)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+94, FIRSTLINE+SPACE1*2,SELECT_2END , FIRSTLINE+SPACE1*3-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2+95, FIRSTLINE+SPACE1*2+2, Sys_Addr_value\
				[SaveData.Sys_Setup.Bus_Mode],  1);	
	
//GPIB��ַ
	
	Black_Select=(Button_Page->index==12)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+94, FIRSTLINE+SPACE1*3,SELECT_2END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
	sprintf((char *)DispBuf,"%2d",SaveData.Sys_Setup.GP_Addr);
	WriteString_16(LIST2+95, FIRSTLINE+SPACE1*3+2,DispBuf ,  1);	
	

//ֻ��
	
	Black_Select=(Button_Page->index==13)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+94, FIRSTLINE+SPACE1*4,SELECT_2END , FIRSTLINE+SPACE1*5-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2+95, FIRSTLINE+SPACE1*4+2, Switch_Value\
				[SaveData.Sys_Setup.Talk_Only],  1);


//ʱ
	Black_Select=(Button_Page->index==14)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Timer_Value.Hour=1;
	LCD_DrawRect( LIST2+94, FIRSTLINE+SPACE1*5,LIST2+90+25, FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1

	Hex_Format(RTC_TIME_DISP.HOUR , 0 , 2 , 1);
	WriteString_16(LIST2+95, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(LIST2+95+20, FIRSTLINE+SPACE1*5+2, ":",  0);
//��
	
	Black_Select=(Button_Page->index==15)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Timer_Value.Min=17;
	LCD_DrawRect( LIST2+94+30, FIRSTLINE+SPACE1*5,LIST2+90+25+30 , FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1

	Hex_Format(RTC_TIME_DISP.MIN , 0 , 2 , 1);
	WriteString_16(LIST2+95+30, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(LIST2+95+20+30, FIRSTLINE+SPACE1*5+2, ":",  0);
	
	
	//��
	
	Black_Select=(Button_Page->index==16)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Timer_Value.Sec=17;
	LCD_DrawRect( LIST2+94+30+32, FIRSTLINE+SPACE1*5,LIST2+90+25+30+32 , FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1

	Hex_Format(RTC_TIME_DISP.SEC , 0 , 2 , 1);
    DispBuf[2]=0;
	WriteString_16(LIST2+95+30+32, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
	
	Disp_Fastbutton();
	switch(Button_Page->index)
	{
		case 0:
				Disp_Button_SysSet();
			break;
		case 1:
//			if(Button_Page->page==0)
//			{
//				Colour.Fword=White;
//				Colour.black=LCD_COLOR_TEST_BUTON;
//				for(i=0;i<4;i++)
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Button_TipPage1[i],  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE-10, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE+10, " 1/3",  0);
////				
//			}
//			else if(Button_Page->page==1)
//			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
//				for(i=0;i<4;i++)
			//	WriteString_16(BUTTOM_X_VALUE+18, BUTTOM_Y_VALUE, "����",  0);
		if(SaveData.Sys_Setup.Language ==1)
		{
			WriteString_16(BUTTOM_X_VALUE+18, BUTTOM_Y_VALUE, "Brid",  0);
		}else if(SaveData.Sys_Setup.Language ==0){
			WriteString_16(BUTTOM_X_VALUE+18, BUTTOM_Y_VALUE, "����",  0);
		}
        
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE-10, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE+10, " 2/3",  0);
				
			
			
//			}
//			else
//			{
//				Colour.Fword=White;
//				Colour.black=LCD_COLOR_TEST_BUTON;
//				WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE,Button_TipPage3[0],  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE-10, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE+10, " 3/3",  0);
//			
//			
//			}
		break;
		case 2:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				if(i==0)
					WriteString_16(BUTTOM_X_VALUE+19, BUTTOM_Y_VALUE, User_Comp[i],  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+24, BUTTOM_Y_VALUE, User_Comp[i],  0);
			}
		break;
		case 3:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				if(i==0)
					WriteString_16(BUTTOM_X_VALUE+19, BUTTOM_Y_VALUE, User_Comp[i],  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+24, BUTTOM_Y_VALUE, User_Comp[i],  0);
			}
			break;
		case 4:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				if(i==0)
				    WriteString_16(BUTTOM_X_VALUE+18, BUTTOM_Y_VALUE, Sys_Language_Value[i],  0);
				else 
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Sys_Language_Value[i],  0);
			}
			break;
//		case 5:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				if(i==0)
//				    WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+24, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
//				else 
//					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
//			}
//			break;
		case 5:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			//for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+14, BUTTOM_Y_VALUE, "HOLD",  0);
			}
			break;
		case 6:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 7:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 8:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 9:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 10:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				if(i==0)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, Sys_Bais_Value[i],  0);
				else 
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, Sys_Bais_Value[i],  0);
			}
			break;
		case 11:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<5;i++)
			{
				if(i==0||i==2||i==3)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, Sys_Addr_value[i],  0);
				else if(i==1)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, Sys_Addr_value[i],  0);
				else 
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+9, BUTTOM_Y_VALUE, Sys_Addr_value[i],  0);
			}
			break;
		case 12:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 13://
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
					if(i==0)
				    WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+24, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
				else 
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);

			}
			break;
		case 14:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 15:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 16:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 19:
			break;
		case 20:
			break;
		case 21:
			break;
		default:
			break;
		
			
	
	
	}

}
//�û�У�������Ӻ���
void Disp_Usercheck_Screen(void)
{
	
	LCD_DrawRect( 0, 0, 150, 22 , LCD_COLOR_TEST_BAR);
//	Disp_TopBar_Color();
	Disp_Fastbutton();//��ʾ���ٰ���
//	Disp_TestSet_MidRect();//��ʾ�м�ɫ��
}
void Disp_FacrCheck_Item(void)
{
	uint32_t i;
	Disp_Usercheck_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	WriteString_16(0, 4, All_TopName_E[10],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	//for(i=0;i<(sizeof(User_Check_Item)/(sizeof(User_Check_Item[0])));i++)
	//if(i<sizeof(User_Check_Item)/(sizeof(User_Check_Item[0]))/2)
	{
		//WriteString_16(LIST1, FIRSTLINE+SPACE1*i, User_Check_Item[i],  0);
        WriteString_16(LIST1, FIRSTLINE+SPACE1, "STARTING",  0);

	}
//	else
//	{
//		WriteString_16(LIST2, FIRSTLINE+SPACE1*(i-sizeof(User_Check_Item)/(sizeof(User_Check_Item[0]))/2), User_Check_Item[i],  0);
//	}	
	Disp_Button_TestSet(2);
}
void Disp_Fac_Debug_R(Button_Page_Typedef* Button_Page)//����У������
{
	uint32_t i;
	uint32_t Black_Select;
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88+4, FIRSTLINE, Correction_Resist[0],  0);//100uF
	

	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1-2,SELECT_1END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1, Correction_Resist[1],  0);//10uF
	
	

	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*2-2,SELECT_1END , FIRSTLINE+SPACE1*3 -4, Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*2, Correction_Resist[2],  0);//1uF
//	
////У����
	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*3-2,SELECT_1END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*3, Correction_Resist[3],  0);	//100nF
//	
////Ƶ��
////	if(index==5)
////		index=6;
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*4-2,SELECT_1END , FIRSTLINE+SPACE1*5-4 , Colour.black ) ;//SPACE1
	
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*4, Correction_Resist[4],  0);//10nF


	Disp_Fastbutton();
	switch(Button_Page->index)
	{
		case 0:
				Disp_Button_TestSet(2);
			break;
        
        default :
			if(Button_Page->page==0)
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				for(i=0;i<2;i++)
				{
					if(i==0)
				      WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, DEBUG_Check_Opencircuit_E[i],  0);
					else 
					  WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+24, BUTTOM_Y_VALUE, DEBUG_Check_Opencircuit_E[i],  0);				
				}
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+13, BUTTOM_Y_VALUE, Button_TipPage1[i],  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "1/3",  0);
//				
			}
			
		break;

	
	}



}
void Disp_Fac_Debug_C(Button_Page_Typedef* Button_Page)//����У������
{
	uint32_t i;
	uint32_t Black_Select;
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88+4, FIRSTLINE, Correction_Capacitance[0],  1);//100uF
	

	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1-2,SELECT_1END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1, Correction_Capacitance[1],  1);//10uF
	
	

	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*2-2,SELECT_1END , FIRSTLINE+SPACE1*3 -4, Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*2, Correction_Capacitance[2],  1);//1uF
//	
////У����
	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*3-2,SELECT_1END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*3, Correction_Capacitance[3],  1);	//100nF
//	
////Ƶ��
////	if(index==5)
////		index=6;
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*4-2,SELECT_1END , FIRSTLINE+SPACE1*5-4 , Colour.black ) ;//SPACE1
	
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*4, Correction_Capacitance[4],  1);//10nF
//		
////�ο�A
	Black_Select=(Button_Page->index==6)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*5-2,SELECT_1END , FIRSTLINE+SPACE1*6-4 , Colour.black ) ;//SPACE1

	{
		WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*5,Correction_Capacitance[5],  0);//1nF
	}
//	
////��·A
	Black_Select=(Button_Page->index==7)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*6-2,SELECT_1END , FIRSTLINE+SPACE1*7-4 , Colour.black ) ;//SPACE1

		
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*6, Correction_Capacitance[6],  0);//100P
	
//	
////��·A
	Black_Select=(Button_Page->index==8)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}

		
	LCD_DrawRect( LIST1+88+4, FIRSTLINE+SPACE1*7-2,SELECT_1END , FIRSTLINE+SPACE1*8-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88+4, FIRSTLINE+SPACE1*7, Correction_Capacitance[7],  0);//10p

	Disp_Fastbutton();
	switch(Button_Page->index)
	{
		case 0:
				Disp_Button_TestSet(2);
			break;
        
        default :
			if(Button_Page->page==0)
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				for(i=0;i<2;i++)
				{
					if(i==0)
				      WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, DEBUG_Check_Opencircuit_E[i],  0);
					else 
					  WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+24, BUTTOM_Y_VALUE, DEBUG_Check_Opencircuit_E[i],  0);				
				}
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+13, BUTTOM_Y_VALUE, Button_TipPage1[i],  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4-1, BUTTOM_Y_VALUE+1, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4+41, BUTTOM_Y_VALUE+1, "1/3",  0);
//				
			}
			
		break;

	
	}



}
void Disp_UserCheck_Item(void)
{
	uint32_t i;
	Disp_Usercheck_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language ==1)
	{
		WriteString_16(0, 4, All_TopName_E[5],  0);
	}else if(SaveData.Sys_Setup.Language ==0){
		WriteString_16(0, 4, All_TopName[5],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<(sizeof(User_Check_Item_E)/(sizeof(User_Check_Item_E[0])));i++)
	//if(i<sizeof(User_Check_Item)/(sizeof(User_Check_Item[0]))/2)
	{
		WriteString_16(LIST1, FIRSTLINE+SPACE1*i, User_Check_Item_E[i],  0);

	}
//	else
//	{
//		WriteString_16(LIST2, FIRSTLINE+SPACE1*(i-sizeof(User_Check_Item)/(sizeof(User_Check_Item[0]))/2), User_Check_Item[i],  0);
//	}	
	Disp_Button_TestSet(0);
}
//�����б������Ӻ���
void Disp_LimitList_MidRect(void)
{

	LCD_DrawRect( 0, 185,479 , 150+32 , LCD_COLOR_TEST_MID ) ;
//	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
}
void Disp_LimitList_Screen(void)
{
	Disp_TopBar_Color();
	Disp_Fastbutton();//��ʾ���ٰ���
//	Disp_LimitList_MidRect();//��ʾ�м�ɫ��	 User_LimitList_Item
}

void Disp_LimitList_Item(void)			//�����б�����
{
	uint32_t i;
	Disp_LimitList_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language ==1)
	{
		WriteString_16(0, 4, All_TopName_E[6],  0);
	}else if(SaveData.Sys_Setup.Language ==0){
		WriteString_16(0, 4, All_TopName[6],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	if(SaveData.Sys_Setup.Language ==1)
	{
		for(i=0;i<(sizeof(User_LimitList_Item_E)/(sizeof(User_LimitList_Item_E[0])));i++)
		if(i<sizeof(User_LimitList_Item_E)/(sizeof(User_LimitList_Item_E[0]))/2)
		{ 
			if(i==2)
			   WriteString_16(330, FIRSTLINE-2, User_LimitList_Item_E[i],  0);
			else
				WriteString_16(LIST1+i*160, FIRSTLINE-2, User_LimitList_Item_E[i],  0);
		}
		else
		{
			WriteString_16(LIST1+(i-sizeof(User_LimitList_Item_E)/(sizeof(User_LimitList_Item_E[0]))/2)*160,
			 FIRSTLINE+SPACE1-4, User_LimitList_Item_E[i],  0);
		}
	}else if(SaveData.Sys_Setup.Language ==0){
		for(i=0;i<(sizeof(User_LimitList_Item)/(sizeof(User_LimitList_Item[0])));i++)
		if(i<sizeof(User_LimitList_Item)/(sizeof(User_LimitList_Item[0]))/2)
		{ 
			if(i==2)
			   WriteString_16(330, FIRSTLINE-2, User_LimitList_Item[i],  0);
			else
				WriteString_16(LIST1+i*160, FIRSTLINE-2, User_LimitList_Item[i],  0);
		}
		else
		{
			WriteString_16(LIST1+(i-sizeof(User_LimitList_Item)/(sizeof(User_LimitList_Item[0]))/2)*160,
			 FIRSTLINE+SPACE1-4, User_LimitList_Item[i],  0);
		}
	}
	
	for(i=0;i<11;i++)			//
	{
		if(i>3&&i<8)
		 	Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
		WriteString_12(LIST1, 65+i*15-2, User_LimitList_Item2[i],  0);
	}
	WriteString_12(160, 65-2,"LOW",  0);
	
		
	WriteString_12(330, 65-2,"HIGH",  0);	
	
	Disp_Set_Unit_16(SaveData.Main_Func.Param.test,SELECT_2END-148,FIRSTLINE-2 );//��ʾ��λ
	
	
	//Disp_Set_Unit_12(SaveData.Main_Func.Param.test,LIST2-154,65);
	Disp_Button_TestSet(0);
}
//�б�ģʽѡ����Ӻ���
void Disp_ListScan_MidRect(void)
{

	LCD_DrawRect( 0, 180,479 , 150+32 , LCD_COLOR_TEST_MID ) ;
//	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
}
void Disp_ListScan_Screen(void)
{
	Disp_TopBar_Color();
	Disp_Fastbutton();//��ʾ���ٰ���	  
//	Disp_ListScan_MidRect();//��ʾ�м�ɫ��	 User_LimitList_Item
}
void Disp_ListScan_Item(void)
{
	uint32_t i;
	Disp_ListScan_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language ==1)
	{
		WriteString_16(0, 4, All_TopName_E[7],  0);
	}else if(SaveData.Sys_Setup.Language ==0){
		WriteString_16(0, 4, All_TopName[7],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	
	if(SaveData.Sys_Setup.Language ==1)
	{
		WriteString_16(LIST1, FIRSTLINE, User_ListScan_Item_E[0],  0);
		for(i=1;i<(sizeof(User_ListScan_Item_E)/(sizeof(User_ListScan_Item_E[0])));i++)
		WriteString_16(LIST1+30+(i-1)*77, FIRSTLINE+SPACE1, User_ListScan_Item_E[i],  0);
	}else if(SaveData.Sys_Setup.Language ==0){
		WriteString_16(LIST1, FIRSTLINE, User_ListScan_Item[0],  0);
		for(i=1;i<(sizeof(User_ListScan_Item)/(sizeof(User_ListScan_Item[0])));i++)
		WriteString_16(LIST1+30+(i-1)*77, FIRSTLINE+SPACE1, User_ListScan_Item[i],  0);
	}
	
//	for(i=0;i<11;i++)
//	WriteString_16(LIST1, FIRSTLINE+SPACE1+i*16, User_LimitScan_Item2[i],0);
	Disp_Button_TestSet(0);
}



//ϵͳ�����Ӻ���
void Disp_SysLine(void)
{
 	uint32_t i;
	if(SaveData.Sys_Setup.Language ==1)
	{
		for(i=0;i<(sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0])));i++)
		{
			if(i<((sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0]))+1)/2))
				LCD_DrawLine( 95, FIRSTLINE+(i+1)*SPACE1-2, 90+100,FIRSTLINE+(i+1)*SPACE1-2, White );
			else
				LCD_DrawLine( 95+250, FIRSTLINE+(i-(sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0]))-1)/2)*SPACE1-2, 90+100+250,FIRSTLINE+(i-(sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0]))-1)/2)*SPACE1-2, White );
		}
	}else if(SaveData.Sys_Setup.Language ==0){
		for(i=0;i<(sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0])));i++)
		{
			if(i<((sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))+1)/2))
				LCD_DrawLine( 95, FIRSTLINE+(i+1)*SPACE1-2, 90+100,FIRSTLINE+(i+1)*SPACE1-2, White );
			else
				LCD_DrawLine( 95+250, FIRSTLINE+(i-(sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))-1)/2)*SPACE1-2, 90+100+250,FIRSTLINE+(i-(sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))-1)/2)*SPACE1-2, White );
		}
	}
	

}
//void Disp_Sys_MidRect(void)
//{
//
//	LCD_DrawRect( 0, 90+32,479 , 150+32 , LCD_COLOR_TEST_MID ) ;
////	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
//}
void Disp_Sys_Screen(void)
{
//	Disp_TopBar_Color();
	LCD_DrawRect(0, 0, 160,22 , LCD_COLOR_TEST_BAR);
	Disp_Fastbutton();//��ʾ���ٰ���	  
//	Disp_ListScan_MidRect();//��ʾ�м�ɫ��	 User_LimitList_Item  
}


// ��ʾϵͳ����
static void  Disp_SysDot(void)
{
	uint8_t i;
	if(SaveData.Sys_Setup.Language ==1)
	{
		for(i=0;i<(sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0])));i++)
		{
			if(i<((sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0]))+1)/2))
				WriteString_16(LIST1+84+4, FIRSTLINE+SPACE1*i, ":",  0);
			else
				WriteString_16(LIST2+84+4, FIRSTLINE+SPACE1*(i-(sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0])))/2), ":",  0);	
		}
	}else if(SaveData.Sys_Setup.Language ==0){
		for(i=0;i<(sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0])));i++)
		{
			if(i<((sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))+1)/2))
				WriteString_16(LIST1+84+4, FIRSTLINE+SPACE1*i, ":",  0);
			else
				WriteString_16(LIST2+84+4, FIRSTLINE+SPACE1*(i-(sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0])))/2), ":",  0);	
		}
	}
	
}


//ϵͳ����
void Disp_Sys_Item(void)
{
	uint32_t i;
	Disp_Sys_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language ==1)
	{
		WriteString_16(0, 4, All_TopName_E[8],  0);
	}else if(SaveData.Sys_Setup.Language ==0){
		WriteString_16(0, 4, All_TopName[8],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
//	WriteString_16(LIST1, FIRSTLINE, User_ListScan_Item[0],  0);
	if(SaveData.Sys_Setup.Language ==1)
	{
		for(i=0;i<(sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0])));i++)
		if(i<sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0]))/2)
		{
			WriteString_16(LIST1, FIRSTLINE+SPACE1*i, Sys_Setitem_E[i],  0);

		}
		else
		{
			WriteString_16(LIST2,FIRSTLINE+SPACE1*(i-(sizeof(Sys_Setitem_E)/(sizeof(Sys_Setitem_E[0]))+1)/2), Sys_Setitem_E[i],  0);
		}
	}else if(SaveData.Sys_Setup.Language ==0){
		for(i=0;i<(sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0])));i++)
		if(i<sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))/2)
		{
			WriteString_16(LIST1, FIRSTLINE+SPACE1*i, Sys_Setitem[i],  0);

		}
		else
		{
			WriteString_16(LIST2,FIRSTLINE+SPACE1*(i-(sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))+1)/2), Sys_Setitem[i],  0);
		}
	}
	
	
	Disp_SysDot();
	Disp_SysLine();
	Disp_Button_SysSet();
}

void Disp_serve_Screen(void)
{

	LCD_DrawRect(0, 0, 112,22 , LCD_COLOR_TEST_BAR);
	Disp_Fastbutton();//��ʾ���ٰ���	   
}

//У���������ý������
void Disp_Serve_correction (void)
{
	uint8_t i;

	lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_serve_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language ==1)
	{
		WriteString_16(0, 4, All_TopName_E[10],  0);
	}else if(SaveData.Sys_Setup.Language ==0){
		WriteString_16(0, 4, All_TopName[10],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<8;i++)
	{
	   if(i<4)
		   WriteString_16(LIST1+100, FIRSTLINE+SPACE1*(i+2), Correction_Capacitance[i],  0);
	   else 
		   WriteString_16(LIST2+60, FIRSTLINE+SPACE1*(i-2), Correction_Capacitance[i],  0);	 
	  
	}
//	 WriteString_16(LIST1+85, FIRSTLINE+SPACE1,"���ݳ�ʼ��",  0);
//SPACE1
		
	Disp_Button_Correction();
	
}


void Disp_Start_Corr(void)
{
	lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_serve_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language ==1)
	{
		WriteString_16(0, 4, All_TopName_E[10],  0);
	}else if(SaveData.Sys_Setup.Language ==0){
		WriteString_16(0, 4, All_TopName[10],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	
	WriteString_16(LIST2-75, FIRSTLINE+SPACE1*4+6,"StarCorr",  0);
	Disp_Button_Correction();

}



//У���������ý������
void Disp_Serve_correctionR (void)
{
	uint8_t i;

	lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_serve_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(SaveData.Sys_Setup.Language ==1)
	{
		WriteString_16(0, 4, All_TopName_E[10],  0);
	}else if(SaveData.Sys_Setup.Language ==0){
		WriteString_16(0, 4, All_TopName[10],  0);
	}
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<5;i++)
	{
		   WriteString_16(LIST2-60, FIRSTLINE+SPACE1*(i+2), Correction_Resist[i],  0);	
	}
//	WriteString_16(LIST2-75, FIRSTLINE+SPACE1,"�����ʼ��",  0);
//SPACE1
		
	Disp_Button_Correction();
	
}

//У������
void Disp_Correction_Set(Button_Page_Typedef* Button_Page)
{
	uint8_t i;
	
	for(i=1;i<9;i++)
	{	
		
	//else
		 if(i<5)
		{
				if(i==Button_Page->index)
				{
					Colour.black=LCD_COLOR_SELECT;
					LCD_DrawRect( LIST1+80, FIRSTLINE+SPACE1*(i+1),LIST1+190 , FIRSTLINE+SPACE1*(i+2)-4 , Colour.black ) ;
					WriteString_16(LIST1+100, FIRSTLINE+SPACE1*(i+1), Correction_Capacitance[i-1],  0);
				}
				else
				{
					Colour.black=LCD_COLOR_TEST_BACK;
					LCD_DrawRect( LIST1+80, FIRSTLINE+SPACE1*(i+1),LIST1+190 , FIRSTLINE+SPACE1*(i+2)-4 , Colour.black ) ;
					WriteString_16(LIST1+100, FIRSTLINE+SPACE1*(i+1), Correction_Capacitance[i-1],  0);
				}
		}
		
		else
		{
				if(i==Button_Page->index)
				{
					Colour.black=LCD_COLOR_SELECT;
					LCD_DrawRect( LIST2+50, FIRSTLINE+SPACE1*(i-3),LIST2+150 , FIRSTLINE+SPACE1*(i-2)-4 , Colour.black ) ;
					WriteString_16(LIST2+60, FIRSTLINE+SPACE1*(i-3), Correction_Capacitance[i-1],  0);				
				}
				else
				{
					Colour.black=LCD_COLOR_TEST_BACK;
					LCD_DrawRect( LIST2+50, FIRSTLINE+SPACE1*(i-3),LIST2+150 , FIRSTLINE+SPACE1*(i-2)-4 , Colour.black ) ;
					WriteString_16(LIST2+60, FIRSTLINE+SPACE1*(i-3), Correction_Capacitance[i-1],  0);	
				}
		
		}
	
	}
	
	
		Disp_Fastbutton();	
	switch(Button_Page->index)
	{
		case 0:
//				Disp_Button_Correction();
//			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				if(SaveData.Sys_Setup.Language ==1)
				{
					WriteString_16(83+19, 271-29, "CONF",  0);	
					WriteString_16(83+83+19, 271-29, "SAVE",  0);
				}else if(SaveData.Sys_Setup.Language ==0){
					WriteString_16(83+19, 271-29, "ȷ��",  0);	
					WriteString_16(83+83+19, 271-29, "����",  0);
				}
				
			break ;
	}
	
	if(Correc_successflag==1)
	{		
		if(SaveData.Sys_Setup.Language ==1)
		{
			if(Button_Page->index<5)
			  WriteString_16(LIST1+210, FIRSTLINE+SPACE1*(Button_Page->index+1), "DONE",  0);
			else
			  WriteString_16(LIST2+170, FIRSTLINE+SPACE1*(Button_Page->index-3), "DONE",  0);
		}else if(SaveData.Sys_Setup.Language ==0){
			if(Button_Page->index<5)
			  WriteString_16(LIST1+210, FIRSTLINE+SPACE1*(Button_Page->index+1), "���",  0);
			else
			  WriteString_16(LIST2+170, FIRSTLINE+SPACE1*(Button_Page->index-3), "���",  0);
		}
			
		
    }		
	
	
	
	
}



//У������
void Disp_Correction_SetR(Button_Page_Typedef* Button_Page)
{
	uint8_t i;
	
	for(i=1;i<6;i++)
	{	
				if(i==Button_Page->index)
				{
					Colour.black=LCD_COLOR_SELECT;
					LCD_DrawRect( LIST2-80, FIRSTLINE+SPACE1*(i+1),LIST2+30 , FIRSTLINE+SPACE1*(i+2)-4 , Colour.black ) ;
					WriteString_16(LIST2-60, FIRSTLINE+SPACE1*(i+1), Correction_Resist[i-1],  0);
				}
				else
				{
					Colour.black=LCD_COLOR_TEST_BACK;
					LCD_DrawRect( LIST2-80, FIRSTLINE+SPACE1*(i+1),LIST2+30 , FIRSTLINE+SPACE1*(i+2)-4 , Colour.black ) ;
					WriteString_16(LIST2-60, FIRSTLINE+SPACE1*(i+1), Correction_Resist[i-1],  0);
				}					
	
	}
	
	
		Disp_Fastbutton();	
	switch(Button_Page->index)
	{
		case 0:
//				Disp_Button_Correction();
//			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
//				WriteString_16(83+19, 271-29, "ȷ��",  0);
//				WriteString_16(83+83+19, 271-29, "����",  0);
		if(SaveData.Sys_Setup.Language ==1)
		{
			WriteString_16(83+19, 271-29, "CONF",  0);
			WriteString_16(83+83+19, 271-29, "SAVE",  0);
		}else if(SaveData.Sys_Setup.Language ==0){
			WriteString_16(83+19, 271-29, "ȷ��",  0);
			WriteString_16(83+83+19, 271-29, "����",  0);
		}
		
			break ;
	}

	if(Correc_successflag==1)
	{
		 if(SaveData.Sys_Setup.Language ==1)
		{
			WriteString_16(LIST2+50, FIRSTLINE+SPACE1*(Button_Page->index+1), "DONE",  0);
		}else if(SaveData.Sys_Setup.Language ==0){
			WriteString_16(LIST2+50, FIRSTLINE+SPACE1*(Button_Page->index+1), "���",  0);
		}
				
    }		
	
}





Sort_TypeDef Time_Set_Cov(Sort_TypeDef *Time)
{
	if(Time->Dot==0)//û��С����
	{
		if(Time->Unit==0)//��λ�Ǻ���
		{
			if(Time->Num>60000)//����60.000  ��ʾ60.000
			{
				Time->Num=60000;
				Time->Dot=3;
			}
			else
			if(Time->Num>999)//����999  ��ʾX.XXX
			{
				//Time->Num=60;
				Time->Dot=3;
				Time->Unit=1;
				
			} 
		
		}
		else//��λ����
		{
			if(Time->Num>60)//����60�� ��ʾ60��
				Time->Num=60;
			Time->Num*=1000;//����1000��  С����Ϊ����λ
			Time->Dot=3;	
			Time->Unit=1;			
		}
	
	
	}
	else if(Time->Dot==1)//�Ӹ�λ����һ��С����Ϊ1
	{
		if(Time->Unit==0)//��λΪ����ʱ
		{
			
				
			if(Time->Num>0)//x.xxxxʱ ��ʾ���λ
			{
				Time->Num/=10000;
			
			}
//			else if(Time->Num/1000>0)//x.xxxʱ��ʾ���λ
//			{
//				Time->Num/=1000;
//			
//			
//			}else if(Time->Num/100>0)//x.xxʱ ��ʾ���λ
//			{
//				Time->Num/=100;
//			
//			
//			}else if(Time->Num/10>0)//x.xʱ ��ʾ���λ
//			{
//				Time->Num/=10;
//			
//			
//			}
			Time->Dot=0;//��Ϊ�Ǻ���  ����û��С����
			Time->Unit=1;
	
		
		}
		else//��λ����
		{
			if(Time->Num>=10000)//x.xxxxʱ ��ʾx.xxx
			{
				//Time->Num/=10;
				Time->Unit=1;
				Time->Dot=3;//��Ϊ�Ǻ���  ����û��С����
				
			
			}else if(Time->Num>=1000)//x.xxxʱ��ʾx.xxx
			{
				
				Time->Unit=0;
				Time->Dot=0;//��Ϊ�Ǻ���  ����û��С����
			
			
			}
			Time->Num/=10;
//			else if(Time->Num>=100)//x.xxʱ ��ʾx.xx0
//			{
//				Time->Num*=10;
//			
//			
//			}else if(Time->Num>=10)//x.xʱ ��ʾx.x00
//			{
//				Time->Num*=100;
//			
//			
//			}
//			else
//				Time->Num*=1000;//x.000
				
			
			
	
		
		
		
		}
	
	
	
	}else if(Time->Dot==2)//xx.
	{
		if(Time->Unit==0)//��λΪ����ʱ
		{
			
			if(Time->Num>=1000)//xx.xxxʱ ��ʾ���λ
			{
				Time->Num/=1000;
				//Time->Dot=0;
				
			
			}
//			else if(Time->Num/100>=10)//xx.xxʱ��ʾ���λ
//			{
//				Time->Num/=1000;
//			
//			
//			}
			else// if(Time->Num<1000)//xx.xʱ ��ʾ���λ
			{
				Time->Num=0;
			
			
			}
			Time->Dot=0;//��Ϊ�Ǻ���  ����û��С����
	
		
		}
		else//��λ����
		{
			if(Time->Num>60000)//
			{
				Time->Num=60000;
			
			}
//			else if(Time->Num>=10000)//xx.xxxʱ��ʾxx.xxx
//			{
//				//Time->Num*=10;
//			
//			
//			}
//			else if(Time->Num>=1000)//xx.xxʱ ��ʾxx.xx0
//			{
//				Time->Num*=10;
//			
//			
//			}
			else if(Time->Num<1000)//xx.xʱ ��ʾxx.x00
			{
				//Time->Num*=100;
				Time->Dot=0;//��Ϊ����  ��С����3
				Time->Unit=0;
			
			
			}
//			else if()
//				Time->Num*=1000;//xx.000
			else
			{
				
				Time->Dot=3;//��Ϊ����  ��С����3
				Time->Unit=1;
			}
	
		
		
		
		}
			
	}else if(Time->Dot==3)//xxx.
	{
		if(Time->Unit==0)//��λΪ����ʱ
		{
			
			if(Time->Num>=100)//xxx.xxʱ ��ʾ���λxxx
			{
				Time->Num/=100;
			
			}
			else
			{
				Time->Num=0;
			
			}
//			else if(Time->Num/10>=100)//xxx.xʱ��ʾ���λxxx
//			{
//				Time->Num/=100;
//			
//			
//			}
			Time->Dot=0;//��Ϊ�Ǻ���  ����û��С����
	
		
		}
		else//��λ����
		{
			if(Time->Num>6000)
			{
				Time->Num=6000;//��һλ��ʾ
				Time->Dot=3;
				Time->Unit=1;
				
				
			}
			else if(Time->Num>=100)
			{
				//Time->Num*=10;
				Time->Dot=3;
				Time->Unit=1;
			
			
			}
			else //if(Time->Num>=0)
			{
				//Time->Num*=10;
				Time->Dot=0;
				Time->Unit=0;
			}
			Time->Num*=10;
			
		
		}
	}else if(Time->Dot==4)//xxxx.x
	{
		if(Time->Unit==0)//��λΪ����ʱ
		{
			
			if(Time->Num>=10000)//xxxx.xʱ ��ʾ���λxxx
			{
				
				{
					//Time->Num/=10;	
					Time->Dot=3;//��Ϊ�Ǻ���  ����û��С����
					Time->Unit=1;
				}
			
			} //xxxxʱ��ʾ���λx.xxx
			else if(Time->Num>0)
			{
				//Time->Num/=10;	
				Time->Dot=0;//��Ϊ�Ǻ���  ����û��С����
				Time->Unit=0;
			
			
			}
			Time->Num/=10;
			
			
	
		
		}
		else//��λ����
		{
			if(Time->Num>600)
			{
				Time->Num=600;
				Time->Dot=3;
				Time->Unit=1;
			}
			else
				if(Time->Num>=10)
				{
					Time->Dot=3;
					Time->Unit=1;
				
				
				
				}
				else
				{
					Time->Dot=0;
					Time->Unit=0;
				
				}
			
		Time->Num*=100;
		}
	}
	else if(Time->Dot==5)//xxxxx.
	{
		if(Time->Unit==0)//��λΪ����ʱ
		{
			
			if(Time->Num>60000)//xxxx.xʱ ��ʾ���λxxx
			{
				
				{
					Time->Num=60000;	
					Time->Dot=3;//��Ϊ�Ǻ���  ����û��С����
					Time->Unit=1;
				}
			
			} //xxxxʱ��ʾ���λx.xxx
			else if(Time->Num>=1000)
			{
				
				Time->Dot=3;//��Ϊ�Ǻ���  ����û��С����
				Time->Unit=1;
			
			
			
			}
			else
			{
				Time->Dot=0;//��Ϊ�Ǻ���  ����û��С����
				Time->Unit=0;
			
			}
			
			
	
		
		}
		else//��λ����
		{
			if(Time->Num>60)
			{
				Time->Num=60;
				
			}
			Time->Num*=1000;
			
			Time->Dot=3;
				Time->Unit=1;
			
		
		}
	}
	else
	{
	
		Time->Num=60000;
		Time->Dot=3;
		Time->Unit=1;
	
	}

	return *Time;
}
Sort_TypeDef Freq_Set_Cov(Sort_TypeDef *Freq)
{
//	uint8_t i;
	if(Freq->Unit==0)//��λ��Hz
	{
		switch(Freq->Dot)
		{
			case 0:
				if(Freq->Num>1e5)
				{
					Freq->Num=1e5;
					Freq->Updata_flag=0;
				
				}
				else
					Freq->Updata_flag=1;
					
				break;
			case 1://1��С���� 
				break;
			case 2://����С����
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=10&&Freq->Num<100)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				Freq->Num/=1000;
				
				break;
			case 3://����С����
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=100&&Freq->Num<1000)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				Freq->Num/=100;
				break;
			case 4://4��С����
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=1e3&&Freq->Num<1e4)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				Freq->Num/=10;
				break;
			default:
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=1e4&&Freq->Num<1e5)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				break;
		
		
		
		}
		
		
		
		
	
	}
	else//��λ��KHZ
	{
		switch(Freq->Dot)
		{
			case 0:
				if(Freq->Num>100000)
				Freq->Num=100000;
				else
					Freq->Num*=1000;
					
				break;
			case 1:
				if(Freq->Num>0)
					Freq->Num/=10;
//				if(Freq->Num>=10000)
				
//				for(i=0;i<5;i++)
//				{
//					if((Freq->Num>=10&&Freq->Num<100)||Freq->Num<10)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				//Freq->Num*=100;
				break;
			case 2:
				
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=10&&Freq->Num<100)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
//				Freq->Num*=1000;
				break;
			case 3:
				if(Freq->Num>10000)
					Freq->Num=10000;
				Freq->Num*=10;
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=10&&Freq->Num<100)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
//				Freq->Num*=1e4;
				break;
			case 4:
				if(Freq->Num>1000)
					Freq->Num=1000;
				Freq->Num*=100;
				
				break;
			case 5:
			if(Freq->Num>100)
				Freq->Num=100;
			Freq->Num*=1000;
			
			break;
			default:
				Freq->Num=1e6;
				break;
		
		}
	
	
	}
	
	
	return *Freq;
}
Sort_TypeDef Input_Set_Cov(Sort_TypeDef *Input_Ref)//
{
//	uint8_t i;
//	uint32_t buff[]={10000,1000,100,10,1};
	uint32_t value;
	value=pow(10.0,(float)Input_Ref->Dot);
	value=Input_Ref->Num/value;
	if(value>=1000)
	{
		Input_Ref->Unit++;
		Input_Ref->Dot+=3;
	
	
	}else if(value==0)
	{
		if(Input_Ref->Unit>0)
		{
			Input_Ref->Unit--;
			if(Input_Ref->Dot>=3)
				Input_Ref->Dot-=3;
		
		
		}
	
	
	}
	
	return *Input_Ref;
}
Sort_TypeDef Input_Set_CovPre(Sort_TypeDef *Input_Ref)//
{
//	uint8_t i;
//	uint32_t buff[]={10000,1000,100,10,1};//10.000
	uint32_t value;
	Input_Ref->Unit=0;
	//if(Input_Ref->Dot)
	value=pow(10.0,(float)Input_Ref->Dot);
	value=Input_Ref->Num/value;
	if(value>100)
	{
		
		Input_Ref->Dot=2;
	
	
	}
	
	return *Input_Ref;
}
void Disp_Cp_D(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
//	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+8*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);

}
void Disp_Cp_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Cp_G(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+6*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_Cp_Rp(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_Cs_Rs(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_Cs_D(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+8*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
//	WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+8);

}
void Disp_Cs_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_Rp(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_Rd(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_D(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+8*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_G(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+6*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Ls_D(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+8*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Ls_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Ls_Rs(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);

}
void Disp_Ls_Rd(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);

}
void Disp_Z_d(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+3*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+10*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10);

}
void Disp_Z_r(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+3*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+11*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10);

}
void Disp_Y_d(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+4*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+10*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10);

}
void Disp_Y_r(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+4*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+11*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10);

}
void Disp_R_X(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+5*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+21*40*55/8);	//X
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);	//�ո�
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10);

}

void Disp_Rs_Q(void)
{
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);//_p
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_Rp_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+5*40*55/8);//R
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_G_B(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+6*40*55/8);//G
	//LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+7*40*55/8);//B
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);	//�ո�
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);	//�ո�
	
	LCD_ShowFontCN_40_55(60+40+40,92,40,55,(uint8_t*)Out_Assic+26*40*55/8);		//ð��
	LCD_ShowFontCN_40_55(60+40+40,92+55,40,55, (uint8_t*)Out_Assic+26*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);
}

//const uint8_t func_Item[][]=
//{};
void Disp_Button_Fun_Set(uint16_t xpos,uint16_t ypos,uint8_t * Disp_Item,Button_Page_Typedef* Button_Page)
{
	uint8_t while_flag=1,i;
	uint8_t key,Disp_Flag=1;
	uint32_t keynum;
	Button_Page->third=0;
	do{
		key=HW_KeyScsn();
	
	}
		while(key!=0xff);
	Button_Page->third=SaveData.Main_Func.Param.test;
	while(while_flag)
	{
		if(Disp_Flag)
		{
			Disp_Flag=0;
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			Disp_Fastbutton();
			Colour.black=LCD_COLOR_SELECT;
			LCD_DrawRect( LIST1+88, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black );//SPACE1
			WriteString_16(LIST1+88, FIRSTLINE, User_FUNC[Button_Page->third],  1);//�����㷨  ��˳��Ĺ���
//			LCD_DrawRect( LIST1+88, ypos-2,SELECT_1END , ypos+SPACE1-4 , Colour.black );//SPACE1
//			WriteString_16(LIST1+88, ypos, User_FUNC[Button_Page->third],  1);//�����㷨  ��˳��Ĺ���
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<5;i++)
			{  
			  if(Button_Page->force==0)
			  {
				if(i==4)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else if(i==3)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+9, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
			  }
			  
			  	 if(Button_Page->force==1)
			  {
				if(i==4)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else if(i==2)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+9, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
			  }
			
			  if(Button_Page->force==4||Button_Page->force==5) 
			  {
				  if(i==4)
				     WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				   else
					 WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
						
			  }

			  	 if(Button_Page->force==2)
			  {
				if(i==4)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else if(i==1||i==2)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+9, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
			  }
			  
			  if(Button_Page->force==3)
			  {
				if(i==4)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else if(i<2)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+9, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
			  }
			  
				  if(Button_Page->force==6)
			  {
				if(i==4)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+4, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else if(i==0)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+19, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
				else
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE+14, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
			  }

			}
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
			Disp_Flag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
					if(Fun_SelectValue[0][Button_Page->force]!=0xff)
					{
					Button_Page->third=Fun_SelectValue[0][Button_Page->force];
						while_flag=0;
						
					}
					//SaveData.Main_Func.Param.test
					SaveData.Main_Func.Param.test=Button_Page->third;
					Button_Page->third=0xff;
					break;
				case Key_F2:
					
					if(Fun_SelectValue[1][Button_Page->force]!=0xff)
					{
						//if(Fun_SelectValue[3][Button_Page->force]!=0xff)
						Button_Page->third=Fun_SelectValue[1][Button_Page->force];
						while_flag=0;
					}
					SaveData.Main_Func.Param.test=Button_Page->third;
					if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=0xff;
					break;
				case Key_F3:
					
					if(Fun_SelectValue[2][Button_Page->force]!=0xff)
					{
						//if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=Fun_SelectValue[2][Button_Page->force];
						while_flag=0;
					}
					SaveData.Main_Func.Param.test=Button_Page->third;
					if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=0xff;
					break;
				case Key_F4:
					
					if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					{
						//if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=Fun_SelectValue[3][Button_Page->force];
						while_flag=0;
					}
					SaveData.Main_Func.Param.test=Button_Page->third;
					if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=0xff;
					break;
				case Key_F5:
					
					while_flag=0;
					break;
				case Key_Disp:
					
                    SetSystemStatus(SYS_STATUS_TEST);
				while_flag=0;
				break;
				case Key_SETUP:
					
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				while_flag=0;
				break;
				case Key_LEFT:
					
					while_flag=0;
//					if(Button_Page->index==0)
//						Button_Page->index=6;
//					else
//					if(Button_Page->index>3)
//						Button_Page->index-=3;
//					else
//						Button_Page->index+=2;
//					Button_Page->page=0;
				Button_Page->index=0;
				break;
				case Key_RIGHT:
					
					while_flag=0;
//					if(Button_Page->index==0)
//						Button_Page->index=1;
//					else
//					if(Button_Page->index<=3)
//						Button_Page->index+=3;
//					else
//						Button_Page->index-=2;
//					Button_Page->page=0;
				Button_Page->index=10;
						
				break;
				case Key_DOWN:
					
					while_flag=0;
//					if(Button_Page->index>5)
//						Button_Page->index=0;
//					else
//						Button_Page->index++;
//					Button_Page->page=0;
				Button_Page->index=2;
					
				break;
				case Key_UP:
					
					while_flag=0;
//					if(Button_Page->index<1)
//						Button_Page->index=6;
//					else
//						Button_Page->index--;
//					Button_Page->page=0;
				Button_Page->index=0;
				break;
				default:
					
					break;
				
			
			
			
			}
			
		
		
		}
	
	
	}


}
void Disp_Testvalue(void)
{
//	Test_Dispvalue.Mainvalue.Dot=3;
//	Hex_Format(timer0_counter, Test_Dispvalue.Mainvalue.Dot , 5 , 0);//��ʾ������
	WriteString_Big(180,92 ,Test_Dispvalue.Main_valuebuff);
	
//	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);
//	WriteString_Big((uint8_t)Set_Unit[Test_Dispvalue.Mainvalue.Unit]);
	//��ʾ��λ
	
//	Hex_Format(Test_Dispvalue.Secondvalue.Num , Test_Dispvalue.Secondvalue.Dot , 5 , 0);//��ʾ��������
	WriteString_Big(180,92+55 ,Test_Dispvalue.Secondvaluebuff);
	
	
	Colour.black=LCD_COLOR_TEST_BACK;
	if(SaveData.Main_Func.V_i==0)
	{
		//Hex_Format(Test_Dispvalue.Vmvalue.Num , Test_Dispvalue.Vmvalue.Dot , 5 , 0);
        if(SaveData.Main_Func.Level==0)
            WriteString_16(30, 92+55+55+8, "0.1001",  0);
        else
            if(SaveData.Main_Func.Level==1)
                 WriteString_16(30, 92+55+55+8, "0.3001",  0);
            else
                WriteString_16(30, 92+55+55+8, "1.0001",  0);
//            WriteOneHzChar_16(Test_Dispvalue.Vmvaluebuff[5]-'0', 30, 92+55+55+8+50, 0);
                
//		WriteString_16(30, 92+55+55+8, Test_Dispvalue.Vmvaluebuff,  0);
		//WriteString_16(30, 92+55+55+8, "1.234",  0);
		Disp_Test_Unit_v(30+60,92+55+55+8);
		
//		Hex_Format(Test_Dispvalue.Imvalue.Num , Test_Dispvalue.Imvalue.Dot , 5 , 0);
		WriteString_16(150, 92+55+55+8, Test_Dispvalue.Imvaluebuff,  0);
		Disp_Test_Unit_mA(150+60,92+55+55+8);
	}
	else
	{
		WriteString_16(30, 92+55+55+8, "OFF",  0);
		WriteString_16(150, 92+55+55+8, "OFF",  0);
	
	
	}
	
	
	//��ʾ����


}
void Disp_Big_MainUnit(uint8_t unit,uint8_t unit1)		//��ʾ��������λ
{
	const uint8_t nuitnum[]={12,15,16,17,22,14,13};
	const uint8_t nuit_nuit[]={18,19,20,2,11,23,22};//F,H,����S r ��ո�
	LCD_ShowFontCN_40_55(DISP_UNIT_XPOS,DISP_UNIT_YPOS,40,55,(uint8_t*)Out_Assic+nuitnum[unit]*(55*40/8));
	LCD_ShowFontCN_40_55(DISP_UNIT_XPOS+40,DISP_UNIT_YPOS,40,55,(uint8_t*)Out_Assic+nuit_nuit[unit1]*(55*40/8));
}

void Disp_Big_SecondUnit(uint8_t unit,uint8_t unit1)	//��ʾ��������λ
{
	const uint8_t nuitnum[]={12,15,16,17,22,14,13};
	const uint8_t nuit_nuit[]={18,19,20,2,11,23,22};//F,H,����S r ��ո�
	LCD_ShowFontCN_40_55(DISP_UNIT_XPOS,DISP_UNIT_YPOS+55,40,55,(uint8_t*)Out_Assic+nuitnum[unit]*(55*40/8));
	LCD_ShowFontCN_40_55(DISP_UNIT_XPOS+40,DISP_UNIT_YPOS+55,40,55,(uint8_t*)Out_Assic+nuit_nuit[unit1]*(55*40/8));
}




//void DelayMs(uint32_t nMs)
//{
//	uint32_t i;
//	for(; nMs >0; nMs--)
//	{
//		for(i=10301;i>0;i--);
//	}
//}



void  Start_Correction(void)			//��ʼУ��
{
	uint8_t Send_corr_start[8]={0xAB,0x01,0x08,0x06,0xC0,0x01,0x00,0xBF};
	uint8_t i=5;
	
	INSTRUCT_TO_DEBUG;
	UARTPuts( LPC_UART0, Send_corr_start);
	INSTRUCT_TO_DEBUG;
	while(i)	
	{
			i--;
		if(ComBuf.rec.buf[SITE]==INSTR_FIVE)
			i=0;		
//		else 
//			UARTPuts( LPC_UART0, Send_corr_start);
		INSTRUCT_TO_DEBUG;			
	}	

	
}

void Init_C(void)//���ݳ�ʼ��У��
{
  uint8_t Send_correction_C[8]={0xAB,0x01,0x08,0x06,0xE0,0x00,0x00,0xBF};
  uint8_t i=5;

		 UARTPuts( LPC_UART0, Send_correction_C);
		 INSTRUCT_TO_DEBUG;
			while(i)	
		{
				i--;
			if(ComBuf.rec.buf[SITE]==INSTR_FIVE)
				i=0;		
//			else 
//				UARTPuts( LPC_UART0, Send_correction_C);
			INSTRUCT_TO_DEBUG;
		}	
}


void Correction_C(Button_Page_Typedef* Button_Page)		//����У��
{
	uint8_t Send_correction_C[8]={0xAB,0x01,0x08,0x06,0xE0,0x00,0x00,0xBF};
	uint8_t i=105;	

		Send_correction_C[5]=Button_Page->index;//����У��ֵ
		UARTPuts( LPC_UART0, Send_correction_C);
		LONGDELAY;
		while(i)	
		{
				i--;
			if(ComBuf.rec.buf[SITE]==INSTR_FIVE)
			{  
				i=0;		
				Correc_successflag=1;
			}

			LONGDELAY;
		}
		Correc_successflag=1;
}
	
void Save_C(void)	//�������ֵ
{
	uint8_t Send_correction_C[8]={0xAB,0x01,0x08,0x06,0xE0,0x0F,0x00,0xBF};		//����У������
	uint8_t i=5;

		 UARTPuts( LPC_UART0, Send_correction_C);
		 INSTRUCT_TO_DEBUG;
			while(i)	
		{
				i--;
			if(ComBuf.rec.buf[CORRE_SITE]==INSTR_FOUR)
				i=0;		
//			else 
//				UARTPuts( LPC_UART0, Send_correction_C);
			INSTRUCT_TO_DEBUG;
		}		

}

//��ʼ������
void Init_R(void)
{
	uint8_t Send_correction_R[8]={0xAB,0x01,0x08,0x06,0xE1,0x00,0x00,0xBF};
	uint8_t i=5;

	UARTPuts( LPC_UART0, Send_correction_R);
	INSTRUCT_TO_DEBUG;
			while(i)	
		{
				i--;
			if(ComBuf.rec.buf[SITE]==INSTR_FIVE)
				i=0;		
//			else 
//				UARTPuts( LPC_UART0, Send_correction_R);
			INSTRUCT_TO_DEBUG;
		}

}

void Correction_R(Button_Page_Typedef* Button_Page)		//����У��
{
	uint8_t Send_correction_R[8]={0xAB,0x01,0x08,0x06,0xE1,0x00,0x00,0xBF};
	uint8_t i=106;
	    Send_correction_R[5]=Button_Page->index;//����У��ֵ
	    UARTPuts( LPC_UART0, Send_correction_R);						
		LONGDELAY;
		while(i)	
		{
				i--;
			if(ComBuf.rec.buf[SITE]==INSTR_FIVE)
			{ 
				i=0;	
				Correc_successflag=1;	
			}
		 LONGDELAY;
		}
	Correc_successflag=1;
	
}


void Save_R(void)	//�������ֵ
{
   uint8_t Send_correction_R[8]={0xAB,0x01,0x08,0x06,0xE1,0x0F,0x00,0xBF};
   uint8_t i=5;

		 UARTPuts( LPC_UART0, Send_correction_R);
		 INSTRUCT_TO_DEBUG;
			while(i)	
		{
				i--;
			if(ComBuf.rec.buf[CORRE_SITE]==INSTR_FOUR)
				i=0;		
//			else 
//				UARTPuts( LPC_UART0, Send_correction_R);
			INSTRUCT_TO_DEBUG;
		}		

}

void Exit_correction(void)		//�˳�У��
{

	uint8_t Exit_corre[8]={0xAB,0x01,0x08,0x06,0xE2,0x00,0x00,0xBF};
	uint8_t i=5;		
		UARTPuts( LPC_UART0, Exit_corre);		//�˳�У��
		INSTRUCT_TO_DEBUG;
			while(i)	
		{
				i--;
			if(ComBuf.rec.buf[CORRE_SITE]==INSTR_FOUR)
				i=0;		
			else 
				UARTPuts( LPC_UART0, Exit_corre);
			INSTRUCT_TO_DEBUG;
		}


}

void Clear_Instr(void)		//���У��
{
	uint8_t Clear_corre[8]={0xAB,0x01,0x08,0x06,0xD0,0x01,0x00,0xBF};
	uint8_t i=3;		
	UARTPuts( LPC_UART0, Clear_corre);		//�˳�У��
//	LONGDELAY;
//		while(i)	
//	{
//			i--;
//		if(ComBuf.rec.buf[CORRE_SITE]==INSTR_FOUR)
//			i=0;		
////		else 
////			UARTPuts( LPC_UART0, Clear_corre);
//		LONGDELAY;
//	}


}

void Close_Clear(void)		//�ر�����
{
	uint8_t Send_buff[8]={0xAB,0X01,0x08,0x06,0xF0,0x00,0x00,0xBF};
	UARTPuts( LPC_UART0, Send_buff);
}


void Open_Clear(void)		//��·����
{
	uint8_t Send_buff[8]={0xAB,0X01,0x08,0x06,0xF0,0x01,0x00,0xBF};
	UARTPuts( LPC_UART0, Send_buff);
}



void Short_Clear(void)			//��·����
{
	uint8_t Send_buff[8]={0xAB,0X01,0x08,0x06,0xF0,0x02,0x00,0xBF};
	UARTPuts( LPC_UART0, Send_buff);
}

void Send_Freq(Send_Ord_Typedef *ord)
{
	uint8_t Send_buff[10]={0xAB,0X01,0x08,0x06};
	Send_buff[4]=ord->Ordel;
	Send_buff[5]=ord->name;
	Send_buff[6]=0X80;//У��� 
	Send_buff[7]=0XBF;
	Send_buff[8]=0;
	UARTPuts( LPC_UART0, Send_buff);	
	//_printf("s%",(const)Send_buff);

}
void Send_UartStart(void)
{
	uint8_t Send_buff[24]={0xAB,0X01,22,0x06};
	uint8_t i=5;
	Send_buff[4]=0x60;
	Send_buff[5]=Disp_Main_Ord[SaveData.Main_Func.Param.test][0];
	Send_buff[6]=0X70;
	Send_buff[7]=Disp_Main_Ord[SaveData.Main_Func.Param.test][1];
	Send_buff[8]=0x71;
	Send_buff[9]=Disp_Main_Ord[SaveData.Main_Func.Param.test][2];
	Send_buff[10]=0x80;
	Send_buff[11]=SaveData.Main_Func.Freq;
	Send_buff[12]=0x90;
	Send_buff[13]=SaveData.Main_Func.Level;
	Send_buff[14]=0xa0;
	Send_buff[15]=SaveData.Main_Func.Speed;
	Send_buff[16]=0xb0;
	Send_buff[17]=SaveData.Main_Func.Range.Range;
//	Send_buff[18]=0xc0;
//	Send_buff[19]=SaveData.Main_Func.Avg;
	Send_buff[18]=0xe0;
	Send_buff[19]=SaveData.Main_Func.Rsou;

//	Send_buff[11]=SaveData.Main_Func.Rsou;
//	Send_buff[10]=0xe0;
//	Send_buff[11]=SaveData.Main_Func.Rsou;
	Send_buff[20]=0X00;//У��� 0
	Send_buff[21]=0XBF;	
//	Send_buff[21]=0X80;//У��� 0
//	Send_buff[22]=0XBF;
//	Send_buff[23]=0;
	UARTPuts( LPC_UART0, Send_buff);	
//	INSTRUCT_TO_DEBUG;
//	while(i)	
//	{
//			i--;
//		if(ComBuf.rec.buf[5]==0x00)
//			i=0;
//		else 
//			UARTPuts( LPC_UART0, Send_buff);
//	INSTRUCT_TO_DEBUG;
//	}
	//_printf("s%",(const)Send_buff);

}
void Send_Request(void)
{
	uint8_t Send_buff[10]={0xAB,0X01,0x06,0x03,0x08,0xbf,0};
//	Send_buff[3]=ord->Ordel;
//	Send_buff[4]=ord->name;
//	Send_buff[5]=0X80;//У��� 
//	Send_buff[6]=0XBF;
//	Send_buff[7]=0;
	UARTPuts( LPC_UART0, Send_buff);
	//_printf("s%",(const)Send_buff);
	


}
void Send_Main_Ord(void)
{
	uint8_t Send_buff[20]={0xAB,0X01,0x0C,0x06};
	//Send_Mainord_Typedef *pt;
	uint8_t *pt;
	uint8_t i;
//	uint8_t dat,Sendflag=3;
	pt=(uint8_t *)&Send_Mainord;
	Send_Mainord.first.name=Disp_Main_Ord[SaveData.Main_Func.Param.test][0];
	Send_Mainord.first.Ordel=0X60;
	Send_Mainord.second.name=Disp_Main_Ord[SaveData.Main_Func.Param.test][1];
	Send_Mainord.second.Ordel=0X70;
	Send_Mainord.third.Ordel=0X71;
	Send_Mainord.third.name=Disp_Main_Ord[SaveData.Main_Func.Param.test][2];
	for(i=0;i<sizeof(Send_Mainord);i++)
	{
		Send_buff[4+i]=*(pt+i);
	
	}
//	Send_buff[3]=Send_Mainord.first.name;
//	Send_buff[4]=ord->name;
	Send_buff[10]=0X00;//У��� 
	Send_buff[11]=0XBF;
//	Send_buff[11]=0;
	UARTPuts( LPC_UART0, Send_buff);
//	sprintf((char *)DispBuf,"%2d",Send_Mainord.first.name);
//	WriteString_16(140, 4, DispBuf,  0);
//	
//	sprintf((char *)DispBuf,"%2d",Send_Mainord.second.name);
//	WriteString_16(170, 4, DispBuf,  0);
//	
//	sprintf((char *)DispBuf,"%2d",Send_Mainord.third.name);
//	WriteString_16(210, 4, DispBuf,  0);
	
}
void Disp_RangeDispvalue(uint8_t data)
{
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(20, 200, RangeDisp_Main[data],  1);	//  Cp��ʾ
	WriteString_16(280, 200, RangeDisp_Second[data],  1);   //Rp��ʾ	
	Colour.black=LCD_COLOR_TEST_MID;
	LCD_ShowFontCN_40_55(80,92+30,40,55, (uint8_t*)Out_Assic+7*40*55/8);
	LCD_ShowFontCN_40_55(80+40,92+30,40,55, (uint8_t*)Out_Assic+24*40*55/8);
	LCD_ShowFontCN_40_55(80+40+20,92+30,40,55, (uint8_t*)Out_Assic+25*40*55/8);
//


}

//������ʾ��Cp�ĵ�λ����ϵ�λ��
void Disp_Range_DispMainUnit(uint8_t num,uint8_t unit)		//
{
    if(num>6)
        num=6;
    if(unit>6)
        unit=6;
	WriteString_16(128, 200, (uint8_t*)Disp_Unit[num],  1);//��λ1
	WriteString_16(138, 200, (uint8_t*)Disp_Range_Main_Disp[unit],  0);//��λ2

}

//������ʾ��Rp�ĵ�λ
void Disp_Range_DispSecondUnit(uint8_t num,uint8_t unit)
{
     if(num>6)
        num=6;
    if(unit>6)
        unit=6;
	WriteString_16(388, 200, (uint8_t*)Disp_Unit[num],  0);//��λ1
	WriteString_16(398, 200, (uint8_t*)Disp_Range_Main_Disp[unit],  0);//��λ2

}


//������ʾ�е�Cp��Rp��ֵ
void Disp_RangeTestvalue(void)
{
	//u8 i;
	const uint8_t DISP_UnitMain[]=
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,3,3,2,2,2};
	const uint8_t DISP_UnitSecond[]=
	{ 6,6,3,2,6,6,2,6,2,2,6,6,6,2,2,5,4,5,4,2,6,6};
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(52,200 ,Test_Dispvalue.Main_valuebuff,1);//
	WriteString_16(312,200 ,Test_Dispvalue.Secondvaluebuff,1);//
	Disp_Range_DispMainUnit(Test_Dispvalue.Unit[0],DISP_UnitMain[SaveData.Main_Func.Param.test]);
	Disp_Range_DispSecondUnit(Test_Dispvalue.Unit[1],DISP_UnitSecond[SaveData.Main_Func.Param.test]);
	//Count_buff
}
const uint8_t NUM_OUT[][4]=
{
	{"1  "},
	{"2  "},
	{"3  "},
	{"4  "},
	{"5  "},
	{"6  "},
	{"7  "},
	{"8  "},
	{"9  "},
	{"OUT"},
	{"0  "},



};
void Disp_Range_Num(uint8_t num)
{
	WriteString_16(300, 160, (uint8_t*)NUM_OUT[num],  0);


}
void Disp_RangeCount(void)
{	
	u8 i;	
	for(i=0;i<11;i++)	
	{
		if((i>1&&i<7)||((i>8)))
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
		Hex_Format(Count_buff[i] , 0, 6 , 0);//�ӵ�λ
				
		WriteString_12(LIST2+140, 58+i*15, DispBuf,  1);//��ʾ���ֵ  ����Ҫ������ʾ��λ
	}

}
void Disp_Set_Unit_16(uint32_t unit,uint32_t x,uint32_t y)//Disp_Range_Main_NUm
{
	WriteString_16(x, y, Disp_Range_Main_Disp[Disp_Range_Main_NUm[unit]],  0);


}
void Disp_Set_Unit_12(uint32_t unit,uint32_t x,uint32_t y)
{
	WriteString_12(x, y, Disp_Range_Main_Disp[unit],  1);


}
void Disp_Test_Unit_v(uint32_t x,uint32_t y)
{
	WriteString_16(x, y, "V",  1);

}
void Disp_Test_Unit_mA(uint32_t x,uint32_t y)
{
	WriteString_16(x, y, "mA",  1);

}
void Savetoeeprom(void)
{
	//Saveeeprom
	u8 *pt;
//	u8 i;
	
	Saveeeprom.Save_Main_Funce.param=SaveData.Main_Func.Param.test;
	Saveeeprom.Save_Main_Funce.freq=SaveData.Main_Func.Freq;
	Saveeeprom.Save_Main_Funce.level=SaveData.Main_Func.Level;
	Saveeeprom.Save_Main_Funce.trig=SaveData.Main_Func.Trig;
	Saveeeprom.Save_Main_Funce.rsou=SaveData.Main_Func.Rsou;
	Saveeeprom.Save_Main_Funce.range=SaveData.Main_Func.Range.Range;
	Saveeeprom.Save_Main_Funce.speed=SaveData.Main_Func.Speed;
	Saveeeprom.Save_Main_Funce.avg=SaveData.Main_Func.Avg;
	Saveeeprom.Save_Main_Funce.v_i=SaveData.Main_Func.V_i;
	Saveeeprom.Save_Main_Funce.level=SaveData.Main_Func.Level;
	Saveeeprom.Save_Main_Funce.beep=SaveData.Main_Func.beep;
	Saveeeprom.Save_Main_Funce.buad=SaveData.Main_Func.buad;
	Saveeeprom.Save_Main_Funce.language=SaveData.Sys_Setup.Language;
	
	Saveeeprom.Limit_Tab=SaveData.Limit_Tab;
	
	pt=(u8*)&Saveeeprom;
	
	EEPROM_Write(0, 0, pt, MODE_8_BIT, sizeof(Saveeeprom));


}
void ReadSavedata(void)
{
	u8 i;
	Saveeeprom_Typedef *pt;
	//Saveeeprom_Typedef 
	pt=&Saveeeprom;
	EEPROM_Read(0, 0, (u8 *)pt, MODE_8_BIT, sizeof(Saveeeprom));
	for(i=0;i<12;i++)
	{
		if(*((u8 *)(pt+i))>50)
			*(u8 *)(pt+i)=0;	
	}
	SaveData.Main_Func.Param.test=Saveeeprom.Save_Main_Funce.param;

	SaveData.Main_Func.Freq=Saveeeprom.Save_Main_Funce.freq;
	SaveData.Main_Func.Level=Saveeeprom.Save_Main_Funce.level;
	SaveData.Main_Func.Trig=Saveeeprom.Save_Main_Funce.trig;
	SaveData.Main_Func.Rsou=Saveeeprom.Save_Main_Funce.rsou;
	SaveData.Main_Func.Range.Range=Saveeeprom.Save_Main_Funce.range;
	SaveData.Main_Func.Speed=Saveeeprom.Save_Main_Funce.speed;
	SaveData.Main_Func.Avg=Saveeeprom.Save_Main_Funce.avg;
	SaveData.Main_Func.V_i=Saveeeprom.Save_Main_Funce.v_i;
	SaveData.Main_Func.Level=Saveeeprom.Save_Main_Funce.level;
	SaveData.Main_Func.beep=Saveeeprom.Save_Main_Funce.beep;
	SaveData.Main_Func.buad=Saveeeprom.Save_Main_Funce.buad;
	SaveData.Sys_Setup.Language=Saveeeprom.Save_Main_Funce.language;
	
	
	SaveData.Limit_Tab=Saveeeprom.Limit_Tab;
	if(SaveData.Limit_Tab.Nom.Unit>6)
		SaveData.Limit_Tab.Nom.Unit=0;	
	if(SaveData.Limit_Tab.Nom.Dot>6)
		SaveData.Limit_Tab.Nom.Dot=3;
	if(SaveData.Limit_Tab.Nom.Num>100000)
		SaveData.Limit_Tab.Nom.Num=10000;
	if(SaveData.Limit_Tab.Comp>2)
		SaveData.Limit_Tab.Comp=0;
	if(SaveData.Limit_Tab.Param>2)
		SaveData.Limit_Tab.Param=0;
	if(SaveData.Limit_Tab.Mode>5)
		SaveData.Limit_Tab.Mode=0;
	if(SaveData.Limit_Tab.count>1)
		SaveData.Limit_Tab.count=0;
	
	
		
	
	for(i=0;i<10;i++)
	{
		if(SaveData.Limit_Tab.Comp_Value[i].high.Dot>5)
			SaveData.Limit_Tab.Comp_Value[i].high.Dot=1;
		if(SaveData.Limit_Tab.Comp_Value[i].high.Unit>10)
			SaveData.Limit_Tab.Comp_Value[i].high.Unit=1;
		if(SaveData.Limit_Tab.Comp_Value[i].high.Num>100000)
			SaveData.Limit_Tab.Comp_Value[i].high.Num=0;
		
		if(SaveData.Limit_Tab.Comp_Value[i].low.Dot>5)
			SaveData.Limit_Tab.Comp_Value[i].low.Dot=1;
		if(SaveData.Limit_Tab.Comp_Value[i].low.Unit>10)
			SaveData.Limit_Tab.Comp_Value[i].low.Unit=1;
		if(SaveData.Limit_Tab.Comp_Value[i].low.Num>100000)
			SaveData.Limit_Tab.Comp_Value[i].low.Num=0;		
	}



}


void lcd_image(uint8_t *pt)
{
    unsigned long i;
	uint32_t color;
	unsigned long *pDst = (unsigned long *)LCD_VRAM_BASE_ADDR;
  
    for( i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
		color=(uint32_t)*pt<<16|(uint32_t)*(pt+1)<<8|*(pt+2);
    	*pDst++ = color; 
		*pt++;
		*pt++;
		*pt++;
    }
}
void Send_Uart3(uint8_t *buff)
{
//	UARTPutChar (LPC_UART3,0xaa);
	UARTPuts(LPC_UART3,buff);
	


}
void Disp_Usbflag(uint8_t flag)
{
	Colour.black=LCD_COLOR_TEST_BUTON;
	if(flag==1)
		Colour.Fword=LCD_COLOR_GREEN;
		
	else if(flag==2)
		Colour.Fword=LCD_COLOR_BLUE;
	else
		Colour.Fword=LCD_COLOR_TEST_BUTON;
//	LCD_ShowFontCN_40_55(uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize,uint8_t * image);
	LCD_ShowFontCN_40_55(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE,BUTTOM_Y_VALUE,48,24,(uint8_t*)nBitmapDot);
	//WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, " ��ʾ",  0);
	

}
