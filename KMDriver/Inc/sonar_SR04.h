#ifndef __SONAR_SR04_H
#define __SONAR_SR04_H

#include "stm32f4xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t dir_channel;
    uint32_t indir_channel;
    GPIO_TypeDef * trig_port;
    uint16_t trig_pin;
    GPIO_TypeDef * echo_port;
    uint16_t echo_pin;
    float distance;
    //占比
    int percent;
} Sonar_HandleTypeDef;

void Sonar_Init(Sonar_HandleTypeDef *sonar);
void Sonar_clearData(Sonar_HandleTypeDef *sonar);
float Sonar_GetDistance(Sonar_HandleTypeDef *sonar);

#endif /* __SONAR_H */
