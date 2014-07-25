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

#ifndef LEX_H
#define LEX_H

#include <stdint.h>
#include <string>
#include <list>

// Symbols
#define WHITE        0  // \n \t space
#define LETTER       1  // Letter
#define DIGIT        2  // Digit
#define EOL_TK       3  // EOL
#define EOF_TK       4  // EOF
#define SLASH        5  // /
#define DITTO        6  // "
#define BACKSLASH    7  // Backslash
#define MINUS        8  // -
#define UNDERSCORE   9  // _
#define PERIOD       10 // .
#define OTHER        11 // other symbols

// Tokens
#define ID_TK        40 // ID name, eg. variable
#define INTEGER_TK   41 // Integer number
#define STRING_TK    42 // String constant
#define DOUBLE_TK    43 // Float number

#define ASSIGN_TK       50 // =
#define LBRACKETS1_TK   51 // [
#define RBRACKETS1_TK   52 // ]
#define LBRACKETS2_TK   53 // (
#define RBRACKETS2_TK   54 // )
#define LBRACKETS3_TK   55 // {
#define RBRACKETS3_TK   56 // }
#define LBRACKETS4_TK   57 // <
#define RBRACKETS4_TK   58 // >
#define QMARK_TK        59 // ;
#define COLON_TK        60 // :
#define COMMA_TK        61 // ,
// Lectical analyzer states
#define ST0 0
#define ST1 1
#define ST2 2
#define ST3 3
#define ST4 4
#define ST5 5
#define ST6 6
#define ST7 7
#define ST8 8

// Outcomes
#define OK  100
#define BK  101
#define ERR 102

#define WSIZE          100
#define SSIZE           12
#define STATESIZE        9
#define DSIZE           12

/**
 * LexAnalyzer - Analyzer object.
 *
 * This class defines the analyzer object. You must first call the open method in order
 * to load a configuration file. Then,tTo analyze the next token use the analyze method.
 */
class LexAnalyzer {
 private:
  uint32_t m_line;
  FILE *m_file;
  
 public:
  LexAnalyzer();
  ~LexAnalyzer();
  
  uint32_t line();
  
  int32_t open(const std::string file);
  int32_t close();
  int32_t analyze(std::string &word);
  
 private:
  int32_t get_id(const char c);
};

#endif

