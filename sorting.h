/**
 * @file sorting.h
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SORTING_H
#define SORTING_H
/**
 * @brief Helper function to sort bytes
 */
int compare_bytes(const void *a, const void *b);

/**
 * @brief Helper function to sort pakets
 */
int compare_pakets(const void *a, const void *b);
#endif // SORTING_H
