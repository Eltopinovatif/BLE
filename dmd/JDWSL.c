
#include "jdwsl.h"
//#include "stdlib.h"
#include "math.h"
//#include <seting.h>

//#define pi              3.14159
//const unsigned char dt_wkt[8] ={0,3,4,5,7,9,12};
 

js jd;

void JDgrenwitc (int tanggal,int bulan,int tahun) { 
               jd.M = bulan < 3 ? bulan+12:bulan;
               jd.Y = bulan < 3 ? tahun-1: tahun;
               jd.A = (int)tahun/100;
               jd.BB = (int)(2 + (jd.A/4) - jd.A);
           jd.JDgw  =  1720994.5+ (uint32_t)(365.25*jd.Y) + (int)(30.60001*(jd.M+1)) +jd.BB + tanggal + 0.5 ;
         }
//hitung julian day lokal dipengarui ZONA +7
float JDlok(void) {
      //JDlok =  ((julian-2000000)- zona/24.0) ; 
            return (jd.JDgw)- jd.zona/24 ;
             }
// menghitung sudut per geseran matahari
float sdt_tgl  (void) {
       return 2*pi*(JDlok()-(2451545))/365.25;
             }
// menghitung masing2 U
float U (void) {
                 return (JDlok()- (2451545))/36525 ;
             }
// menghitung masing2 L0
float L0 (void) {
                 return  (280.46607 + 36000.7698 * U())*pi/180;
             }
// menghitung Radian pergeseran matahari
float delta (void) {
      return (0.37877+23.264*sin((57.297*sdt_tgl()-79.547)*pi/180) + 
                     0.3812*sin((2*57.297*sdt_tgl()-82.682)*pi/180) +
                     0.17132*sin((3*57.297*sdt_tgl()-59.722)*pi/180) ) * pi/180;
                    }
// pergeseran matahari per menit
float Eqs (void) {
      return (-1 * (1789+237*U()) * sin(L0()) - (7146-62*U()) * cos(L0()) +(9934-14*U()) * sin(2*L0()) -
                   (29+5*U()) * cos(2*L0()) +  (74+10*U())* sin(3*L0()) +(320-4*U())* cos(3*L0()) - 
                                212 * sin(4*L0()))/1000; 
                    }

static float rad_ldtde (void) {
       return jd.Lintang*pi/180;
             }
static void tambahan (unsigned char dtr) {
     if (jd.tambah[dtr]>=30) {
     jd.menit = jd.menit + (jd.tambah[dtr]-30);
           if(jd.menit>59) { jd.menit = jd.menit-60; jd.hour = jd.hour+1; };
         } else { 
     if(jd.menit<(30-jd.tambah[dtr])) {
     jd.menit = 60 - ((30-jd.tambah[dtr])-jd.menit); jd.hour=jd.hour-1; } else {
     jd.menit =jd.menit - (30-jd.tambah[dtr]);  jd.hour=jd.hour; };        
         };   
     }

static void conversi(float contv) {
          //return (int) contv + ((int)((contv-(int) contv)*60))/100.0;
          jd.hour=(int) contv ; //+ ((int)((contv-(int) contv)*60));
          jd.menit = ((int)((contv-(int) contv)*60));
          if( (int)(( ((contv-(int) contv)*60)-jd.menit)*60) > 50) { jd.menit = jd.menit+1;
                    if(jd.menit>59) { jd.menit = jd.menit%60; jd.hour = jd.hour+1; }; };
          }          

void WAKTU_DZUHUR (void) {
           jd.dzuhur = 12 + jd.zona - (jd.bujur/15) - Eqs()/60 ;
           //jd.dzuhur = (int) jd.dzuhur + ((int)((jd.dzuhur-(int) jd.dzuhur)*60))/100.0;
            conversi(jd.dzuhur);           
            tambahan (1); jd.pos[2]=jd.hour; jd.pos[3] = jd.menit;
            //jd.waktu[2] = jd.hour*100+jd.menit;
           }

void WAKTU_ASHAR (void) {
     jd.mazhab = 1;                                                                                                    //0.562777189
     conversi( (jd.dzuhur+(12/pi)*acos((sin(atan(1/(jd.mazhab+ tan(fabs(rad_ldtde()-delta())))))- sin(delta())* sin(rad_ldtde()))
                                        /(cos(delta())* cos(rad_ldtde()))))
                                        );
           tambahan (2); jd.pos[4]=jd.hour; jd.pos[5] = jd.menit;
           //jd.waktu[3] = jd.hour*100+jd.menit;
                      }

void WAKTU_MAGRIB (void) {
     conversi( (jd.dzuhur+(12/pi)* acos(( sin((-0.8333-0.0347* pow(jd.H, 0.5))*pi/180)- sin(delta())* sin(rad_ldtde()))/
                    ( cos(delta())* cos(rad_ldtde()))))
                                );
            tambahan (3); jd.pos[6]=jd.hour; jd.pos[7] = jd.menit;
            //jd.waktu[4] = jd.hour*100+jd.menit;
                      }
