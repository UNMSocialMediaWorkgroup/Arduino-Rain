#include <Adafruit_NeoPixel.h>

#define PIN1 3
#define PIN2 6
#define PIN3 9
#define PIN4 11
#define PIN5 10

#define SLENGTH 187

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(SLENGTH, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(SLENGTH, PIN2, NEO_GRB + NEO_KHZ800);

float length = 187.0f;


byte dropLength1 = 20;
byte dropLength2 = 30;

float dropSpeed1 = 1;
float dropSpeed2 = 1;

float dropColor1[3] = {198.0,230.0,255.0}; 
float dropColor2[3] = {198.0,230.0,255.0}; 

float dropredchange1 = (dropColor1[0]-0.0)/length;
float dropgreenchange1 = (dropColor1[1]-23.0)/length;//-0.44
float dropbluechange1 = (dropColor1[2]-87.0)/length;

float dropredchange2 = (dropColor2[0]-0.0)/length;
float dropgreenchange2 = (dropColor2[1]-23.0)/length;//-0.44
float dropbluechange2 = (dropColor2[2]-87.0)/length;


float dropPos1 = 0.0;//true position of drop
float dropPos2 = 0.0;//true position of drop
float dropBackPos1 = 0.0;//true position of drop
float dropBackPos2 = 0.0;//true position of drop

int dropFrontIndex1 = 0; 
int dropFrontIndex2 = 0;
int dropBackIndex1 = 0; 
int dropBackIndex2 = 0;

byte bgColor[3] = {0,23,87};
float redchange = (80.0-0.0)/length;
float greenchange = (28.0-23.0)/length;//-0.44
float bluechange = (70.0-87.0)/length;




//d#c###s#l#

byte sb_strandnum = 0;
byte sb_dropcolor[3] = {255,255,255};
byte sb_speed = 40;
byte sb_length = 20;



void setup() {
  
  Serial.begin(9600);
  
  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
}


void loop(){
    
  readSerial();
  checkDropInit();
  updateDrops();
  animateLoop();
    
  
  
}

void readSerial(){
  
  String sb_string ="";
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      sb_string += c; //makes the string readString
    } 
  }
  if (sb_string.length() >0) {
    //Serial.println(sb_string); //see what was received
        if(sb_string[0] == 'd')
        {
          sb_strandnum = (byte)sb_string[1];
          Serial.println(" d ");
        }
        if(sb_string[0] == 'r')
        {
          sb_dropcolor[0] = sb_string[1];
          Serial.println(" r ");
        }
        if(sb_string[0] == 'g')
        {
           sb_dropcolor[1] = sb_string[1];
           Serial.println(" g ");
        }
        if(sb_string[0] == 'b')
        {
          sb_dropcolor[2] = sb_string[1];
          Serial.println(" b ");
        }
        if(sb_string[0] == 's')
        {
          sb_speed = sb_string[1];
          Serial.println(" s ");
        }
        if(sb_string[0] == 'l')
        {
          sb_length = sb_string[1];
          Serial.println(" l ");
        }
        Serial.println(sb_string);

  }

}

