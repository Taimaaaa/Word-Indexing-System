//Taymaa Nasser  1222640  Section 5.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//type defining pointers of my structs for easier use
typedef struct node *ptr;
typedef ptr MyTree;

typedef struct HashCell *TablePtr;
typedef TablePtr MyTable;

typedef struct node  //struct for avl nodes
{
    char word [100];
    ptr left;
    ptr right;
    int height;
    int frequency;
} node;
typedef struct HashCell //struct for hash table cells
{
    char word [100];
    int frequency;
    char state; //E for empty O for occupied and D for deleted
} HashCell;

//global variables for the original hash table size and capacity
int TableSize=0;
int TableCapacity=227;

//declaring all functions used in project
void displayMenu();
ptr loadStringFile(ptr root);
int alphabetCheck(char c);
int getHeight (ptr root);
int MAX (int x, int y);
ptr singleRotateLeft(ptr x);
ptr singleRotateRight(ptr y);
ptr doubleRotateLeft(ptr x);
ptr doubleRotateRight(ptr y);
ptr FindMin(ptr t);
ptr InsertAVL (ptr t, char newWord []);
ptr DeleteAVL( ptr T, char key[]);
void printInOrder(ptr t);
void printInOrderRecursive(ptr t);
int isPrime(int x);
int nearestPrime(int x);
int hashString(char* key, int TableSize);
void createTable(TablePtr table,int size);
void fillTable(ptr t, TablePtr hashTable);
TablePtr insertTable(TablePtr hashTable, char key[],int freq);
void printTable(TablePtr hashTable, int capacity);
int searchTable(TablePtr hashTable,char key[]);
void deleteTable(TablePtr hashTable, char key[]);
void wordStatistics(TablePtr hashTable, int capacity,int threshold);
TablePtr rehashTable(TablePtr hashTable);

int main()
{
    MyTree T=NULL; //initializing empty tree node that will be updated later in program
    MyTable HashTable = (MyTable)malloc(sizeof(struct HashCell)*TableCapacity); //allocating memory for hash table
    if (HashTable == NULL)
    {
        printf("Memory allocation failed for the hash table.\n");
    }

    int func=0; //initializing flag to enter and end menu.(when func==11, exit menu)
    int loadFlag=0; //initializing flag to prevent loading of txt file more than once
    int createFlag=0; //initializing flag to prevent creating of hash table more than once
    int in,threshold;
    char inserted[100],deleted[100],inserted2[100],deleted2[100],searched[100];  //declaring variables to store user-entered data in menu
    while(func!=11)
    {
        displayMenu();
        scanf("%d",&func);
        getchar(); //to deal with special characters entered by user.
        switch(func)
        {
        case 1:
            if(loadFlag==0)
            {
                T=loadStringFile(T);
                loadFlag=1;
            }
            else
                printf("You can not load the file more than once. Please select another operation. \n");
            break;
        case 2:
            if(loadFlag!=1) //forcing user to load file before creating AVL tree using the loadFlag.
            {
                printf("You must load file before printing it. Please select operation 1.\n");
            }
            else
                printf("Creating AVL tree...\n");
            printf("AVL tree created successfully!\n");
            break;
        case 3:
            printf("Please provide the new word to insert in tree:\n");
            gets(inserted);
            strlwr(inserted); //changes input to lowercase
            T=InsertAVL(T,inserted);
            printf("%s has been successfully inserted into tree!\n",inserted);
            break;
        case 4:
            printf("Please provide word you want to delete from Tree:\n");
            gets(deleted);
            T=DeleteAVL(T,deleted);
            break;
        case 5:
            printf("Printing AVL tree in order...\n");
            printInOrder(T);
            break;
        case 6:
            if(createFlag==0)
            {
                printf("Creating Hash Table...\n");
                createTable(HashTable,TableCapacity);
                fillTable(T,HashTable); //fills table from AVL tree
                printf("Hash Table created!\n");
                createFlag=1;
            }
            else
                printf("You cant create the table more than once!\n");

            break;
        case 7:
            if(createFlag==1)
            {
                printf("Please provide the new word to insert in hash table:\n");
                gets(inserted2);
                strlwr(inserted2);
                HashTable=insertTable(HashTable,inserted2,1);
            }
            else
                printf("You must create and fill table before inserting. Please select 6\n");
            break;
        case 8:
            printf("Please provide word you want to delete from hash table:\n");
            gets(deleted2);
            deleteTable(HashTable,deleted2);
            break;
        case 9:
            printf("Please provide the word you want to find in hash table:\n");
            gets(searched);
            strlwr(searched);
            in=searchTable(HashTable,searched);
            break;
        case 10:
            printf("Please provide the specific threshold for words repeated more than:\n");
            scanf("%d",&threshold);
            wordStatistics(HashTable,TableCapacity,threshold);
            break;
        case 11:
            printf("Thank you for using my word indexing system! Goodbye.\n"); //exiting system
            break;
        case 12:
            printTable(HashTable,TableCapacity); //option to print current hash table (unlisted in menu)
            break;
        default:
            printf("No such operation! Please try again.\n"); //deals with user-entered data that is not a number from 1-9
        }
    }
    return 0;
}


