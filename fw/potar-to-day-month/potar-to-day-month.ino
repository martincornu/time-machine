/**************************************************************************/
/*! 
    @file     potar-to-day-month
    @author   Martin CORNU
	  @date	    15/04/21

This program read an analog value from two potentiometers and them to dd/mm date.
*/
/**************************************************************************/

 #define DEBUG

#define POTAR_DAY_PIN (uint8_t) 2    // select the input pin for the potentiometer controlling the days
#define POTAR_MONTH_PIN (uint8_t) 3    // select the input pin for the potentiometer controlling the months

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  uint16_t potDay = 0;
  uint8_t day = 0;

  uint16_t potMonth = 0;
  uint8_t month = 0;
  
  potDay = analogRead(POTAR_DAY_PIN); 
  potMonth = analogRead(POTAR_MONTH_PIN);
  
  day = map(potDay, 0, 1023, 1, 31); // map analog value to 31 days
  month = map(potMonth, 0, 1023, 1, 12); // map analog value to 12 months
  
  #ifdef DEBUG
	  Serial.print(day, DEC);Serial.print(" / ");Serial.println(month, DEC);
    delay(250);
  #endif
}
