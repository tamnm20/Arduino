typedef enum
OTA_state ota_state = OTA_IDLE
void start_ota()
{
  ota_state = OTA_START_STATE;
  send_frame(frame_header);
}
void app_handeler()
{
  switch(ota_state)
  {
    case OTA_START_STATE:
      if(ACK)
      {
        ota_state = OTA_HEADER_STATE;
        send_frame(frame_header);
      }
      break;
    case OTA_HEADER_STATE:
     if(ACK)
      {
        ota_state = OTA_DATA_STATE;
        send_frame(frame_data);
      }
      break;
    case OTA_DATA_STATE:
     if(ACK)
      {
        //checksum ok 
        ota_state = OTA_DATA_STATE;
        send_frame(frame_data);
      }
      break;
    case OTA_END_STATE:
      break;
  }
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
