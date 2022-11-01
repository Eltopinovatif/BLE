
#include <stdio.h>
#include "types.h"
#include "i2c.h"
#include "prosedure.h"
#include "jdwsl.h"
#include "fs.h"
#include "osal_snv.h"
#include "eltop.h"

const char *dt_day[7] = { " AHAD "," SENIN","SELASA"," RABU "," KAMIS","JUM'AT"," SABTU" }; //	
const char *dt_day1 [7]={"AHAD"," SEN"," SEL"," RAB"," KAM","JUM"," SAB"};
const char *dt_caldr2 [12]={"JAN","FEB","MAR","APR","MEI","JUN","JUL","AGT","SEP","OKT","NOV","DES"};
const char *dt_caldr [12]={"Januari","Februari","Maret","April","Mei","Juni","Juli","Agustus","September","Oktober","November","Desember"};
const char *dt_sholat[5]={"SHUBUH","DHUHUR","ASHAR","MAGHRIB","ISYA'"};
//const static char *dino_jowo[] = {"SENEN", "SELOSO", "REBO", "KEMIS", "JEMUWAH", "SEPTU", "MINGGU"};
//const static char *pasaran[] = {"LEGI", "PAING", "PON","WAGE", "KLIWON"};
//const static char *bln_jowo[] = {"SURO","SAPAR","MULUD","BAKDA MULUT","JUMADILAWAL","JUMADILAKIR","REJEB","RUWAH","POSO","SAWAL","SELO","BESAR"};
const  char *namaBulanHijriah[] = {"MUHARRAM","SAFAR","RABIUL AWAL","RABIUL AKHIR","JUMADIL AWAL","JUMADIL AKHIR","RAJAB","SYA'BAN","RAMADHAN","SYAWAL","DZULKAIDAH","DZULHIJJAH"};

extern BUF bfr;
extern js jd;
extern RMT rmt;
extern flg_bit BIT;
extern SET set;
extern ADZAN_t adzan;
extern OFDISP_t ofdisp;
extern WKT_t wkt;
extern WKT_FIX_t wkt_fix;
extern CEK_t cek;

char hour[10];
uint8_t code[8];
uint8_t AMP,MENIT,secon,hari,tgl,bln,th,HOUR,urut_sow,fg_man_iqm,suara;
u8 contr_bh,fg_ganti,code_sht,pesan_alrm,iqm_off_mnt,iqm_contr_dtk,iqm_contr_mnt,iqm_off_dtk;
u8 conter_beep,urut,FONT,day=1;
uint16 len;
uint32_t trun;
uint16_t jumlahHariPerBulanHijriah[] = {0,30,59,89,118,148,177,207,236,266,295,325,354};
uint16_t jumlahHariPerBulanMasehi[]={0,31,59,90,120,151,181,212,243,273,304,334};
double tahunBulan = 354.367068;
double tahunMatahari = 365.25;
uint32_t jumlahHari;
double jumlahHariMatahari;
   
uint8_t tanggalMasehi,bulanMasehi,tanggalHijriah,bulanHijriah ;
uint16_t tahunMasehi,tahunHijriah ;
datetime_t Ti;
	 
void android () {}
void proses_data(uint8_t in1,uint8_t in2,uint8_t in3,uint8_t in4,uint8_t in5) {
	bfr.hour1[0]=in1;bfr.hour1[1]=in2; bfr.hour1[2]=in3;bfr.hour1[3]=in4;bfr.hour1[4]=in5; 
 }

extern void* pi2c;
void jam (void)  {
	//I2C_10bit_Mult_Read( 0xd0 , 0 , code , 7 );
	//int ret = hal_i2c_read(pi2c, 0x68, 0, code, 7);
	app_datetime(&Ti);//osal_getClock(); osal_ConvertUTCTime(&Ti,osal_getClock()); 
	//code1[0]=1; code1[1]=2; code1[2]=3;
	AMP =  Ti.hour; //bcdToDec( code[2] );
	MENIT= Ti.minutes; //(code[1]);
    secon = Ti.seconds; //(code[0]); //secon = 47;
	//hari = day; //bcdToDec(code[3]);
	tgl  = Ti.day; //(code[4]);
	bln  = Ti.month; //(code[5]);
	th   = Ti.year % 2000; //(code[6]);
	HOUR=AMP;
	//hari=RTC_DateStructure.RTC_WeekDay;
	wkt.total=(HOUR*100)+MENIT;
	//if (pam)   {if (AMP>=13) AMP=AMP-12; if (AMP==0) AMP=12; };
    hour[7] =( secon%10)+0x30; hour[6] = (secon/10)+0x30; // menit
	hour[4] =( MENIT%10)+0x30; hour[3] = (MENIT/10)+0x30; // menit
    hour[1] =(AMP%10)+0x30; 	 hour[0] =(AMP/10)+0x30; //}
    if(day<1 || day>7) day=0;
	uint8 cek = secon;
	if( !cek && !secon && !MENIT ) { if(++day >7) day=1; };
	//tanggalMasehi=tgl; bulanMasehi=bln; tahunMasehi=th;
// if( hari==5 && HOUR==0 && MENIT==0 ) {
	// if(hari==5){jd.pos[2]=wkt.jumat/100; jd.pos[3]=wkt.jumat%100;}else{jd.pos[2]=wkt.duhur/100; jd.pos[3]=wkt.duhur%100;}
	// }
	hari = day;
  }	 

void arabic (unsigned char G) {
   selectFont( arabic10 );
   drawChar( 0+G,0,hour[0], GRAPHICS_NORMAL);
    if(hour[2]==' ') hour[2]=';';
   drawChar( 22+G,0,hour[2], GRAPHICS_NORMAL);
   drawChar(11+G,0,hour[1], GRAPHICS_NORMAL);
   drawChar( 32+G,0,hour[3], GRAPHICS_NORMAL);
   drawChar( 43+G,0,hour[4], GRAPHICS_NORMAL);
}

void arabic6( void ) {
   selectFont( arabic_60 );
   drawChar( 0,0,hour[0], GRAPHICS_NORMAL);
   drawChar( 7,0,hour[1], GRAPHICS_NORMAL);
   if(hour[2]==' ') hour[2]=';';
   drawChar( 13,0,hour[2], GRAPHICS_NORMAL); 
   drawChar( 19,0,hour[3], GRAPHICS_NORMAL);
   drawChar( 26,0,hour[4], GRAPHICS_NORMAL);
}

void big_44digit(void) {
	selectFont(fixednums8x160); 	
	if(hour[2]==' ') hour[2]=';';
	drawChar( 13,0,hour[2], GRAPHICS_NORMAL);
	drawChar( 0,0,hour[0], GRAPHICS_NORMAL);
	drawChar( 7,0,hour[1], GRAPHICS_NORMAL);
	drawChar( 19,0,hour[3], GRAPHICS_NORMAL);
	drawChar( 26,0,hour[4], GRAPHICS_NORMAL);
}

void mini_8x8(void) {
    selectFont(angka7x8);  //selectFont( angka8x8 );
    drawChar( 10,0,hour[0], GRAPHICS_NORMAL);
    drawChar( 19,0,hour[1], GRAPHICS_NORMAL);
    if(hour[2]==' ') hour[2]=';';
    drawChar( 27,0,hour[2], GRAPHICS_NORMAL);
    drawChar( 35,0,hour[3], GRAPHICS_NORMAL);
    drawChar( 44,0,hour[4], GRAPHICS_NORMAL);  
  }

void parameter_big_jam (void) {
    clearScreen(1);  contr_bh=set.big_hr; BIT.big_hour=1;
	 if(fg_ganti==0) rmt.arab=0;
	 if(fg_ganti==1) rmt.arab=1;
}

void big_hour4(void) {
	  parameter_big_jam ();
	while(BIT.big_hour) {
	switch (urut_sow) {
ee:
    case 0: sprintf( bfr.bufr1,"  IMSAK %02u:%02u   ",jd.pos[10],jd.pos[11]); break;
    case 1: sprintf( bfr.bufr1," SHUBUH %02u:%02u   ",jd.pos[0],jd.pos[1]); break;
	case 2: sprintf( bfr.bufr1," TERBIT %02u:%02u   ",jd.pos[12],jd.pos[13]); break;
	case 3: sprintf( bfr.bufr1,"  DHUHA %02u:%02u   ",jd.pos[14],jd.pos[15]); break;
	case 4: sprintf( bfr.bufr1," DZUHUR %02u:%02u   ",jd.pos[2],jd.pos[3]); break;
	case 5: sprintf( bfr.bufr1,"  ASHAR %02u:%02u   ",jd.pos[4],jd.pos[5]); break;
	case 6: sprintf( bfr.bufr1,"MAGHRIB %02u:%02u   ",jd.pos[6],jd.pos[7]); break;
	case 7: sprintf( bfr.bufr1,"  ISYA' %02u:%02u   ",jd.pos[8],jd.pos[9]); break;
	case 8:  urut_sow=0; goto ee;
};  jam();
	if(rmt.arab) {  arabic6(); } else {	big_44digit();	};
	 selectFont( SystemFont5x7 );
    drawString( 42,0,bfr.bufr1,13, GRAPHICS_NORMAL); 
	sprintf( bfr.bufr1,"%s,%02u-%02u-20%02u   ",dt_day1[hari-1],tgl,bln,th);
	drawString( 36,9,bfr.bufr1,15, GRAPHICS_NORMAL );
		
	if(BIT.wkt_azan) break;	 android ();
	} if(fg_ganti==2) rmt.arab=~rmt.arab;
clearScreen(1);
}	

void gbri (void) {
	drawString( 33,0,bfr.bufr1,6, GRAPHICS_NORMAL); 
	drawString( 71,0,bfr.hour2,2,GRAPHICS_NORMAL);  
	drawString( 85,0,bfr.hour1,2, GRAPHICS_NORMAL);
	writePixel(83,2,GRAPHICS_NORMAL, 1); writePixel(83,4,GRAPHICS_NORMAL, 1);
	writePixel(83,1,GRAPHICS_NORMAL, 0); writePixel(83,3,GRAPHICS_NORMAL, 0);
	writePixel(83,5,GRAPHICS_NORMAL, 0);
   
	writePixel(69,0,GRAPHICS_NORMAL, 0); writePixel(69,4,GRAPHICS_NORMAL, 0);
	writePixel(69,1,GRAPHICS_NORMAL, 0); writePixel(69,5,GRAPHICS_NORMAL, 0);
	writePixel(69,2,GRAPHICS_NORMAL, 0); writePixel(69,6,GRAPHICS_NORMAL, 0);
	writePixel(69,3,GRAPHICS_NORMAL, 0); writePixel(69,7,GRAPHICS_NORMAL, 0);
 }

void fixjam3 (void) {
   if(hour[2]==' ') hour[2]=';';
   if(rmt.arab)  selectFont( arabic_60 ); else selectFont(fixednums8x160);   
   drawChar( 1,0,hour[0], GRAPHICS_NORMAL);
   drawChar( 14,0,hour[2], GRAPHICS_NORMAL);
   drawChar( 8,0,hour[1], GRAPHICS_NORMAL);
   drawChar( 18,0,hour[3], GRAPHICS_NORMAL);
   drawChar( 25,0,hour[4], GRAPHICS_NORMAL);
}  

void big_hour3(void) {
	parameter_big_jam ();
	while(BIT.big_hour) { selectFont( SystemFont5x7 );	
	switch (urut_sow) {
			//ee:
		case 0: sprintf( bfr.bufr1," IMSAK");sprintf( bfr.hour2,"%02u ",jd.pos[10]); sprintf(bfr.hour1,"%02u ",jd.pos[11]); 
		        gbri(); break;
		case 1: sprintf( bfr.bufr1,"SHUBUH");sprintf( bfr.hour2,"%02u ",jd.pos[0]);sprintf(bfr.hour1,"%02u ",jd.pos[1]); 
		         gbri(); break;
		case 2: sprintf( bfr.bufr1,"TERBIT");sprintf( bfr.hour2,"%02u ",jd.pos[12]);sprintf(bfr.hour1,"%02u ",jd.pos[13]); 
		         gbri(); break;
		case 3: sprintf( bfr.bufr1," DHUHA");sprintf( bfr.hour2,"%02u ",jd.pos[14]);sprintf(bfr.hour1,"%02u ",jd.pos[15]); 
		         gbri(); break;
		case 4: sprintf( bfr.bufr1,"DZUHUR");sprintf( bfr.hour2,"%02u ",jd.pos[2]);sprintf(bfr.hour1,"%02u ",jd.pos[3]); 
		         gbri(); break;
		case 5: sprintf( bfr.bufr1," ASHAR");sprintf( bfr.hour2,"%02u ",jd.pos[4]);sprintf(bfr.hour1,"%02u ",jd.pos[5]); 
		         gbri(); break;
		case 6: sprintf( bfr.bufr1,"MAGHRB");sprintf( bfr.hour2,"%02u ",jd.pos[6]);sprintf(bfr.hour1,"%02u ",jd.pos[7]);
		         gbri(); break;
		case 7: sprintf( bfr.bufr1," ISYA'");sprintf( bfr.hour2,"%02u ",jd.pos[8]);sprintf(bfr.hour1,"%02u ",jd.pos[9]);
		         gbri();  break;
		case 8:  sprintf( bfr.bufr1,"  %s     ",dt_day[hari-1]); drawString( 34,0,bfr.bufr1,11, GRAPHICS_NORMAL); 
	 };  jam();
		
	fixjam3();
	sprintf( bfr.bufr1,"%02u-%02u-20%02u",tgl,bln,th);
	selectFont(SystemFont5x7 );    drawString( 37,9,bfr.bufr1,11, GRAPHICS_NORMAL);
	if(BIT.wkt_azan) break;	
	android ();
	} if(fg_ganti==2) rmt.arab=~rmt.arab; 
	clearScreen(1);
}	

void big_hour2(void) {
	parameter_big_jam ();
	while(BIT.big_hour) { jam(); 
	if(rmt.arab) {  arabic(6); } else {
		selectFont(fixednums8x160); 			
		if( hour[0]==' ') hour[0]=';';
		hour[2]=':';  hour[5]=':'; 
		drawChar( 4,0,hour[0], GRAPHICS_NORMAL);
		drawChar( 11,0,hour[1], GRAPHICS_NORMAL);
		//if(hour[2]==' ') hour[2]=';';
		drawChar( 18,0,hour[2], GRAPHICS_NORMAL); //else  drawChar( 13,0,' ', GRAPHICS_NORMAL);
		drawChar( 25,0,hour[3], GRAPHICS_NORMAL);
		drawChar( 32,0,hour[4], GRAPHICS_NORMAL);
		drawChar( 39,0,hour[5], GRAPHICS_NORMAL);
		drawChar( 46,0,hour[6], GRAPHICS_NORMAL);
		drawChar( 53,0,hour[7], GRAPHICS_NORMAL);	
		if(BIT.wkt_azan) break;	
		android ();
	} 
  };if(fg_ganti==2) rmt.arab=~rmt.arab; 
	clearScreen(1);
}		

