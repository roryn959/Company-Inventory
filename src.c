#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_LENGTH 40
#define CAPACITY 1000
#define BARCODE_LENGTH 14

#define HELP 0
#define ENTER 1
#define ADD 2
#define SEARCH 3
#define REPORT 4
#define DELETE 5
#define SAVE 6
#define EXIT 7

struct item {
    char *name;
    char *barcode;
    double *price;
    int *quantity;
};

struct item *Stock[CAPACITY];

struct item *CreateItem(char *name, char *barcode, double price, int quantity)
{
    struct item *newItem = calloc(sizeof(struct item), 1);
    
    newItem->name = calloc(sizeof(char), strlen(name)+1);
    strcpy(newItem->name, name);

    newItem->barcode = calloc(sizeof(char), BARCODE_LENGTH);
    strcpy(newItem->barcode, barcode);

    newItem->price = calloc(sizeof(double), 1);
    *(newItem->price) = price;

    newItem->quantity = calloc(sizeof(int), 1);
    *(newItem->quantity) = quantity;

    return newItem;
}

char* getInput(char *message) //Input function referenced from example code.
{
    char buffer[MAX_LENGTH];
    printf("%s", message);
    fgets(buffer, MAX_LENGTH, stdin);
    size_t inputLength = strlen(buffer);
    char *input = calloc(sizeof(char), inputLength);
    strncpy(input, buffer, inputLength);
    input[inputLength] = '\0';
    return input;
}

int getChoice()
{
    int option = 0;
    char *input = getInput("Enter selection: ");
    for (int i=0; input[i]!='\0'; i++)
    {
    if (tolower(input[i])>='a' && tolower(input[i]<='z')) //If input is an alphabet character
        {
            return -1;
        }
    }
    sscanf(input,"%i",&option);
    free(input);
    return option;
}

void Enter()
{
    printf("\nEnter called\n");

    int i=-1;
    while (Stock[++i] != NULL)
    {
        if (i == CAPACITY-1)
        {
            printf("Sorry, stock is full!\n\n");
            return;
        }
    }
    char name[MAX_LENGTH];
    char barcode[BARCODE_LENGTH];
    double price;
    int quantity;

    printf("\nEnter name: ");
    scanf("%s", &name);
    printf("Enter barcode: ");
    scanf("%s", &barcode);
    printf("Enter price: ");
    scanf("%lf", &price);
    printf("Enter quanitity: ");
    scanf("%i", &quantity);
    fflush(stdin);
    if (quantity<0)
    {
        printf("\nCannot have negative quantity of stock\n\n");
        return;
    }
    Stock[i] = CreateItem(name, barcode, price, quantity);
    Stock[i+1] = NULL;
    printf("\nItem added successfully\n\n");
}

int Compare(char *s1, char *s2)
{
    int i=0;
    while (s1[i] != '\0' && s2[i] != '\0') //Until we reach the end either string
    {
        if (tolower(s1[i]) != tolower(s2[i]))
        {
            return 0;
        }
        i++;
    }
    //One string may have invisible unimportant characters. Find if any alphabet characters are remaining
    int x = i-1;
    while (s1[++x] != '\0')
    {
        if (tolower(s1[x])>='a' && tolower(s1[x])<='z')
        {
            return 0;
        }
    }
    x = i-1;
    while (s2[++x] != '\0')
    {
        if (tolower(s1[x])>='a' && tolower(s1[x])<='z')
        {
            return 0;
        }
    }
    return 1;
}

int FindItem(char *name)
{
    int i=-1;
    while (Stock[++i] != NULL)
    {
        if (Compare(name, Stock[i]->name))
        {
            return i;
        }
    }
    return -1; //Returns -1 if item was not found in stock.
}

