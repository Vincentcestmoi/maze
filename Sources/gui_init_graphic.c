#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "gui_init_graphic.h"


/***********/
/*+ Fonts +*/
/***********/

TTF_Font* Sans18, * Sans16, * Paci20;
void init_fonts(void) {
  if (TTF_Init() < 0) {
    FATAL("TTF_Init: %s\n", TTF_GetError());
  }

  if (!(Paci20 = TTF_OpenFont("../Fonts/Pacifico.ttf", 20))) {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }

  if (!(Sans18 = TTF_OpenFont("../Fonts/Arial_Rounded_Bold.ttf", 18))) {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }

  if (!(Sans16 = TTF_OpenFont("../Fonts/DejaVuSans.ttf", 16))) {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }
}

/*************/
/*+ Sprites +*/
/*************/

void init_image(void) {
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
  {
    FATAL("IMG_Init: %s\n", IMG_GetError());
  }
}


SDL_Texture* txt_icons;
SDL_Texture* txt_frames;
SDL_Texture* txt_walls;
SDL_Texture* txt_floor;
SDL_Texture* txt_player;
SDL_Texture* txt_minotaur;
SDL_Texture* txt_objects;
SDL_Texture* txt_bomb;
SDL_Texture* txt_explosion;
SDL_Texture* txt_lightning;
SDL_Texture* txt_impact;

SDL_Rect clp_textures[CLP_SIZE];