void big_4digit(void) {
  selectFont(fixednums8x160); 
   if(hour[2]==' ') hour[2]=';';
   drawChar( 1,0,hour[0], GRAPHICS_NORMAL);
   drawChar( 13,0,hour[2], GRAPHICS_NORMAL);
   drawChar( 8,0,hour[1], GRAPHICS_NORMAL);
   drawChar( 18,0,hour[3], GRAPHICS_NORMAL);
   drawChar( 25,0,hour[4], GRAPHICS_NORMAL);
}


void fixjam (void) {
  if(!BIT.big_hour) { contr_bh = set.big_hr ;  BIT.big_hour=1;
   if(fg_ganti==0) rmt.arab=0;
   if(fg_ganti==1) rmt.arab=1;
   if(fg_ganti==2) rmt.arab=~rmt.arab; 
	jam();
   };
   
   if(hour[2]==' ') hour[2]=';';
   if(rmt.arab)  selectFont( arabic_60 ); else selectFont(fixednums8x160);   
   drawChar( 1,0,hour[0], GRAPHICS_NORMAL);
   drawChar( 14,0,hour[2], GRAPHICS_NORMAL);
   drawChar( 8,0,hour[1], GRAPHICS_NORMAL);
   drawChar( 18,0,hour[3], GRAPHICS_NORMAL);
   drawChar( 25,0,hour[4], GRAPHICS_NORMAL);
}  

void jam_bsr_kiri (void) {
	big_4digit();
}

void jam_mini_bold (void) {
	jam(); 
	if(set.plh_jam) { selectFont( font9x7 );
    drawChar( 6,0,hour[0], GRAPHICS_NORMAL);
	drawChar( 13,0,hour[1], GRAPHICS_NORMAL);
	      //if(hour[2]==' ') hour[2]=';';
	hour[2] = ':';
	drawChar( 19,0,hour[2], GRAPHICS_NORMAL); //else  drawChar( 13,0,' ', GRAPHICS_NORMAL);
	drawChar( 25,0,hour[3], GRAPHICS_NORMAL);
	drawChar( 32,0,hour[4], GRAPHICS_NORMAL);		
	hour[5]=':';
	drawChar( 38,0,hour[5], GRAPHICS_NORMAL);
	drawChar( 44,0,hour[6], GRAPHICS_NORMAL);
	drawChar( 51,0,hour[7], GRAPHICS_NORMAL);
	} else { mini_8x8(); }				 
}

void jdws_sow4 (void) {
	jam(); 
	switch (urut_sow) {
		case 0: sprintf( bfr.bufr1,"  IMSAK %02u:%02u   ",jd.pos[10],jd.pos[11]); break;
		case 1: sprintf( bfr.bufr1," SHUBUH %02u:%02u   ",jd.pos[0],jd.pos[1]); break;
		case 2: sprintf( bfr.bufr1," TERBIT %02u:%02u   ",jd.pos[12],jd.pos[13]); break;
		case 3: sprintf( bfr.bufr1,"  DHUHA %02u:%02u   ",jd.pos[14],jd.pos[15]); break;
		case 4: sprintf( bfr.bufr1," DZUHUR %02u:%02u   ",jd.pos[2],jd.pos[3]); break;
		case 5: sprintf( bfr.bufr1,"  ASHAR %02u:%02u   ",jd.pos[4],jd.pos[5]); break;
		case 6: sprintf( bfr.bufr1,"MAGHRIB %02u:%02u   ",jd.pos[6],jd.pos[7]); break;
		case 7: sprintf( bfr.bufr1,"  ISYA' %02u:%02u   ",jd.pos[8],jd.pos[9]); break;
		case 8: sprintf( bfr.bufr1,"%s,%02u-%02u-%02u   ",dt_day1[hari-1],tgl,bln,th); break;
	 };
	 if(set.fix_jam) { selectFont( SystemFont5x7 ); drawString( 40,0,bfr.bufr1,15, GRAPHICS_NORMAL); } else {
	 jam_dtk4(); selectFont( SystemFont5x7 ); drawString( 51,0,bfr.bufr1,15, GRAPHICS_NORMAL); };
}

void gbr (void) {
	drawString( 33,0,bfr.bufr1,6, GRAPHICS_NORMAL); drawString( 71,0,bfr.hour2,2,GRAPHICS_NORMAL);  
	drawString( 85,0,bfr.hour1,2, GRAPHICS_NORMAL);
	writePixel(83,2,GRAPHICS_NORMAL, 1); writePixel(83,4,GRAPHICS_NORMAL, 1);
	writePixel(83,1,GRAPHICS_NORMAL, 0); writePixel(83,3,GRAPHICS_NORMAL, 0);
	writePixel(83,5,GRAPHICS_NORMAL, 0);  writePixel(83,0,GRAPHICS_NORMAL, 0);
	writePixel(69,3,GRAPHICS_NORMAL, 0);   writePixel(83,6,GRAPHICS_NORMAL, 0);
	writePixel(83,7,GRAPHICS_NORMAL, 0);
 }

void jdws_sow3 (void) {
	jam();
	if(!set.fix_jam) jam_dtk3();
	selectFont( SystemFont5x7 );
	switch (urut_sow) {
		case 0: sprintf( bfr.bufr1," IMSAK ");sprintf( bfr.hour2,"%02u",jd.pos[10]);sprintf(bfr.hour1,"%02u ",jd.pos[11]); 
		        gbr(); break;
		case 1: sprintf( bfr.bufr1,"SHUBUH ");sprintf( bfr.hour2,"%02u",jd.pos[0]);sprintf(bfr.hour1,"%02u ",jd.pos[1]); 
		         gbr(); break;
		case 2: sprintf( bfr.bufr1,"TERBIT ");sprintf( bfr.hour2,"%02u",jd.pos[12]);sprintf(bfr.hour1,"%02u ",jd.pos[13]); 
		         gbr(); break;
		case 3: sprintf( bfr.bufr1," DHUHA ");sprintf( bfr.hour2,"%02u",jd.pos[14]);sprintf(bfr.hour1,"%02u ",jd.pos[15]); 
		         gbr(); break;
		case 4: sprintf( bfr.bufr1,"DZUHUR ");sprintf( bfr.hour2,"%02u",jd.pos[2]);sprintf(bfr.hour1,"%02u ",jd.pos[3]); 
		         gbr(); break;
		case 5: sprintf( bfr.bufr1," ASHAR ");sprintf( bfr.hour2,"%02u",jd.pos[4]);sprintf(bfr.hour1,"%02u ",jd.pos[5]); 
		         gbr(); break;
		case 6: sprintf( bfr.bufr1,"MAGHRB ");sprintf( bfr.hour2,"%02u",jd.pos[6]);sprintf(bfr.hour1,"%02u ",jd.pos[7]);
		         gbr(); break;
		case 7: sprintf( bfr.bufr1," ISYA' ");sprintf( bfr.hour2,"%02u",jd.pos[8]);sprintf(bfr.hour1,"%02u ",jd.pos[9]);
		         gbr();  break;
		case 8: sprintf( bfr.bufr1,"%02u-%02u-20%02u  ",tgl,bln,th); drawString( 35,0,bfr.bufr1,11, GRAPHICS_NORMAL);  break;
	 };
}

void jam_dtk4() {
	selectFont( angka7x8 );
    drawChar( 0,0,hour[0], GRAPHICS_NORMAL);
    hour[2] = ':';
    drawChar( 12,0,hour[2], GRAPHICS_NORMAL); //else  drawChar( 13,0,' ', GRAPHICS_NORMAL);
    drawChar( 6,0,hour[1], GRAPHICS_NORMAL);
    drawChar( 17,0,hour[3], GRAPHICS_NORMAL);
    hour[5]=':';
    drawChar( 29,0,hour[5], GRAPHICS_NORMAL); 
    drawChar( 23,0,hour[4], GRAPHICS_NORMAL);
    drawChar( 34,0,hour[6], GRAPHICS_NORMAL);
    drawChar( 40,0,hour[7], GRAPHICS_NORMAL);
}
	
void jam_dtk3(void)   {
    selectFont( angka7x8 );
    drawChar( 0,0,hour[0], GRAPHICS_NORMAL);
    if(hour[2]==' ') hour[2]=';';
    drawChar( 12,0,hour[2], GRAPHICS_NORMAL); 
    drawChar( 6,0,hour[1], GRAPHICS_NORMAL);
    drawChar( 17,0,hour[3], GRAPHICS_NORMAL);
    drawChar( 23,0,hour[4], GRAPHICS_NORMAL); 
}
	
void show_jds (void) {
	switch (urut_sow) {
ee:
	case 0: sprintf( bfr.bufr1,"  IMSAK %02u:%02u        ",jd.pos[10],jd.pos[11]); break;
    case 1: sprintf( bfr.bufr1," SHUBUH %02u:%02u        ",jd.pos[0],jd.pos[1]); break;
	case 2: sprintf( bfr.bufr1," TERBIT %02u:%02u        ",jd.pos[12],jd.pos[13]); break;
	case 3: sprintf( bfr.bufr1,"  DHUHA %02u:%02u        ",jd.pos[14],jd.pos[15]); break;
	case 4: sprintf( bfr.bufr1," DZUHUR %02u:%02u        ",jd.pos[2],jd.pos[3]); break;
	case 5: sprintf( bfr.bufr1,"  ASHAR %02u:%02u        ",jd.pos[4],jd.pos[5]); break;
	case 6: sprintf( bfr.bufr1,"MAGHRIB %02u:%02u        ",jd.pos[6],jd.pos[7]); break;
	case 7: sprintf( bfr.bufr1,"  ISYA' %02u:%02u        ",jd.pos[8],jd.pos[9]); break;
	case 8:  urut_sow=0; goto ee;
	};
};

void big_hour6(void) {
	parameter_big_jam ();
	while(BIT.big_hour) { jam();  show_jds();
	if(rmt.arab) {  arabic (6); } else { selectFont(fixednums8x160);  show_hour (6); }
	selectFont(System5x7); drawString( 64+6,-1,bfr.bufr1,20, GRAPHICS_NORMAL); 
	sprintf( bfr.bufr1,"%s, %02u-%02u-20%02u",dt_day[hari-1],tgl,bln,th);
	drawString( 64+6,8,bfr.bufr1,strlen(bfr.bufr1), GRAPHICS_NORMAL);

	if(BIT.wkt_azan) break;	
	android (); 
	} if(fg_ganti==2) rmt.arab=~rmt.arab;
	 clearScreen(1);
}	

void big_hour5(void) {
	parameter_big_jam ();
	while(BIT.big_hour) { jam(); show_jds();
	if(rmt.arab) {  arabic (0); } else { selectFont(fixednums8x160);  show_hour (0);};
	selectFont(System5x7);	 drawString( 58,-1,bfr.bufr1,18, GRAPHICS_NORMAL); 
	sprintf( bfr.bufr1,"%s, %02u-%02u-20%02u",dt_day1[hari-1],tgl,bln,th);
    drawString( 58,8,bfr.bufr1,strlen(bfr.bufr1), GRAPHICS_NORMAL);
	if(BIT.wkt_azan) break;	
	android (); 
	} if(fg_ganti==2) rmt.arab=~rmt.arab;
    clearScreen(1);
}	

void jam_mini_bold56 (void) {
	jam(); 
	if(set.plh_jam) { selectFont( font9x7 ); show_hour (6); } else { mini_8x8(); }				 
}

void show_jds2 (void) {
	switch (urut_sow) {
		case 0: sprintf( bfr.bufr1,"  IMSAK %02u:%02u         ",jd.pos[10],jd.pos[11]); break;
		case 1: sprintf( bfr.bufr1," SHUBUH %02u:%02u         ",jd.pos[0],jd.pos[1]); break;
		case 2: sprintf( bfr.bufr1," TERBIT %02u:%02u         ",jd.pos[12],jd.pos[13]); break;
		case 3: sprintf( bfr.bufr1,"  DHUHA %02u:%02u         ",jd.pos[14],jd.pos[15]); break;
		case 4: sprintf( bfr.bufr1," DZUHUR %02u:%02u         ",jd.pos[2],jd.pos[3]); break;
		case 5: sprintf( bfr.bufr1,"  ASHAR %02u:%02u         ",jd.pos[4],jd.pos[5]); break;
		case 6: sprintf( bfr.bufr1,"MAGHRIB %02u:%02u         ",jd.pos[6],jd.pos[7]); break;
		case 7: sprintf( bfr.bufr1,"  ISYA' %02u:%02u         ",jd.pos[8],jd.pos[9]); break;
		case 8: if(set.fix_jam)  sprintf( bfr.bufr1," %02u %03s 20%02u       ",tgl,dt_caldr2[bln-1],th); else 
			    sprintf( bfr.bufr1,"%s,%02u-%02u-20%02u       ",dt_day1[hari-1],tgl,bln,th);  break;
	 };
}

void show_jds22 (void) {
	switch (urut_sow) {
    case 0: sprintf( bfr.bufr1," IMSAK %02u:%02u  SHUBUH %02u:%02u    ",jd.pos[10],jd.pos[11],jd.pos[0],jd.pos[1]); break;
	case 1: sprintf( bfr.bufr1," TERBIT %02u:%02u  DHUHA %02u:%02u     ",jd.pos[12],jd.pos[13],jd.pos[14],jd.pos[15]); break;
	case 2: sprintf( bfr.bufr1," DZUHUR %02u:%02u  ASHAR %02u:%02u     ",jd.pos[2],jd.pos[3],jd.pos[4],jd.pos[5]); break;
	case 3: sprintf( bfr.bufr1,"MAGHRIB %02u:%02u  ISYA' %02u:%02u     ",jd.pos[6],jd.pos[7],jd.pos[8],jd.pos[9]); break;
	case 4: if(set.fix_jam)  sprintf( bfr.bufr1,"    %s, %02u-%02u-20%02u                   ",dt_day[hari-1],tgl,bln,th); else 
			sprintf( bfr.bufr1,"   %s, %02u-%02u-20%02u                     ",dt_day[hari-1],tgl,bln,th);  break;
	case 5: urut_sow=0; break;
	};
}

