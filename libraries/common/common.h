// SoftwareWire.h
//
// 2008, Raul wrote a I2C with bit banging as an exercise.
// http://codinglab.blogspot.nl/2008/10/i2c-on-avr-using-bit-banging.html
//
// 2010-2012, Tod E. Kurt takes some tricks from Raul,
// and wrote the SoftI2CMaster library for the Arduino environment.
// https://github.com/todbot/SoftI2CMaster
// http://todbot.com/blog/
//
// 2014-2015, Testato updates the SoftI2CMaster library to make it faster
// and to make it compatible with the Arduino 1.x API
// Also changed I2C waveform and added speed selection.
//
// 2015, Peter_n renames the library into "SoftwareWire",
// and made it a drop-in replacement for the Wire library.
//

#ifndef common_h
#define common_h

#define READ_DATA_SIZE 32

#define SUB_MODEL_MAIN

const unsigned char programVersion = 44 ;  


enum SELECT_MODEL{
  SELECT_MODEL_NONE,
  SELECT_MODEL_SAMPLE,
  SELECT_MODEL_PRODUCT,
  SELECT_MODEL_MAX,
};


enum{
  ERR_NO3_NONE       = 0b00000000,
  ERR_NO3_CRC        = 0b00000001,
  ERR_NO3_F        = 0b00000010,
  ERR_NO3_E         = 0b00000100,
  ERR_NO3_D        = 0b00001000,
  ERR_NO3_C = 0b00010000,
  ERR_NO3_B = 0b00100000,
  ERR_NO3_A  = 0b01000000,
//  ERR_I2C           = 0b10000000,
};

enum{
  ERR_NO_NONE       = 0b00000000,
  ERR_NO_CRC        = 0b00000001,
  ERR_NO_AWB        = 0b00000010,
  ERR_NO_AF         = 0b00000100,
  ERR_NO_LSC        = 0b00001000,
  ERR_NO_MODEL_NAME = 0b00010000,
  ERR_NO_MODULE_VER = 0b00100000,
  ERR_NO_SENSOR_ID  = 0b01000000,
  ERR_I2C           = 0b10000000,
};
enum{
  ERR2_NO_NONE       = 0b00000000,
  ERR2_NO_CRC        = 0b00000001,
  ERR2_MF_SKIP       = 0b00000010,
  ERR2_SENSOR_INFO   = 0b00000100,
  ERR2_EMPTY_DATA    = 0b00001000,
  ERR2_AF	     = 0b00010000,
  ERR2_FIXD_DATA     = 0b00100000,
  ERR2_DF_SKIP       = 0b01000000,
  ERR2_I2C           = 0b10000000,
};


struct LogData{
  unsigned char sensorId[16];
  unsigned char errFlag;
  unsigned char crc;
  unsigned char checkSumHeader[4];
  unsigned char checkSumShading[4];
  unsigned char dumy[32-16-1-1-4-4];
}logData;

enum{
  I2C_COMMAND_START = 0x0,
  I2C_COMMAND_READ_RESULT = 0x10,
  I2C_COMMAND_READ_STATE = 0x20,
  I2C_COMMAND_TIME_OVER_STATE = 0x30,
  I2C_COMMAND_VERSION_CHECK = 0x40,
  I2C_COMMAND_MACHINE_NUMBER = 0x50,
  I2C_COMMAND_READ_RESULT2   = 0x60,
  I2C_COMMAND_MAX
};

#endif // common_h

