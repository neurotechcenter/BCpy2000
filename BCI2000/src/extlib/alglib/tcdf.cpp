#include "tcdf.h"

float tcdf(float t, unsigned int df)
{
  float x=(t+sqrt(t*t+df))/(2*sqrt(t*t+df));
  return betai(df/2.0f,df/2.0f,x);
}
