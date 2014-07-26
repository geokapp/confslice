Confslice - A simple configuration file parser
==============================================

Confslice is a C++ library for parsing structured configuration files 
and generating configuration schemes. It defines a simple and compact
configuration file format which consists of **entities** and **keys**. Entities
represent independent components such as subsystems, while keys represent
characteristics and can be associated with an entity or the global 
configuration. 


Building and installing Confslice
---------------------------------

To build confslice just type:
   `make`

To build a development version of Confslice with support for debugging 
symbols type:
   `make dev`

To build only the tests type:
   `make tests`

To install the confslice library, run the following as root:
   `make install`
	

Removing Confslice
------------------

To remove the confslice library type as root:
   `make remove`


Testing Confslice
-----------------

We have included several unit tests which check whether the confslice library
works correctly. Check the `tests` folder for details.


Configuration file general syntax
---------------------------------

Here we give a basic overview of the configuration file syntax. You can find
a detailed description of its grammar and syntax in docs/GRAMMAR.md.

The configuration files that confslice understands can contain a) entities and
b) keys. Entities represent independent components and can contain other entities
or keys. An entity can be defined as follows:


```
an_entity: {
   ...
   // This is a comment.
      another entity: {
      ...
   };
};
```

Keys represent characteristics and can be associated with an entity or the 
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
data_server: {
   ip = "10.7.2.3";
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


Using confslice
---------------

To use the confslice library you need to include `#include <confslice/confslice.h`
to your C++ source file. Then, providing you have installed confslice into your 
system, you can build your application as follows:
   
   `$(CC) -o application_binary application.cc -lconfslice`

Into your application's source you can define a ConfSlice object as follows:

   `ConfSlice my_conf;`

To load and analyze a configuration file, call the analyze() method:

   ```
   string filename = "myfile.cfg"
   int result = my_conf.analyze(filename);
   ```

To get the configuration schema just call:

   ```
   if (!result)
      Configuration *conf = my_conf.configuration();
   ```

To get the next entity from the configuration use the `get_next_entity()`
method. Similarly, to get the next key from the configuration use the `get_next_key()`
method. Additionally, to get a nested entity or a key from an entity, use the entity's
`get_next_entity()` and `get_next_key()` methods respectively.

An example follows:
   
   ```
   Entity *entity;
   while (entity = conf->get_next_entity()) {
      Entity *nested_entity;
      while (nested_entity = entity->get_next_entity()) {
         Key *key;
         while (key = nested_entity->get_next_key()) {
            if (key->type == value_tk) {
               // a key value
               ...
            } else if (key->type == array_t) {
               // an array
               ...
            } else if (key->type == list_t)
               // a list
               ...
            } else {// pair_t
               // a pair
               ...
            }
         }
      }
   }
   ```

You can find a detailed description of the API in docs/API/index.html.

Development and Contributing
----------------------------

The initial developer of confslice is [Giorgos Kappes](http://cs.uoi.gr/~gkappes). Feel free to contribute to
the development of confslice by cloning the repository: 
`git clone https://github.com/geokapp/confslice`
You can also send feedback, new feature requests, and bug reports to 
<geokapp@gmail.com>.
