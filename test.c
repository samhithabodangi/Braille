#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define WINDOWS
#ifdef WINDOWS
    #include <windows.h>
#endif

//Letters, numbers, and Punctuation

const char lowercaseLetters[26] = {
    'a','b','c','d','e','f','g','h','i','j',
    'k','l','m','n','o','p','q','r','s','t',
    'u','v','x','y','z','w'
};
const char *lowercaseBraille[26] = {

    "{{1, 0}, {0, 0}, {0, 0}}",//a
    "{{1, 0}, {1, 0}, {0, 0}}",//b
    "{{1, 1}, {0, 0}, {0, 0}}",//c
    "{{1, 1}, {0, 1}, {0, 0}}",//d
    "{{1, 0}, {0, 1}, {0, 0}}",//e
    "{{1, 1}, {1, 0}, {0, 0}}",//f
    "{{1, 1}, {1, 1}, {0, 0}}",//g
    "{{1, 0}, {1, 1}, {0, 0}}",//h
    "{{0, 1}, {1, 0}, {0, 0}}",//i
    "{{0, 1}, {1, 1}, {0, 0}}",//j

    "{{1, 0}, {0, 0}, {1, 0}}",//k
    "{{1, 0}, {1, 0}, {1, 0}}",//l
    "{{1, 1}, {0, 0}, {1, 0}}",//m
    "{{1, 1}, {0, 1}, {1, 0}}",//n
    "{{1, 0}, {0, 1}, {1, 0}}",//o
    "{{1, 1}, {1, 0}, {1, 0}}", //p
    "{{1, 1}, {1, 1}, {1, 0}}", //q
    "{{1, 0}, {1, 1}, {1, 0}}", //r
    "{{0, 1}, {1, 0}, {1, 0}}", //s
    "{{0, 1}, {1, 1}, {1, 0}}", //t
    
    "{{1, 0}, {0, 0}, {1, 1}}", //u
    "{{1, 0}, {1, 0}, {1, 1}}", //v
    "{{1, 1}, {0, 0}, {1, 1}}", //x
    "{{1, 1}, {0, 1}, {1, 1}}", //y
    "{{1, 0}, {0, 1}, {1, 1}}", //z
    "{{0, 1}, {1, 1}, {0, 1}}" //w
};
const char numbers[10] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
};
const char punctuationMarks[26] = {
    '.', ',', ';', ':', '-',
    '?', '!', '"', '(', ')', '*',
    '$','%', '=', '+', '#',
    '|', '\'', '/', '\\', '{', '}',
    '[', ']', '@', '&'

};
const char *punctuationMarksBraille[26] = {
    "⠄", "⠂", "⠆", "⠒", "⠤",
    "⠢", "⠖", "⠦", "⠣", "⠜", "⠔",
    "⠸⠜", "⠸⠴", "⠶", "⠖", "⠼⠐",
    "⠸", "⠄", "⠠⠂", "⠐⠄", "⠐⠸", "⠸⠂",
    "⠷", "⠾", "⠐", "⠠⠯"
};

int main(int argc, char *argv[]) {

    #ifdef WINDOWS
        SetConsoleOutputCP(65001); 
    #endif

    //file
    printf("\n\"Text to Braille\"\n");
    printf("Translator of text documents (.txt) to braille (Unicode)\n\n");

    char sourceDocument[250]; 
    if (argc == 2) {
        strncpy(sourceDocument, argv[1], 250);
    } else {
        printf("Drag the text file (.txt) into this window or enter its path:\n");
        fgets(sourceDocument, 250, stdin);
        sourceDocument[strcspn(sourceDocument, "\n")] = 0;
    }
    printf("Opening '%s'...\n", sourceDocument);

    FILE *source = fopen(sourceDocument, "r"); 
    if (source == NULL) {
        printf("Could not open the file.\nDoes it exist? Is it being used by another application?\n");
        #ifdef WINDOWS
            system("pause");
        #endif
        exit(1);
    }

    char destinationDocument[250]; 
    strcpy(destinationDocument, sourceDocument);
    for (int i=0; i<4; i++) { 
        destinationDocument[strlen(destinationDocument)-1] = '\0';
    }
    strcat(destinationDocument, "(braille).txt"); 
    printf("\nDestination document (in the same directory as the source document):\n'%s'\n",destinationDocument);

    FILE *destination = fopen(destinationDocument, "w"); 
    if (destination == NULL) {
        printf("Could not create the destination document.");
        printf("Is there a document with the same name open by another application?");
        #ifdef WINDOWS
            system("pause");
        #endif
        exit(1);
    }

    //conversion
    int character;
    _Bool isNumeral = 0;

    while ((character = fgetc(source)) != EOF) {

        if (isalpha(character)) {
            if (isNumeral && islower(character) && character < 107)
                fprintf(destination, "%s", "{{0,0},{0,1},{0,0}}");

            if (islower(character)) { // Lowercase
                isNumeral = 0; 
                for (int i=0; i<26; i++) {
                    if (character == lowercaseLetters[i]) {
                        fprintf(destination, "%s", lowercaseBraille[i]);
                        break;
                    }
                }
            } else if (isupper(character)) { // Uppercase
                isNumeral = 0;
                for (int i=0; i<26; i++) {
                    if (tolower(character) == lowercaseLetters[i]) {
                        fprintf(destination, "%s%s", "{{0,1},{0,1},{0,1}}", lowercaseBraille[i]);
                        break;
                    }
                }
            }
        }

          else if (isspace(character)) { 
            isNumeral = 0;
            if (character == ' '){ 
                fprintf(destination, "%s", "⠀");
                continue;
            }
            else {
                fprintf(destination, "%c", character);
                continue;
            }
        }
        else if (isdigit(character)) { 
            isNumeral = 1; 
            if (character < 58) { 
                fprintf(destination, "%s%s", "{{0,1},{0,1},{1,1}}", punctuationMarksBraille[character - 48]);
            } else if (character == 48) { 
                fprintf(destination, "%s", "{{0,1},{0,1},{1,1}} {{0,1},{0,1},{1,1}}");
            }
        }

        else if (ispunct(character)) { 
            isNumeral = 0; 
            for (int i=0; i<26; i++) {
                if (character == punctuationMarks[i]) {
                    fprintf(destination, "%s", punctuationMarksBraille[i]);
                    break;
                }
            }
        }

        else if (character == 160) { 
            fprintf(destination, "%s", "⠀");
        }

        else {
            fprintf(destination, "%c", character); 
        }
    }

    fclose(source); 
    fclose(destination); 

    printf("\nConversion complete.\n");

    #ifdef WINDOWS
        system("pause");
    #endif
}