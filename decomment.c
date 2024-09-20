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
  enum State state;
  state = NORMAL;
  switch (c) {
    case '/':
      state = POTENTIAL_COMMENT;
      break;
    case '"':
      putchar(c);
      state = STRING_LIT;
      break;
    case '\'':
      putchar(c);
      state = CHAR_LIT;
      break;
    case '\n':
      putchar(c);
      state = NORMAL;
      break;
    default:
      putchar(c);
      break;
  }
  return state;
}

enum State handlePotentialComment(int c) {
  enum State state;
  state = POTENTIAL_COMMENT;
  switch (c) {
    case '*':
      putchar(' ');
      state = IN_COMMENT;
      break;
    case '/':
      putchar('/');
      state = POTENTIAL_COMMENT;
      break;
    case '\'':
      putchar('/');
      putchar(c);
      state = CHAR_LIT;
      break;
    case '"':
      putchar('/');
      putchar(c);
      state = STRING_LIT;
      break;
    default:
      putchar('/');
      putchar(c);
      state = NORMAL;
      break;
  }
  return state;
}

enum State handleInComment(int c) {
  enum State state;
  state = IN_COMMENT;
  if (c == '\n') {
    putchar('\n');
  } else if (c == '*') {
    state = STAR_COMMENT;
  }
  return state; 
}

enum State handleStarComment(int c) {
  enum State state;
  switch (c) {
    case '/':
      state = NORMAL;
      break;
    case '*':
      state = STAR_COMMENT;
      break;
    case '\n':
      putchar('\n');
      state = IN_COMMENT;
      break;
    default:
      state = IN_COMMENT;
      break;
  }
  return state;
}

enum State handleStringLit(int c) {
  enum State state;
  state = STRING_LIT;
  if (c == '"') {
    state = NORMAL;
  } else if (c == '\\') {
    state = STRING_ESC;
  }
  putchar(c);
  return state;
}

enum State handleCharLit(int c) {
  enum State state;
  state = CHAR_LIT;
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
        commentStartLine = lineNum;
        state = handlePotentialComment(c);
        break;
      case IN_COMMENT:
        state = handleInComment(c);
        break;
      case STAR_COMMENT:
        state = handleStarComment(c);
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