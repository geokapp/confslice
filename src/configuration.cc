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
 * ----------------------------------------------------------
 * Class: Data
 * ----------------------------------------------------------
 */

/**
 * Data - Constructor.
 * @type: The type of data.
 *
 * This constructor initializes the object with a defined type.
 */
Data::Data(const Data::Type type) {
  m_type = type;
  m_data.clear();
}

/**
 * Data - Constructor.
 *
 * This constructor initializes the object.
 */
Data::Data() {
  m_type = Data::none_t;
  m_data.clear();
}

/**
 * ~Data - Destructor.
 *
 * This is the destructor. It just clears the buffer.
 */
Data::~Data() {
  m_data.clear();
}

/**
 * type - Get data type.
 *
 * This returns the type of the data object.
 */
Data::Type Data::type() {
  return m_type;
}

/**
 * set_data - Set data.
 * @data: the data to store.
 * @type: the type of data.
 *
 * This stores the data to the data object.
 */
void Data::set_data(const string data, const Data::Type type) {
  m_type = type;
  m_data = data;
}

/**
 * data_str - Return data string.
 *
 * This function returns the string that contains the data.
 */
std::string Data::data_str() {
  return m_data;
}


/**
 * ----------------------------------------------------------
 * Class: Key
 * ----------------------------------------------------------
 */

/**
 * Key - Constructor.
 *
 * This constructor initializes the key object.
 */
Key::Key() {
  m_id.clear();
}

/**
 * ~Key - Destructor.
 *
 * This destructor clears the key object.
 */
Key::~Key() {
  m_id.clear();
}

/**
 * set_type - Set key type.
 * @type: the type of key.
 *
 * This sets the type of the key object.
 */
void Key::set_type(const Key::Type type) {
  m_type = type;
}

/**
 * type - Get key type.
 *
 * This returns the type of the key object.
 */
Key::Type Key::type() {
  return m_type;
}

/**
 * set_id - Set Key ID.
 * @id: The key ID.
 *
 * This sets the id part of the key.
 */
void Key::set_id(const string id) {
  m_id = id;
}

/**
 * id - Returns the key id.
 *
 * This returns the id part of the key.
 */
string Key::id() {
  return m_id;
}

/**
 * ----------------------------------------------------------
 * Class: KPairs
 * ----------------------------------------------------------
 */

/**
 * KPairs - Constructor.
 *
 * This constructor initializes the KPairs object.
 */
KPairs::KPairs() {
  this->set_type(Key::pairs_t);
  m_list.clear();
}

/**
 * ~KPairs - Destructor.
 *
 * Clears the list.
 */
KPairs::~KPairs() {
  m_list.clear();
}

/**
 * insert - Insert an element into the list.
 * @key: The ID part. 
 * @value: The value part.
 *
 * This method inserts a key-value pair into the list.
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
 * clear - Clears the list.
 *
 * This method clears the list of pairs.
 */
void KPairs::clear() {
  m_list.clear();
}

/**
 * reset - Reset the iterator.
 *
 * This method resets the list iterator. After returning, the iterator
 * points to the first element.
 */
void KPairs::reset() {
  m_it = m_list.begin();
}

/**
 * size - List size.
 *
 * This returns the number of elements in the list.
 */
int32_t KPairs::size() {
  return m_list.size();
}

/**
 * get_next - Return the next list element.
 *
 * This returns the next pair and moves the iterator one step
 * forward. It also deletes the pair from the list.
 *
 * Return Value: pointer to a new initialized pair object or NULL.
 *               The user should take care by properly deleting the
 *               returned object.
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
 * ----------------------------------------------------------
 * Class: KList
 * ----------------------------------------------------------
 */

/**
 * KList - Constructor.
 *
 * This constructor initializes the KList object.
 */
KList::KList() {
  this->set_type(Key::list_t);
  m_list.clear();
  m_data.clear();
}

