#include <stdbool.h>
#include <stdlib.h>

const short int BACKGROUND_COLOR = 0xffff;

/*-----------------------------------------------AUDIO-------------------------------------------------------*/

const int SAMPLE_RATE = 8000;
const int MAX_RECORDING_SIZE = 480000; // 1 minute at 8000 hz

struct audio_module {
	volatile unsigned int control;
	volatile unsigned char rarc;
	volatile unsigned char ralc;
	volatile unsigned char wsrc;
	volatile unsigned char wslc;
	volatile unsigned int ldata;
	volatile unsigned int rdata;
};

struct audio_module *const audiop = ((struct audio_module *) 0xff203040);

/*-----------------------------------------------BUTTONS-----------------------------------------------------*/

const int KEY0 = 0b1;
const int KEY1 = 0b10;
const int KEY2 = 0b100;
const int KEY3 = 0b1000;

struct PIT_t {
volatile unsigned int DR;
volatile unsigned int DIR;
volatile unsigned int MASK;
volatile unsigned int EDGE;
};

struct PIT_t *buttonp = ((struct PIT_t *) 0xFF200050);

/*-----------------------------------------------SWITCHES AND LEDS-----------------------------------------------------*/

volatile int* leds = (int*) 0xFF200000;
volatile int* switches = (int*) 0xFF200040;

/*-----------------------------------------------CHAR BUFFER-----------------------------------------------------*/

volatile char* char_buffer = (char*) 0x09000000;
const int CHAR_SIZE = 8; // pixels

/*-----------------------------------------------PROVIDED DRAW FUNCTIONS-----------------------------------------------------*/

int xn = 320;
int yn = 240;

bool isBuffer1 = true;

struct fb_t
{
    unsigned short volatile pixels[256][512];
};

struct videoout_t
{
    struct fb_t *volatile fbp;
    struct fb_t *volatile bfbp;
    unsigned int volatile resolution;
    unsigned int volatile StatusControl;
};

struct box
{
    int x;
    int y;
    int x_shift;
    int y_shift;
    unsigned short int color;
};

struct videoout_t volatile *const vp = ((struct videoout_t *)0xFF203020);

struct fb_t buffer1;
struct fb_t buffer2;

void fbswap()
{
    vp->fbp = 1;
    while ((vp->StatusControl & 1) != 0);

    isBuffer1 = (isBuffer1) ? false : true;
}

void plot_pixel(int x, int y, unsigned short int color)
{
    if (isBuffer1) buffer1.pixels[y][x] = color;
    else buffer2.pixels[y][x] = color;
}

void clear_screen()
{
    for (int i = 0; i < xn; ++i)
        for (int j = 0; j < yn; ++j)
            plot_pixel(i, j, BACKGROUND_COLOR);
}

void clear_frame_buffers()
{
    clear_screen();
    fbswap();
    clear_screen();
}

