#ifndef __TFT_DEBUG_DRAW__
#define __TFT_DEBUG_DRAW__

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <box2d/box2d.h>

extern TFT_eSprite sp;

class TFTDebugDraw : public b2Draw {

  void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount,
                   const b2Color &color) {
    for (int i = 0; i < vertexCount; i++) {
      auto p1 = vertices[i];
      auto p2 = vertices[(i + 1) % vertexCount];
      sp.drawLine(p1.x * 10, p1.y * 10, p2.x * 10, p2.y * 10, TFT_GREEN);
    }
  }

  void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount,
                        const b2Color &color) {
    for (int i = 0; i < vertexCount; i++) {
      auto p1 = vertices[i];
      auto p2 = vertices[(i + 1) % vertexCount];
      sp.drawLine(p1.x * 10, p1.y * 10, p2.x * 10, p2.y * 10, TFT_YELLOW);
    }
  }

  void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) {
    sp.drawCircle(center.x * 10, center.y * 10, (int)(radius * 10), TFT_PINK);
  }

  void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis,
                       const b2Color &color) {
    sp.drawCircle(center.x * 10, center.y * 10, (int)(radius * 10), TFT_PINK);
  }

  void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
    sp.drawLine(p1.x * 10, p1.y * 10, p2.x * 10, p2.y * 10, TFT_GREEN);
  }

  void DrawTransform(const b2Transform &xf) {}

  void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) {
    sp.drawPixel(p.x * 10, p.y * 10, TFT_RED);
  }
};

#endif