/* Tien Pham 1001497208 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_arr(int arr[5][5]);
void print_board(int arr[5][5]);
int check_row(int arr[5][5]);
int check_col(int arr[5][5]);
int check_diag1(int arr[5][5]);
int check_diag2(int arr[5][5]);
int pick_number(int arr[75]);
int check_card(int arr[5][5], int x);
void make_bingo(int arr[5][5]);

void print_arr(int arr[5][5])
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void print_board(int arr[5][5])
{
    printf("\n%5c%8c%8c%8c%8c\n", 'B', 'I', 'N', 'G', 'O');
    for(int i = 0; i < 5; i++)
    {
        for(int n = 0; n < 41; n++)
            printf("%c", '-');
        printf("\n");
        for(int j = 0; j < 5; j++)
        {
            if(arr[i][j] == 0)
                printf("|%4c   ", 'X');
            else
                printf("|%4d   ", arr[i][j]);
        }
        printf("|\n");
    }
    for(int n = 0; n < 41; n++)
        printf("%c", '-');
}

int check_row(int arr[5][5])
{
    int count = 0;
    int valid = 0;   
    for(int i = 0; i < 5; i++)
    {
        count = 0;
        for(int j = 0; j < 5; j++)
        {
            if(arr[i][j] == 0)
            {
                count++;
            }
        }
        if(count == 5)
        {
            valid = 1;
        }
    }
    
    return valid;
}

int check_col(int arr[5][5])
{
    int count = 0;
    int valid = 0;
    for(int i = 0; i < 5; i++)
    {
        count = 0;
        for(int j = 0; j < 5; j++)
        {
            if(arr[j][i] == 0)
            {
                count++;
            }
        }
        if(count == 5)
        {
            valid = 1;
        }
    }

    return valid;
}

int check_diag1(int arr[5][5])
{
    int count = 0;
    int valid = 0;
    for(int i = 0; i < 5; i++)
    {
        if(arr[i][i] == 0)
        {
            count++;
        }
    }

    if(count == 5)
        valid = 1;
    return valid;
}

int check_diag2(int arr[5][5])
{
    int count = 0;
    int valid = 0;
    for(int i = 0; i < 5; i++)
    {
        if(arr[i][4-i] == 0)
        {
            count++;
        }
    }

    if(count == 5)
        valid = 1;
    return valid;
}

int pick_number(int arr[75])
{
    srand(time(0));
    int x = rand()%75+1;
    for(int i = 0; i < 75; i++)
    {
        if(x == arr[i])
        {
            x = rand()%75 + 1;
            i = 0;
        }
    }

    if(x < 16)
        printf("\nThe next number is: B%d", x);
    else if(x < 31)
        printf("\nThe next number is: I%d", x);
    else if(x < 46)
        printf("\nThe next number is: N%d", x);
    else if(x < 61)
        printf("\nThe next number is: G%d", x);
    else
        printf("\nThe next number is: O%d", x);
    return x;
}

int check_card(int arr[5][5], int x)
{
    int valid = 0;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(x == arr[i][j])
            {
                arr[i][j] = 0;
                valid = 1;
            }
        }
    }

    return valid;
}

void make_bingo(int arr[5][5])
{
    srand(time(0));
    int x = 0;
    int k = 0;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {   
            x = rand()%15 + 1 + 15*i;
            k = 0;
            while(k < 5)
            {
                if(x == arr[k][i])
                {
                    x = rand()%15 + 1 + 15*i;
                    k = 0;
                }
                else
                {
                    k++;
                }
            }
            arr[j][i] = x;
        }
    }
    arr[2][2] = 0;
}


int main(void)
{
    int intarr[5][5] = {};
    int used_nums[75] = {};
    int counter = 0;
    int new_num = 0;
    int game_state = 0;
    int verify_num = 0;
    char ans[2] = {};
    make_bingo(intarr);
    printf("\n");
    print_board(intarr);
    while(game_state == 0 && counter < 75)
    {
        new_num = pick_number(used_nums);
        used_nums[counter] = new_num;
        printf("\nIs this number on your bingo card? (Y/N): ");
        scanf("%s", ans);
        if(ans[0] == 'Y')
        {
            verify_num = check_card(intarr, new_num);
            if(!(verify_num))
                printf("That number is not on your card! The player is cheating\n");
            else
            {
                if(check_row(intarr) || check_col(intarr) || check_diag1(intarr) || check_diag2(intarr))
                {
                    game_state = 1;
                    if(check_row(intarr))
                    	printf("A row has been completed. The player has won!\n");
                    else if(check_col(intarr))
                    	printf("A column has been completed. The player has won!\n");
                    else if(check_diag1(intarr) || check_diag2(intarr))
                    	printf("A diagonal has been completed. The player has won!\n");
                    else
                    	printf("Win condition error. ???");
                }
            }
        }
        print_board(intarr);
        counter++;
    }
    if(counter == 75 && game_state != 1)
	    printf("\nAll numbers have been called. The player has lost.\n");
    return 0;
}
