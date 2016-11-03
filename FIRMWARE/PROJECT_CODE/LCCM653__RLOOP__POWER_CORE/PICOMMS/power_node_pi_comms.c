/**
* @file		    POWER_CORE__PI_COMMS.C
* @brief		Pi Communications telemetry layer
* @author		David Corrigan
* @copyright	rLoop Inc.
* @st_fileID
*/
/**
* @addtogroup RLOOP
* @{ */
/**
* @addtogroup POWER_NODE
* @ingroup RLOOP
* @{ */
/**
* @addtogroup POWER_NODE__NODE_PICOMMS
* @ingroup POWER_NODE
* @{ */

#include "../power_core.h"


#if C_LOCALDEF__LCCM653__ENABLE_THIS_MODULE == 1U

extern struct _strPWRNODE sPWRNODE;

/***************************************************************************//**
 * @brief
 * Try to send the latest telemetry to the ground station
 *
 * @st_funcMD5		????
 * @st_funcID		???
 */
Luint8 vPWRNODE_PICOMMS_TrySend()
{
	Luint16 u16Length;
	Luint8 *pu8Buffer;

	//TODO: Check if the UART is free, ie last DMA complete
	/*
	if(uartState != TxReady)
		//return 0; UART not ready for new data
	else
		//great, continue on
	*/

	//Reset the packet buffer
	rI2CTX_beginFrame();

	//TODO: Check for each enabled module and add it's parameters to the list

#ifdef WIN32

	rI2CTX_addParameter_float(5000U, sPWRNODE.sWIN32.f32NodeTemperature);

#endif // Win32

	rI2CTX_addParameter_uint8(348, (Luint8)sPWRNODE.sInit.sState);

	//finalize the buffer with a footer and checksum
	rI2CTX_endFrame();

#ifdef WIN32
	//TODO: Call the VB upper layer
	DEBUG_PRINT("Sending a telemetry packet.");
#endif

	//finish the frame
	u16Length = rI2CTX_endFrame();

#ifdef WIN32
	//force it to be sent (this will change in the future)
	pu8Buffer = pu8I2CTx__Get_BufferPointer();
	vPICOMMS_WIN32__TxFrame_Callback(pu8Buffer, u16Length);
#endif

#ifndef WIN32
	pu8Buffer = pu8I2CTx__Get_BufferPointer();

	//If the hardware is ready let's send off the packet
	if(u32RM4_SCI__Is_TxReady(SCI_CHANNEL__2))
	{
		vRM4_SCI__TxByteArray(SCI_CHANNEL__2, u16Length, pu8Buffer);
	}
#endif

	//TODO: Pass it to the UART
	//If Win32 pass it to local loopback(?) or do that at lower layer?

	return 0;
}


#endif //#if C_LOCALDEF__LCCM653__ENABLE_THIS_MODULE == 1U
//safetys
#ifndef C_LOCALDEF__LCCM653__ENABLE_THIS_MODULE
#error
#endif
/** @} */
/** @} */
/** @} */
