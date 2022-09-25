int leds[] = { 7, 8, 9, 10, 11, 12 };
//int tones[] = { 9660, 9510, 9770, 9380, 9510, 9430 };
int tones[] = { 55, 69, 82, 98, 110, 131 };
int buzzer = 13; // on if any led is on
int analog = A0;
int switch1 = 3, switch2 = 2;
bool status[] = { false, false, false, false, false, false };
int numLeds = 6;

/*
 * Lights the led with the specified index,
 * then sleeps `milis` miliseconds
 */
void lightLed(int led, int milis)
{
  if (led == 0)
    PORTD |= 0b10000000; // pin 7 on
  else
  {
    int activatePin = 0b0000001;
    activatePin = activatePin << (led - 1); // move that 1 bit left "led - 1" times 
    PORTB |= activatePin;
  }
  tone(buzzer, tones[led]);
  delay(milis);
  if (led == 0)
    PORTD &= 0b01111111; // pin 7 off
  else
  {
    int activatePin = 0b1000000;
    activatePin = activatePin >> (led - 1); // move that 1 bit left "led - 1" times 
    PORTB &= activatePin;
  }
  noTone(buzzer);
}

/*
 * Lights all leds in array with the given indices,
 * then sleeps `milis` miliseconds.
 * Optionally, after each led `i`, sleep `pauses[i]` milis
 * (This argument is optional)
 */
void lightLedGroup(int ledGroup[], int pauses[], int num, int milis)
{
  for (int i = 0; i < num; i++)
  {
    int led = ledGroup[i];
    if (led == 0)
    	PORTD |= 0b10000000; // pin 7 on
    else
    {
      int activatePin = 0b0000001;
      activatePin = activatePin << (led - 1); // move that 1 bit left "led - 1" times 
      PORTB |= activatePin;
    }
    tone(buzzer, tones[ledGroup[i]]);
    delay(pauses[i]);
  }
  delay(milis);

  noTone(buzzer);
  for (int i = 0; i < num; i++)
  {
    int led = ledGroup[i];
	if (led == 0)
    	PORTD &= 0b01111111; // pin 7 off
    else
    {
      int activatePin = 0b1000000;
      activatePin = activatePin >> (led - 1); // move that 1 bit left "led - 1" times 
      PORTB &= activatePin;
    }
    delay(pauses[i]);
  }
}

/*
 * Override function of the above without the optional parameter
 */
void lightLedGroup(int ledGroup[], int num, int milis)
{
  int pauses[] = { 0, 0, 0, 0, 0, 0 };
  lightLedGroup(ledGroup, pauses, num, milis);
}

void toggleLed(int led)
{
  status[led] = !status[led];
  if(status[led])
    tone(buzzer, tones[led]);
  else
    noTone(buzzer);
  if (status[led]) // on
  {
    if (led == 0)
      PORTD |= 0b10000000; // pin 7 on
    else
    {
      int activatePin = 0b0000001;
      activatePin = activatePin << (led - 1); // move that 1 bit left "led - 1" times 
      PORTB |= activatePin;
    }
  }
  else // off
  {
    if (led == 0)
      PORTD &= 0b01111111; // pin 7 off
    else
    {
      int activatePin = 0b1000000;
      activatePin = activatePin >> (led - 1); // move that 1 bit left "led - 1" times 
      PORTB &= activatePin;
    }
  }
}

/*
 * If potentiometer was a circle, which quarter 
 * of the circle is its output?
 */
int getQuadrant(int input, int max)
{
  if (input < max * 0.25)
    return 0;
  if (input < max * 0.50)
    return 1;
  if (input < max * 0.75)
    return 2;

  return 3;
}

// LOOPS:

float analogIn = 0;

/*
 * simulates an emergency vehicle's lights
 */
void EmergencyVehicle(int milisPause, int milis)
{
  int pauses[] = { milisPause, milisPause, milisPause };
  int leftLeds[] = { 0, 1, 2 };
  int rightLeds[] = { 5, 4, 3 };

  lightLedGroup(leftLeds, pauses, 3, milis);
  lightLedGroup(rightLeds, pauses, 3, milis);
}

