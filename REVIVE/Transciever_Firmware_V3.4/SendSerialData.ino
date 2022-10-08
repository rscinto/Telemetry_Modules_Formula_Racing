//Description: Sends data from the head of the linkedlist. 
//Should only be called when the linkedlist is not empty.

void SendSerialData(){

  if(send_ptr == nullptr && head != nullptr){//if send_ptr reaches end of linkedlist, bring it back to the front
    send_ptr = head;
    
  } else {//send data
    if (send_ptr->ready_to_delete == true)
    {
      send_ptr = send_ptr->next;
    }
    else {
      int data_size = send_ptr->frame_len;
      for (int i = 0; i < data_size; i++) {//send data
        Serial.write(send_ptr->frame_ptr[i]);//might cause a bug due to dereferencing and pointer shit
      }
      //mark node as ready to delete, a temp node is created to prevent the node that send_ptr is pointing at from being deleted before send_ptr can go to send_ptr->next
      Node* temp_node = send_ptr;
      send_ptr = send_ptr->next;
      temp_node->ready_to_delete = true;
    }

  }
  if (millis() - time_callback > TIME_THRESHOLD) {//deletion once there are no longer any OnDataReceive Callbacks
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