void jdws_sow6 (void) {
	jam(); show_jds2 ();
	if(set.fix_jam) { selectFont(System5x7); drawString( 96,-1,bfr.bufr1,20, GRAPHICS_NORMAL); 
	sprintf( bfr.hour2,"%s  ",dt_day[hari-1]); drawString( 42,-1,bfr.hour2,6, GRAPHICS_NORMAL); 	} else {
	jam_dtk6();
	selectFont(System5x7); drawString( 98,-1,bfr.bufr1,20, GRAPHICS_NORMAL); 
	};
}

void jdws_sow26 (void) {
	jam();  show_jds22 ();
	if(set.fix_jam) {  selectFont( SystemFont5x7 ); drawString( 35,0,bfr.bufr1,45, GRAPHICS_NORMAL); } else {
	 //sprintf( code,"%s  ",dt_day[hari-1]); drawString( 42,-1,code,6, GRAPHICS_NORMAL); 	} else {
	jam_dtk6();
	selectFont( SystemFont5x7 ); drawString( 35,0,bfr.bufr1,45, GRAPHICS_NORMAL); 
	};
}

void jdws_sow5 (void) {
	jam();  show_jds2 ();
	if(set.fix_jam) { selectFont(System5x7); drawString( 72,-1,bfr.bufr1,20, GRAPHICS_NORMAL); 
	sprintf( bfr.hour2,"%s  ",dt_day[hari-1]); selectFont( SystemFont5x7 ); drawString( 33,0,bfr.hour2,6, GRAPHICS_NORMAL); 	} else {
	jam_dtk5(); selectFont(System5x7); drawString( 58,-1,bfr.bufr1,20, GRAPHICS_NORMAL); 
	}
}

void jam_dtk6(void) {
	sprintf( bfr.hour2,"%s ",dt_day[hari-1]);
	selectFont( SystemFont5x7 );
	drawString( 0,0,bfr.hour2,6, GRAPHICS_NORMAL); 	
	selectFont( font9x7 );
    show_hour (40);
}

void jam_dtk26(void) { 	
	selectFont( font9x7 );
	drawChar( 0+0,0,hour[0], GRAPHICS_NORMAL);
	if(hour[2]==' ') hour[2]=';';
	drawChar( 13+0,0,hour[2], GRAPHICS_NORMAL);
	drawChar( 7+0,0,hour[1], GRAPHICS_NORMAL);
	drawChar( 19+0,0,hour[3], GRAPHICS_NORMAL);
	drawChar( 26+0,0,hour[4], GRAPHICS_NORMAL);
}	
	
void jam_dtk5(void) {
	selectFont( font9x7 ); show_hour (0);
}
	
void show_hour (uint8_t NO) {
	//selectFont( font9x7 );
    drawChar( 0+NO,0,hour[0], GRAPHICS_NORMAL);
	hour[2] = ':';
	drawChar( 13+NO,0,hour[2], GRAPHICS_NORMAL); //else  drawChar( 13,0,' ', GRAPHICS_NORMAL);
	drawChar( 7+NO,0,hour[1], GRAPHICS_NORMAL);
	drawChar( 19+NO,0,hour[3], GRAPHICS_NORMAL);
	hour[5]=':';
	drawChar( 32+NO,0,hour[5], GRAPHICS_NORMAL);
	drawChar( 26+NO,0,hour[4], GRAPHICS_NORMAL);
	drawChar( 38+NO,0,hour[6], GRAPHICS_NORMAL);
	drawChar( 46+NO,0,hour[7], GRAPHICS_NORMAL);		
}

void cleer (void) { selectFont(System5x7);	drawString( 33,-1,"                              ",30, GRAPHICS_NORMAL); }						
void call_jwds(void) {
	if(set.panel <3) { 
	if (trun + 3*set.speed < osal_GetSystemClock()) {	trun = osal_GetSystemClock();
	if(BIT.A) {
		//clear_bfr();
		sprintf( bfr.bufr,"%s, %02u %s 20%02u. IMSAK %02u:%02u SHUBUH %02u:%02u TERBIT %02u:%02u DHUHA %02u:%02u DZUHUR %02u:%02u ASHAR %02u:%02u MAGHRIB %02u:%02u ISYA' %02u:%02u   ",
		dt_day[hari-1],tgl,dt_caldr[bln-1],th,jd.pos[10],jd.pos[11],jd.pos[0],jd.pos[1],jd.pos[12],jd.pos[13],jd.pos[14],jd.pos[15],jd.pos[2],jd.pos[3],jd.pos[4],jd.pos[5],jd.pos[6],jd.pos[7],jd.pos[8],jd.pos[9]);
		selectFont(SystemFont5x7 );  len= strlen(bfr.bufr); drawMarquee(bfr.bufr,len,9); BIT.A=0;
	}
	if(!BIT.A) { selectFont(SystemFont5x7 );
		BIT.A=stepMarquees(-1,0); jam_mini_bold ();
		if(BIT.wkt_azan) return;
		android ();
	 };
    }
  } else {
	if (trun + 3*set.speed < osal_GetSystemClock()) {	trun = osal_GetSystemClock();
	if(BIT.A) {
		//clear_bfr(); 
		sprintf( bfr.bufr,"%s, %02u %s 20%02u. Imsak %02u:%02u Shubuh %02u:%02u Terbit %02u:%02u Dhuha %02u:%02u Dhuhur %02u:%02u Ashar %02u:%02u Maghrib %02u:%02u Isya' %02u:%02u   ",
		dt_day[hari-1],tgl,dt_caldr[bln-1],th,jd.pos[10],jd.pos[11],jd.pos[0],jd.pos[1],jd.pos[12],jd.pos[13],jd.pos[14],jd.pos[15],jd.pos[2],jd.pos[3],jd.pos[4],jd.pos[5],jd.pos[6],jd.pos[7],jd.pos[8],jd.pos[9]);
		selectFont(Droid_Sans_12); drawMarquee(bfr.bufr,131,1); BIT.A=0; selectFont(System5x7);	drawString( 33,-1,"                              ",30, GRAPHICS_NORMAL); 
	}
	if(!BIT.A){ selectFont(Droid_Sans_12); jam();
		if(set.fix_jam) BIT.A=stepMarqueePanel(-1,0); else BIT.A=stepMarquees(-1,0);
		if(set.fix_jam) fixjam(); 
		if(BIT.wkt_azan) return;
		android ();
	} 
  }
 }	
}

void hitung_jws(void) {
	//JDgrenwitc (5,5,2022); //
	JDgrenwitc (tgl,bln,(th+2000));
	WAKTU_DZUHUR(); WAKTU_ASHAR(); WAKTU_MAGRIB(); WAKTU_SLT_SUDUT (18.00,0);//WAKTU_ISAK (); 
	WAKTU_SLT_SUDUT (20.00,1);WAKTU_SLT_SUDUT (1.13,2); WAKTU_SLT_SUDUT( -3.30,3);
    
	if (cek.subuh) { wkt.subuh = wkt_fix.subuh; jd.pos[0] = wkt.subuh / 100; jd.pos[1] = wkt.subuh % 100; }
	else{ wkt.subuh = (jd.pos[0] * 100) + jd.pos[1]; };
	if (cek.duhur) { wkt.duhur = wkt_fix.duhur; jd.pos[2] = wkt.duhur / 100; jd.pos[3] = wkt.duhur % 100; }
	else { wkt.duhur = (jd.pos[2] * 100) + jd.pos[3]; };
	if (cek.ashar) { wkt.ashar = wkt_fix.ashar; jd.pos[4] = wkt.ashar / 100; jd.pos[5] = wkt.ashar % 100; }
	else { wkt.ashar = (jd.pos[4] * 100) + jd.pos[5]; }
	if (cek.magrib) { wkt.magrib = wkt_fix.magrib; jd.pos[6] = wkt.magrib / 100; jd.pos[7] = wkt.magrib % 100; }
	else { wkt.magrib = (jd.pos[6] * 100) + jd.pos[7]; };
	if (cek.isak) { wkt.isak = wkt_fix.isak; jd.pos[8] = wkt.isak / 100; jd.pos[9] = wkt.isak % 100; }
	else { wkt.isak = (jd.pos[8] * 100) + jd.pos[9]; };
	if (cek.jumat && hari == 6) { wkt.jumat = wkt_fix.jumat; jd.pos[2] = wkt.jumat / 100; jd.pos[3] = wkt.jumat % 100; }
	else { wkt.jumat = (jd.pos[2] * 100) + jd.pos[3]; };
	//if(hari==5 && cek.jumat )    wkt.duhur  = wkt.jumat;
	 //voice_alarm(); condown_jws();
}
void pilih_azan (void) {
	   if(!BIT.one) { BIT.one=1;
		BIT.wkt_azan=1; //if(set.adzan==1){   mp3_TF_F(4); Delay_run(800); mp3_play (suara); }  else { comando (0x0E); BIT.buzer=1; };	
		if(code_sht==1) { set.adzan=adzan.subuh; }
		else if(code_sht==2) { set.adzan=adzan.duhur; }
		else if(code_sht==3) { set.adzan=adzan.ashar; }
		else if(code_sht==4) { set.adzan=adzan.magrib; }
		else if(code_sht==5) { set.adzan=adzan.isak; }
		else { set.adzan=30; }
		iqm_contr_mnt = bfr.buf_timer_iqm[code_sht];
		iqomah_off(); conter_beep=0; BIT.buzer=1;
      //if(MP3) tim_IQM =bfr.buf_timer_iqm[code_sht];				
	 };
  }
uint8_t KLM;
void cek_jwds(void) {
	switch (KLM) {
	case 0 : if(wkt.total==wkt.subuh) { code_sht=1;  suara=1; pilih_azan ();};
		         //if(wkt.total==wkt_sblm.subuh ) { suara=set.vc_sbh;  preaudion (); fg.z=1;};
		         //if(ALRM.cek1 && wkt.total==wkt_ALRM.alrm1) { scan_hari(ALRM.day1,ALRM.file1); };
              KLM=1; break;
	 case 1 :  if(wkt.total==wkt.duhur) { code_sht=2;  suara=2; pilih_azan (); };
		          //if(	hari==5 ) { if(wkt.total==wkt_sblm.jumatan)  { suara=set.vc_jumatan; fg.z=1; preaudion ();}; } else {
		          //if(wkt.total==wkt_sblm.duhur)  { suara=set.vc_dhr; fg.z=1; preaudion ();}; };
				  //if(ALRM.cek2 && wkt.total==wkt_ALRM.alrm2) { scan_hari(ALRM.day2,ALRM.file2); };
              KLM=2; break;
	 case 2 : if(wkt.total==wkt.ashar) { code_sht=3;  suara=2; pilih_azan (); };
		         //if(wkt.total==wkt_sblm.ashar ) { suara=set.vc_asr;  preaudion (); fg.z=1;};
		         //if(ALRM.cek3 && wkt.total==wkt_ALRM.alrm3) {scan_hari(ALRM.day3,ALRM.file3); };
              KLM=3; break;
	 case 3 : if(wkt.total==wkt.magrib){ code_sht=4;  suara=2; pilih_azan (); };
		        // if(wkt.total==wkt_sblm.magrib) { if( set.cek_mlm_jum && hari==4 ) suara=set.vc_mlm_jum ;  else 
                //suara=set.vc_mgb;  preaudion (); fg.z=1; };
		         // if(ALRM.cek4 && wkt.total==wkt_ALRM.alrm4) { scan_hari(ALRM.day4,ALRM.file4); };
              KLM=4; break;
	 case 4 : if(wkt.total==wkt.isak)  { code_sht=5;  suara=2; pilih_azan (); };
		        // if(wkt.total==wkt_sblm.isak )  { suara=set.vc_isak; preaudion (); fg.z=1;};
		        //  if(ALRM.cek5 && wkt.total==wkt_ALRM.alrm5) { scan_hari(ALRM.day5,ALRM.file5); };
              //if(ALRM.cek6 && wkt.total==wkt_ALRM.alrm6) { scan_hari(ALRM.day6,ALRM.file6); };
              KLM=0; break; 
		};
	}
void msk_sholat43(void) {
	//selectFont( SystemFont5x7 );
	//if(hour[2]==':') {drawString(8,0,"ELTOP-INOVATIF ",15,GRAPHICS_NORMAL);} else {
		//drawString(8,0,"                  ",17,GRAPHICS_NORMAL);}
  //p = (uint8_t*)edt; read_eeset(); 
	//pesan_alrm = msg_alm;
	//clearScreen(1); //if(!delay_iqm)  tls_os_time_delay(500);
	//while(BIT.wkt_azan) {
  if (trun + (set.speed*4) < osal_GetSystemClock()) { trun = osal_GetSystemClock();
  if(BIT.A) {
  selectFont(System5x7);
	   if(hari==6 && code_sht ==2) { sprintf(bfr.bufr1,"    Jum'at    ");}  else 
       {sprintf(bfr.bufr1,"Waktu %s  ",	dt_sholat[code_sht-1]);};
	//if(set.panel==4) { if(hari==5 && code_sht ==2) { sprintf(bfr.bufr1,"    Jum'at    ");}  else 
       //{sprintf(bfr.bufr1,"Waktu %s  ",	dt_sholat[code_sht-1]);}; }  else 
	   //{ if(hari==5 && code_sht ==2) { sprintf(bfr.bufr1," Jum'at   ");}else{sprintf(bfr.bufr1,"%s  ",	dt_sholat[code_sht-1]);};	};
	//if(hari==5 && code_sht ==2) { sprintf(bfr.bufr," WAKTU JUM'AT TELAH TIBA......   "); len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8);  } else {
	//selectFont(bold_7x8);	
	sprintf(bfr.bufr,"WAKTU %s TELAH TIBA, MARILAH KITA SHOLAT %s BERJAMA'AH ",dt_sholat[code_sht-1],dt_sholat[ code_sht-1 ] ); 
	drawMarquee(bfr.bufr,strlen(bfr.bufr),8);
	//drawMarquee("INDONESIA RAYA MERDEKA YANG KE 77TH ",36,9);
	BIT.A=0; 
	}
   // while(!BIT.A){	
  if(!BIT.A) {
	selectFont(System5x7); BIT.A=stepMarquees(-1,0);	//jam();
    //if(!set.fix_jam) BIT.A=stepMarquees(-1,0); else BIT.A=stepMarqueePanel(-1,0); jam(); 
	//if(set.fix_jam) fixjam(); 
	//if(set.panel==4) {
		//if(!set.fix_jam) 
	    jam_dtk4();
		selectFont( SystemFont5x7 );
		if(BIT.dot) { 
		drawString( 51,0,bfr.bufr1,13, GRAPHICS_NORMAL );// else  drawString( 41,0,bfr.bufr1,13, GRAPHICS_NORMAL );
		} else {	 
		drawString( 51,0,"             ",13, GRAPHICS_NORMAL );} //else drawString( 41,0,"             ",13, GRAPHICS_NORMAL ); };
	//} else {
		//if(!set.fix_jam)jam_dtk3();
		//selectFont(bold_7x8);
		//if(BIT.dot) {  drawString( 38,-1,bfr.bufr1,8, GRAPHICS_NORMAL );
		//} else {	 drawString( 33,-1,"             ",9, GRAPHICS_NORMAL ); };};					
		//if(BIT.iqomah) {iqm_contr_dtk =0;  iqm_off_mnt = bfr.buf_timer_iqm[0]; 
		//iqm_contr_mnt =bfr.buf_timer_iqm[code_sht]; clearScreen(1); goto IQMH; };
		//Delay(3*set.speed); android ();
	}
  }; //clearScreen(1);
		 
	  /* if(!set.fix_jam) jam_bsr_kiri ();
		 selectFont(Arial_Black_16);
		 if(hari==5 && code_sht ==2) { sprintf(bfr.bufr1," Waktu Jum'at");} else {
		 sprintf(bfr.bufr1,"Waktu %s",	dt_sholat[code_sht-1]);};
		 len= strlen(bfr.bufr1); drawMarquee(bfr.bufr1,len,0);
		 BIT.A=0; 
			while(!BIT.A){ jam(); 
			selectFont(Arial_Black_16); 
			if(!set.fix_jam) BIT.A=stepMarquee(1); else BIT.A=stepMarqueePanel(-1,0);
           // Delay(3*set.speed);
			if(set.fix_jam) fixjam(); android ();
			if(BIT.iqomah) {iqm_contr_dtk =0;  iqm_off_mnt = bfr.buf_timer_iqm[0]; 
			 iqm_contr_mnt =bfr.buf_timer_iqm[code_sht]; clearScreen(1); goto IQMH; };
		    };				
		};
		IQMH:NULL; //p = (uint8_t*)edt; read_eeprog();*/
}

