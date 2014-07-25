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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "configuration.h"

/**
 * GlobalContext - The global context object.
 *
 * This class defiens the global context object that contain global
 * variables between other objects that are able to see the same
 * instance of GlobalContext.
 */
class GlobalContext {
 private:
  Entity *m_current_entity;

 public:
  GlobalContext();
  ~GlobalContext();

  void set_current_entity(Entity *entity);
  Entity *current_entity();
};

#endif

