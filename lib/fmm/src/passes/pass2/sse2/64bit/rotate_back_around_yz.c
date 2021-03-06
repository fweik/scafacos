/*
 * Copyright (C) 2012 Ivo Kabadshow, Holger Dachsel
 *
 * This file is part of ScaFaCoS.
 *
 * ScaFaCoS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ScaFaCoS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *     
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <xmmintrin.h>

void rotate_back_around_yz(long long nmultipoles,
                           double *csmphi, double *csmphipi,
                           double *scr1, double *scr2,
                           double *d3f,
                           long long *jaddress)
{
  int m,mm,mmm;
  int n,nn;
  int i,j,k,l;
  /* TODO rename masks according its value*/
  /* TODO only allow SSE routines for SSE processors and aligned alloc and double precision */
  /* TODO write SSE kernel for single precision */

  __m128d reg00,reg01,reg02,reg03;
  __m128d reg04,reg05,reg06,reg07;
  __m128d reg08,reg09,reg10,reg11;
  __m128d reg12,reg13,reg14,reg15;
  __m128d reg16,reg17;                   /* register for csmphi */
  __m128d reg18,reg19,reg20;             /* gl,g,glm */
  __m128d reg21,reg22,reg23,reg24;       /* csmphi, csmphipi */

  const __m128d regzero = _mm_setzero_pd();
  const __m128d regone = _mm_set_pd(1.0e0l,1.0e0l);

