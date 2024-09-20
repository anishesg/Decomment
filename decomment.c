#include <stdio.h>
#include <stdlib.h>

/* Struct to store the current state, line number, and comment start line */
struct DecommentState {
  enum State currentState;
  int lineNum;
  int commentStartLine;
};

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
 *   - state: a pointer to the DecommentState struct storing the current state and line information.
 * Returns:
 *   The next state after processing the character.
 */
enum State handleNormal(int c, struct DecommentState *ds) {
  switch (c) {
    case '/':
      ds->currentState = POTENTIAL_COMMENT;
      break;
    case '"':
      putchar(c);
      ds->currentState = STRING_LIT;
      break;
    case '\'':
      putchar(c);
      ds->currentState = CHAR_LIT;
      break;
    case '\n':
      putchar(c);
      ds->currentState = NORMAL;
      ds->lineNum++;
      break;
    default:
      putchar(c);
      break;
  }
  return ds->currentState;
}

/* 
 * handleInComment: Processes characters when inside a multi-line comment.
 * Looks for the end of the comment ('*') or newlines to maintain line numbers.
 * Parameters:
 *   - c: the current character being processed.
 *   - state: a pointer to the DecommentState struct storing the current state and line information.
 * Returns:
 *   The next state after processing the character (IN_COMMENT or STAR_COMMENT).
 */
enum State handleInComment(int c, struct DecommentState *ds) {
  if (c == '\n') {
    putchar('\n');
    ds->lineNum++;
  } else if (c == '*') {
    ds->currentState = STAR_COMMENT;
  }
  return ds->currentState;
}

/* 
 * handlePotentialComment: Processes characters after encountering a '/'.
 * Determines if the character sequence is a comment or something else.
 * Parameters:
 *   - c: the current character being processed.
 *   - state: a pointer to the DecommentState struct storing the current state and line information.
 * Returns:
 *   The next state after processing the character.
 */
enum State handlePotentialComment(int c, struct DecommentState *ds) {
  switch (c) {
    case '*':
      putchar(' ');
      ds->currentState = IN_COMMENT;
      ds->commentStartLine = ds->lineNum;
      break;
    case '/':
      putchar('/');
      ds->currentState = POTENTIAL_COMMENT;
      break;
    case '\'':
      putchar('/');
      putchar(c);
      ds->currentState = CHAR_LIT;
      break;
    case '"':
      putchar('/');
      putchar(c);
      ds->currentState = STRING_LIT;
      break;
    default:
      putchar('/');
      putchar(c);
      ds->currentState = NORMAL;
      break;
  }
  return ds->currentState;
}

/* 
 * handleStringLit: Processes characters inside a string literal.
 * Detects the end of the string or escape sequences.
 * Parameters:
 *   - c: the current character being processed.
 *   - state: a pointer to the DecommentState struct storing the current state and line information.
 * Returns:
 *   The next state after processing the character (STRING_LIT, STRING_ESC, or NORMAL).
 */
enum State handleStringLit(int c, struct DecommentState *ds) {
  if (c == '"') {
    ds->currentState = NORMAL;
  } else if (c == '\\') {
    ds->currentState = STRING_ESC;
  }
  putchar(c);
  return ds->currentState;
}

/* 
 * handleStarComment: Processes characters after encountering a '*' in a comment.
 * Determines if the comment ends ('/'), continues ('*'), or has a newline.
 * Parameters:
 *   - c: the current character being processed.
 *   - state: a pointer to the DecommentState struct storing the current state and line information.
 * Returns:
 *   The next state after processing the character (STAR_COMMENT, IN_COMMENT, or NORMAL).
 */
enum State handleStarComment(int c, struct DecommentState *ds) {
  switch (c) {
    case '/':
      ds->currentState = NORMAL;
      break;
    case '*':
      ds->currentState = STAR_COMMENT;
      break;
    case '\n':
      putchar('\n');
      ds->lineNum++;
      ds->currentState = IN_COMMENT;
      break;
    default:
      ds->currentState = IN_COMMENT;
      break;
  }
  return ds->currentState;
}

/* 
 * handleCharLit: Processes characters inside a character literal.
 * Detects the end of the character literal or escape sequences.
 * Parameters:
 *   - c: the current character being processed.
 *   - state: a pointer to the DecommentState struct storing the current state and line information.
 * Returns:
 *   The next state after processing the character (CHAR_LIT, CHAR_ESC, or NORMAL).
 */
enum State handleCharLit(int c, struct DecommentState *ds) {
  if (c == '\'') {
    ds->currentState = NORMAL;
  } else if (c == '\\') {
    ds->currentState = CHAR_ESC;
  }
  putchar(c);
  return ds->currentState;
}

/* 
 * handleStringEsc: Processes escape sequences inside a string literal.
 * Puts the escaped character and returns to STRING_LIT state.
 * Parameters:
 *   - c: the current character being processed.
 *   - state: a pointer to the DecommentState struct storing the current state and line information.
 * Returns:
 *   STRING_LIT after processing the escape character.
 */
enum State handleStringEsc(int c, struct DecommentState *ds) {
  putchar(c);
  ds->currentState = STRING_LIT;
  return ds->currentState;
}

/* 
 * handleCharEsc: Processes escape sequences inside a character literal.
 * Puts the escaped character and returns to CHAR_LIT state.
 * Parameters:
 *   - c: the current character being processed.
 *   - state: a pointer to the DecommentState struct storing the current state and line information.
 * Returns:
 *   CHAR_LIT after processing the escape character.
 */
enum State handleCharEsc(int c, struct DecommentState *ds) {
  putchar(c);
  ds->currentState = CHAR_LIT;
  return ds->currentState;
}

/* 
 * main: Entry point of the decomment program. Reads characters from standard input, 
 * processes them according to the current state (using a DFA), and writes to standard output.
 * If an unterminated comment is detected, prints an error to standard error.
 * Returns EXIT_SUCCESS if no unterminated comments are found, otherwise EXIT_FAILURE.
 */
int main() {
  struct DecommentState ds;
  ds.currentState = NORMAL;
  ds.lineNum = 1;
  ds.commentStartLine = 0;

  int c;

  /* Main loop to read characters until EOF */
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      ds.lineNum++;
    }

    /* Switch statement handles state transitions */
    switch (ds.currentState) {
      case NORMAL:
        ds.currentState = handleNormal(c, &ds);
        break;
      case POTENTIAL_COMMENT:
        ds.currentState = handlePotentialComment(c, &ds);
        break;
      case IN_COMMENT:
        ds.currentState = handleInComment(c, &ds);
        break;
      case STAR_COMMENT:
        ds.currentState = handleStarComment(c, &ds);
        break;
      case STRING_LIT:
        ds.currentState = handleStringLit(c, &ds);
        break;
      case CHAR_LIT:
        ds.currentState = handleCharLit(c, &ds);
        break;
      case STRING_ESC:
        ds.currentState = handleStringEsc(c, &ds);
        break;
      case CHAR_ESC:
        ds.currentState = handleCharEsc(c, &ds);
        break;
    }
  }

  /* Handle unterminated comment error */
  if (ds.currentState == IN_COMMENT || ds.currentState == STAR_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", ds.commentStartLine);
    return EXIT_FAILURE;
  }

  /* Handle incomplete potential comment (single '/') */
  if (ds.currentState == POTENTIAL_COMMENT) {
    putchar('/');
  }

  return EXIT_SUCCESS;
}