#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include "mtwister.h"
#include "Funcs.h"
#include <stdbool.h>
#include <string.h>
#include "debugmalloc.h"


//felszabadítja az eredetileg lefoglalt string helyét, de később fel kell oldani ennek is a helyét
char* Concatc(char* str, char ch)
{
    int len = strlen(str);
    char* strout = (char*) malloc(sizeof(char) * (len + 2));

    strcpy(strout, str);
    free(str);

    strout[len] = ch;
    strout[len + 1] = 0;

    return strout;
}



//felszabadítja az eredetileg lefoglalt string helyét, de később fel kell oldani ennek is a helyét
char* Concats(char* str1, char* str2)
{
    int strlen1 = strlen(str1);
    int strlen2 = strlen(str2);
    char* str = (char*) malloc(sizeof(char) * (strlen1 + strlen2 + 1));

    strcpy(str, str1);
    free(str1);

    int i = strlen1;
    int j = 0;

    while (i <= strlen1 + strlen2)
    {
        str[i++] = str2[j++];
    }

    return str;
}



//fel kell szabadítani a szöveget, beolvassa egy nagy stringbe az egész szöveget
char* readfromtxt(FILE *fp)
{
    // read one character at a time and
    // display it to the output
    const unsigned max_len = 32;
    char buffer[max_len];

    char* str = (char*) malloc(sizeof(char));
    *str = 0;
    while (fgets(buffer, max_len, fp))
    {
        str = Concats(str, buffer);
    }

    return str;
}



//hozzárak egy szavat a tömbhöz, felszabadítja az előző tömböt
char** Add(char** tomb, char* str)
{
    int len = 0;
    while (strcmp(tomb[len], "") != 0)
    {
        len += 1;
    }

    char** ujtomb = (char**) malloc(sizeof(char*) * (len + 2));

    for (int i = 0; i < len; ++i)
    {
        ujtomb[i] = tomb[i];
    }
    free(tomb);

    ujtomb[len] = str;
    ujtomb[len + 1] = "";


    return ujtomb;
}



//fel kell szabadítani, beolvassa a txt-ből a szavakt egy string tömbbe
char** Message(FILE *fp)
{
    char** tomb = (char**) malloc(sizeof(char*));
    char* word = (char*) malloc(sizeof(char));
    *word = 0;
    *tomb = "";

    char ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        if(ch != ' ')
        {

            word = Concatc(word, ch);

        }
        else if(strcmp(word, "") != 0)
        {
            tomb = Add(tomb, word);
            free(word);
            word = (char*) malloc(sizeof(char));
            *word = 0;
        }
    }

    tomb = Add(tomb, word);

    return tomb;
}




//felszabadítja a string tömböt
void Free(char** tomb)
{
    for (int i = 0; strcmp(tomb[i], "") != 0; ++i)
    {
        free(tomb[i]);
    }
    free(tomb);
}



int main()
{

    FILE *fp = fopen("Key.txt", "r");
    char* str = readfromtxt(fp);

    printf("\n%s\n", str);
    free(str);
    // close the file
    fclose(fp);


    FILE *fp2 = fopen("Message.txt", "r");
    char** tomb = Message(fp2);

    printf("\n");
    for (int i = 0; strcmp(tomb[i], "") != 0; ++i)
    {
        printf("%s ",tomb[i]);
    }
    printf("\n");

    Free(tomb);
    // close the file
    fclose(fp2);


    return 0;
}
