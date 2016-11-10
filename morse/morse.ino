#include <SimpleTimer.h>
#define BUFF_SIZE 256
#define MORSE_LED LED_BUILTIN

char input = 0;
char buffer[BUFF_SIZE];
char* writePointer = buffer;
char* readPointer = buffer;
bool morseCompleted = true;
// Lines in morseMap |1  |2  |3  |4  |5  |6  |7  |8  |9  |10 |11 |12 |13
String morseIndex = "abcdefghijklmnopqrstuvwxyz0123456789.,?!-/:`);(=@&";
const char* morseMap[] = {
    ".-",    "-...",  "-.-.",  "-..",      // 1
    ".",     "..-.",  "--.",   "....",     // 2
    "..",    ".---",  "-.-",   ".-..",     // 3
    "--",    "-.",    "---",   ".--.",     // 4
    "--.-",  ".-.",   "...",   "-",        // 5
    "..-",   "...-",  ".--",   "-..-",     // 6
    "-.--",  "--..",  "-----", ".----",    // 7
    "..---", "...--", "....-", ".....",    // 8
    "-....", "--...", "---..", "----.",    // 9
    ".-.-.-","--..--","..--..","-.-.--",   // 10
    "-....-","-..-.", "---...","-.--.-",   // 11
    "-.--.-","-.-.-", "-.--.", "-...-",    // 12
    ".--.-.",".-...",                      // 13
};

SimpleTimer timer;

void setup() {
    pinMode(MORSE_LED, OUTPUT);

    Serial.begin(9600); 
    Serial.println("Write what you like to output in morse.");
}

void loop() {
    if (Serial.available() > 0) {
        input = Serial.read();
        if (input == -1)
            return;
        
        echo(input);
        storeInput(input);
    }
    
    morseLoop();    
    timer.run();
}

void morseLoop() {
    if (morseCompleted && readPointer != writePointer) {
        morseCompleted = false;
        morse(*readPointer);
        readPointer = next(readPointer);
    }
}

void echo(const char input) {
    if (input == '\r') {
        Serial.println("");
    } else {
        Serial.write(input);
    }
}

void storeInput(const char input) {
    *writePointer = input;
    writePointer = next(writePointer);
}

char* next(char* pointer) {
    char* increaced = pointer + 1;
    
    if ((increaced - buffer) >= int(BUFF_SIZE / sizeof(*buffer))) {
        increaced = buffer;
    }
    
    return increaced;
}

void morse(char character) {
    int index = morseIndex.indexOf(tolower(character));
    long timeout = 0;
    
    if (index != -1) {
        const char* code = morseMap[index];
    
        for (unsigned i = 0; i < strlen(code); i++) {
            timer.setTimeout(timeout, ledOn);
            timeout += (code[i] == '.' ) ? 300 : 1000;
            timer.setTimeout(timeout, ledOff);
            timeout += 300;
        }
    }
    
    timeout += 1000;
    timer.setTimeout(timeout, done);
}

void ledOn() {digitalWrite(MORSE_LED, HIGH);}
void ledOff() {digitalWrite(MORSE_LED, LOW);}
void done() {morseCompleted = true;}