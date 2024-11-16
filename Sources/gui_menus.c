#include "gui_menus.h"

/************************/
/*+ Affichage de texte **/
/************************/

char message[512];

// Affichage de texte
int render_text(int x, int y, const char* text, TTF_Font* font) {
    //SDL_Color black = { 0, 0, 0, 0 };
    SDL_Surface* surf_text = TTF_RenderUTF8_Blended(font, text, WHITE);
    SDL_Texture* text_text = SDL_CreateTextureFromSurface(renderer, surf_text);
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    pos.h = surf_text->h;
    pos.w = surf_text->w;
    SDL_RenderCopy(renderer, text_text, NULL, &pos);
    SDL_FreeSurface(surf_text);
    SDL_DestroyTexture(text_text);
    return pos.w;
}
int render_text_centered(int x, int y, const char* text, TTF_Font* font) {
    //SDL_Color black = { 0, 0, 0, 0 };
    SDL_Surface* surf_text = TTF_RenderUTF8_Blended(font, text, WHITE);
    SDL_Texture* text_text = SDL_CreateTextureFromSurface(renderer, surf_text);
    SDL_Rect pos;
    pos.x = x - surf_text->w / 2;
    pos.y = y;
    pos.h = surf_text->h;
    pos.w = surf_text->w;
    SDL_RenderCopy(renderer, text_text, NULL, &pos);
    SDL_FreeSurface(surf_text);
    SDL_DestroyTexture(text_text);
    return pos.w;
}

void print_text_buffer_center(void) {
    if (SDL_GetTicks() % 800 <= 400) {
        sprintf(message, "%s_", var_editbuffer->content);
    }
    else {
        sprintf(message, "%s  ", var_editbuffer->content);
    }
}

/****************************/
/*+ Affichages des boutons **/
/****************************/


void render_bomb_button(int x, int y, bool on) {
    SDL_Rect pos;
    pos.h = 44;
    pos.w = 44;
    pos.x = x - 22;
    pos.y = y - 22;
    if (on) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEOFF], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEON], &pos);
    }
    pos.x = x - 18;
    pos.y = y - 18;
    pos.h = 36;
    pos.w = 36;
    SDL_RenderCopy(renderer, txt_bomb, &clp_textures[CLP_BOMBS], &pos);
}


void render_poly_button(int x, int y, bool on) {
    SDL_Rect pos;
    pos.h = 44;
    pos.w = 44;
    pos.x = x - 22;
    pos.y = y - 22;
    if (on) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEOFF], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEON], &pos);
    }
    pos.x = x - 12;
    pos.y = y - 12;
    pos.h = 24;
    pos.w = 24;
    SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_POLYS], &pos);
}

void render_exit_button(int x, int y, bool on) {
    SDL_Rect pos;
    pos.h = 44;
    pos.w = 44;
    pos.x = x - 22;
    pos.y = y - 22;
    if (on) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEOFF], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEON], &pos);
    }
    pos.x = x - 12;
    pos.y = y - 12;
    pos.h = 24;
    pos.w = 24;
    SDL_RenderCopy(renderer, txt_objects, &clp_textures[CLP_EXITS], &pos);
}

void render_undo_button(int x, int y, bool on) {
    SDL_Rect pos;
    pos.h = 44;
    pos.w = 44;
    pos.x = x - 22;
    pos.y = y - 22;
    if (on) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEOFF], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEON], &pos);
    }
    pos.x = x - 18;
    pos.y = y - 18;
    pos.h = 36;
    pos.w = 36;
    SDL_RenderCopy(renderer, txt_icons, &clp_textures[CLP_UNDO], &pos);
}

void render_redo_button(int x, int y, bool on) {
    SDL_Rect pos;
    pos.h = 44;
    pos.w = 44;
    pos.x = x - 22;
    pos.y = y - 22;
    if (on) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEOFF], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEON], &pos);
    }
    pos.x = x - 18;
    pos.y = y - 18;
    pos.h = 36;
    pos.w = 36;
    SDL_RenderCopyEx(renderer, txt_icons, &clp_textures[CLP_UNDO], &pos, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void render_help_button(int x, int y, bool on) {
    SDL_Rect pos;
    pos.h = 44;
    pos.w = 44;
    pos.x = x - 22;
    pos.y = y - 22;
    if (on) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEOFF], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEON], &pos);
    }
    pos.x = x - 18;
    pos.y = y - 18;
    pos.h = 36;
    pos.w = 36;
    SDL_RenderCopy(renderer, txt_icons, &clp_textures[CLP_QUES], &pos);
}


void render_came_button(int x, int y, bool on) {
    SDL_Rect pos;
    pos.h = 44;
    pos.w = 44;
    pos.x = x - 22;
    pos.y = y - 22;
    if (on) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEOFF], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEON], &pos);
    }
    pos.x = x - 18;
    pos.y = y - 18;
    pos.h = 36;
    pos.w = 36;
    SDL_RenderCopy(renderer, txt_icons, &clp_textures[CLP_TOON], &pos);
}

