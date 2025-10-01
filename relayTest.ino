/* accept and process commands ending with '\n' - commands must not exceed 1024 characters */

#define LED_PIN 13

#define RELAY1_PIN 12
#define RELAY2_PIN 11
#define RELAY3_PIN 10
#define RELAY4_PIN 9

void setup() {
    /* Comms */
    Serial.begin(57600);
    /* LED pin */
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    /* relay pins */
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT);
    pinMode(RELAY4_PIN, OUTPUT);
    digitalWrite(RELAY1_PIN, HIGH);
    digitalWrite(RELAY2_PIN, HIGH);
    digitalWrite(RELAY3_PIN, HIGH);
    digitalWrite(RELAY4_PIN, HIGH);
}

int32_t readingCommand = 0;
char command[1024];

void processCommand() {
    if (strcmp(command, "help") == 0) {
        Serial.print("Commands:\r\n");
        Serial.print("- led on: turn on red LED\r\n");
        Serial.print("- led off: turn off red LED\r\n");
        Serial.print("- 1on: turn on RELAY1\r\n");
        Serial.print("- 1off: turn off RELAY1\r\n");
        Serial.print("- 2on: turn on RELAY2\r\n");
        Serial.print("- 2off: turn off RELAY2\r\n");
        Serial.print("- 3on: turn on RELAY3\r\n");
        Serial.print("- 3off: turn off RELAY3\r\n");
        Serial.print("- 4on: turn on RELAY4\r\n");
        Serial.print("- 4off: turn off RELAY4\r\n");
    } else if (strcmp(command, "led on") == 0) {
        digitalWrite(LED_PIN, HIGH);
        Serial.print("LED on\r\n");
    } else if (strcmp(command, "led off") == 0) {
        digitalWrite(LED_PIN, LOW);
        Serial.print("LED off\r\n");
    } else if (strcmp(command, "1on") == 0) {
        digitalWrite(RELAY1_PIN, LOW);
        Serial.print("RELAY1 on\r\n");
    } else if (strcmp(command, "1off") == 0) {
        digitalWrite(RELAY1_PIN, HIGH);
        Serial.print("RELAY1 off\r\n");
    } else if (strcmp(command, "2on") == 0) {
        digitalWrite(RELAY2_PIN, LOW);
        Serial.print("RELAY2 on\r\n");
    } else if (strcmp(command, "2off") == 0) {
        digitalWrite(RELAY2_PIN, HIGH);
        Serial.print("RELAY2 off\r\n");
    } else if (strcmp(command, "3on") == 0) {
        digitalWrite(RELAY3_PIN, LOW);
        Serial.print("RELAY3 on\r\n");
    } else if (strcmp(command, "3off") == 0) {
        digitalWrite(RELAY3_PIN, HIGH);
        Serial.print("RELAY3 off\r\n");
    } else if (strcmp(command, "4on") == 0) {
        digitalWrite(RELAY4_PIN, LOW);
        Serial.print("RELAY4 on\r\n");
    } else if (strcmp(command, "4off") == 0) {
        digitalWrite(RELAY4_PIN, HIGH);
        Serial.print("RELAY4 off\r\n");
    } else {
        Serial.print("Unknown Command: ");
        Serial.println(command);
    }
}

void loop() {
    int32_t bytesAvailable = Serial.available();
    for (int32_t i = 0; i < bytesAvailable; i++) {
        command[readingCommand] = Serial.read();
        if (command[readingCommand] == '\n') {
            command[readingCommand] = '\0';
            readingCommand = -1;
            processCommand();
        }
        if (readingCommand < 1023) {
            readingCommand++;
        }
    }
    delay(100);
}
