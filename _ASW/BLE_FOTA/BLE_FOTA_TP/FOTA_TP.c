/**
 * @file          FOTA_TP.c
 * @brief         Source file containing FOTA Transport Protocol implementation details.
 * @date          13/02/2026
 * @author        Shivam Chudasama [SC]
 * @copyright     Bajaj Auto Technology Limited (BATL)
 */

/******************************************************************************/
/*                                                                            */
/*                                  INCLUDES                                  */
/*                                                                            */
/******************************************************************************/
#include "FOTA_TP.h"

/******************************************************************************/
/*                                                                            */
/*                                  DEFINES                                   */
/*                                                                            */
/******************************************************************************/
/**
 * @def           <Define name>
 * @brief         <Define details>.
 */

/******************************************************************************/
/*                                                                            */
/*                                   ENUMS                                    */
/*                                                                            */
/******************************************************************************/
// Definition of all the enums
/**
 * @enum          <Enum name>
 * @brief         <Enum details>.
 */

// Declarations of all the enum variables
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */

/******************************************************************************/
/*                                                                            */
/*                                 STRUCTURES                                 */
/*                                                                            */
/******************************************************************************/
// Definition of all the structures
/**
 * @struct        <Structure name>
 * @brief         <Structure details>.
 */

// Declarations of all the structure variables
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */

/******************************************************************************/
/*                                                                            */
/*                                   UNIONS                                   */
/*                                                                            */
/******************************************************************************/
// Definition of all the unions
/**
 * @union         <Union name>
 * @brief         <Union details>.
 */

// Declarations of all the union variables
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */

/******************************************************************************/
/*                                                                            */
/*                       PRIVATE FUNCTION DECLARATIONS                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                              EXTERN VARIABLES                              */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                              PUBLIC VARIABLES                              */
/*                                                                            */
/******************************************************************************/
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */

/******************************************************************************/
/*                                                                            */
/*                             PRIVATE VARIABLES                              */
/*                                                                            */
/******************************************************************************/
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */

/******************************************************************************/
/*                                                                            */
/*                              EXTERN FUNCTIONS                              */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                        PRIVATE FUNCTION DEFINITIONS                        */
/*                                                                            */
/******************************************************************************/
/**
 * @private       <Function name>
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */

/******************************************************************************/
/*                                                                            */
/*                        PUBLIC FUNCTION DEFINITIONS                         */
/*                                                                            */
/******************************************************************************/
/**
 * @public        ge_TP_ParseCtrlPktList
 * @brief         This function parses Control Packet list from received GATT characteristic data.
 * @param[in]     u8pt_data Pointer to advertising data.
 * @param[in]     u8_dataLen Total length of advertising data.
 * @param[out]    stpt_CPList Pointer to CP list structure. The function parses CP
 *                packet and populates this structure back.
 * @return        eTP_OK if no error, otherwise appropriate error code from TPError_E.
 */
TPError_E ge_TP_ParseCtrlPktList(uint8_t *u8pt_data, uint8_t u8_dataLen, \
   CPList_T *stpt_CPList)
{
   TPError_E e_retVal = eTP_OK;
   uint8_t u8_idx = 0;
   uint8_t u8_length;
   uint8_t u8_CPDataLength;
   CPTypes_E e_CPType;
   CPBlock_T *stpt_CPBlock;

   // Reset the count to CP list
   stpt_CPList->u8_count = 0;

   // Parse the CP blocks from received data until total data length is reached or any error occurs
   while (u8_idx < u8_dataLen)
   {
      // Take the length of the new CP block (First byte of each CP block is length)
      u8_length = u8pt_data[u8_idx];

      // Check if the length of the current CP block is zero or if the current index
      // and length together are more than total CP packet length
      if ((u8_length == 0) || (u8_idx + u8_length >= u8_dataLen))
      {
         e_retVal = eTP_INVALID_CP_BLOCK_LENGTH;
      }

      // Check if there is no error in parameters
      if (eTP_OK == e_retVal)
      {
         // Take the CP type of new CP block (Second byte of each CP block is CP type)
         e_CPType = (CPTypes_E)u8pt_data[u8_idx + 1];
         u8_CPDataLength = u8_length - 1;

         // Check if the CP list is not full
         if (stpt_CPList->u8_count < MAX_CP_BLOCKS)
         {
            // Add the new CP block to CP list
            stpt_CPBlock = &stpt_CPList->star_CPBlocks[stpt_CPList->u8_count++];
            stpt_CPBlock->e_CPType = e_CPType;
            stpt_CPBlock->u8_CPBlockLength = u8_CPDataLength;
            memcpy(stpt_CPBlock->u8ar_CPData, &u8pt_data[u8_idx + 2], u8_CPDataLength);
         }

         // Move to next CP block
         u8_idx += u8_length + 1;
      }
   }

   return e_retVal;
}

/**
 * @public        ge_TP_FindCPByType
 * @brief         This function finds CP block from CP list by CP type.
 * @param[in]     stpt_CPList Pointer to CP list structure.
 * @param[in]     e_CPType Type of CP block which to be found from the list.
 * @param[out]    stptpt_CPBlock Pointer pointer to requested CP block.
 * @return        Structure containing CP block of desired type.
 */
TPError_E ge_TP_FindCPByType(const CPList_T *stpt_CPList, CPTypes_E e_CPType, \
   CPBlock_T **stptpt_CPBlock)
{
   TPError_E e_retVal = eTP_NO_SUCH_CP_BLOCK_AVAILABLE;
   uint8_t u8_idx;

   // Loop through all the CP blocks in CP list
   for (uint8_t u8_idx = 0; u8_idx < stpt_CPList->u8_count; u8_idx++)
   {
      // Check if CP block type of any CP block is matching with the requested one
      if (stpt_CPList->star_CPBlocks[u8_idx].e_CPType == e_CPType)
      {
         *stptpt_CPBlock = &stpt_CPList->star_CPBlocks[u8_idx];
         e_retVal = eTP_OK;
         break;
      }
   }

   return e_retVal;
}

/**
 * Copyright(c) Bajaj Auto Technology Limited (BATL) as an unpublished work.
 * THIS SOFTWARE AND/OR MATERIAL IS THE PROPERTY OF BATL.
 * ALL USE, DISCLOSURE, AND/OR REPRODUCTION NOT SPECIFICALLY AUTHORIZED BY
 * BATL IS PROHIBITED.
 *
 * @author:Shivam Chudasama [SC]
 */
