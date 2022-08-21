#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 

#include<stdio.h>
#include<string.h>
#include "card.h"

static void stringCopy(uint8_t*, uint8_t*);
static uint8_t stringLength(uint8_t* str);
static int8_t isAlphabetic(char* str);


EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	EN_cardError_t err;
	uint8_t enteredName[30];
	uint8_t enteredNameLen;
	printf("Please enter card holder name: ");
	gets_s(enteredName, 30);
	enteredNameLen = strlen(enteredName);

	// checking the length
	if (enteredNameLen == 0 || enteredNameLen > 24)
	{
		err = WRONG_NAME;
	}
	else
	{
		if (isAlphabetic(enteredName))
		{
			stringCopy(enteredName, cardData->cardHolderName);
			err = CARD_OK;
		}
		else
		{
			err = WRONG_NAME;
		}
		
	}
	return err;



}


EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{

	printf("Please enter card expiration date: ");
	
	char enteredDate[10];
	gets_s(enteredDate, 10);
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
		enteredDate[2] = '/';
		stringCopy(enteredDate, cardData->cardExpirationDate);
		return CARD_OK;
		
	}

	

}



EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{

	printf("Please enter card PAN number: ");

	char enteredPAN[25];
	gets_s(enteredPAN, 25);
	uint8_t enteredPANLen = strlen(enteredPAN);
	uint8_t i;
	if (enteredPANLen >= 16 && enteredPANLen <= 19)
	{
		for (i = 0; i < enteredPANLen; i++)
		{
			if (enteredPAN[i] < '0' || enteredPAN[i] > '9')
			{
				return WRONG_PAN;
			}
		}
		
		stringCopy(enteredPAN, cardData->primaryAccountNumber);
		return CARD_OK;
	}
	else
	{
		return WRONG_PAN;
	}

}

static void stringCopy(uint8_t* src, uint8_t* dest)
{
	uint8_t i;
	for (i = 0; src[i] != '\0'; i++) dest[i] = src[i];
	dest[i] = '\0';
}

static uint8_t stringLength(uint8_t* str)
{
	uint8_t len=0;
	
	while (str[len++])
	{
		printf("%d, ", str[len - 1]);
	}
	
	return len-1;

}


static int8_t isAlphabetic(char* str)
{
	int8_t i;
	for (i = 0; str[i] != '\0'; i++)
	{
		if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] == ' ')) continue;
		else return 0;

	}
	return 1;
}
