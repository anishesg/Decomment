#include <stdio.h>

/* define constants representing each of 8 states in the DFA */
enum State {NORMAL, AFTER_SLASH, IN_COMMENT, AFTER_STAR, 
            IN_STRING, IN_CHAR, BACKSLASH_STRING, BACKSLASH_CHAR};

/* declare state handling functions to be used in main */
enum State handleNormal(int c, enum State currentState);
enum State handleAfterSlash(int c, enum State currentState);
enum State handleInComment(int c, enum State currentState);
enum State handleAfterStar(int c, enum State currentState);
enum State handleInString(int c, enum State currentState);
enum State handleBackslashString(int c, enum State currentState);
enum State handleInChar(int c, enum State currentState);
enum State handleBackslashChar(int c, enum State currentState);

/* main function called during execution */
int main(void) {
    /* tracks current number of lines read */
    int lineCount = 1;
    /* tracks last line at which a comment began */
    int errorLine = 1;
    /* current character in file */
    int c;
    /* current state of DFA */
    enum State state = NORMAL;

    /* reads characters until end of file */
    while ((c=getchar())!=EOF) {
        /* increment line count at every newline character */
        if (c=='\n') {
            lineCount += 1;
        }
        /* only update line of last comment when not in comment */
        if (state != IN_COMMENT && state != AFTER_STAR) {
            errorLine = lineCount;
        }
        switch(state) {
            case NORMAL:
                state = handleNormal(c, state);
                break;
            case AFTER_SLASH:
                state = handleAfterSlash(c, state);
                break;
            case IN_COMMENT:
                state = handleInComment(c, state);
                break;
            case AFTER_STAR:
                state = handleAfterStar(c, state);
                break;
            case IN_STRING:
                state = handleInString(c, state);
                break;
            case BACKSLASH_STRING:
                state = handleBackslashString(c, state);
                break;
            case IN_CHAR:
                state = handleInChar(c, state);
                break;
            case BACKSLASH_CHAR:
                state = handleBackslashChar(c, state);
                break;
        }
    }
    /* edge case: if last character of file is a slash */
    if (state == AFTER_SLASH) {
        putchar('/');
    }
    /* rejection states - file has ended while inside comment */
    if (state == IN_COMMENT || state == AFTER_STAR) {
        fprintf(stderr, "Error: line %i: unterminated comment\n", errorLine);
        return 1;
    } else {
        return 0;
    }
}

/* implement the NORMAL state of the DFA */
enum State handleNormal(int c, enum State currentState) {
    if (c == '/') {
        return AFTER_SLASH;
    } else if (c == '"') {
        putchar(c);
        return IN_STRING;
    } else if (c == '\'') {
        putchar(c);
        return IN_CHAR;
    } else {
        putchar(c);
        return NORMAL;
    }
}

/* implement the AFTER_SLASH state of the DFA */
enum State handleAfterSlash(int c, enum State currentState) {
    if (c == '*') {
        putchar(' ');
        return IN_COMMENT;
    } else {
        putchar('/');
        return handleNormal(c, NORMAL);
    }
}

/* implement the IN_COMMENT state of the DFA */
enum State handleInComment(int c, enum State currentState) {
    if (c == '*') {
        return AFTER_STAR;
    } else if (c == '\n') {
        putchar('\n');
        return IN_COMMENT;
    } else {
        return IN_COMMENT;
    }
}

/* implement the AFTER_STAR state of the DFA */
enum State handleAfterStar(int c, enum State currentState) {
    if (c == '/') {
        return NORMAL;
    } else {
        return handleInComment(c, IN_COMMENT);
    }
}

/* implement the IN_STRING state of the DFA */
enum State handleInString(int c, enum State currentState) {
    if (c == '"') {
        putchar(c);
        return NORMAL;
    } else if (c == '\\') {
        putchar(c);
        return BACKSLASH_STRING;
    } else {
        putchar(c);
        return IN_STRING;
    }
}

/* implement the BACKSLASH_STRING state of the DFA */
enum State handleBackslashString(int c, enum State currentState) {
    putchar(c);
    return IN_STRING;
}

/* implement the IN_CHAR state of the DFA */
enum State handleInChar(int c, enum State currentState) {
    if (c == '\'') {
        putchar(c);
        return NORMAL;
    } else if (c == '\\') {
        putchar(c);
        return BACKSLASH_CHAR;
    } else {
        putchar(c);
        return IN_CHAR;
    }
}

/* implement the BACKSLASH_CHAR state of the DFA */
enum State handleBackslashChar(int c, enum State currentState) {
    putchar(c);
    return IN_CHAR;
}