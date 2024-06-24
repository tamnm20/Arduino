#include "download_file.h"
#include "bootloader_command.h"
byte recv[3]={0};
typedef enum
{
	OTA_IDLE_STATE,
	OTA_START_STATE,
	OTA_SEND_INFOR_STATE,
	OTA_SEND_DATA_STATE,
	OTA_END_STATE
}OTA_State_Typedef;
OTA_State_Typedef ota_state;
void bootloader_send_data(void* data, uint8_t len)
{
	UART1_Send_String((uint8_t*)data,len);
}
void ota_send_code(OTA_Code_Name ota_code)
{
		OTACode cmd;
		cmd.command_id = OTA_CODE;
		cmd.len = 1;
		cmd.ota_code = ota_code;
		bootloader_send_data(&cmd,sizeof(cmd));
}
void ota_send_response(OTA_Response_Name ack)
{
		OTAResponse response;
		response.command_id = OTA_CODE;
		response.len = 1;
		response.ack = ack;
		bootloader_send_data(&response,sizeof(response));
}
void ota_send_infor(void)
{
	OTAInfor infor;
	infor.command_id = OTA_INFOR;
	infor.len = sizeof(infor.name)+sizeof(infor.version);
	strcpy((char*)&infor.name,"Blink");
	strcpy((char*)&infor.version,"v1.1");
	bootloader_send_data(&infor,sizeof(infor));
}
void ota_send_data(uint8_t * data, uint8_t len)
{
	OTAData cmd;
	cmd.command_id = OTA_DATA;
	cmd.len = len;
	memcpy(&cmd.data, data, len);
	bootloader_send_data(&cmd,sizeof(cmd));
}
void app_handler(void)
{
	switch(ota_state)
	{
		case OTA_IDLE_STATE:
			{
				// recv: 0x00, 0x01, 0x02
				OTACode *ota_code = (OTACode*)recv;
				if(ota_code->command_id == OTA_CODE && ota_code->ota_code ==REQUEST_CODE)
				{
					ota_state = OTA_START_STATE;
					ota_send_code(START_CODE);
				}
			}	
			break;
		case OTA_START_STATE:
			{
				//neu nhan ACK gui infor
				OTAResponse *response = (OTAResponse*)recv;
				if(response->command_id == OTA_RESPONSE && response->ack = ACK)
				{
					ota_state = OTA_SEND_INFOR_STATE;
					ota_send_infor();
				}
				else
				{
					ota_state = OTA_IDLE_STATE;
				}
			}
			break;
		case OTA_SEND_INFOR_STATE:
			{
				OTAResponse *response = (OTAResponse*)recv;
				if(response->command_id == OTA_RESPONSE && response->ack = ACK)				{
					uint8_t hex_data[21];
					char* token = strtok(file_hex,"\n");
					//covert_string_intel

					ota_send_data();
				}
				else
				{
					ota_state = OTA_IDLE_STATE;
				}			
			}
			break;
		case OTA_SEND_DATA_STATE:
			{
				OTAResponse *response = (OTAResponse*)recv;
				if(response->command_id == OTA_RESPONSE && response->ack = ACK)				{
					uint8_t hex_data[21];
					char* token = strtok(NULL,"\n");
					//covert_string_intel
					if(token!=NULL)
					{
						if(hex_data[3]==0x00)
						{
							ota_send_data();
						}
						else
						{
							ota_send_code(END_CODE);
							ota_state = OTA_END_STATE;
						}
					}
					else
					{
						ota_send_code(END_CODE);
					}
				}		
			}
			break;
		default:
			break;
	}
}
void host_bootloader_handle()
{
    //Su ly data nhan ve
}
void host_bootloader_init()
{
    //init uart
	ota_state = OTA_IDLE_STATE;
}