void render_move_button(int x, int y, bool on, move dir) {
    SDL_Rect pos;
    pos.h = 44;
    pos.w = 44;
    pos.x = x - 22;
    pos.y = y - 22;
    if (on) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEOFF], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MOVEON], &pos);
    }

    pos.x = x - 18;
    pos.y = y - 18;
    pos.h = 36;
    pos.w = 36;
    switch (dir)
    {
    case M_NORTH:
        SDL_RenderCopy(renderer, txt_icons, &clp_textures[CLP_ARRN], &pos);
        break;
    case M_EAST:
        SDL_RenderCopy(renderer, txt_icons, &clp_textures[CLP_ARRE], &pos);
        break;
    case M_SOUTH:
        SDL_RenderCopy(renderer, txt_icons, &clp_textures[CLP_ARRS], &pos);
        break;
    case M_WEST:
        SDL_RenderCopy(renderer, txt_icons, &clp_textures[CLP_ARRW], &pos);
        break;
    case M_WAIT:
        SDL_RenderCopy(renderer, txt_icons, &clp_textures[CLP_STOP], &pos);
        break;

    default:
        break;
    }




}

/***************************/
/*+ Affichages des cadres **/
/***************************/



void render_scale_tube(int x, int y, int width, int fill, int maxfill, unsigned char zoom, bool lon, bool ron) {
    if (fill > maxfill) {
        fprintf(stderr, "Error: too large fill");
        exit(EXIT_FAILURE);
    }

    SDL_Rect pos;
    pos.h = zoom * 24;
    pos.w = zoom * 34;
    pos.x = x;
    pos.y = y;

    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TUBEL], &pos);
    pos.x += zoom * 34;

    if (width > 2 * zoom * 34) {
        pos.w = width - 2 * zoom * 34;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TUBEC], &pos);
        pos.x += pos.w;
        pos.w = zoom * 34;
    }
    else {
        width = 2 * zoom * 34;
    }
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TUBER], &pos);


    pos.h = zoom * 14;
    pos.w = zoom * 8;
    pos.x = x + zoom * 27;
    pos.y = y + zoom * 5;

    int fillwidth = width - 2 * zoom * 28;

    if (0 < fill && fill < maxfill) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_FTUBEL], &pos);
        pos.x += zoom * 8;
        pos.w = fill * (fillwidth / maxfill) - 2 * zoom * 8;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_FTUBEC], &pos);
        pos.x += pos.w;
        pos.w = zoom * 8;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_FTUBER], &pos);

    }

    if (fill == maxfill) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_FTUBEL], &pos);
        pos.x += zoom * 8;
        if (fillwidth > 2 * zoom * 14) {
            pos.w = fillwidth - zoom * 14;
            SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_FTUBEC], &pos);
            pos.x += pos.w;
            pos.w = zoom * 8;
        }
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_FTUBER], &pos);

    }


    pos.h = zoom * 14;
    pos.w = zoom * 14;
    pos.x = x;
    pos.y = y;

    pos.x += zoom * 5;
    pos.y += zoom * 5;
    if (lon) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHON], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHOFF], &pos);
    }



    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_MINUS], &pos);
    pos.x += fillwidth + zoom * 32;
    if (ron) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHON], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHOFF], &pos);
    }
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PLUS], &pos);



}

void render_selector(int x, int y, int width, const char* title, TTF_Font* thefont, bool lon, bool ron) {


    SDL_Rect pos;
    pos.h = 48;
    pos.w = 48;
    pos.x = x - (width / 2) - 2 * 46;

    pos.y = y;

    // Affichage du cadre

    pos.w = 2 * 46;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TTITWB], &pos);
    pos.x += 2 * 46;
    pos.w = width;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TTITC], &pos);
    pos.x += pos.w;
    pos.w = 2 * 46;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TTITEB], &pos);


    pos.h = 28;
    pos.w = 28;
    pos.x = x - (width / 2) - 2 * 46;

    pos.x += 10;
    pos.y += 10;

    if (lon) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHON], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHOFF], &pos);
    }


    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_LEFTD], &pos);
    pos.x += 3 * 46 + width - 2;
    if (ron) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHON], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHOFF], &pos);
    }
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_RIGHTD], &pos);

    if (thefont == Paci20) {

        render_text_centered(x, y + 2, title, thefont);
    }
    else {
        render_text_centered(x, y + 12, title, thefont);
    }
}

