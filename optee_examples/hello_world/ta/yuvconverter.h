/*****************************************************************************

File Name: 	<yuvconverter.h>

File Description:
Header File for yuvconverter.cpp

*****************************************************************************/
#ifndef OPTEE_YUVCONVERTER_H
#define OPTEE_YUVCONVERTER_H

#include "inittab.h"

#define NUMARGS 	(5+1)			//actual no. of ARGs+1

/*****************************************************************************

  Extern Variables

 *****************************************************************************/

//Look Up Tables
extern long int crv_tab[];
extern long int cbu_tab[];
extern long int cgu_tab[];
extern long int cgv_tab[];
extern long int tab_76309[];

extern long int yr_tab[];
extern long int yg_tab[];
extern long int yb_tab[];
extern long int ur_tab[];
extern long int ug_tab[];
extern long int ub_tab[];
extern long int vr_tab[];
extern long int vg_tab[];
extern long int vb_tab[];

extern unsigned char clp[];


/*****************************************************************************

  Function Prototypes

 *****************************************************************************/

/* Initialization Routines */
void InitConvt(int width, int height);


/* Functions to convert from YUV420 */

/* yuv420 converters */ 
void  yuv420_prog_planar_to_yuv400_prog_planar(unsigned char *,unsigned char *, int , int );		//110100

void  yuv420_prog_planar_to_yuv420_int_planar(unsigned char *,unsigned char *, int , int );			//110111
void  yuv420_prog_planar_to_yvu420_prog_planar(unsigned char *,unsigned char *, int , int );		//110120
void  yuv420_prog_planar_to_yvu420_int_planar(unsigned char *,unsigned char *, int , int );			//110121
void  yuv420_prog_planar_to_ycbcr_planar(unsigned char *,unsigned char *, int , int );				//110130

void  yuv420_prog_planar_to_yuv422_prog_planar(unsigned char *,unsigned char *, int , int );		//110210
void  yuv420_prog_planar_to_yuv422_int_planar(unsigned char *,unsigned char *, int , int );			//110211
void  yuv420_prog_planar_to_yvu422_prog_planar(unsigned char *,unsigned char *, int , int );		//110220
void  yuv420_prog_planar_to_yvu422_int_planar(unsigned char *,unsigned char *, int , int );			//110221
void  yuv420_prog_planar_to_uyvy_prog_packed(unsigned char *,unsigned char *, int , int );			//110230
void  yuv420_prog_planar_to_uyvy_int_packed(unsigned char *,unsigned char *, int , int );			//110231
void  yuv420_prog_planar_to_yuyv_prog_packed(unsigned char *,unsigned char *, int , int );			//110240
void  yuv420_prog_planar_to_yuyv_int_packed(unsigned char *,unsigned char *, int , int );			//110241
void  yuv420_prog_planar_to_yvyu_prog_packed(unsigned char *,unsigned char *, int , int );			//110250
void  yuv420_prog_planar_to_yvyu_int_packed(unsigned char *,unsigned char *, int , int );			//110251

void  yuv420_prog_planar_to_yuv444_prog_planar(unsigned char *,unsigned char *, int , int );		//110310
void  yuv420_prog_planar_to_yuv444_int_planar(unsigned char *,unsigned char *, int , int );			//110311
void  yuv420_prog_planar_to_yvu444_prog_planar(unsigned char *,unsigned char *, int , int );		//110320
void  yuv420_prog_planar_to_yvu444_int_planar(unsigned char *,unsigned char *, int , int );			//110321
void  yuv420_prog_planar_to_uyv444_prog_planar(unsigned char *,unsigned char *, int , int );		//110330
void  yuv420_prog_planar_to_uyv444_int_planar(unsigned char *,unsigned char *, int , int );			//110331
void  yuv420_prog_planar_to_yuv444_prog_packed(unsigned char *,unsigned char *, int , int );		//110340
void  yuv420_prog_planar_to_yuv444_int_packed(unsigned char *,unsigned char *, int , int );			//110341
void  yuv420_prog_planar_to_yvu444_prog_packed(unsigned char *,unsigned char *, int , int );		//110350
void  yuv420_prog_planar_to_yvu444_int_packed(unsigned char *,unsigned char *, int , int );			//110351
void  yuv420_prog_planar_to_uyv444_prog_packed(unsigned char *,unsigned char *, int , int );		//110360
void  yuv420_prog_planar_to_uyv444_int_packed(unsigned char *,unsigned char *, int , int );			//110361