void clear_char_buffer()
{
    for (int j = 0; j < 60; ++j)
        for (int i = 0; i < 80; ++i)
                *(char_buffer + j * 128 + i) = ' ';
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void draw_rectangle(int x1, int y1, int x2, int y2, unsigned short int color) {
    if (x1 > x2) swap(&x1, &x2);
    if (y1 > y2) swap(&y1, &y2);

    for (int i = x1; i <= x2; ++i)
        for (int j = y1; j < y2; ++j)
            plot_pixel(i, j, color);
}

void draw_line(int x1, int y1, int x2, int y2, short int line_color)
{
    bool is_steep = abs(y2 - y1) > abs(x2 - x1);
    if (is_steep)
    {
        swap(&x1, &y1);
        swap(&x2, &y2);
    }

    if (x1 > x2)
    {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    int delta_x = x2 - x1;
    int delta_y = abs(y2 - y1);
    int error = -(delta_x / 2);

    int y = y1;
    int y_step;
    if (y1 < y2)
        y_step = 1;
    else
        y_step = -1;

    for (int x = x1; x <= x2; ++x)
    {
        if (is_steep)
            plot_pixel(y, x, line_color);
        else
            plot_pixel(x, y, line_color);

        error = error + delta_y;
        if (error > 0)
        {
            y = y + y_step;
            error = error - delta_x;
        }
    }
}

/*-----------------------------------------------MENU BUTTON CONSTANTS-----------------------------------------------------*/

const short int BUTTON_HIGHLIGHT_COLOR = 0x5aeb;
const short int WAVEFORM_BACKGROUND_COLOR = 0xef9d;
const short int PLAY_BUTTON_COLOR = 0x8fed;
const short int REC_BUTTON_COLOR = 0xfb2d;
const short int EDIT_BUTTON_COLOR = 0xd6da;
const short int DELETE_BUTTON_COLOR = 0x6b4d;

const int REC_1_BUTTON = 0;
const int REC_2_BUTTON = 3;
const int REC_3_BUTTON = 6;

const int PLAY_1_BUTTON = 1;
const int PLAY_2_BUTTON = 4;
const int PLAY_3_BUTTON = 7;

const int DELETE_1_BUTTON = 2;
const int DELETE_2_BUTTON = 5;
const int DELETE_3_BUTTON = 8;

const int EDIT_BUTTON = 9;

const int MENU_FIRST_BUTTON = 0;
const int MENU_LAST_BUTTON = 9;

const int REC_BUTTON_SIZE = 24;
const int PLAY_BUTTON_SIZE = 24;
const int DELETE_BUTTON_SIZE = 24;
const int EDIT_BUTTON_SIZE_X = 24;
const int EDIT_BUTTON_SIZE_Y = 24;

const int WAVEFORM_X = 314;
const int WAVEFORM_Y = 60;
const int WAVEFORM_THICKNESS = 50;

const int REC_1_X = 30;
const int REC_1_Y = 98;

const int REC_2_X = 62;
const int REC_2_Y = 98;

const int REC_3_X = 94;
const int REC_3_Y = 98;

const int PLAY_1_X = 30;
const int PLAY_1_Y = 130;

const int PLAY_2_X = 62;
const int PLAY_2_Y = 130;

const int PLAY_3_X = 94;
const int PLAY_3_Y = 130;

const int DELETE_1_X = 30;
const int DELETE_1_Y = 162;

const int DELETE_2_X = 62;
const int DELETE_2_Y = 162;

const int DELETE_3_X = 94;
const int DELETE_3_Y = 162;

const int EDIT_X = 270;
const int EDIT_Y = 114;

/*-----------------------------------------------EDITOR BUTTON CONSTANTS-----------------------------------------------------*/

const short int PLAY_ALL_COLOR = 0xc618;
const short int EXIT_EDITOR_COLOR = 0xc618;
const short int CURSOR_COLOR = 0x5bdf;
const short int CURSOR_ACTIVATE_COLOR = 0x66bf;

const int TRACK_1_X = 5;
const int TRACK_1_Y = 10;
const int TRACK_1_HEIGHT = 50;
const int TRACK_1_WIDTH = 274;

const int TRACK_2_X = 5;
const int TRACK_2_Y = 70;
const int TRACK_2_HEIGHT = 50;
const int TRACK_2_WIDTH = 274;

const int TRACK_3_X = 5;
const int TRACK_3_Y = 130;
const int TRACK_3_HEIGHT = 50;
const int TRACK_3_WIDTH = 274;

const int PLAY_ALL_X = 4;
const int PLAY_ALL_Y = 190;
const int PLAY_ALL_SIZE = 24;

const int EXIT_EDITOR_X = 256;
const int EXIT_EDITOR_Y = 190;
const int EXIT_EDITOR_SIZE = 24;

const int TRACK_1_LOOP_X = 290;
const int TRACK_1_LOOP_Y = 22;
const int TRACK_1_LOOP_SIZE = 24;

const int TRACK_2_LOOP_X = 290;
const int TRACK_2_LOOP_Y = 82;
const int TRACK_2_LOOP_SIZE = 24;

const int TRACK_3_LOOP_X = 290;
const int TRACK_3_LOOP_Y = 142;
const int TRACK_3_LOOP_SIZE = 24;

const int TRACK_1_SELECT = 0;
const int TRACK_1_LOOP = 1;
const int TRACK_2_SELECT = 2;
const int TRACK_2_LOOP = 3;
const int TRACK_3_SELECT = 4;
const int TRACK_3_LOOP = 5;
const int PLAY_ALL_BUTTON = 6;
const int EXIT_EDITOR_BUTTON = 7;

const int EDITOR_LAST_BUTTON = 7;
const int EDITOR_FIRST_BUTTON = 0;


/*-----------------------------------------------TRACK EDITOR BUTTON CONSTANTS-----------------------------------------------------*/

const short int HIGHLIGHT_COLOR = 0x147f;
const int BUTTON_MODE = 1;
const int ZOOM_MODE = 2;
const int HIGHLIGHT_MODE = 3;
const int PASTE_MODE = 4;
const int NONE = -1;

const int EDITING_TRACK_X = 5;
const int EDITING_TRACK_Y = 5;
const int EDITING_TRACK_HEIGHT = 140;
const int EDITING_TRACK_WIDTH = 309;

const int TRACK_OUTLINE = -1;
const int HIGHLIGHT_BUTTON = 1;
const int ZOOM_BUTTON = 2;
const int DELETE_BUTTON = 4;
const int MUTE_BUTTON = 5;
const int COPY_BUTTON = 6;
const int PASTE_BUTTON = 7;
const int PLAY_FROM_CURSOR_BUTTON = 3;

const int EXIT_TRACK_EDITOR_BUTTON = 8;

const int TRACK_EDITOR_FIRST_BUTTON = 1;
const int TRACK_EDITOR_LAST_BUTTON = 8;

const int HIGHLIGHT_X = 8;
const int HIGHLIGHT_Y = 154;
const int HIGHLIGHT_SIZE = 24;

const int ZOOM_X = 40;
const int ZOOM_Y = 154;
const int ZOOM_SIZE = 24;

const int DELETE_X = 8;
const int DELETE_Y = 210;
const int DELETE_SIZE = 24;

const int MUTE_X = 40;
const int MUTE_Y = 210;
const int MUTE_SIZE = 24;

const int COPY_X = 72;
const int COPY_Y = 210;
const int COPY_SIZE = 24;

const int PASTE_X = 104;
const int PASTE_Y = 210;
const int PASTE_SIZE = 24;

const int PLAY_FROM_CURSOR_X = 72;
const int PLAY_FROM_CURSOR_Y = 154;
const int PLAY_FROM_CURSOR_SIZE = 24;

const int EXIT_TRACK_EDITOR_X = 290;
const int EXIT_TRACK_EDITOR_Y = 182;
const int EXIT_TRACK_EDITOR_SIZE = 24;

const int MIN_RESOLUTION = 1;
const int MAX_RESOLUTION = 501; // make all resolutions end in 1 to make math easier

/*----------------------------------------------TRACKS------------------------------------------------------*/

unsigned int rec1[480000];
int rec1_size = 0;
unsigned int rec2[480000];
int rec2_size = 0;
unsigned int rec3[480000];
int rec3_size = 0;

unsigned int clipboard[480000];
int clipboard_size = 0;

/*----------------------------------------------Looping Boolean------------------------------------------------------*/
bool looping_1 = false;
bool looping_2 = false;
bool looping_3 = false;
/*-----------------------------------------------CUSTOM DRAW FUNCTIONS-----------------------------------------------------*/

// converts x, y to char position automatically
// use c = 0 to erase a character
void draw_char(int x_pix, int y_pix, char c) {
    int x = x_pix / 4;
    int y = y_pix / 4;

    *(char_buffer + x + y * 128) = c;
}

void draw_string(int x_pix, int y_pix, char c[]) {
    int x = x_pix / 4;
    int y = y_pix / 4;

    int index = 0;

    while (c[index] != '\0') {
        *(char_buffer + x + y * 128 + index) = c[index];
        ++index;
    }
}

void plot_recording(int rec [], int rec_size, int x_centre, int y_centre, int vert_size, int color, int resolution) {
    double norm = (double) vert_size / 0xFFFFFFFE;

    for (int i = 0; i + 1 <= 2 * x_centre - 319; ++i) {
        if (i * resolution + resolution >= rec_size) continue;

        draw_line(x_centre - i - 1, y_centre + rec[rec_size - 1 - i * resolution - resolution] * norm, 
                  x_centre - i, y_centre + rec[rec_size - 1 - i * resolution] * norm, color);
    }
}

// int cursor is the centre of the recording
void plot_recording_window(int rec [], int rec_size, int cursor, int x_centre, int y_centre, int height, int width, int color, int resolution) {
    double norm = (double) height / 0xFFFFFFFE;

    for (int i = 0; i + 1 <= (width >> 1); i = i + 1) {
        if (cursor + i * resolution + resolution < rec_size)
            draw_line(x_centre + i, y_centre + rec[cursor + i * resolution] * norm,
                      x_centre + i + 1, y_centre + rec[cursor + i * resolution + resolution] * norm, color);
        else plot_pixel(x_centre + i + 1, y_centre, color);

        if (cursor - i * resolution - resolution >= 0)
            draw_line(x_centre - i, y_centre + rec[cursor - i * resolution] * norm,
                      x_centre - i - 1, y_centre + rec[cursor - i * resolution - resolution] * norm, color);
    }
}

void record_and_plot(int rec [], int* rec_size, int x, int y, int vert_size, int rec_num) {
    int data;
    unsigned int* temp = NULL;
    int plot_frequency = 3200; // name is misleading, this is period

    *rec_size = 0;

    int rec_first, rec_second;
    int first_size, second_size;

    if (rec_num != 1 && rec_num != 2) {
        rec_first = 1;
        rec_second = 2;
        first_size = rec1_size;
        second_size = rec2_size;
    }
    else if (rec_num != 1 && rec_num != 3) {
        rec_first = 1;
        rec_second = 3;
        first_size = rec1_size;
        second_size = rec3_size;
    }
    else {
        rec_first = 2;
        rec_second = 3;
        first_size = rec2_size;
        second_size = rec3_size;
    }

    audiop->control = 0xc;
    audiop->control = 0;

    int playback_index = 0;
    int playback_output = 0;

    while (1) {
        // wait for something in the buffer
        while (!audiop->ralc);
        data = audiop->ldata;

        if (playback_index < first_size) {
            if (rec_first == 1) playback_output += rec1[playback_index];
            if (rec_first == 2) playback_output += rec2[playback_index];
        }

        if (playback_index < second_size) {
            if (rec_second == 2) playback_output += rec2[playback_index];
            if (rec_second == 3) playback_output += rec3[playback_index];
        }
        
        if (playback_index < first_size || playback_index < second_size) {
            audiop->ldata = playback_output;
            audiop->rdata = playback_output;
        }

        ++playback_index;

        playback_output = 0;

        // allocate more space for recording
        
        rec[*(rec_size)] = data / 3; // set new space to data

        *rec_size = *rec_size + 1;

        if (*rec_size >= MAX_RECORDING_SIZE) break;

        if (*rec_size % plot_frequency == 0) {
            plot_recording(rec, *rec_size - 2 * plot_frequency, x, y, vert_size, WAVEFORM_BACKGROUND_COLOR, 10);
            plot_recording(rec, *rec_size, x, y, vert_size, 0x0, 10);
            fbswap();
        }

        int mask = KEY0; // if KEY0 is pressed, stop recording
        if (mask & buttonp->EDGE == 0) continue; 

        mask = buttonp->EDGE;
        buttonp->EDGE = mask;

        while (*rec_size % 4000 != 0) {
            rec[*rec_size] = 0;
            *rec_size = *rec_size + 1;
        }
        break;
    }
}

void play_all() {
    int playback_index = 0;
    int rec1_index, rec2_index, rec3_index;
    rec1_index = rec2_index = rec3_index = 0;

    int playback_output = 0;

    *leds = 1;

    int beat_length = SAMPLE_RATE / 2;

    while (playback_index < MAX_RECORDING_SIZE)
    {
        if (looping_1 && rec1_index >= rec1_size) rec1_index = 0;
        if (looping_2 && rec2_index >= rec2_size) rec2_index = 0;
        if (looping_3 && rec3_index >= rec3_size) rec3_index = 0;

        if (rec1_index < rec1_size) playback_output += rec1[rec1_index];
        if (rec2_index < rec2_size) playback_output += rec2[rec2_index];
        if (rec3_index < rec3_size) playback_output += rec3[rec3_index];

        while (!audiop->wslc || !audiop->wsrc);

        audiop->ldata = playback_output;
        audiop->rdata = playback_output;
        
        ++playback_index;

        if (!looping_1 && !looping_2 && !looping_3)
            if (playback_index >= rec1_size && playback_index >= rec2_size && playback_index >= rec3_size) break;

        ++rec1_index;
        ++rec2_index;
        ++rec3_index;
        playback_output = 0;

        if (buttonp->EDGE & 1 != 0) {
            buttonp->EDGE = 1;
            break;
        }
    }

    *leds = 0;
}

/*----------------------------------------------MAIN PROGRAM------------------------------------------------------*/

void draw_menu_buttons() {
    // waveform bar
    clear_char_buffer();
    draw_rectangle(319 - WAVEFORM_X - 1, WAVEFORM_Y - 1 - (WAVEFORM_THICKNESS >> 1), WAVEFORM_X , WAVEFORM_Y + (WAVEFORM_THICKNESS >> 1), WAVEFORM_BACKGROUND_COLOR);  
    
    draw_rectangle(REC_1_X, REC_1_Y, REC_1_X + REC_BUTTON_SIZE, REC_1_Y + REC_BUTTON_SIZE, REC_BUTTON_COLOR);
    draw_rectangle(REC_2_X, REC_2_Y, REC_2_X + REC_BUTTON_SIZE, REC_2_Y + REC_BUTTON_SIZE, REC_BUTTON_COLOR);
    draw_rectangle(REC_3_X, REC_3_Y, REC_3_X + REC_BUTTON_SIZE, REC_3_Y + REC_BUTTON_SIZE, REC_BUTTON_COLOR);

    draw_rectangle(PLAY_1_X, PLAY_1_Y, PLAY_1_X + PLAY_BUTTON_SIZE, PLAY_1_Y + PLAY_BUTTON_SIZE, PLAY_BUTTON_COLOR);
    draw_rectangle(PLAY_2_X, PLAY_2_Y, PLAY_2_X + PLAY_BUTTON_SIZE, PLAY_2_Y + PLAY_BUTTON_SIZE, PLAY_BUTTON_COLOR);
    draw_rectangle(PLAY_3_X, PLAY_3_Y, PLAY_3_X + PLAY_BUTTON_SIZE, PLAY_3_Y + PLAY_BUTTON_SIZE, PLAY_BUTTON_COLOR);

    draw_rectangle(DELETE_1_X, DELETE_1_Y, DELETE_1_X + DELETE_BUTTON_SIZE, DELETE_1_Y + DELETE_BUTTON_SIZE, DELETE_BUTTON_COLOR);
    draw_rectangle(DELETE_2_X, DELETE_2_Y, DELETE_2_X + DELETE_BUTTON_SIZE, DELETE_2_Y + DELETE_BUTTON_SIZE, DELETE_BUTTON_COLOR);
    draw_rectangle(DELETE_3_X, DELETE_3_Y, DELETE_3_X + DELETE_BUTTON_SIZE, DELETE_3_Y + DELETE_BUTTON_SIZE, DELETE_BUTTON_COLOR);

    draw_rectangle(EDIT_X, EDIT_Y, EDIT_X + EDIT_BUTTON_SIZE_X, EDIT_Y + EDIT_BUTTON_SIZE_Y, EDIT_BUTTON_COLOR);

    draw_string(REC_1_X + 4, REC_1_Y + 10, "REC1");
    draw_string(REC_2_X + 4, REC_2_Y + 10, "REC2");
    draw_string(REC_3_X + 4, REC_3_Y + 10, "REC3");

    draw_string(PLAY_1_X + 4, PLAY_1_Y + 10, "PLAY1");
    draw_string(PLAY_2_X + 4, PLAY_2_Y + 10, "PLAY2");
    draw_string(PLAY_3_X + 4, PLAY_3_Y + 10, "PLAY3");

    draw_string(DELETE_1_X + 4, DELETE_1_Y + 10, "DEL1");
    draw_string(DELETE_2_X + 4, DELETE_2_Y + 10, "DEL2");
    draw_string(DELETE_3_X + 4, DELETE_3_Y + 10, "DEL3");

     draw_string(EDIT_X + 4, EDIT_Y + 10, "EDIT");
}

void draw_editor_buttons() {
    // track backgrounds
    clear_char_buffer();
    draw_rectangle(TRACK_1_X, TRACK_1_Y - 1, TRACK_1_X + TRACK_1_WIDTH , TRACK_1_Y + TRACK_1_HEIGHT, WAVEFORM_BACKGROUND_COLOR);
    draw_rectangle(TRACK_2_X, TRACK_2_Y - 1, TRACK_2_X + TRACK_2_WIDTH , TRACK_2_Y + TRACK_2_HEIGHT, WAVEFORM_BACKGROUND_COLOR);
    draw_rectangle(TRACK_3_X, TRACK_3_Y - 1, TRACK_3_X + TRACK_3_WIDTH , TRACK_3_Y + TRACK_3_HEIGHT, WAVEFORM_BACKGROUND_COLOR);

    draw_rectangle(TRACK_1_LOOP_X, TRACK_1_LOOP_Y, TRACK_1_LOOP_X + TRACK_1_LOOP_SIZE, TRACK_1_LOOP_Y + TRACK_1_LOOP_SIZE, PLAY_ALL_COLOR);
    draw_rectangle(TRACK_2_LOOP_X, TRACK_2_LOOP_Y, TRACK_2_LOOP_X + TRACK_2_LOOP_SIZE, TRACK_2_LOOP_Y + TRACK_2_LOOP_SIZE, PLAY_ALL_COLOR);
    draw_rectangle(TRACK_3_LOOP_X, TRACK_3_LOOP_Y, TRACK_3_LOOP_X + TRACK_3_LOOP_SIZE, TRACK_3_LOOP_Y + TRACK_3_LOOP_SIZE, PLAY_ALL_COLOR);

    // play all button
    draw_rectangle(PLAY_ALL_X, PLAY_ALL_Y, PLAY_ALL_X + PLAY_ALL_SIZE, PLAY_ALL_Y + PLAY_ALL_SIZE, PLAY_ALL_COLOR);
    // exit editor button
    draw_rectangle(EXIT_EDITOR_X, EXIT_EDITOR_Y, EXIT_EDITOR_X + EXIT_EDITOR_SIZE, EXIT_EDITOR_Y + EXIT_EDITOR_SIZE, EXIT_EDITOR_COLOR);

    draw_string(PLAY_ALL_X + 4, PLAY_ALL_Y + 10, "PLAY");
    draw_string(EXIT_EDITOR_X + 4, EXIT_EDITOR_Y + 10, "EXIT");
    draw_string(TRACK_1_LOOP_X + 4, TRACK_1_LOOP_Y + 10, "LOOP1");
    draw_string(TRACK_2_LOOP_X + 4, TRACK_2_LOOP_Y + 10, "LOOP2");
    draw_string(TRACK_3_LOOP_X + 4, TRACK_3_LOOP_Y + 10, "LOOP3");
}

void draw_track_editor_buttons() {
    clear_char_buffer();
    draw_rectangle(ZOOM_X, ZOOM_Y, ZOOM_X + ZOOM_SIZE, ZOOM_Y + ZOOM_SIZE, PLAY_ALL_COLOR);
    draw_rectangle(HIGHLIGHT_X, HIGHLIGHT_Y, HIGHLIGHT_X + HIGHLIGHT_SIZE, HIGHLIGHT_Y + HIGHLIGHT_SIZE, PLAY_ALL_COLOR);
    draw_rectangle(EXIT_TRACK_EDITOR_X, EXIT_TRACK_EDITOR_Y, EXIT_TRACK_EDITOR_X + EXIT_TRACK_EDITOR_SIZE, EXIT_TRACK_EDITOR_Y + EXIT_TRACK_EDITOR_SIZE, PLAY_ALL_COLOR);

    draw_rectangle(DELETE_X, DELETE_Y, DELETE_X + DELETE_SIZE, DELETE_Y + DELETE_SIZE, PLAY_ALL_COLOR);
    draw_rectangle(MUTE_X, MUTE_Y, MUTE_X + MUTE_SIZE, MUTE_Y + MUTE_SIZE, PLAY_ALL_COLOR);
    draw_rectangle(COPY_X, COPY_Y, COPY_X + COPY_SIZE, COPY_Y + COPY_SIZE, PLAY_ALL_COLOR);
    draw_rectangle(PASTE_X, PASTE_Y, PASTE_X + PASTE_SIZE, PASTE_Y + PASTE_SIZE, PLAY_ALL_COLOR);

    draw_rectangle(PLAY_FROM_CURSOR_X, PLAY_FROM_CURSOR_Y, PLAY_FROM_CURSOR_X + PLAY_FROM_CURSOR_SIZE, PLAY_FROM_CURSOR_Y + PLAY_FROM_CURSOR_SIZE, PLAY_ALL_COLOR);

    draw_string(HIGHLIGHT_X + 4, HIGHLIGHT_Y + 10, "HILI");
    draw_string(ZOOM_X + 4, ZOOM_Y + 10, "ZOOM");
    draw_string(DELETE_X + 4, DELETE_Y + 10, "DEL");
    draw_string(MUTE_X + 4, MUTE_Y + 10, "MUTE");
    draw_string(COPY_X + 4, COPY_Y + 10, "COPY");
    draw_string(PASTE_X + 4, PASTE_Y + 10, "PSTE");
    draw_string(PLAY_FROM_CURSOR_X + 4, PLAY_FROM_CURSOR_Y + 10, "PLAY");

    draw_string(EXIT_TRACK_EDITOR_X + 4, EXIT_TRACK_EDITOR_Y + 10, "EXIT");
}

int highlight_button(int button, unsigned short int color, char window) {
    int x1, y1, x2, y2;

    x1 = y1 = x2 = y2 = 0;

    // menu buttons
    if (window == 'm') {

        if (button == REC_1_BUTTON) {
            x1 = REC_1_X - 1;
            y1 = REC_1_Y - 1;
            x2 = REC_1_X + REC_BUTTON_SIZE + 1;
            y2 = REC_1_Y + REC_BUTTON_SIZE;
        }

        if (button == REC_2_BUTTON) {
            x1 = REC_2_X - 1;
            y1 = REC_2_Y - 1;
            x2 = REC_2_X + REC_BUTTON_SIZE + 1;
            y2 = REC_2_Y + REC_BUTTON_SIZE;
        }

        if (button == REC_3_BUTTON) {
            x1 = REC_3_X - 1;
            y1 = REC_3_Y - 1;
            x2 = REC_3_X + REC_BUTTON_SIZE + 1;
            y2 = REC_3_Y + REC_BUTTON_SIZE;
        }

        if (button == PLAY_1_BUTTON) {
            x1 = PLAY_1_X - 1;
            y1 = PLAY_1_Y - 1;
            x2 = PLAY_1_X + PLAY_BUTTON_SIZE + 1;
            y2 = PLAY_1_Y + PLAY_BUTTON_SIZE;
        }

        if (button == PLAY_2_BUTTON) {
            x1 = PLAY_2_X - 1;
            y1 = PLAY_2_Y - 1;
            x2 = PLAY_2_X + PLAY_BUTTON_SIZE + 1;
            y2 = PLAY_2_Y + PLAY_BUTTON_SIZE;
        }

        if (button == PLAY_3_BUTTON) {
            x1 = PLAY_3_X - 1;
            y1 = PLAY_3_Y - 1;
            x2 = PLAY_3_X + PLAY_BUTTON_SIZE + 1;
            y2 = PLAY_3_Y + PLAY_BUTTON_SIZE;
        }

        if (button == DELETE_1_BUTTON) {
            x1 = DELETE_1_X - 1;
            y1 = DELETE_1_Y - 1;
            x2 = DELETE_1_X + DELETE_BUTTON_SIZE + 1;
            y2 = DELETE_1_Y + DELETE_BUTTON_SIZE;
        }

        if (button == DELETE_2_BUTTON) {
            x1 = DELETE_2_X - 1;
            y1 = DELETE_2_Y - 1;
            x2 = DELETE_2_X + DELETE_BUTTON_SIZE + 1;
            y2 = DELETE_2_Y + DELETE_BUTTON_SIZE;
        }

        if (button == DELETE_3_BUTTON) {
            x1 = DELETE_3_X - 1;
            y1 = DELETE_3_Y - 1;
            x2 = DELETE_3_X + DELETE_BUTTON_SIZE + 1;
            y2 = DELETE_3_Y + DELETE_BUTTON_SIZE;
        }

        if (button == EDIT_BUTTON) {
            x1 = EDIT_X - 1;
            y1 = EDIT_Y - 1;
            x2 = EDIT_X + EDIT_BUTTON_SIZE_X + 1;
            y2 = EDIT_Y + EDIT_BUTTON_SIZE_Y;
        }
    }

    // editor buttons
    if (window == 'e') {
        if (button == PLAY_ALL_BUTTON) {
            x1 = PLAY_ALL_X - 1;
            y1 = PLAY_ALL_Y - 1;
            x2 = PLAY_ALL_X + PLAY_ALL_SIZE + 1;
            y2 = PLAY_ALL_Y + PLAY_ALL_SIZE;
        }
        
        if (button == TRACK_1_SELECT) {
            x1 = TRACK_1_X - 1;
            y1 = TRACK_1_Y - 1;
            x2 = TRACK_1_X + TRACK_1_WIDTH + 1;
            y2 = TRACK_1_Y + TRACK_1_HEIGHT;
        }

        if (button == TRACK_2_SELECT) {
            x1 = TRACK_2_X - 1;
            y1 = TRACK_2_Y - 1;
            x2 = TRACK_2_X + TRACK_2_WIDTH + 1;
            y2 = TRACK_2_Y + TRACK_2_HEIGHT;
        }

        if (button == TRACK_3_SELECT) {
            x1 = TRACK_3_X - 1;
            y1 = TRACK_3_Y - 1;
            x2 = TRACK_3_X + TRACK_3_WIDTH + 1;
            y2 = TRACK_3_Y + TRACK_3_HEIGHT;
        }

        if (button == EXIT_EDITOR_BUTTON) {
            x1 = EXIT_EDITOR_X - 1;
            y1 = EXIT_EDITOR_Y - 1;
            x2 = EXIT_EDITOR_X + EXIT_EDITOR_SIZE + 1;
            y2 = EXIT_EDITOR_Y + EXIT_EDITOR_SIZE;
        }

        if (button == TRACK_1_LOOP) {
            x1 = TRACK_1_LOOP_X - 1;
            y1 = TRACK_1_LOOP_Y - 1;
            x2 = TRACK_1_LOOP_X + TRACK_1_LOOP_SIZE + 1;
            y2 = TRACK_1_LOOP_Y + TRACK_1_LOOP_SIZE;
        }

        if (button == TRACK_2_LOOP) {
            x1 = TRACK_2_LOOP_X - 1;
            y1 = TRACK_2_LOOP_Y - 1;
            x2 = TRACK_2_LOOP_X + TRACK_2_LOOP_SIZE + 1;
            y2 = TRACK_2_LOOP_Y + TRACK_2_LOOP_SIZE;
        }

        if (button == TRACK_3_LOOP) {
            x1 = TRACK_3_LOOP_X - 1;
            y1 = TRACK_3_LOOP_Y - 1;
            x2 = TRACK_3_LOOP_X + TRACK_3_LOOP_SIZE + 1;
            y2 = TRACK_3_LOOP_Y + TRACK_3_LOOP_SIZE;
        }
    }

    if (window == 't') {
        if (button == HIGHLIGHT_BUTTON) {
            x1 = HIGHLIGHT_X - 1;
            y1 = HIGHLIGHT_Y - 1;
            x2 = HIGHLIGHT_X + HIGHLIGHT_SIZE + 1;
            y2 = HIGHLIGHT_Y + HIGHLIGHT_SIZE;
        }
        if (button == ZOOM_BUTTON) {
            x1 = ZOOM_X - 1;
            y1 = ZOOM_Y - 1;
            x2 = ZOOM_X + ZOOM_SIZE + 1;
            y2 = ZOOM_Y + ZOOM_SIZE;
        }
        if (button == EXIT_TRACK_EDITOR_BUTTON) {
            x1 = EXIT_TRACK_EDITOR_X - 1;
            y1 = EXIT_TRACK_EDITOR_Y - 1;
            x2 = EXIT_TRACK_EDITOR_X + EXIT_TRACK_EDITOR_SIZE + 1;
            y2 = EXIT_TRACK_EDITOR_Y + EXIT_TRACK_EDITOR_SIZE;
        }
        if (button == TRACK_OUTLINE) {
            x1 = EDITING_TRACK_X - 1;
            y1 = EDITING_TRACK_Y - 1;
            x2 = EDITING_TRACK_X + EDITING_TRACK_WIDTH + 1;
            y2 = EDITING_TRACK_Y + EDITING_TRACK_HEIGHT;
        }
        if (button == DELETE_BUTTON) {
            x1 = DELETE_X - 1;
            y1 = DELETE_Y - 1;
            x2 = DELETE_X + DELETE_SIZE + 1;
            y2 = DELETE_Y + DELETE_SIZE;
        }
        if (button == MUTE_BUTTON) {
            x1 = MUTE_X - 1;
            y1 = MUTE_Y - 1;
            x2 = MUTE_X + MUTE_SIZE + 1;
            y2 = MUTE_Y + MUTE_SIZE;
        }
        if (button == COPY_BUTTON) {
            x1 = COPY_X - 1;
            y1 = COPY_Y - 1;
            x2 = COPY_X + COPY_SIZE + 1;
            y2 = COPY_Y + COPY_SIZE;
        }
        if (button == PASTE_BUTTON) {
            x1 = PASTE_X - 1;
            y1 = PASTE_Y - 1;
            x2 = PASTE_X + PASTE_SIZE + 1;
            y2 = PASTE_Y + PASTE_SIZE;
        }
        if (button == PLAY_FROM_CURSOR_BUTTON) {
            x1 = PLAY_FROM_CURSOR_X - 1;
            y1 = PLAY_FROM_CURSOR_Y - 1;
            x2 = PLAY_FROM_CURSOR_X + PLAY_FROM_CURSOR_SIZE + 1;
            y2 = PLAY_FROM_CURSOR_Y + PLAY_FROM_CURSOR_SIZE;
        }
    }

    draw_line(x1, y1, x2, y1, color);
    draw_line(x1, y2, x2, y2, color);
    draw_line(x1, y1, x1, y2, color);
    draw_line(x2, y1, x2, y2, color);
}

void button_feedback(int button, char window) {
    highlight_button(button, 0xa534, window);
    fbswap();

    for (int i = 0; i < 12500000; ++i);

    highlight_button(button, BUTTON_HIGHLIGHT_COLOR, window);
    fbswap();
    highlight_button(button, BUTTON_HIGHLIGHT_COLOR, window);
}

int editor(int starting_button, int cursor, int curr_resolution) {
    draw_editor_buttons();
    fbswap();
    draw_editor_buttons();


    int curr_button = starting_button;
    int prev_button = starting_button;

    plot_recording_window(rec1, rec1_size, cursor, TRACK_1_X + TRACK_1_WIDTH / 2, TRACK_1_Y + TRACK_1_HEIGHT / 2, TRACK_1_HEIGHT, TRACK_1_WIDTH, 0x0, curr_resolution);
    plot_recording_window(rec2, rec2_size, cursor, TRACK_2_X + TRACK_2_WIDTH / 2, TRACK_2_Y + TRACK_2_HEIGHT / 2, TRACK_2_HEIGHT, TRACK_2_WIDTH, 0x0, curr_resolution);
    plot_recording_window(rec3, rec3_size, cursor, TRACK_3_X + TRACK_3_WIDTH / 2, TRACK_3_Y + TRACK_3_HEIGHT / 2, TRACK_3_HEIGHT, TRACK_3_WIDTH, 0x0, curr_resolution);
    fbswap();
    plot_recording_window(rec1, rec1_size, cursor, TRACK_1_X + TRACK_1_WIDTH / 2, TRACK_1_Y + TRACK_1_HEIGHT / 2, TRACK_1_HEIGHT, TRACK_1_WIDTH, 0x0, curr_resolution);
    plot_recording_window(rec2, rec2_size, cursor, TRACK_2_X + TRACK_2_WIDTH / 2, TRACK_2_Y + TRACK_2_HEIGHT / 2, TRACK_2_HEIGHT, TRACK_2_WIDTH, 0x0, curr_resolution);
    plot_recording_window(rec3, rec3_size, cursor, TRACK_3_X + TRACK_3_WIDTH / 2, TRACK_3_Y + TRACK_3_HEIGHT / 2, TRACK_3_HEIGHT, TRACK_3_WIDTH, 0x0, curr_resolution);
    fbswap();

    while (1) {
        highlight_button(prev_button, BACKGROUND_COLOR, 'e');
        highlight_button(curr_button, BUTTON_HIGHLIGHT_COLOR, 'e');
        fbswap();
        highlight_button(prev_button, BACKGROUND_COLOR, 'e');
        highlight_button(curr_button, BUTTON_HIGHLIGHT_COLOR, 'e');

        if ((buttonp->EDGE >> 3) & 1 != 0) {
			prev_button = curr_button;
            if (curr_button > EDITOR_FIRST_BUTTON) --curr_button;
            else curr_button = EDITOR_LAST_BUTTON;
			buttonp->EDGE = 0b1111;
        }
		
		if ((buttonp->EDGE >> 2) & 1 != 0) {
			prev_button = curr_button;
            if (curr_button < EDITOR_LAST_BUTTON) ++curr_button;
            else curr_button = EDITOR_FIRST_BUTTON;
			buttonp->EDGE = 0b1111;
        }
		
		if (buttonp->EDGE & 1 != 0) {
			buttonp->EDGE = 0b1111;
			return curr_button;
		}
    }
}

void delete_highlight(int rec[], int* rec_size, int start, int end) {
    if (start > end) swap(&start, &end);

    *rec_size = *rec_size - end + start;

    for (int i = start; i < *rec_size; ++i)
        rec[i] = rec[end - start + i];
    
    while (*rec_size % 4000 != 0) {
        rec[*rec_size] = 0;
        *rec_size = *rec_size + 1;
    }
}

void mute_highlight(int rec[], int start, int end) {
    if (start > end) swap(&start, &end);

    for (int i = start; i <= end; ++i)
        rec[i] = 0;
}

void copy_highlight(int rec[], int start, int end) {
    if (start > end) swap(&start, &end);

    clipboard_size = 0;
    for (int i = start; i <= end; ++i) {
        clipboard[i - start] = rec[i];
        ++clipboard_size;
    }
}

void paste_clipboard(int rec[], int* rec_size, int cursor) {
    int temp[480000];

    if (cursor > rec_size) {
        for (int i = *rec_size; i < cursor; ++i) {
            rec[i] = 0;
            *rec_size = *rec_size + 1;
        }
    }

    *rec_size = *rec_size + clipboard_size;
    
    for (int i = cursor; i < MAX_RECORDING_SIZE; ++i)
        temp[i - cursor] = rec[i];

    for (int i = 0; i < clipboard_size; ++i)
        rec[cursor + i] = clipboard[i];

    for (int i = cursor + clipboard_size - 1; i < MAX_RECORDING_SIZE; ++i)
        rec[i] = temp[i - cursor - clipboard_size + 1];
    
    while (*rec_size % 4000 != 0) {
        rec[*rec_size] = 0;
        *rec_size = *rec_size + 1;
    }
}

void play_from_highlight(int rec[], int rec_size, int highlight_start, int highlight_end) {
    for (int i = highlight_start; i < highlight_end; ++i) {
        while (!audiop->wslc || !audiop->wsrc);

        audiop->ldata = rec[i];
        audiop->rdata = rec[i];

        if (buttonp->EDGE & 1 != 0) {
            buttonp->EDGE = 1;
            return;
        }
    }
}

void play_from_cursor(int rec[], int rec_size, int cursor) {
    for (int i = cursor; i < rec_size; ++i) {

        while (!audiop->wslc || !audiop->wsrc);

        audiop->ldata = rec[i];
        audiop->rdata = rec[i];


        if (buttonp->EDGE & 1 != 0) {
            buttonp->EDGE = 1;
            return;
        }
    }
}

int track_editor(int rec[], int* rec_size) {
    int curr_mode = BUTTON_MODE;

    int curr_button = TRACK_EDITOR_FIRST_BUTTON;
    int prev_button = TRACK_EDITOR_FIRST_BUTTON;
    int scroll_speed;

    short int cursor_color = CURSOR_COLOR;

    int curr_cursor = 40000;
    int prev_cursor = 40000;

    int highlight_start = NONE;
    int highlight_end = NONE;

    int max_resolution_index = 9;

    int curr_resolution = max_resolution_index; // max resolution of 200
    int prev_resolution = max_resolution_index;

    int resolution_steps[10] = {1, 5, 10, 50, 75, 100, 125, 150, 175, 200};

    clear_frame_buffers();

    draw_rectangle(EDITING_TRACK_X, EDITING_TRACK_Y, EDITING_TRACK_X + EDITING_TRACK_WIDTH, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT, WAVEFORM_BACKGROUND_COLOR);
    draw_track_editor_buttons();
    plot_recording_window(rec, *rec_size, curr_cursor, EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT / 2, EDITING_TRACK_HEIGHT, EDITING_TRACK_WIDTH, 0x0, resolution_steps[curr_resolution]);

    fbswap();

    draw_rectangle(EDITING_TRACK_X, EDITING_TRACK_Y, EDITING_TRACK_X + EDITING_TRACK_WIDTH, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT, WAVEFORM_BACKGROUND_COLOR);
    draw_track_editor_buttons();
    plot_recording_window(rec, *rec_size, curr_cursor, EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT / 2, EDITING_TRACK_HEIGHT, EDITING_TRACK_WIDTH, 0x0, resolution_steps[curr_resolution]);
    fbswap();

    while (1) {
        *leds = *switches;
        prev_cursor = curr_cursor;
        prev_resolution = curr_resolution;

        highlight_button(prev_button, BACKGROUND_COLOR, 't');
        highlight_button(curr_button, BUTTON_HIGHLIGHT_COLOR, 't');
        fbswap();
        highlight_button(prev_button, BACKGROUND_COLOR, 't');
        highlight_button(curr_button, BUTTON_HIGHLIGHT_COLOR, 't');

        if (curr_mode == BUTTON_MODE) {
            highlight_button(TRACK_OUTLINE, BACKGROUND_COLOR, 't');
            fbswap();
            highlight_button(TRACK_OUTLINE, BACKGROUND_COLOR, 't');
        }
        else {
            highlight_button(TRACK_OUTLINE, BUTTON_HIGHLIGHT_COLOR, 't');
            fbswap();
            highlight_button(TRACK_OUTLINE, BUTTON_HIGHLIGHT_COLOR, 't');
        }

        if (curr_resolution == 0) scroll_speed = 1;
        else scroll_speed = resolution_steps[curr_resolution] * 10;

        // move track left
        if ((buttonp->EDGE >> 3) & 1 != 0) {
            if (curr_mode != BUTTON_MODE) {
                if (curr_cursor - scroll_speed >= 0) curr_cursor = curr_cursor - scroll_speed;
            }
            else {
                prev_button = curr_button;
                if (curr_button > TRACK_EDITOR_FIRST_BUTTON) --curr_button;
                else curr_button = TRACK_EDITOR_LAST_BUTTON;
            }
            buttonp->EDGE = 0b1111;
        }
		
        // move track right
		if ((buttonp->EDGE >> 2) & 1 != 0) {
            if (curr_mode != BUTTON_MODE) {
			    if (curr_cursor + scroll_speed < MAX_RECORDING_SIZE) curr_cursor = curr_cursor + scroll_speed;
            }
            else {
                prev_button = curr_button;
                if (curr_button < TRACK_EDITOR_LAST_BUTTON) ++curr_button;
                else curr_button = TRACK_EDITOR_FIRST_BUTTON;
            }
            buttonp->EDGE = 0b1111;
        }

        if ((buttonp->EDGE >> 1) & 1 != 0) {
            buttonp->EDGE = 0b1111;
            // zooming
            if (curr_mode == ZOOM_MODE) {
                if ((*switches) & 1 == 1) {
                    if (curr_resolution + 1 <= max_resolution_index) ++curr_resolution; // zoom out
                }
                else if (curr_resolution - 1 >= 0) --curr_resolution; // zoom in
            }
            // highlighting
            if (curr_mode == HIGHLIGHT_MODE) {
                if (highlight_start == NONE || highlight_end != NONE) {
                    highlight_start = curr_cursor;
                    highlight_end = NONE;
                    cursor_color = CURSOR_ACTIVATE_COLOR;
                }
                else {
                    highlight_end = curr_cursor;
                    cursor_color = CURSOR_COLOR;
                }
            }
            if (curr_mode == PASTE_MODE) {
                paste_clipboard(rec, rec_size, curr_cursor);
            }
        }

        if (buttonp->EDGE & 1 != 0) {
            buttonp->EDGE = 0b1111;
            if (curr_mode == BUTTON_MODE) {
                button_feedback(curr_button, 't');
                if (curr_button == ZOOM_BUTTON) curr_mode = ZOOM_MODE;
                if (curr_button == HIGHLIGHT_BUTTON) curr_mode = HIGHLIGHT_MODE;
                if (curr_button == PASTE_BUTTON) curr_mode = PASTE_MODE;
                if (curr_button == PLAY_FROM_CURSOR_BUTTON) {
                    if (highlight_end == NONE) play_from_cursor(rec, *rec_size, curr_cursor);
                    else play_from_highlight(rec, *rec_size, highlight_start, highlight_end);
                }
                if (curr_button == EXIT_TRACK_EDITOR_BUTTON) {
                    clear_frame_buffers();
                    return curr_button;
                }
                if (curr_button == DELETE_BUTTON && highlight_start != NONE && highlight_end != NONE) {
                    delete_highlight(rec, rec_size, highlight_start, highlight_end);
                    highlight_start = highlight_end = NONE;
                }
                if (curr_button == MUTE_BUTTON && highlight_start != NONE && highlight_end != NONE) {
                    mute_highlight(rec, highlight_start, highlight_end);
                    highlight_start = highlight_end = NONE;
                }
                if (curr_button == COPY_BUTTON && highlight_start != NONE && highlight_end != NONE) {
                    copy_highlight(rec, highlight_start, highlight_end);
                    highlight_start = highlight_end = NONE;
                }
            }
            else if (curr_mode == HIGHLIGHT_MODE) {
                highlight_end = curr_cursor;
                cursor_color = CURSOR_COLOR;
                curr_mode = BUTTON_MODE;
            }
            else curr_mode = BUTTON_MODE;
        }

        int highlight_x1 = 0;
        int highlight_x2 = 0;

        draw_rectangle(EDITING_TRACK_X, EDITING_TRACK_Y, EDITING_TRACK_X + EDITING_TRACK_WIDTH, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT, WAVEFORM_BACKGROUND_COLOR);

        if (highlight_start != NONE) {
            highlight_x1 = EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2 + (highlight_start - curr_cursor) / resolution_steps[curr_resolution];
            highlight_x2 = EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2 + (highlight_end - curr_cursor) / resolution_steps[curr_resolution];

            if (highlight_x1 < EDITING_TRACK_X) highlight_x1 = EDITING_TRACK_X;
            if (highlight_x1 > EDITING_TRACK_X + EDITING_TRACK_WIDTH) highlight_x1 = EDITING_TRACK_X + EDITING_TRACK_WIDTH;

            if (highlight_x2 < EDITING_TRACK_X) highlight_x2 = EDITING_TRACK_X;
            if (highlight_x2 > EDITING_TRACK_X + EDITING_TRACK_WIDTH) highlight_x2 = EDITING_TRACK_X + EDITING_TRACK_WIDTH;

            highlight_x2 = (highlight_end == NONE) ? EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2 : highlight_x2;

            draw_rectangle(highlight_x2, EDITING_TRACK_Y, highlight_x1, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT, HIGHLIGHT_COLOR);
        }
        plot_recording_window(rec, *rec_size, curr_cursor, EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT / 2, EDITING_TRACK_HEIGHT, EDITING_TRACK_WIDTH, 0x0, resolution_steps[curr_resolution]);
        draw_line(EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2, EDITING_TRACK_Y, EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT - 1, cursor_color);

        fbswap();

        draw_rectangle(EDITING_TRACK_X, EDITING_TRACK_Y, EDITING_TRACK_X + EDITING_TRACK_WIDTH, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT, WAVEFORM_BACKGROUND_COLOR);
        if (highlight_start != NONE) draw_rectangle(highlight_x2, EDITING_TRACK_Y, highlight_x1, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT, HIGHLIGHT_COLOR);
        plot_recording_window(rec, *rec_size, curr_cursor, EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT / 2, EDITING_TRACK_HEIGHT, EDITING_TRACK_WIDTH, 0x0, resolution_steps[curr_resolution]);
        draw_line(EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2, EDITING_TRACK_Y, EDITING_TRACK_X + EDITING_TRACK_WIDTH / 2, EDITING_TRACK_Y + EDITING_TRACK_HEIGHT - 1, cursor_color);
    }

    
        
}

// KEY3 moves left, KEY2 moves right
// menu returns button selected
int menu(int starting_button) {

    draw_menu_buttons();
    fbswap();
    draw_menu_buttons();

    int prev_button = starting_button;
    int curr_button = starting_button;

    while (1) {

        highlight_button(prev_button, BACKGROUND_COLOR, 'm');
        highlight_button(curr_button, BUTTON_HIGHLIGHT_COLOR, 'm');
        fbswap();
        highlight_button(prev_button, BACKGROUND_COLOR, 'm');
        highlight_button(curr_button, BUTTON_HIGHLIGHT_COLOR, 'm');

        // fucking bit shift because immediate value always results in andi r2, r2, 1 for some reason

        if ((buttonp->EDGE >> 3) & 1 != 0) {
			prev_button = curr_button;
            if (curr_button > MENU_FIRST_BUTTON) --curr_button;
            else curr_button = MENU_LAST_BUTTON;
			buttonp->EDGE = 0b1111;
        }
		
		if ((buttonp->EDGE >> 2) & 1 != 0) {
			prev_button = curr_button;
            if (curr_button < MENU_LAST_BUTTON) ++curr_button;
            else curr_button = MENU_FIRST_BUTTON;
			buttonp->EDGE = 0b1111;
        }
		
		if (buttonp->EDGE & 1 != 0) {
			buttonp->EDGE = 0b1111;
			return curr_button;
		}
    }
}

const int COUNTDOWN_Y = 50;
const int COUNTDOWN_X = 140;

void countdown() {
    int count = 0;

    draw_rectangle(COUNTDOWN_X, COUNTDOWN_Y, COUNTDOWN_X + 40, COUNTDOWN_Y + 20, PLAY_ALL_COLOR);
    fbswap();
    draw_rectangle(COUNTDOWN_X, COUNTDOWN_Y, COUNTDOWN_X + 40, COUNTDOWN_Y + 20, PLAY_ALL_COLOR);

    draw_string(160, COUNTDOWN_Y + 10, "1");

    for (int count = 0; count < 25000000; ++count);

    draw_string(160, COUNTDOWN_Y + 10, "2");

    for (int count = 0; count < 25000000; ++count);

    draw_string(160, COUNTDOWN_Y + 10, "3");

    for (int count = 0; count < 25000000; ++count);

    draw_string(160, COUNTDOWN_Y + 10, "4");

    for (int count = 0; count < 25000000; ++count);

    draw_string(160, COUNTDOWN_Y + 10, " ");

    draw_rectangle(COUNTDOWN_X, COUNTDOWN_Y, COUNTDOWN_X + 40, COUNTDOWN_Y + 20, WAVEFORM_BACKGROUND_COLOR);
    fbswap();
    draw_rectangle(COUNTDOWN_X, COUNTDOWN_Y, COUNTDOWN_X + 40, COUNTDOWN_Y + 20, WAVEFORM_BACKGROUND_COLOR);

    return;
}

int main(void)
{
    // initialize backbuffer
    vp->bfbp = (int) &buffer2;

    vp->bfbp = (int) &buffer1;
    clear_screen();
    fbswap();
    vp->bfbp = (int) &buffer2;
    clear_screen();
    fbswap();

    int menu_button = 0;
    int editor_button = 0;

    int cursor = 40000;
    int curr_resolution = 250;

    // initialize UI
    while (1) {
        // open menu
        // when user selects a button, the result is returned by menu into button_pressed
        menu_button = menu(menu_button);
        button_feedback(menu_button, 'm');

        if (menu_button == REC_1_BUTTON) {
            countdown();
            rec1_size = 0;
            record_and_plot(rec1, &rec1_size, WAVEFORM_X, WAVEFORM_Y, WAVEFORM_THICKNESS, 1);
        }

        if (menu_button == REC_2_BUTTON) {
            countdown();
            rec2_size = 0;
            record_and_plot(rec2, &rec2_size, WAVEFORM_X, WAVEFORM_Y, WAVEFORM_THICKNESS, 2);
        }

        if (menu_button == REC_3_BUTTON) {
            countdown();
            rec3_size = 0;
            record_and_plot(rec3, &rec3_size, WAVEFORM_X, WAVEFORM_Y, WAVEFORM_THICKNESS, 3);
        }

        audiop->control = 0xc;
        audiop->control = 0;

        int plot_frequency = 3200;

        if (menu_button == PLAY_1_BUTTON) {
            for (int i = 0; i < rec1_size; ++i) {
                while (!audiop->wslc || !audiop->wsrc);
                audiop->ldata = rec1[i];
                audiop->rdata = rec1[i];

                if (i % plot_frequency == 0) {
                    plot_recording(rec1, i + 1 - 2*plot_frequency, WAVEFORM_X, WAVEFORM_Y, WAVEFORM_THICKNESS, WAVEFORM_BACKGROUND_COLOR, 10);
                    plot_recording(rec1, i + 1, WAVEFORM_X, WAVEFORM_Y, WAVEFORM_THICKNESS, 0x0, 10);
                    fbswap();
                }
            }
        }

        if (menu_button == PLAY_2_BUTTON) {
            for (int i = 0; i < rec2_size; ++i) {
                while (!audiop->wslc || !audiop->wsrc);
                audiop->ldata = rec2[i];
                audiop->rdata = rec2[i];

                if (i % plot_frequency == 0) {
                    plot_recording(rec2, i + 1 - 2*plot_frequency, WAVEFORM_X, WAVEFORM_Y, WAVEFORM_THICKNESS, WAVEFORM_BACKGROUND_COLOR, 10);
                    plot_recording(rec2, i + 1, WAVEFORM_X, WAVEFORM_Y, WAVEFORM_THICKNESS, 0x0, 10);
                    fbswap();
                }
            }
        }

        if (menu_button == PLAY_3_BUTTON) {
            for (int i = 0; i < rec3_size; ++i) {
                while (!audiop->wslc || !audiop->wsrc);
                audiop->ldata = rec3[i];
                audiop->rdata = rec3[i];

                if (i % plot_frequency == 0) {
                    plot_recording(rec3, i + 1 - 2*plot_frequency, WAVEFORM_X, WAVEFORM_Y, WAVEFORM_THICKNESS, WAVEFORM_BACKGROUND_COLOR, 10);
                    plot_recording(rec3, i + 1, WAVEFORM_X, WAVEFORM_Y, WAVEFORM_THICKNESS, 0x0, 10);
                    fbswap();
                }
            }
        }

        if (menu_button == DELETE_1_BUTTON) rec1_size = 0;
        if (menu_button == DELETE_2_BUTTON) rec2_size = 0;
        if (menu_button == DELETE_3_BUTTON) rec3_size = 0;

        if (menu_button == EDIT_BUTTON) {
            clear_frame_buffers();
            while (1) {
                editor_button = editor(editor_button, cursor, curr_resolution);
                button_feedback(editor_button, 'e');
                if (editor_button == EXIT_EDITOR_BUTTON) break;
                if (editor_button == TRACK_1_SELECT) track_editor(rec1, &rec1_size);
                if (editor_button == TRACK_2_SELECT) track_editor(rec2, &rec2_size);
                if (editor_button == TRACK_3_SELECT) track_editor(rec3, &rec3_size);
                if (editor_button == PLAY_ALL_BUTTON) play_all();
                if (editor_button == TRACK_1_LOOP) looping_1 = (looping_1) ? false : true;
                if (editor_button == TRACK_2_LOOP) looping_2 = (looping_2) ? false : true;
                if (editor_button == TRACK_3_LOOP) looping_3 = (looping_3) ? false : true;
            }
            clear_frame_buffers();
        }

        // clear remaining waveform
        draw_rectangle(319 - WAVEFORM_X - 1, WAVEFORM_Y - 1 - (WAVEFORM_THICKNESS >> 1), WAVEFORM_X , WAVEFORM_Y + (WAVEFORM_THICKNESS >> 1), WAVEFORM_BACKGROUND_COLOR);
        fbswap();
        draw_rectangle(319 - WAVEFORM_X - 1, WAVEFORM_Y - 1 - (WAVEFORM_THICKNESS >> 1), WAVEFORM_X , WAVEFORM_Y + (WAVEFORM_THICKNESS >> 1), WAVEFORM_BACKGROUND_COLOR);
    }
}
