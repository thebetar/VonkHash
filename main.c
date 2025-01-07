#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define HASH_SIZE 32

const char HEX_DIGITS[16]  = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char * hash_str(char *input_str) {
    int i = 0;
    unsigned long long hash_number = 2;

    char *hash = malloc(HASH_SIZE * sizeof(char));

    unsigned int oscilator = 1;

    while (input_str[i] != '\0') {
        // Get the ASCII code of the character
        const unsigned int char_code = (unsigned int)input_str[i];

        if (oscilator < 4) {
            // Convert the ASCII code to 64-bit long long
            const unsigned long long char_code_long = char_code;
            
            // Repeat 16 bits 4 times to make 64-bit long long
            const unsigned long long char_code_64 = char_code_long << 48 | char_code_long << 32 | char_code_long << 16 | char_code_long;

            if (oscilator == 1) {
                // Bitwise AND
                hash_number = hash_number & char_code_64;
            } else if (oscilator == 2) {
                // Bitwise OR
                hash_number = hash_number | char_code_64;
            } else if (oscilator == 3) {
                // Bitwise XOR
                hash_number = hash_number ^ char_code_64;
            }
        }

        if (oscilator == 4) {
            // Bitwise left shift
            hash_number = hash_number << char_code;
        } else if (oscilator == 5) {
            // Bitwise right shift
            hash_number = hash_number >> char_code;
        } else {
            // Bitwise NOT
            hash_number = ~hash_number;
        }

        oscilator = oscilator + 1 % 7;

        i++;
    }

    for (i = HASH_SIZE; i >= 0; i--) {
        hash[i] = HEX_DIGITS[hash_number % 16];
        hash_number = hash_number * 2 / 3;
    }

    return hash;
}

int main() {
    // Basic string to test
    char *test_str = "Hello, World!";
    char *test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    // Slightly changed to check randomness
    test_str = "Hello, World";
    test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    // Long string to test if it works correct
    test_str = "Long string to test if it works correct, lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec auctor, nunc nec ultricies ultricies, nunc nunc.";
    test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    return 0;
}