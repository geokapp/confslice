// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Confslice - A simple configuration file parser.
 *
 * Copyright (C) 2014 Giorgos Kappes <geokapp@gmail.com>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include "lex.h"

using namespace std;

/**
 * @name LexAnalyzer - Constructor.
 *
 * This method initializes the LexAnalyzer object.
 */
LexAnalyzer::LexAnalyzer() {
  m_line = 1;
  m_file = NULL;
}

/**
 * @name ~LexAnalyzer - Destructor.
 *
 * This method cleans the LexAnalyzer object. It closes the input file if
 * it is still open.
 */
LexAnalyzer::~LexAnalyzer() {
  if (m_file) {
    fclose(m_file);
    m_file = NULL;
  }
  m_line = 0;
}

/**
 * @name line - Line number.
 *
 * Return the current line number.
 *
 * @return The line number.
 */
uint32_t LexAnalyzer::line() {
  return m_line;
}

/**
 * @name open - Open the input file
 * @param file: The filename.
 *
 * This method loads a configuration file to the analyzer.
 *
 * @return 0 on success, 1 on error.
 */
int32_t LexAnalyzer::open(const string file) {
  if (!(m_file = fopen(file.c_str(), "r"))) {
    fprintf(stderr,"File \"%s\" not found. \n",file.c_str());
    m_file = NULL;
    return -1;
  }
  return 0;
}

/**
 * @name close - Close the input file.
 *
 * This method closes the input file.
 *
 * @return 0 on success, 1 on error.
 */
int32_t LexAnalyzer::close() {
  int32_t status = -1;
  if (m_file) {
    status = fclose(m_file);
    m_file = NULL;
  }
  return status;
}

/**
 * @name analyze - Lexixal analyzer.
 * @param word: A reference to the identified token.
 
 * This method parses the configuration file and returns the next identified
 * token and its ID.
 *
 * @return Token ID or -1 on error.
 */
int32_t LexAnalyzer::analyze(string &word) {
  // If the input file has not been opened immediately return.
  if (!m_file)
    return -1;
  
  // The state table. 
  const int32_t STATES[STATESIZE][SSIZE] = {
    //ws,  lt   dg  EOL  EOF    /    "    \    -    _    .    +    o
    {ST0, ST1, ST2, ST0,  OK, ST3, ST5,  OK, ST8,  OK, ST7, ST8,  OK}, // 0
    { BK, ST1, ST1,  BK,  BK,  BK,  BK,  BK, ST1, ST1, ST1, ST1,  BK}, // 1 
    { BK,  BK, ST2,  BK,  BK,  BK,  BK,  BK,  BK,  BK, ST7,  BK,  BK}, // 2
    { BK,  BK,  BK,  BK,  BK, ST4,  BK,  BK,  BK,  BK,  BK,  BK,  BK}, // 3
    {ST4, ST4, ST4, ST0, ERR, ST4, ST4, ST4, ST4, ST4, ST4, ST4, ST4}, // 4
    {ST5, ST5, ST5, ERR, ERR, ST5,  OK, ST6, ST5, ST5, ST5, ST5, ST5}, // 5
    {ST5, ST5, ST5, ERR, ERR, ST5, ST5, ST6, ST5, ST5, ST5, ST5, ST5}, // 6
    { BK, BK,  ST7,  BK,  BK,  BK,  BK,  BK,  BK,  BK,  BK,  BK,  BK}, // 7
    { BK, BK,  ST2,  BK,  BK,  BK,  BK,  BK,  BK,  BK,  BK,  BK,  BK}, // 8
  };
 
  // The defined words.
  const char defined_words[DSIZE] = {'=', '[', ']', '(', ')', '{', '}', '<', '>', ';', ':', ','};
  
  int32_t id, i;
  char c;
  string current;
  int32_t state = ST0;

  while (state != OK && state != ERR && state!= BK) {
    if (state == ST0)
      current.clear();
    
    c = fgetc(m_file);
    if (c == '\n') {
      m_line++;
      id = EOL_TK;
    } else {
      id = get_id(c);
    }
    if (id == ERR) {
	fprintf(stderr, "Error at Line %d: '%c': Not allowed character.\n",m_line,c);
	return -1;
    }
    state = STATES[state][id];
    if (state != BK && state != ERR && id != WHITE && state != ST4) 
      current += c;
  }
  
  if (state == ERR) {
    fprintf(stderr, "Error at line %d: end of line or file is not allowed here.\n",m_line);
    return -1;
  }

  if (state == BK) ungetc(c, m_file);

  for (i = 0; i < DSIZE; i++) {
    if (current.at(0) == defined_words[i]) { 
      word = current;
      return 50 + i;
    }
  }
  word = current;
  if (isalpha(current.at(0))) {
    return ID_TK;
  } else if (current.find('"') != string::npos) {
    return STRING_TK;
  } else if (isdigit(current.at(0)) || (current.at(0) == '-' && isdigit(current.at(1)))) {
    if (current.find('.') != string::npos)
      return DOUBLE_TK;
    else
      return INTEGER_TK;
  } else {
    return id;
  }   
}

/**
 * @name get_id - Returns the ID of a symbol.
 * @param c: A character.
 *
 * This method returns the ID of a input symbol.
 *
 * @return Symbol ID.
 */
int32_t LexAnalyzer::get_id(const char c) {
  char symbols[SSIZE]= {'x','x','x','\n', EOF, '/', '\"', '\\', '-', '_', 'x'};
  
  for (int32_t i = 3; i < SSIZE; i++) {
    if (c == symbols[i]) 
      return i;
  }
  if (isalpha(c)) return 1;
  else if (isdigit(c)) return 2;
  else if (isspace(c)) return 0;
  else return SSIZE - 1;
}
