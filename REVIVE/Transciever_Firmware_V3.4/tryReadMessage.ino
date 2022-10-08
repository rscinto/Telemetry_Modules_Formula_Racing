// Arguments
// message* m: message that tryReadMessage will
//             try to store a valid message in
//
// Returns
// 

uint8_t tryReadMessage(message* m)
{ 
  uint16_t messageBuffer[255];
  
  //Serial.println("Begin tryReadMessage");
  
  // Read in TYPE and DLC
  while(!Serial.available()){};
  uint8_t type_dlc = Serial.read();
  messageBuffer[0] = type_dlc >> 6;
  messageBuffer[1] = type_dlc & 0b00111111;

  //Serial.print("type_dlc is read: ");
  //Serial.print(messageBuffer[0], BIN);
  //Serial.print(", ");
  //Serial.println(messageBuffer[1], BIN);
  
  // Read in the XCVR ID and the first 4 bits
  // of the message #
  while(!Serial.available()){};     
  uint8_t ID_num   = Serial.read(); 
  messageBuffer[2] = ID_num >> 4;
  messageBuffer[3] = ID_num & 0b00001111;
  messageBuffer[3] = messageBuffer[3] << 8;

  //Serial.println("XCVRID and first 4 bits of message number is read");
  
  // Read in the last 4 bits of the message #
  while(!Serial.available()){};
  uint8_t num = Serial.read();
  messageBuffer[3] += num;

  //Serial.print("Message number is read: ");
  //Serial.print(messageBuffer[2], BIN);
  //Serial.print(", ");
  //Serial.println(messageBuffer[3], BIN);

  // Read in PAYLOAD
  while(Serial.available() < messageBuffer[1]) {};  // Wait for the entire message to appear in the serial buffer
  for(uint8_t i = 4; i < messageBuffer[1] + 4; i++) // Load the serial buffer into message buffer
  {
    messageBuffer[i] = Serial.read();
  }

  //Serial.println("Payload is read");
  
  // Read the checksum
  while(Serial.available() < 2) {};
  uint8_t checksumMSB = Serial.read();
  uint8_t checksumLSB = Serial.read();
  uint16_t messageCheckSum = (checksumMSB << 8) + checksumLSB;

  //Serial.println("Checksum is read");

  //Sum the message
  uint32_t messageSum = 0;
  for(uint8_t i = 0; i < messageBuffer[1] + 4; i++) // Sum everything up to the checksum
  {
    messageSum += messageBuffer[i];
  }
  
  //Compares the checksums
  if(messageSum != messageCheckSum) return 0;
  //Serial.println("Checksum has been passed");

  m->type = messageBuffer[0];
  m->dlc  = messageBuffer[1];
  m->xcvrID = messageBuffer[2];
  m->msgNum = messageBuffer[3];
  m->checkSum = messageCheckSum;

  for(uint8_t i = 4; i < messageBuffer[1] + 4; i++) // Load the serial buffer into message buffer
  {
    m->payload[i-2] = messageBuffer[i];
  }

  //Serial.println("Serial buffer has been loaded into message buffer");
  
  return 1;
}
