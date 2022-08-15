#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"terminal.h"
uint8_t stringCompare(uint8_t* str1, uint8_t* str2);

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	EN_terminalError_t err;
	uint8_t transactionDate[11];
	time_t currentTimeInSec = time(NULLPTR);
	struct tm* currentLocalTimeStruct = localtime(&currentTimeInSec);
	uint8_t currentDay = currentLocalTimeStruct->tm_mday, currentMon = currentLocalTimeStruct->tm_mon+1;
	uint32_t currentYear = currentLocalTimeStruct->tm_year + 1900;

	if(currentDay > 9)
	{
		transactionDate[0] = (currentDay / 10)+'0';
		transactionDate[1] = (currentDay % 10) +'0';
	}
	else
	{
		transactionDate[0] = '0';
		transactionDate[1] = currentDay +'0';
	}
	transactionDate[2] = '/';


	if (currentMon > 9)
	{
		transactionDate[3] = (currentMon / 10) + '0';
		transactionDate[4] = (currentMon % 10) + '0';
	}
	else
	{
		transactionDate[3] = '0';
		transactionDate[4] = currentMon + '0';
	}
	transactionDate[5] = '/';


	for (int i = 3; i >=0; i--)
	{
		transactionDate[6 + i] = (currentYear % 10)+'0';
		currentYear /= 10;
	}
	transactionDate[10] = '\0';

	memcpy(termData->transactionDate, transactionDate, 11);
	err = TERMINAL_OK;
	return err;
}




EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	// 08/22
	// 12/08/2022

	uint8_t cardExpMon[3] = { cardData.cardExpirationDate[0],cardData.cardExpirationDate[1], '\0' };
	uint8_t cardExpYear[3] = { cardData.cardExpirationDate[3],cardData.cardExpirationDate[4], '\0'};


	uint8_t TransacMon[3] = { termData.transactionDate[3],termData.transactionDate[4], '\0' };
	uint8_t TransacYear[3] = { termData.transactionDate[8],termData.transactionDate[9], '\0'};

	printf("atoi test         card: %d-%d, terminal: %d-%d\n\n", atoi(cardExpMon), atoi(cardExpYear), atoi(TransacMon), atoi(termData.transactionDate[8]));

	/*
	if (atoi(cardExpYear) > atoi(TransacYear)) return TERMINAL_OK;
	else if (atoi(cardExpYear) < atoi(TransacYear)) return EXPIRED_CARD;
	else
	{
		if (atoi(cardExpMon) >= atoi(TransacMon)) return TERMINAL_OK;
		else return EXPIRED_CARD;
	}
	*/
	 

}















EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);


uint8_t stringCompare(uint8_t* str1, uint8_t* str2)
{
	uint8_t i;
	for (i = 0; str1[i] != '\0'; i++)
	{
		if (str1[i] != str2[i]) return 0;
	}
	return 1;
}