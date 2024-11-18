/*
 * tiles.c
 * program which demonstraes tile mode 0
 */

/* include the image we are using */
#include "background.h"

/* include the tile map we are using */
#include "map.h"
#include "map2.h"

// include sprite files
#include "koopa.h"
#include "sprite.h"

/* the width and height of the screen */
#define WIDTH 240
#define HEIGHT 160

/* the three tile modes */
#define MODE0 0x00
#define MODE1 0x01
#define MODE2 0x02

// define sprite enable bits
#define SPRITE_ENABLE 0x1000 

/* enable bits for the four tile layers */
#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800

/* the control registers for the four tile layers */
volatile unsigned short* bg0_control = (volatile unsigned short*) 0x4000008;
volatile unsigned short* bg1_control = (volatile unsigned short*) 0x400000a;
volatile unsigned short* bg2_control = (volatile unsigned short*) 0x400000c;
volatile unsigned short* bg3_control = (volatile unsigned short*) 0x400000e;

/* palette is always 256 colors */
#define PALETTE_SIZE 256

/* the display control pointer points to the gba graphics register */
volatile unsigned long* display_control = (volatile unsigned long*) 0x4000000;

/* the address of the color palette */
volatile unsigned short* bg_palette = (volatile unsigned short*) 0x5000000;

/* the scanline counter is a memory cell which is updated to indicate how
 * much of the screen has been drawn */
volatile unsigned short* scanline_counter = (volatile unsigned short*) 0x4000006;


/* wait for the screen to be fully drawn so we can do something during vblank */
void wait_vblank() {
    /* wait until all 160 lines have been updated */
    while (*scanline_counter < 160) { }
}

/* return a pointer to one of the 4 character blocks (0-3) */
volatile unsigned short* char_block(unsigned long block) {
    /* they are each 16K big */
    return (volatile unsigned short*) (0x6000000 + (block * 0x4000));
}

/* return a pointer to one of the 32 screen blocks (0-31) */
volatile unsigned short* screen_block(unsigned long block) {
    /* they are each 2K big */
    return (volatile unsigned short*) (0x6000000 + (block * 0x800));
}


/* function to setup background 0 for this program */
void setup_background() {

    /* load the palette from the image into palette memory*/
    for (int i = 0; i < PALETTE_SIZE; i++) {
        bg_palette[i] = background_palette[i];
    }

    /* load the image into char block 0 (16 bits at a time) */
    volatile unsigned short* dest = char_block(0);
    unsigned short* image = (unsigned short*) background_data;
    for (int i = 0; i < ((background_width * background_height) / 2); i++) {
        dest[i] = image[i];
    }

    /* set all control the bits in this register */
    *bg0_control = 1 |    /* priority, 0 is highest, 3 is lowest */
        (0 << 2)  |       /* the char block the image data is stored in */
        (0 << 6)  |       /* the mosaic flag */
        (1 << 7)  |       /* color mode, 0 is 16 colors, 1 is 256 colors */
        (16 << 8) |       /* the screen block the tile data is stored in */
        (1 << 13) |       /* wrapping flag */
        (0 << 14);        /* bg size, 0 is 256x256 */

    /* set all control bits in bg1_control register */
    *bg1_control = 0 |    /* priority, 0 is highest, 3 is lowest */
    (0 << 2)  |       /* the char block the image data is stored in */
    (0 << 6)  |       /* the mosaic flag */
    (1 << 7)  |       /* color mode, 0 is 16 colors, 1 is 256 colors */
    (17 << 8) |       /* the screen block the tile data is stored in (using 17 instead of 16) */
    (1 << 13) |       /* wrapping flag */
    (0 << 14);        /* bg size, 0 is 256x256 */

    /* load the tile data into screen block 16 */
    dest = screen_block(16);
    for (int i = 0; i < (map_width * map_height); i++) {
        dest[i] = map[i];
    }
    /* load the tile data into screen block 17 */
    dest = screen_block(17);
    for (int i = 0; i < (map2_width * map2_height); i++) {
        dest[i] = map2[i];
    }
}
void delay(unsigned int amount) {
    for (int i = 0; i < amount * 10; i++);
}

/* the main function */
int main() {
    /* we set the mode to mode 0 with bg0 on */
    *display_control = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;

    /* setup the background 0 */
    setup_background();

    struct Sprite* koopa = sprite_init(40, 20, SIZE_16_32, 0, 0, 0, 0); // Using SIZE_16_32 for a single frame
    /* loop forever */
    while (1) {
        wait_vblank();
        //update sprite
        sprite_update_all();
        /* delay some */
        delay(50);
    }
}