void render_selector_edit(int x, int y, int width, const char* title, TTF_Font* thefont, bool ron) {

    SDL_Rect pos;
    pos.h = 48;
    pos.w = 48;
    pos.x = x - (width / 2) - 48;

    //pos.x = var_info_mid - 72 - 2 * 46;
    //pos.x = var_maze_width + 5 + 256 - 24 - 48 - 2 * 46;
    pos.y = y;

    // Affichage du cadre

    pos.w = 48;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TTITW], &pos);
    pos.x += 48;
    pos.w = width;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TTITC], &pos);
    pos.x += pos.w;
    pos.w = 2 * 46;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TTITEB], &pos);


    pos.h = 28;
    pos.w = 28;
    pos.x = x - (width / 2) - 2 * 46;

    pos.x += 10;
    pos.y += 10;



    pos.x += 3 * 46 + width - 2;
    if (ron) {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHON], &pos);
    }
    else {
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHOFF], &pos);
    }
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_EDIT], &pos);

    if (thefont == Paci20) {

        render_text_centered(x, y + 2, title, thefont);
    }
    else {
        render_text_centered(x, y + 12, title, thefont);
    }
}


void render_color_indicator(int x, int y, int width, const char* title, int color) {

    SDL_Rect pos;
    pos.h = 48;
    pos.w = 48;
    pos.x = x - (width / 2) - 48;

    pos.y = y;


    pos.h = 48;
    pos.w = 48;

    pos.x = x + (width / 2) + 35;

    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_CIRCLE], &pos);

    pos.h = 28;
    pos.w = 28;

    pos.x += 9;
    pos.y += 10;



    switch (color)
    {
    case 1:
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_RPUSHON], &pos);
        break;
    case 2:
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_GPUSHON], &pos);
        break;
    case 3:
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_PUSHON], &pos);
        break;

    default:
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_KPUSHON], &pos);
        break;
    }
    render_text_centered(x, y + 12, title, Sans18);
}





void render_bigbutton(int y, const char* label, TTF_Font* thefont, bool ron) {
    SDL_Rect pos;
    pos.h = 48;
    pos.w = 80;
    pos.x = var_info_mid - 120;

    pos.y = y;

    // Affichage du cadre

    if (ron) {

        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBLON], &pos);
        pos.x += 80;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBCON], &pos);
        pos.x += 80;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBRON], &pos);
    }
    else {

        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBLOFF], &pos);
        pos.x += 80;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBCOFF], &pos);
        pos.x += 80;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBROFF], &pos);
    }


    if (thefont == Paci20) {

        render_text_centered(var_info_mid, y + 2, label, thefont);
    }
    else {
        render_text_centered(var_info_mid, y + 12, label, thefont);
    }
}


void render_smallbutton(int x, int y, int width, const char* label, bool ron) {

    width -= 2 * 60;
    SDL_Rect pos;
    pos.h = 32;
    pos.w = 60;
    pos.x = x - 60;
    if (width > 0) {
        pos.x -= width / 2;
    }

    pos.y = y;

    // Affichage du cadre

    if (ron) {

        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBLON], &pos);
        pos.x += 60;
        if (width > 0) {
            pos.w = width;
            SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBCON], &pos);
            pos.x += pos.w;
            pos.w = 60;
        }

        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBRON], &pos);
    }
    else {

        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBLOFF], &pos);
        pos.x += 60;

        if (width > 0) {
            pos.w = width;
            SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBCOFF], &pos);
            pos.x += pos.w;
            pos.w = 60;
        }
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGBROFF], &pos);
    }


    render_text_centered(x, y + 4, label, Sans18);



}


static void render_boxed_title(int x, int y, const char* title) {
    SDL_Surface* surf_text = TTF_RenderUTF8_Blended(Paci20, title, WHITE);
    SDL_Texture* text_text = SDL_CreateTextureFromSurface(renderer, surf_text);
    SDL_Rect pos;


    int tw = surf_text->w + 2 * 24;
    pos.x = x + 24;
    pos.y = y - 2;
    pos.h = surf_text->h;
    pos.w = surf_text->w;
    SDL_RenderCopy(renderer, text_text, NULL, &pos);
    SDL_FreeSurface(surf_text);
    SDL_DestroyTexture(text_text);


    pos.h = 48;
    pos.w = 48;
    pos.x = x;
    pos.y = y;

    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TTITW], &pos);
    pos.x += 48;

    if (tw > 2 * 48) {
        pos.w = tw - 2 * 48;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TTITC], &pos);
        pos.x += pos.w;
    }
    pos.w = 48;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TTITE], &pos);

    render_text(x + 24, y + 2, title, Paci20);
}

void render_frame(int x, int y, int w, int h, const char* title) {
    SDL_Rect pos;
    pos.h = 64;
    pos.w = 64;
    pos.x = x;
    pos.y = y;
    // Le cadre principal
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXNW], &pos);
    pos.x += pos.w;

    if (w > 128) {
        pos.w = w - 128;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXN], &pos);
        pos.x += pos.w;
    }
    pos.w = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXNE], &pos);

    pos.x = x;
    pos.y += pos.h;
    if (h > 128) {
        pos.h = h - 128;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXW], &pos);
        pos.x += pos.w;
        if (w > 128) {
            pos.w = w - 128;
            SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXC], &pos);
            pos.x += pos.w;
        }
        pos.w = 64;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXE], &pos);
        pos.y += pos.h;
    }

    pos.x = x;
    pos.h = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXSW], &pos);
    pos.x += pos.w;

    if (w > 128) {
        pos.w = w - 128;
        SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXS], &pos);
        pos.x += pos.w;
    }
    pos.w = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXSE], &pos);


    // Le titre

    y -= 20;
    x += 20;

    render_boxed_title(x, y, title);
}

