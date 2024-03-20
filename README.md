# Project 2 README

## Authors:
Leonardo Celi (lgc59)
Ammar Ghauri (amg610)

## Design Notes:
Description of spchkc.c library
---------------------------------

# spchk
Our program utilizes various data structures and file-handling techniques to achieve the assignment's objectives.

One of the key components of our program is its handling of the dictionary file. The program reads the dictionary file provided as the first argument and creates a TrieNode data structure to represent it. This TrieNode structure allows for efficient storage and retrieval of words, enabling fast dictionary lookups during the spelling check process.

Once the dictionary is loaded into memory, the program proceeds to process each text file specified in the arguments. This involves traversing directories recursively to find all relevant text files and reading the contents of each file using the read() function. The program carefully parses the text, considering line breaks and punctuation rules, to extract individual words for spelling checking.

The spelling check itself is a critical aspect of the program's functionality. For each word encountered in the text files, the program applies a series of checks to determine its correctness. This includes removing leading and trailing punctuation marks and allowing for variations in capitalization. The program then compares each word against the words stored in the dictionary TrieNode to identify any spelling errors. It does this by checking the children of the current letter, and if the next letter is one of the children it continues to check the next. When the whole word in the dictionary, or the whole path in the TrieNode strucutre is traversed, then a word has been found. If at any point the next character of the word is not one of the children of the current node in the TrieNode structure, the program increments the totalError counter as it has now found a spelling error. 

When a spelling error is detected, the program records information about the error, including the word itself, the file containing it, and the specific line and column number where the word appears. This meticulous error reporting ensures that users can easily identify and correct spelling issues in their text files.

Finally, the program ensures a smooth execution flow by handling various exit scenarios. If all files are successfully processed and contain no spelling errors, the program exits with a success status. However, if any issues are encountered during file processing or spelling checking, the program exits with a failure status, indicating the presence of errors that require attention.

# mydict 1 & 2
We also made two test libraries and files to test the functionality of handling variations of words, and reporting them in the right order, with the correct row and column number. The test files are simple but provides assurance that our program can handle specific edge cases. 
