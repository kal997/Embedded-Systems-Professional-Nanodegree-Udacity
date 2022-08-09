#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 

#include<stdio.h>
#include<string.h>
#include "card.h"


EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	EN_cardError_t err;
	uint8_t enteredName[30];
	uint8_t enteredNameLen;
	printf("Please enter card holder name: ");
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

	printf("Please enter card expiration date: ");
	
	char enteredDate[10];
	gets(enteredDate);
	uint8_t enteredDateLen = strlen(enteredDate);
	uint8_t i, cnt=0;
	
	//check for correct length.
	if (enteredDateLen == 0 || enteredDateLen != 5)
	{
		return WRONG_EXP_DATE;
	}
	
	else
	{
		// check if the entered date has a valid format, a one character seperating month and year, not less or not more.
		for (i = 0; i < enteredDateLen; i++)
		{
			if (enteredDate[i] < '0' || enteredDate[i] > '9') cnt++;
		}


		if (cnt!=1) return WRONG_EXP_DATE;
		


		// to unify the separating symbol.
		enteredDate[2] = '-';
		stringCopy(enteredDate, cardData->cardExpirationDate);
		return OK;
		
	}

	

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
