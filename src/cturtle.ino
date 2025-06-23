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

int outOfBounds(struct turtle t) {
  if (t.u < uOffset || t.u >= uOffset + width || t.v < vOffset || t.v >= vOffset + height) {
    return 1; // out of bounds
  }
  return 0; // within bounds
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
  while(j < 2000){
    u8g2.clearBuffer();
    t.u = rand() % width + uOffset-1;
    t.v = rand() % height + vOffset-1;
    t.ang = 0;
    if(t.u < uCenter) {
      if(t.v < vCenter) {
        t.ang = M_PI_4;
      } else {
        t.ang = 5 * M_PI_4;
      }
    } else if(t.u >= uCenter) {
      if(t.v < vCenter) {
        t.ang = 5 * M_PI_4;
      } else {
        t.ang = 7 * M_PI_4;
      }
    }
    t = step(t, 10);
    u8g2.sendBuffer();
    delay(100);

    while (!outOfBounds(t)) {
      if (outOfBounds(t)) {
        break; // stop if out of bounds
      }
      float len = rand() % 5; // step length
      float twist = rand() % 30; 
      twist = pow(-1,rand()) *twist * M_PI / 180.0; // convert to radians
      t = step(t, len);
      t = turn(t, twist);
      delay(10); // delay to visualize the steps
      u8g2.sendBuffer();
    }
    delay(1000);
    j++;
  }
}
