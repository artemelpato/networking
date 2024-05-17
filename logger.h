/**
 * @file Logger module
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdbool.h>

/**
 * Logs and exits
 */
void log_and_exit(const char* msg);

/**
 *  Logs info
 */
void log_info(const char* msg);

/**
 * @brief Checks condition, exit with log if true
 */
void check_and_exit(bool cond, const char* msg);

#endif // __LOGGER_H__