void displayMenu() //function to display main menu
{
    printf("Please select one functionality to run (1-11):\n\n");
    printf("1. Load the input file.\n");
    printf("2. Create the AVL tree.\n");
    printf("3. Insert a word to the AVL tree.\n");
    printf("4. Remove a word from the AVL tree.\n");
    printf("5. Print the words as sorted in the tree.\n");
    printf("6. Create the Hash Table.\n");
    printf("7. Insert a word to the Hash Table.\n");
    printf("8. Remove a word from the Hash Table.\n");
    printf("9. Search for a word in the hash table and print its frequency.\n");
    printf("10. Print words statistics.\n");
    printf("11. Exit System.\n");
}
ptr loadStringFile(ptr root) //function that loads input file input.txt and prints loaded info
{
    FILE *fp; //declaring a file pointer fp
    fp = fopen("input.txt","r"); //using fp to open input.txt file in read mode
    if (fp == NULL)
    {
        printf("Input file failed to load!\n");
        return;
    }
    char FullString[300]; //declaring a variable to contain the tokenized string read from file
    while(!feof(fp)) //read lines until end of file
    {
        char insertedWord[50]="";
        fgets(FullString,300,fp); //read until end of line
        char * token = strtok(FullString, " "); //tokenise the full string after every space " " char
        while(token != NULL )
        {
            int j=0; //initializing var for array of chars of the cleaned word (lower case, alphabets only!!)
            for(int i=0; i<strlen(token); i++) //going over all chars in the current word read from file
            {
                if(alphabetCheck(token[i])) //if the word is alphabets only
                {
                    insertedWord[j]=token[i];//set cleaned word to be inserted in AVL tree to the current word
                    j++; //next index in cleaned word array
                }
            }
            if(j>0) //if the cleaned word has at least one letter
            {
                insertedWord[j]='\0'; //set the last index to NULL
                strlwr(insertedWord); //set to lowercase
                root=InsertAVL(root,insertedWord); //insert cleaned word in AVL tree
            }

            token = strtok(NULL, " ");//tokenise the leftover string after space char.
        }
    }

    printf("Loading data input file ...\n");
    printf("Data file Loaded!\n\n");
    fclose(fp); //closing the file
    return root;
}
int alphabetCheck(char c) //function to check if a specific char is part of the alphabet (1) or not (0)
{
    if((c >= 'A' && c <= 'Z')||(c >= 'a' && c <= 'z'))
    {
        return 1; //is alphabet
    }
    else
        return 0; //is not part of the alphabet
}

int getHeight (ptr root) //function that returns height of specific node in AVL tree
{
    if(root==NULL)
        return -1;
    else
        return root->height;
}
int MAX (int x, int y) //function that returns the bigger number of two specific numbers
{
    if(x>y)
        return x;
    else
        return y;
}

ptr singleRotateLeft(ptr x) //function that rotates an imbalanced tree to the left one time
{
    ptr y;
    y= x->right;
    x->right= y->left;
    y->left=x;

    x->height = 1 + MAX(getHeight(x->left), getHeight(x->right));
    y->height = 1 + MAX(getHeight(x), getHeight(y->right));

    return y;
}
ptr singleRotateRight(ptr y) //function that rotates an imbalanced tree to the right one time
{
    ptr x;
    x= y->left;
    y->left= x->right;
    x->right= y;

    y->height = 1 + MAX(getHeight(y->right), getHeight(y->left));
    x->height = 1 + MAX(getHeight(y), getHeight(x->left));

    return x;
}

