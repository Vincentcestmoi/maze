#include "gui_textbuf.h"

text_buffer* create_text_buffer() {
  text_buffer* new;
  MALLOC(new, 1);
  MALLOC(new->content, 80);
  new->capacity = 80;
  new->length = 0;
  new->content[0] = '\0';
  return new;

}

void free_text_buffer(text_buffer* buffer) {
  if (buffer) {
    free(buffer->content);
    free(buffer);
  }
}

void expand_text_buffer(text_buffer* text, int inc) {
  if (text) {
    text->capacity += inc;
    REALLOC(text->content, text->capacity);
  }
}


static inline int buffer_max(int a, int b) {
  return a > b ? a : b;
}


void append_to_text_buffer(text_buffer* text, char* suffix) {
  if (!suffix) {
    return;
  }

  int increment = strlen(suffix);

  if (text->capacity < text->length + increment + 1) {
    int new_size = buffer_max(increment, 2 * text->capacity);
    expand_text_buffer(text, new_size);
  }

  strncat(text->content, suffix, text->capacity);
  text->length += increment;
}

void remove_char(text_buffer* text) {
  if (text) {
    if (text->length) {
      text->length--;
    }
    text->content[text->length] = 0;
  }
}
