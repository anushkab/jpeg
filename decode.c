/*
+--------------------------------------------------------------------------+
| CHStone : a suite of benchmark programs for C-based High-Level Synthesis |
| ======================================================================== |
|                                                                          |
| * Collected and Modified : Y. Hara, H. Tomiyama, S. Honda,               |
|                            H. Takada and K. Ishii                        |
|                            Nagoya University, Japan                      |
|                                                                          |
| * Remark :                                                               |
|    1. This source code is modified to unify the formats of the benchmark |
|       programs in CHStone.                                               |
|    2. Test vectors are added for CHStone.                                |
|    3. If "main_result" is 0 at the end of the program, the program is    |
|       correctly executed.                                                |
|    4. Please follow the copyright of each benchmark program.             |
+--------------------------------------------------------------------------+
*/
/*************************************************************
Copyright (C) 1990, 1991, 1993 Andy C. Hung, all rights reserved.
PUBLIC DOMAIN LICENSE: Stanford University Portable Video Research
Group. If you use this software, you agree to the following: This
program package is purely experimental, and is licensed "as is".
Permission is granted to use, modify, and distribute this program
without charge for any purpose, provided this license/ disclaimer
notice appears in the copies.  No warranty or maintenance is given,
either expressed or implied.  In no event shall the author(s) be
liable to you or a third party for any special, incidental,
consequential, or other damages, arising out of the use or inability
to use the program for any purpose (or the loss of data), even if we
have been advised of such possibilities.  Any public reference or
advertisement of this source code should refer to it as the Portable
Video Research Group (PVRG) code, and not by any author(s) (or
Stanford University) name.
*************************************************************/

/*
************************************************************
decode.c (original: transform.c)

This file contains the reference DCT, the zig-zag and quantization
algorithms.

************************************************************
*/
/*
 *  Decoder
 *
 *  @(#) $Id: decode.c,v 1.2 2003/07/18 10:19:21 honda Exp $
 */

#include "decode.h"
#include "init.h"
#include <stdio.h>

//void ChenIDct (int *x, int *y);
void ChenIDct_f2r_ChenIDct (int x[DCTSIZE2], int y[DCTSIZE2]);

//const int zigzag_index[64] =	/* Is zig-zag map for matrix -> scan array */
//{ 0, 1, 5, 6, 14, 15, 27, 28,
//  2, 4, 7, 13, 16, 26, 29, 42,
//  3, 8, 12, 17, 25, 30, 41, 43,
//  9, 11, 18, 24, 31, 40, 44, 53,
//  10, 19, 23, 32, 39, 45, 52, 54,
//  20, 22, 33, 38, 46, 51, 55, 60,
//  21, 34, 37, 47, 50, 56, 59, 61,
//  35, 36, 48, 49, 57, 58, 62, 63
//};


/*
 * IZigzagMatrix() performs an inverse zig-zag translation on the
 * input imatrix and places the output in omatrix.
 */
void
//IZigzagMatrix_f2r_IZigzagMatrix (int *imatrix, int *omatrix)
IZigzagMatrix_f2r_IZigzagMatrix (int imatrix[DCTSIZE2], int omatrix[DCTSIZE2])
{
  int i;

  const int zigzag_index[64] =	/* Is zig-zag map for matrix -> scan array */
  { 0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
  };

  for (i = 0; i < DCTSIZE2; i++)
    
    {
      
*(omatrix++) = imatrix[zigzag_index[i]];
    
}
}


/*
 * IQuantize() takes an input matrix and does an inverse quantization
 * and puts the output int qmatrix.
 */
void
IQuantize_f2r_IQuantize (int matrix[DCTSIZE2], unsigned int qmatrix[DCTSIZE2])
{
  int *mptr;

  for (mptr = matrix; mptr < matrix + DCTSIZE2; mptr++)
    {
      *mptr = *mptr * (*qmatrix);
      qmatrix++;
    }
}


/*
 * PostshiftIDctMatrix() adds 128 (2048) to all 64 elements of an 8x8 matrix.
 * This results in strictly positive values for all pixel coefficients.
 */
void
PostshiftIDctMatrix (int matrix[DCTSIZE2], int shift)
{
  int *mptr;
  for (mptr = matrix; mptr < matrix + DCTSIZE2; mptr++)
    {
      *mptr += shift;
    }
}


