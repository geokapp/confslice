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

#include <iostream>
#include <string>
#include "configuration.h"
#include "global.h"
#include "lex.h"
#include "syntax.h"


using namespace std;

/**
 * @name SyntaxAnalyzer - Constructor.
 * @param gc: A pointer to a global context object.
 *
 * Creates the configuration and lectical analyzer objects.
 */
SyntaxAnalyzer::SyntaxAnalyzer(GlobalContext *gc) {
  m_gc_ptr = gc;
  m_lex = new LexAnalyzer();
  m_token_str.clear();
}

/**
 * @name SyntaxAnalyzer - Destructor.
 *
 * Deletes the configuration and lectical analyzer objects.
 */
SyntaxAnalyzer::~SyntaxAnalyzer() {
  if (m_lex)
    delete m_lex;

  m_gc_ptr = NULL;
}

/**
 * @name open - Open a file.
 * @param filename: The filename.
 *
 * Loads a new configuration file for analysis.
 *
 * @return 0 on success, 1 on error.
 */
int32_t SyntaxAnalyzer::open(const string filename) {
  return (m_lex->open(filename));
}

/**
 * @name close - Close the file.
 *
 * Close the configuration file if it is already opened.
 *
 * @return 0 on success, 1 on error.
 */
int32_t SyntaxAnalyzer::close() {
  return m_lex->close();
}

/**
 * @name analyze - Begin the syntax analysis.
 *
 * Close the configuration file if it is already opened.
 *
 * @return 0 on success, 1 on error.
 */
int32_t SyntaxAnalyzer::analyze(Configuration *conf_ptr) {
  int32_t result = begin(conf_ptr);
  close();

  return result;
}

int32_t SyntaxAnalyzer::begin(Configuration *conf_ptr) {
  int32_t result = 0;

  m_token_id = m_lex->analyze(m_token_str);
  while (m_token_id == ID_TK && !result) {
    result = key_or_entity(conf_ptr, m_token_str, 0);
    m_token_id = m_lex->analyze(m_token_str);
  }
  if (!result && m_token_id == EOF_TK) {
    return 0;
  } else {
    fprintf(stderr, "Error at line %d: %s is not allowed here. Entity or key definition was expected.\n",
	    m_lex->line(), m_token_str.c_str());
    return 1;
  }
}

int32_t SyntaxAnalyzer::key_or_entity(Configuration *conf_ptr, const string id, int32_t depth) {
  int32_t result;
  m_token_id = m_lex->analyze(m_token_str);
  if (m_token_id == COLON_TK)
    result = entity(conf_ptr, id, depth);
  else if (m_token_id == ASSIGN_TK)
    result = key(conf_ptr, id);
  else {
    fprintf(stderr, "Error at line %d: %s is not allowed here. : or = was expected.\n",
	    m_lex->line(), m_token_str.c_str());
    return 1;
  }
  // After the key or entity we should find a question mark.
  m_token_id = m_lex->analyze(m_token_str);
  if (m_token_id != QMARK_TK) {
    fprintf(stderr, "Error at line %d: %s is not allowed here. ; was expected.\n",
	    m_lex->line(), m_token_str.c_str());
    return 1;
  }
  return result;
}

