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
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <utility>
#include "configuration.h"

using namespace std;

/**
 * @name Data - Constructor.
 * @param type: The type of data.
 *
 * This constructor initializes the object with a defined type.
 */
Data::Data(const Data::Type type) {
  m_type = type;
  m_data.clear();
}

/**
 * @name Data - Constructor.
 *
 * This constructor initializes the object.
 */
Data::Data() {
  m_type = Data::none_t;
  m_data.clear();
}

/**
 * @name ~Data - Destructor.
 *
 * This is the destructor. It just clears the buffer.
 */
Data::~Data() {
  m_data.clear();
}

/**
 * @name type - Get data type.
 *
 * This returns the type of the data object.
 *
 * @return The type of the data object.
 */
Data::Type Data::type() {
  return m_type;
}

/**
 * @name set_data - Set data.
 * @param data: the data to store.
 * @param type: the type of data.
 *
 * This stores the data to the data object.
 *
 * @return Void.
 */
void Data::set_data(const string data, const Data::Type type) {
  m_type = type;
  m_data = data;
}

/**
 * @name data_str - Return data string.
 *
 * This function returns the string that contains the data.
 *
 * @return A string that contains the data value.
 */
std::string Data::data_str() {
  return m_data;
}

/**
 * @name Key - Constructor.
 *
 * This constructor initializes the key object.
 */
Key::Key() {
  m_id.clear();
}

/**
 * @name ~Key - Destructor.
 *
 * This destructor clears the key object.
 */
Key::~Key() {
  m_id.clear();
}

/**
 * @name set_type - Set key type.
 * @apram type: the type of key.
 *
 * This sets the type of the key object.
 *
 * @return Void.
 */
void Key::set_type(const Key::Type type) {
  m_type = type;
}

/**
 * @name type - Get key type.
 *
 * This returns the type of the key object.
 *
 * @return The key type.
 */
Key::Type Key::type() {
  return m_type;
}

/**
 * @name set_id - Set Key ID.
 * @param id: The key ID.
 *
 * This sets the id part of the key.
 *
 * @return Void.
 */
void Key::set_id(const string id) {
  m_id = id;
}

/**
 * @name id - Returns the key id.
 *
 * This returns the id part of the key.
 *
 * @return The key ID.
 */
string Key::id() {
  return m_id;
}

/**
 * @name KPairs - Constructor.
 *
 * This constructor initializes the KPairs object.
 */
KPairs::KPairs() {
  this->set_type(Key::pairs_t);
  m_list.clear();
}

/**
 * @name ~KPairs - Destructor.
 *
 * Clears the list.
 */
KPairs::~KPairs() {
  m_list.clear();
}

/**
 * @name insert - Insert an element into the list.
 * @param key: The ID part. 
 * @param value: The value part.
 *
 * This method inserts a key-value pair into the list.
 *
 * @return Void.
 */
void KPairs::insert(const string key, const Data value) {
  pair<string, Data> element;
  element.first = key;
  element.second = value;
  m_list.push_back(element);
  if (m_list.size() == 1)
    m_it = m_list.begin();
}

/**
 * @name clear - Clears the list.
 *
 * This method clears the list of pairs.
 *
 * @return Void.
 */
void KPairs::clear() {
  m_list.clear();
}

/**
 * @name reset - Reset the iterator.
 *
 * This method resets the list iterator. After returning, the iterator
 * points to the first element.
 *
 * @return Void.
 */
void KPairs::reset() {
  m_it = m_list.begin();
}

/**
 * @name size - List size.
 *
 * This returns the number of elements in the list.
 *
 * @return The number of elements.
 */
int32_t KPairs::size() {
  return m_list.size();
}

/**
 * @name get_next - Return the next list element.
 *
 * This returns the next pair and moves the iterator one step
 * forward. It also deletes the pair from the list.
 *
 * @return pointer to a new initialized pair object or NULL.
 *         The user should take care by properly deleting the
 *         returned object.
 */
