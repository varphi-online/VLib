#ifndef VJSON_H
#include <stdbool.h>

typedef enum vjson_value {
  VJSON_OBJECT,
  VJSON_ARRAY,
  VJSON_STRING,
  VJSON_NUMBER,
  VJSON_BOOLEAN,
  VJSON_NULL
} vjson_value;

// ptr is a context-specific pointer to a struct of vjson_value type
typedef struct vjson_element {
  vjson_value type;
  void *_ptr;
} vjson_element;

vjson_element *vjson_create_element(vjson_value type);

// Expects a heap-allocated vjson element, which recursively frees all
// children and self based on internal type automatically
void vjson_free_element(vjson_element *element);

// accepts a VJSON_OBJECT typed vjson element
vjson_element *vjson_get_key(vjson_element *object, char *key);

// accepts a VJSON_OBJECT typed vjson element
int vjson_set_key(vjson_element *object, char *key, vjson_element *value);

// accepts a VJSON_ARRAY typed vjson element
void vjson_append(vjson_element *array, vjson_element *to_append);

// pops an element, transferring ownership to the caller
// accepts a VJSON_ARRAY typed vjson element
vjson_element *vjson_pop(vjson_element *array);

// removes the last element of an array silently
// accepts a VJSON_ARRAY typed vjson element
void vjson_lop(vjson_element *array);

// accepts a VJSON_ARRAY typed vjson element
vjson_element *vjson_get_index(vjson_element *array, int index);

char *vjson_stringify(vjson_element *element, bool pretty_print);

vjson_element *vjson_parse(char *input);

// creates a vjson_element of type VJSON_STRING and populates it with the string
// passed
vjson_element *vjson_str(char *str);

vjson_element *vjson_num(float number);

vjson_element *vjson_arr(vjson_element *first);

vjson_element *vjson_nul();

vjson_element *vjson_boo(bool value);

vjson_element *vjson_obj(char *key, vjson_element *value);

#endif