ptr doubleRotateLeft(ptr x) //function that rotates an imbalanced tree to the left one time then the right
{
    x->left= singleRotateLeft(x->left);
    return singleRotateRight(x);
}
ptr doubleRotateRight(ptr y) //function that rotates an imbalanced tree to the right one time then the left
{
    y->right= singleRotateRight(y->right);
    return singleRotateLeft(y);
}
ptr FindMin(ptr t) //function returns a pointer to the node in tree with the smallest data
{
    if(t == NULL)
        return NULL;

    else if( t->left == NULL)
        return t;

    else
        return FindMin(t->left);

}

ptr InsertAVL (ptr t, char newWord []) //function that inserts a word into AVL tree
{
    if(t==NULL) //reached a leaf node/empty tree so we can insert
    {
        t=(ptr)malloc(sizeof(struct node)); //allocating memory for new node
        if(t == NULL)
        {
            printf("Memory can not be allocated.\n");
            return NULL;
        }
        t->height=0;
        t->frequency=1;
        t->left= t->right=NULL;
        strcpy(t->word,newWord);
    }
    else if(strcmp(newWord,t->word)<0) //if inserted word is less than current word
    {
        t->left=InsertAVL(t->left,newWord); //use recursion to insert into left subtree
        if(getHeight(t->left) - getHeight(t->right) >= 2) //check and perform rotations in case of imbalances
        {
            if(strcmp(newWord,t->left->word)<0)
                t=singleRotateRight(t);
            else
                t=doubleRotateLeft(t);
        }
    }
    else if(strcmp(newWord,t->word)>0) //if inserted word is bigger than current word
    {
        t->right = InsertAVL(t->right, newWord); //use recursion to insert into right subtree
        if(getHeight(t->right) - getHeight(t->left) >= 2) //check and perform rotations in case of imbalances
        {
            if(strcmp(newWord,t->right->word)>0)
                t=singleRotateLeft(t);
            else
                t=doubleRotateRight(t);
        }
    }
    else //if inserted word is equal to current word (word exists in tree)
    {
        t->frequency++; //only increment its frequency
    }

    t->height = 1 + MAX(getHeight(t->right), getHeight(t->left)); //update node height

    return t;
}
ptr DeleteAVL( ptr t, char key[]) //function that deleted specific word from AVL tree
{
    ptr TmpCell;
    int deleted=0; //flag to check if word was deleted
    if(t == NULL)  //if tree is empty then key wasnt found
    {
        printf( "Word %s is not found in the AVL tree\n",key );
    }

    else if(strcmp(key,t->word)<0)  //if key word is less than current word
    {
        t->left = DeleteAVL( t->left,key  ); //use recursion to go into left subtree
    }

    else if(strcmp(key,t->word)>0)  //if key word is bigger than current word
    {
        t->right = DeleteAVL( t->right,key ); //use recursion to go into right subtree
    }
    else //if key word is equal to current word (word exists in tree)
    {
        if(t->left && t->right) //key node has two children, replace it with smallest node in right subtree
        {
            TmpCell = FindMin( t->right );
            strcpy(t->word, TmpCell->word);
            t->frequency = TmpCell->frequency;
            t->right = DeleteAVL(t->right, t->word );
        }

        else //key node has one or zero children
        {
            TmpCell = t;
            if( t->left == NULL )
            {
                t= t->right;
            }
            else if( t->right == NULL )
            {
                t= t->left;
            }
            free( TmpCell );
            deleted=1;
        }
    }

    if (t!= NULL)
    {
        t->height = 1 + MAX(getHeight(t->left), getHeight(t->right)); //update height

        //check balance factor and balance if necessary
        if (getHeight(t->left) - getHeight(t->right) >= 2)
        {
            if (t->left!=NULL && strcmp(key,t->left->word)>0)
                t = singleRotateRight(t);
            else
                t = doubleRotateLeft(t);
        }
        else if (getHeight(t->right) - getHeight(t->left) >= 2)
        {
            if (t->right!=NULL && strcmp(key,t->right->word)<0)
                t = singleRotateLeft(t);
            else
                t = doubleRotateRight(t);
        }
    }
    if (deleted)
    {
        printf("%s has been successfully deleted from the AVL tree!\n", key);
    }

    return t;
}
void printInOrder(ptr t) //function to print AVL tree in order traversal
{
    if (t == NULL)
    {
        printf("the AVL tree is empty.\n");
    }
    else
    {
        printInOrderRecursive(t);
    }
    printf("\n");
}
void printInOrderRecursive(ptr t) //function that helps traverse the tree recursively
{
    if(t!= NULL) //my base case
    {
        printInOrderRecursive(t->left);
        printf("%s: %d\t",t->word,t->frequency);
        printInOrderRecursive(t->right);
    }
}

