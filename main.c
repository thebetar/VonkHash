#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define HASH_SIZE 64

const char HEX_DIGITS[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

// Constants for the LCG (a * x + c) % m
const unsigned long long A = 256;        // 2^8
const unsigned long long C = 65536;      // 2^16
const unsigned long long M = 4294967296; // 2^32

// Generate a random number using a Linear Congruential Generator (LCG)
unsigned long long semi_random_number(unsigned long long seed, unsigned long long max_len)
{
    // Generate a "random" number
    unsigned long long random_number = (A * seed + C) % M;

    return random_number % max_len;
}

char *hash_str(char *input_str)
{
    unsigned long long hash_number = 0;
    unsigned int oscilator = 1;
    size_t i = 0;

    // Preallocate memory for the hash
    char *hash = malloc(HASH_SIZE * sizeof(char));

    while (input_str[i] != '\0')
    {
        // Convert the ASCII code to 64-bit long long
        unsigned long long char_code_long = (unsigned long long)input_str[i];
        char_code_long += semi_random_number(char_code_long + i, 3);

        // Repeat 16 bits 4 times to make 64-bit long long
        const unsigned long long char_code_64 = (char_code_long << 48) | (char_code_long << 32) | (char_code_long << 16) | char_code_long;

        // Generate random number to rotate
        const unsigned int random_rotate = semi_random_number(hash_number, 2);

        // Apply bitwise operations
        if (oscilator)
        {
            // Rotate right 8 bits
            hash_number = (hash_number >> random_rotate) | (hash_number << (64 - random_rotate));
            oscilator = 0;
        }
        else
        {
            // Rotate left 4 bits
            hash_number = (hash_number << random_rotate) | (hash_number >> (64 - random_rotate));
            oscilator = 1;
        }

        // XOR with left-shifted hash
        hash_number ^= char_code_64;

        i++;
    }

    for (i = 0; i < HASH_SIZE; i++)
    {
        hash[i] = HEX_DIGITS[hash_number % 16];
        hash_number = hash_number * 2 / 3;
    }
    hash[HASH_SIZE] = '\0';

    return hash;
}

int tests()
{
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

    test_str = "aa";
    test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    // Long string to test if it works correct
    test_str = "Long string to test if it works correct, lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec auctor, nunc nec ultricies ultricies, nunc nunc.";
    test_hash = hash_str(test_str);
    printf("Hash: %s\n", test_hash);

    return 1;
}

int main()
{
    char input_str[256];

    // Get input from user
    printf("Enter a string: ");
    fgets(input_str, sizeof(input_str), stdin);

    // Remove the newline character if present
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n')
    {
        input_str[len - 1] = '\0';
    }

    printf("You entered: %s\n", input_str);

    char *hash = hash_str(input_str);
    printf("Hash: %s\n", hash);

    return 0;
}