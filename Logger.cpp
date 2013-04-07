/*
Logger.cpp - A library for formatting and writing data to various outputs.
Author: Derek Chafin
Version: 1.0.1
Modified: April 6, 2013
License
This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
*/

#include "Logger.h"

Logger::Logger(log_outputs output, data_formats format, boolean add_line_break, uint8_t chip_select)
{
  _chip_select = chip_select;
  _output = output;
  _format = format;
  _add_line_break = add_line_break;
  _filename = "LOGGER00.CSV";
}

int Logger::init()
{
  int result;

  switch(_output)
  {
    case SD_OUTPUT:
    result = initSD();
    break;
    case SERIAL_OUTPUT:
    result = initSerial();
    break;
  }
  
  return result;
}

int Logger::initSD()
{
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  //Hardware SS pin on the Arduino Mega is pin 53
  pinMode(53, OUTPUT);
#else
  pinMode(10, OUTPUT);
#endif

  if (!SD.begin(_chip_select)) {
    return 1;
  }
  
    // create a new file  
  for (uint8_t i = 0; i < 100; i++) {
    _filename[6] = i/10 + '0';
    _filename[7] = i%10 + '0';
    if (!SD.exists(_filename)) { 
      break;  // leave the loop!
    }
  }
  
  _file = SD.open(_filename, FILE_WRITE);
  
  return 0;
}

int Logger::initSerial()
{
  return 0;
}

void Logger::checkAddLineBreak()
{
  if (_format != BINARY)
  {
    if (_add_line_break)
    {
      switch(_output)
      {
        case SD_OUTPUT:
        _file.println("");
        break;
        case SERIAL_OUTPUT:
        Serial.println("");
        break;
      }
    }
    else
    {
      switch(_output)
      {
        case SD_OUTPUT:
        _file.print(',');
        break;
        case SERIAL_OUTPUT:
        Serial.print(',');
        break;
      }
    }
  
  }
}

void Logger::logAsciiByte(byte data)
{  
    switch(_output)
    {
      case SD_OUTPUT:      
        _file.print(data, DEC);
        break;
      case SERIAL_OUTPUT:
        Serial.print(data, DEC);
        break;
    }
}

void Logger::logBinary(byte data)
{
  switch(_output)
  {
    case SD_OUTPUT:      
      _file.write(data);
      break;
    case SERIAL_OUTPUT:
      Serial.write(data);
      break;
  }
}

void Logger::logAsciiBoolean(byte data)
{
  for (int i=0; i < 8; i++)
  {
    switch(_output)
    {
      case SD_OUTPUT:      
        _file.print(bitRead(data, i), DEC);
        break;
      case SERIAL_OUTPUT:
        Serial.print(bitRead(data, i), DEC);
        break;
    }
  }
}

void Logger::logData(byte data)
{
  static int sync_counter = 0;

  switch(_format)
  {
    case ASCII_BYTE:
      logAsciiByte(data);
    break;
    case BINARY:
      logBinary(data);
    break;
    case ASCII_BOOL:
      logAsciiBoolean(data);
    break;
  }
  
  checkAddLineBreak();
  
  sync_counter++;
  if (sync_counter >= 512)
  {
    if (_output == SD_OUTPUT)
    {
      _file.flush();      
    }
    
    if (_output == SERIAL_OUTPUT)
    {
      Serial.flush();
    }    
    sync_counter = 0;
  }
}

// void Logger::logData(int data)
// {

// }

// void Logger::logData(long data)
// {

// }