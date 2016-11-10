/**
 * This sketch makes it posible to directly attach a LED.
 * Attach the anode to pin 4 and catode to pin 5.
 * The INPUT_PULLUP Will always provide a positive current, 
 * even though it is a small one due tot the use of the internal resistor.
 *
 * Set the OUTPUT pin (5) to low to complete the circuit and turn on the light.
 */
void setup()
{
    pinMode(4, INPUT_PULLUP);
    pinMode(5, OUTPUT);
}

void loop() {
    digitalWrite(5, LOW);
    delay(1000);
    digitalWrite(5, HIGH);
    delay(1000);
}