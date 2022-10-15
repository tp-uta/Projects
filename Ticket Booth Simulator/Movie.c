#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "MovieTheaterLib.h"
#include "QueueLib.h"
#include "BSTLib.h"
#include "ListLib.h"
#include "StackLib.h"

void PrintReceipts(SNODE **StackTop)
{
	SNODE *TempPtr = *StackTop;
	if(TempPtr == NULL)
	{
		printf("\nNo receipts\n");
	}
	else
	{
		char TempTicket[10] = {};
		LNODE *TicketPtr = NULL;
		printf("\nToday's receipts\n\n");
		while(TempPtr)
		{
			TicketPtr = TempPtr->TicketList;
			printf("\nReceipt #%d\n\n\t%s\n\n", TempPtr->ReceiptNumber, TempPtr->MovieTheaterName);
			while(TicketPtr)
			{
				ReturnAndFreeLinkedListNode(&TicketPtr, TempTicket);
				printf("\t%s\t", TempTicket);
			}
			pop(&TempPtr);
		}
	}
}

BNODE *PickAndDisplayTheater(BNODE *BSTRoot, char MovieTheaterMap[][MAXCOLS], int *MapRow, int *MapCol)
{
	BNODE *MyTheater = NULL;
	char zip[6] = {};
	char MyDims[6] = {};
	int row = 0;
	int col = 0;
	char delimiter[] = {"x"};
	printf("\n\nPick a theater by entering the zipcode\n\n");
				
	// Traverse the BST in order and print out the theaters in zip code order - use InOrder()			
	InOrder(BSTRoot);
	// Prompt for a zip
	printf("\nEnter zip: ");
	scanf("%s", zip);
	// Call SearchForBNODE()
	MyTheater = SearchForBNODE(BSTRoot, zip);
	// If theater is not found, then print message
	if(MyTheater == NULL)
	{
		printf("Selected theater is not found\n");
	}
	else
	{
		strcpy(MyDims, MyTheater->Dimensions);
		Token = strtok(MyDims, delimiter);
		row = atoi(Token);
		Token = strtok(NULL, delimiter);
		col = atoi(Token);
		if(ReadMovieTheaterFile(MovieTheaterMap, MyTheater->FileName, row, col))
		{
			PrintSeatMap(MovieTheaterMap, row, col);
		}
		else
		{
			printf("Unable to print that seat map at this time. Check back later.\n");
		}
	}
	return MyTheater;
}	

void ReadFileIntoQueue(FILE *QueueFile, QNODE **QH, QNODE **QT)
{
	//	read the passed in file and calls enQueue for each name in the file
	char buffer[20] = {};
	while(fgets(buffer, sizeof(buffer)-1, QueueFile))
	{
		enQueue(buffer, QH, QT);
	}
}

void ReadFileIntoBST(FILE *BSTFile, BNODE **BSTnode)
{
	//	read the passed in file and tokenize each line and pass the information to AddBSTNode
	char buffer[100] = {};
	char Name[50] = {};
	char ZipCode[10] = {};
	char FileName[20] = {};
	char Dimensions[10] = {};
	char *Token = NULL;
	char delimiter[] = {"|"};
	while(fgets(buffer, sizeof(buffer)-1, BSTFILE))
	{
		Token = strtok(buffer, delimiter);
		strcpy(Name, Token);
		Token = strtok(NULL, delimiter);
		strcpy(ZipCode, Token);
		Token = strtok(NULL, delimiter);
		strcpy(FileName, Token);
		Token = strtok(NULL, delimiter);
		strcpy(Dimensions, Token);
		AddBSTNode(BSTnode, Name, ZipCode, FileName, Dimensions);
	}
}

void get_command_line_parameter(char *argv[], char ParamName[], char ParamValue[])
{
	int i = 0; 
	
	while (argv[++i] != NULL)
	{
		if (!strncmp(argv[i], ParamName, strlen(ParamName)))
		{
			strcpy(ParamValue, strchr(argv[i], '=') + 1);
		}
	}
}

int PrintMenu()
{
	int choice = 0;
	
	printf("\n\n1.	Sell tickets to next customer\n\n");
	printf("2.	See who's in line\n\n");
	printf("3.	See the seat map for a given theater\n\n");
	printf("4.	Print today's receipts\n\n");
	printf("Choice : ");
	scanf("%d", &choice);
	
	while (choice < 1 || choice > 4)
	{
		printf("Invalid choice.  Please reenter. ");
		scanf("%d", &choice);
	}
	
	return choice;
}

