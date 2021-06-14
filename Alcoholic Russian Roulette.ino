// LIBRARIES
#include <ServoEasing.h> // for servo motor
#include <Wire.h>               // deals with I2C connections
#include <LiquidCrystal_I2C.h>  // activates the LCD I2C library

ServoEasing myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int Players = "";
int Shots = "";
int randNumber;
int pos = 0; // Store servo position
String stringOne;

char * introMessage = " Welcome to Shot Roulette ";

// MENU ARRAYS
String menuOption[] = {{"Select Players"}, {"Select Shots"}, {"Game Over"}};       // Text on the top line

// PINS ON THE ARDUINO.
// MAKE THESE "CONST INT" VARIABLES AS THESE DO NOT CHANGE IN THE PROGRAM.
const int upButtonPin = 2;         // Pin 7 for "Up" command
const int downButtonPin = 3;       // Pin 8 for "Down" command
const int enterButtonPin = 4;      // Pin 9 for "Enter" command
const int clearButtonPin = 5;     // Pin 10 for "Clear" command

const int buzzer = 10;

// NUMBER COUNT OF BUTTON PRESSES AND COUNTER UNITS.
// MAKE THESE A "INT" VARIABLES TO ALLOW FOR NEGATIVE INTEGERS.
int setting1Counter = 0;    // Counters for settings 1 - 5
int setting2Counter = 1;
char* setting3Counter = "" ;
int selectPressCount = 0;
int directionPush = 0;
int upPressCount = 0;       // This counter measures the amount of times the user pushes the "up" button.
int downPressCount = 0;     // This counter measures the amount of times the user pushes the "down" button.

// BUTTON PRESS STATES FOR EACH FUNCTION, ALL SET TO "LOW".
// MAKE THESE "BOOLEAN" VARIABLES AS THESE ONLY WILL BE "HIGH" OR "LOW".
boolean buttonStateUp = LOW;              // Button states for the "Up" command
boolean lastButtonStateUp = LOW;                
boolean currentButtonStateUp = LOW;
boolean buttonStateDown = LOW;            // Button states for the "Down" command
boolean lastButtonStateDown = LOW;                
boolean currentButtonStateDown = LOW; 
boolean buttonStateEnter = LOW;           // Button states for the "Enter" command
boolean lastButtonStateEnter = LOW;                
boolean currentButtonStateEnter = LOW;
boolean buttonStateClear = LOW;           // Button states for the "Clear" command
boolean lastButtonStateClear = LOW;                
boolean currentButtonStateClear = LOW;

// DEBOUNCE VARIABLES TO MEASURE THE DEBOUNCING TIME OF A BUTTON PUSH.
// MAKE THESE "UNSIGNED LONG" VARIABLES AS THE NUMERICAL VALUE WILL HAVE AN EXTENDED SIZE.
unsigned long lastDebounceTime = 0;       // This variable takes a "snapshot" of time when any button is pushed.
unsigned long debounceDelay = 50;         // Delay time in milliseconds; the amount of time the button is pressed must be higher than the delay to register a push.


// OBJECT DECLARATION
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Activates the LCD "object"



// SETUP
void setup() {
  randomSeed(analogRead(0));
  lcd.backlight();                  // THESE COMMANDS TURN ON AND CLEAR THE LCD SCREEN
  lcd.init();
  lcd.setCursor(15,0);
  delay(500);
  for(int positionCounter = 0; positionCounter < 25; positionCounter++) {
    lcd.print(introMessage[positionCounter]);
    lcd.scrollDisplayLeft();
    delay(125);
  }
  delay(2000);
  lcd.clear();
  myservo.attach(9);
  myservo.write(180);
  pinMode(upButtonPin, INPUT_PULLUP);      // SETS THE upButtonPin AS AN INPUT
  pinMode(downButtonPin, INPUT_PULLUP);    // SETS THE downButtonPin AS AN INPUT
  pinMode(enterButtonPin, INPUT_PULLUP);   // SETS THE enterButtonPin AS AN INPUT
  pinMode(clearButtonPin, INPUT_PULLUP);   // SETS THE clearButtonPin AS AN INPUT
  
  delay(1000);

}



// MAIN LOOP

// FUNCTION FOR WAITING UNTIL NEXT PLAYER IS READY
void buttonWait(int enterButtonPin){ 
  int buttonStateEnter = 0;
  while(1){
    buttonStateEnter = digitalRead(enterButtonPin);
    if (buttonStateEnter == LOW) {
      return;
    }
  }
}

