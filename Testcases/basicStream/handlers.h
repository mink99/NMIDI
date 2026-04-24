void _handleNoteON(uint8_t port, Channel ch, byte note, byte velocity)
{
  Serial.print(">> NoteON  : channel ");
  Serial.print(ch);
  Serial.print(",Note ");
  Serial.print(note);
  Serial.print(",Vel ");
  Serial.print(velocity);
  Serial.println();
}
void _handleNoteOFF(uint8_t port, Channel ch, byte note, byte velocity)
{
  Serial.print(">> NoteOFF : channel ");
  Serial.print(ch);
  Serial.print(",Note ");
  Serial.print(note);
  Serial.print(",Vel ");
  Serial.print(velocity);
  Serial.println();
}
void _handleProgramChange(uint8_t port, Channel ch, byte program)
{
  Serial.print(">> ProgramChange  : channel ");
  Serial.print(ch);
  Serial.print(",Program ");
  Serial.print(program);
  Serial.println();
}
boolean _processData(uint8_t _port, CommandType cmd, Channel ch, uint8_t msgLen, byte data[]) {
  //Print MIDI Event Data:  
  Serial.print("MESSAGE > ");
  Serial.print(_port);
  Serial.print(" ** ");
  Serial.print(portA.commandTypeToString(cmd));
  Serial.print(" ,channel ");
  if (ch == CH_ALL) Serial.print("[N/A]");
  else Serial.print(ch);
  Serial.print(", {");
  for (int i = 0; i < msgLen; i++)
  {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println("}");
  return true;
}
