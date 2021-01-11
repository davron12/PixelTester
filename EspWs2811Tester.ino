#include <FastLED.h>
#define DATA_PIN 7
#define SPEED_PIN A0
#define MODE_PIN A1
#define LENGTH_PIN A2
#define LED_PIN 13

#define NUM_LEDS 128

CRGB leds[NUM_LEDS];
CRGB sequence[3];
int currentStep = 0;

int stepDelay = 1000;
int ledValue = LOW;
int mode = 5;
int hue = 0;
    
void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
  pinMode(SPEED_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);
  initMode();

  // DEBUG
  //Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  readInputs();
  //Serial.println(stepDelay);
  updateSequence();
  FastLED.show(); 
  
//  sleep();
//
//  
//
//  FastLED.show(); 
  if( ledValue == LOW )
    ledValue = HIGH;
  else
    ledValue = LOW;
    
  digitalWrite(LED_PIN, ledValue);
  
  delay(stepDelay);

  
//  leds[0] = CRGB::Red; 
//  delay(100);
//
//  leds[0] = CRGB::Green; 
//  FastLED.show(); 
//  delay(100);
//
//  leds[0] = CRGB::Blue; 
//  FastLED.show(); 
//  delay(100);
}

void readInputs()
{
  stepDelay = analogRead(SPEED_PIN) / 1024.0 * 1000;
  int newMode = analogRead(MODE_PIN) / 1024.0 * 5;

  if( newMode != mode )
  {
    mode = newMode;
    initMode();
  }
}

void initMode(  )
{
  switch( mode)
  {
    case 0:
      initModeChase();
      break;
    case 1:
      initModeRGBChase();
      break;
    case 2:
      initModeRGB();
      break;
    case 3:
      initModeRainbow();
      break;
    case 4:
      initModeRainbowChase();
      break;
  }
}

void updateSequence(  )
{
  switch( mode )
  {
    case 0:
      stepModeChase();
      break;
    case 1:
      stepModeRGBChase();
      break;
    case 2:
      stepModeRGB();
      break;
    case 3:
      stepModeRainbow();
      break;
    case 4:
      stepModeRainbowChase();
      break;
  }
}
void initModeChase()
{
  for( int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB::Black; 
  }
  leds[0] = CRGB::White;
}

void stepModeChase()
{
  CRGB last = leds[NUM_LEDS-1];

  for( int i = NUM_LEDS - 1; i > 0; i-- )
  {
    leds[i] = leds[i-1]; 
  }
  leds[0] = last;
}

void initModeRGBChase()
{
  sequence[0] = CRGB::Red;
  sequence[1] = CRGB::Green;
  sequence[2] = CRGB::Blue;
}

void stepModeRGBChase()
{
  for( int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = sequence[i%3]; 
  }  

  CRGB first = sequence[0];
  sequence[0] = sequence[1];
  sequence[1] = sequence[2];
  sequence[2] = first;
}

void initModeRainbow()
{
  hue = 0;
  
  for( int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CHSV( hue, 255, 255 );
  }  
}
void stepModeRainbow()
{
  hue = ( hue + 1 ) % 255;
  
  for( int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CHSV( hue, 255, 255 );
  }  
  
}
void initModeRainbowChase()
{
  hue = 0;
  
  for( int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CHSV( (int) ( hue + ( (float) i / NUM_LEDS * 255 ) ) % 255, 255, 255);
  }  
}
void stepModeRainbowChase()
{
  hue = ( hue + 1 ) % 255;
  
  for( int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CHSV( (int) ( hue + (float) i / NUM_LEDS * 255 ) % 255, 255, 255);
  }  
}

void initModeRGB()
{
  sequence[0] = CRGB::Red;
  sequence[1] = CRGB::Green;
  sequence[2] = CRGB::Blue;
}

void stepModeRGB()
{ 
  CRGB nextColor;
  
  if( leds[0] == sequence[0] )
    nextColor = sequence[1];
  else if( leds[0] == sequence[1] )
    nextColor = sequence[2];
  else
    nextColor = sequence[0];
    
  for( int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = nextColor;
  }  
}

