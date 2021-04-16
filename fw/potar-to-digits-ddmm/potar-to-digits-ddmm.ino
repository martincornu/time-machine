/**************************************************************************/
/*! 
    @file     potar-to-digits-ddmm
    @author   Martin CORNU
    @date     16/04/21

This program read an analog value from two potentiometers and them to dd/mm date.
Then it display it to 4 large digits from sparkfun.
*/
/**************************************************************************/

 #define DEBUG

#define POTAR_DAY_PIN   (uint8_t)2    // select the input pin for the potentiometer controlling the days
#define POTAR_MONTH_PIN (uint8_t)3    // select the input pin for the potentiometer controlling the months
#define DIGITS_NUMBER   (uint8_t)4

//segment GPIO declarations
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
byte segmentClock = 6;
byte segmentLatch = 5;
byte segmentData = 7;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);
}

int number = 0;

void loop() {
  uint16_t potDay = 0;
  uint8_t day = 0;

  uint16_t potMonth = 0;
  uint8_t month = 0;

  uint16_t dd_mm = 0; // final number to display as dd/mm
  
  potDay = analogRead(POTAR_DAY_PIN); 
  potMonth = analogRead(POTAR_MONTH_PIN);
  
  day = map(potDay, 0, 1023, 1, 31); // map analog value to 31 days
  month = map(potMonth, 0, 1023, 1, 12); // map analog value to 12 months
  
  #ifdef DEBUG
    Serial.print(day, DEC);Serial.print(" / ");Serial.println(month, DEC);
  #endif

  dd_mm = (day * 100) + month;
  showNumber(dd_mm);
  
  delay(250);
}

//Takes a number and displays 4 numbers. Displays absolute value (no negatives)
void showNumber(float value)
{
  int number = abs(value); //Remove negative signs and any decimals

  for (byte x = 0 ; x < DIGITS_NUMBER ; x++)
  {
    int remainder = number % 10;

    postNumber(remainder, false);

    number /= 10;
  }

  //Latch the current segment data
  digitalWrite(segmentLatch, LOW);
  digitalWrite(segmentLatch, HIGH); //Register moves storage register on the rising edge of RCK
}

//Given a number, or '-', shifts it out to the display
void postNumber(byte number, boolean decimal)
{
  //    -  A
  //   / / F/B
  //    -  G
  //   / / E/C
  //    -. D/DP

#define a  1<<0
#define b  1<<6
#define c  1<<5
#define d  1<<4
#define e  1<<3
#define f  1<<1
#define g  1<<2
#define dp 1<<7

  byte segments;

  switch (number)
  {
    case 1: segments = b | c; break;
    case 2: segments = a | b | d | e | g; break;
    case 3: segments = a | b | c | d | g; break;
    case 4: segments = f | g | b | c; break;
    case 5: segments = a | f | g | c | d; break;
    case 6: segments = a | f | g | e | c | d; break;
    case 7: segments = a | b | c; break;
    case 8: segments = a | b | c | d | e | f | g; break;
    case 9: segments = a | b | c | d | f | g; break;
    case 0: segments = a | b | c | d | e | f; break;
    case ' ': segments = 0; break;
    case 'c': segments = g | e | d; break;
    case '-': segments = g; break;
  }

  if (decimal) segments |= dp;

  //Clock these bits out to the drivers
  for (byte x = 0 ; x < 8 ; x++)
  {
    digitalWrite(segmentClock, LOW);
    digitalWrite(segmentData, segments & 1 << (7 - x));
    digitalWrite(segmentClock, HIGH); //Data transfers to the register on the rising edge of SRCK
  }
}
