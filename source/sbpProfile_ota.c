/**************************************************************************************************
*******
**************************************************************************************************/

/**************************************************************************************************
    Filename:       sbpProfile_ota.c
    Revised:
    Revision:

    Description:    This file contains the Simple GATT profile sample GATT service
                  profile for use with the BLE sample application.


**************************************************************************************************/

/*********************************************************************
    INCLUDES
*/
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"
//#include "log.h"
#include "sbpProfile_ota.h"
#include "simpleBLEPeripheral.h"
#include "eltop.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        12

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 simpleProfileServUUID[ATT_BT_UUID_SIZE] =
{ 
	LO_UINT16(SIMPLEPROFILE_SERV_UUID), HI_UINT16(SIMPLEPROFILE_SERV_UUID)
};

// Characteristic 1 UUID: 0xFFF3
CONST uint8 simpleProfilechar1UUID[ATT_BT_UUID_SIZE] =
{ 
	LO_UINT16(SIMPLEPROFILE_CHAR1_UUID), HI_UINT16(SIMPLEPROFILE_CHAR1_UUID)
};

// Characteristic 2 UUID: 0xFFF4
CONST uint8 simpleProfilechar2UUID[ATT_BT_UUID_SIZE] =
{ 
	LO_UINT16(SIMPLEPROFILE_CHAR2_UUID), HI_UINT16(SIMPLEPROFILE_CHAR2_UUID)
};

