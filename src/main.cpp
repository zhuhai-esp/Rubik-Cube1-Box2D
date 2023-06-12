#pragma GCC optimize("O3")

#include "MPU6886.h"
#include "TFTDebugDraw.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <box2d/box2d.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sp(&tft);
b2World *myWorld = NULL;
unsigned long lastMs = 0;
I2C_MPU6886 imu;

void createSomeBalls() {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.allowSleep = true;
  auto r = (random() % 130) / 100;
  bodyDef.position.Set(16 + r, 4);
  auto *body = myWorld->CreateBody(&bodyDef);

  b2CircleShape shape;
  shape.m_radius = 0.7 + r;

  b2FixtureDef f;
  f.shape = &shape;
  f.density = 0.5;
  f.friction = 0.4;
  f.restitution = 0.5;

  body->CreateFixture(&f);
  body->ApplyForce(b2Vec2(10, 6 + r), b2Vec2(0, 0), true);
}

void createSomeBoxes() {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.allowSleep = true;
  auto r = (random() % 150) / 100;
  bodyDef.position.Set(17 + r, 3 + r);
  auto *body = myWorld->CreateBody(&bodyDef);

  b2PolygonShape shape;
  shape.SetAsBox(0.8 + r, 0.8 + r);

  b2FixtureDef f;
  f.shape = &shape;
  f.density = 0.8;
  f.friction = 0.4;
  f.restitution = 0.6;

  body->CreateFixture(&f);
}

void createSomeWorld() {
  b2Vec2 gravity(0, 10);
  myWorld = new b2World(gravity);

  b2Draw *draw = new TFTDebugDraw();
  draw->SetFlags(1);
  myWorld->SetAllowSleeping(true);
  myWorld->SetDebugDraw(draw);

  b2BodyDef groundBodyDef;
  b2Body *groundBody = myWorld->CreateBody(&groundBodyDef);
  b2PolygonShape groundBox;

  groundBox.SetAsBox(16, .05, b2Vec2(16, 0), 0);
  groundBody->CreateFixture(&groundBox, 0.0f);

  groundBox.SetAsBox(16, .05, b2Vec2(16, 24), 0);
  groundBody->CreateFixture(&groundBox, 0.0f);

  groundBox.SetAsBox(.05, 12, b2Vec2(0, 12), 0);
  groundBody->CreateFixture(&groundBox, 0.0f);

  groundBox.SetAsBox(.05, 12, b2Vec2(32, 12), 0);
  groundBody->CreateFixture(&groundBox, 0.0f);

  for (int i = 0; i < 14; i++) {
    createSomeBoxes();
  }
  for (int i = 0; i < 24; i++) {
    createSomeBalls();
  }
}

void setup() {
  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);
  ledcSetup(0, 800, 10);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, 200);

  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);

  sp.createSprite(320, 240);
  createSomeWorld();
  delay(1000);
  Wire.begin(21, 22, 2000);
  imu.begin();
}

void loop() {
  myWorld->Step(0.1, 6, 2);
  sp.fillRect(0, 0, 320, 240, TFT_BLACK);
  myWorld->DebugDraw();
  sp.pushSprite(0, 0);
  if (millis() - lastMs >= 1000) {
    lastMs = millis();
    float gx, gy, gz;
    imu.getAccel(&gx, &gy, &gz);
    myWorld->SetGravity(b2Vec2(-gy * 10, -gx * 10));
  }
}
