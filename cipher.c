#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/*
    A data type to store relevant information
*/

struct Data{
    char site[512]; //It stores site name
    char password[256]; //It stores encrypted passwords
    int length; //It stores encrypted passwords lengths
    char key[25]; //It stores key on which the password is encrypted
};

/*
    Function to select option from a menu
*/

int menu(){

    printf("Select what you want to do from options 1-3:\n");
    printf("1. Add a website & password\n");
    printf("2. Print all the webites and passwords\n");
    printf("3. Quit\n");

    int selection = -1;
    while(selection < 0 || selection > 3){
        scanf("%d",&selection);
    }
}

//Function to encrypt password using Xor Cipher. The loop on key will be passed (passwordlength/keylength + remainder) times 

void encrypt(struct Data *d){
    int i,j;
    d->length = 0;
    for(i = 0,j=0; i < strlen(d->password); i++,j++){
        if(j >= strlen(d->key)){
            j = 0;
        }
        d->password[i] = d->password[i] ^ d->key[j];
        if(d->password[i]=='\0'){
            d->password[i]=d->key[j];
        }
        d->length++;
    }
}

//Function to decrypt password

struct Data decrypt(struct Data d){
    int i,j;
    d.length = 0;
    for(i = 0,j=0; d.password[i]!='\0'; i++,j++){
        if(j >= strlen(d.key)){
            j = 0;
        }
        d.password[i] = d.password[i] ^ d.key[j];
        d.length++;
    }
    return d;
}


//Function to write data to file.
void writetoFile(struct Data *d){
    FILE *file = fopen("Encrypted.txt", "w");
    if(!file){
        printf("Unable to open file");
    }
    else{
        char line[1024] = {"\0"};
        strcat(line, d->site);
        strcat(line, ",");
        strcat(line, d->password);
        strcat(line, ",");
        strcat(line, d->key);
        strcat(line, "\0");
        fputs(line, file);
    }

    printf("Encrypted and Stored in file.\n");

}


// The buffer read from file is passed then site, password and key is tokenized on "," then store it in data struct
struct Data tokenize(char *buf){
    struct Data d;
    const char s[2] = ",";
    char *token;
    token = strtok(buf, s);
    int i = 0;
    while( token != NULL ) {
        if(i == 0){
            strcpy(d.site, token);
            i++;
        }
        else if(i == 1){
            strcpy(d.password, token);
            i++;
        }  
        else if(i == 2){
            strcpy(d.key, token);
            i = 0;
        }  
    
        token = strtok(NULL, s);
   }

   return d;

}

//It reads line and call tokenize function, it also stores data into array

void readFile(struct Data *arr, int *currentIndex)
{
    FILE *file = fopen("Encrypted.txt", "r");
    if(!file){
        printf("Unable to open file");
    }
    else{
        struct Data d;
        char buffer[2056];
        while(fscanf_s(file, "%s", buffer) != EOF ){
            d = tokenize(buffer);
            //decrypt(&arr[*currentIndex]);
            arr[(*currentIndex)++] = d;
        }

    }
}

//Print passwords after decrypting
void printPasswords(struct Data arr[], int currentIndex){
    for(int i=0; i< currentIndex; i++){
        struct Data temp = decrypt(arr[i]);
        printf("%s\t\t%s\t\t%s\n", temp.site, temp.password ,temp.key);
    }
}


int main(){

    struct Data arr[100];
    int count = 0;

    readFile(arr, &count);

    int selectedOption = 0;
    while(selectedOption!=3){
        selectedOption = menu();

        if(selectedOption == 1){
            struct Data input;
            printf("Input Site: ");
            scanf("%s",&input.site);
            printf("Input Password: ");
            scanf("%s", &input.password);
            printf("Enter Master Key: ");
            scanf("%s", &input.key);
            encrypt(&input);
            arr[count++] = input;
            writetoFile(&input);
        }

        if(selectedOption == 2){
            printPasswords(arr,count);
        }
    }
    return 0;
}