/*
  reg00 = _mm_load_pd(&scr1[0]);
  _mm_store_sd(&scr2[jaddress[ 0]],reg00);
  reg00 = _mm_shuffle_pd(reg00,reg00,0x3);
  _mm_store_sd(&scr2[jaddress[ 1]],reg00);

  reg01 = _mm_load_pd(&scr1[2]);
  _mm_store_sd(&scr2[jaddress[ 2]],reg01);
  reg01 = _mm_shuffle_pd(reg01,reg01,0x3);
  _mm_store_sd(&scr2[jaddress[ 3]],reg01);

  reg02 = _mm_load_pd(&scr1[4]);
  _mm_store_sd(&scr2[jaddress[ 4]],reg02);
  reg02 = _mm_shuffle_pd(reg02,reg02,0x3);
  _mm_store_sd(&scr2[jaddress[ 5]],reg02);

  reg03 = _mm_load_pd(&scr1[6]);
  _mm_store_sd(&scr2[jaddress[ 6]],reg03);
  reg03 = _mm_shuffle_pd(reg03,reg03,0x3);
  _mm_store_sd(&scr2[jaddress[ 7]],reg03);

  reg04 = _mm_load_pd(&scr1[8]);
  _mm_store_sd(&scr2[jaddress[ 8]],reg04);
  reg04 = _mm_shuffle_pd(reg04,reg04,0x3);
  _mm_store_sd(&scr2[jaddress[ 9]],reg04);

  reg05 = _mm_load_pd(&scr1[10]);
  _mm_store_sd(&scr2[jaddress[10]],reg05);
  reg05 = _mm_shuffle_pd(reg05,reg05,0x3);
  _mm_store_sd(&scr2[jaddress[11]],reg05);

  reg06 = _mm_load_pd(&scr1[12]);
  _mm_store_sd(&scr2[jaddress[12]],reg06);
  reg06 = _mm_shuffle_pd(reg06,reg06,0x3);
  _mm_store_sd(&scr2[jaddress[13]],reg06);

  reg07 = _mm_load_pd(&scr1[14]);
  _mm_store_sd(&scr2[jaddress[14]],reg07);
  reg07 = _mm_shuffle_pd(reg07,reg07,0x3);
  _mm_store_sd(&scr2[jaddress[15]],reg07);
*/

  scr2[jaddress[ 0]] = scr1[ 0];
  scr2[jaddress[ 1]] = scr1[ 1];
  scr2[jaddress[ 2]] = scr1[ 2];
  scr2[jaddress[ 3]] = scr1[ 3];

  scr2[jaddress[ 4]] = scr1[ 4];
  scr2[jaddress[ 5]] = scr1[ 5];
  scr2[jaddress[ 6]] = scr1[ 6];
  scr2[jaddress[ 7]] = scr1[ 7];

  scr2[jaddress[ 8]] = scr1[ 8];
  scr2[jaddress[ 9]] = scr1[ 9];
  scr2[jaddress[10]] = scr1[10];
  scr2[jaddress[11]] = scr1[11];

  scr2[jaddress[12]] = scr1[12];
  scr2[jaddress[13]] = scr1[13];
  scr2[jaddress[14]] = scr1[14];
  scr2[jaddress[15]] = scr1[15];


  mmm = 0;
  i = 0;
  j = 1;

  reg18 = regone;
  
  for(l=1;l<=nmultipoles;++l)
  {
    reg18 = _mm_sub_pd(regzero,reg18);

    i += 1;
    j += 16 * l;
    n = j;

    //mmm += 1;

    reg00 = _mm_load_pd(&scr1[n-1]);
    reg01 = _mm_load_pd(&scr1[n+1]);
    reg04 = _mm_load_pd(&scr1[n+7]);
    reg05 = _mm_load_pd(&scr1[n+9]);

    reg16 = _mm_load1_pd(&d3f[mmm]);

    reg08 = _mm_mul_pd(reg00,reg16); /* reg16 = d3f[mmm] */
    reg09 = _mm_mul_pd(reg01,reg16);

    reg04 = _mm_mul_pd(reg04,reg18); /* reg18 = gl*/
    reg12 = _mm_mul_pd(reg04,reg16);

    reg05 = _mm_mul_pd(reg05,reg18);
    reg13 = _mm_mul_pd(reg05,reg16);

    //mmm += 1;
    mmm += 2;

    nn = n + 16;
    n += 16 * l;

    //g = gl;
    reg19 = reg18;
    for(k=nn;k<=n;k+=16)
    {
      reg19 = _mm_sub_pd(regzero,reg19);

      //mmm += 1;

      reg00 = _mm_load_pd(&scr1[k-1]);
      reg01 = _mm_load_pd(&scr1[k+1]);
      reg04 = _mm_load_pd(&scr1[k+7]);
      reg05 = _mm_load_pd(&scr1[k+9]);

      reg16 = _mm_load1_pd(&d3f[mmm]);

      reg08 = _mm_add_pd(reg08,_mm_mul_pd(reg00,reg16));
      reg09 = _mm_add_pd(reg09,_mm_mul_pd(reg01,reg16));

      reg04 = _mm_mul_pd(reg04,reg19);
      reg12 = _mm_add_pd(reg12,_mm_mul_pd(reg04,reg16));

      reg05 = _mm_mul_pd(reg05,reg19);
      reg13 = _mm_add_pd(reg13,_mm_mul_pd(reg05,reg16));

      mmm += 2;
    }

    _mm_store_sd(&scr2[jaddress[ 0]+i],reg08); /* store lower 64 bits to unaligned scr2[...] */
    reg08 = _mm_shuffle_pd(reg08,reg08,0x3);
    _mm_store_sd(&scr2[jaddress[ 1]+i],reg08);

    _mm_store_sd(&scr2[jaddress[ 2]+i],reg09);
    reg09 = _mm_shuffle_pd(reg09,reg09,0x3);
    _mm_store_sd(&scr2[jaddress[ 3]+i],reg09);

    _mm_store_sd(&scr2[jaddress[ 4]+i],regzero);
    _mm_store_sd(&scr2[jaddress[ 5]+i],regzero);

    _mm_store_sd(&scr2[jaddress[ 6]+i],regzero);
    _mm_store_sd(&scr2[jaddress[ 7]+i],regzero);

    _mm_store_sd(&scr2[jaddress[ 8]+i],reg12);
    reg12 = _mm_shuffle_pd(reg12,reg12,0x3);
    _mm_store_sd(&scr2[jaddress[ 9]+i],reg12);

    _mm_store_sd(&scr2[jaddress[10]+i],reg13);
    reg13 = _mm_shuffle_pd(reg13,reg13,0x3);
    _mm_store_sd(&scr2[jaddress[11]+i],reg13);

    _mm_store_sd(&scr2[jaddress[12]+i],regzero);
    _mm_store_sd(&scr2[jaddress[13]+i],regzero);

    _mm_store_sd(&scr2[jaddress[14]+i],regzero);
    _mm_store_sd(&scr2[jaddress[15]+i],regzero);

    //reg20 = reg18;
    reg20 = _mm_sub_pd(regzero,reg18);
    
    for(m=1;m<=2*l;m+=2)
    {
      //reg20 = _mm_sub_pd(regzero,reg18);

      i += 1;
      n = j;

      reg00 = _mm_load_pd(&scr1[n- 1]);
      reg01 = _mm_load_pd(&scr1[n+ 1]);
      reg02 = _mm_load_pd(&scr1[n+ 3]);
      reg03 = _mm_load_pd(&scr1[n+ 5]);
      reg04 = _mm_load_pd(&scr1[n+ 7]);
      reg05 = _mm_load_pd(&scr1[n+ 9]);
      reg06 = _mm_load_pd(&scr1[n+11]);
      reg07 = _mm_load_pd(&scr1[n+13]);

//      reg16 = _mm_load_pd(&d3f[mmm+1]);
      reg16 = _mm_load_pd(&d3f[mmm]);
      /* generate imag part in reg17=[imag,imag]
       * generate reg17=[a1,a1] from [a0,a1] and [a0,a1]  with mask = 3*/
      reg17 = _mm_shuffle_pd(reg16, reg16, 0x3);

      /* generate real part in reg17=[real, real]
       * generate reg16=[a0,a0] from [a0,a1] and [a0,a1]  with mask = 0*/
      reg16 = _mm_shuffle_pd(reg16, reg16, 0x0);

      reg08 = _mm_mul_pd(reg00,reg16);
      reg09 = _mm_mul_pd(reg01,reg16);
      reg10 = _mm_mul_pd(reg02,reg17);
      reg11 = _mm_mul_pd(reg03,reg17);

      reg04 = _mm_mul_pd(reg04,reg20);
      reg12 = _mm_mul_pd(reg04,reg16);
      reg05 = _mm_mul_pd(reg05,reg20);
      reg13 = _mm_mul_pd(reg05,reg16);

      reg20 = _mm_sub_pd(regzero,reg20);

      reg06 = _mm_mul_pd(reg06,reg20);
      reg14 = _mm_mul_pd(reg06,reg17);
      reg07 = _mm_mul_pd(reg07,reg20);
      reg15 = _mm_mul_pd(reg07,reg17);

      mmm += 2;
      nn = n + 16;
      n += 16 * l;

      //reg20 = _mm_sub_pd(regzero,reg20);
      //reg19 = reg20;
      reg19 = reg20;
      
      for(k=nn;k<=n;k+=16)
      {
        //reg19 = _mm_sub_pd(regzero,reg19);

        reg00 = _mm_load_pd(&scr1[k- 1]);
        reg01 = _mm_load_pd(&scr1[k+ 1]);
        reg02 = _mm_load_pd(&scr1[k+ 3]);
        reg03 = _mm_load_pd(&scr1[k+ 5]);
        reg04 = _mm_load_pd(&scr1[k+ 7]);
        reg05 = _mm_load_pd(&scr1[k+ 9]);
        reg06 = _mm_load_pd(&scr1[k+11]);
        reg07 = _mm_load_pd(&scr1[k+13]);

//        reg16 = _mm_load_pd(&d3f[mmm+1]);
        reg16 = _mm_load_pd(&d3f[mmm]);	
        /* generate imag part in reg17=[imag,imag]
         * generate reg17=[a1,a1] from [a0,a1] and [a0,a1]  with mask = 3*/
        reg17 = _mm_shuffle_pd(reg16, reg16, 0x3);

        /* generate real part in reg17=[real, real]
         * generate reg16=[a0,a0] from [a0,a1] and [a0,a1]  with mask = 0*/
        reg16 = _mm_shuffle_pd(reg16, reg16, 0x0);

        reg08 = _mm_add_pd(reg08,_mm_mul_pd(reg00,reg16));
        reg09 = _mm_add_pd(reg09,_mm_mul_pd(reg01,reg16));
        reg10 = _mm_add_pd(reg10,_mm_mul_pd(reg02,reg17));
        reg11 = _mm_add_pd(reg11,_mm_mul_pd(reg03,reg17));

        reg04 = _mm_mul_pd(reg04,reg19);
        reg12 = _mm_add_pd(reg12,_mm_mul_pd(reg04,reg16));
        reg05 = _mm_mul_pd(reg05,reg19);
        reg13 = _mm_add_pd(reg13,_mm_mul_pd(reg05,reg16));

        reg19 = _mm_sub_pd(regzero,reg19);
      
        reg06 = _mm_mul_pd(reg06,reg19);
        reg14 = _mm_add_pd(reg14,_mm_mul_pd(reg06,reg17));
        reg07 = _mm_mul_pd(reg07,reg19);
        reg15 = _mm_add_pd(reg15,_mm_mul_pd(reg07,reg17));

        //reg19 = _mm_sub_pd(regzero,reg19);

        mmm += 2;
      }

      reg21 = _mm_load_pd(&csmphi[m-1]);
      reg22 = _mm_shuffle_pd(reg21, reg21, 0x3);
      reg21 = _mm_shuffle_pd(reg21, reg21, 0x0);

      reg23 = _mm_load_pd(&csmphipi[m-1]);
      reg24 = _mm_shuffle_pd(reg23, reg23, 0x3);
      reg23 = _mm_shuffle_pd(reg23, reg23, 0x0);

      reg00 = _mm_mul_pd(reg08,reg23);
      reg00 = _mm_sub_pd(reg00,_mm_mul_pd(reg10,reg24));
      reg01 = _mm_mul_pd(reg09,reg23);
      reg01 = _mm_sub_pd(reg01,_mm_mul_pd(reg11,reg24));

      reg02 = _mm_mul_pd(reg10,reg23);
      reg02 = _mm_add_pd(reg02,_mm_mul_pd(reg08,reg24));
      reg03 = _mm_mul_pd(reg11,reg23);
      reg03 = _mm_add_pd(reg03,_mm_mul_pd(reg09,reg24));

      reg04 = _mm_mul_pd(reg12,reg21);
      reg04 = _mm_sub_pd(reg04,_mm_mul_pd(reg14,reg22));
      reg05 = _mm_mul_pd(reg13,reg21);
      reg05 = _mm_sub_pd(reg05,_mm_mul_pd(reg15,reg22));

      reg06 = _mm_mul_pd(reg14,reg21);
      reg06 = _mm_add_pd(reg06,_mm_mul_pd(reg12,reg22));
      reg07 = _mm_mul_pd(reg15,reg21);
      reg07 = _mm_add_pd(reg07,_mm_mul_pd(reg13,reg22));

      _mm_store_sd(&scr2[jaddress[ 0]+i],reg00);
      reg00 = _mm_shuffle_pd(reg00,reg00,0x3);
      _mm_store_sd(&scr2[jaddress[ 1]+i],reg00);

      _mm_store_sd(&scr2[jaddress[ 2]+i],reg01);
      reg01 = _mm_shuffle_pd(reg01,reg01,0x3);
      _mm_store_sd(&scr2[jaddress[ 3]+i],reg01);

      _mm_store_sd(&scr2[jaddress[ 4]+i],reg02);
      reg02 = _mm_shuffle_pd(reg02,reg02,0x3);
      _mm_store_sd(&scr2[jaddress[ 5]+i],reg02);

      _mm_store_sd(&scr2[jaddress[ 6]+i],reg03);
      reg03 = _mm_shuffle_pd(reg03,reg03,0x3);
      _mm_store_sd(&scr2[jaddress[ 7]+i],reg03);

      _mm_store_sd(&scr2[jaddress[ 8]+i],reg04);
      reg04 = _mm_shuffle_pd(reg04,reg04,0x3);
      _mm_store_sd(&scr2[jaddress[ 9]+i],reg04);

      _mm_store_sd(&scr2[jaddress[10]+i],reg05);
      reg05 = _mm_shuffle_pd(reg05,reg05,0x3);
      _mm_store_sd(&scr2[jaddress[11]+i],reg05);

      _mm_store_sd(&scr2[jaddress[12]+i],reg06);
      reg06 = _mm_shuffle_pd(reg06,reg06,0x3);
      _mm_store_sd(&scr2[jaddress[13]+i],reg06);

      _mm_store_sd(&scr2[jaddress[14]+i],reg07);
      reg07 = _mm_shuffle_pd(reg07,reg07,0x3);
      _mm_store_sd(&scr2[jaddress[15]+i],reg07);
    }
  }
}
