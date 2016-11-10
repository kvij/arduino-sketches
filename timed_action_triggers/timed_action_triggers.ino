#define BUTTON_COUNT 3

// Use a namespace because arduino IDE messes up with classes and structs http://stackoverflow.com/questions/19308071/struct-and-arduino#19311292
namespace btn {
  struct Button
  {
    int readyLed; // Ready for input.
    int blockedLed; // Motor is active when blockedLed is HIGH.
    int input;
    bool active = false;
    unsigned long activationMillis = 0;
    unsigned long interval = 3000;
    void pinSetup();
  };
  
  void Button::pinSetup() {
    pinMode (readyLed, OUTPUT);
    pinMode (blockedLed, OUTPUT);
    pinMode (input, INPUT);
    digitalWrite(readyLed,HIGH);
  }
}

btn::Button buttons[BUTTON_COUNT];

void setup() {
  // Knop1
  btn::Button button1;
  button1.readyLed = 8;
  button1.blockedLed =  7;
  button1.input = 5;
  buttons[0] = button1;
  
  // Knop2
  btn::Button button2;
  button2.readyLed = 10;
  button2.blockedLed =  9;
  button2.input = 6;
  buttons[1] = button2;
  
  // Knop3
  btn::Button button3;
  button3.readyLed = 0;
  button3.blockedLed =  1;
  button3.input = 4;
  button3.interval = 1500; // A bit faster
  buttons[2] = button3;
  
  // Perform pinsetup for all buttons
  for (int i = 0; i < BUTTON_COUNT; i++) {
    buttons[i].pinSetup();
  }
}

void buttonLogic(btn::Button &button) {
  unsigned long currentMillis = millis();
  
  if (!button.active) { // Only read button state when motor is not running.
    if (digitalRead(button.input) == HIGH) {
      button.active = true;
      button.activationMillis = currentMillis;
      digitalWrite(button.readyLed, LOW);
      digitalWrite(button.blockedLed, HIGH);
    }
  } else if (currentMillis - button.activationMillis >= button.interval) { // Check if interval has passed since activation
    digitalWrite(button.readyLed, HIGH);
    digitalWrite(button.blockedLed, LOW);
    button.active = false; // Register motor inactive and start checking for input agian.
  }
}

void loop() {
  // Loop over array with buttons
  for (int i = 0; i < BUTTON_COUNT; i++) {
    buttonLogic(buttons[i]);
  }  
}