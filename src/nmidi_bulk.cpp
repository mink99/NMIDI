
/** MidiPort Libary

based on ArduMidi C++ FILE v2.1 by Pecacheu
NMidi Libary C++ FILE v2.1 by Mink

 block modes : NRPN & SYSEX
 */

#include <Stream.h>
#include <Arduino.h>

#include "NMidi.h"


using namespace nmidi;



/**
 *  \brief Start a sysex conversation
 *  
 *  \param [in] manufacturerID manufacturerID
 *  \param [in] deviceID deviceID
 *  
 *  \details 
 * [1]  [2]  [3]  [4]  [5]  [6]      [7]      [8]  [9]
 *  F0   41   10   42   11   401100   000002   2D   F7
 * 
 *  Parts [1], [2] and [9] are part of the MIDI specification and are required by all SysEx messages. 
 *  What is in between is specific to the manufacturer, identified by part [2], which is 41h in Roland's case.
 */
void MidiPort::beginTransferSysex(byte manufacturerID, byte deviceID)
{
	enableRunningStatus(false);
	byte _msg[3];
    _msg[0] = SYSEX_START;
    _msg[1] = manufacturerID;
    _msg[2] = deviceID;
    writeMsg(_msg);
}; //0xf0

/**
 *  \brief SYSEX Roland Style
 *  
 *  \param [in] data[] the sysex bytes
 *  \param [in] size size of the array
 *  \param [in] prev checksum from previous call
 *  \return new checksum
 *  
 *  \details 
 *  Calculating the roland checksum follows a simple formula which is easily learned and applied. Following this formula will gaurantee correct checksums:

 * 1. Convert the hex values to decimal.
 * 2. Add the values together, but if the answer to any sum exceeds 127 then subtract 128.
 * 3. Subtract the final answer from 128.
 * 4. Convert the resulting value back to hex and use as the checksum. (but not here)
 */
uint8_t MidiPort::transferSysexR(byte data[], uint8_t size, uint8_t prev) 
{
	int8_t sum = prev;
	for(int i = 0; i < size; i++)
	{
		sum += data[i];
		if (sum > 128) sum = 128 - sum;
	}
	writeMsg(data,size);
	return sum;
}; 
/**
 *  \brief transfers an array of bytes tin a sysex session
 *  
 *  \param [in] data[] the sysex bytes
 *  \param [in] size size of the array

 */
void MidiPort::transferSysex(byte data[], uint8_t size) 
{
	writeMsg(data,size);
};
/**
 *  \brief ends the sysex session
 *  
 */
void MidiPort::endTransferSysex() 
{
	writeMsg(SYSEX_END);
}; //0xf7
/**
 *  \brief ends the sysex session
 *  
 * 3. Subtract the final answer from 128.
 * 4. Convert the resulting value back to hex and use as the checksum. (here)  
 *  \param [in] checksum the roland checksum
 */
void MidiPort::endTransferSysexR(byte checksum)
{
	byte _msg[2];
    _msg[0] = 128- checksum;
    _msg[1] = SYSEX_END;
    writeMsg(_msg,2);
}




// status	99	NRPN MSB	98	NRPN LSB	06	value	101	127	100	127

void MidiPort::beginTransferNRPN(Channel ch , byte _registered_msb , byte _registered_lsb) 
{
	enableRunningStatus(false);
	byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = NRPN_MSB;
    _msg[2] = _registered_msb;
    writeMsg(_msg);	
	_msg[0] = CTRL_CHANGE + ch;
    _msg[1] = NRPN_LSB;
    _msg[2] = _registered_lsb;
    writeMsg(_msg);	
}; //0x65
void MidiPort::transferNRPN(Channel ch ,  byte ctrl , byte data) 
{
	byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = ctrl;
    _msg[2] = data;
    writeMsg(_msg);	
	
}; //0xf7
/**
 *  \brief NRPN Null Message
 *  
 *  \param [in] ch the channel
 */
void MidiPort::endTransferNRPN(Channel ch) 
{
	byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = RPN_MSB;
    _msg[2] = NRPN_TERM;
    writeMsg(_msg);	
	_msg[0] = CTRL_CHANGE + ch;
    _msg[1] = RPN_LSB;
    _msg[2] = NRPN_TERM;
    writeMsg(_msg);
}; //0x63 0xf7, 0xf7

