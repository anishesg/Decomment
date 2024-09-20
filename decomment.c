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

/* Helper function to handle printing newlines and updating line number */
void handleNewLine(int *lineNum) {
  putchar('\n');
  (*lineNum)++;
}

/* Handles all regular character output */
void outputChar(int c) {
  putchar(c);
}

/* Handle comment detection state transition */
enum State checkPotentialComment(int c) {
  return (c == '*') ? IN_COMMENT : NORMAL;
}

enum State handleNormal(int c) {
  if (c == '/') return POTENTIAL_COMMENT;
  if (c == '\'') {
    outputChar(c);
    return CHAR_LIT;
  }
  if (c == '"') {
    outputChar(c);
    return STRING_LIT;
  }
  if (c == '\n') {
    return NORMAL;
  }
  outputChar(c);
  return NORMAL;
}

enum State handlePotentialComment(int c) {
  if (c == '*') {
    outputChar(' ');
    return IN_COMMENT;
  }
  if (c == '/') {
    outputChar('/');
    return POTENTIAL_COMMENT;
  }
  outputChar('/');
  outputChar(c);
  return (c == '\'') ? CHAR_LIT : (c == '"') ? STRING_LIT : NORMAL;
}

enum State handleInComment(int c) {
  if (c == '*') return STAR_COMMENT;
  if (c == '\n') handleNewLine(&lineNum);
  return IN_COMMENT;
}

enum State handleStarComment(int c) {
  if (c == '/') return NORMAL;
  if (c == '*') return STAR_COMMENT;
  if (c == '\n') handleNewLine(&lineNum);
  return IN_COMMENT;
}

enum State handleStringLit(int c) {
  outputChar(c);
  if (c == '"') return NORMAL;
  if (c == '\\') return STRING_ESC;
  return STRING_LIT;
}

enum State handleCharLit(int c) {
  outputChar(c);
  if (c == '\'') return NORMAL;
  if (c == '\\') return CHAR_ESC;
  return CHAR_LIT;
}

enum State handleStringEsc(int c) {
  outputChar(c);
  return STRING_LIT;
}

enum State handleCharEsc(int c) {
  outputChar(c);
  return CHAR_LIT;
}

/* Function to read input and transition states */
enum State processInput(enum State state, int c, int *lineNum, int *commentStartLine) {
  switch (state) {
    case NORMAL:
      return handleNormal(c);
    case POTENTIAL_COMMENT:
      *commentStartLine = *lineNum;
      return handlePotentialComment(c);
    case IN_COMMENT:
      return handleInComment(c);
    case STAR_COMMENT:
      return handleStarComment(c);
    case STRING_LIT:
      return handleStringLit(c);
    case CHAR_LIT:
      return handleCharLit(c);
    case STRING_ESC:
      return handleStringEsc(c);
    case CHAR_ESC:
      return handleCharEsc(c);
  }
  return NORMAL; // Default return to avoid compiler warning
}

int main() {
  int c;
  int lineNum = 1;
  int commentStartLine = 0;
  enum State state = NORMAL;

  while ((c = getchar()) != EOF) {
    if (c == '\n') handleNewLine(&lineNum);
    state = processInput(state, c, &lineNum, &commentStartLine);
  }

  if (state == IN_COMMENT || state == STAR_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", commentStartLine);
    return EXIT_FAILURE;
  }

  if (state == POTENTIAL_COMMENT) {
    outputChar('/');
  }
  
  return EXIT_SUCCESS;
}