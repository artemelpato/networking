#include <stdio.h>
#include <stdlib.h>
#include "better_string.h"

static int string_is_delim(char c, StringView delims) {
    for (size_t i = 0; i < delims.len; ++i) {
        if (c == delims.str[i]) 
            return 1;
    }

    return 0;
}

void string_strip_prec_ws(StringView* str) {
    size_t count = 0;

    for (char* c = str->str; c != str->str + str->len; ++c) {
        if (*c == ' ') {
            count++;
            continue;
        }

        if (*c != ' ') {
            str->str = c;
            str->len -= count;
            return;
        }
    }

    return;
}

void string_strip_end_ws(StringView* str) {
    size_t count = 0;

    for (char* c = str->str + str->len - 1; c >= str->str; c--) {
        if (*c == ' ') {
            count++;
            continue;
        }

        if (*c != ' ') {
            str->len -= count;
            return;
        }

    }

    return;
};

StringView string_tokenize(StringView* str, StringView delims) {
    char* current_word = str->str;
    size_t current_len = 0;

    if (!str->str || !str->len) 
        return (StringView){0};

    for (char* c = str->str; c != str->str + str->len; ++c) {

        if (string_is_delim(*c, delims)) {
            StringView token = {
                .str = str->str,
                .len = current_len
            };

            string_strip_prec_ws(&token);
            string_strip_end_ws(&token);

            str->str = c + 1; 
            str->len -= current_len;

            string_strip_prec_ws(str);

            return token;
        }

        current_len++;
    }

    (void) current_word;
    (void) current_len;
    (void) delims;

    return (StringView){0};
}

int main() {
    StringView request = STR_VIEW_LITERAL("hello, han dsome, you, my, love");
    StringView delims = STR_VIEW_LITERAL(",");
    
    StringView token = string_tokenize(&request, delims);
    while (token.str) {
        printf("token: |%.*s|\n", (int)token.len, token.str);
        token = string_tokenize(&request, delims);
    }

    return 0;
};
