void setup() {
  // put your setup code here, to run once:
  DDRB = DDRB | B00111111;
}

void loop() {
  //PORTB = Bxxxxxx, xxxxxx = [0,63]
  //PORTB = [0,63]; //0-5V, 0-63.


    PORTB = 63;

}
