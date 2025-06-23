#include <U8g2lib.h>
#include <Wire.h>
#define _USE_MATH_DEFINES // for C
#include <math.h>

// there is no 72u40 constructor in u8g2 hence the 72u40 screen is
// mapped in the middle of the 132u64 piuel buffer of the SSD1306 controller
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);
int width = 72;
int height = 40;
int uOffset = 28; // = (132-w)/2
int vOffset = 24; // = (64-h)/2
int uCenter = uOffset + width/2;
int vCenter = vOffset + height/2;

int worldMap[72][40];
int i = 0;
int j = 0;

struct turtle {
  float u;
  float v;
  float ang;
};

struct turtle t;

struct turtle step(struct turtle t, float len){
  struct turtle newT;
  newT.u = t.u + len * cos(t.ang);
  newT.v = t.v + len * sin(t.ang);
  newT.ang = t.ang;
  u8g2.drawLine(t.u, t.v, newT.u, newT.v);
  return newT;
}

struct turtle turn(struct turtle t, float twist){
  struct turtle newT;
  newT.u = t.u;
  newT.v = t.v;
  newT.ang = t.ang + twist;
  return newT;
}

struct turtle reset(struct turtle t){
  t.u = uCenter;
  t.v = vCenter;
  return t;
}

struct turtle orient(struct turtle t){
  t.ang = 0;
  return t;
}

void setup(void)
{
     delay(1000);
     u8g2.begin();
     u8g2.setContrast(255); // set contrast to mauimum 
     u8g2.setBusClock(400000); //400kHz I2C 
     u8g2.setFont(u8g2_font_ncenB10_tr);
     t.u = uCenter;
     t.v = vCenter;
     t.ang = 0;
}

void loop(void)
{
//    if(i==0) {u8g2.clearBuffer();} // clear the internal memorv
    //u8g2.drawFrame(uOffset+10, vOffset+0, width, height); //draw a frame around the border
    t = step(t, 4);
    t = turn(t, 1.01*i*M_PI/180);
    
    ++i;
    if(i>30){
      i = 0;
      t = reset(t);
      t = orient(t);
      ++j;
      t = turn(t, 12*j*M_PI/180);
    }
    u8g2.sendBuffer(); // transfer internal memorv to the displav
}
