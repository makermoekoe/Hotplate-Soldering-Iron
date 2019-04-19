#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "max6675.h"


#define COOLDOWN_TIME 60 //60
#define PREHEAT_TIME 60
#define REFLOW_TIME 60


#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

int thermoDO = 12;
int thermoCS = 13;
int thermoCLK = 14;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

const int button = 2;
const int solidstate = 15;


const int temp_preheat = 150;
const int temp_reflow = 240;

int temp_now = 0;
int temp_next = 0;
String state[] = {"OFF", "PREHEAT", "REFLOW", "COOLING"};
int state_now = 0;

int time_count = 0;
int perc = 0;

int offset = 0;

void setup() {
  pinMode(button, INPUT);
  pinMode(solidstate, OUTPUT);
  digitalWrite(solidstate, LOW);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

long t = millis();
long t_solder = millis();

void loop() {
  temp_now = thermocouple.readCelsius();

  if (millis() > t + 200 || millis() < t) {
    PrintScreen(state[state_now], temp_next, temp_now, time_count, perc);
    t = millis();
  }


  if (digitalRead(button) == 0) {
    delay(100);
    int c = 0;
    while (digitalRead(button) == 0) {
      c++;
      if (c > 150) {
        digitalWrite(solidstate, LOW);
        state_now = 0;
        display.fillScreen(WHITE);
        display.setTextColor(BLACK);
        display.setTextSize(2);
        display.setCursor(X(2, 3), Y(2, 0.5));
        display.println("OFF");
        display.display();
        while (digitalRead(button) == 0) delay(1);
        return;
      }
      delay(10);
    }

    t_solder = millis();
    perc = 0,
    state_now++;
    if (state_now == 0) temp_next = 0;
    else if (state_now == 1) temp_next = temp_preheat;
    else if (state_now == 2) temp_next = temp_reflow;
    else if (state_now == 3) temp_next = 0;
    else if (state_now == 4) state_now = 0;
  }


  if (state_now == 1) { //PREHEAT
    regulate_temp(temp_now, temp_next);

    perc = int((float(temp_now) / float(temp_next)) * 100.00);
  }
  else if (state_now == 2) { //REFLOW
    regulate_temp(temp_now, temp_next);

    perc = int((float(temp_now) / float(temp_next)) * 100.00);
    if (perc >= 100) {
      state_now = 3;
      t_solder = millis();
      perc = 0;
      temp_next = 0;
    }
  }
  else if (state_now == 3) { //COOLING
    digitalWrite(solidstate, LOW);

    time_count = int((t_solder + COOLDOWN_TIME * 1000 - millis()) / 1000);
    if (time_count <= 0) {
      state_now = 0;
    }
  }
  else {
    digitalWrite(solidstate, LOW);
    time_count = 0;
  }

  delay(50);
}




void regulate_temp(int temp, int should) {
  if (should < temp - offset) {
    digitalWrite(solidstate, LOW);
  }
  else if (should > temp + offset) {
    digitalWrite(solidstate, HIGH);
  }
}


void PrintScreen(String state, int soll_temp, int ist_temp, int tim, int percentage) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(state);

  display.setCursor(80, 0);
  String str = String(soll_temp) + " deg";
  display.println(str);

  if (tim != 0) {
    display.setCursor(0, 50);
    str = String(tim) + " sec";
    display.println(str);
  }

  if (percentage != 0) {
    display.setCursor(80, 50);
    str = String(percentage) + " %";
    display.println(str);
  }

  display.setTextSize(2);
  display.setCursor(30, 22);
  str = String(ist_temp) + " deg";
  display.println(str);

  display.display();
}


int X(int textgroesse, int n) {
  //gibt die X koordinate aus, damit text mit n zeichen mittig ist

  return (0.5 * (display.width() - textgroesse * (6 * n - 1)));
}//end int X

int Y(int textgroesse, float f) {
  //gibt die Y koordinate aus, damit text mittig ist

  return (f * display.height() - (textgroesse * 4));
}//end int Y