/**
 * KList - Copy Constructor.
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
 * ~KList - Destructor.
 *
 * Clears the list.
 */
KList::~KList() {
  m_list.clear();
  m_data.clear();
}

/**
 * insert_data - Insert a Data object.
 * @data: The Data object.
 *
 * This method inserts a data object into the list.
 */
void KList::insert_data(const Data data) {
  m_data.push_back(data);
  if (m_data.size() == 1)
    m_it_data = m_data.begin();
}

/**
 * insert_klist - Insert a KList object.
 * @klist: The KList object.
 *
 * This method inserts a klist object into the list.
 */
void KList::insert_klist(const KList klist) {
  m_list.push_back(klist);
  if (m_list.size() == 1)
    m_it_list = m_list.begin();
}

/**
 * clear_data - Clears the data list.
 *
 * This method clears the data list.
 */
void KList::clear_data() {
  m_data.clear();
}

/**
 * clear_klist - Clears the klist list.
 *
 * This method clears the klist list.
 */
void KList::clear_klist() {
  m_list.clear();
}

/**
 * reset_data - Reset the data iterator.
 *
 * This method resets the data list iterator. After returning, the iterator
 * points to the first element.
 */
void KList::reset_data() {
  m_it_data = m_data.begin();
}

/**
 * reset_klist - Reset the klist iterator.
 *
 * This method resets the entities list iterator. After returning, the iterator
 * points to the first element.
 */
void KList::reset_klist() {
  m_it_list = m_list.begin();
}

/**
 * size_of_data - Data list size.
 *
 * This returns the number of elements in the data list.
 */
int32_t KList::size_of_data() {
  return m_data.size();
}

/**
 * size_of_klist - KList list size.
 *
 * This returns the number of elements in the KList list.
 */
int32_t KList::size_of_klist() {
  return m_list.size();
}

/**
 * get_next_data - Return the next Data element.
 *
 * This returns the next Data element and moves the iterator one step
 * forward. It also deletes the element from the list.
 *
 * Return Value: Pointer to a new initialized Data object or NULL.
 *               The user should take care by properly deleting the
 *               returned object.
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
 * get_next_klist - Return the next KList element.
 *
 * This returns the next KList element and moves the iterator one step
 * forward. It also deletes the element from the list.
 *
 * Return Value: Pointer to a new initialized KList object or NULL.
 *               The user should take care by properly deleting the
 *               returned object.
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
 * ----------------------------------------------------------
 * Class: KArray
 * ----------------------------------------------------------
 */

/**
 * KArray - Constructor.
 *
 * This constructor initializes the KArray object.
 */
KArray::KArray() {
  this->set_type(Key::array_t);
  m_array.clear();
}

/**
 * ~KArray - Destructor.
 *
 * Clears the array.
 */
KArray::~KArray() {
  m_array.clear();
  
}

/**
 * Operator [] - Array operator.
 * @index: The array index.
 *
 * Overloads the array operator.
 */
Data &KArray::operator[] (int32_t index) {
  return m_array[index];
}

/**
 * size - The array size.
 *
 * Returns the array size.
 */
int32_t KArray::size() {
  return m_array.size();
}

/**
 * ----------------------------------------------------------
 * Class: KValue
 * ----------------------------------------------------------
 */

/**
 * KValue - Constructor.
 *
 * This constructor initializes the KValue object.
 */
KValue::KValue() {
  this->set_type(Key::value_t);
}

/**
 * ~KValue - Destructor.
 *
 * Currently it does nothing.
 */
KValue::~KValue() {
}

/**
 * set_value - Set the value.
 * @value: the Data object that contains the value.
 *
 * This method sets the value of the object.
 */
void KValue::set_value(const Data value) {
  m_value = value;
}

/**
 * value - Return the value.
 *
 * This method returns the value of the object.
 */
Data KValue::value() {
  return m_value;
}

/**
 * ----------------------------------------------------------
 * Class: Entity
 * ----------------------------------------------------------
 */

