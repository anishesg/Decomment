#include <stdio.h>
#include <stdlib.h>

enum State {
  NORMAL,
  POTENTIAL_COMMENT,
  IN_COMMENT,
  STAR_COMMENT,
  STRING_LIT,
  CHAR_LIT,
  STRING_ESC,
  CHAR_ESC
};

enum State processStateNormal(int c) {
  enum State currentState = NORMAL;

  if (c == '/') {
    currentState = POTENTIAL_COMMENT;
  } else if (c == '\'') {
    putchar(c);
    currentState = CHAR_LIT;
  } else if (c == '"') {
    putchar(c);
    currentState = STRING_LIT;
  } else if (c == '\n') {
    putchar(c);
  } else {
    putchar(c);
  }
  
  return currentState;
}

enum State processStatePotentialComment(int c, int* startLine, int currLine) {
  enum State currentState = POTENTIAL_COMMENT;
  
  if (c == '*') {
    putchar(' ');
    currentState = IN_COMMENT;
    *startLine = currLine;
  } else {
    putchar('/');
    putchar(c);
    currentState = NORMAL;
  }
  
  return currentState;
}

enum State processStateInComment(int c) {
  enum State currentState = IN_COMMENT;

  if (c == '*') {
    currentState = STAR_COMMENT;
  } else if (c == '\n') {
    putchar(c);
  }

  return currentState;
}

enum State processStateStarComment(int c) {
  enum State currentState = STAR_COMMENT;

  if (c == '/') {
    currentState = NORMAL;
  } else if (c == '*') {
    currentState = STAR_COMMENT;
  } else {
    currentState = IN_COMMENT;
    if (c == '\n') {
      putchar(c);
    }
  }

  return currentState;
}

enum State processStateStringLit(int c) {
  enum State currentState = STRING_LIT;

  if (c == '"') {
    currentState = NORMAL;
  } else if (c == '\\') {
    currentState = STRING_ESC;
  }

  putchar(c);
  return currentState;
}

enum State processStateCharLit(int c) {
  enum State currentState = CHAR_LIT;

  if (c == '\'') {
    currentState = NORMAL;
  } else if (c == '\\') {
    currentState = CHAR_ESC;
  }

  putchar(c);
  return currentState;
}

enum State processStateStringEsc(int c) {
  putchar(c);
  return STRING_LIT;
}

enum State processStateCharEsc(int c) {
  putchar(c);
  return CHAR_LIT;
}

int main() {
  enum State currentState = NORMAL;
  int c;
  int lineNumber = 1;
  int commentLine = 0;

  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      lineNumber++;
    }

    switch (currentState) {
      case NORMAL:
        currentState = processStateNormal(c);
        break;
      case POTENTIAL_COMMENT:
        currentState = processStatePotentialComment(c, &commentLine, lineNumber);
        break;
      case IN_COMMENT:
        currentState = processStateInComment(c);
        break;
      case STAR_COMMENT:
        currentState = processStateStarComment(c);
        break;
      case STRING_LIT:
        currentState = processStateStringLit(c);
        break;
      case CHAR_LIT:
        currentState = processStateCharLit(c);
        break;
      case STRING_ESC:
        currentState = processStateStringEsc(c);
        break;
      case CHAR_ESC:
        currentState = processStateCharEsc(c);
        break;
    }
  }

  if (currentState == IN_COMMENT || currentState == STAR_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", commentLine);
    return EXIT_FAILURE;
  }

  if (currentState == POTENTIAL_COMMENT) {
    putchar('/');
  }

  return EXIT_SUCCESS;
}