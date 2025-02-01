#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void string_to_hex(const char *str) {
    while (*str) {
        printf("%02x", (unsigned char)*str);
        str++;
    }
    printf("\n");
}

void hex_to_string(const char *hex) {
    while (*hex && *(hex + 1)) {
        char byte[3] = { *hex, *(hex + 1), '\0' };
        printf("%c", (char)strtol(byte, NULL, 16));
        hex += 2;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s --hex <string> | --text <hex>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--hex") == 0) {
        string_to_hex(argv[2]);
    } else if (strcmp(argv[1], "--text") == 0) {
        hex_to_string(argv[2]);
    } else {
        printf("Invalid option. Use --hex or --text.\n");
        return 1;
    }

    return 0;
}