// Characteristic 3 UUID: 0xFFF5
CONST uint8 simpleProfilechar3UUID[ATT_BT_UUID_SIZE] =
{ 
	LO_UINT16(SIMPLEPROFILE_CHAR3_UUID), HI_UINT16(SIMPLEPROFILE_CHAR3_UUID)
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static simpleProfileCBs_t *simpleProfile_AppCBs = NULL;
static	uint8	ReadNotify_Len		=	0;

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t simpleProfileService = { ATT_BT_UUID_SIZE, simpleProfileServUUID };


// Simple Profile Characteristic 1 Properties
static uint8 simpleProfileChar1Props 			= 	GATT_PROP_WRITE | GATT_PROP_WRITE_NO_RSP;
static uint8 simpleProfileChar1[BLE_ATT_CMD_LED]=	{0,};			// Characteristic 1 Value
static uint8 simpleProfileChar1UserDesp[] 		= 	"Commond\0";	// Simple Profile Characteristic 1 User Description


// Simple Profile Characteristic 2 Properties
static uint8 simpleProfileChar2Props 			= 	GATT_PROP_READ |GATT_PROP_NOTIFY;
static uint8 simpleProfileChar2[BLE_ATT_CMD_LED]= 	{0,};			// Characteristic 2 Value
static uint8 simpleProfileChar2UserDesp[] 		= 	"Response\0";	// Simple Profile Characteristic 2 User Description
static gattCharCfg_t simpleProfileChar2Config[GATT_MAX_NUM_CONN];		//

// Simple Profile Characteristic 3 Properties
static uint8 simpleProfileChar3Props 			= 	GATT_PROP_READ |GATT_PROP_NOTIFY;
static uint8 simpleProfileChar3[BLE_ATT_CMD_LED]= 	{0,};			// Characteristic 2 Value
static uint8 simpleProfileChar3UserDesp[] 		= 	"Response\1";	// Simple Profile Characteristic 2 User Description
static gattCharCfg_t simpleProfileChar3Config[GATT_MAX_NUM_CONN];		//

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t simpleProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
	/* type */								/* permissions */			/* handle */	/* pValue */
	// Simple Profile Service
	{{ ATT_BT_UUID_SIZE, primaryServiceUUID },			GATT_PERMIT_READ,		0,					(uint8 *)&simpleProfileService},

	// Characteristic 1 Declaration
	{{ ATT_BT_UUID_SIZE, characterUUID },				GATT_PERMIT_READ,		0,					&simpleProfileChar1Props},
	// Characteristic Value 1
	{{ ATT_BT_UUID_SIZE, simpleProfilechar1UUID },		GATT_PERMIT_READ | GATT_PERMIT_WRITE,	0,	&simpleProfileChar1[0]},
	// Characteristic 1 User Description
	{{ ATT_BT_UUID_SIZE, charUserDescUUID },			GATT_PERMIT_READ,		0,					simpleProfileChar1UserDesp},      

	// Characteristic 2 Declaration
	{{ ATT_BT_UUID_SIZE, characterUUID },				GATT_PERMIT_READ,		0,					&simpleProfileChar2Props},
	// Characteristic Value 2
	{{ ATT_BT_UUID_SIZE, simpleProfilechar2UUID },		GATT_PERMIT_READ | GATT_PERMIT_WRITE,	0,	&simpleProfileChar2[0]},
	// Characteristic 2 configuration
	{{ ATT_BT_UUID_SIZE, clientCharCfgUUID },			GATT_PERMIT_READ | GATT_PERMIT_WRITE,	0,	(uint8 *)simpleProfileChar2Config}, 
	// Characteristic 2 User Description
	{{ ATT_BT_UUID_SIZE, charUserDescUUID },			GATT_PERMIT_READ,		0,					simpleProfileChar2UserDesp},      

// Characteristic 3 Declaration
	{{ ATT_BT_UUID_SIZE, characterUUID },				GATT_PERMIT_READ,		0,					&simpleProfileChar3Props},
	// Characteristic Value 3
	{{ ATT_BT_UUID_SIZE, simpleProfilechar3UUID },		GATT_PERMIT_READ | GATT_PERMIT_WRITE,	0,	&simpleProfileChar3[0]},
	// Characteristic 3 configuration
	{{ ATT_BT_UUID_SIZE, clientCharCfgUUID },			GATT_PERMIT_READ | GATT_PERMIT_WRITE,	0,	(uint8 *)simpleProfileChar3Config}, 
	// Characteristic 3 User Description
	{{ ATT_BT_UUID_SIZE, charUserDescUUID },			GATT_PERMIT_READ,		0,					simpleProfileChar3UserDesp},     	
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t	simpleProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,uint8 *pValue, uint16 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t 	simpleProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,uint8 *pValue, uint16 len, uint16 offset );
static void 		simpleProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType );
/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t simpleProfileCBs =
{
	simpleProfile_ReadAttrCB,  // Read callback function pointer
	simpleProfile_WriteAttrCB, // Write callback function pointer
	NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t SimpleProfile_AddService( uint32 services )
{
	uint8 status = SUCCESS;

	// Initialize Client Characteristic Configuration attributes
	GATTServApp_InitCharCfg( INVALID_CONNHANDLE, simpleProfileChar2Config );

	// Register with Link DB to receive link status change callback
	VOID linkDB_Register( simpleProfile_HandleConnStatusCB );  

	if ( services & SIMPLEPROFILE_SERVICE )
	{
		// Register GATT attribute list and CBs with GATT Server App
		status = GATTServApp_RegisterService( simpleProfileAttrTbl, 
		                                  GATT_NUM_ATTRS( simpleProfileAttrTbl ),
		                                  &simpleProfileCBs );
	}

	return ( status );
}


/*********************************************************************
 * @fn      SimpleProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t SimpleProfile_RegisterAppCBs( simpleProfileCBs_t *appCallbacks )
{
	if ( appCallbacks ){
		simpleProfile_AppCBs = appCallbacks;
		return ( SUCCESS );
	}else{
		return ( bleAlreadyInRequestedMode );
	}
}
  

/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_SetParameter( uint8 param, uint8 len, void *value )
{
	bStatus_t ret = SUCCESS;
	switch ( param )
	{
		case SIMPLEPROFILE_CHAR1:
			if ( len <= BLE_ATT_CMD_LED ){
				osal_memcpy(simpleProfileChar1, value, len);				
			}else{
				ret = bleInvalidRange;
			}
		break;
		default:
			ret = INVALIDPARAMETER;
		break;
	}

	return ( ret );
}

/*********************************************************************
 * @fn      SimpleProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_GetParameter( uint8 param, void *value )
{
	bStatus_t ret = SUCCESS;
	switch ( param )
	{
		// case SIMPLEPROFILE_CHAR1:
		// 	VOID osal_memcpy( value, simpleProfileChar1, BLE_ATT_CMD_LED);    
		// break;

		case SIMPLEPROFILE_CHAR2:
			VOID osal_memcpy( value, simpleProfileChar2, BLE_ATT_CMD_LED );    
		break;      

		default:
			ret = INVALIDPARAMETER;
		break;
	}
	return ( ret );
}

/*********************************************************************
 * @fn          simpleProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static bStatus_t simpleProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint16 *pLen, uint16 offset, uint8 maxLen )
{
	bStatus_t status = SUCCESS;

	// If attribute permissions require authorization to read, return error
	if ( gattPermitAuthorRead( pAttr->permissions ) )
	{
		// Insufficient authorization
		return ( ATT_ERR_INSUFFICIENT_AUTHOR );
	}
  
	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}
 
	if ( pAttr->type.len == ATT_BT_UUID_SIZE )
	{
		uint8 *newValue = (uint8*)osal_mem_alloc(30);
		// 16-bit UUID
		uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
		switch ( uuid )
		{
			// No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
			// gattserverapp handles those reads
			case SIMPLEPROFILE_CHAR2_UUID:
				//*pLen = BLE_ATT_CMD_LED;
				//VOID osal_memcpy( pValue, pAttr->pValue, *pLen );
				//LOG("Read_UUID2:\n");
			//newValue[0]='1';newValue[1]='2';
			//SimpleProfile_GetParameter( SIMPLEPROFILE_CHAR2, newValue );
		   *pLen = 20;
            pValue[0] = 'O';
            pValue[1] = 'K';
            pValue[2] = 'E';
            //pValue[3] = ' ';
            //pValue[4] = 'E';
            //pValue[5] = 'L';
            //pValue[6] = 'T';
		    //pValue[7] = 'O';
			//pValue[8] = 'P';
			//pValue[9] = 'Y';  pValue[10] = 'E';  pValue[11] = 'S';
			//pValue[12] = ' ';  pValue[13] = '0';  pValue[14] = '1';
			//pValue[15] = 'Y';  pValue[16] = 'E';  pValue[17] = 'S';
			//pValue[18] = 'T';  pValue[19] = 'O';  pValue[20] = 'P';
			//pValue[21] = ' '; // pValue[22] = '9';  pValue[23] = '4';
			//pValue[24] = '3';  pValue[25] = '2';  pValue[26] = '1';
			LOG("Balas_UUID2:=%s\n",pValue);
			break;
			case SIMPLEPROFILE_CHAR3_UUID:
		   *pLen = 10;
            pValue[0] = '1';
            pValue[1] = '2';
            pValue[2] = '3';
            pValue[3] = '4';
            pValue[4] = '5';
            pValue[5] = '6';
            pValue[6] = '7';
		    pValue[7] = '8';
			pValue[8] = '9';
			
			LOG("Read_UUID2:Len=%d data=%s\n",*pLen,pValue);
			break;
			default:
				// Should never get here! (characteristics 3 and 4 do not have read permissions)
				*pLen = 0;
				status = ATT_ERR_ATTR_NOT_FOUND;
			break;
		}
	}else{
		// 128-bit UUID
		*pLen = 0;
		status = ATT_ERR_INVALID_HANDLE;
	}
	return ( status );
}

/*********************************************************************
 * @fn      simpleProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  Success or Failure
 */
uint8 rx_idx;
extern flg_bit BIT;

extern BUF bfr; //char dt_rx[256];
 // TODO: test this function
static bStatus_t simpleProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint16 len, uint16 offset )
{
	bStatus_t status = SUCCESS;
	uint8 notifyApp = 0xFF;

	// If attribute permissions require authorization to write, return error
	if ( gattPermitAuthorWrite( pAttr->permissions ) )
	{
	// Insufficient authorization
	return ( ATT_ERR_INSUFFICIENT_AUTHOR );
	}
  
	if ( pAttr->type.len == ATT_BT_UUID_SIZE )
	{   NVIC_DisableIRQ((IRQn_Type)TIM5_IRQn);
		// 16-bit UUID
		uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
		switch ( uuid )
		{
			case SIMPLEPROFILE_CHAR1_UUID:
				//Validate the value
				// Make sure it's not a blob oper
				if ( offset == 0 ){
					if ( len > BLE_ATT_CMD_LED){
						status = ATT_ERR_INVALID_VALUE_SIZE;
					}
				}else{
					status = ATT_ERR_ATTR_NOT_LONG;
				}
				//Write the value
				if ( status == SUCCESS ){  
					//uint8 *pCurValue = (uint8 *)pAttr->pValue;        
					//VOID osal_memcpy(pCurValue, pValue, len );
					//LOG("receive data = 0x=%d ",len); LOG("%s ",pCurValue);
					
					for(uint8_t i=0; i<  len;  i++  ) { bfr.dt_rx [i + (rx_idx*20) ] = pValue[i]; }
					rx_idx++;  
					if( len <20 ) {  //LOG("\nrx_idx=%d || %s *%c\n",rx_idx,bfr.dt_rx,bfr.dt_rx[0]);
						rx_idx=0; BIT.RX=1; //dbg_printf("rx_idx=%d || %s\n",rx_idx,bfr.dt_rx);
						}
						//LOG_DUMP_BYTE(pCurValue, len); osal_memset(bfr.dt_rx,0,rx_idx);
					//	DO NOT deal data in call back!!!Copy data and start an event.
					//osal_set_event(simpleBLEPeripheral_TaskID, SBP_DEALDATA);
					notifyApp = SIMPLEPROFILE_CHAR1;
					
				}		 	  	
			break;
			
			case GATT_CLIENT_CHAR_CFG_UUID:
				LOG("Enable/Disable Notity\n");
				status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
				                                     offset, GATT_CLIENT_CFG_NOTIFY );
			break;

			default:
				// Should never get here! (characteristics 2 and 4 do not have write permissions)
				status = ATT_ERR_ATTR_NOT_FOUND;
			break;
		} NVIC_EnableIRQ((IRQn_Type)TIM5_IRQn);
	}else{
		// 128-bit UUID
		status = ATT_ERR_INVALID_HANDLE;
	}
	// If a charactersitic value changed then callback function to notify application of change
	if ( (notifyApp != 0xFF ) && simpleProfile_AppCBs && simpleProfile_AppCBs->pfnSimpleProfileChange ){
		simpleProfile_AppCBs->pfnSimpleProfileChange( notifyApp );  
	}
	return ( status );
}

