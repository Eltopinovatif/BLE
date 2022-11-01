#ifndef __JDWSL_H
#define __JDWSL_H

#include "OSAL.h"
#ifdef __cplusplus
 extern "C" {
#endif 

//#include "stdlib.h"
#include "math.h"
//#include <seting.h>

typedef struct _js {
     float  Lintang,bujur ,JDgw,dzuhur, zona;
     int  BB, M,Y,A,H,sdt_subuh,sdt_isak,mazhab;
	   unsigned char hour,menit;
     unsigned char pos[18],tambah[10]; 
     //unsigned int  waktu[7];
     }js;

#define pi              3.14159
//const unsigned char dt_wkt[8] ={0,3,4,5,7,9,12};

void JDgrenwitc (int tanggal,int bulan,int tahun);	 
//hitung julian day lokal dipengarui ZONA +7
float JDlok(void);
// menghitung sudut per geseran matahari
float sdt_tgl  (void); 
// menghitung masing2 U
float U (void);
// menghitung masing2 L0
float L0 (void);
// menghitung Radian pergeseran matahari
float delta (void);
// pergeseran matahari per menit
float Eqs (void);

static float rad_ldtde (void) ;
static void tambahan (unsigned char dtr);

static void conversi(float contv);
void WAKTU_DZUHUR (void);

void WAKTU_ASHAR (void) ;
void WAKTU_MAGRIB (void) ;
void WAKTU_SLT_SUDUT (float sudut,uint8_t pilih);
//void WAKTU_ISAK (void) ;

//void WAKTU_SUBUH (void);
//void WAKTU_IMSAK (void) ;

   
#ifdef __cplusplus
}
#endif


#endif 	


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
			