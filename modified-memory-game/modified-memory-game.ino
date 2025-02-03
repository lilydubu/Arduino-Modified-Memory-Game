#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Set up buzzer
const int buzzer = A1;
const int buzzer_freq[] = {622, 659, 698, 739};
const int buzz_time = 250;

// Set up pins
const int leds[] = {3, 5, 7, 9};
const int buttons[] = {2, 4, 6, 8};
const int game_button = 13;
const int BGR[] = {10, 11, 12};

// Number of leds and buttons
const int n = 4; 

// For tracking moves in the serial monitor
int player_move = 0;
int current_move = 0;
 
// Max rounds
const int max_rounds = 10;

// Where we store the sequence (Maximum of 12 for 10 rounds)
int sequence[12];

// Time limit per move
int time_limit = 3000; 

// Time delay between light blinks
int time_delay = 750;

// Tracks the last time the LCD was updated
unsigned long last_update = 0;

// Toggles LCD press to start message  
bool show_pressme = true;  


void setup(){
  Serial.begin(9600);

  // Get random seed
  randomSeed(analogRead(A0));
  
  // LED and Buttons
  for(int i = 0; i < n; i++){
  	pinMode(leds[i], OUTPUT);
    pinMode(buttons[i], INPUT_PULLUP);
  }

  // BGR LEDs
  for(int i = 0; i < 3; i++){
  	pinMode(BGR[i], OUTPUT);
  }
  
  pinMode(buzzer, OUTPUT);
  pinMode(game_button, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.backlight();
}

void loop(){
  press_indicator();
  int debugger = check_button(); // I use this to test the physical buttons and leds (i also use it as a fidget toy)
  //Serial.println(debugger); // Uncomment this if you want to validate button inputs

  if(digitalRead(game_button) == LOW){
    Serial.print("\n\nNEW GAME");
  	lcd.clear();

    game_press();

    if(modified_memory() == true){
      winner();
    }
    else{
      wrong_move();
      delay(time_delay);
      validate();
      delay(time_delay);
    }
    Serial.println("\nEND OF GAME");
  }
}

bool modified_memory(){
  int round = 1;
  int seq_len = 3; // Minimum moves
  
  // Run until we reach final round
  while(round <= max_rounds){
  	// Show current round
    round_indicator(round);
    
    // Create new random Sequence
    generate_sequence(seq_len);
    
    // Play the sequence
  	light_sequence(seq_len, time_delay);
    
    // Indicate start
    go_indicator();
    
    // Player's turn
    Serial.print("Player: ");
    for(int i = 0; i < seq_len; i++){
      current_move = sequence[i];
      player_move = get_move();

      Serial.print(player_move);
      if(player_move == -1 || player_move != current_move) return false;
      else if(player_move == game_button){
          return 0;
        }
      }
      
      round++;
      seq_len++;
      delay(time_delay);
      correct_move();
      delay(1000);
    }
    return true;
}

void generate_sequence(int seq_len){
	Serial.print("\nSequence: ");
  for (int i = 0; i < seq_len; i++){
    	sequence[i] = random(0, n);
      Serial.print(sequence[i]);
  	}
  Serial.println();
}

void light_sequence(int seq_len, int time_delay){
  for(int i = 0; i < seq_len; i++){
    int led = sequence[i];
    digitalWrite(leds[led], HIGH);
    tone(buzzer, buzzer_freq[led], buzz_time);
    delay(time_delay);
    digitalWrite(leds[led], LOW);
    delay(time_delay);
  }
}

int get_move(){
	long start_time = millis();
  int move;
  
  	// If time limit per move is not reached yet
  	while((millis() - start_time) < time_limit){
    	move = check_button();
      
      // Button has been pressed
      if(move != -1){
      	// Buzz sound 
        tone(buzzer, buzzer_freq[move], buzz_time);
        
        // Wait for button to release
        while(check_button() != -1);
        delay(0);
        
        return move;
      }	
    }
  	// Timer ran out
  	return -1; 
}

int check_button() {
    for (int i = 0; i < n; i++) {
        // Check if button is pressed
        if (digitalRead(buttons[i]) == LOW) { 
            delay(10); // Short debounce delay - w/o interfering with lights

            // Confirm stable press
            if (digitalRead(buttons[i]) == LOW) {
                digitalWrite(leds[i], HIGH);
                delay(buzz_time); // Delay before turning off light
                tone(buzzer, buzzer_freq[i], buzz_time);
                digitalWrite(leds[i], LOW);   // Turn LED off

                return i;
            }
        }
    }
    return -1; // No button pressed
}

void game_press(){
  // White light
  digitalWrite(BGR[0], HIGH);
  digitalWrite(BGR[1], HIGH);
  digitalWrite(BGR[2], HIGH);

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("READY!");
  tone(buzzer, 880, buzz_time);
  delay(1000);

  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("SET!");
  tone(buzzer, 880, buzz_time);
  delay(1000);

  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("G0!");
  tone(buzzer, 1760, buzz_time);
  delay(1000);

  digitalWrite(BGR[0], LOW);
  digitalWrite(BGR[1], LOW);
  digitalWrite(BGR[2], LOW);
  delay(1000);
}

void press_indicator(){
  // Checks time to flash on/off
  if (millis() - last_update >= 400) {
    // Update the timer
    last_update = millis(); 

    if (show_pressme) {
      lcd.setCursor(1, 0);
      lcd.print("PRESS TO START");
    } 
    else {
      lcd.clear();
    }
  }
    // Toggle message on and off
    show_pressme = !show_pressme; 
}

void round_indicator(int round){
  char str[20];
  sprintf(str, "Round: %d", round);
  
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(str);
}

void go_indicator(){
  tone(buzzer, 131, 100);

  lcd.setCursor(1, 1);
  lcd.print("PLAYER'S TURN");

  // White light
  digitalWrite(BGR[0], HIGH);
  digitalWrite(BGR[1], HIGH);
  digitalWrite(BGR[2], HIGH);
  delay(1000);
  digitalWrite(BGR[0], LOW);
  digitalWrite(BGR[1], LOW);
  digitalWrite(BGR[2], LOW);
}

void correct_move(){
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Correct!");
  
  tone(buzzer, 1760, 300);
  delay(300);
  tone(buzzer, 880, 300);
  delay(300);
  tone(buzzer, 440, 300);

  // Green light
  digitalWrite(BGR[1], HIGH);
  delay(1000);
  digitalWrite(BGR[1], LOW);
}

void winner(){
  delay(1000);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Well played.");

  delay(1000);

  lcd.setCursor(3, 2);
  lcd.print("YOU WIN!");
  
  // Party lights
  for(int i = 0; i < 3; i++){
    
    digitalWrite(BGR[0], HIGH);
    delay(200);
    digitalWrite(BGR[0], LOW);

    digitalWrite(BGR[1], HIGH);
    delay(200);
    digitalWrite(BGR[1], LOW);

    digitalWrite(BGR[2], HIGH);
    delay(200);
    digitalWrite(BGR[2], LOW);

    winner_sound();
    delay(300);
  }
  
  tone(buzzer, 932, 300);
  delay(300);
  tone(buzzer, 987, 600);

  delay(1000);
  lcd.clear();
}

void wrong_move(){
  // Red light
  digitalWrite(BGR[2], HIGH);
    
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Wrong move.");
  delay(time_delay);

  lose_sound();
  
  lcd.setCursor(3, 2);
  lcd.print("YOU LOSE!");
  delay(2000);
  lcd.clear();
  
  digitalWrite(BGR[2], LOW);
}

void validate(){
  char str1[50];
  char str2[50];

  lcd.clear();

  lcd.setCursor(0,0);
  sprintf(str1, "Player move: %d", player_move);
  lcd.print(str1);
  
  lcd.setCursor(0,1);
  sprintf(str2, "Expected: %d", current_move);
  lcd.print(str2);
  
  delay(2000);
  lcd.clear();
}

void lose_sound(){
  tone(buzzer, buzzer_freq[3], 300);
  delay(200);
  tone(buzzer, buzzer_freq[2], 400);
  delay(200);
  tone(buzzer, buzzer_freq[1], 400);
  delay(200);
  tone(buzzer, buzzer_freq[0], 600);
}

void winner_sound(){
  tone(buzzer, 830, 500);
  delay(200);
  tone(buzzer, 1661, 300);
  delay(200);
  tone(buzzer, 3322, 400);
  delay(200);
  tone(buzzer, 1760, 500);
  delay(200);
  tone(buzzer, 1864, 500);
}