/*
 * BoundIDctMatrix bounds the inverse dct matrix so that no pixel has a
 * value greater than 255 (4095) or less than 0.
 */
void
BoundIDctMatrix_f2r_BoundIDctMatrix (int matrix[DCTSIZE2], int Bound)
{
  int *mptr;

  for (mptr = matrix; mptr < matrix + DCTSIZE2; mptr++)
    {
      if (*mptr < 0)
	{
	  *mptr = 0;
	}
      else if (*mptr > Bound)
	{
	  *mptr = Bound;
	}
    }
}



void
WriteOneBlock_f2r_WriteOneBlock (int store[DCTSIZE2], unsigned char out_buf[DCTSIZE2], int width, int height,
	       int voffs, int hoffs)
{
  int i, e;


  /* Find vertical buffer offs. */
  loop_1:for (i = voffs; i < voffs + DCTSIZE; i++)
    {
      if (i < height)
	{
	  int diff;
	  diff = width * i;
	  for (e = hoffs; e < hoffs + DCTSIZE; e++)
	    {
	      if (e < width)
		{
		  out_buf[diff + e] = (unsigned char) (*(store++));
		}
	      else
		{
		  break;
		}
	    }
	}
      else
	{
	  break;
	}
    }


}

/*
 * WriteBlock() writes an array of data in the integer array pointed to
 * by store out to the driver specified by the IOB.  The integer array is
 * stored in row-major form, that is, the first row of (8) elements, the
 * second row of (8) elements....
 * ONLY for MCU 1:1:1
 */
void
WriteBlock_f2r_WriteBlock (int store[DCTSIZE2], int *p_out_vpos, int *p_out_hpos,
	    unsigned char p_out_buf[DCTSIZE2], short p_jinfo_image_width, short p_jinfo_image_height, int p_jinfo_MCUWidth)
{
  int voffs, hoffs;

  /*
   * Get vertical offsets
   */
  voffs = *p_out_vpos * DCTSIZE;
  hoffs = *p_out_hpos * DCTSIZE;

  /*
   * Write block
   */
  WriteOneBlock_f2r_WriteOneBlock (store,
		 p_out_buf,
		 p_jinfo_image_width, p_jinfo_image_height, voffs, hoffs);

  /*
   *  Add positions
   */
	(*p_out_hpos)++;
	(*p_out_vpos)++;

  if (*p_out_hpos < p_jinfo_MCUWidth)
    {
    (*p_out_vpos)--;
    }
  else
    {
      *p_out_hpos = 0;		/* If at end of image (width) */
    }
}

/*
 *  4:1:1
 */
void
Write4Blocks_f2r_Write4Blocks(int store1[DCTSIZE2], int store2[DCTSIZE2], int store3[DCTSIZE2], int store4[DCTSIZE2],
	      int *p_out_vpos, int *p_out_hpos, unsigned char p_out_buf[DCTSIZE2],
	      short p_jinfo_image_width, short p_jinfo_image_height, int p_jinfo_MCUWidth)
{
  int voffs, hoffs;

  /*
   * OX
   * XX
   */
  voffs = *p_out_vpos * DCTSIZE;
  hoffs = *p_out_hpos * DCTSIZE;
  WriteOneBlock_f2r_WriteOneBlock (store1, p_out_buf,
		 p_jinfo_image_width, p_jinfo_image_height, voffs, hoffs);

  /*
   * XO
   * XX
   */
  hoffs += DCTSIZE;
  WriteOneBlock_f2r_WriteOneBlock (store2, p_out_buf,
		 p_jinfo_image_width, p_jinfo_image_height, voffs, hoffs);

  /*
   * XX
   * OX
   */
  voffs += DCTSIZE;
  hoffs -= DCTSIZE;
  WriteOneBlock_f2r_WriteOneBlock (store3, p_out_buf,
		 p_jinfo_image_width, p_jinfo_image_height, voffs, hoffs);


  /*
   * XX
   * XO
   */
  hoffs += DCTSIZE;
  WriteOneBlock_f2r_WriteOneBlock (store4,
		 p_out_buf, p_jinfo_image_width, p_jinfo_image_height,
		 voffs, hoffs);

  /*
   * Add positions
   */
  *p_out_hpos = *p_out_hpos + 2;
  *p_out_vpos = *p_out_vpos + 2;


  if (*p_out_hpos < p_jinfo_MCUWidth)
    {
      *p_out_vpos = *p_out_vpos - 2;
    }
  else
    {
      *p_out_hpos = 0;		/* If at end of image (width) */
    }
}


