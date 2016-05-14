/**
 * Arduino Code for MoodOrb
 * Mario Keller 
 *
 *
 **/


#include <FastSPI_LED.h>

#define NUM_LEDS 48

// Sometimes chipsets wire in a backwards sort of way
struct CRGB {
  unsigned char g;
  unsigned char r;
  unsigned char b;
};
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

typedef struct
{
  double R;
  double G;
  double B;
}
RgbFColor;

#define PIN 4

int a0,a1,a2 =0;
int oldbrightness = -1;
int oldvalue = -1;

int groupSize = 5;
int inverseGroupSize = 51;

int value = 0;
int brightness = 0;
int mode = 0;
void setup()
{

  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);
  FastSPI_LED.setPin(PIN);
  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData();

  analogReference(DEFAULT);
  //calibrate readAnalog
  for(int i = 0;i<50;i++) {
    a0 = readAnalog(a0,A0);
    a1 = readAnalog(a1,A1);
    a2 = readAnalog(a2,A2);
  }

  mode = getMode();
  value = getVal();
  brightness = getBrightness();

  randomSeed(A3);
}


void loop(){

  mode = getMode();
  value = getVal();
  brightness = getBrightness();
  switch(mode) {
  case 0:
    color(0);
    break;
  case 1:
    rainbow(1);
    break; 
  case 2:
    color_ball(2);
    break;
  case 3:
    rainbow3(3);
    break;
  case 4:
    rainbow4(4);
    break; 
  case 5:
    rainbow5(5);
    break;
  case 6:   
    bubbles(6); 
    break;
  case 7:
    white(7);     
    break;    
  }
}

int getMode() {
  a0 = readAnalog(a0,A0);
  int mode =  map(a0,0,1023,0,8);
  if (mode == 8) mode = 7;

  return mode;
}

int getVal() {
  a1 = readAnalog(a1,A1);
  int value =  map(a1,0,1023,0,258);
  if(value > 255) value = 255;
  if(abs(oldvalue - value) > 1 ) {
    oldvalue = value;     
  }

  return value;  
}

int getBrightness() {
  a2 = readAnalog(a2,A2);
  int brightness =  map(a2,0,1023,0,258);
  if(brightness > 255) brightness = 255;
  if(abs(oldbrightness - brightness) > 1 ) {
    oldbrightness = brightness;     
  }

  return brightness;  
}


int readAnalog(int prevvalue, int pin) {

  prevvalue = ( prevvalue *7) +analogRead(pin);
  prevvalue = prevvalue / 8;

  return prevvalue;
}


void white(int mode) {
  int oldbrightness = -1;
  while(mode == getMode()) {
    brightness = getBrightness();
    if(brightness != oldbrightness) {
      for(int j = 0; j < NUM_LEDS; j++ ){
        setRGB(brightness,brightness,brightness,j);
        oldbrightness = brightness;
      }
      FastSPI_LED.show();
    }
  }
}

void color(int mode) {
  while(mode == getMode()) {
    value = getVal();
    brightness = getBrightness();    
    for(int j = 0; j < NUM_LEDS; j++ ){
      setHue(value,brightness,j); 
    }
    FastSPI_LED.show();
  }
}

void rainbow(int mode)
{ 
  while(mode == getMode()) {
    for(int i = 0; i < 256; i++)
    {
      for(int j=0;j< NUM_LEDS; j++) {
        brightness = getBrightness();    
        setHue(i,brightness,j);
      }
      FastSPI_LED.show();
      delay(getVal());
      if( mode != getMode()) break;
    }
  }
}



