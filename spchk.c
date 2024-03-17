#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#ifndef BUFLENGTH
#define BUFLENGTH 16
#endif

void processFile(const char *path, const char* dictionary) {
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
            if (buf[position] == '\n'){ // found a line starting a linestart and ending before position
                buf[position] = '\0';
                //buf + lineStart is where line begins, divide the line into words, hack off beginning and trailing punctuation, check against dictionary. (possibly make a new function for this)
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
    if (position > 0){
        if (position == bufLength){
            buffer = realloc(buffer, bufLength + 1);
        }
        buffer[position] = '\0';
        //buf + lineStart is where line begins, divide the line into words, hack off beginning and trailing punctuation, check against dictionary. (possibly make a new function for this)
    }

    close(fileDescriptor);
}

void traverseDir(const char *path, const char *dictionary) {
    DIR *dir;
    struct dirent * dirElement

    dir = opendir(path);
    if (dir == NULL){
        exit(EXIT_FAILURE);
    }

    while((dirElement = readdir(dir)) != NULL){
        if(strcmp(dirElement->d_name, "..") != 0 && strcmp(dirElement->d_name, ".") != 0){
            if (direElement->d_type == DT_DIR){
                char subDirPath[1024];
                snprintf(path, sizeof(path), "%s/%s", path, dirElement->d_name);
                traverseDir(subDirPath);
            }
            else {
                processFile((dirElement->d_name), dictionary)
            }
        }
    }
    
}    
int main() {

}