/*
 * Transform from Yuv into RGB
 */
void
YuvToRgb_f2r_YuvToRgb (int p, int y_buf[DCTSIZE2], int u_buf[DCTSIZE2], int v_buf[DCTSIZE2], int rgb_buf[4][RGB_NUM][DCTSIZE2])
{
  int r, g, b;
  int y, u, v;
  int i;

  for (i = 0; i < DCTSIZE2; i++)
    {
      y = y_buf[i];
      u = u_buf[i] - 128;
      v = v_buf[i] - 128;

      r = (y * 256 + v * 359 + 128) >> 8;
      g = (y * 256 - u * 88 - v * 182 + 128) >> 8;
      b = (y * 256 + u * 454 + 128) >> 8;

      if (r < 0)
	r = 0;
      else if (r > 255)
	r = 255;

      if (g < 0)
	g = 0;
      else if (g > 255)
	g = 255;

      if (b < 0)
	b = 0;
      else if (b > 255)
	b = 255;

      rgb_buf[p][0][i] = r;
      rgb_buf[p][1][i] = g;
      rgb_buf[p][2][i] = b;

    }
}


/*
 * Decode one block
 */
void
decode_block (int comp_no, int out_buf[DCTSIZE2], int HuffBuff[DCTSIZE2], unsigned int p_jinfo_quant_tbl_quantval[NUM_QUANT_TBLS][DCTSIZE2], char p_jinfo_comps_info_quant_tbl_no[NUM_COMPONENT],
		char p_jinfo_comps_info_dc_tbl_no[NUM_COMPONENT], int p_jinfo_dc_xhuff_tbl_huffval[NUM_HUFF_TBLS][257],
		int p_jinfo_dc_dhuff_tbl_ml[NUM_HUFF_TBLS], int p_jinfo_dc_dhuff_tbl_maxcode[NUM_HUFF_TBLS][36], int p_jinfo_dc_dhuff_tbl_mincode[NUM_HUFF_TBLS][36], int p_jinfo_dc_dhuff_tbl_valptr[NUM_HUFF_TBLS][36],
		unsigned char CurHuffReadBuf[JPEG_FILE_SIZE], int p_jinfo_ac_xhuff_tbl_huffval[NUM_HUFF_TBLS][257], int p_jinfo_ac_dhuff_tbl_ml[NUM_HUFF_TBLS], int p_jinfo_ac_dhuff_tbl_maxcode[NUM_HUFF_TBLS][36], int p_jinfo_ac_dhuff_tbl_mincode[NUM_HUFF_TBLS][36],
		int p_jinfo_ac_dhuff_tbl_valptr[NUM_HUFF_TBLS][36])
{
  int QuantBuff[DCTSIZE2];
  unsigned int *p_quant_tbl;

  DecodeHuffMCU (HuffBuff, comp_no, p_jinfo_comps_info_dc_tbl_no, p_jinfo_dc_xhuff_tbl_huffval,
		  p_jinfo_dc_dhuff_tbl_ml, p_jinfo_dc_dhuff_tbl_maxcode, p_jinfo_dc_dhuff_tbl_mincode, p_jinfo_dc_dhuff_tbl_valptr, CurHuffReadBuf,
		  p_jinfo_ac_xhuff_tbl_huffval, p_jinfo_ac_dhuff_tbl_ml, p_jinfo_ac_dhuff_tbl_maxcode, p_jinfo_ac_dhuff_tbl_mincode,
		  p_jinfo_ac_dhuff_tbl_valptr);

  IZigzagMatrix_f2r_IZigzagMatrix (HuffBuff, QuantBuff);

  p_quant_tbl =
    &p_jinfo_quant_tbl_quantval[(int)p_jinfo_comps_info_quant_tbl_no[comp_no]][DCTSIZE2];
  IQuantize_f2r_IQuantize (QuantBuff, p_quant_tbl);

  ChenIDct_f2r_ChenIDct (QuantBuff, out_buf);
  //ChenIDct (*QuantBuff, *out_buf);

  PostshiftIDctMatrix (out_buf, IDCT_SHIFT);

  BoundIDctMatrix_f2r_BoundIDctMatrix (out_buf, IDCT_BOUNT);

}


