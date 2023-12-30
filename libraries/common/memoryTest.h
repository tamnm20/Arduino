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

#define READ_DATA_SIZE 64

const unsigned char programVersion = 5;  

enum SELECT_MODEL{
  SELECT_MODEL_NONE,
  SELECT_MODEL_HERO_5M,
  SELECT_MODEL_ON7_13M_AF,
  SELECT_MODEL_J7_2017_13M_AF,
  SELECT_MODEL_DREAM1_LSI_AF,
  SELECT_MODEL_DREAM1_QUALCOMM_AF,
  SELECT_MODEL_JADE_DUAL,
  SELECT_MODEL_A5A7_13M_AF,
  SELECT_MODEL_A7_16M_FF,
  SELECT_MODEL_C9_16M_FF,
  SELECT_MODEL_JADEVT_16M_FF,
  SELECT_MODEL_A320F_13M,    
  SELECT_MODEL_ON7_8M_FF,
  SELECT_MODEL_A9_8M_FF,
  SELECT_MODEL_J7_LITE_8M_FF,
  SELECT_MODEL_AK7371_AGING,
  SELECT_MODEL_J7_MAX_13M_AF,
  SELECT_MODEL_JACKPOT_DUAL_AF,
  SELECT_MODEL_NEW_J5_13M,
  SELECT_MODEL_MAX,
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
  START_FLAG_STANDBY,
  START_FLAG_PROCESSING,
  START_FLAG_END,
};

enum{
	E2P_MODEL_ADDR,
	E2P_MACHINE_NUMBER,
	E2P_PARA_NUMBER,
	E2P_MAX
};

enum{
	SPI_SUB_CHECK = 'A',
	SPI_SUB_START_FLAG = 'B',
	SPI_SUB_END_FLAG = 'C',
	SPI_MAX
};



#endif // common_h

