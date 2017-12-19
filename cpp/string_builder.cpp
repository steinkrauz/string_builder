/*
     * This program is free software. It comes without any warranty, to
     * the extent permitted by applicable law. You can redistribute it
     * and/or modify it under the terms of the Do What The Fuck You Want
     * To Public License, Version 2, as published by Sam Hocevar. See
     * http://www.wtfpl.net/ for more details. 

     Copyright © 2017 Stein Krauz <steinkrauz@yahoo.com>

     See https://bitbucket.org/steinkrauz/string_builder for the latest version
*/
#include "string_builder.h"

void string_builder::add_bucket(unsigned size)
{
	bucket_p new_bucket = new bucket_s;
	new_bucket->data = new char[size];
	memset(new_bucket->data, 0, size);
	new_bucket->pos = 0;
	new_bucket->next = NULL;
	curr->next = new_bucket;
	curr = new_bucket;
	capacity += size;
}

void string_builder::init(unsigned add)
{
	root = &_base;
	root->data = _bdata;
	memset(root->data, 0, BASE_SIZE);
	root->pos = 0;
	root->next = NULL;
	curr = root;
	add_size = add;
	len = 0;
	capacity = BASE_SIZE;
}

string_builder::string_builder()
{
	init(ADD_SIZE);
}

string_builder::string_builder(unsigned add)
{
	init(add);
}

void string_builder::append(const char c)
{
	if (len == capacity) {
		add_bucket(add_size);
	}
	curr->data[curr->pos] = c;
	curr->pos++;
	len++;
}


void string_builder::append(const char *str)
{
	size_t size = strlen(str);
	append(str, size);
}

void string_builder::append(const char *str, size_t size)
{
	size_t avail = capacity - len;
	if (avail >= size) {
		memcpy(curr->data+curr->pos, str, size);
		curr->pos += size;
		len += size;
		return;
	}
	memcpy(curr->data+curr->pos, str, avail);
	curr->pos += avail;
	len += avail;
	size_t rest = size - avail;
	add_bucket(2 * rest);
	append(str + avail, rest);
}

char * string_builder::to_string()
{
	char *str = new char[len + 1];
	str[len] = '\0';
	bucket_p pb = root;
	char *pC = str;
	while (pb) {
		memcpy(pC, pb->data, pb->pos);
		pC += pb->pos;
		pb = pb->next;
	}
	return str;
}

string_builder::~string_builder()
{
	bucket_p pb = root->next;
	while (pb) {
		bucket_p del = pb;
		pb = pb->next;
		delete[] del->data;
		delete del;
	}
}

void string_builder::dump()
{
	bucket_p pb = root;
	int i = 0;
	while (pb) {
		printf("Bucket %i:\n", i++);
		printf("Data:");
		for (unsigned j = 0; j < pb->pos; j++)
			printf("%c", pb->data[j]);
		printf("\nPos: %i\n\n", pb->pos);
		pb = pb->next;
	}
}