void msk_sholat2(void) {
	//p = (uint8_t*)edt; read_eeset();
	//pesan_alrm = msg_alm;
	 clearScreen(1);//if(!delay_iqm)  tls_os_time_delay(500);
	while(BIT.wkt_azan) {
	selectFont(System5x7);
	if(hari==6 && code_sht ==2) {	sprintf(bfr.bufr,"WAKTU JUM'AT TELAH TIBA, KHUTBAH JUM'AT AKAN DIMULAI......"); 
	  len= strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8);  } else {
	selectFont(System5x7);
	sprintf(bfr.bufr,"WAKTU %s TELAH TIBA, MARILAH KITA SHOLAT %s BERJAMA'AH",dt_sholat[code_sht-1],dt_sholat[ code_sht-1 ] ); 
		        len= strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8); 
	};clearScreen(1); 
	BIT.A=0; 
     while(!BIT.A){	 selectFont(System5x7);
       BIT.A=stepMarquees(-1,0); jam();
		  if(BIT.dot) {  jam();  mini_8x8();
			   } else { drawString( 0,0,"           ",11, GRAPHICS_NORMAL ); };
		  if(BIT.iqomah) {iqm_contr_dtk =0;  iqm_off_mnt = bfr.buf_timer_iqm[0]; 
			   iqm_contr_mnt =bfr.buf_timer_iqm[code_sht]; clearScreen(1); goto IQMH; };
		 }; clearScreen(1);  
	if(hari==6 && code_sht ==2) { sprintf(bfr.bufr1," Waktu Jum'at    ");} else {
	sprintf(bfr.bufr1,"Waktu %s  ",	dt_sholat[code_sht-1]);};	 
	selectFont(Droid_Sans_12); len= strlen(bfr.bufr1); drawMarquee(bfr.bufr1,len,2);
	BIT.A=0; 
	while(!BIT.A){ BIT.A=stepMarquees(-1,0); android ();
    //Delay(3*set.speed); android();
	};
	};
	IQMH: NULL; //uint8_t*)edt; read_eeprog();
}

uint8_t lp,JUMPLH,baris;
void iqm_mn(void) {
	 if(BIT.dot) {drawString( baris,-1,"IQOMAH     ",11, GRAPHICS_NORMAL ); } else {
		drawString( baris,-1,"           ",11, GRAPHICS_NORMAL ); };
}

void BIG_IQM43 (void) {
	proses_data( (iqm_contr_mnt/10)+0x30,(iqm_contr_mnt%10)+0x30,':',(iqm_contr_dtk/10)+0x30,
	( iqm_contr_dtk%10)+0x30 ); bfr.hour1[5]=' ';
	if(iqm_contr_mnt/10==0) bfr.hour1[0]='<'; 
	/*if(set.panel==3) { 
	selectFont(angka8x16); drawString( 52,0,bfr.hour1,6, GRAPHICS_NORMAL);
	selectFont(Droid_Sans_12);
	if(BIT.dot) { drawString( 0,1,"IQOMAH",6, GRAPHICS_NORMAL ); } else {
		drawString( 0,1,"          ",8, GRAPHICS_NORMAL ); };
	  } else { */
	selectFont(fixednums8x160);drawString( 82,0,bfr.hour1,6, GRAPHICS_NORMAL );
	selectFont(Droid_Sans_12);
	if(BIT.dot) {
		drawString( 8,1,"IQOMAH",6, GRAPHICS_NORMAL );  } else {
		drawString( 8,1,"        ",7, GRAPHICS_NORMAL ); };
	//};		 
   android();		

}

void IQOMAH43(void) {
	//p = (uint8_t*)edt; read_eeset(); 
	//pesan_iqm = msg_iqm;
	/*selectFont(bold_7x8);	
	if(set.iqm==0 && fg_man_iqm==0 ) { BIT.iqomah=0; }; clearScreen(1);
	if(iqm_contr_mnt==0) {  BIT.msg_iqm=0; iqm_off_dtk=0; iqm_off_mnt = bfr.buf_timer_iqm[0]; BIT.iqm_off=1; goto jumper1; };
	while(BIT.iqomah) {
	selectFont(bold_7x8);	
	if(set.IQM_disp) {
	sprintf(bfr.bufr,"PERHATIKAN TIMER IQOMAH SEBELUM SHOLAT SUNNAH          SHAF DEPAN YANG KOSONG MOHON SEGERA DITEMPATI"); 	
           len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8); 
	BIT.A=0; 
     while(!BIT.A){	 selectFont(bold_7x8);	
		if(set.panel==3) { 		 BIT.A=stepMarquees(-1,0); } else {
        if(!set.fix_jam) {BIT.A=stepMarquees(-1,0);  } else { BIT.A=stepMarqueePanel(-1,0); fixjam(); };
		};
		proses_data( (iqm_contr_mnt/10)+0x30,(iqm_contr_mnt%10)+0x30,':',(iqm_contr_dtk/10)+0x30,
		 ( iqm_contr_dtk%10)+0x30 ); bfr.hour1[5]=' ';
		if(iqm_contr_mnt/10==0) bfr.hour1[0]='-'; selectFont(bold_8x8); 
		if(set.panel==3) { drawString( 1,0,bfr.hour1,6, GRAPHICS_NORMAL );
		selectFont(bold_7x8);	
	    if(BIT.dot) { drawString( 45,-1,"IQOMAH",6, GRAPHICS_NORMAL ); } else {
		    drawString( 45,-1,"        ",8, GRAPHICS_NORMAL ); };
	  } else {
		if(set.fix_jam) { drawString(31+ 64,0,bfr.hour1,6, GRAPHICS_NORMAL );
		  selectFont(bold_7x8);	
	    if(BIT.dot) { drawString(31+ 9,-1,"IQOMAH",6, GRAPHICS_NORMAL ); 
		     } else { drawString(31+ 9,-1,"        ",8, GRAPHICS_NORMAL ); };
		} else {
		drawString( 17,0,bfr.hour1,6, GRAPHICS_NORMAL );
		selectFont(bold_7x8);	
	    if(BIT.dot) { drawString( 58,-1,"IQOMAH",6, GRAPHICS_NORMAL ); } else {
		    drawString( 58,-1,"        ",8, GRAPHICS_NORMAL ); };
		};
	};					
	//Delay(3*set.speed); android();
	if(!BIT.iqomah) goto jumpr; 
	}
jumpr:
	selectFont(bold_7x8); 
	} else { BIG_IQM43 (); };*/
	if(!BIT.iqm_off) { BIG_IQM43 (); } else {
//};
/*	clearScreen(1); 
	if(set.panel==3) baris=26; else baris=38; 
	while(BIT.msg_iqm) { selectFont(bold_7x8);
	for(lp=0; lp<2; lp++) {  
	if(set.panel==3) {
	for(u8 j=0;j<8;j++) { drawString( 1,15-j," LURUSKAN SHAF ",15, GRAPHICS_NORMAL );//tls_os_time_delay(30); 
	iqm_mn(); }; 
	for(u8 j=0;j<17;j++) { iqm_mn();} //tls_os_time_delay(60); };
    for(u8 j=0;j<8;j++) {drawString( 0,9,"                                  ",32, GRAPHICS_NORMAL );
      drawString( 1,10+j," LURUSKAN SHAF ",15, GRAPHICS_NORMAL ); //tls_os_time_delay(20);
	iqm_mn();};	 
	drawString( 0,9,"                                  ",32, GRAPHICS_NORMAL );
	for(u8 j=0;j<8;j++) { drawString( 1,15-j," RAPATKAN SHAF ",15, GRAPHICS_NORMAL ); //tls_os_time_delay(30);
	iqm_mn(); }; 
	for(u8 j=0;j<17;j++) {iqm_mn();}	//tls_os_time_delay(60);};
    for(u8 j=0;j<8;j++) {drawString( 0,9,"                                  ",32, GRAPHICS_NORMAL );
       drawString( 1,10+j," RAPATKAN SHAF ",15, GRAPHICS_NORMAL ); //''tls_os_time_delay(20);
	iqm_mn(); }; 		 
	drawString( 2,9,"                                  ",32, GRAPHICS_NORMAL); 
	} else {
	for(u8 j=0;j<8;j++) {  drawString( 14,15-j," LURUSKAN SHAF ",15, GRAPHICS_NORMAL );//tls_os_time_delay(30); 
	iqm_mn(); }; 
	for(u8 j=0;j<17;j++) { iqm_mn();} // tls_os_time_delay(60); };
       for(u8 j=0;j<8;j++) { drawString( 2,9,"                                  ",32, GRAPHICS_NORMAL );
       drawString( 14,9+j," LURUSKAN SHAF ",15, GRAPHICS_NORMAL ); //tls_os_time_delay(20);
	   iqm_mn();};	 
	drawString( 2,9,"                                  ",32, GRAPHICS_NORMAL );
	for(u8 j=0;j<8;j++) {
    drawString( 14,15-j," RAPATKAN SHAF ",15, GRAPHICS_NORMAL ); //tls_os_time_delay(30);
	iqm_mn(); }; 
	for(u8 j=0;j<17;j++) {iqm_mn(); } //tls_os_time_delay(60);};
    for(u8 j=0;j<8;j++) {  drawString( 2,9,"                                  ",32, GRAPHICS_NORMAL );
      drawString( 14,9+j," RAPATKAN SHAF ",15, GRAPHICS_NORMAL );//tls_os_time_delay(20);
	iqm_mn(); }; 		 
	drawString( 2,9,"                                  ",32, GRAPHICS_NORMAL );
	}
 };  android();
	 BIT.msg_iqm=0; iqm_off_dtk=0; iqm_off_mnt = bfr.buf_timer_iqm[0]; BIT.iqm_off=1;
};
	jumper1: clearScreen(1); //iqomah_off(); //RELAY_ON;// comando('Y');
*/	
	if(BIT.iqm_off) { jam(); set.iqm_dot=1; set.fix_jam=1;
		if(!set.iqm_dot) { clearScreen(1); } else { //tls_os_time_delay(50); } else {
		if(set.fix_jam) {  fixjam(); } else {
		if(BIT.dot) {
		drawString( baris,0,"     :     ",11, GRAPHICS_NORMAL);} else {
		drawString( baris,0,"           ",11, GRAPHICS_NORMAL ); };
	        };
	      }; android();
		}; //clearScreen(1); fg_man_iqm=0;
	}
	
}

void BIG_IQM2(void) {
	proses_data( (iqm_contr_mnt/10)+0x30,(iqm_contr_mnt%10)+0x30,':',(iqm_contr_dtk/10)+0x30,
	( iqm_contr_dtk%10)+0x30 ); bfr.hour1[5]=' ';
	if(iqm_contr_mnt/10==0) bfr.hour1[0]='-';  
	selectFont(fixednums8x160); //selectFont(Arial_Black_16); 
	drawString( 35,1,bfr.hour1,6, GRAPHICS_NORMAL );	
	if(BIT.dot) { drawString( 0,1,"IQM  ",4, GRAPHICS_NORMAL); 
	 } else { drawString( 0,1,"     ",4, GRAPHICS_NORMAL ); };
android();
}

void IQOMAH2 (void) { //iqm_contr_mnt =bfr.buf_timer_iqm[5];
	//p = (uint8_t*)edt; read_eeset(); 
	//if(set.warnaPnl) COL_CONT_IQM=2; else COL_CONT_IQM=1; 
	//pesan_iqm = msg_iqm;
	if(set.iqm==0 && fg_man_iqm==0 ) { BIT.iqomah=0;}; clearScreen(1);
	if(iqm_contr_mnt==0) {  BIT.msg_iqm=0; iqm_off_dtk=0; iqm_off_mnt = bfr.buf_timer_iqm[0]; BIT.iqm_off=1; goto jumper2; };
	while(BIT.iqomah) { 
	if(set.IQM_disp) {	
		selectFont(System5x7); clearScreen(1); 
		sprintf(bfr.bufr,"PERHATIKAN TIMER IQOMAH SEBELUM SHOLAT SUNNAH          SHAF DEPAN YANG KOSONG MOHON SEGERA DITEMPATI"); 	
           len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8); 
	BIT.A=0; //if(set.warnaPnl) COL_CONT_IQM=2; else COL_CONT_IQM=1; clearScreen(1);
    while(!BIT.A){	 selectFont(System5x7); //if(!set.warnaPnl) COL_IQM=1;
       BIT.A=stepMarquees(-1,0); 
		proses_data( (iqm_contr_mnt/10)+0x30,(iqm_contr_mnt%10)+0x30,':',(iqm_contr_dtk/10)+0x30,
		    ( iqm_contr_dtk%10)+0x30 ); bfr.hour1[5]=' ';
		if(iqm_contr_mnt/10==0) bfr.hour1[0]='-'; drawString( 36,-1,bfr.hour1,6, GRAPHICS_NORMAL );
		if(BIT.dot) { selectFont( SystemFont5x7 );
		drawString( -1,0,"IQOMAH",6, GRAPHICS_NORMAL ); } else {
		drawString( -1,0,"      ",6, GRAPHICS_NORMAL ); };
		 //drawString( 36,0,hour,5, GRAPHICS_NORMAL );
		//Delay(3*set.speed); android();
		if(!BIT.iqomah) goto jumpr; 
	  }
		jumpr:
		  clearScreen(1); 
		} else { BIG_IQM2 (); };	
	 };
	 
  while(BIT.msg_iqm) {
	selectFont(System5x7);
	sprintf(bfr.bufr,"LURUS DAN RAPATKAN SHAF UNTUK SEMPURNANYA SHOLAT BERJAMA'AH"); 	
       len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8); 
	BIT.A=0; 
    while(!BIT.A){	 BIT.A=stepMarquees(-1,0);  android ();
	if(BIT.dot) { selectFont(System5x7);
		drawString( 3,-1," IQOMAH  ",10, GRAPHICS_NORMAL ); } else {
		drawString( 3,-1,"          ",10, GRAPHICS_NORMAL ); }; 
		//Delay(3*set.speed); android();
	};
	BIT.msg_iqm=0; iqm_off_dtk=0; iqm_off_mnt = bfr.buf_timer_iqm[0]; BIT.iqm_off=1;
	};
	jumper2: clearScreen(1); //iqomah_off();  //RELAY_ON; 
	while(BIT.iqm_off) {
		jam();
		if(!set.iqm_dot) { clearScreen(1); } else {//tls_os_time_delay(50); } else {
		if(BIT.dot) { drawChar( 31,0,':', GRAPHICS_NORMAL);} else {
		 drawString( 1,0,"          ",10, GRAPHICS_NORMAL ); };
	    };
		if( BIT.dot ) { android();
   		}				
	};clearScreen(1); fg_man_iqm=0;
}