int32_t SyntaxAnalyzer::entity(Configuration *conf_ptr, const string id, int32_t depth) {
  int32_t result = 0;
  Entity *my_entity;
  
  depth++;
  m_token_id = m_lex->analyze(m_token_str);
  if (m_token_id == LBRACKETS3_TK) {
    // We must find an ID here. We expect a nested entity or a key.
    m_token_id = m_lex->analyze(m_token_str);
    if (m_token_id == ID_TK) {
      // Proccess the new entity.
      my_entity = new Entity;
      my_entity->set_id(id);
      m_gc_ptr->set_current_entity(my_entity);
    } else {
      fprintf(stderr, "Error at line %d: %s is not allowed here. Entity or key definition was expected.\n",
	      m_lex->line(), m_token_str.c_str());
      return 1;
    }
    while (m_token_id == ID_TK && !result) {
      result = key_or_entity(conf_ptr, m_token_str, depth);

      if (!result) {
	// If m_current_entity does not match my_entity then a nested entity was found.
	// We should add this to the entity list of my_entity and assign m_current_entity
	// to my_entity.
	if (m_gc_ptr->current_entity() != my_entity) {
	  my_entity->add_entity(m_gc_ptr->current_entity()); 
	  m_gc_ptr->set_current_entity(my_entity);
	}
      } else {
	// Something strange happened. We must clean the pending entities and
	// return an error.
	if (m_gc_ptr->current_entity())
	  if (m_gc_ptr->current_entity() != my_entity)
	    delete m_gc_ptr->current_entity();

	m_gc_ptr->set_current_entity(NULL);
	if (my_entity)
	  delete my_entity;
	my_entity = NULL;
	return 1;
      }
      m_token_id = m_lex->analyze(m_token_str);
    }

    // We should find a } here.
    if (m_token_id == RBRACKETS3_TK) {
      // If this is a depth 1 entity we must put it into the configuration.
      if (depth == 1) {
	conf_ptr->add_entity(my_entity);
	m_gc_ptr->set_current_entity(NULL);
      }
      return result;
    } else {
      m_gc_ptr->set_current_entity(NULL);
      if (my_entity)
	delete my_entity;
      my_entity = NULL;
      fprintf(stderr, "Error at line %d: %s is not allowed here. } was expected.\n",
	      m_lex->line(), m_token_str.c_str());
      return 1;
    }
  } else {
    fprintf(stderr, "Error at line %d: %s is not allowed here. { was expected.\n",
	    m_lex->line(), m_token_str.c_str());
    // We can safely return an error. No entity was created.
    return 1;
  }
}

int32_t SyntaxAnalyzer::key(Configuration *conf_ptr, const string id) {
  int32_t result = 0;

  m_token_id = m_lex->analyze(m_token_str);
  if (m_token_id == INTEGER_TK || m_token_id == STRING_TK || m_token_id == DOUBLE_TK) {
    // Key with a single value.
    // Add it to the configuration.
    KValue *kv = new KValue;
    kv->set_id(id);
    // Set Value.
    Data data;
    if (m_token_id == INTEGER_TK)
      data.set_data(m_token_str, Data::int_t);
    else if (m_token_id == STRING_TK)
      data.set_data(m_token_str, Data::string_t);
    else
      data.set_data(m_token_str, Data::double_t);
    kv->set_value(data);
    
    if (m_gc_ptr->current_entity())
      m_gc_ptr->current_entity()->add_key(kv);
    else
      conf_ptr->add_key(kv);
    return 0;
  } else if (m_token_id == LBRACKETS1_TK) {
    // Key with array of values.
    result = key_array(conf_ptr, id);
    return result;
  } else if (m_token_id == LBRACKETS4_TK) {
    // Key with list of values.
    KList *klist = NULL;
    result = key_list(conf_ptr, id, &klist, 0);
    return result;
  } else if (m_token_id == LBRACKETS3_TK) {
    // Key with list of pairs.
    result = key_pairs(conf_ptr, id);
    return result;
  } else {
    fprintf(stderr, "Error at line %d: %s is not allowed here. Either a value, [, <, or { was expected.\n",
	    m_lex->line(), m_token_str.c_str());
    return 1;
  }
}

int32_t SyntaxAnalyzer::key_array(Configuration *conf_ptr, const string id) {
  int32_t index = 0;
  KArray *ka = new KArray;
  ka->set_id(id);
  
  do {
    m_token_id = m_lex->analyze(m_token_str); 
    
    // This should be a value.
    if (m_token_id == INTEGER_TK || m_token_id == STRING_TK || m_token_id == DOUBLE_TK) {
      // Add this value to the array.
      Data data;
      if (m_token_id == INTEGER_TK)
	data.set_data(m_token_str, Data::int_t);
      else if (m_token_id == STRING_TK)
	data.set_data(m_token_str, Data::string_t);
      else
	data.set_data(m_token_str, Data::double_t);
      
      (*ka)[index] = data;
      
    } else {
      fprintf(stderr, "Error at line %d: %s is not allowed here. A value was expected.\n",
	      m_lex->line(), m_token_str.c_str());
      return 1;
    }
    m_token_id = m_lex->analyze(m_token_str);
    index++;
  } while (m_token_id == COMMA_TK);
  
  // The token should now be a ].
  if (m_token_id == RBRACKETS1_TK) { 
    if (m_gc_ptr->current_entity())
      m_gc_ptr->current_entity()->add_key(ka);
    else
      conf_ptr->add_key(ka);
    return 0;
  } else { 
    // Delete the array object.
    delete ka;
    fprintf(stderr, "Error at line %d: %s is not allowed here. ] was expected.\n",
	    m_lex->line(), m_token_str.c_str());
    return 1;
  }
}

