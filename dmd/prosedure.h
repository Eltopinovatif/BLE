
/**************************************************************************************************
    Filename:       prosedure.h
    Revised:
    Revision:

    Description:    This file contains the Simple BLE Peripheral sample application
                  definitions and prototypes.
**************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "types.h"
#include "OSAL.h"
#include "DMD.h"
#include "jdwsl.h"
//#include "I2Csoft.h"

#include "SystemFont5x7.h"
//#include "bold_8x8.h"
//#include "fixednums8x16.h"
#include "font9x7.h"
#include "angka7x8.h"
//#include "angka8x8.h"	 
#include "arabic10.h"
//#include "arabic_6.h"
//#include "Arial_black_16.h"
//#include "angka8x16.h"	 
//#include "Font7x8Bold.h"
//#include "bold_6x12.h"
//#include "bold_8x8.h"
//#include "bold_7x8.h"
#include "Droid_Sans_12.h"
//#include "font_iqomah.h"
//#include "angka_10x6.h"	
#include "arabic_60.h"
#include "fixednums8x160.h"


void arabic (unsigned char G);
void arabic6( void );
void big_44digit(void);
void mini_8x8(void);
void parameter_big_jam (void);
void big_hour4(void);
void gbri (void);
void fixjam3 (void);
void big_hour3(void);
void big_hour2(void);
void big_4digit(void);
void fixjam (void);
void jam_bsr_kiri (void);
void jam_mini_bold (void);
void jdws_sow4 (void);
void gbr (void);
void jdws_sow3 (void);
void jam_dtk4(void);
void jam_dtk3(void);
void show_jds (void);
void big_hour6(void);
void big_hour5(void);
void jam_mini_bold56 (void);
void show_jds2 (void);
void show_jds22 (void);
void jdws_sow6 (void);
void jdws_sow26 (void);
void jdws_sow5 (void);
void jam_dtk6(void);
void jam_dtk26(void);
void jam_dtk5(void);
void show_hour (uint8_t NO);
void call_jwds(void);

void hitung_jws(void);
void cek_jwds(void);
void msk_sholat43(void);
void msk_sholat2(void);
void iqm_mn(void);
void BIG_IQM43 (void);
void IQOMAH43(void);
void BIG_IQM2(void);
void IQOMAH2 (void);
void JUMATAN (void);
void call_jwds56(void);
void msk_sholat65(void);
void BIG_IQM56 (void);
void IQOMAH65(void);
void JUMATAN56 (void);
uint16_t time_set(uint8_t input0,uint8_t input1,uint8_t input2);
void pilih_azan (void);
uint32_t jumlahHariDariTanggal(uint8_t tanggal, uint8_t bulan, uint16_t tahun);
void masehiKeHijriah(uint32_t HariMasehi, uint8_t *tanggal, uint8_t *bulan, uint16_t *tahun);
void up_date_hij_jawa (void);
void call_hijriah(void);
void sow_panel (void);
void disp_panel (void);
void disp_panel56 (void);
//void hitung_jws(void);
//void timer_process(uint8_t evt);
//void eltop_Init( uint8 task_id );
//uint16 eltop_ProcessEvent( uint8 task_id, uint16 events );
void rxscan(void);
void read_data_flash (void);
void set_buf_iqm(void);
void iqomah_off(void);
/*********************************************************************
*********************************************************************/