void JUMATAN (void) { 
	//p = (uint8_t*)edt;  read_eeset();	 
	while(BIT.jumat) {
	selectFont(Droid_Sans_12);
	BIT.A=0;  sprintf(bfr.bufr,"MOHON DIAM SAAT KHUTBAH"); 	
    len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,1); 
	while(!BIT.A){	selectFont(Droid_Sans_12); jam();
	if(set.panel==2)  { BIT.A=stepMarquees(-1,0); } else {
	if(set.fix_jam) { BIT.A=stepMarqueePanel(-1,0); fixjam(); } else {BIT.A=stepMarquees(-1,0);}
	};
	//Delay(3*set.speed);  android();
	}; selectFont(System5x7);
		BIT.A=0;  sprintf(bfr.bufr,"MOHON HP. DIMATIKAN SAAT KHUTBAH"); 	
        len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8); 
		while(!BIT.A){	 
	    selectFont(System5x7);	
	    if(set.panel==2)  { BIT.A=stepMarquees(-1,0); } else {
	    if(set.fix_jam) BIT.A=stepMarqueePanel(-1,0); else BIT.A=stepMarquees(-1,0);
		};
		switch (set.panel) {
			case 4: jdws_sow4(); if(set.fix_jam) fixjam(); break;
			case 3: jdws_sow3(); if(set.fix_jam) fixjam(); break;
			case 2: jam_mini_bold (); break;
		};
		 //Delay(3*set.speed); 
		android();
	}; 	
 }
}

 // seting untuk 5&6 panel
void call_jwds56(void) {
	sprintf( bfr.bufr,"%s, %02u %s 20%02u. Imsak %02u:%02u Shubuh %02u:%02u Terbit %02u:%02u Dhuha %02u:%02u Dhuhur %02u:%02u Ashar %02u:%02u Maghrib %02u:%02u Isya' %02u:%02u   ",
	dt_day[hari-1],tgl,dt_caldr[bln-1],th,jd.pos[10],jd.pos[11],jd.pos[0],jd.pos[1],jd.pos[12],jd.pos[13],jd.pos[14],jd.pos[15],jd.pos[2],jd.pos[3],jd.pos[4],jd.pos[5],jd.pos[6],jd.pos[7],jd.pos[8],jd.pos[9]);
	selectFont(Droid_Sans_12); len= strlen(bfr.bufr); drawMarquee(bfr.bufr,len,1); BIT.A=0; selectFont(System5x7);	drawString( 33,-1,"                              ",30, GRAPHICS_NORMAL); 
	while(!BIT.A){ selectFont(Droid_Sans_12); jam();
	     if(set.fix_jam) BIT.A=stepMarqueePanel(-1,0); else BIT.A=stepMarquees(-1,0); 
		    //Delay(3*set.speed); 
		fixjam();
	    if(BIT.wkt_azan) break;
		android ();
	}; 
}
 
void msk_sholat65(void) {
    clearScreen(1); //if(!delay_iqm)  tls_os_time_delay(500);
	while(BIT.wkt_azan) {
	if(hari==6 && code_sht ==2) { sprintf(bfr.bufr1,"   JUM'ATAN      ");
	sprintf(bfr.bufr,"WAKTU JUM'ATAN TELAH TIBA.....   "); selectFont(System5x7); len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8);} else {
	sprintf(bfr.bufr1,"WAKTU %s      ",	dt_sholat[code_sht-1]);
	selectFont(System5x7); 
	sprintf(bfr.bufr,"WAKTU %s TELAH TIBA, MARILAH KITA SHOLAT %s BERJAMA'AH",dt_sholat[code_sht-1],dt_sholat[code_sht-1] ); 
		    len= strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8);   break;
	BIT.A=0; 
    while(!BIT.A){	 
	jam(); selectFont(System5x7);	
    if(set.fix_jam) BIT.A=stepMarqueePanel(-1,0); else BIT.A=stepMarquees(-1,0);  
		if(set.panel==6) {
		if(!set.fix_jam)  {jam_dtk6(); } else { sprintf( bfr.hour2,"%s  ",dt_day[hari-1]); 
		selectFont(System5x7); drawString( 42,-1,bfr.hour2,6, GRAPHICS_NORMAL); fixjam(); };
		selectFont(System5x7);
		if(BIT.dot) { 
		 drawString( 96,-1,bfr.bufr1,13, GRAPHICS_NORMAL );} else {	 
		 drawString( 96 ,-1,"                     ",20, GRAPHICS_NORMAL ); };
				} else {
		if(!set.fix_jam)  {jam_dtk5(); } else {  fixjam(); };
		selectFont(System5x7);
		  if(BIT.dot) { 
		  drawString( 64,-1,bfr.bufr1,13, GRAPHICS_NORMAL );
		} else {	 drawString( 64,-1,"                       ",20, GRAPHICS_NORMAL ); };
		};					
		if(BIT.iqomah) {iqm_contr_dtk =0;  iqm_off_mnt = bfr.buf_timer_iqm[0]; 
			 iqm_contr_mnt =bfr.buf_timer_iqm[code_sht]; clearScreen(1); goto IQMH; };
			// Delay(3*set.speed);  android ();					
	}; clearScreen(1);
	if(!set.fix_jam) jam_bsr_kiri ();
	selectFont(Droid_Sans_12);
	if(hari==6 && code_sht ==2) { sprintf(bfr.bufr1,"WAKTU JUM'AT  "); } else {
		sprintf(bfr.bufr1,"Waktu %s  ",	dt_sholat[code_sht-1]);	}			 
	len = strlen(bfr.bufr1); drawMarquee(bfr.bufr1,len,1);
	BIT.A=0; 
	while(!BIT.A){
	jam(); selectFont(Droid_Sans_12);
	if(!set.fix_jam) BIT.A=stepMarquee(1); else BIT.A=stepMarqueePanel(-1,0);
    //Delay(3*set.speed);  
	if(set.fix_jam) fixjam(); android ();
	if(BIT.iqomah) {iqm_contr_dtk =0;  iqm_off_mnt = bfr.buf_timer_iqm[0]; 
	iqm_contr_mnt =bfr.buf_timer_iqm[code_sht]; clearScreen(1); goto IQMH; };
	 };				
	};
 IQMH:NULL; // p = (uint8_t*)edt; read_eeprog();
 }
}

void BIG_IQM56 (void) {
	contr_bh=set.big_hr; BIT.big_hour=1;
	if(fg_ganti==0) rmt.arab=0;
	if(fg_ganti==1) rmt.arab=1;
while(BIT.big_hour) { jam();
	if(set.panel==6) { 	
	if(rmt.arab) {  arabic (6); } else { selectFont(fixednums8x160);  show_hour (6); }; } else { 
	if(rmt.arab) {  arabic (0); } else { selectFont(fixednums8x160);  show_hour (0);};	};
	
	proses_data( (iqm_contr_mnt/10)+0x30,(iqm_contr_mnt%10)+0x30,':',(iqm_contr_dtk/10)+0x30,
		( iqm_contr_dtk%10)+0x30 ); bfr.hour1[5]=' ';
	if(iqm_contr_mnt/10==0) bfr.hour1[0]='<';  
	if(set.panel==5) { 
	selectFont(fixednums8x160); //if(bfr.hour1[0]=='-') bfr.hour1[0]='<';
	drawString( 51+64,0,bfr.hour1,6, GRAPHICS_NORMAL );
	selectFont(Droid_Sans_12);
	if(BIT.dot) { drawString( 0+64,1,"IQOMAH",6, GRAPHICS_NORMAL ); } else {
	drawString( 0+64,1,"         ",8, GRAPHICS_NORMAL ); };
	 } else { selectFont(fixednums8x160);
	drawString( 83+64,0,bfr.hour1,6, GRAPHICS_NORMAL );
	selectFont(Droid_Sans_12);;
	if(BIT.dot) { drawString( 8+64,1,"IQOMAH",6, GRAPHICS_NORMAL );  } else {
	drawString( 8+64,0,"        ",7, GRAPHICS_NORMAL ); };
	};		android ();

 }
if(fg_ganti==2) rmt.arab=~rmt.arab; clearScreen(1);
			
}

void IQOMAH65(void) {
	selectFont(System5x7);
	if(set.iqm==0 && fg_man_iqm==0 ) { BIT.iqomah=0; }; clearScreen(1);
	if(iqm_contr_mnt==0) {  BIT.msg_iqm=0; iqm_off_dtk=0; iqm_off_mnt = bfr.buf_timer_iqm[0]; BIT.iqm_off=1; goto jumper1; };
	while(BIT.iqomah) {
	if(set.IQM_disp) { selectFont(System5x7);	
	 sprintf(bfr.bufr,"PERHATIKAN TIMER IQOMAH SEBELUM SHOLAT SUNNAH          SHAF DEPAN YANG KOSONG MOHON SEGERA DITEMPATI"); 	
           len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8); 
	BIT.A=0; 
    while(!BIT.A){	 selectFont(System5x7);
    if(!set.fix_jam) BIT.A=stepMarquees(-1,0); else BIT.A=stepMarqueePanel(-1,0);  jam();
	if(set.panel==5) { if(!set.fix_jam) jam_dtk5(); else fixjam(); } else { 
	if(!set.fix_jam) jam_dtk6();  else fixjam(); };
	proses_data( (iqm_contr_mnt/10)+0x30,(iqm_contr_mnt%10)+0x30,':',(iqm_contr_dtk/10)+0x30,
		    ( iqm_contr_dtk%10)+0x30 ); bfr.hour1[5]=' ';
	if(iqm_contr_mnt/10==0) bfr.hour1[0]='-'; selectFont(System5x7);
	if(set.panel==5) { 
	if(set.fix_jam) {drawString( 131-12,0,bfr.hour1,6, GRAPHICS_NORMAL );
		selectFont(System5x7);
	    if(BIT.dot) {drawString( 75-15,-1,"IQOMAH",6, GRAPHICS_NORMAL ); } else {
		drawString( 75-15,-1,"        ",7, GRAPHICS_NORMAL ); }; }  else {
		drawString( 131,0,bfr.hour1,6, GRAPHICS_NORMAL );
		selectFont(System5x7);	
	    if(BIT.dot) { drawString( 75,-1,"IQOMAH",6, GRAPHICS_NORMAL ); 
		} else { drawString( 75,-1,"        ",7, GRAPHICS_NORMAL ); };
	    }
	    } else {
			if(set.fix_jam) { drawString( 163-11,0,bfr.hour1,6, GRAPHICS_NORMAL );
		    selectFont(System5x7);	
	      if(BIT.dot) { drawString( 107-11,-1,"IQOMAH",6, GRAPHICS_NORMAL ); 
		    } else { drawString( 107-11,-1,"       ",7, GRAPHICS_NORMAL ); };
		sprintf( bfr.hour2,"%s  ",dt_day[hari-1]); drawString( 42,-1,bfr.hour2,6, GRAPHICS_NORMAL);
		   } else { drawString( 163,0,bfr.hour1,6, GRAPHICS_NORMAL );
		    selectFont(System5x7);	
	      if(BIT.dot) { drawString( 107,-1,"IQOMAH",6, GRAPHICS_NORMAL ); 
		    } else { drawString( 107,-1,"       ",7, GRAPHICS_NORMAL ); };
			};	
		}			 
		// Delay(3*set.speed);  android ();
		 if(!BIT.iqomah) goto jumpr; 
	  }
		jumpr:
		  selectFont(System5x7);	//clearScreen(1); 
		} else { BIG_IQM56 (); };
	 };
	 clearScreen(1);
	 if(set.panel==5) baris=75; else baris=107; 
   while(BIT.msg_iqm) {
		 for(u8 j=0; j<1; j++) { selectFont(System5x7);	
			 drawMarquee("LURUS DAN RAPATKAN SHAF UNTUK SEMPURNANYA SHOLAT BERJAMA'AH        ",61,8);
			 BIT.A=0; 
       while(!BIT.A){	 
       if(!set.fix_jam) BIT.A=stepMarquees(-1,0); else BIT.A=stepMarqueePanel(-1,0 );  jam();
			 if(set.panel==5) { if(!set.fix_jam) jam_dtk5(); else fixjam(); } else { if(!set.fix_jam) {jam_dtk6(); } else {
			    sprintf( bfr.hour2,"%s  ",dt_day[hari-1]); selectFont(System5x7); drawString( 42,-1,bfr.hour2,6, GRAPHICS_NORMAL); fixjam();}; };
				selectFont(System5x7);	 
			 if(BIT.dot) {
					drawString( baris,-1,"IQOMAH     ",11, GRAPHICS_NORMAL ); } else {
					drawString( baris,-1,"           ",11, GRAPHICS_NORMAL ); }; //Delay(3*set.speed);android ();
			    };
		     }; BIT.msg_iqm=0; iqm_off_dtk=0; iqm_off_mnt = bfr.buf_timer_iqm[0]; BIT.iqm_off=1;
	     };
 jumper1: clearScreen(1); //iqomah_off();  RELAY_ON;
	while(BIT.iqm_off) {
		jam();
		if(!set.iqm_dot) { clearScreen(1); } else {//tls_os_time_delay(50); } else {
		if(set.fix_jam) {  fixjam(); } else {
		if(BIT.dot) {
		drawString( baris,0,"     :     ",11, GRAPHICS_NORMAL );} else {
		drawString( baris,0,"           ",11, GRAPHICS_NORMAL ); };
	              };  
		};  android ();
		};clearScreen(1); //p = (uint8_t*)edt; read_eeprog(); 
	}

