/*
 * Server.h
 *
 */

#ifndef SERVER_H_
#define SERVER_H_

#define QLEN 5
#define BUFFSIZE 10

#include "header.h"

// Function to choose a random character from a character set
char random_char(const char *charset);

// generate password only with numbers
void generate_numeric(char *output, size_t length);

// Generate password with only lowercase letters
void generate_alpha(char *output, size_t length);

// Generate passwords with lowercase letters and numbers
void generate_mixed(char *output, size_t length);

// Generate passwords with uppercase letters, numbers and symbols
void generate_secure(char *output, size_t length);


#endif /* SERVER_H_ */
