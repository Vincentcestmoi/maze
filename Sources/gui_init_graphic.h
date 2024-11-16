/**
 * @file gui_init_graphic.h
 * @brief
 * Interface: initialisation de l'affichage. Ne PAS modifier.
 *
 * @attention
 * Cette partie est déjà implémentée. Il ne faut PAS la modifier.
 */

#ifndef UIDEFS_H_
#define UIDEFS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "gui_textbuf.h"
#include "gui_gamevars.h"

 /*  ___       _ _   _       _ _           _   _                   _       */
 /* |_ _|_ __ (_) |_(_) __ _| (_)___  __ _| |_(_) ___  _ __     __| | ___  */
 /*  | || '_ \| | __| |/ _` | | / __|/ _` | __| |/ _ \| '_ \   / _` |/ _ \ */
 /*  | || | | | | |_| | (_| | | \__ \ (_| | |_| | (_) | | | | | (_| |  __/ */
 /* |___|_| |_|_|\__|_|\__,_|_|_|___/\__,_|\__|_|\___/|_| |_|  \__,_|\___| */
 /* | ( ) __ _ / _|/ _(_) ___| |__   __ _  __ _  ___                       */
 /* | |/ / _` | |_| |_| |/ __| '_ \ / _` |/ _` |/ _ \                      */
 /* | | | (_| |  _|  _| | (__| | | | (_| | (_| |  __/                      */
 /* |_|  \__,_|_| |_| |_|\___|_| |_|\__,_|\__, |\___|                      */
 /*                                       |___/                            */


 /**************/
 /*+ Couleurs +*/
 /**************/

/**
 * @brief Noir.
 */
static const SDL_Color BLACK = { 0, 0, 0, 255 };

/**
 * @brief Blanc.
 */
static const SDL_Color WHITE = { 255, 255, 255, 255 };

/**
 * @brief Rouge pour les chemins.
 */
static const SDL_Color REDT = { 255, 43, 31, 200 };

/**
 * @brief Vert pour les chemins.
 */
static const SDL_Color GREENT = { 16, 253, 56, 200 };

/**
 * @brief Bleu pour les chemins.
 */
static const SDL_Color BLUET = { 25, 25, 252, 200 };

/**
 * @brief Orange pour les parcours.
 */
static const SDL_Color ORANGET = { 239, 216, 7, 200 };

/**
 * @brief Chemins complexes.
 */
static const SDL_Color CARML = { 255,0,56, 255 };

/**
 * @brief Chemins complexes.
 */
static const SDL_Color CARMT = { 250,235,215, 200 };

#define COLOR_TUPLE(color) ((color).r), ((color).g), ((color).b), ((color).a)

/***********/
/*+ Fonts +*/
/***********/

extern TTF_Font* Sans18, * Sans16, * Paci20;

/**
 * @brief Chargement des fonts.
 */
void init_fonts(void);

/*************/
/*+ Sprites +*/
/*************/

/**
 * @brief Initialisation du module pour charger les PNGs.
 */
void init_image(void);

extern SDL_Texture* txt_icons;
extern SDL_Texture* txt_frames;
extern SDL_Texture* txt_walls;
extern SDL_Texture* txt_floor;
extern SDL_Texture* txt_player;
extern SDL_Texture* txt_minotaur;
extern SDL_Texture* txt_objects;
extern SDL_Texture* txt_bomb;
extern SDL_Texture* txt_explosion;
extern SDL_Texture* txt_lightning;
extern SDL_Texture* txt_impact;

/**
 * @brief Noms internes des coordonnées des textures.
 */