void JUMATAN56 (void) { 
	
	if(set.msg_jum) {
	selectFont(Droid_Sans_12);
	BIT.A=0;  sprintf(bfr.bufr,"MOHON DIAM SAAT KHUTBAH"); 	
    len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,1); 
	while(!BIT.A){	 selectFont(Droid_Sans_12); jam();
	if(set.fix_jam) {BIT.A=stepMarqueePanel(-1,0); fixjam(); } else {BIT.A=stepMarquees(-1,0);}
	//Delay(3*set.speed);   android ();
								
							}; selectFont(System5x7);
		BIT.A=0;  sprintf(bfr.bufr,"MOHON HP. DIMATIKAN SAAT KHUTBAH"); 	
        len = strlen(bfr.bufr); drawMarquee(bfr.bufr,len,8); 
		while(!BIT.A){	            
		switch (set.panel) {
			case 6: if(!set.jwsDisply2) jdws_sow6(); else jdws_sow26(); break;
			case 5: jdws_sow5(); break;
		};
		selectFont(System5x7);
		if(set.fix_jam) {BIT.A=stepMarqueePanel(-1,0); fixjam(); } else {BIT.A=stepMarquees(-1,0);}
					    //elay(3*set.speed); 
							 android ();
		}; 
	};  //RELAY_OF; //p = (uint8_t*)edt; read_eeprog();
 }	

 uint16_t time_set(uint8_t input0,uint8_t input1,uint8_t input2) {
   uint8_t red; 
   uint16_t input;
   //red = (120+input2) - dt_wkt[input0] ;
   red = (120+input2) - input0 ;
   input=(((input1 - 2) + (red/60))*100)+(red%60); 
   return(input);
   }
 
 
uint32_t jumlahHariDariTanggal(uint8_t tanggal, uint8_t bulan, uint16_t tahun) {
	//tanggal=22; bulan=9; tahun=18;
uint16_t j;
 uint32_t jumlahHari = (uint32_t)tahun * 365;
  for (j = 0; j < tahun; j++)  {
    if (!(j%4)){ jumlahHari++;}
  }
  jumlahHari += jumlahHariPerBulanMasehi[bulan-1];
  if ( (bulan >= 2) && !(tahun % 4)) {  jumlahHari++; }
  jumlahHari+= tanggal;
  return jumlahHari + 259;// base 18 April 1999
}

 //unsigned char testt;
void masehiKeHijriah(uint32_t HariMasehi, uint8_t *tanggal, uint8_t *bulan, uint16_t *tahun)
{
uint16_t sisaHari;
*tahun = floor(HariMasehi/tahunBulan);
sisaHari = HariMasehi - (tahunBulan * *tahun);
 
  for(uint8_t i=0;i<sizeof(jumlahHariPerBulanHijriah);i++)
  {
    if(sisaHari <= jumlahHariPerBulanHijriah[i])
    {
      sisaHari -= jumlahHariPerBulanHijriah[i-1];
      *bulan = i;
      break;
    }
  }
  *tanggal = sisaHari;
  *tahun += 1420;
	//testt = sisaHari;
}

void up_date_hij_jawa (void) {

	jumlahHari = jumlahHariDariTanggal( tanggalMasehi, bulanMasehi, tahunMasehi );
	//weton = jumlahHari%5;
  masehiKeHijriah(jumlahHari, &tanggalHijriah, &bulanHijriah, &tahunHijriah);
	if(tanggalHijriah==0) tanggalHijriah=1; if(bulanHijriah==0) bulanHijriah=1; 
	if( set.ajust_cldr<6) { tanggalHijriah = tanggalHijriah + set.ajust_cldr; } 
  else if ( set.ajust_cldr>5 && set.ajust_cldr<10 ) { tanggalHijriah = tanggalHijriah - (set.ajust_cldr-5); } //-1,2,3
	//send(tanggalHijriah); send(' '); send(bulanHijriah); send(' '); send(tahunHijriah>>8); send(tahunHijriah);	
}

void call_hijriah(void) {
	jam (); up_date_hij_jawa ();
	//if(rmt.jawa) {
	//	sprintf( bfr.bufr,"%s %s, %02u %s %04u",dino_jowo[hari-1],pasaran[weton],tanggalHijriah,
	//    bln_jowo[bulanHijriah-1],tahunHijriah+512 ); } else {
	//sprintf( bfr.bufr,"%s, %02u %s %04uH",dt_day[hari-1],tanggalHijriah,namaBulanHijriah[bulanHijriah-1],tahunHijriah); 
	//					 }
	sprintf( bfr.bufr,"%s, %02u %s %04uH",dt_day[hari-1],tanggalHijriah,namaBulanHijriah[bulanHijriah-1],tahunHijriah); 
	len= strlen(bfr.bufr);  
	if(set.cek_font) clearScreen(1);	
	if(set.mdl_hj) {selectFont(Droid_Sans_12);  set.cek_font=0; drawMarquee(bfr.bufr,len,1); } 
  else { if(set.panel==1) { selectFont( SystemFont5x7 ); drawMarquee(bfr.bufr,len,9); } else 
                  { selectFont(System5x7);  drawMarquee(bfr.bufr,len,8); }  set.cek_font=1;  }
	
	BIT.A=0; 
	while(!BIT.A){ //big_bfr.hour2();
	if(set.mdl_hj) {
    selectFont(Droid_Sans_12); } else {
	switch (set.panel) {
		//case 1: jam_1p();    break;	
		case 4: jdws_sow4(); break;
		case 3: jdws_sow3(); break;
		case 2: jam_mini_bold (); break;	
		case 6: if(!set.jwsDisply2) jdws_sow6(); else jdws_sow26(); break;
		case 5: jdws_sow5(); break;
		//case 7: if(!set.jwsDisply2) jdws_sow7(); else show_jds27(); break;
		//case 8: if(!set.jwsDisply2) jdws_sow8(); else show_jds28(); break;
		//case 10:if(!set.jwsDisply2) jdws_sow10();else show_jds210(); break;
		//case 9: if(!set.jwsDisply2) jdws_sow9(); else show_jds29(); break;
	}; 
	selectFont(System5x7); //if(set.panel==1) selectFont( SystemFont5x7 ); else  selectFont(bold_8x8); 
	}
	 
	if(set.panel >2) {
	  if(set.fix_jam) BIT.A=stepMarqueePanel(-1,0); else BIT.A=stepMarquees(-1,0); } else {BIT.A=stepMarquees(-1,0); 
					};
	 if(set.panel==1)  // Delay(6*set.speed);				
	 if(set.panel>1 && set.panel<7)   //Delay(3*set.speed);
	 if(set.panel>6 ) 	//Delay(2*set.speed);
	 if( set.fix_jam && set.panel >2 ) fixjam(); 
	android (); 
	 if(BIT.wkt_azan) break;	
	}; 
}

void sow_panel (void) {
	switch (set.panel) {
	case 4: jdws_sow4(); break;
	case 3: jdws_sow3(); break;
	case 2: jam_mini_bold (); break;	
	};
}	 

void disp_panel (void) {
 
 set.fix_jam=1; if(FONT>1) FONT=0;
 if (trun + (set.speed*4) < osal_GetSystemClock()) { trun = osal_GetSystemClock();	
 if(BIT.A){	
	 hitung_jws();
	 osal_mem_free(bfr.bufr);
	switch (urut) {
		case 0: hal_fs_item_read(fs_text01,(uint8*)bfr.dt_rx,200,NULL) ;
			FONT=bfr.dt_rx[0]-'1'; for(int i=0; i<200; i++ ) bfr.bufr[i]=bfr.dt_rx[i+1];
		    len=strlen(bfr.bufr); if(!len) { urut++; return; }  //LOG("F=%d |Text1=%s\n",FONT,bfr.bufr);
			osal_mem_free(bfr.dt_rx); break;
		case 1: hal_fs_item_read(fs_text02,(uint8*)bfr.dt_rx,200,NULL) ;
			FONT=bfr.dt_rx[0]-'1'; for(int i=0; i<200; i++ ) bfr.bufr[i]=bfr.dt_rx[i+1];
		    len=strlen(bfr.bufr); if(!len) { urut++; return; } //LOG("F=%d |Text2=%s\n",bfr.dt_rx[0],bfr.bufr); 
			osal_mem_free(bfr.dt_rx); break;
		case 2: hal_fs_item_read(fs_text03,(uint8*)bfr.dt_rx,200,NULL) ;
			FONT=bfr.dt_rx[0]-'1'; for(int i=0; i<200; i++ ) bfr.bufr[i]=bfr.dt_rx[i+1];
		    len=strlen(bfr.bufr); if(!len) { urut++; return; } //LOG("F=%d |Text3=%s\n",bfr.dt_rx[0],bfr.bufr);
			osal_mem_free(bfr.dt_rx); break;
		case 3: hal_fs_item_read(fs_text04,(uint8*)bfr.dt_rx,200,NULL) ;
			FONT=bfr.dt_rx[0]-'1'; for(int i=0; i<200; i++ ) bfr.bufr[i]=bfr.dt_rx[i+1];
		    len=strlen(bfr.bufr); if(!len) { urut++; return; } //LOG("F=%d |Text1=%s\n",bfr.dt_rx[0],bfr.bufr); 
			osal_mem_free(bfr.dt_rx); break;
		case 4: hal_fs_item_read(fs_text05,(uint8*)bfr.dt_rx,200,NULL) ;
			FONT=bfr.dt_rx[0]-'1'; for(int i=0; i<200; i++ ) bfr.bufr[i]=bfr.dt_rx[i+1];
		    len=strlen(bfr.bufr); if(!len) { urut++; return; } //LOG("F=%d |Text1=%s\n",bfr.dt_rx[0],bfr.bufr);
			osal_mem_free(bfr.dt_rx); break;
	}
	switch (FONT) {
		case 0 : selectFont( SystemFont5x7 ); baris =9; break;
		case 1 : selectFont(Droid_Sans_12); baris =1; break;
		default : selectFont(Droid_Sans_12); baris =1; break;
		};
	//sprintf(bfr.bufr,"SELAMAT DATANG INDONESIA ELTOP");
	drawMarquee(bfr.bufr,strlen(bfr.bufr),baris); BIT.A=0; //
	if(FONT>0) clearScreen(1);
 }
 if(!BIT.A){	
	if(FONT==0) { sow_panel (); } ; //else { jam(); };
	switch (FONT) {
		case 0 : selectFont( SystemFont5x7 ); break;
        //case 1 : selectFont(bold_7x8); break;		
		//case 2 : selectFont(bold_8x8); break;	
	//case 3 : selectFont(bold_6x12); baris =2; break;	
		case 1 : selectFont(Droid_Sans_12); break;	
		//case 4 : selectFont(Arial_Black_16); baris =0; break;	
		default : selectFont(Droid_Sans_12); break;
		};
	if(set.panel >2) {
		if(set.fix_jam) BIT.A=stepMarqueePanel(-1,0); else BIT.A=stepMarquees(-1,0); } 
	    else {BIT.A=stepMarquees(-1,0);  };
	if( set.fix_jam && set.panel >2 ) fixjam(); 	 
	if(BIT.A) { BIT.urut = 1; urut ++; if(urut  >=5) urut=0; }						
  }; 	
 }
}


void disp_panel56 (void) {
	//   if(set.warnaPnl) {
   //  COLOR = bufprog[0]-'0'; if(COLOR ==0 || COLOR >4) COLOR=4;
  //   if(COLOR==4) { if(++warna2>3) warna2=1; COLOR = warna2; }; } else { COLOR=1; warna2=1; warna2=1;}
	switch (FONT) {
	case 0 : selectFont( SystemFont5x7 ); baris =9; break;
    //case 1 : selectFont(bold_7x8); baris =8; break;		
	//case 2 : selectFont(bold_8x8); baris =8; break;	
	//case 3 : selectFont(bold_6x12); baris =2; break;	
	case 2 : selectFont(Droid_Sans_12); baris =1; break;	
	//case 4 : selectFont(Arial_Black_16); baris =0; break;	
	};
	drawMarquee(bfr.bufr,len,baris); 
	BIT.A=0; //
	if(FONT>2) clearScreen(1);
	while(!BIT.A){	
			 //BIT.A=stepMarquees(-1,0);;
	if(FONT<3) {
		switch (set.panel) {
		case 6: if(!set.jwsDisply2) jdws_sow6(); else jdws_sow26(); break;
		case 5: jdws_sow5(); break;
		}; } else  { jam(); };
		switch (FONT) {
		case 0 : selectFont( SystemFont5x7 ); baris =9; break;
        //case 1 : selectFont(bold_7x8); baris =8; break;		
		//case 2 : selectFont(bold_8x8); baris =8; break;	
		//case 3 : selectFont(bold_6x12); baris =2; break;	
		case 1 : selectFont(Droid_Sans_12); baris =1; break;	
		//case 4 : selectFont(Arial_Black_16); baris =0; break;	
		};
		 if(set.fix_jam) BIT.A=stepMarqueePanel(-1,0); else BIT.A=stepMarquees(-1,0); 
		// Delay(3*set.speed); 
		 //Delay(3*20); 
		 if(set.fix_jam) fixjam(); 						
		 if(BIT.wkt_azan) break;
		 android (); 
	}; 	 					  	
}


