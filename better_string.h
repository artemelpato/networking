#ifndef __BETTER_STRING_H__
#define __BETTER_STRING_H__

#include <stddef.h>

typedef struct StringView {
    char* str;
    size_t len;
} StringView;

typedef struct String {
    char* str;
    size_t len;
} String;

#define STR_VIEW_LITERAL(x) (StringView){ .str = x, .len = sizeof(x) - 1 }

size_t string_count_tokens(StringView str, StringView delims);
int    string_tokenize(StringView* out, size_t n, StringView str, StringView delims);

#endif