typedef enum {
    // Icones
    CLP_ARRN,
    CLP_ARRE,
    CLP_ARRS,
    CLP_ARRW,
    CLP_STOP,
    CLP_UNDO,
    CLP_QUES,
    CLP_TOON,

    // UI
    CLP_TBOXNW,
    CLP_TBOXN,
    CLP_TBOXNE,
    CLP_TBOXW,
    CLP_TBOXC,
    CLP_TBOXE,
    CLP_TBOXSW,
    CLP_TBOXS,
    CLP_TBOXSE,

    CLP_TTITW,
    CLP_TTITC,
    CLP_TTITE,
    CLP_TTITWB,
    CLP_TTITEB,


    CLP_PUSHOFF,
    CLP_PUSHON,
    CLP_GPUSHOFF,
    CLP_GPUSHON,
    CLP_RPUSHOFF,
    CLP_RPUSHON,
    CLP_KPUSHOFF,
    CLP_KPUSHON,

    CLP_CIRCLE,


    CLP_MOVEON,
    CLP_MOVEOFF,

    CLP_LEFTD,
    CLP_RIGHTD,
    CLP_BELOWD,

    CLP_EDIT,



    CLP_PLUS,
    CLP_MINUS,

    CLP_TUBEL,
    CLP_TUBEC,
    CLP_TUBER,

    CLP_FTUBEL,
    CLP_FTUBEC,
    CLP_FTUBER,


    CLP_BIGL,
    CLP_BIGC,
    CLP_BIGR,

    CLP_BIGBLON,
    CLP_BIGBCON,
    CLP_BIGBRON,
    CLP_BIGBLOFF,
    CLP_BIGBCOFF,
    CLP_BIGBROFF,

    // Les murs

    CLP_LANGLE,
    CLP_RANGLE,
    CLP_LWALL,
    CLP_RWALL,
    CLP_HWALL1,
    CLP_HWALL2,
    CLP_HWALL3,

    // Le sol

    CLP_FLOOR1,
    CLP_FLOOR2,
    CLP_FLOOR3,
    CLP_FLOOR4,

    // Les toons

    CLP_NWALK1,
    CLP_NWALK2,
    CLP_NWALK3,
    CLP_NWALK4,
    CLP_NWALK5,
    CLP_NWALK6,
    CLP_NWALK7,
    CLP_NWALK8,
    CLP_NWALK9,

    CLP_EWALK1,
    CLP_EWALK2,
    CLP_EWALK3,
    CLP_EWALK4,
    CLP_EWALK5,
    CLP_EWALK6,
    CLP_EWALK7,
    CLP_EWALK8,
    CLP_EWALK9,

    CLP_SWALK1,
    CLP_SWALK2,
    CLP_SWALK3,
    CLP_SWALK4,
    CLP_SWALK5,
    CLP_SWALK6,
    CLP_SWALK7,
    CLP_SWALK8,
    CLP_SWALK9,

    CLP_WWALK1,
    CLP_WWALK2,
    CLP_WWALK3,
    CLP_WWALK4,
    CLP_WWALK5,
    CLP_WWALK6,
    CLP_WWALK7,
    CLP_WWALK8,
    CLP_WWALK9,

    CLP_WIN,

    CLP_DEAD,

    CLP_NSWING1,
    CLP_NSWING2,
    CLP_NSWING3,
    CLP_NSWING4,
    CLP_NSWING5,
    CLP_NSWING6,

    CLP_ESWING1,
    CLP_ESWING2,
    CLP_ESWING3,
    CLP_ESWING4,
    CLP_ESWING5,
    CLP_ESWING6,

    CLP_SSWING1,
    CLP_SSWING2,
    CLP_SSWING3,
    CLP_SSWING4,
    CLP_SSWING5,
    CLP_SSWING6,

    CLP_WSWING1,
    CLP_WSWING2,
    CLP_WSWING3,
    CLP_WSWING4,
    CLP_WSWING5,
    CLP_WSWING6,

    // Les objets

    CLP_BLOOD,
    CLP_TREA1,
    CLP_TREA2,
    CLP_TREA3,
    CLP_BOMBS,
    CLP_EXITS,
    CLP_SLASH,
    CLP_ASHES,
    CLP_POLYS,
    CLP_FILLS,

    // Explosion

    CLP_EXPLO1,
    CLP_EXPLO2,
    CLP_EXPLO3,
    CLP_EXPLO4,
    CLP_EXPLO5,
    CLP_EXPLO6,
    CLP_EXPLO7,

    // Eclair

    CLP_LIGHT1,
    CLP_LIGHT2,
    CLP_LIGHT3,
    CLP_LIGHT4,
    CLP_LIGHT5,
    CLP_LIGHT6,

    // IMPACT

    CLP_IMPACT1,
    CLP_IMPACT2,
    CLP_IMPACT3,
    CLP_IMPACT4,
    CLP_IMPACT5,
    CLP_IMPACT6,


    // La taille du tableau des clips
    CLP_SIZE,
} textureclips;
extern SDL_Rect clp_textures[CLP_SIZE];


/**
 * @brief Initialisation des sprites.
 */
void init_load_sprites(void);

/*****************/
/*+ Fenêtre SDL +*/
/*****************/

extern SDL_Window* window;
extern SDL_Renderer* renderer;

/**
 * @brief Initialisation de la fenêtre SDL.
 */
bool init_SDL(short //!< Largeur.
    , short //!< Hauteur.
);

/**
 * @brief Fermeture de SDL.
 */
void shutdown_SDL(void);






#endif // UIDEFS_H_