void iqomah_off(void) {
	switch (code_sht) {
	case 1: iqm_off_mnt = ofdisp.subuh; break;
	case 2: iqm_off_mnt = ofdisp.duhur; break;
	case 3: iqm_off_mnt = ofdisp.ashar; break;
	case 4: iqm_off_mnt = ofdisp.magrib; break;
	case 5: iqm_off_mnt = ofdisp.isak; break;
	default: iqm_off_mnt = 5; break;
	};
}
void ajust_jws(void) {
	jd.tambah[0] =set.ajds_sbh; jd.tambah[1] =set.ajds_dhr; jd.tambah[2] =set.ajds_asr;
	jd.tambah[3] =set.ajds_mgb; jd.tambah[4] =set.ajds_isak; jd.tambah[5] = set.ajds_terbit ;
	jd.tambah[6] = set.ajds_duha;	
}
void set_buf_iqm(void) {
	bfr.buf_timer_iqm[0] = 1;
	bfr.buf_timer_iqm[1] = set.iqm_sbh;
	bfr.buf_timer_iqm[2] = set.iqm_dhr;
	bfr.buf_timer_iqm[3] = set.iqm_asr;
	bfr.buf_timer_iqm[4] = set.iqm_mgb;
	bfr.buf_timer_iqm[5] = set.iqm_isak;
	iqm_contr_mnt = 2;
}
void rxscan(void) {
	//int i=6;  uint8_t a=0; //uint8 urut=0; uint8 pg_urut=0;
	uint8 *fsb;
	fsb = (uint8 *)osal_mem_alloc(0xFF);
	extern void *pi2c;
	NVIC_DisableIRQ((IRQn_Type)TIM5_IRQn);
	switch (bfr.dt_rx[0]) {
	case '`':  // Text 
		switch (bfr.dt_rx[1]) {
			case 'a':
		//int i=6;  //uint8_t a=0; //uint8 urut=0; uint8 pg_urut=0; //Serial.printf("ISI Eeprom:0\n");
				fsb[0]= bfr.dt_rx[1];  fsb[1]= bfr.dt_rx[2];  fsb[2]= bfr.dt_rx[3];
				fsb[3] = bfr.dt_rx[5]; 
				hal_fs_item_write( fs_text00,fsb,6 );//LOG("fs_text0= %d \n",hal_fs_item_write( fs_text00,fsb,6 ));
				for(int x=0; x<=0xFF; x++)  fsb[x] = 0;
			    break;
	        case 'b'://a=0;
				//do { fsb[0+a++] = bfr.dt_rx[i++];  } while(bfr.dt_rx[i] !='|'); 
			    for(int x=0; x<=100; x++)  fsb[x] = bfr.dt_rx[x+2];;
				hal_fs_item_write( fs_text01,fsb,200);//LOG("fs_text1= %d pesan= %s\n",hal_fs_item_write( fs_text01,fsb,200),fsb);
				for(int x=0; x<=0xFF; x++)  fsb[x] = 0;
				break;	
			case 'c': //a=0; i++; 
				//do { fsb[0+a++] = bfr.dt_rx[i++]; } while(bfr.dt_rx[i] !='|'); 
			    for(int x=0; x<=100; x++)  fsb[x] = bfr.dt_rx[x+2];
				hal_fs_item_write( fs_text02,fsb,200 );//LOG("fs_text2= %d \n",hal_fs_item_write( fs_text02,fsb,200 ));
				for(int x=0; x<=0xFF; x++)  fsb[x] = 0;
				break;
			case 'd': //a=0; i++; 
				//do { fsb[0+a++] = bfr.dt_rx[i++]; } while(bfr.dt_rx[i] !='|'); 
				for(int x=0; x<=100; x++)  fsb[x] = bfr.dt_rx[x+2];
				hal_fs_item_write( fs_text03,fsb,200 );//LOG("fs_text3= %d \n",hal_fs_item_write( fs_text03,fsb,200 ));
				for(int x=0; x<=0xFF; x++)  fsb[x] = 0;
				break;
			case 'e': //a=0; i++; 
				//do {  fsb[0+a++] = bfr.dt_rx[i++]; } while(bfr.dt_rx[i] !='|'); 
				for(int x=0; x<=100; x++)  fsb[x] = bfr.dt_rx[x+2];
				hal_fs_item_write( fs_text04,fsb,200 );//LOG("fs_text4= %d \n",hal_fs_item_write( fs_text04,fsb,200 ));
				for(int x=0; x<=0xFF; x++)  fsb[x] = 0;
				break;
			case 'f': //a=0; i++; 
				//do { fsb[0+a++] = bfr.dt_rx[i++]; } while(bfr.dt_rx[i] !='|'); 
				for(int x=0; x<=100; x++)  fsb[x] = bfr.dt_rx[x+2];
				hal_fs_item_write( fs_text05,fsb,200 );//LOG("fs_text5= %d \n",hal_fs_item_write( fs_text05,fsb,200 ));
				for(int x=0; x<=0xFF; x++)  fsb[x] = 0;
				break;
		}
		//for(int i; i< strlen(bfr.dt_rx)-1; i++) {
		//	fsb[i]= bfr.dt_rx[i+1];
		//}
		//LOG("fs_text= %d \n",hal_fs_item_write( fs_text,fsb,strlen(bfr.dt_rx)-1 ));
		break;
	case '$': //SetJam
		//code[0] = decToBcd(((bfr.dt_rx[14] - '0') * 10 + (bfr.dt_rx[15] - '0'))); // detik
		//code[1] = decToBcd(((bfr.dt_rx[12] - '0') * 10 + (bfr.dt_rx[13] - '0')));
		//code[2] = decToBcd(((bfr.dt_rx[10] - '0') * 10 + (bfr.dt_rx[11] - '0')));
		//code[3] = decToBcd(((bfr.dt_rx[9] - '0')));                                // day
		//code[4] = decToBcd(((bfr.dt_rx[7] - '0') * 10 + (bfr.dt_rx[8] - '0')));
		//code[5] = decToBcd(((bfr.dt_rx[5] - '0') * 10 + (bfr.dt_rx[6] - '0')));
		//code[6] = decToBcd(((bfr.dt_rx[3] - '0') * 10 + (bfr.dt_rx[4] - '0'))); //set th
	    //pi2c =hal_i2c_init(I2C_0,I2C_CLOCK_100K); WaitMs(1);
		//hal_i2c_write(pi2c, 0x68, 0, code, 7); WaitMs(1);//clearScreen(1);  //jam();
	    code[0] = ((bfr.dt_rx[14] - '0') * 10 + (bfr.dt_rx[15] - '0')); // detik
		code[1] = ((bfr.dt_rx[12] - '0') * 10 + (bfr.dt_rx[13] - '0'));
		code[2] = ((bfr.dt_rx[10] - '0') * 10 + (bfr.dt_rx[11] - '0'));
		code[3] = ((bfr.dt_rx[9] - '0'));                                // day
		code[4] = ((bfr.dt_rx[7] - '0') * 10 + (bfr.dt_rx[8] - '0'));
		code[5] = ((bfr.dt_rx[5] - '0') * 10 + (bfr.dt_rx[6] - '0'));
		code[6] = ((bfr.dt_rx[3] - '0') * 10 + (bfr.dt_rx[4] - '0')); //set th
	    Ti.hour =code[2]; Ti.minutes=code[1]; Ti.seconds=code[0]; day=code[3];
		Ti.day=code[4]; Ti.month=code[5]; Ti.year=code[6]+2000;
	    app_datetime_set( Ti );
		break;
		//LOG("bfr.dt_rx= %s,%c\n",bfr.dt_rx,bfr.dt_rx[1]);//LOG("Cek-RX\n");
	case '&': //Iqomah
		set.iqm_sbh = ((bfr.dt_rx[1]-'0')*10 + (bfr.dt_rx[2]-'0'));  // Shubuh
        set.iqm_dhr = ((bfr.dt_rx[3]-'0')*10 + (bfr.dt_rx[4]-'0'));
        set.iqm_asr = ((bfr.dt_rx[5]-'0')*10 + (bfr.dt_rx[6]-'0'));
        set.iqm_mgb = ((bfr.dt_rx[7]-'0')*10 + (bfr.dt_rx[8]-'0'));
        set.iqm_isak= ((bfr.dt_rx[9]-'0')*10 + (bfr.dt_rx[10]-'0')) ;  // isak
              //set.iqm_off = ((bfr.dt_rx[11]-'0')*10 + (bfr.dt_rx[12]-'0')) ;  // waktu sholat
              //set.jumat   = ((bfr.dt_rx[11]-'0')*100 + (bfr.dt_rx[12]-'0')*10 + (bfr.dt_rx[13]-'0')); //timer jumatan
		adzan.subuh  = ((bfr.dt_rx[11]-'0')*10 + (bfr.dt_rx[12]-'0'));  // Shubuh
		adzan.duhur  = ((bfr.dt_rx[13]-'0')*10 + (bfr.dt_rx[14]-'0'));  // dhuhur
		adzan.ashar  = ((bfr.dt_rx[15]-'0')*10 + (bfr.dt_rx[16]-'0'));  // asar
		adzan.magrib = ((bfr.dt_rx[17]-'0')*10 + (bfr.dt_rx[18]-'0'));  // magrib
		adzan.isak   = ((bfr.dt_rx[19]-'0')*10 + (bfr.dt_rx[20]-'0'));  // isak
							
		ofdisp.subuh = ((bfr.dt_rx[21]-'0')*10 + (bfr.dt_rx[22]-'0'));  // sbh
		ofdisp.duhur = ((bfr.dt_rx[23]-'0')*10 + (bfr.dt_rx[24]-'0'));  // dhr
		ofdisp.ashar = ((bfr.dt_rx[25]-'0')*10 + (bfr.dt_rx[26]-'0'));  // asr
		ofdisp.magrib = ((bfr.dt_rx[27]-'0')*10 + (bfr.dt_rx[28]-'0')); // mgb
		ofdisp.isak  = ((bfr.dt_rx[29]-'0')*10 + (bfr.dt_rx[30]-'0'));  // isk
		set.jumat    = ((bfr.dt_rx[31]-'0')*10 + (bfr.dt_rx[32]-'0'));  // timer jumatan
		set.msg_jum  = (bfr.dt_rx[33]-'0');    // pesan jumatan
							
		set.iqm = (bfr.dt_rx[34]-'0');    // man-auto
		set.IQM_disp = (bfr.dt_rx[35]-'0');    // tetx besar-kecil
		fsb[0]=set.iqm_sbh; fsb[1]=set.iqm_dhr; fsb[2]=set.iqm_asr; fsb[3]=set.iqm_mgb;
		fsb[4]=set.iqm_isak; fsb[5]=adzan.subuh; fsb[6]=adzan.duhur; fsb[7]=adzan.ashar;
		fsb[8]=adzan.magrib; fsb[9]=adzan.isak; fsb[10]=ofdisp.subuh; fsb[11]=ofdisp.duhur;
		fsb[12]=ofdisp.ashar; fsb[13]=ofdisp.magrib; fsb[14]=ofdisp.isak; fsb[15]=set.jumat;
		fsb[16]=set.msg_jum; fsb[17]=set.iqm; fsb[18]=set.IQM_disp; 
		set_buf_iqm(); 
		hal_fs_item_write(fs_iqomah,fsb,19);//LOG("fs_iqomah= %d \n",hal_fs_item_write(fs_iqomah,fsb,19) );
		break;
	case '^':  // Bios
        set.britnes = ((bfr.dt_rx[1]-'0')*10 + (bfr.dt_rx[2]-'0')) ;  // britnes
        set.speed   = ((bfr.dt_rx[3]-'0')*10 + (bfr.dt_rx[4]-'0')) ;  // speed
        set.big_hr  = ((bfr.dt_rx[5]-'0')*10 + (bfr.dt_rx[6]-'0')) ;  // speed
        set.plh_hemat= ((bfr.dt_rx[7]-'0')*10 + (bfr.dt_rx[8]-'0')) ;  // waktu hemat
        cek.mdl_hemat= (bfr.dt_rx[9]-'0');    //pilih jam hemat mati/redup
        fg_ganti = (bfr.dt_rx[10]-'0');    //model jam
	    fsb[0]=set.britnes; fsb[1]=set.speed ; fsb[2]=set.big_hr; fsb[3]=set.plh_hemat;
		fsb[4]=cek.mdl_hemat; fsb[5]=fg_ganti;
	    hal_fs_item_write(fs_bios,fsb,6);//LOG("fsb_bios=%d,%d,%d  Write=%d\n",fsb[0],fsb[1],fsb[2],hal_fs_item_write(fs_bios,fsb,6));
	    //osal_snv_write(fs_bios,6,fsb);
		break;
	case '#': // Lintang bujur
        if(bfr.dt_rx[1]=='S') fsb[0]=1;
        if(bfr.dt_rx[1]=='U') fsb[0]=0;
           fsb[1] = (((bfr.dt_rx[2]-0x30)*100) + ((bfr.dt_rx[3]-0x30)*10) + (bfr.dt_rx[4]-0x30)) ;
           fsb[2] = (((bfr.dt_rx[6]-0x30)*10) + (bfr.dt_rx[7]-0x30));  //poin lintang
        if(bfr.dt_rx[8]=='B') fsb[3]=1;
        if(bfr.dt_rx[8]=='T') fsb[3]=0;
           fsb[4] = (((bfr.dt_rx[9]-0x30)*100) + ((bfr.dt_rx[10]-0x30)*10) + (bfr.dt_rx[11]-0x30)) ;
           fsb[5] = (((bfr.dt_rx[13]-0x30)*10) + (bfr.dt_rx[14]-0x30)) ;  //poin lintang
           fsb[6] = (bfr.dt_rx[15]-0x30);    //zonz waktu
	
		jd.Lintang = fsb[1]+ ((0.01*fsb[2])/0.6); if(fsb[0]) jd.Lintang = -1*jd.Lintang;
		jd.bujur =fsb[4]+ ((0.01*fsb[5])/0.6); if(fsb[3]) jd.bujur = -1*jd.bujur; 
		jd.zona = fsb[6]; jd.H = 30; 
        //LOG("LB=%d %d %d %d %d %d %d %d \n",fsb[0],fsb[1],fsb[2],fsb[3],fsb[4],fsb[5],fsb[6]);
        hal_fs_item_write(fs_lt_bjr,fsb,7);//LOG("fs_lt_bjr= %d \n",hal_fs_item_write(fs_lt_bjr,fsb,7) );
		jam();  hitung_jws();
		break;
	case '%':  // jws
		fsb[0] = ((bfr.dt_rx[1]-'0')*10 + (bfr.dt_rx[2]-'0')) ; //ajst sbh
        fsb[1] = ((bfr.dt_rx[3]-'0')*10 + (bfr.dt_rx[4]-'0')) ; //ajst dhr
        fsb[2] = ((bfr.dt_rx[5]-'0')*10 + (bfr.dt_rx[6]-'0')) ; //ajst asr
        fsb[3] = ((bfr.dt_rx[7]-'0')*10 + (bfr.dt_rx[8]-'0')) ; //ajst mgrb
        fsb[4] = ((bfr.dt_rx[9]-'0')*10 + (bfr.dt_rx[10]-'0')) ; //ajst isk
        fsb[5] = ((bfr.dt_rx[11]-'0')*10 + (bfr.dt_rx[12]-'0')) ; //ajst trbt
        fsb[6] = ((bfr.dt_rx[13]-'0')*10 + (bfr.dt_rx[14]-'0')) ; //ajst duha
							 
        fsb[7] = ((bfr.dt_rx[15]-'0')*10 + (bfr.dt_rx[16]-'0')) ; //fixjam sbh		
        fsb[8] = ((bfr.dt_rx[17]-'0')*10 + (bfr.dt_rx[18]-'0')) ; //fixmen sbh	
		fsb[9] = ((bfr.dt_rx[19]-'0')*10 + (bfr.dt_rx[20]-'0')) ; //fixjam dhr	
        fsb[10] = ((bfr.dt_rx[21]-'0')*10 + (bfr.dt_rx[22]-'0')) ; //fixmen dhr
	    fsb[11] = ((bfr.dt_rx[23]-'0')*10 + (bfr.dt_rx[24]-'0')) ; //fixjam asr		
        fsb[12] = ((bfr.dt_rx[25]-'0')*10 + (bfr.dt_rx[26]-'0')) ; //fixmen asr	
		fsb[13] = ((bfr.dt_rx[27]-'0')*10 + (bfr.dt_rx[28]-'0')) ; //fixjam mgb		
        fsb[14] = ((bfr.dt_rx[29]-'0')*10 + (bfr.dt_rx[30]-'0')) ; //fixmen mgb	
		fsb[15] = ((bfr.dt_rx[31]-'0')*10 + (bfr.dt_rx[32]-'0')) ; //fixjam isk		
        fsb[16] = ((bfr.dt_rx[33]-'0')*10 + (bfr.dt_rx[34]-'0')) ; //fixmen isk	
		fsb[17] = ((bfr.dt_rx[35]-'0')*10 + (bfr.dt_rx[36]-'0')) ; //fixjam jum		
        fsb[18] = ((bfr.dt_rx[37]-'0')*10 + (bfr.dt_rx[38]-'0')) ; //fixmen jum	
		fsb[19] =  (bfr.dt_rx[39]-'0'); //cekfix sbh
		fsb[20] =  (bfr.dt_rx[40]-'0'); //cekfix dhr
		fsb[21] =  (bfr.dt_rx[41]-'0'); //cekfix asr
		fsb[22] =  (bfr.dt_rx[42]-'0'); //cekfix mgb
		fsb[23] =  (bfr.dt_rx[43]-'0'); //cekfix isk
		fsb[24] =  (bfr.dt_rx[44]-'0'); //cekfix jum
		
		wkt_fix.subuh  = (fsb[7]*100) + fsb[8];
		wkt_fix.duhur  = (fsb[9]*100) + fsb[10];
		wkt_fix.ashar  = (fsb[11]*100) + fsb[12];
		wkt_fix.magrib = (fsb[13]*100) + fsb[14];
		wkt_fix.isak   = (fsb[15]*100) + fsb[16];
		wkt_fix.jumat  = (fsb[17]*100) + fsb[18];
		cek.subuh  = fsb[19]; cek.duhur  = fsb[20];
		cek.ashar  = fsb[21]; cek.magrib = fsb[22];
		cek.isak   = fsb[23]; cek.jumat  = fsb[24];
							 
		set.ajds_sbh = fsb[0]; set.ajds_dhr= fsb[1]; set.ajds_asr= fsb[2]; set.ajds_mgb= fsb[3]; 
		set.ajds_isak= fsb[4]; set.ajds_terbit = fsb[5]; set.ajds_duha = fsb[6];
		hal_fs_item_write(fs_jws,fsb,25); //LOG("fs_jws= %d \n",hal_fs_item_write(fs_jws,fsb,25) );
		ajust_jws();
		break;
	
	case '+': // seting panel
		set.panel  = (bfr.dt_rx[1]-'0')+0; // jumplah modul
		set.polaris = (bfr.dt_rx[2]-'0')-1;
        fsb[2] 	= (bfr.dt_rx[3]-'0');
        fsb[3]	= (bfr.dt_rx[4]-'0');
        if(set.polaris>2) set.polaris = 0;
		fsb[1] = set.polaris;
		fsb[0]=set.panel ; 
	    hal_fs_item_write( fs_panel,fsb,4 ); //LOG("fs_panel= %d \n",hal_fs_item_write( fs_panel,fsb,4 ));
		break;
	}
	NVIC_EnableIRQ((IRQn_Type)TIM5_IRQn);
	osal_mem_free(bfr.dt_rx);//osal_memset(bfr.dt_rx,0,sizeof(bfr.dt_rx));
	osal_mem_free(fsb);
}

