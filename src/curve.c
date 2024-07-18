#include "curve.h"

#include <stdio.h>
#include <stdlib.h>

#define assertIndex(x, c) if (x >= c) { fprintf(stderr, "Index %lu is out of range!\n", x); return; }

AnimationCurve AnimationCurve_Create() {
  AnimationCurve curve = {
    .PointsCount = 0,
  };
  return curve;
}

// _/""\_
AnimationCurve AnimationCurve_CreateLinearFade(float delay, float unfade, float stay, float fade, float endDelay) {
  AnimationCurve curve = {
    .PointsCount = 6,
    .Points = {
      { .Type = ACPT_FIXED, .Time = 0.0f, .Value = 0.0f },
      { .Type = ACPT_LINEAR, .Time = delay, .Value = 0.0f },
      { .Type = ACPT_FIXED, .Time = delay + unfade, .Value = 1.0f },
      { .Type = ACPT_LINEAR, .Time = delay + unfade + stay, .Value = 1.0f },
      { .Type = ACPT_FIXED, .Time = delay + unfade + stay + fade, .Value = 0.0f },
      { .Type = ACPT_FIXED, .Time = delay + unfade + stay + fade + endDelay, .Value = 0.0f },
    },
  };
  return curve;
}

void AnimationCurve_AddPoint(AnimationCurve* this, enum AnimationCurvePointType type, float time, float value) {
  if (this->PointsCount >= MAX_CURVE_POINTS) {
    fprintf(stderr, "Curve max points limit %d reached!\n", MAX_CURVE_POINTS);
    return;
  }

  this->Points[this->PointsCount].Type = type;
  this->Points[this->PointsCount].Time = time;
  this->Points[this->PointsCount].Value = value;
  this->PointsCount++;

  AnimationCurve_SortByTime(this);
}

void AnimationCurve_SetType(AnimationCurve* this, size_t index, enum AnimationCurvePointType type) {
  assertIndex(index, this->PointsCount);

  this->Points[index].Type = type;
}

void AnimationCurve_SetTime(AnimationCurve* this, size_t index, float time) {
  assertIndex(index, this->PointsCount);

  this->Points[index].Time = time;

  AnimationCurve_SortByTime(this);
}

void AnimationCurve_SetValue(AnimationCurve* this, size_t index, float value) {
  assertIndex(index, this->PointsCount);

  this->Points[index].Value = value;
}

void AnimationCurve_RemoveAllPoints(AnimationCurve* this) {
  this->PointsCount = 0;
}

float AnimationCurve_GetValue(AnimationCurve* this, float time) {
  if (time < this->Points[0].Time) {
    return this->Points[0].Value;
  }

  if (time > this->Points[this->PointsCount - 1].Time) {
    return this->Points[this->PointsCount - 1].Value;
  }

  for (size_t i = 0, j = 1; i < this->PointsCount - 1; i++, j++) {
    AnimationCurvePoint* a = &this->Points[i];
    AnimationCurvePoint* b = &this->Points[j];

    if (a->Time == time) return a->Value;
    if (b->Time == time) return b->Value;

    if (a->Time < time && time < b->Time) {
      if (a->Type == ACPT_FIXED) return a->Value;
      if (a->Type == ACPT_LINEAR) {
        float t = (time - a->Time) / (b->Time - a->Time);
        return a->Value + (b->Value - a->Value) * t;
      }
    }
  }

  // Maybe unreached forever, but who knows
  return 0.0f;
}

float AnimationCurve_GetMaxTime(AnimationCurve* this) {
  return this->PointsCount != 0
    ? this->Points[this->PointsCount - 1].Time
    : 0.0f;
}

int compareTimes(const void* aPtr, const void* bPtr) {
  const AnimationCurvePoint* a = aPtr;
  const AnimationCurvePoint* b = bPtr;
  return a->Time - b->Time;
}

void AnimationCurve_SortByTime(AnimationCurve* this) {
  if (this->PointsCount == 0) return;

  qsort(this->Points, this->PointsCount, sizeof(AnimationCurvePoint), compareTimes);
}
