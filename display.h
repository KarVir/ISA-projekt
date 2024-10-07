/**
 * @file isa-top.c
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

/**
 * @brief Vlákno pro výpis statistiky
 * 
 * @param arg 
 * @return void* 
 */
void* statistics_thread(void* arg);