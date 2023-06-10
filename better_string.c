#include <stdio.h>
#include <stdlib.h>
#include "better_string.h"

size_t string_count_tokens(StringView str, StringView delims) {
    size_t n = 0;
    for (size_t i = 0; i < str.len; ++i) {
        for (size_t j = 0; j < delims.len; ++j) {
            if (str.str[i] == delims.str[j])  {
                n++;
            }
        }
    }
     
    return n + 1;
}

int string_tokenize(StringView* out, size_t n, StringView str, StringView delims) {
    char* current_word = str.str;
    size_t current_len = 0;
    size_t out_count = 0;

    for (size_t i = 0; i < str.len; i++) {
        for (size_t j = 0; j < delims.len; j++) {
            if (str.str[i] == delims.str[j]) {
                out[out_count++] = (StringView) {
                    .str = current_word,
                    .len = current_len
                };
                current_word = &str.str[i + 1];
                current_len = 0;
            } else {
                current_len++;
            }
        }
    }

    out[out_count] = (StringView) {
        .str = current_word,
        .len = current_len
    };

    (void)n;

    return 0;
}

int main() {
    StringView request = STR_VIEW_LITERAL("hello, handsome, you, my, love");
    StringView delims = STR_VIEW_LITERAL(",");

    size_t n_tokens = string_count_tokens(request, delims);
    StringView* tokens = malloc(n_tokens * sizeof(StringView));
    string_tokenize(tokens, n_tokens, request, delims);


    for (size_t i = 0; i < n_tokens; i++) {
        printf("--- %.*s\n", (int)tokens[i].len, tokens[i].str);
    }

    return 0;
};
