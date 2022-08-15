#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"terminal.h"

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

	
	if (atoi(cardExpYear) > atoi(TransacYear)) return TERMINAL_OK;
	else if (atoi(cardExpYear) < atoi(TransacYear)) return EXPIRED_CARD;
	else
	{
		if (atoi(cardExpMon) >= atoi(TransacMon)) return TERMINAL_OK;
		else return EXPIRED_CARD;
	}

	 

}















EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float transactionAmount;

	printf("Please enter the transaction amount: ");
	scanf("%f", &transactionAmount);
	if (transactionAmount <= 0) return INVALID_AMOUNT;
	
	termData->transAmount = transactionAmount;
	return TERMINAL_OK;

}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	float maxTransactionAmount;

	printf("Please enter the maximum transaction amount: ");
	scanf("%f", &maxTransactionAmount);
	if (maxTransactionAmount <= 0) return INVALID_MAX_AMOUNT;

	termData->maxTransAmount = maxTransactionAmount;
	return TERMINAL_OK;

}

