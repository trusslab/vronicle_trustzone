/*****************************************************************************

File Name: 	<inittab.h>

File Description:
Contains YUV conversion defines

*****************************************************************************/

#define TAB_76309
#ifdef TAB_76309

//YUV to RGB conversion
//
//    |R|     | 1.1644    0       1.596   |   |Y-16 |
//    |G|  =  | 1.1644    -0.392  -0.813  |   |U-128|
//    |B|     | 1.1644    2.02    0       |   |V-128|
//

#define SCALEY     76309
#define SCALECrv   104597
#define SCALECbu   132201
#define SCALECgu   25675
#define SCALECgv   53279

#define SHIFTY     16
#define SHIFTCC    128
#define SHIFTCrv   128
#define SHIFTCbu   128
#define SHIFTCgu   128
#define SHIFTCgv   128


//RGB to YUV conversion
//
//    |Y|     |  0.2568    0.5042    0.0978| |R| + |16 |
//    |U|  =  | -0.1480   -0.2906    0.4387| |G| + |128|
//    |V|     |  0.4392   -0.3678   -0.0714| |B| + |128|
//

#define SCALEYR     16831
#define SCALEYG     33040
#define SCALEYB     6412
#define SCALEUR    -9702
#define SCALEUG    -19046
#define SCALEUB     28748
#define SCALEVR     28783
#define SCALEVG    -24105 
#define SCALEVB    -4678

/* LUTs for YUV to RGB conversion */
long int crv_tab[256];
long int cbu_tab[256];
long int cgu_tab[256];
long int cgv_tab[256];
long int tab_76309[256];

long int yr_tab[256];
long int yg_tab[256];
long int yb_tab[256];
long int ur_tab[256];
long int ug_tab[256];
long int ub_tab[256];
long int vr_tab[256];
long int vg_tab[256];
long int vb_tab[256];

unsigned char clp[1024];



/*****************************************************************************
  Routine:  void InitConvt(int width, int height)

        returns        	: void
		width			: frame width
		height			: frame height
  Description          	: Initializes the LUTs for yuvtorgb conversion
*****************************************************************************/

void InitConvt(int width, int height) {
   long int crv,cbu,cgu,cgv;
   int i,ind;   
     
   crv = SCALECrv; cbu = SCALECbu;
   cgu = SCALECgu; cgv = SCALECgv;
  
   for (i = 0; i < 256; i++) {
        crv_tab[i] = (i-SHIFTCrv) * crv;
        cbu_tab[i] = (i-SHIFTCbu) * cbu;
        cgu_tab[i] = (i-SHIFTCgu) * cgu;
        cgv_tab[i] = (i-SHIFTCgv) * cgv;
        tab_76309[i] = SCALEY*(i-SHIFTY);

        yr_tab[i] = i*SCALEYR;
        yg_tab[i] = i*SCALEYG;
        yb_tab[i] = i*SCALEYB;
        
        ur_tab[i] = i*SCALEUR;
        ug_tab[i] = i*SCALEUG;
        ub_tab[i] = i*SCALEUB;
        
        vr_tab[i] = i*SCALEVR;
        vg_tab[i] = i*SCALEVG;
        vb_tab[i] = i*SCALEVB;
      
   }
	 
   for (i=0; i<384; i++)
	  clp[i] =0;
   ind=384;
   for (i=0;i<256; i++)
	   clp[ind++]=i;
   ind=640;
   for (i=0;i<384;i++)
	   clp[ind++]=255;
}

#endif