/********************************/
/*+ Affichage des informations **/
/********************************/

void render_gamenull(void) {
    int w = 550 - 128;
    int h = 150 - 128;

    int left = var_maze_width / 2 - (w + 128) / 2;


    SDL_Rect pos;
    pos.h = 64;
    pos.w = 64;
    pos.x = left;
    pos.y = var_screen_height / 2 - 90 - (h + 258) / 2;
    // Le cadre principal
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXNW], &pos);
    pos.x += pos.w;


    pos.w = w;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXN], &pos);
    pos.x += pos.w;

    pos.w = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXNE], &pos);

    pos.x = left;
    pos.y += pos.h;

    pos.h = h;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXW], &pos);
    pos.x += pos.w;
    pos.w = w;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXC], &pos);
    pos.x += pos.w;
    pos.w = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXE], &pos);
    pos.y += pos.h;


    pos.x = left;
    pos.h = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXSW], &pos);
    pos.x += pos.w;

    pos.w = w;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXS], &pos);
    pos.x += pos.w;

    pos.w = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXSE], &pos);

    render_text_centered(var_maze_width / 2, var_screen_height / 2 - 200, "Erreur de génération.", Paci20);
    render_text_centered(var_maze_width / 2, var_screen_height / 2 - 170, "Vous avez des fonctions à écrire!", Paci20);
}


void render_victory(void) {
    int w = 550 - 128;
    int h = 250 - 128;

    int left = var_screen_width / 2 - (w + 128) / 2;


    SDL_Rect pos;
    pos.h = 64;
    pos.w = 64;
    pos.x = left;
    pos.y = var_screen_height / 2 - 90 - (h + 128) / 2;
    // Le cadre principal
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXNW], &pos);
    pos.x += pos.w;


    pos.w = w;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXN], &pos);
    pos.x += pos.w;

    pos.w = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXNE], &pos);

    pos.x = left;
    pos.y += pos.h;

    pos.h = h;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXW], &pos);
    pos.x += pos.w;
    pos.w = w;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXC], &pos);
    pos.x += pos.w;
    pos.w = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXE], &pos);
    pos.y += pos.h;


    pos.x = left;
    pos.h = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXSW], &pos);
    pos.x += pos.w;

    pos.w = w;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXS], &pos);
    pos.x += pos.w;

    pos.w = 64;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_TBOXSE], &pos);

    render_text_centered(var_screen_width / 2, var_screen_height / 2 - 200, "Bravo! Vous avez sorti Jean-Kevin du labyrinthe!", Paci20);
    render_text_centered(var_screen_width / 2, var_screen_height / 2 - 170, "Il va enfin pouvoir retourner en cours d'algorithmique!", Paci20);

    pos.h = 128;
    pos.w = 128;
    pos.x = left + 20;
    pos.y -= 100;

    SDL_RenderCopy(renderer, txt_player, &clp_textures[CLP_WIN], &pos);

    render_text(var_screen_width / 2 - 130, pos.y + 35, "Il est temps de vous remettre à programmer!", Paci20);
    render_text(var_screen_width / 2 - 130, pos.y + 65, "Vous avez un projet à finir!", Paci20);
}










void render_informations(game* thegame) {
    int xbasis = var_maze_width + 5;
    int ybasis = 30;
    // Les informations sur le labyrinthe 
    render_frame(xbasis, ybasis, var_screen_width - var_maze_width - 10, 0, "Informations sur le plateau");
    //  render_frame_title(xbasis, ybasis, 3, );


    int w = 21;

    if (thegame && thegame->m) {

        // Informations sur le labyrinthe
       // Nombre de cellules
        sprintf(message, "Cellules accessibles: %d", thegame->m->nb_reachable);
        render_text(xbasis + w, ybasis + 28, message, Sans18);
        // Nombre de cul-de-sacs
        sprintf(message, "Cul-de-sacs: %d", thegame->nb_deadends);
        render_text(xbasis + w, ybasis + 28 + 22, message, Sans18);
        // Nombre de sorties
        sprintf(message, "Sorties: %d", thegame->exits);
        render_text(xbasis + w, ybasis + 28 + 44, message, Sans18);
        // Nombre de minotaures
        sprintf(message, "Minotaures: %d", thegame->m->nb_minotaurs);
        render_text(xbasis + w, ybasis + 28 + 66, message, Sans18);
    }
    else {
        // Nombre de cellules
        sprintf(message, "Cellules accessibles: N/A");
        render_text(xbasis + w, ybasis + 28, message, Sans18);
        // Nombre de cul-de-sacs
        sprintf(message, "Cul-de-sacs: N/A");
        render_text(xbasis + w, ybasis + 28 + 22, message, Sans18);
        // Nombre de sorties
        sprintf(message, "Sorties: N/A");
        render_text(xbasis + w, ybasis + 28 + 44, message, Sans18);
        // Nombre de minotaures
        sprintf(message, "Minotaures: N/A");

        render_gamenull();
    }

}


