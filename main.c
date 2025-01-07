#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define HASH_SIZE 32

const char HEX_DIGITS[16]  = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

// Constants for the LCG (a * x + c) % m
const unsigned long long a = 256; // 2^8
const unsigned long long c = 65536; // 2^16
const unsigned long long m = 4294967296; // 2^32

// Generate a random number using a Linear Congruential Generator (LCG)
unsigned long long semi_random_number(unsigned long long seed, unsigned long long max_len) {
    // Generate a "random" number
    unsigned long long random_number = (a * seed + c) % m;

    return random_number % max_len;
}

char * hash_str(char *input_str) {
    int i = 0;
    unsigned long long hash_number;

    // Preallocate memory for the hash
    char *hash = malloc(HASH_SIZE * sizeof(char));

    unsigned int oscilator = 1;

    while (input_str[i] != '\0') {
        // Convert the ASCII code to 64-bit long long
        unsigned long long char_code_long = (unsigned long long) input_str[i];
        char_code_long = char_code_long + semi_random_number(char_code_long + i, 3);
        
        // Repeat 16 bits 4 times to make 64-bit long long
        const unsigned long long char_code_64 = char_code_long << 48 | char_code_long << 32 | char_code_long << 16 | char_code_long;

        // XOR with left-shifted hash
        hash_number = hash_number ^ char_code_64;

        // Apply bitwise operations
        if (oscilator) {
            // Rotate right 8 bits
            hash_number = (hash_number >> 8) | (hash_number << (64 - 8));
            oscilator = 0;
        } else {
            // Rotate left 4 bits
            hash_number = (hash_number << 4) | (hash_number >> (64 - 4));
            oscilator = 1;
        }

        i++;
    }

    for (i = 0; i < HASH_SIZE; i++) {
        hash[i] = HEX_DIGITS[hash_number % 16];
        hash_number = hash_number * 2 / 3;
    }

    return hash;
}

int tests() {
    // Basic string to test
    char *test_str = "Hello, World!";
    char *test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    // Slightly changed to check randomness
    test_str = "Hello, World";
    test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    // Testing "a" and "b"
    test_str = "a";
    test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    test_str = "b";
    test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    // Long string to test if it works correct
    test_str = "Long string to test if it works correct, lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec auctor, nunc nec ultricies ultricies, nunc nunc.";
    test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    return 1;
}

int main() {
    char input_str[256];

    // Get input from user
    printf("Enter a string: ");
    fgets(input_str, sizeof(input_str), stdin);

    // Remove the newline character if present
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
    }

    printf("You entered: %s\n", input_str);

    char *hash = hash_str(input_str);
    printf("Hash: %s\n", hash);

    return 0;
}