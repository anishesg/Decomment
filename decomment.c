#include <stdio.h>
#include <stdlib.h>

/* Enum to define all possible states in the de-commenting process */
enum State {
  NORMAL,              /* Default state: no comment or special character handling */
  POTENTIAL_COMMENT,   /* State after encountering a '/' character */
  IN_COMMENT,          /* State inside a multi-line comment */
  STAR_COMMENT,        /* State after encountering a '*' in a comment */
  STRING_LIT,          /* State inside a string literal ("...") */
  CHAR_LIT,            /* State inside a character literal ('...') */
  STRING_ESC,          /* State after encountering a '\' in a string literal */
  CHAR_ESC             /* State after encountering a '\' in a character literal */
};

/* 
 * handleNormal: Processes characters when in the NORMAL state. 
 * Handles potential comment beginnings ('/'), string literals ('"'),
 * and character literals ('\'').
 * Parameters:
 *   - c: the current character being processed.
 * Returns:
 *   The next state after processing the character.
 */
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

/* 
 * handleInComment: Processes characters when inside a multi-line comment.
 * Looks for the end of the comment ('*') or newlines to maintain line numbers.
 * Parameters:
 *   - c: the current character being processed.
 * Returns:
 *   The next state after processing the character (IN_COMMENT or STAR_COMMENT).
 */
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

/* 
 * handlePotentialComment: Processes characters after encountering a '/'.
 * Determines if the character sequence is a comment */ 
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

/* 
 * handleStringLit: Processes characters inside a string literal.
 * Detects the end of the string or escape sequences.
 * Parameters:
 *   - c: the current character being processed.
 * Returns:
 *   The next state after processing the character (STRING_LIT, STRING_ESC, or NORMAL).
 */
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

/* 
 * handleStarComment: Processes characters after encountering a '*' in a comment.
 * Determines if the comment ends ('/'), continues ('*'), or has a newline.
 * Parameters:
 *   - c: the current character being processed.
 * Returns:
 *   The next state after processing the character (STAR_COMMENT, IN_COMMENT, or NORMAL).
 */
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

/* 
 * handleCharLit: Processes characters inside a character literal.
 * Detects the end of the character literal or escape sequences.
 * Parameters:
 *   - c: the current character being processed.
 * Returns:
 *   The next state after processing the character (CHAR_LIT, CHAR_ESC, or NORMAL).
 */
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

/* 
 * handleStringEsc: Processes escape sequences inside a string literal.
 * Puts the escaped character and returns to STRING_LIT state.
 * Parameters:
 *   - c: the current character being processed.
 * Returns:
 *   STRING_LIT after processing the escape character.
 */
enum State handleStringEsc(int c) {
  putchar(c);
  return STRING_LIT;
}

/* 
 * handleCharEsc: Processes escape sequences inside a character literal.
 * Puts the escaped character and returns to CHAR_LIT state.
 * Parameters:
 *   - c: the current character being processed.
 * Returns:
 *   CHAR_LIT after processing the escape character.
 */
enum State handleCharEsc(int c) {
  putchar(c);
  return CHAR_LIT;
}

/* 
 * main: Entry point of the decomment program. Reads characters from standard input, 
 * processes them according to the current state (using a DFA), and writes to standard output.
 * If an unterminated comment is detected, prints an error to standard error.
 * Returns EXIT_SUCCESS if no unterminated comments are found, otherwise EXIT_FAILURE.
 */
int main() {
  enum State state;
  int c;
  int lineNum = 1;           /* Current line number in the input */
  int commentStartLine = 0;   /* Line number where a comment starts */
  state = NORMAL;

  /* Main loop to read characters until EOF */
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      lineNum++;
    }

    /* Switch statement handles state transitions */
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

  /* Handle unterminated comment error */
  if (state == IN_COMMENT || state == STAR_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", commentStartLine);
    return EXIT_FAILURE;
  }

  /* Handle incomplete potential comment (single '/') */
  if (state == POTENTIAL_COMMENT) {
    putchar('/');
  }

  return EXIT_SUCCESS;
}