void render_play_informations(game* thegame) {
    int xbasis = var_maze_width + 5;
    int ybasis = 180;

    render_selector(var_info_mid, ybasis, 144, "Contrôle du jeu", Paci20, var_buttons_flags[BT_LMODE], var_buttons_flags[BT_RMODE]);

    ybasis += 90;
    render_frame(xbasis, ybasis, var_screen_width - var_maze_width - 10, 0, "Partie en cours");
    //render_frame_title(xbasis, ybasis, 3, "Commandes du joueur");

    int w = 21;
    if (thegame && thegame->m && thegame->log) {
        sprintf(message, "Tours: %d joués %d annulables %d restaurables", thegame->turns, sizeprev_history(thegame->log), sizenext_history(thegame->log));
        render_text(xbasis + w, ybasis + 28, message, Sans18);
    }
    else if (thegame && thegame->m) {
        sprintf(message, "Tours: %d joués (historique non-implémenté)", thegame->turns);
        render_text(xbasis + w, ybasis + 28, message, Sans18);
    }
    else {
        sprintf(message, "Tours: N/A joués (historique non-implémenté)");
        render_text(xbasis + w, ybasis + 28, message, Sans18);
    }

    if (thegame && thegame->m) {

        sprintf(message, "Score: %d", thegame->score);
        render_text(xbasis + w, ybasis + 28 + 22, message, Sans18);
        sprintf(message, "Bombes: %d", thegame->nbombs);
        render_text(xbasis + w, ybasis + 28 + 44, message, Sans18);
        sprintf(message, "Polys d'algo: %d", thegame->npolys);
        render_text(xbasis + w, ybasis + 28 + 66, message, Sans18);
    }
    else {

        sprintf(message, "Score: N/A");
        render_text(xbasis + w, ybasis + 28 + 22, message, Sans18);
        sprintf(message, "Bombes: N/A");
        render_text(xbasis + w, ybasis + 28 + 44, message, Sans18);
        sprintf(message, "Polys d'algo: N/A");
        render_text(xbasis + w, ybasis + 28 + 66, message, Sans18);
    }


    ybasis += 160;
    render_frame(xbasis, ybasis, var_screen_width - var_maze_width - 10, 180, "Paramètres");


    // Jauges vitesse et zoom


    render_text_centered(xbasis + 20 + 115, ybasis + 28, "Vitesse (^/$)", Sans18);
    render_scale_tube(xbasis + 20, ybasis + 50, 230, 8 - var_render_speed_level, 7, 1, var_buttons_flags[BT_LSPEED], var_buttons_flags[BT_RSPEED]);

    render_text_centered(var_screen_width - 250 + 115, ybasis + 28, "Zoom (-/+)", Sans18);
    render_scale_tube(var_screen_width - 250, ybasis + 50, 230, var_render_zoom_level, 5, 1, var_buttons_flags[BT_LZOOM], var_buttons_flags[BT_RZOOM]);

    // Affichage de la stratégie des minotaures
    //sprintf(message, "Stratégie des minotaures: %s", str_names[var_mino_strat]);
    render_text_centered(var_info_mid, ybasis + 90, "Stratégie des minotaures (s)", Sans18);

    sprintf(message, "%s", str_names[var_mino_strat]);


    render_selector(var_info_mid, ybasis + 116, 144, message, Sans18, var_buttons_flags[BT_LSTRAT], var_buttons_flags[BT_RSTRAT]);

    SDL_Rect pos;
    pos.x = var_info_mid - 80;
    pos.y = 635;
    pos.w = 160;
    pos.h = 96;

    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGC], &pos);
    pos.w = 48;
    pos.x -= 48;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGL], &pos);
    pos.x += 160 + 48;
    SDL_RenderCopy(renderer, txt_frames, &clp_textures[CLP_BIGR], &pos);

    render_text_centered(var_info_mid, 660, "Commmandes de jeu", Paci20);

    render_move_button(var_info_mid, 800, var_buttons_flags[BT_UP], M_NORTH);
    render_move_button(var_info_mid, 850, var_buttons_flags[BT_WAIT], M_WAIT);
    render_move_button(var_info_mid, 900, var_buttons_flags[BT_DOWN], M_SOUTH);

    render_move_button(var_info_mid + 50, 850, var_buttons_flags[BT_RIGHT], M_EAST);
    render_move_button(var_info_mid - 50, 850, var_buttons_flags[BT_LEFT], M_WEST);


    render_bomb_button(var_info_mid + 160, 800, var_buttons_flags[BT_BOMB]);
    render_poly_button(var_info_mid + 160, 850, var_buttons_flags[BT_POLY]);
    render_exit_button(var_info_mid + 160, 900, var_buttons_flags[BT_EXIT]);

    render_undo_button(var_info_mid - 160, 800, var_buttons_flags[BT_UNDO]);
    render_redo_button(var_info_mid - 160, 850, var_buttons_flags[BT_REDO]);
    render_came_button(var_info_mid - 160, 900, var_buttons_flags[BT_CAMERA]);
}


