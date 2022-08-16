#include<stdio.h>
#include"server.h"


static ST_accountsDB_t accountsDB[4] = { { 15000.5, "27983973780030443" }, {20500.2, "5261716147787190"}, {1456.6, "5261716147787190"}, {14590.352, "2170049069582404320"}};
static ST_transaction_t transactionDB[255] = { 0 };
static uint8_t transactionDBIdx = 0;


/*
This function will take card data and validate these data.
It checks if the PAN exists or not in the server's database.
If the PAN doesn't exist will return DECLINED_STOLEN_CARD, else will return OK
*/
EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	uint8_t i;
	for (i = 0; i < sizeof(accountsDB) / sizeof(accountsDB[0]); i++)
	{
		if (!strcmp(cardData->primaryAccountNumber ,accountsDB[i].primaryAccountNumber)) return SERVER_OK;
	}
	return ACCOUNT_NOT_FOUND;
}