//Returns 1 if the message structures are the same
uint8_t compareMessage(message* m1, message* m2)
{
  if(m1->type     != m2->type) return 0;
  if(m1->dlc      != m2->dlc ) return 0;
  if(m1->xcvrID   != m2->xcvrID) return 0;
  if(m1->msgNum   != m2->msgNum) return 0;
  if(!compareArray(m1->payload, m2->payload, m1->dlc)) return 0;
  if(m1->checkSum != m2->checkSum) return 0;
  return 1;
}
