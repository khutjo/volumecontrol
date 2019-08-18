
const int PinA = 2; 

// Used for reading DT signal
const int PinB = 3;  

// Used for the push button switch
const int PinSW = 4;  

// Keep track of last rotary value
int PinLED = 13;

// Updated by the ISR (Interrupt Service Routine)
volatile int virtualPosition = 0;
volatile bool readready = true;

bool led = false;
bool kickout = false;
int mill = 0;

// ------------------------------------------------------------------
// INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT 
// ------------------------------------------------------------------

void isr ()  {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  bool other = digitalRead(PinB);
  if (interruptTime - lastInterruptTime > 100) {
    if (other == HIGH)
      virtualPosition = 1;
    else
      virtualPosition = -1;
    readready = true;
    lastInterruptTime = interruptTime;
  }
}

// ------------------------------------------------------------------
// SETUP    SETUP    SETUP    SETUP    SETUP    SETUP    SETUP    
// ------------------------------------------------------------------



void setup() {
  // Just whilst we debug, view output on serial monitor
  Serial.begin(9600);
  // Rotary pulses are INPUTs
  pinMode(PinA, INPUT_PULLUP);
  pinMode(PinB, INPUT_PULLUP);

  // Switch is floating so use the in-built PULLUP so we don't need a resistor
  pinMode(PinSW, INPUT_PULLUP);

  // Attach the routine to service the interrupt
  attachInterrupt(digitalPinToInterrupt(PinA), isr, FALLING);
pinMode(PinLED,OUTPUT);
}

void loop() {
 if (readready == true){
    if (virtualPosition == 1)
      Serial.println("left");
    if (virtualPosition == -1)
      Serial.println("right");
    virtualPosition = 0;
    readready = false;
 }
 
 if (digitalRead(PinSW) == LOW){
    kickout = true;
    mill = millis();
    Serial.println(millis());
  }
  if (kickout && digitalRead(PinSW) == HIGH && (mill + 10) < millis()){
    kickout = false;
    mill = millis();
    led = !led;
    Serial.println(led == HIGH ? "Led ON" : "Led OFF");
    digitalWrite(PinLED, led);
  }
}