pair<string, Data> *KPairs::get_next() {
  if (m_it != m_list.end() && !m_list.empty()) {
    pair<string, Data> *result = new pair<string, Data>(*m_it);
    ++m_it;
    m_list.pop_front();    
    return result;
  } else {
    return NULL;
  }
}

/**
 * @name KList - Constructor.
 *
 * This constructor initializes the KList object.
 */
KList::KList() {
  this->set_type(Key::list_t);
  m_list.clear();
  m_data.clear();
}

/**
 * @name KList - Copy Constructor.
 *
 * This constructor copies a  KList object.
 */
KList::KList(KList *kl_ptr) {
  this->set_type(kl_ptr->type());
  for (std::list<Data>::iterator it = kl_ptr->m_data.begin(); it != kl_ptr->m_data.end(); ++it) 
    m_data.push_back(*it);

  for (std::list<KList>::iterator it = kl_ptr->m_list.begin(); it != kl_ptr->m_list.end(); ++it) 
    m_list.push_back(*it);

  if (!m_data.empty())
    m_it_data = m_data.begin();

  if (!m_list.empty())
    m_it_list = m_list.begin();
}

/**
 * @name ~KList - Destructor.
 *
 * Clears the list.
 */
KList::~KList() {
  m_list.clear();
  m_data.clear();
}

/**
 * @name insert_data - Insert a Data object.
 * @param data: The Data object.
 *
 * This method inserts a data object into the list.
 *
 * @return Void.
 */
void KList::insert_data(const Data data) {
  m_data.push_back(data);
  if (m_data.size() == 1)
    m_it_data = m_data.begin();
}

/**
 * @name insert_klist - Insert a KList object.
 * @klist: The KList object.
 *
 * This method inserts a klist object into the list.
 *
 * @return Void.
 */
void KList::insert_klist(const KList klist) {
  m_list.push_back(klist);
  if (m_list.size() == 1)
    m_it_list = m_list.begin();
}

/**
 * @name clear_data - Clears the data list.
 *
 * This method clears the data list.
 *
 * Return: Void.
 */
void KList::clear_data() {
  m_data.clear();
}

/**
 * @name clear_klist - Clears the klist list.
 *
 * This method clears the klist list.
 *
 * @return Void.
 */
void KList::clear_klist() {
  m_list.clear();
}

/**
 * @name reset_data - Reset the data iterator.
 *
 * This method resets the data list iterator. After returning, the iterator
 * points to the first element.
 *
 * @return Void.
 */
void KList::reset_data() {
  m_it_data = m_data.begin();
}

/**
 * @name reset_klist - Reset the klist iterator.
 *
 * This method resets the entities list iterator. After returning, the iterator
 * points to the first element.
 *
 * @return Void.
 */
void KList::reset_klist() {
  m_it_list = m_list.begin();
}

/**
 * @name size_of_data - Data list size.
 *
 * This returns the number of elements in the data list.
 *
 * @return The number of elements.
 */
int32_t KList::size_of_data() {
  return m_data.size();
}

/**
 * @name size_of_klist - KList list size.
 *
 * This returns the number of elements in the KList list.
 *
 * @return The number of elements.
 */
int32_t KList::size_of_klist() {
  return m_list.size();
}

/**
 * @name get_next_data - Return the next Data element.
 *
 * This returns the next Data element and moves the iterator one step
 * forward. It also deletes the element from the list.
 *
 * @return Pointer to a new initialized Data object or NULL.
 *         The user should take care by properly deleting the
 *         returned object.
 */
Data *KList::get_next_data() {
  if (m_it_data != m_data.end() && !m_data.empty()) {
    Data *result = new Data;
    result->set_data((*m_it_data).data_str(), (*m_it_data).type());
    ++m_it_data;
    m_data.pop_front();
    return result;
  } else {
    return NULL;
  }
}

/**
 * @name get_next_klist - Return the next KList element.
 *
 * This returns the next KList element and moves the iterator one step
 * forward. It also deletes the element from the list.
 *
 * @return Pointer to a new initialized KList object or NULL.
 *         The user should take care by properly deleting the
 *         returned object.
 */