int isPrime(int x) //function to check if a number is prime (1) or not (0)
{
    if (x <= 1)
        return 0;
    for(int num=2; num<x; num++)
    {
        if(x%num==0)
        {
            return  0;
        }
    }
    return 1;

}
int nearestPrime(int x) //function that returns the nearest prime number greater than x
{
    int prime=0;

    if (x <= 1)
        return 2;

    prime = x;
    int foundFlag = 0;
    while (!foundFlag)
    {
        prime++; // increment prime to check the next number
        if (isPrime(prime))
            foundFlag = 1;
    }
    return prime;
}
int hashString(char* key, int TableSize) //function to hash a string key into an integer value within the range of TableSize
{
    int hashValue = 0; //initializing the hash value

    while(*key != '\0') // iterating over each character of the key until NULL is reached
    {
        hashValue = (( hashValue * 32 )+ *key++) %TableSize; //update hash value using hash function (shifting 5 bits to left)
    }
    return hashValue;
}
void createTable(TablePtr table,int capacity) //function that creates and initializes hash table
{
    for(int i=0; i<capacity; i++)
    {
        table[i].frequency=0;
        table[i].state='E'; //set all slots as empty
    }
}
void fillTable(ptr t, TablePtr hashTable) //function to fill the hash table with words from my AVL tree
{
    if (t!= NULL)
    {
        fillTable(t->left, hashTable);
        hashTable=insertTable(hashTable, t->word,t->frequency);
        fillTable(t->right, hashTable);
    }
}
TablePtr insertTable(TablePtr hashTable, char key[],int freq) //function to insert value into hash table
{

    int hashed = hashString(key,TableCapacity); //hashing the word string first
    int ind = hashed; //initializing index
    int i= 1; //initializing counter for double hashing
    float LFactor=((float)TableSize/(float)TableCapacity); //calculating load factor for rehashing
    int h2=(223 - (hashed%223)); //second hash function for double hashing

    if(LFactor>=0.7)
    {
        printf("load factor exceeded accepted value, rehashing into bigger table...\n");
        hashTable=rehashTable(hashTable); //rehash table
        printf("Rehashing successful!\n");
    }

    if(TableSize == TableCapacity) //if table is full no more insertions are available
    {
        printf("The hash table is full, no more space to insert words \n");
        return hashTable;
    }

    while(hashTable[ind].state !='E') //traverse the table until we reach an empty node for inserting
    {
        if(strcmp(hashTable[ind].word, key) == 0 && hashTable[ind].state == 'O') // if key already exists then just update its frequency
        {
            printf("word %s already exists in the hash table.\n", key);
            hashTable[ind].frequency++;
            return hashTable;
        }
        ind = (hashed + (h2)*i)% TableCapacity; //calculate next index using double hashing
        i++;
    }

    //insert new key into the table
    hashTable[ind].state='O';
    strcpy(hashTable[ind].word, key);
    hashTable[ind].frequency=freq;
    TableSize++; //increment table size
    printf("%d: %s \n",ind,key);

    return hashTable;

}
int searchTable(TablePtr hashTable,char key[]) //function that returns index of searched word (-1) if word was not found.
{
    int hashed = hashString(key,TableCapacity); //hashing the word string first
    int ind = hashed; //initializing index
    int i = 1; //initializing counter for double hashing
    int h2=(223 -(hashed%223)); //second hash function for double hashing


    if(TableSize == 0)
    {
        printf("Hash Table is currently empty\n");
        return -1;
    }

    while(hashTable[ind].state != 'E') //traverse the table until we reach an empty node for inserting
    {
        if(strcmp(hashTable[ind].word, key) == 0 && hashTable[ind].state == 'O') // if key exists then just return its index
        {
            printf("Word %s exists at index: %d with frequency: %d\n",key,ind,hashTable[ind].frequency);
            return ind;
        }
        ind = (hashed + (h2)*i) % TableCapacity;
        if (ind == hashed) //if entire table was searched
        {
            printf("Word %s not found in the hash table\n", key);
            return -1;
        }
        i++;
    }
    printf("Word %s is not found in the hash table\n", key);
    return -1;
}
void deleteTable(TablePtr hashTable, char key[]) //function that marks a key in table as deleted
{
    int ind = searchTable(hashTable, key); //search for the key in the hash table
    if (ind != -1)  //if the key is found
    {
        hashTable[ind].state = 'D'; //mark the node as deleted
        TableSize--; //decrement the table size
        printf("Word %s has been successfully deleted from the table!\n", key);
    }

}