void  yuv420_prog_planar_to_rgb_planar(unsigned char *,unsigned char *, int , int );				//110410
void  yuv420_prog_planar_to_bgr_planar(unsigned char *,unsigned char *, int , int );				//110420
void  yuv420_prog_planar_to_rgb_packed(unsigned char *,unsigned char *, int , int );				//110430
void  yuv420_prog_planar_to_bgr_packed(unsigned char *,unsigned char *, int , int );				//110440





/* Functions to convert to YUV420 */

/* yuv420 converters */ 
void  yuv400_prog_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//100110

void  yuv420_int_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//111110
void  yvu420_prog_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//120110
void  yvu420_int_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//121110
void  ycbcr_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );				//130110

void  yuv422_prog_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//210110
void  yuv422_int_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//211110
void  yvu422_prog_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//220110
void  yvu422_int_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//221110
void  uyvy_prog_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//230110
void  uyvy_int_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//231110
void  yuyv_prog_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//240110
void  yuyv_int_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//241110
void  yvyu_prog_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//250110
void  yvyu_int_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//251110

void  yuv444_prog_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//310110
void  yuv444_int_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//311110
void  yvu444_prog_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//320110
void  yvu444_int_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//321110
void  uyv444_prog_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//330110
void  uyv444_int_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//331110
void  yuv444_prog_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//340110
void  yuv444_int_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//341110
void  yvu444_prog_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//350110
void  yvu444_int_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//351110
void  uyv444_prog_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );		//360110
void  uyv444_int_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );			//361110

void  rgb_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );				//410110
void  bgr_planar_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );				//420110
void  rgb_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );				//430110
void  bgr_packed_to_yuv420_prog_planar(unsigned char *,unsigned char *, int , int );				//440110


