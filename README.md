# time-machine
## Time machine arduino for escape room

This program read an analog value from two potentiometers and them to dd/mm date.
Then it display it to 4 large digits from sparkfun.
A second arduino is use for the year. It set SUCCESS_YEAR_PIN to LOW if this is the right year.
If date ddmmyyyy is right, then it activates an output (relay) and display random numbers on digits.