KList *KList::get_next_klist() {
  if (m_it_list != m_list.end() && !m_list.empty()) {
    KList *result = new KList(*m_it_list);
    ++m_it_data;
    m_list.pop_front();
    return result;
  } else {
    return NULL;
  }
}

/**
 * @name KArray - Constructor.
 *
 * This constructor initializes the KArray object.
 */
KArray::KArray() {
  this->set_type(Key::array_t);
  m_array.clear();
}

/**
 * @name ~KArray - Destructor.
 *
 * Clears the array.
 */
KArray::~KArray() {
  m_array.clear();
  
}

/**
 * @name Operator [] - Array operator.
 * @param index: The array index.
 *
 * Overloads the array operator.
 *
 * @return A reference to a data object.
 */
Data &KArray::operator[] (int32_t index) {
  return m_array[index];
}

/**
 * @name size - The array size.
 *
 * Returns the array size.
 *
 * @return The array size.
 */
int32_t KArray::size() {
  return m_array.size();
}

/**
 * @name KValue - Constructor.
 *
 * This constructor initializes the KValue object.
 */
KValue::KValue() {
  this->set_type(Key::value_t);
}

/**
 * @name ~KValue - Destructor.
 *
 * Currently it does nothing.
 */
KValue::~KValue() {
}

/**
 * @name set_value - Set the value.
 * @param value: the Data object that contains the value.
 *
 * This method sets the value of the object.
 *
 * @return Void.
 */
void KValue::set_value(const Data value) {
  m_value = value;
}

/**
 * @name value - Return the value.
 *
 * This method returns the value of the object.
 *
 * @return Data
 */
Data KValue::value() {
  return m_value;
}

/**
 * @name Entity - Constructor.
 *
 * This constructor initializes the Entity object.
 */
Entity::Entity() {
  m_keys.clear();
  m_entities.clear();
  m_id.clear();
}

/**
 * @name ~Entity - Destructor.
 *
 * Clears the entities and keys lists.
 */
Entity::~Entity() {
  while (!m_entities.empty()) {
    Entity *front = m_entities.front();
    delete front;
    m_entities.pop_front();
  }

  while (!m_keys.empty()) {
    Key *front = m_keys.front();
    delete front;
    m_keys.pop_front();
  }
}

/**
 * @name set_id - Set entity ID.
 * @param id: The ID string.
 *
 * This method a new ID to the entity.
 *
 * @return Void.
 */
void Entity::set_id(const std::string id) {
  m_id = id;
}

/**
 * @name id - Get entity ID.
 *
 * This method returns the entity ID.
 *
 * @return The ID string.
 */
string Entity::id() {
  return m_id;
}

/**
 * @name find_key - Search for a particular key.
 * @param id: The id of the key to search.
 *
 * This searches for a key with a given ID and returns it. It does not
 * delete the object from the list.
 *
 * @return The key object or NULL. The user should take
 *         care by properly deleting the returned object.
 */
Key *Entity::find_key(std::string id) {
  for (list<Key *>::iterator it = m_keys.begin(); it != m_keys.end(); ++it)
    if ((*it)->id() == id)
      return *it;
  return NULL;
}

/**
 * @name find_entity - Search for a particular entity.
 * @param id: The id of the entity to search.
 *
 * This searches for an entity with a given ID and returns it. It does not
 * delete the object from the list.
 *
 * @return The entity object or NULL. The user should take
 *         care by properly deleting the returned object.
 */
