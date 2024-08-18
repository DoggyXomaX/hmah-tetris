#ifndef __CURVE_H__
#define __CURVE_H__

#include <stddef.h>

#define MAX_CURVE_POINTS 32

enum AnimationCurvePointType {
  ACPT_FIXED,
  ACPT_LINEAR,
};

typedef struct AnimationCurvePoint_s {
  enum AnimationCurvePointType Type;
  float Time;
  float Value;
} AnimationCurvePoint;

typedef struct AnimationCurve_s {
  AnimationCurvePoint Points[MAX_CURVE_POINTS];
  size_t PointsCount;
} AnimationCurve;

AnimationCurve AnimationCurve_Create(void);
AnimationCurve AnimationCurve_CreateLinearFade(float delay, float unfade, float stay, float fade, float endDelay);

void AnimationCurve_AddPoint(AnimationCurve* this, enum AnimationCurvePointType type, float time, float value);

void AnimationCurve_SetType(AnimationCurve* this, size_t index, enum AnimationCurvePointType type);
void AnimationCurve_SetTime(AnimationCurve* this, size_t index, float time);
void AnimationCurve_SetValue(AnimationCurve* this, size_t index, float value);

void AnimationCurve_RemoveAllPoints(AnimationCurve* this);

float AnimationCurve_GetValue(AnimationCurve* this, float time);
float AnimationCurve_GetMaxTime(AnimationCurve* this);

void AnimationCurve_SortByTime(AnimationCurve* this);

#endif // __CURVE_H__