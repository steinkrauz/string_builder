#include "string_builder_c.h"

void add_bucket(string_builder_p sb, unsigned size)
{
	bucket_p new_bucket = (bucket_p)malloc(sizeof(bucket_s));
	if (size>sb->add_size)
		new_bucket->data = (char *)malloc(size);
	else
		new_bucket->data = (char *)malloc(sb->add_size);
	memset(new_bucket->data, 0, size);
	new_bucket->pos = 0;
	new_bucket->next = NULL;
	sb->curr->next = new_bucket;
	sb->curr = new_bucket;
	sb->capacity += size;
}
	

void init(string_builder_p sb, unsigned add)
{
	sb->root = &sb->_base;
	sb->root->data = sb->_bdata;
	memset(sb->root->data, 0, BASE_SIZE);
	sb->root->pos = 0;
	sb->root->next = NULL;
	sb->curr = sb->root;
	sb->add_size = add;
	sb->len = 0;
	sb->capacity = BASE_SIZE;
}

void SB_init(string_builder_p sb)
{
	init(sb, ADD_SIZE);
}

void SB_init_ex(string_builder_p sb, unsigned add)
{
	init(sb, add);
}

string_builder_p SB_create()
{
	string_builder_p sb = (string_builder_p)malloc(sizeof(string_builder_s));
	init(sb, ADD_SIZE);
	return sb;
}

string_builder_p SB_create_ex(unsigned add)
{
	string_builder_p sb = (string_builder_p)malloc(sizeof(string_builder_s));
	init(sb, add);
	return sb;
}

void SB_append_char(string_builder_p sb, const char c)
{
	if (sb->len == sb->capacity) {
		add_bucket(sb, sb->add_size);
	}
	sb->curr->data[sb->curr->pos] = c;
	sb->curr->pos++;
	sb->len++;
}

void SB_append(string_builder_p sb, const char *str)
{
	size_t size = strlen(str);
	SB_append_length(sb, str, size);
}

void SB_append_length(string_builder_p sb, const char *str, size_t size)
{
	size_t avail = sb->capacity - sb->len;
	if (avail >= size) {
		memcpy(sb->curr->data+sb->curr->pos, str, size);
		sb->curr->pos += size;
		sb->len += size;
		return;
	}
	memcpy(sb->curr->data+sb->curr->pos, str, avail);
	sb->curr->pos += avail;
	sb->len += avail;
	size_t rest = size - avail;
	add_bucket(sb, 2 * rest);
	SB_append_length(sb, str + avail, rest);
}

char *SB_to_string(string_builder_p sb)
{
	char *str = (char *) malloc(sb->len + 1);
	memset(str, 0, sb->len + 1);
	bucket_p pb = sb->root;
	char *pC = str;
	while (pb) {
		memcpy(pC, pb->data, pb->pos);
		pC += pb->pos;
		pb = pb->next;
	}
	return str;
}

void SB_delete(string_builder_p sb)
{
	bucket_p pb = sb->root->next;
	while (pb) {
		bucket_p del = pb;
		pb = pb->next;
		free(del->data);
		free(del);
	}
}

void SB_dump(string_builder_p sb)
{
	bucket_p pb = sb->root;
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