/*
static void WAKTU_ISAK (void) {
      conversi( (jd.dzuhur+(12/pi)* acos(( sin((-1*jd.sdt_isak)* pi/180)- sin(delta())* sin(rad_ldtde()))/
                  ( cos(delta())* cos(rad_ldtde()))))
                              );
            tambahan (4); jd.pos[8]=jd.hour; jd.pos[9] = jd.menit;
            //jd.waktu[5] = jd.hour*100+jd.menit;
                       }

static void WAKTU_SUBUH (void) {
      conversi( (jd.dzuhur-(12/pi)* acos(( sin((-1*jd.sdt_subuh)* pi/180)- sin(delta())* sin(rad_ldtde()))/
                    ( cos(delta())* cos(rad_ldtde()))))
                                );
             tambahan (0); jd.pos[0]=jd.hour; jd.pos[1] = jd.menit;
	// imsak
             if(jd.menit<10) {  jd.hour = (jd.hour-1) ;  jd.menit =(60-(10-jd.menit)); } else { jd.menit = (jd.menit-10);};
             if(jd.menit<15) {  jd_jam  = (jd.hour-1) ;  jd_men   =(60-(15-jd.menit)); } else { jd_men = (jd.menit-15); jd_jam=jd.hour; };
             jd.pos[10]=jd.hour; jd.pos[11] = jd.menit;
           }
*/
void WAKTU_SLT_SUDUT (float sudut,uint8_t pilih) {
	int8_t pola;
	int jd_jam,jd_men;
	if(pilih==0) pola=-1; else pola = 1;
	   conversi( (jd.dzuhur-(pola*(12/pi))* acos(( sin((-1*sudut)* pi/180)- sin(delta())* sin(rad_ldtde()))/
             ( cos(delta())* cos(rad_ldtde()))))
                                );
     switch (pilih) {
				case 0:  
						   tambahan (4); jd.pos[8]=jd.hour; jd.pos[9] = jd.menit; break; // isak
				case 1:
						   tambahan (0); jd.pos[0]=jd.hour; jd.pos[1] = jd.menit; // shubuh
	// imsak
             if(jd.menit<10) {  jd.hour = (jd.hour-1) ;  jd.menit =(60-(10-jd.menit)); } else { jd.menit = (jd.menit-10);};
             if(jd.menit<15) {  jd_jam  = (jd.hour-1) ;  jd_men   =(60-(15-jd.menit)); } else { jd_men = (jd.menit-15); jd_jam=jd.hour; };
             jd.pos[10]=jd.hour; jd.pos[11] = jd.menit; break; // imsak
       case 2:					 
	           tambahan (5); jd.pos[12]=jd.hour; jd.pos[13] = jd.menit;break; // terbit
			 case 3:  
						 tambahan (6); jd.pos[14]=jd.hour; jd.pos[15] = jd.menit; break; // dhuha
					  };
      } 
/*
 static void WAKTU_DHUHA (void) {
	         conversi( (jd.dzuhur-(12/pi)* acos(( sin((-1*-3.30)* pi/180)- sin(delta())* sin(rad_ldtde()))/
                    ( cos(delta())* cos(rad_ldtde()))))
                                );
	         tambahan (0); jd.pos[14]=jd.hour; jd.pos[15] = jd.menit;
           } */
/*
static void WAKTU_IMSAK (void) {
           WAKTU_SUBUH(); 
           if(jd.menit<10) {  jd.hour = (jd.hour-1) ;  jd.menit =(60-(10-jd.menit)); } else { jd.menit = (jd.menit-10);};
           if(jd.menit<15) {  jd_jam  = (jd.hour-1) ;  jd_men   =(60-(15-jd.menit)); } else { jd_men = (jd.menit-15); jd_jam=jd.hour; };
           jd.pos[0]=jd.hour; jd.pos[1] = jd.menit;
           //jd.waktu[0] = jd.hour*100 + jd.menit;
           //jam_ON = (jd_jam*100)+jd_men;  
           } 
*/


/*					 
unsigned char isi[15];
static void WAKTU_SHOLAT (void) {
                    //rtc_get_time (&hr,&men,&sec);
                    //rtc_get_date(&day,&date,&month,&year); 
										
        jd.sdt_subuh=20; jd.sdt_isak=18;
				isi[6] = 20; isi[4] =112; isi[5] =45; isi[3] = 0;
				isi[1] = 7; isi[2] = 14; isi [0] =1; isi[7] = 7;
				isi[8] = 2; isi[9] = 2; isi[10] = 2; isi[11] = 2; isi[12] = 2;
        JDgrenwitc (12,4,2000+14);  jd.H = isi[6]*10; 
        jd.bujur =isi[4]+ ((0.01*isi[5])/0.6); if(isi[3]) jd.bujur = -1*jd.bujur;
        jd.Lintang = isi[1]+ ((0.01*isi[2])/0.6); if(isi[0]) jd.Lintang = -1*jd.Lintang;
        for(i=0;i<5;i++) {jd.tambah[i] = isi[i+8]; };  jd.zona =isi[7]; 
                    //jd.waktu[6] = hr*100 + men;    
         WAKTU_DZUHUR(); WAKTU_ASHAR(); WAKTU_MAGRIB();WAKTU_ISAK(); WAKTU_SUBUH(); WAKTU_IMSAK();   
      }
*/

  
			

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

						