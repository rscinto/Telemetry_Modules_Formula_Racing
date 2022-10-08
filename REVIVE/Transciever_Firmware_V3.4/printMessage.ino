void printMessage(message* m1)
{
  Serial.print("Size of message: ");
  Serial.println(sizeof(m1));
  Serial.print("Message Type = ");
  Serial.println(m1->type);
  Serial.print("Message DLC = ");
  Serial.println(m1->dlc);
  Serial.print("xcvrID = ");
  Serial.println(m1->xcvrID);
  Serial.print("msgNum = ");
  Serial.println(m1->msgNum);
  Serial.print("Payload = ");
  for(byte i = 0; i< m1->dlc; i++)
  {
    Serial.print(m1->payload[i]);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("Message Checksum = ");
  Serial.println(m1->checkSum);
  Serial.println("");
}
