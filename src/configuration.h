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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdint.h>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <utility>

/**
 * @name Data - The data object.
 *
 * This class defines the data object. it uses a string buffer to store
 * the data. The data can be either a string, an itneger, or a double.
 * The get method uses a template type to return the correct type.
 */
class Data {
 public:
  enum Type 
  {
    int_t,
    double_t,
    string_t,
    none_t
  };

 private:
  Type m_type;
  std::string m_data;

 public: 
  explicit Data(const Data::Type type);
  explicit Data();
  ~Data();
  
  Data::Type type();
  void set_data(const std::string data, const Data::Type type);
  template<typename T> 
  T data() {
    T result;
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    if (m_type == double_t) {
      // Keep precision.
      ss.precision(m_data.length());
      ss << std::fixed;
    }    
    ss << m_data;
    ss >> result;
    return result;
  }
  std::string data_str();
  
};

/**
 * @name Key - The Key object.
 *
 * This class defines the base key object which holds its type.
 */
class Key {
 public:
  enum Type 
  {
    value_t,
    array_t,
    list_t,
    pairs_t
  };
    
 private:
  Type m_type;
  std::string m_id;

 public:
  Key();
  virtual ~Key();

  void set_type(const Key::Type type);
  Key::Type type();
  void set_id(const std::string id);
  std::string id();
};

class KPairs : public Key {
 private:
  std::list<std::pair<std::string, Data> > m_list;
  std::list<std::pair<std::string, Data> >::iterator m_it;

 public:
  KPairs();
  ~KPairs();
  void insert(const std::string key, const Data value);
  std::pair<std::string, Data> *get_next();
  void clear();
  void reset();
  int32_t size();
};

/**
 * @name KList - The Key List.
 *
 * This class defines a key list object which holds a list
 * of data objects or a list of lists.
 */
class KList : public Key {
 private:
  std::list<Data> m_data;
  std::list<KList> m_list;
  std::list<Data>::iterator m_it_data;
  std::list<KList>::iterator m_it_list;

 public:
  KList();
  KList(KList *kl_ptr);
  ~KList();

  void insert_data(const Data data);
  void insert_klist(const KList klist);

  Data *get_next_data();
  KList *get_next_klist();
  
  void clear_data();
  void clear_klist();
  
  void reset_data();
  void reset_klist();
  
  int32_t size_of_data();
  int32_t size_of_klist();
};

/**
 * @name KArray - The Key Array.
 *
 * This class defines a key array object which holds an array
 * of data objects.
 */
class KArray : public Key {
 private:
  std::map<int32_t, Data> m_array;

 public:
  KArray();
  ~KArray();

  Data &operator[] (int32_t index);
  int32_t size();
 };

/**
 * @name KValue - The Key Value.
 *
 * This class defines a key value object which holds a single
 * data object.
 */
class KValue : public Key {
 private:
  Data m_value;

 public:
  KValue();
  ~KValue();

  void set_value(const Data value);
  Data value();
};

/**
 * @name Entity - The Entity object.
 *
 * This class defines the entity object which is used to describe an
 * independent component. It holds the entity ID, as well as two lists:
 * one that holds the 1-level nested entities and another one that holds
 * the keys. It also includes two iterators which help to get the
 * contained entities and keys.
 */
class Entity {
 private:
  std::string m_id;
  std::list<Key *> m_keys;
  std::list<Entity *> m_entities;
  std::list<Key *>::iterator m_it_keys;
  std::list<Entity *>::iterator m_it_entities;

 public:
  Entity();
  ~Entity();

  void set_id(const std::string id);
  std::string id();

  Key *find_key(std::string id);
  Entity *find_entity(std::string id);
  
  void add_entity(Entity *entity);
  void add_key(Key *key);

  Key *get_next_key();
  Entity *get_next_entity();
  
  void clear_keys();
  void clear_entities();
  
  void reset_keys();
  void reset_entities();
  
  int32_t size_of_keys();
  int32_t size_of_entities();
};

/**
 * @name Configuration - The Configuration object.
 *
 * This class defines the configuration object which is used to describe
 * a system. It holds two lists: one that holds the 1-level entities and
 * another one that holds the keys. It also includes two iterators which
 * help to get the contained entities and keys.
 */
class Configuration {
 private:
  std::list<Key *> m_keys;
  std::list<Entity *> m_entities;
  std::list<Key *>::iterator m_it_keys;
  std::list<Entity *>::iterator m_it_entities;

 public:
  Configuration();
  ~Configuration();

  Key *find_key(std::string id);
  Entity *find_entity(std::string id);
  
  void add_entity(Entity *entity);
  void add_key(Key *key);

  Key *get_next_key();
  Entity *get_next_entity();

  void clear_keys();
  void clear_entities();
  
  void reset_keys();
  void reset_entities();
  
  int32_t size_of_keys();
  int32_t size_of_entities();
};

#endif