void Alter()
{
    printf("\nAlter called\n\n");
    char *name = getInput("Enter the name of the item: ");
    int index = FindItem(name);
    if (index<0)
    {
        printf("\nError - could not find item: %s\n", name);
        free(name);
        return;
    }
    printf("\nItem found.\n");
    char *choice = getInput("\nWould you like to increase or decrease stock? (I/D)\n");
    int difference;
    printf("\nBy how much would you like to alter the quantity of %s", name);
    scanf("%i", &difference);
    if (tolower(choice[0])=='d')
    {
        difference *= -1;
    }
    int newVal = *(Stock[index]->quantity) + difference;
    fflush(stdin);
    if (newVal<0)
    {
        printf("\nCannot have negative quantity of stock.\n\n");
        free(name);
        free(choice);
        return;
    }
    printf("\nAdjusting stock of %s by %i\n\n", Stock[index]->name, difference);
    *(Stock[index]->quantity) = newVal;
    free(name);
    free(choice);
}

void Search()
{
    printf("\nSearch called\n\n");

    char *name = getInput("Enter the name of the item: ");
    
    int index = FindItem(name);
    if (index<0)
    {
        printf("\nError - could not find item: %s\n", name);
        free(name);
        return;
    }
    printf("\nName: %s\nBarcode: %s\nPrice: %.2f\nQuantity: %i\n\n",
    Stock[index]->name, Stock[index]->barcode, *(Stock[index]->price), *(Stock[index]->quantity));
    free(name);
}

void Report()
{
    if (Stock[0] == NULL)
    {
        printf("\nNothing in stock...\n\n");
        return;
    }
    int i=0;
    while (Stock[i] != NULL)
    {
        printf("\nEntry: %i\nName: %s\nBarcode: %s\nPrice: %.2f\nQuantity: %i\n",
        i+1, Stock[i]->name, Stock[i]->barcode, *(Stock[i]->price), *(Stock[i]->quantity));
        i++;
    }
    printf("\n");
}

void Delete()
{
    printf("\nDelete called\n\n");
    char *name = getInput("Enter name of item to be deleted: ");
    int index = FindItem(name);
    free(name);
    if (index<0)
    {
        printf("\nError - could not find item\n\n");
        return;
    }
    char *sure = getInput("\nItem found. Are you sure you want to delete this item? (Y/N) ");
    if (tolower(sure[0])!='y')
    {
        printf("\n**Deletion aborted**\n\n");
        free(sure);
        return;
    }

    free(Stock[index]->name);
    free(Stock[index]->barcode);
    free(Stock[index]->price);
    free(Stock[index]->quantity);
    free(Stock[index]);
    free(sure);
    do
    {
        Stock[index] = Stock[index+1];
    } while (Stock[++index]!=NULL);
    printf("\nItem successfully deleted\n\n");
}

void Save()
{
    printf("\nSaving data\n\n");
    FILE *data = fopen("data.txt", "w");
    if (data==NULL)
    {
        printf("Error - couldn't write to file - data NOT saved");
        fclose(data);
        return;
    }
    int i=-1;
    while (Stock[++i]!=NULL)
    {
        fprintf(data, "%s\n", Stock[i]->name);
        fprintf(data, "%s\n", Stock[i]->barcode);
        fprintf(data, "%lf\n", *(Stock[i]->price));
        fprintf(data, "%i\n", *(Stock[i]->quantity));
    }
    fclose(data);
    printf("Data saved successfully\n\n");
}

void Help()
{
    printf("\nHelp menu: 0\nEnter new type of item: 1\nIncrease or decrease quantity of stock: 2\nSearch for item: 3\nGet report of stock: 4\nDelete item: 5\nSave data: 6\nLog out: 7\n\n");
}

void Execute(int choice)
{
    switch (choice)
    {
        case HELP : Help(); break;
        case ENTER : Enter(); break;
        case ADD : Alter(); break;
        case SEARCH : Search(); break;
        case REPORT : Report(); break;
        case DELETE : Delete(); break;
        case SAVE : Save(); break;
        default : printf("\nInvalid entry. Input 0 for help\n\n");
    }
}

void InitialiseSampleData()
{
    Stock[0] = CreateItem("Bread", "07985277217", 0.50, 45);
    Stock[1] = CreateItem("Apple Juice", "56788", 1.30, 30);
    Stock[2] = CreateItem("Crumpets", "656434", 2.10, 200);
    Stock[3] = CreateItem("Eggs", "9584", 0.50, 12);
    Stock[4] = CreateItem("Oranges", "12343", 0.45, 150);
}

