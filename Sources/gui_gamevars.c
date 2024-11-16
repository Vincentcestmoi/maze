#include "gui_gamevars.h"


/******************************/
/*+ Dimensions de la fenêtre **/
/******************************/

int var_screen_width, var_screen_height;
int var_maze_width, var_maze_height;
int var_info_width, var_info_height, var_info_mid;

/***************************/
/*+ Contrôle de la Souris **/
/***************************/

int var_mousePosx;
int var_mousePosy;
bool var_leftMouseButtonDown;

/**************************************/
/*+ Contrôle du zoom et du Scrolling **/
/**************************************/

double var_render_zoom = 1;
int var_render_zoom_level = 4;
int var_tile_size = 32;

int var_scrollOffsetx;
int var_scrollOffsety;
int var_startx;
int var_starty;
int var_endx;
int var_endy;
int var_topleftx;
int var_toplefty;

void scroll_update_borders(void) {
    if (var_scrollOffsetx >= 0) {
        var_startx = 0;
        var_topleftx = var_scrollOffsetx;
        var_endx = var_startx + (var_maze_width - var_topleftx) / (2 * var_tile_size) + 2;
    }
    else {
        var_topleftx = var_scrollOffsetx % (2 * var_tile_size);
        var_startx = (var_topleftx - var_scrollOffsetx) / (2 * var_tile_size);
        var_endx = var_startx + var_maze_width / (2 * var_tile_size) + 2;
    }

    if (var_scrollOffsety >= 0) {
        var_starty = 0;
        var_toplefty = var_scrollOffsety;
        var_endy = var_starty + (var_maze_height - var_toplefty) / (2 * var_tile_size) + 2;
    }
    else {
        var_starty = (-1 * var_scrollOffsety) / (2 * var_tile_size);
        var_toplefty = var_scrollOffsety % (2 * var_tile_size);
        var_endy = var_starty + var_maze_height / (2 * var_tile_size) + 2;
    }
}

void decrease_zoom(void) {
    if (var_render_zoom_level <= 0) {
        return;
    }
    else {
        var_render_zoom = var_render_zoom / 2;
        var_tile_size = var_tile_size / 2;
        var_render_zoom_level--;
        var_scrollOffsetx = (var_scrollOffsetx - var_maze_width / 2) / 2 + var_maze_width / 2;
        var_scrollOffsety = (var_scrollOffsety - var_maze_height / 2) / 2 + var_maze_height / 2;
        scroll_update_borders();
    }
}
void increase_zoom(void) {
    if (var_render_zoom_level >= 5) {
        return;
    }
    else {
        var_render_zoom = var_render_zoom * 2;
        var_tile_size = var_tile_size * 2;
        var_render_zoom_level++;
        var_scrollOffsetx = (var_scrollOffsetx - var_maze_width / 2) * 2 + var_maze_width / 2;
        var_scrollOffsety = (var_scrollOffsety - var_maze_height / 2) * 2 + var_maze_height / 2;
        scroll_update_borders();
    }
}


void scroll_manual(int x, int y) {
    var_scrollOffsetx += x;
    var_scrollOffsety += y;
    scroll_update_borders();
}

void scroll_with_mouse(int x, int y) {
    var_scrollOffsetx += x - var_mousePosx;
    var_scrollOffsety += y - var_mousePosy;
    scroll_update_borders();
}

void scroll_to_cell(game* thegame, int cell) {
    if (thegame && thegame->m) {
        int step = 64 * var_render_zoom;
        var_scrollOffsetx = -1 * (cell % thegame->m->hsize) * step + var_maze_width / 2;
        var_scrollOffsety = -1 * (cell / thegame->m->hsize) * step + var_maze_height / 2;
        scroll_update_borders();
    }
}

void scroll_to_player(game* thegame) {
    if (thegame && thegame->m) {
        scroll_to_cell(thegame, thegame->m->player);
    }
}

/****************************/
/*+ Contrôle de la vitesse **/
/****************************/

int var_render_speed = 32;
int var_render_speed_level = 5;


void increase_speed(void) {
    if (var_render_speed_level > 1) {
        var_render_speed_level--;
    }
}
void decrease_speed(void) {
    if (var_render_speed_level < 8) {
        var_render_speed_level++;
    }
}

/**************************/
/*+ Les différents menus **/
/**************************/

menu_mode var_menu = MENU_PLAY;


/*************************/
/*+ Lecture des masques **/
/*************************/

struct dirent** var_maskstab = NULL;
int var_maskindex = -1;
int var_masktabsize = 0;

static int is_mask_file(const struct dirent* dir) {
    char* ext = strstr(dir->d_name, ".msk");
    if (ext && ext[4] == '\0') {
        return true;
    }
    else {
        return false;
    }
}

void var_init_maskstab(void) {
    var_maskindex = -1;
    var_masktabsize = scandir("../Masques/", &var_maskstab, is_mask_file, alphasort);
}

void var_free_maskstab(void) {

    if (var_maskstab) {
        for (int i = 0; i < var_masktabsize;i++)
        {
            free(var_maskstab[i]);
        }
        free(var_maskstab);
    }
    var_maskstab = NULL;

    var_maskindex = -1;
    var_masktabsize = 0;
}

/********************************/
/*+ Entrée de texte au clavier **/
/********************************/

edit_mode var_edit_mode = ED_NONE;
text_buffer* var_editbuffer = NULL;

/****************/
/*+ Animations **/
/****************/

int var_frame = 0;
anim_mode var_crender = AN_IDLE;

int an_player_pos;
int* an_minotaurs_pos;
move an_player_move;
move* an_minotaurs_move;
object an_obj_taken;
bool an_killed;
int an_killer;
cardinal an_killdir;
bool* an_minotaurs_alive;
cardinal an_bombdir;
bool an_hadwall;
bool an_destroyed;

/******************************/
/*+ Stratégie des minotaures **/
/******************************/

strategy var_mino_strat = STR_AGRE;

/**********************************/
/*+ Variables pour la génération **/
/**********************************/

generator gen_mode = GEN_HKDFS;
objgenerator obj_mode = OBJ_SIMPLE;
int gen_hsize = 200;
int gen_vsize = 200;
int gen_nbminotaurs = 200;
int gen_braiding = 0;
char gen_maskname[80] = { 'P', 'a', 's', ' ', 'd', 'e', ' ', 'm', 'a', 's', 'q', 'u', 'e', '\0' };
char* gen_initialmask = NULL;


/*****************/
/*+ Les boutons **/
/*****************/

int var_button_countdown = 0;
bool var_buttons_flags[BT_SIZE];

void var_reset_buttons(void) {
    for (int i = 0; i < BT_SIZE;i++) {
        var_buttons_flags[i] = false;
    }
}

void var_push_button(buttons bt) {
    var_buttons_flags[bt] = true;
    var_button_countdown = SDL_GetTicks();
}

void var_release_button(buttons bt) {
    var_buttons_flags[bt] = false;
}

/*********************************/
/*+ Variables le solveur simple **/
/*********************************/


sim_algorithm var_sim_algorithm = ALG_BFS;
sim_goal var_sim_goal = GOA_EXIT;
bool var_sim_minos = false;
sim_search* var_sim_search = NULL;
heuristic var_heuristic = HEU_MANH;
sim_path* var_path_red = NULL;
sim_path* var_path_green = NULL;
sim_path* var_path_blue = NULL;
bool var_display_search = false;
int var_search_step = 0;


cmp_algorithm var_cmp_algorithm = CMP_TREAS;
cmp_search* var_adv_path = NULL;
unsigned char var_adv_mode = 0;
int var_adv_num = 0;
