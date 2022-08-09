#include<stdio.h>
#include<string.h>
#include "card.h"





EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	EN_cardError_t err;
	uint8_t enteredName[30];
	uint8_t enteredNameLen;
	printf("Please Enter Card Holder Name: ");
	gets(enteredName);
	enteredNameLen = strlen(enteredName);

	if (enteredNameLen == 0 || enteredNameLen > 24)
	{
		err = WRONG_NAME;
		return err;
	}
	else
	{
		stringCopy(enteredName, cardData->cardHolderName);
		err = OK;
		return err;
	}

}


EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{

}


EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{

}

void stringCopy(uint8_t* src, uint8_t* dest)
{
	uint8_t i;
	for (i = 0; src[i] != '\0'; i++) dest[i] = src[i];
	dest[i] = '\0';
}
