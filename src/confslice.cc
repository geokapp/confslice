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
#include <iostream>
#include <string>
#include "confslice.h"
#include "configuration.h"
#include "global.h"
#include "syntax.h"

using namespace std;

/**
 * ----------------------------------------------------------
 * Class: ConfSlice
 * ----------------------------------------------------------
 */

/**
 * ConfSlice - Constructor.
 *
 * Creates the confslice object.
 */
ConfSlice::ConfSlice() {
  m_gc = new GlobalContext;
  m_syntax = new SyntaxAnalyzer(m_gc);
  m_configuration = new Configuration;
}

/**
 * ConfSlice - Destructor.
 *
 * Deletes the the ConfSlice object and frees memory resources.
 */
ConfSlice::~ConfSlice() {
  if (m_gc)
    delete m_gc;
  if (m_syntax)
    delete m_syntax;
  if (m_configuration)
    delete m_configuration; 
}

/**
 * analyze - Begin the configuration analysis.
 *
 * Analyzes a configuration file.
 */
int32_t ConfSlice::analyze(string filename) {
  int32_t result;
  result = m_syntax->open(filename);  
  if (!result)
    result = m_syntax->analyze(m_configuration);

  return result;
}

Configuration *ConfSlice::configuration() {
  return m_configuration;
}

