/**************************************************************************/
/*! 
    @file     potar-to-digits-yyyy
    @author   Martin CORNU
    @date     22/04/21

This program read an analog value from a potentiometer and map it to a yyyy year.
Then it display it to 4 large digits from sparkfun.
A second arduino is use for the day and month ddmm.
If year is right, then it activates an output (success signal for second arduino) 
and display random numbers on digits.
*/
/**************************************************************************/

#define DEBUG

#define DIGITS_NUMBER       (uint8_t)4

#define POTAR_YEAR_PIN      (uint8_t)A2    // potar year
#define SUCCESS_YEAR_PIN    (uint8_t)3     // LOW if year is the right one

#define DIGIT_CLK_PIN     (uint8_t)6
#define DIGIT_LAT_PIN     (uint8_t)5
#define DIGIT_SER_PIN     (uint8_t)7

#define SUCCESS_YEAR_YYYY (uint16_t)1997  // year success as yyyy format

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif

  pinMode(DIGIT_CLK_PIN, OUTPUT);
  pinMode(DIGIT_SER_PIN, OUTPUT);
  pinMode(DIGIT_LAT_PIN, OUTPUT);
  digitalWrite(DIGIT_CLK_PIN, LOW);
  digitalWrite(DIGIT_SER_PIN, LOW);
  digitalWrite(DIGIT_LAT_PIN, LOW);

  pinMode(SUCCESS_YEAR_PIN, OUTPUT);
  digitalWrite(SUCCESS_YEAR_PIN, HIGH);
}

void loop() {
  uint16_t potYear = 0;
  uint8_t year = 0;
  
  potYear = analogRead(POTAR_YEAR_PIN); 
  
  year = map(potYear, 0, 1023, 1900, 2100); // map analog value to years
  
  #ifdef DEBUG
    Serial.print(year, DEC);
  #endif

  showNumber(year);

  if(year == SUCCESS_YEAR_YYYY) {
    #ifdef DEBUG
      Serial.println("success!");
    #endif
    digitalWrite(SUCCESS_YEAR_PIN, LOW);

    while(1)
    {
        showNumber(random(0, 9999));      
        delay(250);
    };
  }
  
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
  digitalWrite(DIGIT_LAT_PIN, LOW);
  digitalWrite(DIGIT_LAT_PIN, HIGH); //Register moves storage register on the rising edge of RCK
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
    digitalWrite(DIGIT_CLK_PIN, LOW);
    digitalWrite(DIGIT_SER_PIN, segments & 1 << (7 - x));
    digitalWrite(DIGIT_CLK_PIN, HIGH); //Data transfers to the register on the rising edge of SRCK
  }
}