int main(int argc, char *argv[])
{
	int i, j, k;
	FILE *queueFile = NULL;
	FILE *zipFile = NULL;
	char arg_value[20];
	char queuefilename[20];
	char zipfilename[20];
	int ReceiptNumber = 0;
	int choice = 0;
	int SeatNumber;
	char Row;
	char Ticket[5];
	int ArrayRow, ArrayCol;
	int MapRow, MapCol;
	char MovieTheaterMap[MAXROWS][MAXCOLS] = {};
	LNODE *TicketLinkedListHead = NULL;
	QNODE *QueueHead = NULL;
	QNODE *QueueTail = NULL;
	BNODE *BSTRoot = NULL;
	BNODE *MyTheater = NULL;
	SNODE *StackTop = NULL;
	int NumberOfTickets = 0;
	
	if (argc != 4)
	{
		printf("%s QUEUE=xxxxxx ZIPFILE=xxxxx RECEIPTNUMBER=xxxxx\n", argv[0]);
		exit(0);
	}
	
	get_command_line_parameter(argv, "QUEUE=", queuefilename);
	get_command_line_parameter(argv, "ZIPFILE=", zipfilename);
	get_command_line_parameter(argv, "RECEIPTNUMBER=", arg_value);
	ReceiptNumber = atoi(arg_value);
		
	/* call function to open queuefilename - if it does not open, print message and exit */	
	queueFile = fopen(queuefilename, "r");
	if(queueFile == NULL)
		exit(0);
	/* call function to open zipfilename - if it does not open, print message and exit */
	zipFile = fopen(zipfilename, "r");
	if(zipFile == NULL)
		exit(0);
	ReadFileIntoQueue(queueFile, &QueueHead, &QueueTail);
	ReadFileIntoBST(zipFile, &BSTRoot);
	
	while (QueueHead != NULL)
	{
		choice = PrintMenu();
	
		switch (choice)
		{
			case 1 :
				printf("\n\nHello %s\n", QueueHead->name);				
				MyTheater = PickAndDisplayTheater(BSTRoot, MovieTheaterMap, &MapRow, &MapCol);
				if (MyTheater != NULL)
				{
					printf("\n\nHow many movie tickets do you want to buy? ");
					scanf("%d", &NumberOfTickets);
					for (i = 0; i < NumberOfTickets; i++)
					{
						do
						{
							printf("\nPick a seat (Row Seat) ");
							scanf(" %c%d", &Row, &SeatNumber);
							Row = toupper(Row);
							ArrayRow = (int)Row - 65;
							ArrayCol = SeatNumber - 1;
						
							if ((ArrayRow < 0 || ArrayRow >= MapRow) ||
								(ArrayCol < 0 || ArrayCol >= MapCol))
							{
								printf("\nThat is not a valid seat.  Please choose again\n\n");
							}		
						}
						while ((ArrayRow < 0 || ArrayRow >= MapRow) ||
							   (ArrayCol < 0 || ArrayCol >= MapCol));
						
						if (MovieTheaterMap[ArrayRow][ArrayCol] == 'O')
						{	
							MovieTheaterMap[ArrayRow][ArrayCol] = 'X';
							sprintf(Ticket, "%c%d", Row, SeatNumber); 
							InsertNode(&TicketLinkedListHead, Ticket);
						}
						else
						{
							printf("\nSeat %c%d is not available.\n\n", Row, SeatNumber);
							i--;
						}
						PrintSeatMap(MovieTheaterMap, MapRow, MapCol);
					}
					
					WriteSeatMap(MyTheater, MovieTheaterMap, MapRow, MapCol);
					push(&StackTop, TicketLinkedListHead, ReceiptNumber, MyTheater->MovieTheaterName);
					TicketLinkedListHead = NULL;
					ReceiptNumber++;
					printf("\nThank you %s - enjoy your movie!\n", QueueHead->name);
					deQueue(&QueueHead);
				}
				break;
			case 2 : 
				printf("\n\nCustomer Queue\n\n");
				DisplayQueue(QueueHead);
				printf("\n\n");
				break;
			case 3 :
				PickAndDisplayTheater(BSTRoot, MovieTheaterMap, &MapRow, &MapCol);
				break;
			case 4 : 
				PrintReceipts(&StackTop);
				break;
			default :
				printf("Bad menu choice");
		}
	}
	
	printf("Good job - you sold tickets to all of the customers in line.\n");
	PrintReceipts(&StackTop);
	
	return 0;
}