void
decode_start (int *out_data_image_width, int *out_data_image_height,
	      int out_data_comp_vpos[RGB_NUM], int out_data_comp_hpos[RGB_NUM],
	      unsigned int p_jinfo_quant_tbl_quantval[NUM_QUANT_TBLS][DCTSIZE2], char p_jinfo_comps_info_quant_tbl_no[NUM_COMPONENT],
	      unsigned char CurHuffReadBuf[JPEG_FILE_SIZE], unsigned char p_jinfo_jpeg_data[JPEG_FILE_SIZE], short p_jinfo_image_width, short p_jinfo_image_height, int p_jinfo_MCUWidth,
	      int p_jinfo_smp_fact, int p_jinfo_NumMCU,
	      char p_jinfo_comps_info_dc_tbl_no[NUM_COMPONENT], int p_jinfo_dc_xhuff_tbl_huffval[NUM_HUFF_TBLS][257], int p_jinfo_dc_dhuff_tbl_ml[NUM_HUFF_TBLS],
	      int p_jinfo_dc_dhuff_tbl_maxcode[NUM_HUFF_TBLS][36], int p_jinfo_dc_dhuff_tbl_mincode[NUM_HUFF_TBLS][36], int p_jinfo_dc_dhuff_tbl_valptr[NUM_HUFF_TBLS][36],
	      int rgb_buf[4][RGB_NUM][DCTSIZE2], unsigned char OutData_comp_buf[RGB_NUM][BMP_OUT_SIZE], int p_jinfo_ac_xhuff_tbl_huffval[NUM_HUFF_TBLS][257], int p_jinfo_ac_dhuff_tbl_ml[NUM_HUFF_TBLS], int p_jinfo_ac_dhuff_tbl_maxcode[NUM_HUFF_TBLS][36], int p_jinfo_ac_dhuff_tbl_mincode[NUM_HUFF_TBLS][36],
			int p_jinfo_ac_dhuff_tbl_valptr[NUM_HUFF_TBLS][36])
{
  int i;
  int CurrentMCU = 0;
  int HuffBuff[NUM_COMPONENT][DCTSIZE2];
  int IDCTBuff[6][DCTSIZE2];

  /* Read buffer */
  CurHuffReadBuf = p_jinfo_jpeg_data;

  /*
   * Initial value of DC element is 0
   */
  loop_1:for (i = 0; i < NUM_COMPONENT; i++)
    {
      HuffBuff[i][0] = 0;
    }

  /*
   * Set the size of image to output buffer
   */
  *out_data_image_width = p_jinfo_image_width;
  *out_data_image_height = p_jinfo_image_height;

  /*
   * Initialize output buffer
   */
  for (i = 0; i < RGB_NUM; i++)
    {
      out_data_comp_vpos[i] = 0;
      out_data_comp_hpos[i] = 0;
    }


  if (p_jinfo_smp_fact == SF1_1_1)
    {
      printf ("Decode 1:1:1 NumMCU = %d\n", p_jinfo_NumMCU);

      /*
       * 1_1_1
       */
      while (CurrentMCU < p_jinfo_NumMCU)
	{

	  for (i = 0; i < NUM_COMPONENT; i++)
	    {
	      decode_block (i, IDCTBuff[i], HuffBuff[i], p_jinfo_quant_tbl_quantval, p_jinfo_comps_info_quant_tbl_no, p_jinfo_comps_info_dc_tbl_no,
	    		  p_jinfo_dc_xhuff_tbl_huffval, p_jinfo_dc_dhuff_tbl_ml, p_jinfo_dc_dhuff_tbl_maxcode, p_jinfo_dc_dhuff_tbl_mincode, p_jinfo_dc_dhuff_tbl_valptr, CurHuffReadBuf,
	    		  p_jinfo_ac_xhuff_tbl_huffval, p_jinfo_ac_dhuff_tbl_ml, p_jinfo_ac_dhuff_tbl_maxcode, p_jinfo_ac_dhuff_tbl_mincode, p_jinfo_ac_dhuff_tbl_valptr);
	    }


	  YuvToRgb_f2r_YuvToRgb (0, IDCTBuff[0], IDCTBuff[1], IDCTBuff[2], rgb_buf);
	  /*
	   * Write
	   */
	  for (i = 0; i < RGB_NUM; i++)
	    {
	      WriteBlock_f2r_WriteBlock (&rgb_buf[0][i][0],
			  &out_data_comp_vpos[i],
			  &out_data_comp_hpos[i], &OutData_comp_buf[i][0], p_jinfo_image_width, p_jinfo_image_height, p_jinfo_MCUWidth);
	    }
	  CurrentMCU++;
	}

    }
  else
    {
      printf ("Decode 4:1:1 NumMCU = %d\n", p_jinfo_NumMCU);
      /*
       * 4_1_1
       */
      while (CurrentMCU < p_jinfo_NumMCU)
	{
	  /*
	   * Decode Y element
	   * Decoding Y, U and V elements should be sequentially conducted for the use of Huffman table
	   */

	  for (i = 0; i < 4; i++)
	    {
	      decode_block (0, IDCTBuff[i], HuffBuff[0], p_jinfo_quant_tbl_quantval, p_jinfo_comps_info_quant_tbl_no, p_jinfo_comps_info_dc_tbl_no,
	    		  p_jinfo_dc_xhuff_tbl_huffval, p_jinfo_dc_dhuff_tbl_ml, p_jinfo_dc_dhuff_tbl_maxcode, p_jinfo_dc_dhuff_tbl_mincode, p_jinfo_dc_dhuff_tbl_valptr, CurHuffReadBuf,
	    		  p_jinfo_ac_xhuff_tbl_huffval, p_jinfo_ac_dhuff_tbl_ml, p_jinfo_ac_dhuff_tbl_maxcode, p_jinfo_ac_dhuff_tbl_mincode, p_jinfo_ac_dhuff_tbl_valptr);
	    }

	  /* Decode U */
	  decode_block (1, IDCTBuff[4], HuffBuff[1], p_jinfo_quant_tbl_quantval, p_jinfo_comps_info_quant_tbl_no, p_jinfo_comps_info_dc_tbl_no,
			  p_jinfo_dc_xhuff_tbl_huffval, p_jinfo_dc_dhuff_tbl_ml, p_jinfo_dc_dhuff_tbl_maxcode, p_jinfo_dc_dhuff_tbl_mincode, p_jinfo_dc_dhuff_tbl_valptr, CurHuffReadBuf,
			  p_jinfo_ac_xhuff_tbl_huffval, p_jinfo_ac_dhuff_tbl_ml, p_jinfo_ac_dhuff_tbl_maxcode, p_jinfo_ac_dhuff_tbl_mincode, p_jinfo_ac_dhuff_tbl_valptr);

	  /* Decode V */
	  decode_block (2, IDCTBuff[5], HuffBuff[2], p_jinfo_quant_tbl_quantval, p_jinfo_comps_info_quant_tbl_no, p_jinfo_comps_info_dc_tbl_no,
			  p_jinfo_dc_xhuff_tbl_huffval, p_jinfo_dc_dhuff_tbl_ml, p_jinfo_dc_dhuff_tbl_maxcode, p_jinfo_dc_dhuff_tbl_mincode, p_jinfo_dc_dhuff_tbl_valptr, CurHuffReadBuf,
			  p_jinfo_ac_xhuff_tbl_huffval, p_jinfo_ac_dhuff_tbl_ml, p_jinfo_ac_dhuff_tbl_maxcode, p_jinfo_ac_dhuff_tbl_mincode, p_jinfo_ac_dhuff_tbl_valptr);


	  /* Transform from Yuv into RGB */

	  for (i = 0; i < 4; i++)
	    {
	      YuvToRgb_f2r_YuvToRgb (i, IDCTBuff[i], IDCTBuff[4], IDCTBuff[5], rgb_buf);
	    }


	  for (i = 0; i < RGB_NUM; i++)
	    {
	      Write4Blocks_f2r_Write4Blocks (&rgb_buf[0][i][0],
			    &rgb_buf[1][i][0],
			    &rgb_buf[2][i][0],
			    &rgb_buf[3][i][0],
			    &out_data_comp_vpos[i],
			    &out_data_comp_hpos[i], &OutData_comp_buf[i][0],
			    p_jinfo_image_width, p_jinfo_image_height, p_jinfo_MCUWidth);
	    }


	  CurrentMCU += 4;
	}
    }
}
