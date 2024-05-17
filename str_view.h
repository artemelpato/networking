#ifndef __STR_VIEW_H__
#define __STR_VIEW_H__

#include <stddef.h>

/**
 * @brief Non owning string view struct
 */
struct str_view {
    char*  str;
    size_t len;
};


/**
 * @brief Literal of non owning string view
 */
#define STR_VIEW_LITERAL(x) (struct str_view){ .str = x, .len = sizeof(x) - 1 }

/**
 * @brief Tokenize string view at specified delimeters
 * @param[in,ou] str String to tokenize
 * @param[in] delims Delimeters at which string will be tokenized
 * @return Next token
 */
struct str_view str_view__tokenize(struct str_view* str, struct str_view delims);

/**
 * @brief Compares two strings
 * @param[in] s1 First string
 * @param[in] s2 Second string
 * @return 1 if s1 is bigger, else -1,
 * number of divergent character, 0 if strings are the same
 */
int str_view__compare(struct str_view s1, struct str_view s2);

#endif