void init_load_sprites(void) {

  // Variable pour charges les images
  SDL_Surface* image = NULL;

  /****************/
  /*+ Les images **/
  /****************/

  // La texture
  image = IMG_Load("../Sprites/icons.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  //SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 89, 86, 82));
  txt_icons = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  clp_textures[CLP_ARRN].h = 50;
  clp_textures[CLP_ARRN].w = 50;
  clp_textures[CLP_ARRN].x = 0;
  clp_textures[CLP_ARRN].y = 400;
  clp_textures[CLP_ARRE].h = 50;
  clp_textures[CLP_ARRE].w = 50;
  clp_textures[CLP_ARRE].x = 0;
  clp_textures[CLP_ARRE].y = 450;
  clp_textures[CLP_ARRS].h = 50;
  clp_textures[CLP_ARRS].w = 50;
  clp_textures[CLP_ARRS].x = 50;
  clp_textures[CLP_ARRS].y = 50;
  clp_textures[CLP_ARRW].h = 50;
  clp_textures[CLP_ARRW].w = 50;
  clp_textures[CLP_ARRW].x = 50;
  clp_textures[CLP_ARRW].y = 0;


  clp_textures[CLP_STOP].h = 50;
  clp_textures[CLP_STOP].w = 50;
  clp_textures[CLP_STOP].x = 150;
  clp_textures[CLP_STOP].y = 0;

  clp_textures[CLP_UNDO].h = 50;
  clp_textures[CLP_UNDO].w = 50;
  clp_textures[CLP_UNDO].x = 200;
  clp_textures[CLP_UNDO].y = 50;


  clp_textures[CLP_QUES].h = 50;
  clp_textures[CLP_QUES].w = 50;
  clp_textures[CLP_QUES].x = 200;
  clp_textures[CLP_QUES].y = 100;

  clp_textures[CLP_TOON].h = 50;
  clp_textures[CLP_TOON].w = 50;
  clp_textures[CLP_TOON].x = 150;
  clp_textures[CLP_TOON].y = 50;

  /****************/
  /*+ Les cadres **/
  /****************/

  // La texture
  image = IMG_Load("../Sprites/ui.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  //SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 89, 86, 82));
  txt_frames = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  // Le cadre d'un boite (9 tiles  32x32)
  for (int i = CLP_TBOXNW; i <= CLP_TBOXSE; i++) {
    clp_textures[i].h = 32;
    clp_textures[i].w = 32;
  }

  clp_textures[CLP_TBOXNW].x = 16;
  clp_textures[CLP_TBOXNW].y = 40;
  clp_textures[CLP_TBOXN].x = 49;
  clp_textures[CLP_TBOXN].y = 40;
  clp_textures[CLP_TBOXNE].x = 82;
  clp_textures[CLP_TBOXNE].y = 40;

  clp_textures[CLP_TBOXW].x = 478;
  clp_textures[CLP_TBOXW].y = 24;
  clp_textures[CLP_TBOXC].x = clp_textures[CLP_TBOXW].x + 33;
  clp_textures[CLP_TBOXC].y = 24;
  clp_textures[CLP_TBOXE].x = clp_textures[CLP_TBOXC].x + 33;
  clp_textures[CLP_TBOXE].y = 24;

  clp_textures[CLP_TBOXSW].x = 478;
  clp_textures[CLP_TBOXSW].y = 80;
  clp_textures[CLP_TBOXS].x = clp_textures[CLP_TBOXSW].x + 33;
  clp_textures[CLP_TBOXS].y = 80;
  clp_textures[CLP_TBOXSE].x = clp_textures[CLP_TBOXS].x + 33;
  clp_textures[CLP_TBOXSE].y = 80;


  // Titres (avec et sans boutons)

  clp_textures[CLP_TTITW].h = 24;
  clp_textures[CLP_TTITW].w = 24;
  clp_textures[CLP_TTITW].x = 634;
  clp_textures[CLP_TTITW].y = 118;

  clp_textures[CLP_TTITC].h = 24;
  clp_textures[CLP_TTITC].w = 23;
  clp_textures[CLP_TTITC].x = clp_textures[CLP_TTITW].x + 25;
  clp_textures[CLP_TTITC].y = 118;

  clp_textures[CLP_TTITE].h = 24;
  clp_textures[CLP_TTITE].w = 25;
  clp_textures[CLP_TTITE].x = clp_textures[CLP_TTITC].x + 24;
  clp_textures[CLP_TTITE].y = 118;


  clp_textures[CLP_TTITWB].h = 24;
  clp_textures[CLP_TTITWB].w = 46;
  clp_textures[CLP_TTITWB].x = 709;
  clp_textures[CLP_TTITWB].y = 143;

  clp_textures[CLP_TTITEB].h = 24;
  clp_textures[CLP_TTITEB].w = 47;
  clp_textures[CLP_TTITEB].x = clp_textures[CLP_TTITWB].x + 47;
  clp_textures[CLP_TTITEB].y = 143;





  // Boutons bleus et verts

  clp_textures[CLP_PUSHOFF].h = 14;
  clp_textures[CLP_PUSHOFF].w = 14;
  clp_textures[CLP_PUSHOFF].x = 32;
  clp_textures[CLP_PUSHOFF].y = 121;


  clp_textures[CLP_PUSHON].h = 14;
  clp_textures[CLP_PUSHON].w = 14;
  clp_textures[CLP_PUSHON].x = 32;
  clp_textures[CLP_PUSHON].y = 137;


  clp_textures[CLP_GPUSHOFF].h = 14;
  clp_textures[CLP_GPUSHOFF].w = 14;
  clp_textures[CLP_GPUSHOFF].x = 16;
  clp_textures[CLP_GPUSHOFF].y = 121;


  clp_textures[CLP_GPUSHON].h = 14;
  clp_textures[CLP_GPUSHON].w = 14;
  clp_textures[CLP_GPUSHON].x = 16;
  clp_textures[CLP_GPUSHON].y = 137;


  clp_textures[CLP_RPUSHOFF].h = 14;
  clp_textures[CLP_RPUSHOFF].w = 14;
  clp_textures[CLP_RPUSHOFF].x = 48;
  clp_textures[CLP_RPUSHOFF].y = 121;


  clp_textures[CLP_RPUSHON].h = 14;
  clp_textures[CLP_RPUSHON].w = 14;
  clp_textures[CLP_RPUSHON].x = 48;
  clp_textures[CLP_RPUSHON].y = 137;


  clp_textures[CLP_KPUSHOFF].h = 14;
  clp_textures[CLP_KPUSHOFF].w = 14;
  clp_textures[CLP_KPUSHOFF].x = 64;
  clp_textures[CLP_KPUSHOFF].y = 121;


  clp_textures[CLP_KPUSHON].h = 14;
  clp_textures[CLP_KPUSHON].w = 14;
  clp_textures[CLP_KPUSHON].x = 64;
  clp_textures[CLP_KPUSHON].y = 137;


  clp_textures[CLP_CIRCLE].h = 24;
  clp_textures[CLP_CIRCLE].w = 24;
  clp_textures[CLP_CIRCLE].x = 762;
  clp_textures[CLP_CIRCLE].y = 86;

  // Gros boutons 

  clp_textures[CLP_MOVEON].h = 44;
  clp_textures[CLP_MOVEON].w = 44;
  clp_textures[CLP_MOVEON].x = 802;
  clp_textures[CLP_MOVEON].y = 431;

  clp_textures[CLP_MOVEOFF].h = 44;
  clp_textures[CLP_MOVEOFF].w = 44;
  clp_textures[CLP_MOVEOFF].x = 802;
  clp_textures[CLP_MOVEOFF].y = 481;

  // Directions + et -


  clp_textures[CLP_LEFTD].h = 14;
  clp_textures[CLP_LEFTD].w = 14;
  clp_textures[CLP_LEFTD].x = 113;
  clp_textures[CLP_LEFTD].y = 121;

  clp_textures[CLP_RIGHTD].h = 14;
  clp_textures[CLP_RIGHTD].w = 14;
  clp_textures[CLP_RIGHTD].x = 129;
  clp_textures[CLP_RIGHTD].y = 121;

  clp_textures[CLP_BELOWD].h = 14;
  clp_textures[CLP_BELOWD].w = 14;
  clp_textures[CLP_BELOWD].x = 129;
  clp_textures[CLP_BELOWD].y = 137;

  clp_textures[CLP_EDIT].h = 14;
  clp_textures[CLP_EDIT].w = 14;
  clp_textures[CLP_EDIT].x = 81;
  clp_textures[CLP_EDIT].y = 105;

  clp_textures[CLP_PLUS].h = 14;
  clp_textures[CLP_PLUS].w = 14;
  clp_textures[CLP_PLUS].x = 81;
  clp_textures[CLP_PLUS].y = 121;

  clp_textures[CLP_MINUS].h = 14;
  clp_textures[CLP_MINUS].w = 14;
  clp_textures[CLP_MINUS].x = 97;
  clp_textures[CLP_MINUS].y = 121;

  // Tubes

  clp_textures[CLP_TUBEL].h = 24;
  clp_textures[CLP_TUBEL].w = 34;
  clp_textures[CLP_TUBEL].x = 259;
  clp_textures[CLP_TUBEL].y = 88;

  clp_textures[CLP_TUBEC].h = 24;
  clp_textures[CLP_TUBEC].w = 23;
  clp_textures[CLP_TUBEC].x = 284;
  clp_textures[CLP_TUBEC].y = 63;

  clp_textures[CLP_TUBER].h = 24;
  clp_textures[CLP_TUBER].w = 34;
  clp_textures[CLP_TUBER].x = 294;
  clp_textures[CLP_TUBER].y = 88;


  clp_textures[CLP_FTUBEL].h = 14;
  clp_textures[CLP_FTUBEL].w = 8;
  clp_textures[CLP_FTUBEL].x = 428;
  clp_textures[CLP_FTUBEL].y = 72;

  clp_textures[CLP_FTUBEC].h = 14;
  clp_textures[CLP_FTUBEC].w = 7;
  clp_textures[CLP_FTUBEC].x = 437;
  clp_textures[CLP_FTUBEC].y = 72;

  clp_textures[CLP_FTUBER].h = 14;
  clp_textures[CLP_FTUBER].w = 8;
  clp_textures[CLP_FTUBER].x = 445;
  clp_textures[CLP_FTUBER].y = 72;



  clp_textures[CLP_BIGL].h = 64;
  clp_textures[CLP_BIGL].w = 32;
  clp_textures[CLP_BIGL].x = 16;
  clp_textures[CLP_BIGL].y = 184;

  clp_textures[CLP_BIGC].h = 64;
  clp_textures[CLP_BIGC].w = 30;
  clp_textures[CLP_BIGC].x = 49;
  clp_textures[CLP_BIGC].y = 184;

  clp_textures[CLP_BIGR].h = 64;
  clp_textures[CLP_BIGR].w = 32;
  clp_textures[CLP_BIGR].x = 82;
  clp_textures[CLP_BIGR].y = 184;


  clp_textures[CLP_BIGBLOFF].h = 24;
  clp_textures[CLP_BIGBLOFF].w = 40;
  clp_textures[CLP_BIGBLOFF].x = 634;
  clp_textures[CLP_BIGBLOFF].y = 25;

  clp_textures[CLP_BIGBCOFF].h = 24;
  clp_textures[CLP_BIGBCOFF].w = 40;
  clp_textures[CLP_BIGBCOFF].x = 675;
  clp_textures[CLP_BIGBCOFF].y = 25;

  clp_textures[CLP_BIGBROFF].h = 24;
  clp_textures[CLP_BIGBROFF].w = 40;
  clp_textures[CLP_BIGBROFF].x = 716;
  clp_textures[CLP_BIGBROFF].y = 25;


  clp_textures[CLP_BIGBLON].h = 24;
  clp_textures[CLP_BIGBLON].w = 40;
  clp_textures[CLP_BIGBLON].x = 634;
  clp_textures[CLP_BIGBLON].y = 85;

  clp_textures[CLP_BIGBCON].h = 24;
  clp_textures[CLP_BIGBCON].w = 40;
  clp_textures[CLP_BIGBCON].x = 675;
  clp_textures[CLP_BIGBCON].y = 85;

  clp_textures[CLP_BIGBRON].h = 24;
  clp_textures[CLP_BIGBRON].w = 40;
  clp_textures[CLP_BIGBRON].x = 716;
  clp_textures[CLP_BIGBRON].y = 85;








  /************/
  /* Les murs */
  /************/

  // La texture
  image = IMG_Load("../Sprites/walls.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 184, 184, 184));
  txt_walls = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  // Les clips
  clp_textures[CLP_LANGLE].x = 0;
  clp_textures[CLP_LANGLE].y = 0 + 5 * 16;
  clp_textures[CLP_LANGLE].h = 16;
  clp_textures[CLP_LANGLE].w = 16;

  clp_textures[CLP_RANGLE].x = 32;
  clp_textures[CLP_RANGLE].y = 0 + 5 * 16;
  clp_textures[CLP_RANGLE].h = 16;
  clp_textures[CLP_RANGLE].w = 16;

  clp_textures[CLP_LWALL].x = 0;
  clp_textures[CLP_LWALL].y = 16 + 5 * 16;
  clp_textures[CLP_LWALL].h = 16;
  clp_textures[CLP_LWALL].w = 16;

  clp_textures[CLP_RWALL].x = 32;
  clp_textures[CLP_RWALL].y = 16 + 5 * 16;
  clp_textures[CLP_RWALL].h = 16;
  clp_textures[CLP_RWALL].w = 16;

  for (int i = CLP_HWALL1; i <= CLP_HWALL3;i++) {
    clp_textures[i].h = 16;
    clp_textures[i].w = 16;
  }

  // Mur par défaut
  clp_textures[CLP_HWALL1].x = 16;
  clp_textures[CLP_HWALL1].y = 32 + 5 * 16;;

  // Mur alternatif 1
  clp_textures[CLP_HWALL2].x = 32;
  clp_textures[CLP_HWALL2].y = 32 + 5 * 16;;

  // Mur alternatif 2
  clp_textures[CLP_HWALL3].x = 48;
  clp_textures[CLP_HWALL3].y = 16 + 5 * 16;;





  /**********/
  /* Le sol */
  /**********/



  // La texture
  image = IMG_Load("../Sprites/cobbles.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  txt_floor = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);


  // Le tableau des clips




  for (int i = 0;i < 4; i++) {
    clp_textures[CLP_FLOOR1 + i].x = i * 16;
    clp_textures[CLP_FLOOR1 + i].y = 0 + 5 * 16;
    clp_textures[CLP_FLOOR1 + i].h = 16;
    clp_textures[CLP_FLOOR1 + i].w = 16;
  }
  clp_textures[CLP_FLOOR4].x = 32;
  clp_textures[CLP_FLOOR4].y = 16 + 5 * 16;

  /*******************************/
    /* Le joueur et les minotaures */
    /*******************************/
  image = IMG_Load("../Sprites/player.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  txt_player = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);


  // Sprites du minotaure
  image = IMG_Load("../Sprites/minotaur.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  txt_minotaur = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);



  int p = 0;
  for (int i = 0; i < 9;i++) {
    clp_textures[CLP_NWALK1 + i].h = 64;
    clp_textures[CLP_NWALK1 + i].w = 64;
    clp_textures[CLP_NWALK1 + i].x = p;
    clp_textures[CLP_NWALK1 + i].y = 512;


    clp_textures[CLP_EWALK1 + i].h = 64;
    clp_textures[CLP_EWALK1 + i].w = 64;
    clp_textures[CLP_EWALK1 + i].x = p;
    clp_textures[CLP_EWALK1 + i].y = 704;

    clp_textures[CLP_SWALK1 + i].h = 64;
    clp_textures[CLP_SWALK1 + i].w = 64;
    clp_textures[CLP_SWALK1 + i].x = p;
    clp_textures[CLP_SWALK1 + i].y = 640;


    clp_textures[CLP_WWALK1 + i].h = 64;
    clp_textures[CLP_WWALK1 + i].w = 64;
    clp_textures[CLP_WWALK1 + i].x = p;
    clp_textures[CLP_WWALK1 + i].y = 576;

    p += 64;
  }

  clp_textures[CLP_WIN].h = 64;
  clp_textures[CLP_WIN].w = 64;
  clp_textures[CLP_WIN].x = 64;
  clp_textures[CLP_WIN].y = 640 + 4 * 64;

  clp_textures[CLP_DEAD].h = 64;
  clp_textures[CLP_DEAD].w = 64;
  clp_textures[CLP_DEAD].x = 5 * 64;
  clp_textures[CLP_DEAD].y = 1280;

  p = 0;

  for (int i = 0; i < 6;i++) {
    clp_textures[CLP_NSWING1 + i].h = 192;
    clp_textures[CLP_NSWING1 + i].w = 192;
    clp_textures[CLP_NSWING1 + i].x = p;
    clp_textures[CLP_NSWING1 + i].y = 2880 + 64;

    clp_textures[CLP_ESWING1 + i].h = 192;
    clp_textures[CLP_ESWING1 + i].w = 192;
    clp_textures[CLP_ESWING1 + i].x = p;
    clp_textures[CLP_ESWING1 + i].y = 3468 + 64;

    clp_textures[CLP_SSWING1 + i].h = 192;
    clp_textures[CLP_SSWING1 + i].w = 192;
    clp_textures[CLP_SSWING1 + i].x = p;
    clp_textures[CLP_SSWING1 + i].y = 3272 + 64;

    clp_textures[CLP_WSWING1 + i].h = 192;
    clp_textures[CLP_WSWING1 + i].w = 192;
    clp_textures[CLP_WSWING1 + i].x = p;
    clp_textures[CLP_WSWING1 + i].y = 3076 + 64;


    p += 192;
  }


  /**************/
  /* Les objets */
  /**************/
  image = IMG_Load("../Sprites/objects.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  txt_objects = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  image = IMG_Load("../Sprites/bomb.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  txt_bomb = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  clp_textures[CLP_BLOOD].w = 32;
  clp_textures[CLP_BLOOD].h = 32;
  clp_textures[CLP_BLOOD].x = 14 * 32;
  clp_textures[CLP_BLOOD].y = 51 * 32;

  clp_textures[CLP_TREA1].w = 32;
  clp_textures[CLP_TREA1].h = 32;
  clp_textures[CLP_TREA2].w = 32;
  clp_textures[CLP_TREA2].h = 32;
  clp_textures[CLP_TREA3].w = 32;
  clp_textures[CLP_TREA3].h = 32;

  clp_textures[CLP_TREA1].x = 42 * 32;
  clp_textures[CLP_TREA1].y = 40 * 32;
  clp_textures[CLP_TREA2].x = 47 * 32;
  clp_textures[CLP_TREA2].y = 40 * 32;
  clp_textures[CLP_TREA3].x = 29 * 32;
  clp_textures[CLP_TREA3].y = 1 * 32;



  clp_textures[CLP_BOMBS].w = 32;
  clp_textures[CLP_BOMBS].h = 32;
  clp_textures[CLP_BOMBS].x = 0 * 32;
  clp_textures[CLP_BOMBS].y = 4 * 32;


  clp_textures[CLP_EXITS].w = 32;
  clp_textures[CLP_EXITS].h = 32;
  clp_textures[CLP_EXITS].x = 54 * 32;
  clp_textures[CLP_EXITS].y = 11 * 32;

  clp_textures[CLP_SLASH].w = 32;
  clp_textures[CLP_SLASH].h = 32;
  clp_textures[CLP_SLASH].x = 9 * 32;
  clp_textures[CLP_SLASH].y = 52 * 32;


  clp_textures[CLP_ASHES].w = 32;
  clp_textures[CLP_ASHES].h = 32;
  clp_textures[CLP_ASHES].x = 40 * 32;
  clp_textures[CLP_ASHES].y = 25 * 32;

  clp_textures[CLP_POLYS].w = 32;
  clp_textures[CLP_POLYS].h = 32;
  clp_textures[CLP_POLYS].x = 50 * 32;
  clp_textures[CLP_POLYS].y = 43 * 32;

  clp_textures[CLP_FILLS].w = 32;
  clp_textures[CLP_FILLS].h = 32;
  clp_textures[CLP_FILLS].x = 26 * 32;
  clp_textures[CLP_FILLS].y = 18 * 32;

  /**************/
  /* Explosion */
  /**************/
  image = IMG_Load("../Sprites/explosion_96.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  txt_explosion = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  for (int i = CLP_EXPLO1;i <= CLP_EXPLO7; i++) {
    clp_textures[i].x = (i % 8) * 96;
    clp_textures[i].y = 0;
    clp_textures[i].h = 96;
    clp_textures[i].w = 96;
  }

  /**************/
  /* Eclair */
  /**************/
  image = IMG_Load("../Sprites/lightning.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  txt_lightning = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  for (int i = 0;i < 6; i++) {
    clp_textures[CLP_LIGHT1 + i].x = (i % 6) * 100;
    clp_textures[CLP_LIGHT1 + i].y = 175;
    clp_textures[CLP_LIGHT1 + i].h = 175;
    clp_textures[CLP_LIGHT1 + i].w = 100;
  }



  image = IMG_Load("../Sprites/impact.png");
  if (!image)
  {
    printf("Erreur de chargement des sprites : %s", IMG_GetError());
    return;
  }
  txt_impact = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  for (int i = 0;i < 6; i++) {
    clp_textures[CLP_IMPACT1 + i].x = (i % 5) * 192;
    clp_textures[CLP_IMPACT1 + i].y = (i / 5) * 192;
    clp_textures[CLP_IMPACT1 + i].h = 192;
    clp_textures[CLP_IMPACT1 + i].w = 192;
  }


}


/*****************/
/*+ Fenêtre SDL +*/
/*****************/

SDL_Window* window;
SDL_Renderer* renderer;

bool init_SDL(short width, short height) {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    FATAL("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
  }

  // Chargement des fonts
  init_fonts();

  // Initialisation des PNGs
  init_image();

  // Chargement des masques
  var_init_maskstab();

  // Création de la fenêtre
  window = SDL_CreateWindow("Les catacombes du bâtiment A22",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    width, height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |
    SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    FATAL("Could not create window: %s\n", SDL_GetError());
  }

  // Création de l'affichage
  renderer = SDL_CreateRenderer(window, -1,
    SDL_RENDERER_ACCELERATED |
    SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    FATAL("Could not create renderer: %s\n", SDL_GetError());
  }

  // Initialisation des sprites
  init_load_sprites();

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(BLACK));
  SDL_RenderClear(renderer);

  return true;
}

void shutdown_SDL(void) {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }

  if (window) {
    SDL_DestroyWindow(window);
  }
  var_free_maskstab();
  SDL_Quit();
}


