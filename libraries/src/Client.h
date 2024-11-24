/*
 * Client.h
 *
 *  Created on: 24 nov 2024
 *      Author: simon
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "header.h"
#define BUFFSIZE 512
#define MAXPSW 32

void menu();

int msgValidate(const char* messaggio, char* error);

#endif /* CLIENT_H_ */
