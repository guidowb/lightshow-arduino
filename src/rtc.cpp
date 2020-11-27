/**************************************************************************************/
/* Real-Time Clock                                                                    */
/**************************************************************************************/

#include <Arduino.h>
#include <Wire.h>

#define RTC_ADDRESS 0x68

void initializeI2C()
{
  Wire.begin();
}

byte decToBcd(byte val) { return( (val/10*16) + (val%10) ); }
byte bcdToDec(byte val) { return( (val/16*10) + (val%16) ); }

void setRTC(
  byte second,
  byte minute,
  byte hour,
  byte dayOfWeek,
  byte dayOfMonth,
  byte month,
  byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void getRTC(
  byte *second,
  byte *minute,
  byte *hour,
  byte *dayOfWeek,
  byte *dayOfMonth,
  byte *month,
  byte *year)
{
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

/**************************************************************************************/
/* Emulated Clock                                                                     */
/**************************************************************************************/

const unsigned long day_millis = 24L * 60L * 60L * 1000L;

unsigned long midnight_millis = 0;
unsigned long start_millis = 0;
unsigned long end_millis = 0;

void setClock(int hour, int minute) {
  unsigned long minutes_since_midnight = hour * 60L + minute;
  unsigned long seconds_since_midnight = minutes_since_midnight * 60L;
  unsigned long  millis_since_midnight = seconds_since_midnight * 1000L;
  midnight_millis = millis() - millis_since_midnight;
}

void getClock(int *hour, int *minute) {
  unsigned long current_millis = (millis() - midnight_millis) % day_millis;
  unsigned long current_seconds = current_millis / 1000L;
  unsigned long current_minutes = current_seconds / 60L;
  unsigned long current_hours = current_minutes / 60L;
  *hour   = (int) current_hours;
  *minute = (int) current_minutes % 60L; 
}

void setTimeWindow(int start_hour, int start_minute, int end_hour, int end_minute) {
  unsigned long minutes_to_start = start_hour * 60L + start_minute;
  unsigned long seconds_to_start = minutes_to_start * 60L;
  unsigned long  millis_to_start = seconds_to_start * 1000L;
  start_millis = millis_to_start;
  unsigned long minutes_to_end   = end_hour * 60L + end_minute;
  unsigned long seconds_to_end   = minutes_to_end * 60L;
  unsigned long  millis_to_end   = seconds_to_end * 1000L;
  end_millis = millis_to_end;
}

bool inTimeWindow() {
  unsigned long current_millis = (millis() - midnight_millis) % day_millis;
  if (end_millis > start_millis) {
    if (current_millis < start_millis) return false;
    if (current_millis > end_millis)   return false;
    return true;
  }
  else {
    if (current_millis < end_millis) return true;
    if (current_millis > start_millis) return true;
    return false;
  }
}
