#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 

#include<stdio.h>
#include<time.h>

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

	printf("%s", transactionDate);

	err = TERMINAL_OK;
	return err;
}




EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);