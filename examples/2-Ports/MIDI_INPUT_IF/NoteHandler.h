

class NoteHandler: public INotehandler
{
  public:
    void _handleNoteOn(const uint8_t portID, Channel ch, byte key, byte velocity)  //Channel ch, key (0-127), velocity (0-127)
    {
      Serial.print("IF ***** Note ON (");
      Serial.print(portID);
      Serial.print(",");
      Serial.print(ch + 1);
      Serial.print("):");
      Serial.println(decodeNote2(key));
      portA.sendNoteOn(ch, key, velocity);
    }
    void _handleNoteOff(const uint8_t portID, Channel ch, byte key, byte velocity)  //Channel ch, key (0-127), velocity (0-127)
    {
      Serial.print("IF ***** Note OFF(");
      Serial.print(ch + 1);
      Serial.print("):");
      Serial.println(decodeNote2(key));
      portA.sendNoteOff(ch, key, velocity);      
    }
}
;