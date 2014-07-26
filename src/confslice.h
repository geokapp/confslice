// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Confslice - Configuration file parser. 
 *
 * Copyright (C) 2014 Giorgos Kappes <geokapp@gmail.com>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */

#ifndef CONFSLICE_H
#define CONFSLICE_H

#include <string>
#include "configuration.h"
#include "global.h"
#include "syntax.h"

/**
 * @name ConfSlice - The configuration slicer object.
 *
 * This is the main object of confslice. It contains a global context object,
 * the syntax analyzer, as well as a pointer to a congiguration. You can use
 * this object to load and analyze configuration files. You shoud 
 * To load and analyze a configuration file use the "analyze()" method. To
 * get the generated configuration use the "configuration()" method.
 */
class ConfSlice {  
 private:
  GlobalContext *m_gc;
  SyntaxAnalyzer *m_syntax;
  Configuration *m_configuration;
    
 public:
  ConfSlice();
  ~ConfSlice();
  int32_t analyze(const std::string filename);
  Configuration *configuration();
};

#endif

