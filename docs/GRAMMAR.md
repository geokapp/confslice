Configuration File Grammar
==========================

Confslice supports a simple configuration file format that consists of
**entities** and **keys**. Entities can be used to describe components,
while keys can be used to describe component or system-wide properties.


Tokens
------

The alphabet of confslice consists of: 
1. The upper-case and lower-case letters of the Latin alphabet 
   ('A', ..., 'Z', and 'a', ..., 'z'). Not that confslice is 
   case-sensitive.

2. The digits (0, ..., 9).

3. The UTF-8 symbols.

Entity and key IDs are strings that consist of letters and digits, and 
always starting with a letter. White spaces like tabs, spaces, and new 
lines are ignored and can be used for text formatting. Confslice also
supports line comments delimited by "//". Comments can be either at the 
start or at the end of the line and they are ignored by the lexical 
analyzer.

Key values can be either strings, integers, or doubles.  A valid double 
number in confslice is formed by an optional sign character ('+' or '-'), 
followed by a sequence of digits, optionally containing a decimal-point 
character '.'.


Configuration file syntax
-------------------------

Here we give an overview of the configuration file syntax. You can find
a detailed description of its syntax in the section `Detailed syntax`.

The configuration files that confslice understands can contain a) entities and
b) keys. Entities represent independent components and can contain other entities
or keys. An entity can be defined as follows:


```
an_entity: {
   ...

};

The first string is the entity ID and it is followed by a ':' and a '{'. 
Then,other entity definitions or keys are following. The entity definition 
is ended with a } followed by a ;.
In the following example, "sys" is a nested entity that belongs to the entity
with ID "server":

```
server: {
      // This is a comment.
      sys: {
      ...
   };
};
```

Keys represent properties and can be associated with an entity or the 
global configuration. Confslice understands four different types of keys:

1. **Key-Value**
   This type of key contains an ID and a single value and can be defined as 
   follows:

   `my_key = 1000;`

2. **Array**
   This type of key contains an ID and an array of values and can be defined 
   as follows:

   `small_array = [4, 5, 3, 1000];`

3. **List**
   This type of key contains an ID and a list of values and sub-lists and can 
   be defined as follows:

   `groups = <100, 300, <43, 2, <12, 3>, 9>, 10>;`

4. **Pairs**
   This type of key contains an ID and a list of key-value pairs. It can be 
   defined as follows:

   ```
   my_pairs = {
      name = "George";
      city = "Greece";
      age = 27;
   };
   ````

IDs are **strings**, while values can be either **strings, integers**, or **doubles**. 
A complete example of a configuration file follows:

```
// A comment.
data_server: {
   ip = "10.7.2.3"; // Another comment.
   open_ports = [7000,7001,7002,7003];
   hostname = "blade";
   disk.1: {
      disk_size = "1T";
      journal_size = 10000;
   };
   admin = {
      username = "root";
      realname = "Super Admin";
      uid = 0;
   };
};
security = "kerberos";
```

Detailed syntax
---------------

<CONFIGURATION> ::= <DECLARATIONS>

<DECLARATIONS ::= nil | (<ENTITY>**;** | <KEY>**;**)(<ENTITY>**;** | <KEY>**;**)*

<ENTITY> ::= **ID : {** <ENTITYBODY> **}**

<ENTITYBODY> ::= (<ENTITY**;** | <KEY>**;**)(<ENTITY>**;** | <KEY>**;**)*

<KEY> ::= <KEYVALUE> | <KEYARRAY> | <KEYLIST> | <KEYPAIRS>

<KEYVALUE> ::= **ID =** <VALUE>

<KEYARRAY> ::= **ID = [** <KEYARRAYBODY> **]**

<KEYARRAYBODY> ::= <VALUE>(**,**<VALUE>)*

<KEYLIST> ::= **ID = <** <KEYLISTBODY> **>**

<KEYLISTBODY ::= (<VALUE> | <SUBLIST>)(**,**<VALUE> | <SUBLIST>)*

<SUBLIST> ::= **<** <KEYLISTBODY> **>**

<KEYPAIRS> ::= **ID =** <KEYPAIRSBODY>

<KEYPAIRSBODY> ::= **{** <KEYVALUE> (**;**(KEYVALUE>)* **}**

<VALUE> ::= **STRING** | **INTEGER** | **DOUBLE**

