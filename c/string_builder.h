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
*
* \brief Trivial StringBuilder implementation
*
* These routines are intended to provide simple, lightweight drop-in
* functionality for building a C string. You may want to fine-tune
* them by changing \c BASE_SIZE and \c ADD_SIZE definitions.
*
* \note The first buffer is size of BASE_SIZE and allocated statically. The resulted string
* returned by \c SB_to_string() function is allocated dynamically and must be freed after use.
*
*/

#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/// Size of base buffer. Make it large enought to fit most of possible strings.
#define BASE_SIZE	64
/// Mininal size of additional buffers.
#define ADD_SIZE	16

/// A string builder's buffer (an element of a linked list)
typedef struct bucket_t {
	/// string storage
	char *data;
	/// current position/size of full buffer
	unsigned pos;
	/// pointer to the next buffer
	struct bucket_t *next;
} bucket_s;

/// pointer alias
typedef bucket_s * bucket_p;


typedef struct string_builder_t {
	bucket_s _base; /// first buffer static
	char _bdata[BASE_SIZE]; /// static storage for the first buffer
	bucket_p root; /// buffer list start
	bucket_p curr; /// current buffer element
	size_t len;    /// the lenght of the string being builded
	size_t capacity;   /// string builder current capacity
	unsigned add_size; /// mininal new buffer size
} string_builder_s;


/// pointer alias
typedef string_builder_s * string_builder_p;


	void SB_init(string_builder_p sb); /// Initialises existing string builder
	/**
	Initialises an existing string builder.
        \param add minimum size for a new buffer
        */
	void SB_init_ex(string_builder_p sb, unsigned add);
	string_builder_p SB_create(); /// Allocates new string builder
	string_builder_p SB_create_ex(unsigned add); /// Allocates new string builder with min size
	void SB_append_char(string_builder_p sb, const char c); /// Obvious
	void SB_append(string_builder_p sb, const char *str);   /// Obvious
	/**
        Appends a string to the builder
        \param str a string
        \param size size of the string
        */
	void SB_append_length(string_builder_p sb, const char *str, size_t size);
	/**
        Returns the builded string
        \return A copy of builded string, must be freed.
        */
	char *SB_to_string(string_builder_p sb);
	void SB_delete(string_builder_p sb); /// Frees all allocated storage
	 /**
        Just a debugging routine to show the internals of the builder.
        There is no need to use it.
        */
	void SB_dump(string_builder_p sb);

#endif
