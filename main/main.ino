#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

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
const int redPins[] = {RED3, RED2, RED1};

#define RGB_BLUE 4
#define RGB_GREEN 3
#define RGB_RED 2
const int rgbPins[] = {RGB_BLUE, RGB_GREEN, RGB_RED};

#define A_GREEN A2
#define A_YELLOW A3
#define A_BLUE A4
const int basePins[] = {A_GREEN, A_YELLOW, A_BLUE};


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
String currentText[2]; // Array to store text for each row
String savedLastPlay = "";
int savedOuts = 0;
String savedBases = "000";

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

  // digitalWrite(RGB_RED, HIGH);
  // digitalWrite(RGB_GREEN, HIGH);
  // digitalWrite(RGB_BLUE, HIGH);

  pinMode(A_GREEN, OUTPUT);
  pinMode(A_YELLOW, OUTPUT);
  pinMode(A_BLUE, OUTPUT);

  // digitalWrite(A_GREEN, HIGH);
  // digitalWrite(A_YELLOW, HIGH);
  // digitalWrite(A_BLUE, HIGH);



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

void displayBases(String bases) {
  Serial.println("Displaying bases");
  for (int i = 0; i < 3; i++) {
    Serial.print(i);
    Serial.println(bases.charAt(i));
      if (bases.charAt(i) != '0') {
        digitalWrite(basePins[i], HIGH);
    } else {
      digitalWrite(basePins[i], LOW);
    }
  }
}

void homeRun() {
  int j = 2;
  while (j > 0) {
    // Red
    analogWrite(RGB_RED, 255);
    analogWrite(RGB_GREEN, 0);
    analogWrite(RGB_BLUE, 0);
    delay(100);

    // 1st base
    digitalWrite(A_GREEN, HIGH);
    digitalWrite(A_YELLOW, LOW);
    digitalWrite(A_BLUE, LOW);

    // RBG Off
    analogWrite(RGB_RED, 0);
    analogWrite(RGB_GREEN, 0);
    analogWrite(RGB_BLUE, 0);
    delay(100);


    // Green
    analogWrite(RGB_RED, 0);
    analogWrite(RGB_GREEN, 255);
    analogWrite(RGB_BLUE, 0);
    delay(100);

    // 2nd base
    digitalWrite(A_GREEN, LOW);
    digitalWrite(A_YELLOW, HIGH);
    digitalWrite(A_BLUE, LOW);

    // RGB OFF
    analogWrite(RGB_RED, 0);
    analogWrite(RGB_GREEN, 0);
    analogWrite(RGB_BLUE, 0);
    delay(100);

    
    // Blue
    analogWrite(RGB_RED, 0);
    analogWrite(RGB_GREEN, 0);
    analogWrite(RGB_BLUE, 255);
    delay(100);

    // 3rd base
    digitalWrite(A_GREEN, LOW);
    digitalWrite(A_YELLOW, LOW);
    digitalWrite(A_BLUE, HIGH);

    // RBG Off
    analogWrite(RGB_RED, 0);
    analogWrite(RGB_GREEN, 0);
    analogWrite(RGB_BLUE, 0);
    delay(100);


    // White
    analogWrite(RGB_RED, 255);
    analogWrite(RGB_GREEN, 255);
    analogWrite(RGB_BLUE, 255);
    delay(100);

    // RBG off
    analogWrite(RGB_RED, 0);
    analogWrite(RGB_GREEN, 0);
    analogWrite(RGB_BLUE, 0);
    delay(100);

    // Bases off
    digitalWrite(A_GREEN, LOW);
    digitalWrite(A_YELLOW, LOW);
    digitalWrite(A_BLUE, LOW);

    j--;
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
      Serial.println(jsonStr); // Print the JSON string received
  
    } else {
            Serial.println(jsonStr); // Print the JSON string received

        // Access JSON data
        String awayTeam = game_data["aT"];
        String awayAbrv = game_data["aA"];
        String homeTeam = game_data["hT"];
        String homeAbrv = game_data["hA"];
        int awayScore = game_data["aS"];
        int homeScore = game_data["hS"];
        int strikes = game_data["cnt"][0];
        int balls = game_data["cnt"][1];
        int inning = game_data["in"];
        String inningHalf = game_data["iH"];
        String lastPlay = game_data["lP"];
        int outs = game_data["outs"];
        String bases = game_data["bases"];

        Serial.print("Strikes: ");
        Serial.println(bases);

        String teamsFullDisplay = awayTeam + String(" v ") + homeTeam;
        String teamAbrvDisplay = awayAbrv + String(" v ") + homeAbrv;
        String score = awayScore + String("-") + homeScore;
        String count = strikes + String("-") + balls;
        char symbol = (inningHalf.equals("top")) ? 't' : 'b';

        String inningStatus = symbol + String(inning);



        Serial.println(strikes);
        Serial.println(balls);
        // Serial.println(score);
        // Serial.println(count);
        // Serial.println(inning);

        Serial.println("Last Plays:");
        Serial.println(savedLastPlay);
        Serial.println(lastPlay);
        Serial.println();

        currentText[0] = teamAbrvDisplay;


        if (awayTeam == "TBD"){
          currentText[0] = String("No Games");
          currentText[1] = String("Right Now");
          updateScreen();

          // Outs
          digitalWrite(RED1, LOW);
          digitalWrite(RED2, LOW);
          digitalWrite(RED3, LOW);

          // Bases
          digitalWrite(A_GREEN, LOW);
          digitalWrite(A_YELLOW, LOW);
          digitalWrite(A_BLUE, LOW);

          // RGB
          analogWrite(RGB_RED, 0);
          analogWrite(RGB_GREEN, 0);
          analogWrite(RGB_BLUE, 0);

          savedLastPlay = "";

        } else{

        if (savedLastPlay != lastPlay) {
          currentText[0] = teamsFullDisplay;
          currentText[1] = lastPlay;
          updateScreen();
          savedLastPlay = lastPlay;

          if (lastPlay == "Home Run") {
            homeRun();
          }

          if (lastPlay == "Game End") {
            currentText[0] = String("Game End");
            currentText[1] = awayAbrv + String(" - ") + awayScore + String("  ") + homeAbrv + String(" - ") + homeScore;
            savedLastPlay = "ge";
            updateScreen();
          }
        } else {
          currentText[1] = String("S:") + score + String(" ") + String("C:") + count + String(" ") + inningStatus;
          updateScreen();
        }


        if (savedOuts != outs) {
          displayOuts(outs);
          savedOuts = outs;
        }

        if (savedBases != bases) {
          displayBases(bases);
          savedBases = bases;
        }   

      delay(1000); // delay 3 seconds
    }
  }
}

