/**
 * @file display.h
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "packet_handler.h"
#include "sorting.h"

#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * @brief Thread to dump statistics
 */
void* statistics_thread(void* arg);
#endif // DISPLAY_H