#include <android/log.h>
#include "../SDL/include/SDL.h"

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GLES2_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_gles2.h"

char buf[256] = {0};


char test[] = "A long time ago, in a galaxy far, far away, "
               "It is a period of civil war. Rebel "
               "spaceships, striking from a hidden "
               "base, have won their first victory "
               "against the evil Galactic Empire. "
               "During the battle, Rebel spies managed "
               "to steal secret plans to the Empire's "
               "ultimate weapon, the Death Star, an "
               "armored space station with enough "
               "power to destroy an entire planet. "
               "Pursued by the Empire's sinister agents, "
               "Princess Leia races home aboard her "
               "starship, custodian of the stolen plans "
               "that can save her people and restore "
               "freedom to the galaxy....";



void render_gui(struct nk_context *ctx, SDL_Window *win, int w, int h, int dpi) {
    /* GUI */
    int RH = h/10;
    if (nk_begin(ctx, "Demo", nk_rect(0, 0, w, h), NK_WINDOW_NO_SCROLLBAR))
    {
        enum {EASY, HARD, VHARD};
        static int op = EASY;

        nk_layout_row_dynamic(ctx, RH, 1);
        if (nk_button_label(ctx, "button"))
            __android_log_print(ANDROID_LOG_INFO, "sdltest", "button pressed");
        nk_layout_row_dynamic(ctx, RH, 3);
        if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
        if (nk_option_label(ctx, "very hard", op == VHARD)) op = VHARD;

        nk_layout_row_dynamic(ctx, RH, 1);
        nk_flags event = nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, buf, 256, 0);
        if (event & NK_EDIT_ACTIVATED) {
            SDL_StartTextInput();
        }
        if (event & NK_EDIT_DEACTIVATED) {
            SDL_StopTextInput();
        }

        nk_layout_row_dynamic(ctx, RH*5, 1);
        nk_text_wrap(ctx,test, sizeof(test));

    }
    nk_end(ctx);


    /* Draw */
    {float bg[4];
        int win_width, win_height;
        nk_color_fv(bg, nk_rgb(0,0,0));
        SDL_GetWindowSize(win, &win_width, &win_height);
        glViewport(0, 0, win_width, win_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg[0], bg[1], bg[2], bg[3]);
        /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
         * with blending, scissor, face culling, depth test and viewport and
         * defaults everything back into a default state.
         * Make sure to either a.) save and restore or b.) reset your own state after
         * rendering the UI. */
        nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
        SDL_GL_SwapWindow(win);}
}


void dark_theme(struct nk_context *ctx) {
    struct nk_color table[NK_COLOR_COUNT];

    struct nk_color bg = nk_rgba(20, 20, 25, 255);
    struct nk_color def = nk_rgba(40, 40, 50, 255);
    struct nk_color hv = nk_rgba(40, 40, 50, 255);
    struct nk_color act = nk_rgba(50, 50, 62, 255);
    struct nk_color fg = nk_rgba(200, 200, 200, 255);
    struct nk_color na = nk_rgba(255, 0, 0, 255);


    table[NK_COLOR_TEXT] = fg;
    table[NK_COLOR_WINDOW] = bg;
    table[NK_COLOR_HEADER] = bg;
    table[NK_COLOR_BORDER] = fg;
    table[NK_COLOR_BUTTON] = bg;
    table[NK_COLOR_BUTTON_HOVER] = hv;
    table[NK_COLOR_BUTTON_ACTIVE] = act;
    table[NK_COLOR_TOGGLE] = def;
    table[NK_COLOR_TOGGLE_HOVER] = hv;
    table[NK_COLOR_TOGGLE_CURSOR] = fg;
    table[NK_COLOR_SELECT] = def;
    table[NK_COLOR_SELECT_ACTIVE] = fg;
    table[NK_COLOR_SLIDER] = def;
    table[NK_COLOR_SLIDER_CURSOR] = fg;
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = hv;
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = fg;
    table[NK_COLOR_PROPERTY] = fg;
    table[NK_COLOR_EDIT] = bg;
    table[NK_COLOR_EDIT_CURSOR] = fg;
    table[NK_COLOR_COMBO] = na;
    table[NK_COLOR_CHART] = na;
    table[NK_COLOR_CHART_COLOR] = na;
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = na;
    table[NK_COLOR_SCROLLBAR] = na;
    table[NK_COLOR_SCROLLBAR_CURSOR] = na;
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = na;
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = na;
    table[NK_COLOR_TAB_HEADER] = na;
    nk_style_from_table(ctx, table);
}


int SDL_main(int argc, char *argv[]) {
    __android_log_print(ANDROID_LOG_INFO, "sdltest", "SDL_main started");


    struct nk_context *ctx;
    SDL_GLContext glContext;
    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Window *win = SDL_CreateWindow("Demo",
                                       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                       0, 0,
                                       SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    glContext = SDL_GL_CreateContext(win);

    int w,h;
    float dpi;
    SDL_GetWindowSize(win, &w, &h);
    SDL_GetDisplayDPI(0, 0, &dpi, 0);
    float mult = dpi/96;

    __android_log_print(ANDROID_LOG_INFO, "sdltest", "screen: %dx%d %f mult",w,h,mult);


    /* OpenGL setup */
    glViewport(0, 0, w, h);

    ctx = nk_sdl_init(win);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {
        struct nk_font_atlas *atlas;
        nk_sdl_font_stash_begin(&atlas);
        atlas->default_font = nk_font_atlas_add_default(atlas, 12 * mult, 0);
        nk_sdl_font_stash_end();
    }
    dark_theme(ctx);

    int quit = 0;
    while(!quit) {
        SDL_Event e;
        nk_input_begin(ctx);
        if (SDL_WaitEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_AC_BACK)) break;
            //__android_log_print(ANDROID_LOG_INFO, "sdltest", "event: %x",e.type);
            nk_sdl_handle_event(&e);
            render_gui(ctx, win, w, h, mult);
        }
        nk_input_end(ctx);
    }


    nk_sdl_shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(win);
    SDL_Quit();
    exit(0);
}

