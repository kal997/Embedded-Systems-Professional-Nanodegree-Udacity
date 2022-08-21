#include<stdio.h>
#include"server.h"



static ST_accountsDB_t accountsDB[] = { { 15000.5, RUNNING, "27983973780030443" }, {1456.6,BLOCKED, "5261716147787190"}, {14590.352,BLOCKED, "2170049069582404320"}};
static ST_transaction_t transactionDB[TRANSACTION_DB_SIZE] = { 0 };
static uint8_t transactionDBIdx = 0;
uint32_t number = 50;



EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t** accountRefrence)
{
	uint8_t i;
	for (i = 0; i < ACCOUNTS_DB_SIZE ; i++)
	{
		if (!strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber))
		{
			//printf("inside: local address:%p, address bta3 el ptr: %p, data in it(foundAcc address):NULL\n", &accountRefrence, accountRefrence);
			*accountRefrence = &accountsDB[i];
			//printf("inside: local address:%p, address bta3 el ptr: %p, data in it(foundAcc address):%p\n", &accountRefrence, accountRefrence, *accountRefrence);
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	EN_serverError_t err;
	switch (accountRefrence->state)
	{
	case BLOCKED:
		err = BLOCKED_ACCOUNT;
		break;
	case RUNNING:
		err = SERVER_OK;
		break;	
	}
	return err;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	if (termData->transAmount > accountRefrence->balance)
	{
		return LOW_BALANCE;
	}
	return SERVER_OK;
}
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	EN_serverError_t errSaveTransc;
	
	
	// check if the Transaction db is full
	if (transactionDBIdx < TRANSACTION_DB_SIZE)
	{
		uint32_t generatedTransactionSequenceNumber;
		// generating transaction sequence number (it's function of transaction's index in DB).
		generatedTransactionSequenceNumber = transactionDBIdx + 10;
		number = generatedTransactionSequenceNumber;

		// saving the transaction sequence number.
		transData->transactionSequenceNumber = generatedTransactionSequenceNumber;
		
		// saving the transaction struct, and increment the transactionDBIdx by 1 for the future transaction.
		transactionDB[transactionDBIdx++] = *transData;
		

		// checking if the transaction saved or not.
		ST_transaction_t* foundTransData = NULL;
		EN_serverError_t errGetTransaction = getTransaction(generatedTransactionSequenceNumber, foundTransData);
		if (errGetTransaction == SERVER_OK)
		{
			// if it's found, return serverOk
			errSaveTransc = SERVER_OK;
		}
		else if(errGetTransaction == TRANSACTION_NOT_FOUND) // not found, return savingFailed.
		{
			errSaveTransc = SAVING_FAILED;
		}
	}
	else
	{
		// not saved, return savingFailed
		errSaveTransc = SAVING_FAILED;
	}

	return errSaveTransc;
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	int8_t i;
	for (i = 0; i < transactionDBIdx; i++)
	{
		if (transactionDB[i].transactionSequenceNumber == transactionSequenceNumber)
		{
			transData = &transactionDB[i];
			return SERVER_OK;
		}
	}
	return TRANSACTION_NOT_FOUND;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_serverError_t errAccountValidation, errIsBlocked, errAmountAvailable, errRecieveTransaction = SERVER_OK, errSaveTransaction;
	ST_accountsDB_t* foundAccountPtr = NULL;
	//printf("before: address:%p, value in it(address of element): 0\n", &foundAccountPtr);
	errAccountValidation  = isValidAccount(&transData->cardHolderData, &foundAccountPtr);
	//printf("after: address:%p, value in it(address of element): %p\n", &foundAccountPtr, foundAccountPtr);
	switch (errAccountValidation)
	{
	case SERVER_OK:
		//printf("res: VALID ACCOUNT\n");
		//printf("%s\n", foundAccountPtr->primaryAccountNumber);
		//printf("%f\n", foundAccountPtr->balance);
		//printf("%s\n", foundAccountPtr->state == 0?"(RUNNING)":"(BLOCKED)");
		//printf("******************valid done ************************\n");

		errIsBlocked = isBlockedAccount(foundAccountPtr);

		if (errIsBlocked == BLOCKED_ACCOUNT)
		{
			//printf("BLOCKED\n******blocked done*******\n");
			errRecieveTransaction = DECLINED_STOLEN_CARD;
			//printf("declined 3shan stolen");
		}
		else if (errIsBlocked == SERVER_OK)
		{
			//printf("RUNNING\n********block done*********");
			errAmountAvailable = isAmountAvailable(&transData->terminalData, foundAccountPtr);
			if (errAmountAvailable == LOW_BALANCE)
			{
				//printf("\n * *******isAmountDone***\n*** mafesh fund******");
				errRecieveTransaction = DECLINED_INSUFFECIENT_FUND;
			}
			else if(errAmountAvailable == SERVER_OK)
			{
				//printf("\n * *******isAmountDone***\n*** fe fund******");
				transData->transState = APPROVED;
				errSaveTransaction = saveTransaction(transData);
				if (errSaveTransaction == SAVING_FAILED) errRecieveTransaction = INTERNAL_SERVER_ERROR;
				else if (errSaveTransaction == SERVER_OK)
				{
					//printf("\n * *******saveTransacDone***\n*********");
					//printf("\n balance: %f\n", foundAccountPtr->balance);
					foundAccountPtr->balance -= transData->terminalData.transAmount;
					//printf("\n balance: %f\n", foundAccountPtr->balance);
					errRecieveTransaction = APPROVED;
				}
			}
		}
		break;
	case ACCOUNT_NOT_FOUND:
		errRecieveTransaction = FRAUD_CARD;
		break;
	}
	return errRecieveTransaction;
}
