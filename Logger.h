/*
Logger.h - A library for formatting and writing data to various outputs.
Author: Derek Chafin
Version: 1.0.1
Modified: April 6, 2013
License
This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
*/

#ifndef Logger_h
#define Logger_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <SD.h>

enum log_outputs
{
  SERIAL_OUTPUT,
  SD_OUTPUT
};

enum data_formats
{
  ASCII_BYTE,
  BINARY,
  ASCII_BOOL
};

class Logger
{
  public:
    Logger(log_outputs output, data_formats format, boolean add_line_break, uint8_t chip_select);
    void logData(byte data);
    int init();
//    void logData(int data);
//    void logData(long data);
  private:    
    int initSD();
    int initSerial();
    void checkAddLineBreak();
    void logAsciiByte(byte data);
    void logBinary(byte data);
    void logAsciiBoolean(byte data);
    char* _filename;
    File _file;
    log_outputs _output;
    data_formats _format;
    boolean _add_line_break;
    uint8_t _chip_select;
};
#endif