/*********************************************************************
 * @fn          simpleProfile_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void simpleProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
	// Make sure this is not loopback connection
	if ( connHandle != LOOPBACK_CONNHANDLE ){
		// Reset Client Char Config if connection has dropped
		if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )||( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) &&( !linkDB_Up( connHandle ) ) ) ){ 
			GATTServApp_InitCharCfg( connHandle, simpleProfileChar2Config );
		}
	}
}

bStatus_t simpleProfile_Notify( uint8 param, uint8 len, void *value )
{
	bStatus_t ret = SUCCESS;
	uint16 notfEnable;
	
	switch ( param )
	{  
		case SIMPLEPROFILE_CHAR2:
			notfEnable = GATTServApp_ReadCharCfg( 0, simpleProfileChar2Config );

			// If notifications enabled
			if ( notfEnable & GATT_CLIENT_CFG_NOTIFY ){
				VOID osal_memcpy( simpleProfileChar2, value, len );
				ReadNotify_Len	=	len;
				ret = GATTServApp_ProcessCharCfg( simpleProfileChar2Config, simpleProfileChar2, FALSE,
				        simpleProfileAttrTbl, GATT_NUM_ATTRS( simpleProfileAttrTbl ),
				        INVALID_TASK_ID );      
			}else{
				ret = bleNotReady;
			}
		break;

		default:
			ret = INVALIDPARAMETER;
		break;
	}
	return ( ret );
}


/*********************************************************************
*********************************************************************/
