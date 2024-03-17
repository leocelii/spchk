#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#ifndef BUFLENGTH
#define BUFLENGTH 16
#endif

#define ALPHABET_SIZE 128 //All ASCI Characters, including letters and punctuation. 
#define BUFSIZE 1024

// Creates definiton of a TrieNode, used to represent the dictionary that we will use to test all our text files against, for spelling. 
typedef struct TrieNode{
    struct TrieNode *children[ALPHABET_SIZE]; //uppercase and lowercase
    int wordEnd;
} TrieNode;

// Creates an instance of a TrieNode, and intializes its children as necessary.
TrieNode *newNode() {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node) {
        node->wordEnd = 0;
        for (int letter = 0; letter < ALPHABET_SIZE; letter++){
            node->children[letter] = NULL;
        }
    }
    return node;
}

// Inserts each word from the dictionary into our TrieNode dictionary. 
// This function is used to populate our TrieNode dictionary, with the words from the given dictionary.
void insertWord(TrieNode *rootNode, const char *word){
    TrieNode *node = rootNode;
    int length = strlen(word);
    for (int i = 0; i < length; i++){
        int letterIndex = (int)(word[i]);
        if (!node->children[letterIndex]){
            node->children[letterIndex] = newNode();
        }
        node = node->children[letterIndex];
    }
    node->wordEnd = 1;
}

int main(int argc, char *argv[]) {
    //read in dictionary as second argument and create a TrieNode to represent it
    if (argc < 3){
        printf("Requires the input of files/folders for spelling check.");
        exit(EXIT_FAILURE);
    }
    
    int dictionaryFd = open(argv[1], O_RDONLY);
    if (dictionaryFd == -1){
        printf("Error: Unable to open dictionary file.\n");
        exit(EXIT_FAILURE);
    }

    TrieNode *dictionary = createNode();
    char buffer[BUFSIZE];
    ssize_t bytesRead;
    char *wordStart = buffer;
    while ((bytesRead = read(dictionaryFd, buffer, BUFSIZE)) > 0) {
        for(int i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n') {
                buffer[i] == '\0';
                insertWord(dictionary, wordStart)
                wordStart = &buffer[i+1];
            }
        }
    }
    //inserts last word if not followed by a newLine char
    if (wordStart < (&buffer[BUFSIZE])){
        insertWord(dictionary, wordStart);
    }
    close(dictionaryFd);


    for (int i = 2; i < argc; i++){
        struct stat data;
        if (stat(argv[i], &data) == 0){
            if(S_ISDIR(data.st_mode)){
                traverseDir(argv[i], dictionary);
            } else if (S_ISREG(data.st_mode)) {
                processFile(argv[i], dictionary);
            } else {
                printf("Error: %s is neither a file nor a directory.\n", argv[i]);
                exit(EXIT_FAILURE);
            }
        }
        else {
            printf("Error: Unable to stat %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }
    
}

void traverseDir(const char *path, const char *dictionary) {
    DIR *dir;
    struct dirent * dirElement

    dir = opendir(path);
    if (dir == NULL){
        exit(EXIT_FAILURE);
    }

    while((dirElement = readdir(dir)) != NULL){
        if(dirElement->d_name[0] != ".") {
            char subDirPath[BUFSIZE];
            snprintf(subDirPath, sizeof(subDirPath), "%s/%s", path, dirElement->d_name);
            if (dirElement->d_type == DT_DIR){ //dirElement is a directory
                traverseDir(subDirPath, dictionary);
            } //dirElement is a file
            else if ((dirElement->d_type == DT_REG) && (strstr(dirElement->d_name, ".txt") != NULL)){
                processFile(subDirPath, dictionary)
            } 
        }
    }
    
}    

void processFile(const char *path, TrieNode *dictionary) {
    int fileDescriptor = open(path, O_RDONLY);
    if (fileDescriptor < 0) {
        perror(path);
        exit(EXIT_FAILURE);
    }
    int bufLength = BUFLENGTH
    char *buffer = malloc(BUFLENGTH);

    int position = 0;
    int bytesRead;
    int lineStart;

    while ((bytesRead = read(fileDescriptor, buffer + position, bufLength - position)) > 0) {
        lineStart = 0;
        int bufEnd = position + bytesRead;
        while (position < bufEnd) {
            if (buffer[position] == '\n') { // found a line starting a linestart and ending before position
                buffer[position] = '\0';
                processLine(buffer + lineStart, dictionary, path);
                lineStart = position + 1;
            }
            position++;
        }
    
        if (lineStart == position) {
            position = 0;
        }
        else if (lineStart > 0){
            int segmentLength = position - lineStart;
            memmove(buffer, buffer + lineStart, segmentLength);
            position = segmentLength;
        }
        else if (bufEnd == bufLength){
            bufLength *= 2;
            buffer = realloc(buffer, bufLength);
        }
    }
    //partial line in buffer after EOF
    if (position > 0){
        if (position == bufLength){
            buffer = realloc(buffer, bufLength + 1);
        }
        buffer[position] = '\0';
        processLine(buffer + lineStart, dictionary, path);
    }
    
    free(buffer);
    close(fileDescriptor);
}

void processLine(const char *line, TrieNode *dictionary, const char *path){
    char *splitWord = strtok(line," ");
    while (splitWord != NULL){
        removePunct(splitWord);
        if(!searchWord(dictionary, splitWord)){
            printf ("%s (%d, %d): %s", path, row, column, splitWord);
            // determine the row and column number of the incorrect word in the original text file.
            //add 1 to a universal error counter. at the end of the main function, if counter > 0, exit with EXIT_FAILURE. 
            // if all files could be opened and counter = 0, exit with EXIT_SUCCESS
        }
    }
}

// Once our TrieNode dictionary is fully populated, and we are in the process of spell-checking text files, this function 
// is used to check the spelling, letter by letter of the word, with the dictionary. It will skip over correctly spelled words,
// and return 0, upon a mispelled word. 
void searchWord(TrieNode *dictionary, const char *word){
    TrieNode *node = dictionary;
    int length = strlen(word);
    for (int i = 0; i < length; i++){
        int letterIndex = (int)word[i];
        if(!node->children[letterIndex]){
            return 0; //incorrect character found, word spelled incorrectly
        }
        node = node->children[letterIndex]
    }
    return node != NULL && node->wordEnd;
}

// removes all header and trailing punctuation from an input word
void removePunct(char *word) {
    int len = strlen(word);
    int start = 0;
    int end = len - 1;

    // remove starting punctuation
    while (start < len && ispunct(word[start]))
        start++;
    //remove ending punctuation
    while (end >= 0 && ispunct(word[end]))
        end--;

    memmove(word, word + start, end - start + 1);
    word[end - start + 1] = '\0';
}
