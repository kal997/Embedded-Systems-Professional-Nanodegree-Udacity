#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 
#include<stdio.h>
#include"app.h"


static En_options_t getUserDecision(void);

void appStart(void)
{
	ST_cardData_t userCard;
	En_options_t userDecision = PROCEED;
	uint8_t choice;
	EN_cardError_t errCardHolderName, errCardExpiryDate, errCardPAN;
		
	
	// getCardHolderName
	errCardHolderName = getCardHolderName(&userCard);
	while((errCardHolderName != CARD_OK ) && (userDecision == PROCEED) )
	{
		printf("Wrong format\n");
		if (getUserDecision() == PROCEED) errCardHolderName = getCardHolderName(&userCard);
		else userDecision = QUIT;
		
	}

	// display
	if (userDecision ==	PROCEED)
	{
		system("cls");
		printf("NAME: %s\n\n", userCard.cardHolderName);
		errCardExpiryDate = getCardExpiryDate(&userCard);
	}
	
	// errCardExpiryDate
	while ((userDecision == PROCEED) && (errCardExpiryDate != CARD_OK) )
	{

		printf("Wrong date format, (expected format MM/YY).\n");
		if (getUserDecision() == PROCEED) errCardExpiryDate = getCardExpiryDate(&userCard);
		else userDecision = QUIT;

	}
	
	// display
	if (userDecision == PROCEED)
	{
		system("cls");
		printf("NAME: %s\n", userCard.cardHolderName);
		printf("NAME: %s\n\n", userCard.cardExpirationDate);
		errCardPAN = getCardPAN(&userCard);
	}
	

	// getCardPAN
	while ((userDecision == PROCEED) && (errCardPAN != CARD_OK))
	{

		printf("Wrong PAN format\n");
		if(getUserDecision() == PROCEED) errCardPAN = getCardPAN(&userCard);
		else userDecision = QUIT;
	}

	// display
	if (userDecision == PROCEED)
	{
		system("cls");
		printf("NAME: %s\n", userCard.cardHolderName);
		printf("EXP DATE: % s\n", userCard.cardExpirationDate);
		printf("PAN: %s\n\n", userCard.primaryAccountNumber);
		
	}
	

	/***************************** CARD DONE*******************************************/


	/***************************** TERMINAL *******************************************/

	En_options_t terminalDecision = PROCEED;
	ST_terminalData_t terminal;
	
	if (userDecision == PROCEED)
	{
		EN_terminalError_t errIsCardExpired, errIsValidPAN, errSetTransacAmount, errIsBelowMax;
		En_options_t userEnterAmountDecision = PROCEED;


		getTransactionDate(&terminal);
		setMaxAmount(&terminal);

		errIsCardExpired = isCardExpired(&userCard, &terminal);
		if (errIsCardExpired == EXPIRED_CARD)
		{
			printf("Declined Card\nExpired Card.");
		}
		else if (errIsCardExpired == TERMINAL_OK)
		{
			while ((terminalDecision == PROCEED) && (userEnterAmountDecision == PROCEED))
			{
				errSetTransacAmount = getTransactionAmount(&terminal);


				// getting a valid transaction amount
				while (errSetTransacAmount != TERMINAL_OK)
				{
					printf("invalid transaction amount\nTry Again.\n");
					errSetTransacAmount = getTransactionAmount(&terminal);
				}

				errIsBelowMax = isBelowMaxAmount(&terminal);
				switch (errIsBelowMax)
				{
				case EXCEED_MAX_AMOUNT:
					printf("Sorry, The entered amount exceeds the max amount for this terminal.\n");
					if (getUserDecision() == QUIT) terminalDecision = QUIT;
					break;
				case TERMINAL_OK:
					userEnterAmountDecision = QUIT;
					break;
				}
			}

		}
	}
	/***************************** TERMINAL DONE *****************************************/
	
	
	
	
	/***************************** SERVER *******************************************/
	En_options_t serverDecision;
	if ((terminalDecision == PROCEED) && (userDecision == PROCEED))
	{
		ST_transaction_t transaction;
		transaction.cardHolderData = userCard;
		transaction.terminalData = terminal;
		
		EN_transState_t errTransacState = recieveTransactionData(&transaction);
		switch (errTransacState)
		{
		case APPROVED:
			printf("Valid card.\n");
			printf("Suffecient fund.\n");
			printf("Saving transaction: Done\n");
			printf("Transaction succeeded.\n");
			break;
		case DECLINED_INSUFFECIENT_FUND:
			printf("Valid card.\n");
			printf("Declined insuffecient fund.\n");
			break;
		case DECLINED_STOLEN_CARD:
			printf("Declined stolen card.\n");
			break;
		case FRAUD_CARD:
			printf("Fraud Card.\n");
			break;
		case INTERNAL_SERVER_ERROR:
			printf("Valid card.\n");
			printf("Suffecient fund.\n");
			printf("Internal server error\n");
			break;
		default:
			break;
		}
	}
	else
	{
		printf("quitting..\n");

	}

	printf("Thanks.\n");







}

static En_options_t getUserDecision(void)
{
	uint8_t choice;
	while (1)
	{
		printf("\nQUIT (Y/N): ");

		choice = getchar();
		getchar();
		if ((choice == 'Y') || (choice == 'y')) return QUIT;
		else if ((choice == 'N') || (choice == 'n')) return PROCEED;
	}
	
}