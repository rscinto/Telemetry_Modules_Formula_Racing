
bool BroadcastData(message* m){
  Serial.println();
  Serial.println("Broadcasting Message");

  uint8_t abuf[255];
  uint8_t len = msgToFrame(m, abuf);

  return esp_now_send(broadcastAddress, abuf, len);
}
