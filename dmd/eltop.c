
/**************************************************************************************************
    Filename:       bleuart.c
    Revised:
    Revision:

    Description:    This file contains the Simple BLE Peripheral sample application


**************************************************************************************************/

#include <string.h>
#include "types.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "pwrmgr.h"
#include "rf_phy_driver.h"
#include "flash.h"
#include "fs.h"
#include "osal_snv.h"
#include "log.h"

#include "app_datetime.h" //#include "OSAL_Clock.h"
#include "eltop.h"
#include "pwm.h"
#include "DMD.h"
#include "jdwsl.h"
//#include "I2Csoft.h"
//#include "i2c.h"
#include "adc.h"
#include "osal_snv.h"
//#include "prosedure.h"
//#include "battery.h" //#include "ntc.h"


extern void* pi2c;

extern void jam (void);
//extern void big_4digit(void);
extern void jdws_sow4 (void);
extern void hitung_jws(void);
extern void cek_jwds(void);
extern void rxscan(void);
void msk_sholat43(void);
void IQOMAH43(void);
void read_data_flash (void);
void iqomah_off(void);
void disp_panel (void);

uint8 eltop_TaskID;   // Task ID for internal task/event processing

extern uint8_t britnes;

extern js jd;
extern uint32_t trun;
extern uint8_t code[8];
extern char hour[10];
extern uint8_t AMP,MENIT,secon,hari,tgl,bln,th,HOUR,urut_sow;
//char *buf;
//extern char dt_rx[500];

RMT rmt;
SET set;
flg_bit BIT;
ADZAN_t adzan;
OFDISP_t ofdisp;
WKT_t wkt;
WKT_FIX_t wkt_fix;
CEK_t cek;
BUF bfr;
extern datetime_t Ti;
extern uint8 code_sht,KLM,urut;
extern u8 contr_bh,fg_ganti,code_sht,pesan_alrm,iqm_off_mnt,iqm_contr_dtk,iqm_contr_mnt,iqm_off_dtk;

void timer_process(uint8_t evt)
{
	if(evt==HAL_EVT_TIMER_5) scanDisplayBySPI();
   /* switch(evt)
    {
    case HAL_EVT_TIMER_5:
			 scanDisplayBySPI();
        //if(++test>5000) { test=0; LOG("t5\n"); };
        break;

    case HAL_EVT_WAKEUP:
        LOG("wakeup\n");
        LOG("timer will disable when sleep,so if you want it work please init it when wakeup");
        break;

    case HAL_EVT_SLEEP:
        LOG("sleep\n");
        break;

    default:
        LOG("err ");
        break;
    }*/
}

//int  messageOffset;
void eltop_Init( uint8 task_id )	{
    eltop_TaskID = task_id;
    
		hal_pwm_init(PWM_CH0, PWM_CLK_DIV_8,PWM_CNT_UP, PWM_POLARITY_FALLING);
		hal_pwm_set_count_val(PWM_CH0, 50, 256);
		hal_pwm_open_channel(PWM_CH0,P34);
    // Setup a delayed profile startup
		hal_pwm_start();
	hal_gpio_pin_init(P2,GPIO_OUTPUT);  // A
	hal_gpio_pin_init(P3,GPIO_OUTPUT);  // B
	hal_gpio_pin_init(P7,GPIO_OUTPUT);  // Cl
	hal_gpio_pin_init(P11,GPIO_OUTPUT);  // S
	hal_gpio_pin_init(P14,GPIO_OUTPUT);  // R
	
	DMD(4,1); clearScreen(1);selectFont(System5x7);

	//hal_gpio_pin_init(GPIO_P24,IE);
	//hal_gpio_pin_init(GPIO_P23,IE);
	//hal_gpio_pull_set(GPIO_P24,STRONG_PULL_UP);
	//hal_gpio_pull_set(GPIO_P23,STRONG_PULL_UP);
	//hal_i2c_pin_init(I2C_0, GPIO_P23, GPIO_P24);
	//pi2c =hal_i2c_init(I2C_0,I2C_CLOCK_100K);
	
	//uint32_t batt_degree = batt_voltage();
	//LOG("bat:%d\n",batt_degree);
	osal_start_reload_timer( eltop_TaskID, EVENT_05S, 500);
	  //osal_start_reload_timer( eltop_TaskID, EVENT_1_DETIK, 1000);
	//buf = (char *)osal_mem_alloc(100);
	osal_set_event(eltop_TaskID, TEST_PROG);
	osal_start_timerEx( eltop_TaskID, TEST_PROG2, 3000);
	//osal_start_timerEx( eltop_TaskID, EVENT_ADZAN, 5000);
	//osal_start_timerEx( eltop_TaskID, TEST_PROG3, 500);
	hal_timer_init(timer_process);
	hal_timer_set(AP_TIMER_ID_5,220);
    //LOG("Eltop-Init \n");
		set.britnes = 5; set.speed=10;//trun = osal_GetSystemClock(); 
	LOG("FS-Osal-Init=%d\n",osal_snv_init()); // cek init flash
	//if( !hal_fs_item_read(fs_bios,(uint8*)bfr.dt_rx,6,NULL) ) {
	//LOG("init read=%d\n",hal_fs_item_read(fs_bios,(uint8*)bfr.dt_rx,6,NULL) );
	//set.britnes=bfr.dt_rx[0]; set.speed=bfr.dt_rx[1]; set.big_hr=bfr.dt_rx[2]; set.plh_hemat=bfr.dt_rx[3];}
    //PWM_SET_CMP_VAL(PWM_CH0,set.britnes*10);
	app_datetime_init();
	//Ti.hour=0; Ti.minutes=0; Ti.seconds =0; Ti.day=1; Ti.month=1; Ti.year=2022;
	//osal_ConvertUTCSecs( &Ti ); osal_setClock( App_time );
	
	read_data_flash (); PWM_SET_CMP_VAL(PWM_CH0,set.britnes*10);
	BIT.A=1; jam();  hitung_jws();
}