void render_gen_informations(game*) {
    int xbasis = var_maze_width + 5;
    int ybasis = 180;

    render_selector(var_info_mid, ybasis, 144, "Génération", Paci20, var_buttons_flags[BT_LMODE], var_buttons_flags[BT_RMODE]);

    ybasis += 90;
    render_frame(xbasis, ybasis, var_screen_width - var_maze_width - 10, 550, "Paramètres");

    // Algo de génération
    render_text_centered(var_info_mid, ybasis + 28, "Algorithme (a)", Sans18);
    sprintf(message, "%s", gen_names[gen_mode]);
    render_selector(var_info_mid, ybasis + 28 + 26, 300, message, Sans18, var_buttons_flags[BT_LALGO], var_buttons_flags[BT_RALGO]);

    // Masque
    render_text_centered(var_info_mid, ybasis + 28 + 86, "Masque (k)", Sans18);
    sprintf(message, "%s", gen_maskname);
    render_selector(var_info_mid, ybasis + 28 + 26 + 86, 300, message, Sans18, var_buttons_flags[BT_LMASK], var_buttons_flags[BT_RMASK]);

    // Algo de génération des trésors
    render_text_centered(var_info_mid, ybasis + 28 + 2 * 86, "Génération des objets (o)", Sans18);
    sprintf(message, "%s", obj_names[obj_mode]);
    render_selector(var_info_mid, ybasis + 28 + 2 * 86 + 26, 300, message, Sans18, var_buttons_flags[BT_LOBJ], var_buttons_flags[BT_ROBJ]);

    // Tressage
    render_text_centered(var_info_mid, ybasis + 28 + 3 * 86, "Tressage (b)", Sans18);
    if (var_edit_mode == ED_BRAID) {
        print_text_buffer_center();
    }
    else {
        sprintf(message, "%d%%", gen_braiding);
    }
    render_selector_edit(var_info_mid, ybasis + 28 + 3 * 86 + 26, 300, message, Sans18, var_buttons_flags[BT_BRAID]);

    // Taille horizontale
    render_text_centered(var_info_mid - 119, ybasis + 28 + 4 * 86, "Taille horizontale (x)", Sans18);
    if (var_edit_mode == ED_X) {
        print_text_buffer_center();
    }
    else {
        sprintf(message, "%d", gen_hsize);
    }
    render_selector_edit(var_info_mid - 119, ybasis + 28 + 4 * 86 + 26, 60, message, Sans18, var_buttons_flags[BT_X]);

    // Taille verticale
    render_text_centered(var_info_mid + 119, ybasis + 28 + 4 * 86, "Taille verticale (y)", Sans18);
    if (var_edit_mode == ED_Y) {
        print_text_buffer_center();
    }
    else {
        sprintf(message, "%d", gen_vsize);
    }
    render_selector_edit(var_info_mid + 119, ybasis + 28 + 4 * 86 + 26, 60, message, Sans18, var_buttons_flags[BT_Y]);

    // Minotaures
    render_text_centered(var_info_mid, ybasis + 28 + 5 * 86, "Nombre de minotaures (r)", Sans18);
    if (var_edit_mode == ED_MINO) {
        print_text_buffer_center();
    }
    else {
        sprintf(message, "%d", gen_nbminotaurs);
    }
    render_selector_edit(var_info_mid, ybasis + 28 + 5 * 86 + 26, 300, message, Sans18, var_buttons_flags[BT_MINO]);


    render_bigbutton(850, "Générer (g)", Paci20, var_buttons_flags[BT_GEN]);
    /*


    sprintf(message, "(z) Nombre de minotaures: %d", gen_nbminotaurs);
    render_text(var_maze_width + 16, 28 * h++, message, Sans18);
    sprintf(message, "(n) Lancer une génération.");
    render_text(var_maze_width + 16, 28 * h++, message, Sans18);
 */

}






