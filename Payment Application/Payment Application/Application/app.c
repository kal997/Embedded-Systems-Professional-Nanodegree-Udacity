#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 
#include<stdio.h>
#include"app.h"




void appStart(void)
{
	ST_cardData_t userCard;
	En_userOptions_t userDecision = PROCEED;
	uint8_t choice;
	EN_cardError_t errCardHolderName, errCardExpiryDate, errCardPAN;
		
	
	errCardHolderName = getCardHolderName(&userCard);
	while((errCardHolderName != CARD_OK ) && (userDecision == PROCEED) )
	{
		printf("Wrong format\n");
		printf("QUIT (Y/N): ");
		choice = getchar();
		getchar();
		if ((choice == 'Y') || (choice == 'y'))
		{
			userDecision = QUIT;
		}
		else if ((choice == 'N') || (choice == 'n'))
		{
			errCardHolderName = getCardHolderName(&userCard);
		}
		
	}

	if (userDecision ==	PROCEED)
	{
		system("cls");
		printf("NAME: %s\n\n", userCard.cardHolderName);
		errCardExpiryDate = getCardExpiryDate(&userCard);
	}
	

	while ((userDecision == PROCEED) && (errCardExpiryDate != CARD_OK) )
	{

		printf("Wrong date format, (expected format MM/YY).\n");
		printf("QUIT (Y/N): ");
		choice = getchar();
		getchar();
		if ((choice == 'Y') || (choice == 'y'))
		{
			userDecision = QUIT;
		}
		else if ((choice == 'N') || (choice == 'n'))
		{
			errCardExpiryDate = getCardExpiryDate(&userCard);
		}

	}
	if (userDecision == PROCEED)
	{
		system("cls");
		printf("NAME: %s\n", userCard.cardHolderName);
		printf("NAME: %s\n\n", userCard.cardExpirationDate);
		errCardPAN = getCardPAN(&userCard);
	}
	
	while ((userDecision == PROCEED) && (errCardPAN != CARD_OK))
	{

		printf("Wrong PAN format\n");
		printf("QUIT (Y/N): ");
		choice = getchar();
		getchar();
		if ((choice == 'Y') || (choice == 'y'))
		{
			userDecision = QUIT;
		}
		else if ((choice == 'N') || (choice == 'n'))
		{
			errCardPAN = getCardPAN(&userCard);

		}

	}


	if (userDecision == PROCEED)
	{
		system("cls");
		printf("NAME: %s\n", userCard.cardHolderName);
		printf("EXP DATE: % s\n", userCard.cardExpirationDate);
		printf("PAN: %s\n\n", userCard.primaryAccountNumber);
		getchar();
	}
	




}