void color_ball(int mode) {
  byte tail[4] = {
    40,24,8,4          }; 
  int len=6;
  //byte tail[4] = {40,28,16,4};
  int hue[4];
  hue[0] = getBrightness();  
  hue[1] = (getBrightness() +  64) % 255;  
  hue[2] = (getBrightness() + 128) % 255;  
  //hue[3] = (getBrightness() + 192) % 255; 
  int v = 0;
  while(mode == getMode()) { 

    for(int i=0;i<3;i++) {

      setHue(hue[i],(255-v),tail[i]);
      setHue(hue[i],v,(tail[i]+len)%48);
    }
    v+=5;
    if(v>255) {
      v=0;
      for(int i=0;i<3;i++)
        tail[i] = (tail[i] + 1) % 48;
    }  
    FastSPI_LED.show();
    hue[0] = getBrightness();  
    hue[1] = (getBrightness() +  64) % 255;  
    hue[2] = (getBrightness() + 128) % 255;  
    //hue[3] = (getBrightness() + 192) % 255; 
    delay(getVal()/32);   
  }


} 

void rainbow2(int mode){
  while(1) {
    for(int i = 255; i >= 0; i--)
    {
      for (int col = 0; col < 6; col++) {
        for(int row = 0; row < 8; row++) {
          int v = ((10*row) + i) % 255;
          setHue2(v,brightness,(8*col + row));        
        } 
      }

      FastSPI_LED.show();
      delay(getVal()/3);
      if( mode != getMode()) break;
    }

    if( mode != getMode()) break;
  }

}

void rainbow3(int mode){
  while(1) {
    for(int i = 255; i >= 0; i--)
    {
      int v = i;
      for (int row = 0; row < 5; row++) {
        for(int col = 0; col < 4; col++) {
          v = (v + 12) % 255;
          setHue2(v,brightness,(5*col + row));        
        } 
      }

      FastSPI_LED.show();
      delay(getVal()/3);
      if( mode != getMode()) break;
    }

    if( mode != getMode()) break;
  }

}

void rainbow4(int mode){
  while(1) {
    for(int i = 255; i >= 0; i -=3)
    {
      for (int row = 0; row < 8; row++) {
        for(int col = 0; col < 6; col++) {
          int v = ((20*col) + i) % 255;
          setHue2(v,brightness,(8*col + row));        
        } 
      }

      FastSPI_LED.show();
      delay(getVal());
      if( mode != getMode()) break;
    }

    if( mode != getMode()) break;
  }

}


void rainbow5(int mode){
  while(1) {
    for(int i = 0; i < 256; i++)
    {
      for (int row = 0; row < 5; row++) {
        for(int col = 0; col < 4; col++) {
          int v = ((10*col) + i) % 255;
          setHue2(v,brightness,(5*col + row));        
        } 
      }

      FastSPI_LED.show();
      delay(getVal()/3);
      if( mode != getMode()) break;
    }

    if( mode != getMode()) break;
  }

}


void bubbles(int mode) {
#define NUM_BUBBLES 20
  byte bubblecol[NUM_BUBBLES];
  byte bubblerow[NUM_BUBBLES];
  byte bubblespeed[NUM_BUBBLES];
  byte bubblehue[NUM_BUBBLES];

  byte numbubbles = 0;
  for(int i=0;i<NUM_BUBBLES;i++) {
    bubblecol[i] = 0;
    bubblerow[i] = -1;
  }
  int stepcounter =0;

  while(1) {
    if(numbubbles < NUM_BUBBLES) {
      for(int i=0;i<NUM_BUBBLES;i++) {
        if(bubblecol[i]==0) {
          numbubbles++;
          bubblecol[i]=random(1,7);
          bubblerow[i] = -1;
          bubblespeed[i]=random(1,11);
          bubblehue[i]=random(0,256);
          break;
        }
      }
    }
    stepcounter++;
    for(int i=0;i<NUM_BUBBLES;i++) {
      if(bubblecol[i]!=0) {
        if(stepcounter % bubblespeed[i] == 0) {

          bubblerow[i]++;
          if(bubblerow[i] > 7) {
            setRGB(0,0,0,(8*(bubblecol[i]-1) + 7));
            bubblecol[i] = 0;
            bubblerow[i] = 0;
            numbubbles--;
            setRGB(0,0,0,(8*(bubblecol[i]-1) + 7));
          } 
          else {
            if(bubblerow[i] > 0) setRGB(0,0,0,(8*(bubblecol[i]-1) + bubblerow[i]-1));
            setHue2(bubblehue[i],getBrightness(),(8*(bubblecol[i]-1) + bubblerow[i]));
            FastSPI_LED.show();

          }

        }
      }
    }
    delay(getVal());
    if( mode != getMode()) break;
  }  
}

