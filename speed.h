/**
 * @file speed.h
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */
#include "vector.h"

#ifndef SPEED_H
#define SPEED_H
/**
 * @brief Adding units by value
 */
float unit_conversion(float value, char *units);

/**
 * @brief Speed ​​calculation
 */
void speed(packet_info_t* user, int number, int time);
#endif // SPEED_H