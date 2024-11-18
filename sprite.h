// Define constants
#define NUM_SPRITES 128
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

// Structure representing a Sprite's attributes
struct Sprite {
    unsigned short attribute0;
    unsigned short attribute1;
    unsigned short attribute2;
    unsigned short attribute3;
};

// Enumeration for different sprite sizes
enum SpriteSize {
    SIZE_8_8,
    SIZE_16_16,
    SIZE_32_32,
    SIZE_64_64,
    SIZE_16_8,
    SIZE_32_8,
    SIZE_32_16,
    SIZE_64_32,
    SIZE_8_16,
    SIZE_8_32,
    SIZE_16_32,
    SIZE_32_64
};

// Global array to hold all sprites

extern volatile unsigned short* bg0_control;
extern volatile unsigned short* bg1_control;
extern volatile unsigned short* bg2_control;
extern volatile unsigned short* bg3_control;

extern volatile unsigned long* display_control;
extern volatile unsigned short* bg_palette;
extern volatile unsigned short* scanline_counter;
extern struct Sprite sprites[NUM_SPRITES];
extern int next_sprite_index;

// Function declarations
struct Sprite* sprite_init(int x, int y, enum SpriteSize size, int horizontal_flip, int vertical_flip, int tile_index, int priority);
void sprite_position(struct Sprite* sprite, int x, int y);
void sprite_move(struct Sprite* sprite, int dx, int dy);
void sprite_set_vertical_flip(struct Sprite* sprite, int vertical_flip);
void sprite_set_horizontal_flip(struct Sprite* sprite, int horizontal_flip);
void sprite_set_offset(struct Sprite* sprite, int offset);
void sprite_update_all();
