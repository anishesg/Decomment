#include <stdio.h>
#include <stdlib.h>

/* Macros to handle common actions */
#define HANDLE_NEWLINE(c, lineNum) { putchar(c); lineNum++; }
#define HANDLE_PUTCHAR(x) { putchar(x); }

/* State definitions remain unchanged */
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

/* Updated function signatures to pass lineNum as a parameter */
enum State handleNormal(int c, int* lineNum) {
  switch (c) {
    case '/':
      return POTENTIAL_COMMENT;
    case '"':
      HANDLE_PUTCHAR(c);
      return STRING_LIT;
    case '\'':
      HANDLE_PUTCHAR(c);
      return CHAR_LIT;
    case '\n':
      HANDLE_NEWLINE(c, *lineNum);
      return NORMAL;
    default:
      HANDLE_PUTCHAR(c);
      return NORMAL;
  }
}

enum State handlePotentialComment(int c) {
  switch (c) {
    case '*':
      HANDLE_PUTCHAR(' ');
      return IN_COMMENT;
    case '/':
      HANDLE_PUTCHAR('/');
      return POTENTIAL_COMMENT;
    case '\'':
      HANDLE_PUTCHAR('/');
      HANDLE_PUTCHAR(c);
      return CHAR_LIT;
    case '"':
      HANDLE_PUTCHAR('/');
      HANDLE_PUTCHAR(c);
      return STRING_LIT;
    default:
      HANDLE_PUTCHAR('/');
      HANDLE_PUTCHAR(c);
      return NORMAL;
  }
}

enum State handleInComment(int c, int* lineNum) {
  switch (c) {
    case '\n':
      HANDLE_NEWLINE(c, *lineNum);
      return IN_COMMENT;
    case '*':
      return STAR_COMMENT;
    default:
      return IN_COMMENT;
  }
}

enum State handleStarComment(int c, int* lineNum) {
  switch (c) {
    case '/':
      return NORMAL;
    case '*':
      return STAR_COMMENT;
    case '\n':
      HANDLE_NEWLINE(c, *lineNum);
      return IN_COMMENT;
    default:
      return IN_COMMENT;
  }
}

enum State handleStringLit(int c) {
  switch (c) {
    case '"':
      return NORMAL;
    case '\\':
      HANDLE_PUTCHAR(c);
      return STRING_ESC;
    default:
      HANDLE_PUTCHAR(c);
      return STRING_LIT;
  }
}

enum State handleCharLit(int c) {
  switch (c) {
    case '\'':
      return NORMAL;
    case '\\':
      HANDLE_PUTCHAR(c);
      return CHAR_ESC;
    default:
      HANDLE_PUTCHAR(c);
      return CHAR_LIT;
  }
}

enum State handleStringEsc(int c) {
  HANDLE_PUTCHAR(c);
  return STRING_LIT;
}

enum State handleCharEsc(int c) {
  HANDLE_PUTCHAR(c);
  return CHAR_LIT;
}

int main() {
  enum State state = NORMAL;
  int c;
  int lineNum = 1;
  int commentStartLine = 0;

  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      lineNum++;
    }

    /* Reordered case statements for variability */
    switch (state) {
      case STRING_ESC:
        state = handleStringEsc(c);
        break;
      case CHAR_ESC:
        state = handleCharEsc(c);
        break;
      case NORMAL:
        state = handleNormal(c, &lineNum);
        break;
      case POTENTIAL_COMMENT:
        commentStartLine = lineNum;
        state = handlePotentialComment(c);
        break;
      case IN_COMMENT:
        state = handleInComment(c, &lineNum);
        break;
      case STAR_COMMENT:
        state = handleStarComment(c, &lineNum);
        break;
      case STRING_LIT:
        state = handleStringLit(c);
        break;
      case CHAR_LIT:
        state = handleCharLit(c);
        break;
    }
  }

  /* Keeping exit conditions intact */
  if (state == IN_COMMENT || state == STAR_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", commentStartLine);
    return EXIT_FAILURE;
  }

  if (state == POTENTIAL_COMMENT) {
    HANDLE_PUTCHAR('/');
  }

  return EXIT_SUCCESS;
}