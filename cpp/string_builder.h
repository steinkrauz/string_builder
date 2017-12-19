/*
     * This program is free software. It comes without any warranty, to
     * the extent permitted by applicable law. You can redistribute it
     * and/or modify it under the terms of the Do What The Fuck You Want
     * To Public License, Version 2, as published by Sam Hocevar. See
     * http://www.wtfpl.net/ for more details. 

     Copyright © 2017 Stein Krauz <steinkrauz@yahoo.com>

     See https://bitbucket.org/steinkrauz/string_builder for the latest version
*/

/**
* \class string_builder
*
* \brief Trivial StringBuilder implementation
*
* This class is intended to provide simple, lightweight drop-in
* functionality for building a C-style string. You may want to fine-tune
* this class by changing \c BASE_SIZE and \c ADD_SIZE definitions.
*
* \note The first buffer is size of BASE_SIZE and allocated statically. The resulted string
* returned by \c to_string() method is allocated dynamically and must be freed after use.
*
*/
#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include <string.h>
#include <stdio.h>

/// Size of base buffer. Make it large enought to fit most of possible strings.
#define BASE_SIZE	64
/// Mininal size of additional buffers. 
#define ADD_SIZE	16

/// A string builder's buffer (an element of a linked list)
struct bucket_s {
	/// string storage
	char *data;	
	/// current position/size of full buffer
	unsigned pos; 
	/// pointer to the next buffers
	bucket_s *next;	
};

/// pointer alias
typedef bucket_s * bucket_p;

class string_builder {
private:
	bucket_s _base;	/// first buffer static
	char _bdata[BASE_SIZE]; /// static storage for the first buffer
	bucket_p root; /// buffer list start
	bucket_p curr; /// current buffer element
	size_t len;		/// the lenght of the string being builded
	size_t capacity;/// string builder current capacity
	unsigned add_size;/// mininal new buffer size
	void add_bucket(unsigned size); /// adds a buffer of the given size to the list
	void init(unsigned add);	/// initialises fields
public:
	string_builder(); /// default constructor
	/**
	\param add minimum size for a new buffer
	*/
	string_builder(unsigned add); 
	void append(const char c); /// obvious
	void append(const char *str); /// obvious
	/**
	Appends a string to the builder
	\param str a string
	\param size size of the string
	*/
	void append(const char *str, size_t size);
	/**
	Returns the builded string
	\return A copy of builded string, must be freed.
	*/
	char *to_string();
	~string_builder();
	/**
	Just a debugging routine to show the internals of the builder.
	There is no need to use it.
	*/
	void dump();
};

#endif
