#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

enum ProcessState {DEFAULT, IN_QUOTE, IN_SINGLE_CHAR, 
SINGLE_CHAR_ESCAPED, QUOTE_ESCAPED, INSIDE_COMMENT, 
POSSIBLE_COMMENT_BEGIN, POSSIBLE_COMMENT_END};

enum ProcessState handleDefaultState(int inputChar, int *currentLine) {
    enum ProcessState process;
    if (inputChar == '"') {
        putchar(inputChar);
        process = IN_QUOTE;
    }
    else if (inputChar == '\'') {
        putchar(inputChar);
        process = IN_SINGLE_CHAR;
    } 
    else if (inputChar == '/') {
        process = POSSIBLE_COMMENT_BEGIN;
    }
    else  {
        if (inputChar == '\n') {
            (*currentLine)++;
        } 
        putchar(inputChar);
        process = DEFAULT;     
    }
    return process;
}

enum ProcessState handleQuoteState(int inputChar) {
    enum ProcessState process;
    if (inputChar == '\\') {
        putchar(inputChar);
        process = QUOTE_ESCAPED;
    }
    else if (inputChar == '"') {
        putchar(inputChar);
        process = DEFAULT;
    } 
    else {
        putchar(inputChar);
        process = IN_QUOTE;
    }
    return process;
}

enum ProcessState handleSingleCharState(int inputChar) {
    enum ProcessState process;
    if (inputChar == '\\') { 
        putchar(inputChar);
        process = SINGLE_CHAR_ESCAPED;
    }
    else if (inputChar == '\'') {
        putchar(inputChar);
        process = DEFAULT;
    } 
    else {
        putchar(inputChar);
        process = IN_SINGLE_CHAR;
    }
    return process;
}

enum ProcessState handleQuoteEscapedState(int inputChar) {
    enum ProcessState process;
    putchar(inputChar);
    process = IN_QUOTE;
    return process; 
}

enum ProcessState handleSingleCharEscapedState(int inputChar) {
    enum ProcessState process;
    putchar(inputChar);
    process = IN_SINGLE_CHAR;
    return process; 
}

enum ProcessState handleCommentState(int inputChar) {
    enum ProcessState process;
    if (inputChar == '*') {
        process = POSSIBLE_COMMENT_END; 
    }
    else if (inputChar == '\n') {
        putchar(inputChar); 
        process = INSIDE_COMMENT; 
    }
    else {
        process = INSIDE_COMMENT;
    }
    return process;
}

enum ProcessState handlePossibleCommentBegin(int inputChar, int *currentLine, int *errorLine) {
    enum ProcessState process;
    if (inputChar == '/') {
        printf("/"); 
        process = POSSIBLE_COMMENT_BEGIN;
    }
    else if (inputChar == '\'') {
        printf("/%c", inputChar);
        process = IN_SINGLE_CHAR;
    }
    else if (inputChar =='"') {
        printf("/%c", inputChar); 
        process = IN_QUOTE;
    }
    else if (inputChar == '*') {
        printf(" ");
        process = INSIDE_COMMENT;
    } 
    else {
        printf("/%c", inputChar);
        process = DEFAULT;
    }
    return process;
}

enum ProcessState handlePossibleCommentEnd(int inputChar) {
    enum ProcessState process;
    if (inputChar == '*') { 
        process = POSSIBLE_COMMENT_END;
    }
    else if (inputChar == '\n') {     
        putchar(inputChar);
        process = INSIDE_COMMENT;
    }
    else if (inputChar == '/') { 
        process = DEFAULT;
    }
    else {
        process = INSIDE_COMMENT;
    }
    return process;
}

int main(void) {
    enum ProcessState process = DEFAULT;
    int currentLine = 1;
    int errorLine = currentLine;
    int inputChar;

    while ((inputChar = getchar()) != EOF) { 
        switch (process) {
            case DEFAULT: 
                process = handleDefaultState(inputChar, &currentLine);
                break;
            case IN_QUOTE:
                process = handleQuoteState(inputChar);
                break;
            case IN_SINGLE_CHAR: 
                process = handleSingleCharState(inputChar);
                break;
            case SINGLE_CHAR_ESCAPED: 
                process = handleSingleCharEscapedState(inputChar);
                break;
            case QUOTE_ESCAPED: 
                process = handleQuoteEscapedState(inputChar);
                break;
            case INSIDE_COMMENT: 
                process = handleCommentState(inputChar);
                break;
            case POSSIBLE_COMMENT_BEGIN: 
                process = handlePossibleCommentBegin(inputChar, &currentLine, &errorLine); 
                break;
            case POSSIBLE_COMMENT_END: 
                process = handlePossibleCommentEnd(inputChar);
                break;
        }

        if (inputChar == '\n' && process != POSSIBLE_COMMENT_BEGIN && process != INSIDE_COMMENT) {
            errorLine = currentLine;
        }
        
        if (inputChar == '\n' && process != DEFAULT) {
            currentLine = currentLine + 1; 
        } 
    }

    if (process == POSSIBLE_COMMENT_END || process == INSIDE_COMMENT) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", errorLine);
        return EXIT_FAILURE;
    }
    else {
        if (process == POSSIBLE_COMMENT_BEGIN) {
            printf("/");
        }
        return EXIT_SUCCESS;
    }
}