/**
 * Entity - Constructor.
 *
 * This constructor initializes the Entity object.
 */
Entity::Entity() {
  m_keys.clear();
  m_entities.clear();
  m_id.clear();
}

/**
 * ~Entity - Destructor.
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
 * set_id - Set entity ID.
 * @id: The ID string.
 *
 * This method a new ID to the entity.
 */
void Entity::set_id(const std::string id) {
  m_id = id;
}

/**
 * id - Get entity ID.
 *
 * This method returns the entity ID.
 */
string Entity::id() {
  return m_id;
}

/**
 * find_key - Search for a particular key.
 *
 * This searches for a key with a given ID and returns it. It does not
 * delete the object from the list.
 *
 * Return Value: The key object or NULL. The user should take
 *               care by properly deleting the returned object.
 */
Key *Entity::find_key(std::string id) {
  for (list<Key *>::iterator it = m_keys.begin(); it != m_keys.end(); ++it)
    if ((*it)->id() == id)
      return *it;
  return NULL;
}

/**
 * find_entity - Search for a particular entity.
 *
 * This searches for an entity with a given ID and returns it. It does not
 * delete the object from the list.
 *
 * Return Value: The entity object or NULL. The user should take
 *               care by properly deleting the returned object.
 */
Entity *Entity::find_entity(std::string id) {
  for (list<Entity *>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
    if ((*it)->id() == id)
      return *it;
  return NULL;
}

/**
 * add_entity - Insert an entity into the entity list.
 * @entity: The new entity.
 *
 * This method inserts a new entity object into the list if it does
 * not already exist.
 */
void Entity::add_entity(Entity *entity) { 
  if (!find_entity(entity->id())) {      
    m_entities.push_back(entity);
    if (m_entities.size() == 1)
      m_it_entities = m_entities.begin();
  }
}

/**
 * add_key - Insert a key into the key list.
 * @key: The new entity.
 *
 * This method inserts a new entity object into the list if it does
 * not already exist.
 */
void Entity::add_key(Key *key) {
  if (!find_key(key->id())) {      
    m_keys.push_back(key);
    if (m_keys.size() == 1)
      m_it_keys = m_keys.begin();
  }
}

/**
 * get_next_key - Return the next key.
 *
 * This returns the next key object and moves the iterator one step
 * forward. It also deletes the key pointer from the list.
 *
 * Return Value: The next key object or NULL. The user should take
 *               care by properly deleting the returned object.
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
 * get_next_entity - Return the next entity.
 *
 * This returns the next entity object and moves the iterator one step
 * forward. It also deletes the key pointer from the list.
 *
 * Return Value: The next entity object or NULL. The user should take
 *               care by properly deleting the returned object.
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
 * clear_keys - Clears the keys list.
 *
 * This method clears the keys list.
 */
void Entity::clear_keys() {
  while (!m_keys.empty()) {
    Key *front = m_keys.front();
    delete front;
    m_keys.pop_front();
  }
}

/**
 * clear_entities - Clears the entities list.
 *
 * This method clears the entities list.
 */
void Entity::clear_entities() {
  while (!m_entities.empty()) {
    Entity *front = m_entities.front();
    delete front;
    m_entities.pop_front();
  }
}

/**
 * reset_keys - Reset the keys iterator.
 *
 * This method resets the keys list iterator. After returning, the iterator
 * points to the first element.
 */
void Entity::reset_keys() {
  m_it_keys = m_keys.begin();
}

/**
 * reset_entities - Reset the entities iterator.
 *
 * This method resets the entities list iterator. After returning, the iterator
 * points to the first element.
 */
void Entity::reset_entities() {
  m_it_entities = m_entities.begin();
}

/**
 * size_of_keys - Keys list size.
 *
 * This returns the number of elements in the keys list.
 */
int32_t Entity::size_of_keys() {
  return m_keys.size();
}

/**
 * size_of_entities - Entities list size.
 *
 * This returns the number of elements in the entities list.
 */
int32_t Entity::size_of_entities() {
  return m_entities.size();
}

/**
 * ----------------------------------------------------------
 * Class: Configuration
 * ----------------------------------------------------------
 */

/**
 * Configuration - Constructor.
 *
 * This constructor initializes the Configuration object.
 */
Configuration::Configuration() {
  m_keys.clear();
  m_entities.clear();
}

/**
 * ~Configuration - Destructor.
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
 * get_next_key - Return the next key.
 *
 * This returns the next key object and moves the iterator one step
 * forward. It also deletes the key pointer from the list.
 *
 * Return Value: The next key object or NULL. The user should take
 *               care by properly deleting the returned object.
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
 * get_next_entity - Return the next entity.
 *
 * This returns the next entity object and moves the iterator one step
 * forward. It also deletes the key pointer from the list.
 *
 * Return Value: The next entity object or NULL. The user should take
 *               care by properly deleting the returned object.
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
 * find_key - Search for a particular key.
 *
 * This searches for a key with a given ID and returns it. It does not
 * delete the object from the list.
 *
 * Return Value: The key object or NULL. The user should take
 *               care by properly deleting the returned object.
 */
Key *Configuration::find_key(std::string id) {
  for (list<Key *>::iterator it = m_keys.begin(); it != m_keys.end(); ++it)
    if ((*it)->id() == id)
      return *it;
  return NULL;
}

/**
 * find_entity - Search for a particular entity.
 *
 * This searches for an entity with a given ID and returns it. It does not
 * delete the object from the list.
 *
 * Return Value: The entity object or NULL. The user should take
 *               care by properly deleting the returned object.
 */
Entity *Configuration::find_entity(std::string id) {
  for (list<Entity *>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
    if ((*it)->id() == id)
      return *it;
  return NULL;
}

/**
 * add_entity - Insert an entity into the entity list.
 * @entity: The new entity.
 *
 * This method inserts a new entity object into the list if it does
 * not already exist.
 */
void Configuration::add_entity(Entity *entity) {
  if (!find_entity(entity->id())) {      
    m_entities.push_back(entity);
    if (m_entities.size() == 1)
      m_it_entities = m_entities.begin();
  }
}

/**
 * add_key - Insert a key into the key list.
 * @key: The new entity.
 *
 * This method inserts a new entity object into the list.
 */
void Configuration::add_key(Key *key) {
  if (!find_key(key->id())) {      
    m_keys.push_back(key);
    if (m_keys.size() == 1)
      m_it_keys = m_keys.begin();
  }
}

/**
 * clear_keys - Clears the keys list.
 *
 * This method clears the keys list.
 */
void Configuration::clear_keys() {
  while (!m_keys.empty()) {
    Key *front = m_keys.front();
    delete front;
    m_keys.pop_front();
  }
}

/**
 * clear_entities - Clears the entities list.
 *
 * This method clears the entities list.
 */
void Configuration::clear_entities() {
  while (!m_entities.empty()) {
    Entity *front = m_entities.front();
    delete front;
    m_entities.pop_front();
  }
}

/**
 * reset_keys - Reset the keys iterator.
 *
 * This method resets the keys list iterator. After returning, the iterator
 * points to the first element.
 */
void Configuration::reset_keys() {
  m_it_keys = m_keys.begin();
}

/**
 * reset_entities - Reset the entities iterator.
 *
 * This method resets the entities list iterator. After returning, the iterator
 * points to the first element.
 */
void Configuration::reset_entities() {
  m_it_entities = m_entities.begin();
}

/**
 * size_of_keys - Keys list size.
 *
 * This returns the number of elements in the keys list.
 */
int32_t Configuration::size_of_keys() {
  return m_keys.size();
}

/**
 * size_of_entities - Entities list size.
 *
 * This returns the number of elements in the entities list.
 */
int32_t Configuration::size_of_entities() {
  return m_entities.size();
}

