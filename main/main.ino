//www.elegoo.com
//2016.12.9

/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 10
 * LCD D5 pin to digital pin 11
 * LCD D6 pin to digital pin 12
 * LCD D7 pin to digital pin 13
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <Arduino_JSON.h>

#define RED1 7
#define RED2 6
#define RED3 5
const int redPins[] = {7, 6, 5};

#define RGB_BLUE 4
#define RGB_GREEN 3
#define RGB_RED 2

const int rgbPins[] = {4, 3, 2};

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
String currentText[2]; // Array to store text for each row
String savedLastPlay = "";
int savedOuts = 0;

// rgb values
int redValue;
int greenValue;
int blueValue;


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);

  // red lights
  pinMode(RED1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(RED3, OUTPUT);

  // rgb pins
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  
  redValue = 64;
  greenValue = 224;
  blueValue = 208;

  digitalWrite(RGB_RED, HIGH);
  digitalWrite(RGB_GREEN, HIGH);
  digitalWrite(RGB_BLUE, HIGH);


  // HIGH for on LOW for off
  // digitalWrite(RED1, HIGH);
  // digitalWrite(RED2, HIGH);
  // digitalWrite(RED3, HIGH);
}

// triggers red led lights to display the outs
void displayOuts(int outs) {
  for (int i = 0; i < 3; i ++) {
    if (i < outs) {
      Serial.println(redPins[i]);
      digitalWrite(redPins[i], HIGH);
    } else {
      digitalWrite(redPins[i], LOW);
    }
  }
}

void updateScreen() {
  lcd.clear(); // Clear the display
  lcd.setCursor(0, 0); // Set cursor to start of first row
  lcd.print(currentText[0]); // Display text for the first row
  lcd.setCursor(0, 1); // Set cursor to start of second row
  lcd.print(currentText[1]); // Display text for the second row
}

void loop() {
  if (Serial.available()) {
    Serial.println("Connected");
    String jsonStr = Serial.readStringUntil('\n'); // Read the incoming JSON string

    JSONVar game_data = JSON.parse(jsonStr); // Parse the received JSON string

    if (JSON.typeof(game_data) == "undefined") {
      Serial.println("Failed to parse JSON");
      return;
    }

    // Access JSON data
    String awayTeam = game_data["away-team"];
    String homeTeam = game_data["home-team"];
    int awayScore = game_data["away-score"];
    int homeScore = game_data["home-score"];
    int strikes = game_data["count"][0];
    int balls = game_data["count"][1];
    int inning = game_data["inning"];
    String inningHalf = game_data["inning-half"];
    String lastPlay = game_data["last-play"];
    int outs = game_data["outs"];

    String teams = awayTeam + String(" v ") + homeTeam;
    String score = awayScore + String("-") + homeScore;
    String count = strikes + String("-") + balls;
    char symbol = (inningHalf.equals("top")) ? 't' : 'b';

    String inningStatus = symbol + String(inning);


    // Serial.println(teams);
    // Serial.println(score);
    // Serial.println(count);
    // Serial.println(inning);

    currentText[0] = teams;

    if (savedLastPlay != lastPlay) {
      currentText[1] = lastPlay;
      updateScreen();
      savedLastPlay = lastPlay;
      delay(2000); // delay 2 seconds
    } else {
      currentText[1] = String("S:") + score + String(" ") + String("C:") + count + String(" ") + inningStatus;
      updateScreen();
    }


    if (savedOuts != outs) {
      displayOuts(outs);
      savedOuts = outs;
    }


    // Determine the size of the array
    // int arraySize = sizeof(currentText) / sizeof(currentText[0]);

    // // Iterate through each string in the array and print its contents
    // for (int i = 0; i < arraySize; i++) {
    //   Serial.println(currentText[i]);
    // }
    

  // Add other functionalities or actions here
  }
}