void loop() {
  
  lcd.setCursor(0,0);                           // Menu displayed on the LCD.
  lcd.print(menuOption[selectPressCount]);         // The menuOption that is displayed is determined by the number of selections pressed
  lcd.setCursor(0,1);    
  
  if (selectPressCount == 0) // The setting counter that is displayed is determined by number of selection pressed
  {
    lcd.print(setting1Counter);
  }
  else if (selectPressCount == 1)
  {
    lcd.print(setting2Counter);
  }
  else if (selectPressCount == 2)
  {
    lcd.print(setting3Counter);
  }


  if (setting1Counter <= 1 && selectPressCount == 1) // ensures more than 2 people are playing always
  {
    setting1Counter = 0;
    selectPressCount = lastButtonStateEnter;
    lcd.setCursor(0,1);
    lcd.print("Got no friends?");
    delay(2500);
    lcd.clear();
  }

  // The program at this point is waiting for a button press.
  currentButtonStateUp = digitalRead(upButtonPin);          
  currentButtonStateDown = digitalRead(downButtonPin);    
  currentButtonStateEnter = digitalRead(enterButtonPin);
  currentButtonStateClear = digitalRead(clearButtonPin);    
  
  if (currentButtonStateUp != lastButtonStateUp || currentButtonStateDown != lastButtonStateDown || currentButtonStateEnter != lastButtonStateEnter)        
  // If there is a button push on any of the buttons, the following routine runs to check if it was a valid press:
  {
  lastDebounceTime = millis();      // lastDebounceTime is set equal to the running millis() function.
  }  

  if ((millis() - lastDebounceTime) > debounceDelay)   
  // If the lastDebounceTime (aka. the "snapshot" time) minus the running millis() function is higher than the set debounce delay, the following routine
  // below runs and checks which button was pushed: 
  {                                             

    // The current state for each button is set not equal to the pressed state and when it changes, the pressed state becomes equal to the current state.

    // UP BUTTON PRESS
    if (currentButtonStateUp != buttonStateUp)           // Up button scrolls the setting upward. 
    { 
    buttonStateUp = currentButtonStateUp;  
                 
      if (buttonStateUp == LOW && selectPressCount == 0)    // Can only change variable in the first menu                      
      {                
      upPressCount++;     
      downPressCount = 0;                                // The downPressCount is reset to zero.    
      setting1Counter++;

        if (upPressCount > 15)                            // Maximum players currently capped at 15 people (can change if we want to)
        {                                                
        setting1Counter = 15;
        lcd.setCursor(0,1);
        lcd.print("Max Players"); 
        delay(2500);
        }
      }  
      
      if (buttonStateUp == LOW && selectPressCount == 1)  // Can only change variables in the second menu                       
      {            
      upPressCount++;     
      downPressCount = 0;                                             
      setting2Counter++;

        if (upPressCount >= setting1Counter) // Number of shots cannot exceed number of players
        {
        setting2Counter = setting1Counter;
        lcd.setCursor(0,1);
        lcd.print("Max Shots");
        delay(2500);
        }
      } 

      if (buttonStateUp == LOW && selectPressCount == 2)  // Proceeds to game                       
      {            
      upPressCount++;     
      downPressCount = 0;                                             
      setting3Counter++;

        if (upPressCount > 1)
        {
        lcd.setCursor(0,1);
        lcd.print("");
        delay(2500);
        }
      }  
    lcd.clear();  
    }


    // DOWN BUTTON PRESS
    if (currentButtonStateDown != buttonStateDown)        // Down button scrolls the setting downward.
    { 
    buttonStateDown = currentButtonStateDown;  
                 
      if (buttonStateDown == LOW && selectPressCount == 0)   // The first 5 times in which the "down" button is pushed, each push will subtract 1 increment to the setting.                      
      {                      
      downPressCount++;
      upPressCount = 0;                                   // The upPressCount is reset to zero.
      setting1Counter--;

        if (setting1Counter < 0)                           // If the "down" button is pushed more than 5 times consecutively, the setting increment decreases by 5
        {                                                 // with every "down" button push and resets back when the up, left or right button is pushed.
        setting1Counter = 0;
        }
      }  

      if (buttonStateDown == LOW && selectPressCount == 1)                         
      {              
      downPressCount++;
      upPressCount = 0;                                         
      setting2Counter--;

        if (setting2Counter < 1)
        {
        setting2Counter = 1;
        }
        
      }  

    lcd.clear();
    }
     
  }  


    // ENTER BUTTON PRESS
    if (currentButtonStateEnter != buttonStateEnter)        
    { 
    buttonStateEnter = currentButtonStateEnter; 

    if (buttonStateEnter == LOW)
    {
      selectPressCount++;
      upPressCount = 0;
      downPressCount = 0;

      if (selectPressCount > 2) {
        selectPressCount = 0;
      }
    }
    lcd.clear();
   
    
      if (buttonStateEnter == LOW && selectPressCount == 2)      // The Enter button simply enters the setting and flashes a brief message.                         
      {                                                       // Please feel free to expand on this code to add more functions
      lcd.clear();
      lcd.setCursor(8,0); 
      lcd.print(setting1Counter);
      lcd.setCursor(5,1);
      lcd.print("Players");
      delay(1800);
      Players = setting1Counter;
      setting1Counter = 0;                                                                             
      lcd.clear();
      lcd.setCursor(8,0);
      lcd.print(setting2Counter);
      lcd.setCursor(6,1);
      lcd.print("Shots");
      delay(1800);
      Shots = setting2Counter;
      setting2Counter = 0;
      lcd.clear();

    while(1){
    stringOne += "0";
    if (stringOne.length() == Players) {
      break;
      }
    }

    int i = 0;
    while (i<Shots) {
      randNumber = random(Players);
      if (stringOne[randNumber] != '1'){
        stringOne[randNumber] = '1';
        i = i + 1;
        }
      }

      for (int n = 0; n < stringOne.length(); n++) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Goodluck...");
      buttonWait(enterButtonPin);
      if (buttonStateEnter == LOW){
        if (stringOne.charAt(n) == '1'){
              delay(500);
              lcd.setCursor(0,1);
              lcd.print("3");
              tone(buzzer, 1500);
              delay(500);
              noTone(buzzer);
              delay(500);
              lcd.setCursor(0,1);
              lcd.print("2");
              tone(buzzer, 1500);
              delay(500);
              noTone(buzzer);
              delay(500);
              lcd.setCursor(0,1);
              lcd.print("1");
              tone(buzzer, 1500);
              delay(500);
              noTone(buzzer);
              delay(500);
              int i = 0;
              while (i < 9) {
                tone(buzzer, 3500);
                delay(100);
                noTone(buzzer);
                delay(10);
                i = i + 1;
              }
              noTone(buzzer);
              delay(10);
              myservo.write(0);
              delay(2000);
              for (pos = 0; pos <= 180; pos +=2) {
              myservo.write(pos);
              lcd.setCursor(0,0);
              lcd.print("Taste good? ;)");
              delay(10);
              }
            }
       else {
              delay(500);
              lcd.setCursor(0,1);
              lcd.print("3");
              tone(buzzer, 1500);
              delay(500);
              noTone(buzzer);
              delay(500);
              lcd.setCursor(0,1);
              lcd.print("2");
              tone(buzzer, 1500);
              delay(500);
              noTone(buzzer);
              delay(500);
              lcd.setCursor(0,1);
              lcd.print("1");
              tone(buzzer, 1500);
              delay(500);
              noTone(buzzer);
              delay(500);
              int i = 1350;
              while (i>500) {
                tone(buzzer, i);
                delay(10);
                i = i - 10;
                noTone(buzzer);
              }
       }
    }
  }
      }
    lcd.clear();
    }


    // CLEAR BUTTON PRESS
    if (currentButtonStateClear != buttonStateClear)        
    { 
    buttonStateClear = currentButtonStateClear;  
                 
      if (buttonStateClear == LOW && selectPressCount == 1)    // The Clear button clears all setting data depending on what menu option you are viewing.                      
      {                                                     // It flahses a brief message stating that the data has been cleared.
      selectPressCount = 0;
      }  

      if (buttonStateClear == LOW && selectPressCount == 2)                         
      {                                                       
      selectPressCount = 1;
      }  

      if (buttonStateClear == LOW && selectPressCount == 3)
      {
      selectPressCount = 2;
      }

    lcd.clear();
    }
  // After a button is pushed and the count recorded, all the states reset back to LOW for the data to be processed correctly.
  lastButtonStateUp = currentButtonStateUp;           // resets the up button state to LOW
  lastButtonStateDown = currentButtonStateDown;       // resets the down button state to LOW
  lastButtonStateEnter = currentButtonStateEnter;     // resets the enter button state to LOW
  lastButtonStateClear = currentButtonStateClear;     // resets the clear button state to LOW
}
