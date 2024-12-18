/*
 * Client.c
 *
 *  Created on: 24 nov 2024
 *      Author: simon
 */


#include "Client.h"

void menu(){
	printf("\n\n(input format: <letter StringLenght>)"
			"\n(StringLenght: Min 6 , Max 32)\n\n"
			"enter one of the following options:\n"
			"n) numeric ( digits only )\n"
			"a) alphanumeric ( lowercase only )\n"
			"m) mixed (digits and lowercase)\n"
			"s) secure (digits and uppercase)");
}

int msgValidate(const char* messaggio, char* error) {
    char buffer[100]; 		//container of the message that can be edited
    char del[] = " ";		//set of delimiters
    strcpy(buffer, messaggio);
    char* tipo = strtok(buffer, del);
    char* len = strtok(NULL, del);
    int lenNum;

    if(*tipo=='q'){
        	strcpy(error, "exit from the menu...");
        	return 0;
        }
    if (len == NULL) {
    	strcpy(error, "string does not contain the space as required");
    	return 0;
    }
    if (*tipo == 'm' || *tipo == 'n' || *tipo == 's' || *tipo == 'a') {
       if ((lenNum = atoi(len)) != 0) {
           if (lenNum >= 6 && lenNum <= MAXPSW)
                return 1;
           else {
                 strcpy(error, "the length entered is outside the allowed range");
                 return 0;
           }
       } else {
              strcpy(error, "you did not enter a number after the space");
              return 0;
       }
    }
    strcpy(error, "the type of password entered is not allowed");
    return 0;
}