int32_t SyntaxAnalyzer::key_list(Configuration *conf_ptr, const string id, KList **klist, int32_t depth) {
  int32_t result = 0;

  depth++;
  
  if (!(*klist))
    *klist = new KList;
  (*klist)->set_id(id);
  
  do {
    m_token_id = m_lex->analyze(m_token_str);
    
    // This should be a value or a < for a sublist.
    if (m_token_id == INTEGER_TK || m_token_id == STRING_TK || m_token_id == DOUBLE_TK) {
      // add this to the list.
      Data data;
      if (m_token_id == INTEGER_TK)
	data.set_data(m_token_str, Data::int_t);
      else if (m_token_id == STRING_TK)
	data.set_data(m_token_str, Data::string_t);
      else
	data.set_data(m_token_str, Data::double_t);

      (*klist)->insert_data(data);
    } else if (m_token_id == LBRACKETS4_TK) {
      KList *klist_nested = NULL;
      result = key_list(conf_ptr, id, &klist_nested, depth);
      if (!result) {
	// Add the sub list object.
	if (klist_nested) {
	  (*klist)->insert_klist(*klist_nested);
	  // Delete the sublist object. We do not needed any more.
	  delete klist_nested;
	}
      } else {
	if (klist_nested)
	  delete klist_nested;
	if (*klist)
	  delete *klist;
	*klist = klist_nested = NULL;
	return 1;
      }
    } else {
      fprintf(stderr, "Error at line %d: %s is not allowed here. A value or a < was expected.\n",
	      m_lex->line(), m_token_str.c_str());
      if (*klist)
	delete (*klist);
      *klist = NULL;
      return 1;
    }
    m_token_id = m_lex->analyze(m_token_str);    
  } while(m_token_id == COMMA_TK);

  // The token should now be a >.
  if (m_token_id == RBRACKETS4_TK) {
    // If this is the top list add the klist object to the configuration.
    if (depth == 1)
      conf_ptr->add_key(*klist);
    return 0;
  } else {      
    // Delete the list object.
    if (*klist)
      delete *klist;
    *klist = NULL;
    fprintf(stderr, "Error at line %d: %s is not allowed here. > was expected.\n",
	    m_lex->line(), m_token_str.c_str());
    return 1;
  }
}

int32_t SyntaxAnalyzer::key_pairs(Configuration *conf_ptr, const string id) {
  KPairs *kp_ptr = new KPairs;
  kp_ptr->set_id(id);

  do {
    string pair_id;
    
    // This should be an ID.
    m_token_id = m_lex->analyze(m_token_str);
    if (m_token_id == ID_TK) {
      pair_id = m_token_id;
      m_token_id = m_lex->analyze(m_token_str);
      // This should be the =.
      if (m_token_id == ASSIGN_TK) {
	m_token_id = m_lex->analyze(m_token_str);
	//this should be a value.
	if (m_token_id == INTEGER_TK || m_token_id == STRING_TK || m_token_id == DOUBLE_TK) {
	  // Put this pair to the key.
	  Data data;
	  if (m_token_id == INTEGER_TK)
	    data.set_data(m_token_str, Data::int_t);
	  else if (m_token_id == STRING_TK)
	    data.set_data(m_token_str, Data::string_t);
	  else
	    data.set_data(m_token_str, Data::double_t);
	} else {
	  fprintf(stderr, "Error at line %d: %s is not allowed here. A value was expected.\n",
		  m_lex->line(), m_token_str.c_str());
	  return 1;
	}
      } else {
	fprintf(stderr, "Error at line %d: %s is not allowed here. =  was expected.\n",
	      m_lex->line(), m_token_str.c_str());
	return 1;
      }
    } else {
      fprintf(stderr, "Error at line %d: %s is not allowed here. Am ID was expected.\n",
	      m_lex->line(), m_token_str.c_str());
      return 1;
    }
    m_token_id = m_lex->analyze(m_token_str);
  } while (m_token_id == QMARK_TK);

  // The token should now be a }.
  if (m_token_id == RBRACKETS3_TK) {
    conf_ptr->add_key(kp_ptr);
    return 0;
  } else {
    // Delete the pairs object.
    delete kp_ptr;
    kp_ptr = NULL;
    fprintf(stderr, "Error at line %d: %s is not allowed here. } was expected.\n",
	    m_lex->line(), m_token_str.c_str());
    
    return 1;
  }
}
