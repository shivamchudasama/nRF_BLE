/**
 * @file      BALOS_types.h
 * @brief     header file with type definitions common to all projects
 *            developed using BALOS.
 * @date      10/11/2016
 * @author    Aniket Bhattacharya (ABB)
 * @copyright Bajaj Auto Limited (BAL)
 */

#ifndef BALOS_TYPES_H
#define BALOS_TYPES_H

#ifndef NULL_PTR
#define NULL_PTR ((void*)(0))
#endif
#ifndef false
#define false (0u)
#endif
#ifndef true
#define true  (1u)
#endif
#ifndef NULL
#define NULL ((void*)(0))
#endif
#ifndef FALSE
#define FALSE (0u)
#endif
#ifndef TRUE
#define TRUE  (1u)
#endif
#ifndef OKAY
#define OKAY (1u)
#endif
#ifndef FAIL
#define FAIL (0u)
#endif

#ifndef EVEN
#define EVEN (0u)
#endif

#ifndef ODD
#define ODD (1u)
#endif

typedef unsigned char      UInt8_T;
typedef unsigned char      Bool_T;
typedef signed char        SInt8_T;
typedef unsigned short     UInt16_T;
typedef signed short       SInt16_T;
typedef unsigned long      UInt32_T;
typedef unsigned long      int32_t;
typedef signed long        SInt32_T;
typedef unsigned char      bool;
typedef unsigned long long UInt64_T;
typedef signed long long   SInt64_T;
typedef float              Real32_T;

typedef union
{
   UInt8_T  byVar;
   SInt8_T  sbyVar;
   UInt16_T wVar;
   SInt16_T swVar;
   UInt32_T dwVar;
   SInt32_T sdwVar;
}Universal_T;

typedef union
{
   UInt8_T  Byte[ 4 ];
   UInt16_T HalfWord[ 2 ];
   UInt32_T Word;
} TypeConversion_U;

typedef struct
{
   UInt8_T u8_lowerNibble: 4;                /**< Lower nibble */
   UInt8_T u8_upperNibble: 4;                /**< Upper nibble */
}Nibbles_T;

#define NIBBLE_SIZE_BITS   4
#define BYTE_SIZE_IN_BITS  8
#define WORD_SIZE_IN_BITS  16

#define REG_GET_BIT(reg, bit)     (reg >> bit) & 0x01
#define REG_SET_BIT(reg, bit)     reg |= (1 << bit);
#define REG_CLR_BIT(reg, bit)     reg &= ~(1 << bit);
#define REG_FLIP_BIT(reg, bit)    reg ^= (1 << bit);
#endif //BALOS_TYPES_H
/****************************************************************************/
/* Copyright (c) Bajaj Auto Limited (BAL) as an unpublished work.           */
/* THIS SOFTWARE AND/OR MATERIAL IS THE PROPERTY OF BAL.                    */
/* ALL USE, DISCLOSURE, AND/OR REPRODUCTION NOT SPECIFICALLY AUTHORIZED BY  */
/* BAL IS PROHIBITED.                                                       */
/*                                                                          */
/* AUTHOR: Aniket Bhattacharya (ABB)                                        */
/****************************************************************************/
