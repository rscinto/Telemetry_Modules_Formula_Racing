//Waits for the start flag (0xFF 0xFF), then tries to read a message
uint8_t tryToConnect()
{
  while(!Serial.available())
  {
  }
  
  while(Serial.available())
  {
    buf[1] = buf[0];
    buf[0] = Serial.read();
    
    if(compareArray(buf, startFlag, 2))
    {
      Serial.println("After compare array");
      buf[0] = 0; buf[1] = 0;
      if(tryReadMessage(&msg))
      {
        Serial.println("After tryreadmessage");
        if(compareMessage(&msg, &handshake))
        {
          Serial.println("After Compare message");
          return 1;
        }
        
        buf[0] = 0;//This resets the buffer so that it does not get read in if it reads in only one 0xFF
        buf[1] = 0;
        
      }
    } 
  }
  return 0;
}
