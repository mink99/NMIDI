void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial1.begin(31250);
Serial.println("----------------MON-----------");
}

void loop() {
  // put your main code here, to run repeatedly:

}
void serialEvent1() {
  static int i = 0;
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    if (inChar != 0xFE)
    {
      Serial.print("  ");
      Serial.print(inChar,HEX);
      if ( i == 2)
      {
        Serial.println();
        i = 0;
      }
      else
      {
        i++;
      }
    }
  }
  //Serial.println();
  Serial.flush();
  Serial1.flush();
}
  