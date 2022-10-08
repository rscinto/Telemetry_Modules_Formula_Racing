// Possible problems: Dont recall, but I am assuming this is only called on successful recieves.
// Not sure if there are ownership problems wen the temp node is transfered into the linkedlist

// Description: Callback function when the esp32 recieves data. It converts the 
// recieved message in to the message structure. It then adds it to the end of
// the linked list.
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  time_callback = millis();
  uint8_t *recieved_frame = (uint8_t*)malloc(len * sizeof(uint8_t));
  for (int i = 0; i < len ; i++) {
    recieved_frame[i] = incomingData[i];
  }

  
  if (list_size == 0) {//Case where the linkedlist is empty
    Node* temp_node = new Node();
    temp_node->next = nullptr;
    temp_node->frame_ptr = recieved_frame;
    temp_node->frame_len = len;
    temp_node->ready_to_delete = false;
    tail = temp_node;
    head = temp_node;
    send_ptr = temp_node;
    list_size++;
    Serial.println();
    Serial.print("add empty: ");
    Serial.println(list_size);
  }
  else {// Case where linkedlist is not empty
    Node* temp_node = new Node();
    temp_node->next = nullptr;
    temp_node->frame_ptr = recieved_frame;
    temp_node->frame_len = len;
    temp_node->ready_to_delete = false;
    tail->next = temp_node;
    tail = tail->next;
    temp_node = nullptr;
    list_size++;
    Serial.println();
    Serial.print("add: ");
    Serial.println(list_size);
  }
  
  if(head->ready_to_delete){//delete from head if ready_to_Delete is true
    free(head->frame_ptr);
    Node* temp_node = head;
    head = head->next;
    free(temp_node);

    list_size--;
    Serial.println();
    Serial.print("delete: ");
    Serial.println(list_size);
  }
}
