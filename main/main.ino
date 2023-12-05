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

// Away team colors (Dodgers)
#define AWAY_R 0
#define AWAY_G 0
#define AWAY_B 200

// Home team colors (Padres)
#define HOME_R 200
#define HOME_G 200
#define HOME_B 0

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

    pinMode(A_GREEN, OUTPUT);
    pinMode(A_YELLOW, OUTPUT);
    pinMode(A_BLUE, OUTPUT);
}

// triggers red led lights to display the outs
void displayOuts(int outs) {
    for (int i = 0; i < 3; i++) {
        if (i < outs) {
            digitalWrite(redPins[i], HIGH);
        } else {
            digitalWrite(redPins[i], LOW);
        }
    }
}

int getRunnersOnBase(String savedBases) {
    int runners = 1;
    for (int i = 0; i < 3; i++) {
        if (savedBases.charAt(i) != '0') {
            runners++;
        }
    }
    if (runners == 1) {
        runners = 3;
    }
    return runners;
}

void displayBases(String bases) {
    for (int i = 0; i < 3; i++) {
        if (bases.charAt(i) != '0') {
            digitalWrite(basePins[i], HIGH);
        } else {
            digitalWrite(basePins[i], LOW);
        }
    }
}

void homeRun(int runs) {
    while (runs > 0) {
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

        runs--;
    }
}

void setTeamColor(int r, int g, int b) {
    analogWrite(RGB_RED, r);
    analogWrite(RGB_GREEN, g);
    analogWrite(RGB_BLUE, b);
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

            String teamsFullDisplay = awayTeam + String(" v ") + homeTeam;
            String teamAbrvDisplay = awayAbrv + String(" v ") + homeAbrv;
            String score = awayScore + String("-") + homeScore;
            String count = strikes + String("-") + balls;
            char symbol = (inningHalf.equals("top")) ? 't' : 'b';

            String inningStatus = symbol + String(inning);

            currentText[0] = teamAbrvDisplay;


            if (awayTeam == "TBD") {
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

            } else {

                if (savedLastPlay != lastPlay) {
                    currentText[0] = teamsFullDisplay;
                    currentText[1] = lastPlay;
                    updateScreen();
                    savedLastPlay = lastPlay;

                    if (lastPlay == "Home Run") {
                        int runners = getRunnersOnBase(savedBases);
                        homeRun(runners);
                    }

                    if (lastPlay == "Game End") {
                        currentText[0] = String("Game End");
                        currentText[1] =
                                awayAbrv + String(" - ") + awayScore + String("  ") + homeAbrv + String(" - ") +
                                homeScore;
                        savedLastPlay = "ge";
                        updateScreen();
                    }
                } else {
                    currentText[1] =
                            String("S:") + score + String(" ") + String("C:") + count + String(" ") + inningStatus;
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

                if (inningHalf == "top" && savedLastPlay != "ge") {
                    setTeamColor(AWAY_R, AWAY_G, AWAY_B);
                } else if (savedLastPlay != "ge") {
                    setTeamColor(HOME_R, HOME_G, HOME_B);
                } else {
                    analogWrite(RGB_RED, 0);
                    analogWrite(RGB_GREEN, 0);
                    analogWrite(RGB_BLUE, 0);
                }

                delay(1000); // delay 3 seconds
            }
        }
    }
}


