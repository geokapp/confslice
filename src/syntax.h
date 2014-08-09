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
 * Foundation.  See file LICENSE.
 * 
 */

#ifndef SYNTAX_H
#define SYNTAX_H

#include <stdio.h>
#include <string>
#include "configuration.h"
#include "global.h"
#include "lex.h"

/**
 * @name SyntaxAnalyzer - The syntax analyzer object.
 *
 * This class defines the syntax analyzer object. It is used to parse and
 * syntactically analyze configuration files. It builds a configuration object
 * that contains all the entities and keys that exist  in the configuration
 * file.
 * You should first call the "open()" method to load a configuration file and
 * then the "analyze()" method to analyze it and build the configuration.
 */
class SyntaxAnalyzer {  
 private:
  LexAnalyzer *m_lex;
  GlobalContext *m_gc_ptr;
  int32_t m_token_id;
  std::string m_token_str;
    
 public:
  SyntaxAnalyzer(GlobalContext *gc);
  ~SyntaxAnalyzer();
  int32_t open(const std::string filename);
  int32_t close();
  int32_t analyze(Configuration *conf_ptr);
  
 private:
  int32_t begin(Configuration *conf_ptr);
  int32_t key_or_entity(Configuration *conf_ptr, const std::string id, int32_t depth);
  int32_t entity(Configuration *conf_ptr, const std::string id, int32_t depth);
  int32_t key(Configuration *conf_ptr, const std::string id);
  int32_t key_array(Configuration *conf_ptr, const std::string id);
  int32_t key_list(Configuration *conf_ptr, const std::string id, KList **klist, int32_t depth);
  int32_t key_pairs(Configuration *conf_ptr, const std::string id);  
};

#endif

