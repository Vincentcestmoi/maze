/**
 * @file gui_textbuf.h
 * @brief
 * Interface: gestion des buffers utilisés lors de la saisie de texte. Ne PAS modifier.
 *
 * @attention
 * Cette partie est déjà implémentée. Il ne faut PAS la modifier.
 */

#ifndef TEXTBUF_H_
#define TEXTBUF_H_

#include <stdlib.h>
#include <string.h>
#include "alloc.h"


 /*  ____        _     _            _        _            _        */
 /* / ___|  __ _(_)___(_) ___    __| | ___  | |_ _____  _| |_ ___  */
 /* \___ \ / _` | / __| |/ _ \  / _` |/ _ \ | __/ _ \ \/ / __/ _ \ */
 /*  ___) | (_| | \__ \ |  __/ | (_| |  __/ | ||  __/>  <| ||  __/ */
 /* |____/ \__,_|_|___/_|\___|  \__,_|\___|  \__\___/_/\_\\__\___| */

/**
 * @brief Le buffer.
*/
typedef struct {
  char* content; //!< La chaîne de caractères.
  int capacity; //!< La capacité actuelle du buffer.
  int length; //!< La longueur de la chaîne.
} text_buffer;


/**
 * @brief Création d'un nouveau buffer.
*/
text_buffer* create_text_buffer(void);

/**
 * @brief Libération d'un buffer.
*/
void free_text_buffer(text_buffer* //!< Le buffer.
);

/**
 * @brief Agrandissement de la capacité d'un buffer.
*/
void expand_text_buffer(text_buffer* //!< Le buffer.
  , int //!< La capcité à ajouter.
);


/**
 * @brief Concatène une chaîne de caractères à la fin d'un buffer.
*/
void append_to_text_buffer(text_buffer* //!< Le buffer.
  , char* //!< La chaîne de caractères.
);

/**
 * @brief Suppression du dernier caractère dans un buffer.
*/
void remove_char(text_buffer* //!< Le buffer.
);

#endif // TEXTBUF_H_