void render_sim_informations(game*) {
    int xbasis = var_maze_width + 5;
    int ybasis = 180;



    render_selector(var_info_mid, ybasis, 144, "Solveur - Simple", Paci20, var_buttons_flags[BT_LMODE], var_buttons_flags[BT_RMODE]);

    ybasis += 90;
    render_frame(xbasis, ybasis, var_screen_width - var_maze_width - 10, 374, "Calcul d'un chemin simple");

    // Type de parcours
    render_text_centered((var_maze_width + var_info_mid) / 2 + 6, ybasis + 28, "Algorithme (a)", Sans18);
    render_text_centered((var_screen_width + var_info_mid) / 2 - 6, ybasis + 28, "Recherche (s)", Sans18);
    render_selector((var_maze_width + var_info_mid) / 2 + 6, ybasis + 28 + 26, 50, salgo_names[var_sim_algorithm], Sans18, var_buttons_flags[BT_SIML], var_buttons_flags[BT_SIMR]);
    render_selector((var_screen_width + var_info_mid) / 2 - 6, ybasis + 28 + 26, 50, sgoal_names[var_sim_goal], Sans18, var_buttons_flags[BT_SIMGL], var_buttons_flags[BT_SIMGR]);

    render_smallbutton((var_maze_width + var_info_mid) / 2 + 6, 380, 230, "Minotaures (t)", var_buttons_flags[BT_SIMMINOS]);
    if (var_sim_minos) {
        render_text(var_info_mid + 14, 384, "Pris en compte", Sans18);
    }
    else {
        render_text(var_info_mid + 14, 384, "Ignorés", Sans18);
    }

    if (var_sim_algorithm == ALG_AST) {
        render_text_centered(var_info_mid, 416, "Heuristique (h)", Sans18);
        render_selector(var_info_mid, 442, 300, heu_names[var_heuristic], Sans18, var_buttons_flags[BT_HEUL], var_buttons_flags[BT_HEUR]);
    }




    render_smallbutton((var_maze_width + var_info_mid) / 2 + 6, 498, 230, "Nouveau calcul (c)", var_buttons_flags[BT_SIMCOMP]);
    render_smallbutton((var_screen_width + var_info_mid) / 2 - 6, 498, 230, "Effacer le calcul (e)", var_buttons_flags[BT_SIMDELE]);




    //ybasis = 390;
    ybasis = 508;
    int w = 21;

    if (var_sim_search) {

        sprintf(message, "Parcours enregistré: %s", salgo_names[var_sim_search->algo]);
        render_text(xbasis + w, ybasis + 28, message, Sans18);
        sprintf(message, "Nombre de cellules visitées: %d", size_dyn(var_sim_search->search));
        render_text(xbasis + w, ybasis + 28 + 22, message, Sans18);

        if (var_sim_search->path) {
            sprintf(message, "Chemin trouvé: %s", sgoal_names[var_sim_search->goal]);
            render_text(xbasis + w, ybasis + 28 + 44, message, Sans18);
            sprintf(message, "Longueur: %d", var_sim_search->path->length);
            render_text(xbasis + w, ybasis + 28 + 66, message, Sans18);
        }
        else {
            sprintf(message, "Chemin trouvé: non");
            render_text(xbasis + w, ybasis + 28 + 44, message, Sans18);
        }
    }
    else {
        sprintf(message, "Parcours enregistré: non");
        render_text(xbasis + w, ybasis + 28, message, Sans18);
    }

    ybasis = 680;
    if (true) {
        render_frame(xbasis, ybasis, var_screen_width - var_maze_width - 10, 264, "Affichage");


        render_text_centered(var_info_mid, ybasis + 28, "Affichage d'un chemin: trois couleurs disponibles", Sans18);

        if (var_path_red) {
            render_color_indicator(var_info_mid - 150, ybasis + 28 + 26, 0, "Rouge", 1);
        }
        else {
            render_color_indicator(var_info_mid - 150, ybasis + 28 + 26, 0, "Rouge", 0);
        }
        if (var_path_green) {
            render_color_indicator(var_info_mid, ybasis + 28 + 26, 0, "Vert", 2);
        }
        else {
            render_color_indicator(var_info_mid, ybasis + 28 + 26, 0, "Vert", 0);
        }
        if (var_path_blue) {
            render_color_indicator(var_info_mid + 150, ybasis + 28 + 26, 0, "Bleu", 3);

        }
        else {
            render_color_indicator(var_info_mid + 150, ybasis + 28 + 26, 0, "Bleu", 0);

        }

        if (var_sim_search && var_sim_search->path) {
            render_smallbutton(var_info_mid - 122, ybasis + 28 + 26 + 56, 0, "Affecter", var_buttons_flags[BT_RED1]);
            render_smallbutton(var_info_mid + 28, ybasis + 28 + 26 + 56, 0, "Affecter", var_buttons_flags[BT_GREEN1]);
            render_smallbutton(var_info_mid + 178, ybasis + 28 + 26 + 56, 0, "Affecter", var_buttons_flags[BT_BLUE1]);
        }

        render_smallbutton(var_info_mid - 122, ybasis + 28 + 26 + 96, 0, "Effacer", var_buttons_flags[BT_RED2]);
        render_smallbutton(var_info_mid + 28, ybasis + 28 + 26 + 96, 0, "Effacer", var_buttons_flags[BT_GREEN2]);
        render_smallbutton(var_info_mid + 178, ybasis + 28 + 26 + 96, 0, "Effacer", var_buttons_flags[BT_BLUE2]);


        ybasis += 28 + 36 + 96;
        render_text_centered(var_info_mid, ybasis + 28, "Affichage d'un parcours complet", Sans18);

        if (var_sim_search) {
            render_smallbutton(var_info_mid - 122, ybasis + 28 + 26, 0, "Afficher", var_buttons_flags[BT_DTR]);
            render_smallbutton(var_info_mid + 28, ybasis + 28 + 26, 0, "Animer", var_buttons_flags[BT_ATR]);
        }

        render_smallbutton(var_info_mid + 178, ybasis + 28 + 26, 0, "Effacer", var_buttons_flags[BT_RTR]);
    }
}



