#include "sprite.h"
// Define sprite memory and palette memory (volatile pointers)
volatile unsigned short* sprite_attribute_memory = (volatile unsigned short*) 0x7000000;

// Define the array of sprites and next index tracker
struct Sprite sprites[NUM_SPRITES];
int next_sprite_index = 0;

// Initialize a sprite
struct Sprite* sprite_init(int x, int y, enum SpriteSize size, int horizontal_flip, int vertical_flip, int tile_index, int priority) {
    int index = next_sprite_index++;
    int size_bits, shape_bits;

    switch (size) {
        case SIZE_8_8:   size_bits = 0; shape_bits = 0; break;
        case SIZE_16_16: size_bits = 1; shape_bits = 0; break;
        case SIZE_32_32: size_bits = 2; shape_bits = 0; break;
        case SIZE_64_64: size_bits = 3; shape_bits = 0; break;
        case SIZE_16_8:  size_bits = 0; shape_bits = 1; break;
        case SIZE_32_8:  size_bits = 1; shape_bits = 1; break;
        case SIZE_32_16: size_bits = 2; shape_bits = 1; break;
        case SIZE_64_32: size_bits = 3; shape_bits = 1; break;
        case SIZE_8_16:  size_bits = 0; shape_bits = 2; break;
        case SIZE_8_32:  size_bits = 1; shape_bits = 2; break;
        case SIZE_16_32: size_bits = 2; shape_bits = 2; break;
        case SIZE_32_64: size_bits = 3; shape_bits = 2; break;
    }

    int h = horizontal_flip ? 1 : 0;
    int v = vertical_flip ? 1 : 0;

    sprites[index].attribute0 = y | (0 << 8) | (0 << 10) | (0 << 12) | (1 << 13) | (shape_bits << 14);
    sprites[index].attribute1 = x | (0 << 9) | (h << 12) | (v << 13) | (size_bits << 14);
    sprites[index].attribute2 = tile_index | (priority << 10) | (0 << 12);

    return &sprites[index];
}

// Set a sprite's position
void sprite_position(struct Sprite* sprite, int x, int y) {
    sprite->attribute0 = (sprite->attribute0 & 0xff00) | (y & 0xff);
    sprite->attribute1 = (sprite->attribute1 & 0xfe00) | (x & 0x1ff);
}

// Move a sprite
void sprite_move(struct Sprite* sprite, int dx, int dy) {
    int y = sprite->attribute0 & 0xff;
    int x = sprite->attribute1 & 0x1ff;
    sprite_position(sprite, x + dx, y + dy);
}
void sprite_update_all() {
    for (int i = 0; i < next_sprite_index; i++) {
        sprite_attribute_memory[i * 4 + 0] = sprites[i].attribute0; // Attribute 0
        sprite_attribute_memory[i * 4 + 1] = sprites[i].attribute1; // Attribute 1
        sprite_attribute_memory[i * 4 + 2] = sprites[i].attribute2; // Attribute 2
        sprite_attribute_memory[i * 4 + 3] = 0; // Attribute 3 (not used in this case)
    }
}
// Additional functions like sprite_set_vertical_flip, sprite_set_horizontal_flip, sprite_set_offset, etc., can be implemented similarly.
