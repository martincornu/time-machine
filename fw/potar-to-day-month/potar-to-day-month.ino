/**************************************************************************/
/*! 
    @file     potar-to-day-month
    @author   Martin CORNU
	@date	  15/04/21

This program read an analog value from a potentiometer and map it to dd/mm date.
*/
/**************************************************************************/

#define DEBUG

int potPin = 2;    // select the input pin for the potentiometer

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  uint16_t potar = 1;
  uint16_t potarLast = 0;

  uint16_t potarAvg = 1;
  uint16_t potarAvgLast = 0;
  
  uint8_t day = 1;
  uint8_t dayLast = 0;
  
  uint8_t month = 1;
  
  potar = analogRead(potPin) + 1; // add 1 to avoid 0
  
  // debounce potar
  float alpha = 1; // factor to tune
  potarAvg = alpha * potar + (1-alpha) * potarAvgLast;
  
  day = (map(potarAvg, 1, 1024, 1, 384)) % 32; // map analog value to 12 * 32 days (32 because of % -> one 0 value per month)

  if ((day < dayLast) && (potarAvg > potarAvgLast)) {
    month++;
  } else if((day > dayLast) && (potarAvg < potarAvgLast)) {
    month--;
  }

  potarLast = potar;
  potarAvgLast = potarAvg;
  dayLast = day;
  
  #ifdef DEBUG
	  if ((day > 0) && (day < 32) && (month > 0) && (month < 13)) {
	   Serial.print(day, DEC);Serial.print(" / ");Serial.println(month, DEC); 
	  }
  #endif
  
  delay(250);
}