/*
 * simulates an inline-six engine (BMW, Audi...) 
 * with an input for RPM (analog)
 *
 * FIRING ORDER: 0 4 2 5 1 3
 */

void StraightSixEngineSim(int milisFire, float analogIn)
{
  int fireOrder[] = { 0, 4, 2, 5, 1, 3 }; 

  for (int i = 0; i < 6; i++)
  {
    int milisBreak = (analogIn / 1023.0) * 1000;
    lightLed(fireOrder[i], milisFire + milisBreak / 2.5);
    delay(milisBreak);
  }
}

/*
 * simulates the revlimiter leds of F1 car
 * with an input for RPM (analog)
 *
 */
void Revlimiter(int milisBlink, float analogIn)
{
  int revsPercent = (analogIn / 1026.0) * 100;
  Serial.println(revsPercent);
  
  if (revsPercent < 2.4 )
  {
    int allLeds[] = { 0, 1, 2, 3, 4, 5 };
    lightLedGroup(allLeds, 6, milisBlink);
    lightLedGroup(allLeds, 6, milisBlink);
    delay(150);
    
    return;
  }
  int numBlink = (revsPercent / 15.0 - 1.5);

  for (int i = numLeds - 1; i > numBlink; i--)
  {
    if (i == 0)
      PORTD |= 0b10000000; // pin 7 on
    else
    {
      int activatePin = 0b0000001;
      activatePin = activatePin << (i - 1); // move that 1 bit left "led - 1" times 
      PORTB |= activatePin;
    }
  }
  
  for (int i = numBlink; i >= 0; i--)
  {
    if (i == 0)
    	PORTD &= 0b01111111; // pin 7 off
    else
    {
      int activatePin = 0b1000000;
      activatePin = activatePin >> (i - 1); // move that 1 bit left "led - 1" times 
      PORTB &= activatePin;
    }
  }
}

void Random(float analogIn)
{
    randomSeed(analogRead(4)); //read unconnected pin for noise
    int randEffect = random(3);
    bool multipleEffects = random(1);
    switch(randEffect)
    {
      case 0:
        lightLed(random(6), analogIn * 0.2);
        if (!multipleEffects)
        {
          multipleEffects = 0;
          break;
        }
      case 1:
        toggleLed(random(6));
        if (!multipleEffects)
        {
            multipleEffects = 0;
            break;
        }
      case 2:
        int numLedsRand = random(6);
        int *ledsRand = (int*) malloc(sizeof(int) * numLedsRand);
        
        for (int i = 0; i < numLedsRand; i++)
        {
          ledsRand[i] = random(6);
        }
        lightLedGroup(ledsRand, numLedsRand, analogIn * 0.5);
      break;
    }
    delay(analogIn * 0.25);
}

void setup()
{
  Serial.begin(9600);

  // switches: 2, 3
  // leds: 7-12
  // buzzer: 13
  DDRD |= 0b10000000; // 7 as output
  DDRB |= 0b11111111; // 13 - 8 as output
  PORTD |= 0b00001100; // pullup 2, 3
  DDRD &= 0b11110011; // input 2, 3
}

void loop()
{
  int show = digitalRead(switch1) * 2 + digitalRead(switch2);
  analogIn = analogRead(analog) * 1.51;
  Serial.println(analogIn);
  Serial.println(show);

  switch (show)
  {
    case 0:
      Serial.println("EmergencyVeh");
      EmergencyVehicle(analogIn / 10, analogIn / 10);
      delay(analogIn / 10);
      EmergencyVehicle(analogIn / 20, analogIn / 15);
      delay(analogIn / 10);
      break;
    case 1:
      Serial.println("i6");
      // use A1 potentiometer to control speed
      StraightSixEngineSim(100, analogIn);
      break;
    case 2:
      Serial.println("revlimit");
      // use A1 potentiometer to control RPM
      Revlimiter(50, analogIn);
      break;
    case 3:
      Serial.println("Random");
      Random(analogIn);
      break;          
  }   
}
