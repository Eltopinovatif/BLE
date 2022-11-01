
/**************************************************************************************************
    Filename:       eltop.h
    Revised:
    Revision:

    Description:    This file contains the Simple BLE Peripheral sample application
                  definitions and prototypes.


**************************************************************************************************/
#include "app_datetime.h" //#include "OSAL_Clock.h"

#define TEST_PROG                                         0x0008  // used text
#define TEST_PROG2                                        0x0002  // used text
#define TEST_PROG3                                        0x0004  // used text
#define TEST_PROG4                                        0x0010  // used text
#define TEST_PROG5                                        0x0020  // used text
#define EVENT_05S                                         0x0001  // used timer 0.5-Sec
#define EVENT_ADZAN                                       0x0040  // used alarm sholat
#define EVENT_IQOMAH                                      0x0080  // used Iqomah
extern uint8 eltop_TaskID; // Task ID for internal task/event processing

typedef struct RMT{
	int arab:1,jawa:1;
} RMT;
typedef struct _flg_bit { // bit data fung
	int A:1, B:1, C:1, big_hour:1;
	int wkt_azan :1, buzer:1, dot:1, iqomah:1;
	int msg_iqm:1,one:1,jumat:1,iqm_off:1;
	int RX:1,urut;
 }flg_bit;
typedef struct _SET{ //seting program
	uint16_t cdkota;
    uint8_t  vol, vc_sbh,vc_dhr,vc_asr,vc_mgb,vc_isak;
	uint8_t  iqm,iqm_sbh,iqm_dhr,iqm_asr,iqm_mgb,iqm_isak,iqm_off;
	uint8_t  big_hr,speed,britnes,adzan,jumat,plh_jam,panel,plh_hemat,msg_jum,iqm_dot,sow; //jum10..tot 31
	uint8_t ajds_sbh,ajds_dhr,ajds_asr,ajds_mgb,ajds_isak;
	//uint8_t vc_jumatan,vc_mlm_jum,cek_mlm_jum;
	uint8_t IQM_disp,hari,ajust_cldr,cek_font,mdl_hj;
	uint8_t ajds_terbit,ajds_duha,fix_jam,jwsDisply2;
	uint8_t tim_adzan,polaris,contr_jumat;
 }SET; 
typedef struct _ADZAN_t{ // fixcek
  uint8_t subuh,duhur,ashar,magrib,isak;
 } ADZAN_t;
typedef struct _OFDISP_t { // fixcek
  uint8_t subuh,duhur,ashar,magrib,isak,jumat,jumat_psn;
 } OFDISP_t;
typedef struct _WKT_t{ // waktu sholat 5wkt
  uint16_t subuh,duhur,ashar,magrib,isak,total,jumat,jumatan;
 }WKT_t;
typedef struct _WKT_FIX_t { // waktu seblm sholat 5wkt
  uint16_t subuh,duhur,ashar, magrib,isak,jumat;
 } WKT_FIX_t;
typedef struct _CEK_t{ // waktu seblm sholat 5wkt
  uint8_t subuh,duhur,ashar, magrib,isak,jumat,mdl_hemat,alrmPerjam;
 }CEK_t;
typedef struct _BUF{ // waktu seblm sholat 5wkt
  char dt_rx[256];
  char hour1[10],hour2[10],bufr1[100],bufr[256];
  uint8_t buf_timer_iqm[8];
 }BUF;
void hitung_jws(void);
void timer_process(uint8_t evt);
void eltop_Init( uint8 task_id );
uint16 eltop_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/



