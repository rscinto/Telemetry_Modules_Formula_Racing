uint8_t sendMessage(message* m)
{
  Serial.write(0xFF);
  Serial.write(0xFF);
  Serial.write(((m->type) << 6) + m->dlc);

  for(byte i = 0; i< m->dlc; i++)
  {
    Serial.write(m->payload[i]);
  }

  Serial.write((uint8_t) m->checkSum >> 8);
  Serial.write((uint8_t) m->checkSum & 0x00FF);
}
