
#include <Arduino.h>
#include "NMidi.h"

// Notenames
const char  *const notenames[]  = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
const char notefmtString[]  = "%2.2s%1d";
char notelogString[4];


unsigned short combineBytes(unsigned char _msb, unsigned char _lsb)
{
    unsigned short _14bit;
    _14bit = (unsigned short)_lsb;
    _14bit <<= 7;
    _14bit |= (unsigned short)_msb;
    return(_14bit);
}

/**
 *  \brief will provide a readable char* of the given Note Number
 *
 *  \param [in] note NoteNumer 0-127
 *  \return the given char* as "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
 *
 *  \details this is not reentrant, you must copy away the result before the next call
 */
const char *decodeNote (byte note)
{
    byte index = note % 12;
    return notenames[index];
}
/**
 *  \brief will provide a readable chr* of the octave of the given Note Numer
 *
 *  \param [in] note note NoteNumer 0-127
 *  \return Return description
 *
 *  \details More details
 */
byte decodeOct (byte note)
{
    return (note  / 12) - 1;

}
/**
 *  \brief will provide a readable char* of the given Note Number
 *
 *  \param [in] note NoteNumer 0-127
 *  \return the given char* note + oct as "C1", "C#2", "D3", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
 *
 *  \details this is not reentrant, you must copy away the result before the next call
 */
char *decodeNote2 (byte note)
{
    sprintf(notelogString, notefmtString, decodeNote(note), decodeOct(note));
    return notelogString;
}
/**
 *  \brief will calculate the required Ticks for a timer Object on a given bpm
 *
 *  \param [in] bpm
 *  \return the the parameter for a timerX.setPeriod  
 *
 *  \details this is not reentrant, you must copy away the result before the next call
 */
long bpm2tick(uint8_t __bpm)
{
  return magic_number / (long)__bpm;
}
/**
 *  \brief will calculate the required Ticks for a timer Object on a given bpm, rounded to a base of 4
 *
 *  \param [in] bpm
 *  \return the the parameter for a timerX.setPeriod  
 *
 *  \details this is not reentrant, you must copy away the result before the next call
 */

long bpm2tick4(uint8_t __bpm)
{
  long l = magic_number / (long)__bpm;
  l /= 4;
  l *= 4;
  return l;
  
}
boolean isChannelMessage(byte cmd)
{
    if ((cmd > 0x7f) && (cmd < 0xf0)) return true;
    return false ;
}
boolean isSystemMessage(byte cmd)
{
    if (cmd >= 0xf0) return true;
    return false ;
}
boolean isRealtimeMessage(byte cmd)
{
    if ((cmd >= 0xf8) && (cmd < 0xfd)) return true;
    return false ;
}