void render_cmp_searchs(game*) {
    int xbasis = var_maze_width + 5;
    int ybasis = 180;



    render_selector(var_info_mid, ybasis, 144, "Solveur - Avancé", Paci20, var_buttons_flags[BT_LMODE], var_buttons_flags[BT_RMODE]);

    ybasis += 90;
    render_frame(xbasis, ybasis, var_screen_width - var_maze_width - 10, 234, "Calcul d'un chemin complexe");

    // Type de parcours
    render_text_centered(var_info_mid, ybasis + 28, "Algorithme (a)", Sans18);
    render_selector(var_info_mid, ybasis + 28 + 26, 300, cmp_names[var_cmp_algorithm], Sans18, var_buttons_flags[BT_ADVL], var_buttons_flags[BT_ADVR]);

    render_smallbutton((var_maze_width + 30 + var_info_mid) / 2, 380, 220, "Nouveau calcul (c)", var_buttons_flags[BT_ADVCOMP]);
    render_smallbutton((var_screen_width - 30 + var_info_mid) / 2, 380, 220, "Effacer le calcul (e)", var_buttons_flags[BT_ADVDELE]);




    ybasis = 390;
    int w = 21;

    if (var_adv_path) {

        sprintf(message, "Chemin enregistré: %s", cmp_names[var_adv_path->algo]);
        render_text(xbasis + w, ybasis + 28, message, Sans18);
        // Nombre de cul-de-sacs
        sprintf(message, "Nombre de segments: %d", var_adv_path->size);
        render_text(xbasis + w, ybasis + 28 + 22, message, Sans18);
        // Nombre de sorties
        sprintf(message, "Longueur totale: %d", var_adv_path->length);
        render_text(xbasis + w, ybasis + 28 + 44, message, Sans18);
    }
    else {
        sprintf(message, "Chemin enregistré: non");
        render_text(xbasis + w, ybasis + 28, message, Sans18);
    }

    ybasis = 540;
    if (var_adv_path) {
        render_frame(xbasis, ybasis, var_screen_width - var_maze_width - 10, 220, "Affichage");
        render_text_centered(var_info_mid, ybasis + 28, "Mode d'affichage (d)", Sans18);

        if (var_adv_mode == 2) {
            render_selector(var_info_mid, ybasis + 28 + 26, 300, "Vue segment par segment", Sans18, var_buttons_flags[BT_ADVMODEL], var_buttons_flags[BT_ADVMODER]);

            if (var_edit_mode == ED_SEG) {
                print_text_buffer_center();
            }
            else {
                sprintf(message, "Segment %d/%d (longueur: %d)", var_adv_num + 1, var_adv_path->size, var_adv_path->array[var_adv_num]->length);
            }
            render_selector_edit(var_info_mid, ybasis + 28 + 26 + 56, 300, message, Sans18, var_buttons_flags[BT_ADVSEG]);


            render_smallbutton((var_maze_width + 30 + var_info_mid) / 2, 706, 220, "Précédent (p)", var_buttons_flags[BT_ADVP]);
            render_smallbutton((var_screen_width - 30 + var_info_mid) / 2, 706, 220, "Suivant (n)", var_buttons_flags[BT_ADVN]);
        }
        else if (var_adv_mode == 1) {

            render_selector(var_info_mid, ybasis + 28 + 26, 300, "Vue globale", Sans18, var_buttons_flags[BT_ADVMODEL], var_buttons_flags[BT_ADVMODER]);
        }
        else {

            render_selector(var_info_mid, ybasis + 28 + 26, 300, "Caché", Sans18, var_buttons_flags[BT_ADVMODEL], var_buttons_flags[BT_ADVMODER]);

        }
    }
}

void render_all_informations(game* thegame) {

    // Affichage du masque droit
    SDL_Rect themask;
    themask.x = var_maze_width;
    themask.y = 0;
    themask.h = var_screen_height;
    themask.w = var_info_width;
    SDL_RenderFillRect(renderer, &themask);

    // Affichage du masque inférieur
    SDL_Rect themask2;
    themask2.x = 0;
    themask2.y = var_maze_height;
    themask2.h = 64;
    themask2.w = var_screen_width;
    SDL_RenderFillRect(renderer, &themask2);

    render_informations(thegame);

    switch (var_menu)
    {
    case MENU_PLAY:
        render_play_informations(thegame);
        break;
    case MENU_GENE:
        render_gen_informations(thegame);
        break;
    case MENU_SOSI:
        render_sim_informations(thegame);
        break;
    case MENU_SOAD:
        render_cmp_searchs(thegame);
        break;
    default:
        break;
    }
}

