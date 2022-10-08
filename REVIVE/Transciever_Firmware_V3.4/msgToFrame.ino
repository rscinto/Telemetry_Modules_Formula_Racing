uint8_t msgToFrame( message* m, uint8_t* a) {
  //Serial.print("msgNum: ");
  //Serial.println(m->msgNum);
  a[0] = (m->type<<6)   + (m->dlc);
  a[1] = (m->xcvrID<<4) + (m->msgNum>>8);
  a[2] = (m->msgNum&0x00FF);
  for(byte i = 0; i<m->dlc; i++)
  {
    a[i+3] = m->payload[i];
  }
  a[m->dlc +3] = m->checkSum >> 8;//bug originally +2
  a[m->dlc +4] = m->checkSum & 0x00FF;

  return 5 + m->dlc;
}
