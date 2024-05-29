#include <stdio.h>
#include <stdlib.h>
#include "str_view.h"

static i32 str_view__is_delim(char c, struct str_view delims) {
    for (size_t i = 0; i < delims.len; ++i) {
        if (c == delims.str[i]) 
            return 1;
    }

    return 0;
}

static void str_view__strip_prec_ws(struct str_view* str) {
    size_t count = 0;

    if (!str->str || !str->len) 
        return;

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

static void str_view__strip_end_ws(struct str_view* str) {
    size_t count = 0;

    if (!str->str || !str->len) 
        return;

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

struct str_view str_view__tokenize(struct str_view* str, struct str_view delims) {
    size_t current_len = 0;

    if (!str->str || !str->len) 
        return (struct str_view){0};

    str_view__strip_prec_ws(str);
    str_view__strip_end_ws(str);

    for (char* c = str->str; c != str->str + str->len; ++c) {
        current_len++;

        if (str_view__is_delim(*c, delims)) {
            struct str_view token = {
                .str = str->str,
                .len = current_len - 1
            };

            str_view__strip_prec_ws(&token);
            str_view__strip_end_ws(&token);

            if (c + 1 > str->str + str->len) 
                return (struct str_view) {0};

            str->str = c + 1; 
            str->len -= current_len;

            str_view__strip_prec_ws(str);

            return token;
        }
    }

    struct str_view token = {
        .str = str->str,
        .len = str->len
    };

    str_view__strip_prec_ws(&token);
    str_view__strip_end_ws(&token);

    str->str = NULL;
    str->len = 0;

    return token;
}

i32 str_view__compare(struct str_view s1, struct str_view s2) {
    i32 count = 0;

    if (s1.len != s2.len)
        return  s1.len > s2.len ? 1 : -1;

    if (s1.len == s2.len && s1.str == s2.str) 
        return 0;

    for (size_t i = 0; i < s1.len; ++i) {
        count += s1.str[i] != s2.str[i];
    }

    return count;
}

/*int main() {*/
    /*StringView request = STR_VIEW_LITERAL(*/
        /*"GET /other_page.html HTTP/1.1\r\n\r\n"*/
        /*"Host: localhost:8080\n"*/
        /*"User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0\n"*/
        /*"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,;q=0.8\n"*/
        /*"Accept-Language: en-US,en;q=0.5\n"*/
        /*"Accept-Encoding: gzip, deflate, br\n"*/
        /*"Connection: keep-alive\n"*/
        /*"Referer: http://localhost:8080/\n"*/
        /*"Upgrade-Insecure-Requests: 1\n"*/
        /*"Sec-Fetch-Dest: document\n"*/
        /*"Sec-Fetch-Mode: navigate\n"*/
        /*"Sec-Fetch-Site: same-origin\n"*/
        /*"Sec-Fetch-User: ?1\n");*/

    /*StringView delims = STR_VIEW_LITERAL("\n\r");*/
    /*StringView subdelims = STR_VIEW_LITERAL(" ");*/
    
    /*StringView token = string_tokenize(&request, delims);*/
    /*while (token.str) {*/
        /*printf("token: |%.*s|\n", (int)token.len, token.str);*/

        /*StringView subtoken = string_tokenize(&token, subdelims);*/

        /*while (subtoken.str) {*/
            /*printf("\tsubtoken: |%.*s|\n", (int)subtoken.len, subtoken.str);*/

            /*if (string_compare(subtoken, STR_VIEW_LITERAL("GET")) == 0) {*/
                /*printf("GET request!!!\n");*/
            /*}*/

            /*subtoken = string_tokenize(&token, subdelims);*/
        /*}*/

        /*token = string_tokenize(&request, delims);*/
    /*}*/

    /*return 0;*/
/*};*/