Entity *Entity::find_entity(std::string id) {
  for (list<Entity *>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
    if ((*it)->id() == id)
      return *it;
  return NULL;
}

/**
 * @name add_entity - Insert an entity into the entity list.
 * @param entity: The new entity.
 *
 * This method inserts a new entity object into the list if it does
 * not already exist.
 *
 * @return Void.
 */
void Entity::add_entity(Entity *entity) { 
  if (!find_entity(entity->id())) {      
    m_entities.push_back(entity);
    if (m_entities.size() == 1)
      m_it_entities = m_entities.begin();
  }
}

/**
 * @name add_key - Insert a key into the key list.
 * @param key: The new entity.
 *
 * This method inserts a new entity object into the list if it does
 * not already exist.
 *
 * @return Void
 */
void Entity::add_key(Key *key) {
  if (!find_key(key->id())) {      
    m_keys.push_back(key);
    if (m_keys.size() == 1)
      m_it_keys = m_keys.begin();
  }
}

/**
 * @name get_next_key - Return the next key.
 *
 * This returns the next key object and moves the iterator one step
 * forward. It also deletes the key pointer from the list.
 *
 * @return The next key object or NULL. The user should take
 *         care by properly deleting the returned object.
 */
Key *Entity::get_next_key() {
  if (m_it_keys != m_keys.end() && !m_keys.empty()) {
    Key *key = *m_it_keys;
    ++m_it_keys;
    m_keys.pop_front();
    return key;
  } else {
    return NULL;
  }
}

/**
 * @name get_next_entity - Return the next entity.
 *
 * This returns the next entity object and moves the iterator one step
 * forward. It also deletes the key pointer from the list.
 *
 * @return The next entity object or NULL. The user should take
 *         care by properly deleting the returned object.
 */
Entity *Entity::get_next_entity() {
  if (m_it_entities != m_entities.end() && !m_entities.empty()) {
    Entity *entity = *m_it_entities;
    ++m_it_entities;
    m_keys.pop_front();
    return entity;
  } else {
    return NULL;
  }
}

/**
 * @name clear_keys - Clears the keys list.
 *
 * This method clears the keys list.
 *
 * @return Void.
 */
void Entity::clear_keys() {
  while (!m_keys.empty()) {
    Key *front = m_keys.front();
    delete front;
    m_keys.pop_front();
  }
}

/**
 * @name clear_entities - Clears the entities list.
 *
 * This method clears the entities list.
 *
 * @return Void.
 */
void Entity::clear_entities() {
  while (!m_entities.empty()) {
    Entity *front = m_entities.front();
    delete front;
    m_entities.pop_front();
  }
}

/**
 * @name reset_keys - Reset the keys iterator.
 *
 * This method resets the keys list iterator. After returning, the iterator
 * points to the first element.
 *
 * @return Void.
 */
void Entity::reset_keys() {
  m_it_keys = m_keys.begin();
}

/**
 * @name reset_entities - Reset the entities iterator.
 *
 * This method resets the entities list iterator. After returning, the iterator
 * points to the first element.
 *
 * @return Void.
 */
void Entity::reset_entities() {
  m_it_entities = m_entities.begin();
}

/**
 * @name size_of_keys - Keys list size.
 *
 * This returns the number of elements in the keys list.
 *
 * @return The number of keys in the list.
 */
int32_t Entity::size_of_keys() {
  return m_keys.size();
}

/**
 * @name size_of_entities - Entities list size.
 *
 * This returns the number of elements in the entities list.
 *
 * @return The number of entities in the list.
 */
int32_t Entity::size_of_entities() {
  return m_entities.size();
}

/**
 * @name Configuration - Constructor.
 *
 * This constructor initializes the Configuration object.
 */
Configuration::Configuration() {
  m_keys.clear();
  m_entities.clear();
}

/**
 * @name ~Configuration - Destructor.
 *
 * Clears the entities and keys lists.
 */
Configuration::~Configuration() {
  while (!m_keys.empty()) {
    Key *front = m_keys.front();
    delete front;
    m_keys.pop_front();
  }
  while (!m_entities.empty()) {
    Entity *front = m_entities.front();
    delete front;
    m_entities.pop_front();
  }
}

/**
 * @name get_next_key - Return the next key.
 *
 * This returns the next key object and moves the iterator one step
 * forward. It also deletes the key pointer from the list.
 *
 * @return The next key object or NULL. The user should take
 *         care by properly deleting the returned object.
 */
Key *Configuration::get_next_key() {
  if (m_it_keys != m_keys.end() && !m_keys.empty()) {
    Key *key = *m_it_keys;
    ++m_it_keys;
    m_keys.pop_front();
    return key;
  } else {
    return NULL;
  }
}

/**
 * @name get_next_entity - Return the next entity.
 *
 * This returns the next entity object and moves the iterator one step
 * forward. It also deletes the key pointer from the list.
 *
 * @return The next entity object or NULL. The user should take
 *         care by properly deleting the returned object.
 */
Entity *Configuration::get_next_entity() {
  if (m_it_entities != m_entities.end() && !m_entities.empty()) {
    Entity *entity = *m_it_entities;
    ++m_it_entities;
    m_keys.pop_front();
    return entity;
  } else {
    return NULL;
  }
}

/**
 * @name find_key - Search for a particular key.
 * @param id: The id of the key to search.
 *
 * This searches for a key with a given ID and returns it. It does not
 * delete the object from the list.
 *
 * @return The key object or NULL. The user should take
 *         care by properly deleting the returned object.
 */
Key *Configuration::find_key(std::string id) {
  for (list<Key *>::iterator it = m_keys.begin(); it != m_keys.end(); ++it)
    if ((*it)->id() == id)
      return *it;
  return NULL;
}

/**
 * @name find_entity - Search for a particular entity.
 * @param id: The id of the entity to search.
 *
 * This searches for an entity with a given ID and returns it. It does not
 * delete the object from the list.
 *
 * @return The entity object or NULL. The user should take
 *         care by properly deleting the returned object.
 */
Entity *Configuration::find_entity(std::string id) {
  for (list<Entity *>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
    if ((*it)->id() == id)
      return *it;
  return NULL;
}

/**
 * @name add_entity - Insert an entity into the entity list.
 * @param entity: The new entity.
 *
 * This method inserts a new entity object into the list if it does
 * not already exist.
 *
 * @return Void.
 */
void Configuration::add_entity(Entity *entity) {
  if (!find_entity(entity->id())) {      
    m_entities.push_back(entity);
    if (m_entities.size() == 1)
      m_it_entities = m_entities.begin();
  }
}

/**
 * @name add_key - Insert a key into the key list.
 * @param key: The new entity.
 *
 * This method inserts a new entity object into the list.
 *
 * @return Void.
 */
void Configuration::add_key(Key *key) {
  if (!find_key(key->id())) {      
    m_keys.push_back(key);
    if (m_keys.size() == 1)
      m_it_keys = m_keys.begin();
  }
}

/**
 * @name clear_keys - Clears the keys list.
 *
 * This method clears the keys list.
 *
 * @return Void.
 */
void Configuration::clear_keys() {
  while (!m_keys.empty()) {
    Key *front = m_keys.front();
    delete front;
    m_keys.pop_front();
  }
}

/**
 * @name clear_entities - Clears the entities list.
 *
 * This method clears the entities list.
 *
 * @return Void.
 */
void Configuration::clear_entities() {
  while (!m_entities.empty()) {
    Entity *front = m_entities.front();
    delete front;
    m_entities.pop_front();
  }
}

/**
 * @name reset_keys - Reset the keys iterator.
 *
 * This method resets the keys list iterator. After returning, the iterator
 * points to the first element.
 *
 * @return Void.
 */
void Configuration::reset_keys() {
  m_it_keys = m_keys.begin();
}

/**
 * @name reset_entities - Reset the entities iterator.
 *
 * This method resets the entities list iterator. After returning, the iterator
 * points to the first element.
 *
 * @return Void.
 */
void Configuration::reset_entities() {
  m_it_entities = m_entities.begin();
}

/**
 * @name size_of_keys - Keys list size.
 * 
 * This returns the number of elements in the keys list.
 *
 * @return The number of keys.
 */
int32_t Configuration::size_of_keys() {
  return m_keys.size();
}

/**
 * @name size_of_entities - Entities list size.
 *
 * This returns the number of elements in the entities list.
 *
 * @return The number of entities.
 */
int32_t Configuration::size_of_entities() {
  return m_entities.size();
}