void  yuv420_prog_planar_to_rgb_packed(
							unsigned char *bufsrc, 
							unsigned char *bufdest, 
							int width, int height
							)
{

#define YUV_420_PLANAR_PROG
#define SRC
#include "yuv420offsets.h"
#undef SRC
#undef YUV_420_PLANAR_PROG

#define RGB_PACKED
#define DEST
#include "rgboffsets.h"
#undef DEST
#undef RGB_PACKED

	int crv, cgu, cgv, cbu;
	int y, u, v;

	int i, j;

	for(j=0; j<(height/4); j++)
	{
		for(i=0;i<(width/2);i++)
		{

			/* First and Second Lines */	
			u = (*ptrsrccb1++);
			v = (*ptrsrccr1++);
			crv = crv_tab[v];
			cgu = cgu_tab[u];
			cgv = cgv_tab[v];
			cbu = cbu_tab[u];

			/* Top Left */
			y = tab_76309[(*ptrsrcy1++)];	
			(*ptrdestr1) = clp[384+((y + crv)>>16)];  
			(*ptrdestg1) = clp[384+((y - cgu - cgv)>>16)];
            (*ptrdestb1) = clp[384+((y + cbu)>>16)];

			ptrdestr1 += 3;
			ptrdestg1 += 3;
			ptrdestb1 += 3;

			/* Top Right */
			y = tab_76309[(*ptrsrcy1++)];	
			(*ptrdestr1) = clp[384+((y + crv)>>16)];  
			(*ptrdestg1) = clp[384+((y - cgu - cgv)>>16)];
            (*ptrdestb1) = clp[384+((y + cbu)>>16)];

			ptrdestr1 += 3;
			ptrdestg1 += 3;
			ptrdestb1 += 3;

			/* Bottom Left */
			y = tab_76309[(*ptrsrcy2++)];	
			(*ptrdestr2) = clp[384+((y + crv)>>16)];  
			(*ptrdestg2) = clp[384+((y - cgu - cgv)>>16)];
            (*ptrdestb2) = clp[384+((y + cbu)>>16)];

			ptrdestr2 += 3;
			ptrdestg2 += 3;
			ptrdestb2 += 3;

			/* Bottom Right */
			y = tab_76309[(*ptrsrcy2++)];	
			(*ptrdestr2) = clp[384+((y + crv)>>16)];  
			(*ptrdestg2) = clp[384+((y - cgu - cgv)>>16)];
            (*ptrdestb2) = clp[384+((y + cbu)>>16)];

			ptrdestr2 += 3;
			ptrdestg2 += 3;
			ptrdestb2 += 3;


			/* Third and Fourth Lines */
			u = (*ptrsrccb2++);
			v = (*ptrsrccr2++);
			crv = crv_tab[v];
			cgu = cgu_tab[u];
			cgv = cgv_tab[v];
			cbu = cbu_tab[u];

			/* Top Left */
			y = tab_76309[(*ptrsrcy3++)];	
			(*ptrdestr3) = clp[384+((y + crv)>>16)];  
			(*ptrdestg3) = clp[384+((y - cgu - cgv)>>16)];
            (*ptrdestb3) = clp[384+((y + cbu)>>16)];

			ptrdestr3 += 3;
			ptrdestg3 += 3;
			ptrdestb3 += 3;

			/* Top Right */
			y = tab_76309[(*ptrsrcy3++)];	
			(*ptrdestr3) = clp[384+((y + crv)>>16)];  
			(*ptrdestg3) = clp[384+((y - cgu - cgv)>>16)];
            (*ptrdestb3) = clp[384+((y + cbu)>>16)];

			ptrdestr3 += 3;
			ptrdestg3 += 3;
			ptrdestb3 += 3;

			/* Bottom Left */
			y = tab_76309[(*ptrsrcy4++)];	
			(*ptrdestr4) = clp[384+((y + crv)>>16)];  
			(*ptrdestg4) = clp[384+((y - cgu - cgv)>>16)];
            (*ptrdestb4) = clp[384+((y + cbu)>>16)];

			ptrdestr4 += 3;
			ptrdestg4 += 3;
			ptrdestb4 += 3;

			/* Bottom Right */
			y = tab_76309[(*ptrsrcy4++)];	
			(*ptrdestr4) = clp[384+((y + crv)>>16)];  
			(*ptrdestg4) = clp[384+((y - cgu - cgv)>>16)];
            (*ptrdestb4) = clp[384+((y + cbu)>>16)];

			ptrdestr4 += 3;
			ptrdestg4 += 3;
			ptrdestb4 += 3;

		}

		/* Update src pointers */
		ptrsrcy1  += srcystride;
		ptrsrcy2  += srcystride;
		ptrsrcy3  += srcystride;
		ptrsrcy4  += srcystride;
		
		ptrsrccb1 += srcccstride;
		ptrsrccb2 += srcccstride;
	
		ptrsrccr1 += srcccstride;
		ptrsrccr2 += srcccstride;

		
		/* Update dest pointers */
		ptrdestr1 += destrgbstride;
		ptrdestr2 += destrgbstride;
		ptrdestr3 += destrgbstride;
		ptrdestr4 += destrgbstride;

		ptrdestg1 += destrgbstride;
		ptrdestg2 += destrgbstride;
		ptrdestg3 += destrgbstride;
		ptrdestg4 += destrgbstride;

		ptrdestb1 += destrgbstride;
		ptrdestb2 += destrgbstride;
		ptrdestb3 += destrgbstride;
		ptrdestb4 += destrgbstride;

	}


} // yuv420_prog_planar_to_rgb_packed.end

/*****************************************************************************

FileName: <rgbto420planar.cpp>

File Description:
	Modules to convert rgb to yuv420planar format
	List of Functions:

	rgb_planar_to_yuv420_prog_planar
	bgr_planar_to_yuv420_prog_planar
	rgb_packed_to_yuv420_prog_planar
	bgr_packed_to_yuv420_prog_planar

*****************************************************************************/