void setHue2(int h, int brightness, int pin) {
  //this is the algorithm to convert from RGB to HSV
  double r=0; 
  double g=0; 
  double b=0;

  double hf=h/42.5;

  int i=(int)floor(h/42.5);
  double f = h/42.5 - i;
  double qv = 1 - f;
  double tv = f;

  switch (i)
  {
  case 0: //rojo dominante
    r = 1;
    g = tv;
    break;
  case 1: //verde
    r = qv;
    g = 1;
    break;
  case 2: 
    g = 1;
    b = tv;
    break;
  case 3: //azul
    g = qv;
    b = 1;
    break;
  case 4:
    r = tv;
    b = 1;
    break;
  case 5: //rojo
    r = 1;
    b = qv;
    break;
  }

  //set each component to a integer value between 0 and 255
  leds[pin].r = constrain((int)brightness*r,0,255);
  leds[pin].g = constrain((int)brightness*g,0,255);
  leds[pin].b = constrain((int)brightness*b,0,255);

}

void setRGB(int r, int g, int b, int pin) {
  //set each component to a integer value between 0 and 255
  leds[pin].r = constrain((int)r,0,255);
  leds[pin].g = constrain((int)g,0,255);
  leds[pin].b = constrain((int)b,0,255);
}

//Converts an HSV color to RGB color
void setHue(int hue, int brightness, int pin)
{
  float r, g, b, h, v; //this function works with floats between 0 and 1
  float f, p, q, t;
  int i;

  h = (float)(hue / 256.0);
  v = (float)(brightness / 256.0);

  //if saturation > 0, more complex calculations are needed

  h *= 6.0; //to bring hue to a number between 0 and 6, better for the calculations
  i = (int)(floor(h)); //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
  f = h - i;//the fractional part of h

  q = (float)(v * (1.0 - f));
  t = (float)(v * f);

  switch(i)
  {
  case 0: 
    r=v; 
    g=t; 
    b=0; 
    break;
  case 1: 
    r=q; 
    g=v; 
    b=0; 
    break;
  case 2: 
    r=0; 
    g=v; 
    b=t; 
    break;
  case 3: 
    r=0; 
    g=q; 
    b=v; 
    break;
  case 4: 
    r=t; 
    g=0; 
    b=v; 
    break;
  case 5: 
    r=v; 
    g=0; 
    b=q; 
    break;
  default: 
    r = g = b = 0; 
    break;
  }

  //set each component to a integer value between 0 and 255
  leds[pin].r = constrain((int)255*r,0,255);
  leds[pin].g = constrain((int)255*g,0,255);
  leds[pin].b = constrain((int)255*b,0,255);
}

void fadeOn(  int pin, int r, int g, int b) {

#define steps 75
  float dr = (float)r / steps; 
  float dg = (float)r / steps; 
  float db = (float)r / steps; 
  for(int i = 1; i <= 100; i++) {
    //set each component to a integer value between 0 and 255
    leds[pin].r = constrain((int)(i*dr),0,255);
    leds[pin].g = constrain((int)(i*dg),0,255);
    leds[pin].b = constrain((int)(i*db),0,255);
    FastSPI_LED.show();
    delay(getVal());
  }
}

void fadeOff( int pin, int r, int g, int b) {
#define steps 75
  float dr = (float)r / steps; 
  float dg = (float)r / steps; 
  float db = (float)r / steps; 
  for(int i = 99; i >= 0; i--) {
    //set each component to a integer value between 0 and 255
    leds[pin].r = constrain((int)(i*dr),0,255);
    leds[pin].g = constrain((int)(i*dg),0,255);
    leds[pin].b = constrain((int)(i*db),0,255);
    FastSPI_LED.show();
    delay(getVal());
  }
}