void animateLoop()
{

  float r1 =0;
  float g1 =0;
  float b1 =0;
 
  float r2 =0;
  float g2 =0;
  float b2 =0;
 
  r1 =  bgColor[0]+(dropBackIndex1*redchange);
  g1 =  bgColor[1]+(dropBackIndex1*greenchange);
  b1 =  bgColor[2]+(dropBackIndex1*bluechange);
   
  r1 = clamp(r1, 0, 255);
  g1 = clamp(g1, 0, 255);
  b1 = clamp(b1, 0, 255);
   
  r2 =  bgColor[0]+(dropBackIndex2*redchange);
  g2 =  bgColor[1]+(dropBackIndex2*greenchange);
  b2 =  bgColor[2]+(dropBackIndex2*bluechange);
   
  r2 = clamp(r2, 0, 255);
  g2 = clamp(g2, 0, 255);
  b2 = clamp(b2, 0, 255);
  
  strip1.setPixelColor(dropFrontIndex1, strip1.Color(dropColor1[0]+(dropFrontIndex1*dropredchange1),dropColor1[1]+(dropFrontIndex1*dropgreenchange1),dropColor1[2]+(dropFrontIndex1*dropbluechange1)));
  strip2.setPixelColor(dropFrontIndex2, strip2.Color(dropColor2[0]+(dropFrontIndex2*dropredchange2),dropColor2[1]+(dropFrontIndex2*dropgreenchange2),dropColor2[2]+(dropFrontIndex2*dropbluechange2)));

  strip1.setPixelColor(dropBackIndex1, strip1.Color(r1,g1,b1));
  strip2.setPixelColor(dropBackIndex2, strip2.Color(r2,g2,b2));
  
 
  strip1.show(); 
  strip2.show(); 

}

void checkDropInit()
{
   if(dropPos1 < 0 && sb_strandnum == 1)
   {
      initDrop1();
   } 
   else if(dropPos2 < 0 && sb_strandnum == 2)
   {
      initDrop2();
   } 
}


void initDrop1()
{
    dropLength1 = sb_length;
    dropSpeed1 = (float)sb_speed/100.0f;
    dropColor1[0] = (float)sb_dropcolor[0]; 
    dropColor1[1] = (float)sb_dropcolor[1]; 
    dropColor1[2] = (float)sb_dropcolor[2];
    dropredchange1 = (dropColor1[0]-0.0)/length; 
    dropgreenchange1 = (dropColor1[1]-23.0)/length;
    dropbluechange1 = (dropColor1[2]-87.0)/length;
    dropPos1 = 0;
    
    sb_strandnum = 0;//clear drop buffer so a new one can be processed
}

void initDrop2()
{
    dropLength2 = sb_length;
    dropSpeed2 = (float)sb_speed/100.0f;
    dropColor2[0] = (float)sb_dropcolor[0]; 
    dropColor2[1] = (float)sb_dropcolor[1]; 
    dropColor2[2] = (float)sb_dropcolor[2];
    dropredchange2 = (dropColor2[0]-0.0)/length; 
    dropgreenchange2 = (dropColor2[1]-23.0)/length;
    dropbluechange2 = (dropColor2[2]-87.0)/length;
    dropPos2 = 0;

    sb_strandnum = 0;//clear drop buffer so a new one can be processed
}

void updateDrops()
{
   if(dropPos1 >= 0)
   {
     updateDrop1Position();
   }
   if(dropPos2 >= 0)
   {
     updateDrop2Position();
   }
}



void updateDrop1Position()
{
  dropPos1 = dropPos1+dropSpeed1;//true position of drop
  dropBackPos1 = dropPos1-dropLength1;
  
  if(dropBackPos1 >= length)
  {
    dropPos1 = -1;
  }
 
  dropFrontIndex1 = clamp((int)dropPos1, 0, length);
  dropBackIndex1  = clamp((int)dropBackPos1, 0, length);
}

void updateDrop2Position()
{
  dropPos2 = dropPos2+dropSpeed2;//true position of drop
  dropBackPos2 = dropPos2-dropLength2;
  
  if(dropBackPos2 >= length)
  {
    dropPos2 = -1;
  }
 
  dropFrontIndex2 = clamp((int)dropPos2, 0, length);
  dropBackIndex2  = clamp((int)dropBackPos2, 0, length);
}






byte addColortoChannel(float _color1, float _color2)
{
    float colorOut = _color1+_color2;  
    colorOut = clamp((int)colorOut, 255, 0);
    return (byte)colorOut;
}

int clamp(int i, int _min, int _max)
{
  if(i > _max)
  {
   return _max; 
  } 
  else if(i < _min)
  {
    return _min; 
  }
  else 
  {
    return i; 
  }
}



