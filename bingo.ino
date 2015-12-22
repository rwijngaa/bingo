#include <Entropy.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

uint8_t random_byte;
const int button = 2;
const int aantalGetallen = 75;
int buttonState;
int prevButtonState;
int trekkingNummer = 0;
long vorigeBingoGetal = 0;
String vorigeBingoLetter;
long bingoGetal = 0;
String bingoLetter;
int trekking[aantalGetallen+1]; // 0..75, 0=not used

void setup() {
    Serial.begin(9600);
    lcd.begin (16,2);
    pinMode (button, INPUT);
    startBingo(); 
    Entropy.initialize();   
}

void startBingo() {
    vorigeBingoGetal = 0;
    bingoGetal = 0;    
    bingoLetter = "";
    vorigeBingoLetter = "";     
    trekkingNummer = 0;  
    
    // trekking schoonmaken
    for (int i = 0; i <= aantalGetallen; i++) {
      trekking[i] = -1;
    }

    printToLcd (
        "B I N G O",
        "Druk op de knop!");
        
    prevButtonState = digitalRead (button);            
}

void stopBingo() {
    for (int i = 5; i>0; i--) {
    printToLcd (
        "We zijn klaar!",
        "Reset in " + String(i) + " sec."
    );
    delay(1000);
  }
}

void loop() {
    buttonState = digitalRead (button);
    if (buttonState != prevButtonState && buttonState == HIGH)
    {
        if (trekkingNummer == aantalGetallen) {
          stopBingo();
          startBingo();
          return;
        }
        
        trekkingNummer++;
        vorigeBingoGetal = bingoGetal;
        vorigeBingoLetter = bingoLetter;
        doBlinkOn();
        do {
          // bingoGetal = random (1,aantalGetallen+1);//
          bingoGetal = Entropy.random(1,aantalGetallen+1); // returns a value from 1 to 6
          // Serial.println("Trekking " + String(trekkingNummer) + ": " + String(bingoGetal));
        } while (trekking[bingoGetal] != -1);
        doBlinkOff();
        trekking[bingoGetal] = 1; // markeer als geweest
        bingoLetter = bepaalBingoLetter(bingoGetal);
        
        if (trekkingNummer==1) {
          printToLcd (
              "We zijn begonnen",
              "Trek. " + String (trekkingNummer)   + ") " + bingoLetter + " " + bingoGetal
          ); 
        } else {
          printToLcd (
              "Trek. " + String (trekkingNummer-1) + ") " + vorigeBingoLetter + " " + vorigeBingoGetal,
              "Trek. " + String (trekkingNummer)   + ") " + bingoLetter + " " + bingoGetal
          ); 
        }                
       delay (500);
    }
    prevButtonState = buttonState;
}

String bepaalBingoLetter(long getal) {
  if (getal <= 15) {
    return "B";
  } else if (getal <= 30) {
    return "I";
  } else if (getal <= 45) {
    return "N";
  }  else if (getal <= 60) {
    return "G";
  }  else if (getal <= 75) {
    return "O";
  }  
  return "";
}

void printToLcd (String line1, String line2) {
    lcd.clear ();
    lcd.print (line1);
    lcd.setCursor (0, 1);
    lcd.print (line2);
}

void doBlinkOn () {
    lcd.setCursor (15, 1);
    lcd.blink();
}

void doBlinkOff () {
    lcd.setCursor (15, 1);
    lcd.noBlink();
}