/*****************************************************************************
  Routine:  void rgb_planar_to_yuv420_prog_planar(
  								unsigned char *bufsrc, 
								unsigned char *bufdest, 
  								int width, int height, 
								)

        returns        	: void
        bufsrc         	: input buffer
        bufdest        	: output buffer
		width			: frame width
		height			: frame height
  Description          	: Converts rgb planar to yuv420 prog format 
*****************************************************************************/

void  rgb_planar_to_yuv420_prog_planar(
							unsigned char *bufsrc, 
							unsigned char *bufdest, 
							int width, int height
							)
{

#define RGB_PLANAR
#define SRC
#include "rgboffsets.h"
#undef SRC
#undef RGB_PLANAR 

#define YUV_420_PLANAR_PROG 
#define DEST
#include "yuv420offsets.h"
#undef DEST
#undef YUV_420_PLANAR_PROG

    int yr, yg, yb;
    int ur, ug, ub;
    int vr, vg, vb;
    int r, g, b;
    
	int i, j;

	for(j=0; j<(height/4); j++)
	{
		for(i=0;i<(width/2);i++)
		{

            /* First and Second Line */
            /* Top Left */
			r = (*ptrsrcr1++);
			g = (*ptrsrcg1++);
			b = (*ptrsrcb1++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
			ur = ur_tab[r];
			ug = ug_tab[g];
			ub = ub_tab[b];
			vr = vr_tab[r];
			vg = vg_tab[g];
			vb = vb_tab[b];
            (*ptrdestcb1++) = clp[384+((ur + ug + ub)>>16)+SHIFTCC];  
            (*ptrdestcr1++) = clp[384+((vr + vg + vb)>>16)+SHIFTCC];  
            (*ptrdesty1++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  

            /* Top Right */
			r = (*ptrsrcr1++);
			g = (*ptrsrcg1++);
			b = (*ptrsrcb1++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty1++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            
            /* Bottom Left */
			r = (*ptrsrcr2++);
			g = (*ptrsrcg2++);
			b = (*ptrsrcb2++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty2++) = clp[384+((yr + yg + yb)>>16)+SHIFTY]; 

            /* Bottom Right */
			r = (*ptrsrcr2++);
			g = (*ptrsrcg2++);
			b = (*ptrsrcb2++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty2++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];


            /* Third and Fourth Line */
            /* Top Left */
			r = (*ptrsrcr3++);
			g = (*ptrsrcg3++);
			b = (*ptrsrcb3++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
			ur = ur_tab[r];
			ug = ug_tab[g];
			ub = ub_tab[b];
			vr = vr_tab[r];
			vg = vg_tab[g];
			vb = vb_tab[b];
            (*ptrdestcb2++) = clp[384+((ur + ug + ub)>>16)+SHIFTCC];  
            (*ptrdestcr2++) = clp[384+((vr + vg + vb)>>16)+SHIFTCC];  
            (*ptrdesty3++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  

            /* Top Right */
			r = (*ptrsrcr3++);
			g = (*ptrsrcg3++);
			b = (*ptrsrcb3++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty3++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            
            /* Bottom Left */
			r = (*ptrsrcr4++);
			g = (*ptrsrcg4++);
			b = (*ptrsrcb4++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty4++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];

            /* Bottom Right */
			r = (*ptrsrcr4++);
			g = (*ptrsrcg4++);
			b = (*ptrsrcb4++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty4++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  

            
		}

		
		/* Update src pointers */
		ptrsrcr1 += srcrgbstride;
		ptrsrcr2 += srcrgbstride;
		ptrsrcr3 += srcrgbstride;
		ptrsrcr4 += srcrgbstride;

		ptrsrcg1 += srcrgbstride;
		ptrsrcg2 += srcrgbstride;
		ptrsrcg3 += srcrgbstride;
		ptrsrcg4 += srcrgbstride;

		ptrsrcb1 += srcrgbstride;
		ptrsrcb2 += srcrgbstride;
		ptrsrcb3 += srcrgbstride;
		ptrsrcb4 += srcrgbstride;


        /* Update dest pointers */
		ptrdesty1  += destystride;
		ptrdesty2  += destystride;
		ptrdesty3  += destystride;
		ptrdesty4  += destystride;
		
		ptrdestcb1 += destccstride;
		ptrdestcb2 += destccstride;
	
		ptrdestcr1 += destccstride;
		ptrdestcr2 += destccstride;

	}

} // rgb_planar_to_yuv420_prog_planar.end



/*****************************************************************************
  Routine:  void bgr_planar_to_yuv420_prog_planar(
  								unsigned char *bufsrc, 
								unsigned char *bufdest, 
  								int width, int height, 
								)

        returns        	: void
        bufsrc         	: input buffer
        bufdest        	: output buffer
		width			: frame width
		height			: frame height
  Description          	: Converts bgr planar to yuv420 prog format 
*****************************************************************************/

void  bgr_planar_to_yuv420_prog_planar(
							unsigned char *bufsrc, 
							unsigned char *bufdest, 
							int width, int height
							)
{

#define BGR_PLANAR
#define SRC
#include "rgboffsets.h"
#undef SRC
#undef BGR_PLANAR 

#define YUV_420_PLANAR_PROG 
#define DEST
#include "yuv420offsets.h"
#undef DEST
#undef YUV_420_PLANAR_PROG

    int yr, yg, yb;
    int ur, ug, ub;
    int vr, vg, vb;
    int r, g, b;
    
	int i, j;

	for(j=0; j<(height/4); j++)
	{
		for(i=0;i<(width/2);i++)
		{

            /* First and Second Line */
            /* Top Left */
			r = (*ptrsrcr1++);
			g = (*ptrsrcg1++);
			b = (*ptrsrcb1++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
			ur = ur_tab[r];
			ug = ug_tab[g];
			ub = ub_tab[b];
			vr = vr_tab[r];
			vg = vg_tab[g];
			vb = vb_tab[b];
            (*ptrdestcb1++) = clp[384+((ur + ug + ub)>>16)+SHIFTCC];  
            (*ptrdestcr1++) = clp[384+((vr + vg + vb)>>16)+SHIFTCC];  
            (*ptrdesty1++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  

            /* Top Right */
			r = (*ptrsrcr1++);
			g = (*ptrsrcg1++);
			b = (*ptrsrcb1++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty1++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            
            /* Bottom Left */
			r = (*ptrsrcr2++);
			g = (*ptrsrcg2++);
			b = (*ptrsrcb2++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty2++) = clp[384+((yr + yg + yb)>>16)+SHIFTY]; 

            /* Bottom Right */
			r = (*ptrsrcr2++);
			g = (*ptrsrcg2++);
			b = (*ptrsrcb2++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty2++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];


            /* Third and Fourth Line */
            /* Top Left */
			r = (*ptrsrcr3++);
			g = (*ptrsrcg3++);
			b = (*ptrsrcb3++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
			ur = ur_tab[r];
			ug = ug_tab[g];
			ub = ub_tab[b];
			vr = vr_tab[r];
			vg = vg_tab[g];
			vb = vb_tab[b];
            (*ptrdestcb2++) = clp[384+((ur + ug + ub)>>16)+SHIFTCC];  
            (*ptrdestcr2++) = clp[384+((vr + vg + vb)>>16)+SHIFTCC];  
            (*ptrdesty3++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  

            /* Top Right */
			r = (*ptrsrcr3++);
			g = (*ptrsrcg3++);
			b = (*ptrsrcb3++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty3++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            
            /* Bottom Left */
			r = (*ptrsrcr4++);
			g = (*ptrsrcg4++);
			b = (*ptrsrcb4++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty4++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];

            /* Bottom Right */
			r = (*ptrsrcr4++);
			g = (*ptrsrcg4++);
			b = (*ptrsrcb4++);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty4++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  

            
		}

		
		/* Update src pointers */
		ptrsrcr1 += srcbgrstride;
		ptrsrcr2 += srcbgrstride;
		ptrsrcr3 += srcbgrstride;
		ptrsrcr4 += srcbgrstride;

		ptrsrcg1 += srcbgrstride;
		ptrsrcg2 += srcbgrstride;
		ptrsrcg3 += srcbgrstride;
		ptrsrcg4 += srcbgrstride;

		ptrsrcb1 += srcbgrstride;
		ptrsrcb2 += srcbgrstride;
		ptrsrcb3 += srcbgrstride;
		ptrsrcb4 += srcbgrstride;


        /* Update dest pointers */
		ptrdesty1  += destystride;
		ptrdesty2  += destystride;
		ptrdesty3  += destystride;
		ptrdesty4  += destystride;
		
		ptrdestcb1 += destccstride;
		ptrdestcb2 += destccstride;
	
		ptrdestcr1 += destccstride;
		ptrdestcr2 += destccstride;

	}

} // bgr_planar_to_yuv420_prog_planar.end



/*****************************************************************************
  Routine:  void rgb_packed_to_yuv420_prog_planar(
  								unsigned char *bufsrc, 
								unsigned char *bufdest, 
  								int width, int height, 
								)

        returns        	: void
        bufsrc         	: input buffer
        bufdest        	: output buffer
		width			: frame width
		height			: frame height
  Description          	: Converts rgb packed to yuv420 prog format 
*****************************************************************************/

void  rgb_packed_to_yuv420_prog_planar(
							unsigned char *bufsrc, 
							unsigned char *bufdest, 
							int width, int height
							)
{

#define RGB_PACKED
#define SRC
#include "rgboffsets.h"
#undef SRC
#undef RGB_PACKED

#define YUV_420_PLANAR_PROG 
#define DEST
#include "yuv420offsets.h"
#undef DEST
#undef YUV_420_PLANAR_PROG

    int yr, yg, yb;
    int ur, ug, ub;
    int vr, vg, vb;
    int r, g, b;
    
	int i, j;

	for(j=0; j<(height/4); j++)
	{
		for(i=0;i<(width/2);i++)
		{

            /* First and Second Line */
            /* Top Left */
			r = (*ptrsrcr1);
			g = (*ptrsrcg1);
			b = (*ptrsrcb1);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
			ur = ur_tab[r];
			ug = ug_tab[g];
			ub = ub_tab[b];
			vr = vr_tab[r];
			vg = vg_tab[g];
			vb = vb_tab[b];
            (*ptrdestcb1++) = clp[384+((ur + ug + ub)>>16)+SHIFTCC];  
            (*ptrdestcr1++) = clp[384+((vr + vg + vb)>>16)+SHIFTCC];  
            (*ptrdesty1++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            ptrsrcr1 += 3; 
            ptrsrcg1 += 3;            
            ptrsrcb1 += 3;
            
            /* Top Right */
			r = (*ptrsrcr1);
			g = (*ptrsrcg1);
			b = (*ptrsrcb1);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty1++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            ptrsrcr1 += 3; 
            ptrsrcg1 += 3;            
            ptrsrcb1 += 3;
            
            /* Bottom Left */
			r = (*ptrsrcr2);
			g = (*ptrsrcg2);
			b = (*ptrsrcb2);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty2++) = clp[384+((yr + yg + yb)>>16)+SHIFTY]; 
            ptrsrcr2 += 3; 
            ptrsrcg2 += 3;            
            ptrsrcb2 += 3;

            /* Bottom Right */
			r = (*ptrsrcr2);
			g = (*ptrsrcg2);
			b = (*ptrsrcb2);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty2++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            ptrsrcr2 += 3; 
            ptrsrcg2 += 3;            
            ptrsrcb2 += 3;


            /* Third and Fourth Line */
            /* Top Left */
			r = (*ptrsrcr3);
			g = (*ptrsrcg3);
			b = (*ptrsrcb3);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
			ur = ur_tab[r];
			ug = ug_tab[g];
			ub = ub_tab[b];
			vr = vr_tab[r];
			vg = vg_tab[g];
			vb = vb_tab[b];
            (*ptrdestcb2++) = clp[384+((ur + ug + ub)>>16)+SHIFTCC];  
            (*ptrdestcr2++) = clp[384+((vr + vg + vb)>>16)+SHIFTCC];  
            (*ptrdesty3++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  
            ptrsrcr3 += 3; 
            ptrsrcg3 += 3;            
            ptrsrcb3 += 3;

            /* Top Right */
			r = (*ptrsrcr3);
			g = (*ptrsrcg3);
			b = (*ptrsrcb3);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty3++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            ptrsrcr3 += 3; 
            ptrsrcg3 += 3;            
            ptrsrcb3 += 3;
            
            /* Bottom Left */
			r = (*ptrsrcr4);
			g = (*ptrsrcg4);
			b = (*ptrsrcb4);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty4++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            ptrsrcr4 += 3; 
            ptrsrcg4 += 3;            
            ptrsrcb4 += 3;

            /* Bottom Right */
			r = (*ptrsrcr4);
			g = (*ptrsrcg4);
			b = (*ptrsrcb4);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty4++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  
            ptrsrcr4 += 3; 
            ptrsrcg4 += 3;            
            ptrsrcb4 += 3;

            
		}

		
		/* Update src pointers */
		ptrsrcr1 += srcrgbstride;
		ptrsrcr2 += srcrgbstride;
		ptrsrcr3 += srcrgbstride;
		ptrsrcr4 += srcrgbstride;

		ptrsrcg1 += srcrgbstride;
		ptrsrcg2 += srcrgbstride;
		ptrsrcg3 += srcrgbstride;
		ptrsrcg4 += srcrgbstride;

		ptrsrcb1 += srcrgbstride;
		ptrsrcb2 += srcrgbstride;
		ptrsrcb3 += srcrgbstride;
		ptrsrcb4 += srcrgbstride;


        /* Update dest pointers */
		ptrdesty1  += destystride;
		ptrdesty2  += destystride;
		ptrdesty3  += destystride;
		ptrdesty4  += destystride;
		
		ptrdestcb1 += destccstride;
		ptrdestcb2 += destccstride;
	
		ptrdestcr1 += destccstride;
		ptrdestcr2 += destccstride;

	}

} // rgb_packed_to_yuv420_prog_planar.end



/*****************************************************************************
  Routine:  void bgr_packed_to_yuv420_prog_planar(
  								unsigned char *bufsrc, 
								unsigned char *bufdest, 
  								int width, int height, 
								)

        returns        	: void
        bufsrc         	: input buffer
        bufdest        	: output buffer
		width			: frame width
		height			: frame height
  Description          	: Converts bgr packed to yuv420 prog format 
*****************************************************************************/

void  bgr_packed_to_yuv420_prog_planar(
							unsigned char *bufsrc, 
							unsigned char *bufdest, 
							int width, int height
							)
{

#define BGR_PACKED
#define SRC
#include "rgboffsets.h"
#undef SRC
#undef BGR_PACKED

#define YUV_420_PLANAR_PROG 
#define DEST
#include "yuv420offsets.h"
#undef DEST
#undef YUV_420_PLANAR_PROG

    int yr, yg, yb;
    int ur, ug, ub;
    int vr, vg, vb;
    int r, g, b;
    
	int i, j;

	for(j=0; j<(height/4); j++)
	{
		for(i=0;i<(width/2);i++)
		{

            /* First and Second Line */
            /* Top Left */
			r = (*ptrsrcr1);
			g = (*ptrsrcg1);
			b = (*ptrsrcb1);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
			ur = ur_tab[r];
			ug = ug_tab[g];
			ub = ub_tab[b];
			vr = vr_tab[r];
			vg = vg_tab[g];
			vb = vb_tab[b];
            (*ptrdestcb1++) = clp[384+((ur + ug + ub)>>16)+SHIFTCC];  
            (*ptrdestcr1++) = clp[384+((vr + vg + vb)>>16)+SHIFTCC];  
            (*ptrdesty1++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  
            ptrsrcr1 += 3; 
            ptrsrcg1 += 3;
            ptrsrcb1 += 3;

            /* Top Right */
			r = (*ptrsrcr1);
			g = (*ptrsrcg1);
			b = (*ptrsrcb1);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty1++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            ptrsrcr1 += 3; 
            ptrsrcg1 += 3;
            ptrsrcb1 += 3;
            
            /* Bottom Left */
			r = (*ptrsrcr2);
			g = (*ptrsrcg2);
			b = (*ptrsrcb2);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty2++) = clp[384+((yr + yg + yb)>>16)+SHIFTY]; 
            ptrsrcr2 += 3; 
            ptrsrcg2 += 3;
            ptrsrcb2 += 3;
            

            /* Bottom Right */
			r = (*ptrsrcr2);
			g = (*ptrsrcg2);
			b = (*ptrsrcb2);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty2++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            ptrsrcr2 += 3; 
            ptrsrcg2 += 3;
            ptrsrcb2 += 3;
            


            /* Third and Fourth Line */
            /* Top Left */
			r = (*ptrsrcr3);
			g = (*ptrsrcg3);
			b = (*ptrsrcb3);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
			ur = ur_tab[r];
			ug = ug_tab[g];
			ub = ub_tab[b];
			vr = vr_tab[r];
			vg = vg_tab[g];
			vb = vb_tab[b];
            (*ptrdestcb2++) = clp[384+((ur + ug + ub)>>16)+SHIFTCC];  
            (*ptrdestcr2++) = clp[384+((vr + vg + vb)>>16)+SHIFTCC];  
            (*ptrdesty3++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  
            ptrsrcr3 += 3; 
            ptrsrcg3 += 3;
            ptrsrcb3 += 3;
            

            /* Top Right */
			r = (*ptrsrcr3);
			g = (*ptrsrcg3);
			b = (*ptrsrcb3);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty3++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            ptrsrcr3 += 3; 
            ptrsrcg3 += 3;
            ptrsrcb3 += 3;
            
            /* Bottom Left */
			r = (*ptrsrcr4);
			g = (*ptrsrcg4);
			b = (*ptrsrcb4);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty4++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];
            ptrsrcr4 += 3; 
            ptrsrcg4 += 3;
            ptrsrcb4 += 3;

            /* Bottom Right */
			r = (*ptrsrcr4);
			g = (*ptrsrcg4);
			b = (*ptrsrcb4);
			yr = yr_tab[r];
			yg = yg_tab[g];
			yb = yb_tab[b];
            (*ptrdesty4++) = clp[384+((yr + yg + yb)>>16)+SHIFTY];  
            ptrsrcr4 += 3; 
            ptrsrcg4 += 3;
            ptrsrcb4 += 3;
            
		}

		
		/* Update src pointers */
		ptrsrcr1 += srcbgrstride;
		ptrsrcr2 += srcbgrstride;
		ptrsrcr3 += srcbgrstride;
		ptrsrcr4 += srcbgrstride;

		ptrsrcg1 += srcbgrstride;
		ptrsrcg2 += srcbgrstride;
		ptrsrcg3 += srcbgrstride;
		ptrsrcg4 += srcbgrstride;

		ptrsrcb1 += srcbgrstride;
		ptrsrcb2 += srcbgrstride;
		ptrsrcb3 += srcbgrstride;
		ptrsrcb4 += srcbgrstride;


        /* Update dest pointers */
		ptrdesty1  += destystride;
		ptrdesty2  += destystride;
		ptrdesty3  += destystride;
		ptrdesty4  += destystride;
		
		ptrdestcb1 += destccstride;
		ptrdestcb2 += destccstride;
	
		ptrdestcr1 += destccstride;
		ptrdestcr2 += destccstride;

	}

} // bgr_packed_to_yuv420_prog_planar.end


#endif //for OPTEE_YUVCONVERTER_H