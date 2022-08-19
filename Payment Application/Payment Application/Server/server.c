#include<stdio.h>
#include"server.h"

// -1 (not found), any non-negative(found)
static int16_t accountIDx = -1;


static ST_accountsDB_t accountsDB[4] = { { 15000.5, "27983973780030443" }, {20500.2, "5261716147787190"}, {1456.6, "5261716147787190"}, {14590.352, "2170049069582404320"}};
static ST_transaction_t transactionDB[255] = { 0 };
static uint8_t transactionDBIdx = 0;



EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	uint8_t i;
	for (i = 0; i < sizeof(accountsDB) / sizeof(accountsDB[0]); i++)
	{
		if (!strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber))
		{
			accountIDx = i;
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (termData->transAmount > accountsDB[accountIDx].balance)
	{
		printf("\n   %f\n", accountsDB[accountIDx].balance);
		return LOW_BALANCE;
	}
	printf("\n   %f\n", accountsDB[accountIDx].balance);
	return SERVER_OK;
	
	

}