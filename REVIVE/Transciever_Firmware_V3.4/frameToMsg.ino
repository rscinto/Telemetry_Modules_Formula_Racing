uint8_t frameToMsg(const uint8_t* data_in, message* m) {
  
  m->type   =  (data_in[0] & 0b11000000) >> 6;
  m->dlc    =  (data_in[0] & 0b00111111);
  m->xcvrID =  (data_in[1] & 0b11110000) >> 4;
  m->msgNum =  (data_in[1] & 0b00001111) << 8;
  m->msgNum += (data_in[2] & 0b11111111);
  
  for (uint8_t i = 0; i < m->dlc; i++)
  {
   m->payload[i] = data_in[i + 3];
  }
  
  m->checkSum = data_in[m->dlc + 3] << 8;
  m->checkSum += data_in[m->dlc + 4];

  return 1;
}
