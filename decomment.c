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

enum State handleNormal(int c) {
  enum State state = NORMAL;
  if (c == '/') {
    state = POTENTIAL_COMMENT;
  } else if (c == '\'') {
    putchar(c);
    state = CHAR_LIT;
  } else if (c == '"') {
    putchar(c);
    state = STRING_LIT;
  } else if (c == '\n') { 
    putchar(c);
    state = NORMAL;
  } else {
    putchar(c);
  }
  return state;
}

enum State handlePotentialComment(int c, int *commentStartLine, int lineNum) {
  enum State state = POTENTIAL_COMMENT;
  if (c == '*') {
    putchar(' ');
    *commentStartLine = lineNum;
    state = IN_COMMENT;
  } 
  else if (c == '/') {
    putchar('/');
    state = POTENTIAL_COMMENT;
  }
  else if (c == '\''){
    putchar('/');
    putchar(c);
    state = CHAR_LIT;
  }
  else if (c == '"'){
    putchar('/');
    putchar(c);
    state = STRING_LIT;
  }
  else {
    putchar('/');
    putchar(c);
    state = NORMAL;
  }
  return state;
}

enum State handleInComment(int c, int *lineNum) {
  enum State state = IN_COMMENT;
  if (c == '\n') {
    putchar('\n');
    (*lineNum)++;
  } else if (c == '*') {
    state = STAR_COMMENT;
  }
  return state; 
}

enum State handleStarComment(int c, int *lineNum) {
  enum State state = STAR_COMMENT;
  if (c == '/') {
    state = NORMAL;
  } else if (c == '*') {
    state = STAR_COMMENT;
  } else if (c == '\n') {
    putchar('\n');
    (*lineNum)++;
    state = IN_COMMENT;
  } else {
    state = IN_COMMENT;
  }
  return state;
}

enum State handleStringLit(int c) {
  enum State state = STRING_LIT;
  if (c == '"') {
    state = NORMAL;
  } else if (c == '\\') {
    state = STRING_ESC;
  }
  putchar(c);
  return state;
}

enum State handleCharLit(int c) {
  enum State state = CHAR_LIT;
  if (c == '\'') {
    state = NORMAL;
  } else if (c == '\\') {
    state = CHAR_ESC;
  }
  putchar(c);
  return state;
}

enum State handleStringEsc(int c) {
  putchar(c);
  return STRING_LIT;
}

enum State handleCharEsc(int c) {
  putchar(c);
  return CHAR_LIT;
}

int main() {
  enum State state;
  int c;
  int lineNum = 1;
  int commentStartLine = 0;

  state = NORMAL;

  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      lineNum++;
    }

    switch (state) {
      case NORMAL:
        state = handleNormal(c);
        break;
      case POTENTIAL_COMMENT:
        state = handlePotentialComment(c, &commentStartLine, lineNum);
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
      case STRING_ESC:
        state = handleStringEsc(c);
        break;
      case CHAR_ESC:
        state = handleCharEsc(c);
        break;
    }
  }

  if (state == IN_COMMENT || state == STAR_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", commentStartLine);
    return EXIT_FAILURE;
  }

  if (state == POTENTIAL_COMMENT) {
    putchar('/');
  }

  return EXIT_SUCCESS;
}