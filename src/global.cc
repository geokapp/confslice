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
#include "configuration.h"
#include "global.h"

using namespace std;

/**
 * ----------------------------------------------------------
 * Class: GlobalContext
 * ----------------------------------------------------------
 */

/**
 * GlobalContext - Constructor.
 *
 * Creates a new GlobalContext object.
 */
GlobalContext::GlobalContext() {
  m_current_entity = NULL;
}

/**
 * GlobalContext - Destructor.
 *
 * Destroys the GlobalContext object.
 */
GlobalContext::~GlobalContext() {
  if (m_current_entity)
    delete m_current_entity;
  m_current_entity = NULL;
}

/**
 * set_current_entity - Set the current entity.
 * @entity: The entity object.
 */
void GlobalContext::set_current_entity(Entity *entity) {
  m_current_entity = entity;
}

/**
 * current_entity - Return the current entity.
 */
Entity *GlobalContext::current_entity() {
  return m_current_entity;
}
