// 2,000 and 5,000 SPEAKER FREQUENCIES
// tone( pin number, frequency in hertz, duration in milliseconds);
// tone( 8 , 2000 , 1000);

#include <stdio.h>
#include <stdlib.h>
#include <LiquidCrystal.h>
#include <avr/sleep.h>

#define ON HIGH
#define OFF LOW
#define DELAY 400

// Yellow , Green , Blue , Red
int buttons[] = {2 , 3 , 4 , 5};
int leds[]    = {6 , 7 , 8 , 9};
int btones[]   = {932 , 831 , 740 , 622};
int speaker = 10;
int seq[100];
int turn = 1;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

void play(int frequency , int duration)
{
  tone(speaker , frequency , duration);
  delay(duration);
}

void playSong(int frequency , int duration)
{
  for (int i = 0; i < 4; i++)
    digitalWrite(leds[i] , ON);

  play(frequency , duration);

  for (int i = 0; i < 4; i++)
    digitalWrite(leds[i] , OFF);

  delay(200);
}

void simonSays(int i)
{
  digitalWrite(leds[i] , ON);
  play(btones[i] , DELAY);
  digitalWrite(leds[i] , OFF);
  delay(DELAY / 2);
}

void gameover()
{
  lcd.clear();
  lcd.print("GAME OVER");
  lcd.setCursor(0 , 1);
  lcd.print("Score: ");
  lcd.print(turn - 1);
  delay(200);
  
  int sounds[] = {587 , 554 , 523 , 493};
  int durations[] = {500 , 400 , 500 , 1500};
  
  for (int i = 0; i < 4; i++)
    playSong(sounds[i] , durations[i]);

  exit(0);
}

void gamestart()
{
  int sounds[]    = {622 , 466 , 415 , 622 , 466};
  int durations[] = {350 , 300 , 500 , 250 , 800};

  for (int i = 0; i < 5; i++)
    playSong(sounds[i] , durations[i]);
}

void setup()
{
  turn = 1;
  
  for (int i = 0; i < 4; i++)
  {
    pinMode(buttons[i] , INPUT);
    pinMode(leds[i], OUTPUT);
  }
  
  pinMode(speaker , OUTPUT);

  lcd.begin(16 , 2);
  lcd.println("Hello! Press any");
  lcd.setCursor(0 , 1);  
  lcd.println("button to start!");
  
  int start = 0;
  
  while (true)
  {
    if (digitalRead(buttons[start]))
      break;

    start = (start + 1) % 4;
  }

  lcd.clear();

  gamestart();

  srand(micros());
  rand();

  for (int i = 0; i < 100; i++)
    seq[i] = rand() % 4;

  delay(1000);
}

void loop()
{
  lcd.clear();
  lcd.print("Simon Says!");
  lcd.setCursor(0 , 1);
  lcd.print("Score: ");
  lcd.print(turn - 1);
  
  for (int i = 0; i < turn; i++)
    simonSays(seq[i]);

  lcd.clear();
  lcd.print("Your turn!");
  lcd.setCursor(0 , 1);
  lcd.print("Score: ");
  lcd.print(turn - 1);

  for (int i = 0; i < turn; i++)
  {
    int match = seq[i];
    int user = -1;

    // Wait for user move
    while (user == -1)
    {
      for (int j = 0; j < 4; j++)
        if (digitalRead(buttons[j]))
        {
          delay(30);
          
          if (digitalRead(buttons[j]))
          {
            user = j;
            break;
          }
        }

      digitalWrite(leds[user] , ON);

      while (digitalRead(buttons[user]))
        play(btones[user] , 100);

      digitalWrite(leds[user] , OFF);
    }

    if (user != match)
      gameover();
  }

  turn++;
  delay(1000);
}
