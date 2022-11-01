/*
 * definisi.h
 *
 *  Created on: Jul 23, 2022
 *      Author: eltop
 */

#ifndef INCLUDE_DEFINISI_H_
#define INCLUDE_DEFINISI_H_

//#include "types.h"
//#include "OSAL.h"

//#ifdef __cplusplus
//extern "C" {
//#endif



/*struct { // bit data fung
	int  A:1, B:1,  C:1,  big_hour:1;
	int  wkt_azan :1,  buzer:1,  dot:1, iqomah:1;
	int msg_iqm:1,  iqm_off:1, alarm:1, updt_jwds:1;
	int  one:1, kbt:1, in_data:1, kbt_fls:1, dot_aktif :1,RTC_SET:1,bzr_togle:1;
	int beep:1,jam_mini:1,jumat:1,hemat:1;
 } BIT;*/

/*struct { //seting program
   uint8_t  vol;
	 uint8_t  vc_sbh,vc_dhr,vc_asr,vc_mgb,vc_isak;
	 uint8_t  vc_mlm,jam_mlm, mnt_mlm,vc_duha,jam_duha,mnt_duha;
	 uint8_t  iqm,iqm_sbh,iqm_dhr,iqm_asr,iqm_mgb,iqm_isak,iqm_off;
	 uint8_t  big_hr,speed,britnes,adzan,jumat,plh_jam,panel,plh_hemat,msg_jum,iqm_dot,ajd_jwds; //jum10..tot 31
	 uint16_t cdkota;
	 uint8_t ajds_sbh,ajds_dhr,ajds_asr,ajds_mgb,ajds_isak;
	 uint8_t IQM_disp;
 } set;*/

/*struct jadwal_sholat {
 	float  Lintang, bujur, JDgw, dzuhur, zona;
 	int  BB, M, Y, A, H, sdt_subuh, sdt_isak, mazhab;
 	int hour, menit;
 	int pos[16], tambah[8];
 	//unsigned int  waktu[7];
 }jd;*/

 /*struct {  //waktu alaram 1-10
 	uint8_t Jalrm1, Jalrm2, Jalrm3, Jalrm4, Jalrm5, Jalrm6, Jalrm7, Jalrm8, Jalrm9, Jalrm10;
 	uint8_t Malrm1, Malrm2, Malrm3, Malrm4, Malrm5, Malrm6, Malrm7, Malrm8, Malrm9, Malrm10;
 	uint8_t file1, file2, file3, file4, file5, file6, file7, file8, file9, file10;
 	uint8_t cek1, cek2, cek3, cek4, cek5, cek6, cek7, cek8, cek9, cek10;
 	uint8_t day1, day2, day3, day4, day5, day6, day7, day8, day9, day10;
 } ALRM;*/

 /*struct { // back light time
 	uint16_t jam_on, men_on, jam_of, men_of, tot_on, tot_of;
 } light;

 struct { // waktu sholat 5wkt
 	uint16_t subuh, duhur, ashar, magrib, isak, total, jumat, jumatan;
 } wkt;
 struct { // waktu seblm sholat 5wkt
 	uint16_t subuh, duhur, ashar, magrib, isak, jumat, terbit, duha, imsak;
 } wkt_sblm;

 struct { // waktu seblm sholat 5wkt
 	uint8_t subuh, duhur, ashar, magrib, isak, jumat, mlm_jum, mdl_hemat, alrmPerjam;
 }cek;

 struct { // waktu display_of 5wkt
 	uint8_t subuh, duhur, ashar, magrib, isak;
 }disp_of;

 struct { // waktu condownJws 5wkt
 	uint16_t subuh, duhur, ashar, magrib, isak, jumatan;
 }condownjws;

 struct { // waktu seblm sholat 5wkt
 	uint16_t alrm1, alrm2, alrm3, alrm4, alrm5, alrm6, alrm7, alrm8, alrm9, alrm10;
 	//uint16_t alrm11, alrm12, alrm13, alrm14, alrm15, alrm16, alrm17, alrm18, alrm19, alrm20;
 	//uint16_t alrm21, alrm22, alrm23, alrm24, alrm25, alrm26, alrm27, alrm28, alrm29, alrm30;
 } wkt_ALRM;

 struct { // waktu seblm sholat 5wkt
 	uint16_t subuh, duhur, ashar, magrib, isak, jumat;
 } wkt_fix;*/

// uint8_t secon,AMP,MENIT,HOUR,hari,tgl,bln,th;
// uint8_t   code1[10];
 //char hour[10];
 //uint8_t s;
// uint32_t test;

//#ifdef __cplusplus
//}
//#endif
#endif /* INCLUDE_DEFINISI_H_ */
