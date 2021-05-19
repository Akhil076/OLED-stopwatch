#include <Bounce2.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

long int centiseconds = 0;
int seconds = 0;
int minutes = 0;
long int init_time;
long int run_time;
long int sleep_time;
int y = 0;
int z = 0;

bool startState = LOW;
bool resetState = LOW;

const int startButton = 25;
// Instantiate a Bounce object
Bounce startDebouncer1 = Bounce();

const int resetButton = 26;
// Instantiate another Bounce object
Bounce resetDebouncer2 = Bounce();

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

 
void setup() {
  Serial.begin(115200);
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(2000);
  pinMode(startButton, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  startDebouncer1.attach(startButton);
  startDebouncer1.interval(5); // interval in ms

  pinMode(resetButton, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  resetDebouncer2.attach(resetButton);
  resetDebouncer2.interval(5); // interval in ms
  displayTime();
}
 
void loop() {
  if((digitalRead(startButton) == LOW && y < 1)||(resetState)){
    init_time = millis();
    if(resetState == LOW){
      y++;
    }
  }
  startDebouncer1.update();
  if ( startDebouncer1.fell() ) { // Call code if button transitions from HIGH to LOW
    while((z < 1)||(resetState)){
      sleep_time = 0;
      resetState = LOW;
      z++;
    }
    startState = !startState;         // Toggle start button state
  }
  
  if(startState){
    run_time = millis() - sleep_time;
    displayTime();
    setclock();
  }
  if(!startState){
    sleep_time = millis() - run_time;
  }

  resetDebouncer2.update();

  if (resetDebouncer2.fell()){
    resetState = HIGH;
  }

  if (resetState){
    if (startState){
      startState = LOW;
    }
    init_time = 0;
    run_time = 0;
    sleep_time = 0;
    setclock();
    displayTime();
  }
}

void setclock(void){
  int x = run_time - init_time;
  centiseconds = ((x)/10)%100;
  seconds = ((x)/1000)%60;
  minutes = ((x)/60000)%60;
}

void displayTime(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 30);
  if (minutes < 10){
    display.print("0");
    display.print(minutes);
    display.print(":");
  }
  else{
    display.print(minutes);
    display.print(":");
  }
  
  if (seconds < 10){
    display.print("0");
    display.print(seconds);
    display.print(":");
  }
  else{
    display.print(seconds);
    display.print(":");
  }

  if (centiseconds < 10){
    display.print("0");
    display.print(centiseconds);
  }
  else{
    display.print(centiseconds);
  }
  display.display();
}
