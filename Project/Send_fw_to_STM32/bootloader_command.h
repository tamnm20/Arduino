typedef enum
{
	OTA_CODE,
	OTA_INFOR,
	OTA_DATA,
	OTA_RESPONSE	
}CommandId;

typedef enum
{
	START_CODE,
	STOP_CODE,
	REQUEST_CODE	
}OTA_Code_Name;

typedef enum
{
	ACK,
	NACK	
}OTA_Response_Name;

// typedef struct
// {
// 	uint8_t commandID;
// 	uint8_t len;
// 	uint8_t ota_code;
// }__attribute__((packed))OTACode;

// typedef struct
// {
// 	uint8_t commandID;
// 	uint8_t len;
// 	uint8_t ack;
// }__attribute__((packed))OTAResponse;
// typedef struct
// {
// 	uint8_t command_id;
// 	uint8_t len;
// 	uint8_t name[50];
//     uint8_t version[5];
// }__attribute__((packed))OTAInfor;

typedef struct
{
	uint8_t command_id;
	uint8_t len;
	uint8_t ota_code;
}OTACode;

typedef struct
{
	uint8_t command_id;
	uint8_t len;
	uint8_t ack;
}OTAResponse;

typedef struct
{
	uint8_t command_id;
	uint8_t len;
	uint8_t name[50];
    uint8_t version[5];
}OTAInfor;