void read_data_flash (void) {
	int ret; extern uint8 oe;
	uint8 *fsb;
	fsb = (uint8 *)osal_mem_alloc(0xFF);
	NVIC_DisableIRQ((IRQn_Type)TIM5_IRQn);
	osal_mem_free(fsb); ret=hal_fs_item_read(fs_bios,fsb,6,NULL);
	if(ret == 0) {
		//LOG("fs_bios sukses \n");
		set.britnes =fsb[0]; set.speed = fsb[1]; set.big_hr = fsb[2];
		set.plh_hemat=fsb[3]; cek.mdl_hemat = fsb[4]; fg_ganti = fsb[5];
	  } else {
		//LOG("fs_bios = error-%d \n",ret);
		set.britnes =3; set.speed = 8; set.big_hr = 10;
		set.plh_hemat=24; cek.mdl_hemat = 0; fg_ganti = 0;
		fsb[0]=set.britnes; fsb[1]=set.speed ; fsb[2]=set.big_hr; fsb[3]=set.plh_hemat;
		fsb[4]=cek.mdl_hemat; fsb[5]=fg_ganti;
		  hal_fs_item_write(fs_bios,fsb,6);
		//LOG("fsb_bios=%d,%d,%d  Write=%d\n",fsb[0],fsb[1],fsb[2],hal_fs_item_write(fs_bios,fsb,6));
	} //WaitMs(2);
	osal_mem_free(fsb); ret = hal_fs_item_read(fs_iqomah,fsb,19,NULL);
	if( ret == 0) {
		//LOG("fs_iqomah sukses \n");
		set.iqm_sbh=fsb[0];   set.iqm_dhr=fsb[1];    set.iqm_asr=fsb[2];   set.iqm_mgb=fsb[3];
		set.iqm_isak=fsb[4];  adzan.subuh=fsb[5];    adzan.duhur=fsb[6];   adzan.ashar=fsb[7];
		adzan.magrib=fsb[8];  adzan.isak=fsb[9];     ofdisp.subuh=fsb[10]; ofdisp.duhur=fsb[11];
		ofdisp.ashar=fsb[12]; ofdisp.magrib=fsb[13]; ofdisp.isak=fsb[14];  set.jumat=fsb[15];
		set.msg_jum=fsb[16];  set.iqm=fsb[17];       set.IQM_disp=fsb[18]; 
		set_buf_iqm();
	  } else {
		//LOG("fs_iqomah = error-%d \n",ret);
		fsb[0]=2; 	fsb[1]=2; 	fsb[2]=2; 	fsb[3]=2;
		fsb[4]=2; 	fsb[5]=2; 	fsb[6]=2; 	fsb[7]=2;
		fsb[8]=2; 	fsb[9]=2; 	fsb[10]=1; 	fsb[11]=1;
		fsb[12]=1; 	fsb[13]=1; 	fsb[14]=1; 	fsb[15]=1;
		fsb[16]=1; 	fsb[17]=0; 	fsb[18]=1; 
		hal_fs_item_write(fs_iqomah,fsb,19); set_buf_iqm();
		//LOG("fs_iqomah= %d \n",hal_fs_item_write(fs_iqomah,fsb,19) );
	} //WaitMs(2);
	osal_mem_free(fsb); ret = hal_fs_item_read(fs_lt_bjr,fsb,7,NULL);
	if(ret == 0) {  
		//LOG("fs_lt_bjr sukses \n");	
		jd.Lintang = fsb[1]+ ((0.01*fsb[2])/0.6); if(fsb[0]) jd.Lintang = -1*jd.Lintang;
		jd.bujur =fsb[4]+ ((0.01*fsb[5])/0.6); if(fsb[3]) jd.bujur = -1*jd.bujur; 
		jd.zona = fsb[6]; 
		jd.H = 30; jam();  hitung_jws();
	  } else {  //1,7,14,0,112,44,7 surabaya
		//LOG("fs_lt_bjr = error-%d \n",ret);
		fsb[0]=1; 	fsb[1] = 7 ; fsb[2] = 14;  //poin lintang
        fsb[3]=0; 	fsb[4] = 112 ; fsb[5] = 44 ;  //poin lintang
        fsb[6] = 7;    //zonz waktu
		hal_fs_item_write(fs_lt_bjr,fsb,7); jam();  hitung_jws();
		//LOG("fs_lt_bjr= %d \n",hal_fs_item_write(fs_lt_bjr,fsb,7) );  
	} //WaitMs(2);
	osal_mem_free(fsb); ret = hal_fs_item_read(fs_jws,fsb,25,NULL);
	if(ret == 0) {  
		//LOG("fs_jws sukses \n");	
		set.ajds_sbh = fsb[0]; set.ajds_dhr= fsb[1]; set.ajds_asr= fsb[2]; set.ajds_mgb= fsb[3]; 
		set.ajds_isak= fsb[4]; set.ajds_terbit = fsb[5]; set.ajds_duha = fsb[6];
		wkt_fix.subuh  = (fsb[7]*100) + fsb[8];
		wkt_fix.duhur  = (fsb[9]*100) + fsb[10];
		wkt_fix.ashar  = (fsb[11]*100) + fsb[12];
		wkt_fix.magrib = (fsb[13]*100) + fsb[14];
		wkt_fix.isak   = (fsb[15]*100) + fsb[16];
		wkt_fix.jumat  = (fsb[17]*100) + fsb[18];
		cek.subuh  = fsb[19]; cek.duhur  = fsb[20];
		cek.ashar  = fsb[21]; cek.magrib = fsb[22];
		cek.isak   = fsb[23]; cek.jumat  = fsb[24];		ajust_jws();	
	  } else {
		//LOG("fs_jws = error-%d \n",ret);
		fsb[0] = 32; //ajst sbh
        fsb[1] = 32; //ajst dhr
        fsb[2] = 32; //ajst asr
        fsb[3] = 32; //ajst mgrb
        fsb[4] = 32; //ajst isk
        fsb[5] = 32; //ajst trbt
        fsb[6] = 32; //ajst duha
							 
        fsb[7] = 4; //fixjam sbh		
        fsb[8] = 0; //fixmen sbh	
		fsb[9] = 12; //fixjam dhr	
        fsb[10] = 0; //fixmen dhr
	    fsb[11] = 15; //fixjam asr		
        fsb[12] = 0; //fixmen asr	
		fsb[13] = 17; //fixjam mgb		
        fsb[14] = 30; //fixmen mgb	
		fsb[15] = 19; //fixjam isk		
        fsb[16] = 0; //fixmen isk	
		fsb[17] = 12; //fixjam jum		
        fsb[18] = 0; //fixmen jum	
		fsb[19] =  0; //cekfix sbh
		fsb[20] =  0; //cekfix dhr
		fsb[21] =  0; //cekfix asr
		fsb[22] =  0; //cekfix mgb
		fsb[23] =  0; //cekfix isk
		fsb[24] =  0; //cekfix jum
		
		set.ajds_sbh = fsb[0]; set.ajds_dhr= fsb[1]; set.ajds_asr= fsb[2]; set.ajds_mgb= fsb[3]; 
		set.ajds_isak= fsb[4]; set.ajds_terbit = fsb[5]; set.ajds_duha = fsb[6];
		wkt_fix.subuh  = (fsb[7]*100) + fsb[8];
		wkt_fix.duhur  = (fsb[9]*100) + fsb[10];
		wkt_fix.ashar  = (fsb[11]*100) + fsb[12];
		wkt_fix.magrib = (fsb[13]*100) + fsb[14];
		wkt_fix.isak   = (fsb[15]*100) + fsb[16];
		wkt_fix.jumat  = (fsb[17]*100) + fsb[18];
		cek.subuh  = fsb[19]; cek.duhur  = fsb[20];
		cek.ashar  = fsb[21]; cek.magrib = fsb[22];
		cek.isak   = fsb[23]; cek.jumat  = fsb[24];
		hal_fs_item_write(fs_jws,fsb,25); ajust_jws();			
		//LOG("fs_jws= %d \n",hal_fs_item_write(fs_jws,fsb,25) );
	} //WaitMs(2); 
	osal_mem_free(fsb); ret = hal_fs_item_read(fs_panel,fsb,4,NULL);
	if(ret == 0) {  
		//LOG("fs_Panel sukses \n");
		set.panel  = fsb[0]; // jumplah modul
		set.polaris = fsb[1];  if(set.polaris>2) set.polaris = 0;
        //fsb[2] 	= 0;
        oe	= fsb[3];
	  } else {
		//LOG("fs_Panel = error-%d \n",ret);
        fsb[2] = 0;  // dt
        fsb[3] = 0; // oe
		fsb[1] = 0; //polaris
		fsb[0] = 4 ; //panel
	    //LOG("fs_panel= %d \n",hal_fs_item_write( fs_panel,fsb,4 ));
		hal_fs_item_write( fs_panel,fsb,4 );
	}
	osal_mem_free(fsb); ret = hal_fs_item_read(fs_text01,fsb,200,NULL);
	if(ret == 0) {  
		//LOG("fs_Text01 sukses \n");
	  } else {
		 //LOG("fs_Text01 = error-%d \n",ret);
		sprintf((char *) fsb,"1WELCOMO TO BLE JWS");
		hal_fs_item_write( fs_text01,fsb,200 );//LOG("fs_Text01= %d \n",hal_fs_item_write( fs_text01,fsb,200 ));
		osal_mem_free(fsb);
		  sprintf((char *) fsb,"1");
		hal_fs_item_write( fs_text02,fsb,200 ); //LOG("fs_Text02= %d \n",hal_fs_item_write( fs_text02,fsb,200 ));
		  osal_mem_free(fsb);
		  sprintf((char *) fsb,"1");
		hal_fs_item_write( fs_text03,fsb,200 );//LOG("fs_Text03= %d \n",hal_fs_item_write( fs_text03,fsb,200 ));
		  osal_mem_free(fsb);
		  sprintf((char *) fsb,"1");
		hal_fs_item_write( fs_text04,fsb,200 );//LOG("fs_Text04= %d \n",hal_fs_item_write( fs_text04,fsb,200 ));
		  osal_mem_free(fsb);
		  sprintf((char *) fsb,"1");
		hal_fs_item_write( fs_text05,fsb,200 );//LOG("fs_Text05= %d \n",hal_fs_item_write( fs_text05,fsb,200 ));
	}
    osal_mem_free(fsb); urut=0;
	NVIC_EnableIRQ((IRQn_Type)TIM5_IRQn);
	LOG("error-%d \n",ret);
} 

//