void printTable(TablePtr hashTable, int capacity) //function that prints content of table
{
    printf("***Hash Table Contents:***\n"); //iterating over all the slots in the hash table
    for (int i= 0; i< capacity; i++)
    {
        printf("%d State: %c ", i, hashTable[i].state);
        if (hashTable[i].state=='O')
        {
            printf("Word: %s\tFrequency: %d", hashTable[i].word, hashTable[i].frequency);
        }
        else
        {
            printf("Word: -\tFrequency: -");
        }
        printf("\n");
    }
}
void wordStatistics(TablePtr hashTable, int capacity,int threshold) //function that calculates hash table statistics
{
    int uniqueCounter = 0; //counter for number of unique words
    int freq = 0; //variable to store the highest frequency
    char mostFreq[100]; //array of chars to store the most frequent word
    int thresholdFlag = 0; //flag to indicate if words with frequency higher than the threshold are found

    printf("-The words with frequency higher than %d are:\n",threshold);
    for (int i= 0; i< capacity; i++) //iterating over all the slots in the hash table
    {
        if (hashTable[i].state == 'O') //slot has to be occupied
        {
            if(hashTable[i].frequency==1)
            {
                uniqueCounter++;
            }
            if(freq <hashTable[i].frequency)
            {
                freq=hashTable[i].frequency;
                strcpy(mostFreq,hashTable[i].word);
            }
            if(hashTable[i].frequency >threshold)
            {
                printf("%s: %d \n",hashTable[i].word,hashTable[i].frequency);
                thresholdFlag=1;
            }
        }
    }
    if(thresholdFlag==0)
    {
        printf("no words are higher than the threshold\n");
    }
    printf("-There are %d unique words in the table\n",uniqueCounter);
    printf("-Most frequent word is %s with frequency: %d\n",mostFreq,freq);

}
TablePtr rehashTable(TablePtr hashTable) //function to rehash table if load factor was too big
{
    int originalCapacity=TableCapacity; //variable that stores the original capacity

    TableCapacity = nearestPrime(2*TableCapacity); //find the capacity of the new rehashed table

    TablePtr rehashedTable=(TablePtr)malloc(sizeof (struct HashCell)*TableCapacity); //allocating memory for rehashed table

    if (rehashedTable == NULL)
    {
        printf("Memory allocation failed for the hash table.\n");
        return NULL;
    }
    TableSize=0; //resetting table size to 0 for rehashed table

    for (int i = 0; i < TableCapacity; i++) //initializing all slots in table to empty with frequency 0
    {
        rehashedTable[i].state = 'E';
        rehashedTable[i].frequency = 0;
    }

    for(int i=0; i<originalCapacity; i++)  //iterating through the original hash table to reinsert its elements into the rehashed table
    {
        if (hashTable[i].state=='O' )
        {
            rehashedTable=insertTable(rehashedTable, hashTable[i].word, hashTable[i].frequency);
        }
    }
    free(hashTable); //free memory allocated for original table
    return rehashedTable; //return pointer to new rehashed table
}