uint8 X; 
//bool A=1;
uint16 eltop_ProcessEvent( uint8 task_id, uint16 events )
{
    VOID task_id; // OSAL required parameter that isn't used in this function
	if (events & EVENT_05S ) {   
		BIT.dot = ~BIT.dot; 
		if(BIT.dot)  { hour[2]=':'; jam(); } else {hour[2]=' '; };
			// AT_LOG("HOUR %02d:%02d:%02d \n",fg.dot,bcdToDec(MENIT),bcdToDec(secon) ); 
		if(++X>4) { X=0; if(++urut_sow>8) urut_sow=0; };
		if(BIT.wkt_azan && BIT.dot ) { if(++set.tim_adzan>(set.adzan*60)) {set.tim_adzan=0; BIT.wkt_azan=0; BIT.iqomah=1;
			iqm_contr_dtk =0; iqm_off_mnt = 0; iqm_contr_mnt = bfr.buf_timer_iqm[code_sht]; BIT.one =0; }
			};
		if(BIT.iqomah && BIT.dot ) {   // iqmh qktif
			if(iqm_contr_dtk==0 || iqm_contr_dtk==0xff ) { iqm_contr_mnt = iqm_contr_mnt-1; iqm_contr_dtk=59; } else {
			iqm_contr_dtk--; };
			if( iqm_contr_dtk==10 && (iqm_contr_mnt ==0 || iqm_contr_mnt ==0xFF) ) { BIT.buzer=1;  };
			if( iqm_contr_dtk==0 && (iqm_contr_mnt ==0 || iqm_contr_mnt ==0xFF) ) { BIT.iqomah=0; BIT.buzer=0;  
				 iqm_off_dtk=0;  iqomah_off();  BIT.iqm_off=1; clearScreen(1); return (events ^ EVENT_05S); };
		  };
		if(BIT.iqm_off) {	 //timer iqm off
			if(iqm_off_dtk==0) { iqm_off_mnt = iqm_off_mnt-1; iqm_off_dtk=59; } else {iqm_off_dtk--; };
			if( iqm_off_dtk==0 && iqm_off_mnt==0) {BIT.iqomah=0; BIT.buzer=0; BIT.iqm_off=0; set.fix_jam=0; };		
   		  };
		if(BIT.jumat) { if(++set.contr_jumat>(60*set.jumat)) {set.contr_jumat=0; BIT.jumat=0; BIT.iqomah=0; }; };  
			
		return (events ^ EVENT_05S);
	}
		
	if (events & TEST_PROG )   {
		/*if (trun + (set.speed*4) < osal_GetSystemClock()) { trun = osal_GetSystemClock();
			if(BIT.A) {
				//clearScreen(1);
				selectFont(System5x7);//selectFont(Arial_Black_16);
				//drawString(40,0,"ELTOP-INOVATIF SURABAYA GERBANG K-SUSILA ",28,GRAPHICS_NORMAL);
				drawMarquee("INDONESIA RAYA MERDEKA YANG KE 77TH ",36,9);
				BIT.A=0; //big_4digit();
			}
			if(!BIT.A) {
				selectFont(System5x7); BIT.A=stepMarquees(-1,0);
				jdws_sow4 ();
			}
		}*/
		disp_panel();
		if(BIT.RX) {  BIT.RX=0;  osal_set_event(eltop_TaskID, TEST_PROG3); 
			LOG("RX-dt=%d || %s *%c\n",strlen(bfr.dt_rx),bfr.dt_rx,bfr.dt_rx[0]);
			return ( events ^ TEST_PROG ); }
		if(BIT.wkt_azan) {  osal_set_event(eltop_TaskID, EVENT_ADZAN); 
			clearScreen(1); BIT.A=1; return ( events ^ TEST_PROG ); }
		osal_start_timerEx( eltop_TaskID, TEST_PROG, 1);
		return ( events ^ TEST_PROG );
	}
	
	if (events & TEST_PROG2 ) // test 7segTM1637
		{  
		//if(BIT.RX) { LOG("Cek-RX\n"); BIT.RX=0; }
		//bfr.dt_rx[0]=0; bfr.dt_rx[1]=0 ; bfr.dt_rx[2]=0; bfr.dt_rx[3]=0;
		//LOG("Read=%d\n",hal_fs_item_read(fs_bios,(uint8*)bfr.dt_rx,4,NULL));
		//extern void disp_panel (void);//read_data_flash ();	
			//disp_panel();
 LOG("HOUR %02d:%02d:%02d  %02d-%02d-%02d  \n",Ti.hour,Ti.minutes,Ti.seconds,Ti.day,Ti.month,Ti.year ); 	    
		cek_jwds(); 	
		//LOG("T-data=%d,%d,%d,%d,%d,%d,%d\n",ofdisp.duhur,adzan.duhur,set.adzan,iqm_contr_mnt,iqm_off_mnt,code_sht,set.iqm_dhr);
		osal_start_reload_timer( eltop_TaskID, TEST_PROG2, 1000); 
		return (events ^ TEST_PROG2);
	}
		
	if (events & TEST_PROG3 )  { // test 7segTM1637
		//batt_measure();
		//uint32_t batt_degree = batt_voltage();
		//LOG("bat:%d\n",batt_degree);
	   //osal_start_timerEx( eltop_TaskID, TEST_PROG3, 2000);
		rxscan();
		if(bfr.dt_rx[0]=='^') { PWM_SET_CMP_VAL(PWM_CH0,(set.britnes*10)); }
		//LOG("bfr.dt_rx= %c,%c\n",bfr.dt_rx[1],bfr.dt_rx[2]);}//LOG("Cek-RX\n");
		//osal_memset(bfr.dt_rx,0,10);
		//LOG("read=%d\n",hal_fs_item_read(fs_bios,(uint8*)bfr.dt_rx,6,NULL) );
		//LOG("data=%d,%d,%d \n",bfr.dt_rx[0],bfr.dt_rx[1],bfr.dt_rx[2] );
		//osal_start_reload_timer( eltop_TaskID, TEST_PROG, 1);
		osal_set_event(eltop_TaskID, TEST_PROG);
		return (events ^ TEST_PROG3);
	}	
	
	if (events & EVENT_ADZAN )  { // test 7segTM1637
		osal_stop_timerEx( eltop_TaskID, TEST_PROG);
		msk_sholat43();
		if(BIT.iqomah) { osal_set_event( eltop_TaskID, EVENT_IQOMAH); clearScreen(1); BIT.A=1;
			return (events ^ EVENT_ADZAN); }
		if(BIT.RX) {  BIT.RX=0;  osal_set_event(eltop_TaskID, TEST_PROG3); 
			return ( events ^ EVENT_ADZAN ); }
		osal_start_timerEx( eltop_TaskID, EVENT_ADZAN, 1);
		return (events ^ EVENT_ADZAN);
	}
	if (events & EVENT_IQOMAH )  { // test 7segTM1637
		osal_stop_timerEx( eltop_TaskID, EVENT_ADZAN);
		IQOMAH43();
		if(!BIT.iqomah && !BIT.iqm_off) { osal_set_event( eltop_TaskID, TEST_PROG); clearScreen(1); BIT.A=1;
			return (events ^ EVENT_IQOMAH); }
		if(BIT.RX) {  BIT.RX=0;  osal_set_event(eltop_TaskID, TEST_PROG3); 
			return ( events ^ EVENT_IQOMAH ); }
		osal_start_timerEx( eltop_TaskID, EVENT_IQOMAH, 1);
		return (events ^ EVENT_IQOMAH);
	}
    return 0;
}


/*********************************************************************
*********************************************************************/
