#ifndef __APP_SONAR_H
#define __APP_SONAR_H

#include "sonar_SR04.h"



void App_Sonar_Init(void);

void SonarReadTask(void const * argument);

void App_Sonar_Test(void);

void App_Sonar_Proc(void);

int mapToRange(float value, float inputMin, float inputMax, int outputMin, int outputMax);

float movingAverage(float values[], int index, float newValue);

void RubbishWav_F(int type);

#endif