int go()
{
    //InitialiseSampleData();
    while (1)
    {
        int choice = getChoice();
        if (choice == 7)
        {
            char *sure = getInput("\nPlease make sure you have saved before logging off. Are you sure? (Y/N) ");
            printf("\n");
            if (tolower(sure[0])=='y')
            {
                break;
            }
        }
        else {Execute(choice);}
    }
    return 0;
}

char* FindUser(char *username)
{
    FILE *login_file = fopen("logins.txt", "r");
    if (login_file==NULL)
    {
        printf("\nNo login file detected. Program will create one\n");
        login_file = fopen("logins.txt", "w");
        char blank[] = {"-\n\n"};
        fprintf(login_file, "%s", blank);
        fclose(login_file);
        return "0";
    }
    char name[MAX_LENGTH];
    char *pass = calloc(sizeof(char), MAX_LENGTH);
    while (fgets(name, MAX_LENGTH, login_file)!= NULL)
    {
        //printf("\nchecking name: %s", name);
        if (Compare(name, username))
        {
            fgets(pass, MAX_LENGTH, login_file);
            fclose(login_file);
            return pass;
        }
        fgets(name, MAX_LENGTH, login_file); //Skip one line
    }
    fclose(login_file);
    return "0";
}

void AddUser(char *username, char *password)
{
    if (FindUser(username)!="0")
    {
        printf("\nError - username already taken\n\n");
        return;
    }
    FILE *login_file = fopen("logins.txt", "a");
    if (login_file==NULL)
    {
        printf("\nError in adding new user");
    }
    else
    {
        fprintf(login_file, "%s", username);
        fprintf(login_file, "%s", password);
    }
    fclose(login_file);
}

void NewAccount()
{
    char *input_key = getInput("\n**Create new account**\n\nPlease enter admin key: ");
    FILE *key_file = fopen("key.txt", "r");
    char admin_key[MAX_LENGTH];
    fgets(admin_key, MAX_LENGTH, key_file);
    if (Compare(admin_key, input_key))
    {
        char *username = getInput("\nPlease enter a username: ");
        char *password = getInput("\nPlease enter password: ");
        AddUser(username, password);
        free(username);
        free(password);
        printf("\n");
    }
    else
    {
        printf("\nIncorrect admin key. New account creation aborted\n\n");
    }
    free(input_key);
    fclose(key_file);    
}

void LoadStock(char *username)
{
    printf("Loading data...\n\n");
    Stock[0] = NULL;
    int i=0;
    FILE *data_file = fopen("data.txt", "r");
    if (data_file==NULL)
    {
        printf("Error - failed to read stock data. Creating file if one is lacking\n\n");
        fopen("data.txt", "w");
        fclose(data_file);
        return;
    }
    char name[MAX_LENGTH];
    char barcode[MAX_LENGTH];
    double price;
    int quantity;

    while (fscanf(data_file, "%s", name)>0)
    {
        fscanf(data_file, "%s", barcode);
        fscanf(data_file, "%lf", &price);
        fscanf(data_file, "%i", &quantity);
        Stock[i] = CreateItem(name, barcode, price, quantity);
        Stock[++i] = NULL;
    }
}

int main()
{
    while (1)
    {
        char *username = getInput("Please enter your username, '0' to create a new username, or '1' to exit: ");
        if (Compare("1", username))
        {
            printf("\nProgram terminating");
            return 0;
        }
        if (Compare("0", username))
        {
            NewAccount();
            continue;
        }
        char *targetPassword = FindUser(username);
        if (targetPassword=="0")
        {
            printf("\nSorry, we couldn't find that user name in the database\n\n");
            continue;
        }
        else
        {
            char *attempt = getInput("\nPlease enter the password for your account: ");
            if (Compare(targetPassword, attempt))
            {
                printf("\n");
                LoadStock(username);
                go();
            }
            else
            {
                printf("\nIncorrect password\n\n");
            }            
        }
        free(targetPassword);
        free(username);
    }
}