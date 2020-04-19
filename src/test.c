#include <assert.h>
#include "test.h"
#include "rasterization.h"
#include "simulation.h"
#include "extern/stb_image_write.h"
#include "extern/palette.h"

//-----------------------------------------------------------------------------
void test_clear(image_buffers* image)
{
    clear_image(image, na16_light_grey);

    int result = stbi_write_png("test_clear.png", image->width, image->height, 4, image->color_buffer, sizeof(uint32_t) * image->width);

    assert(result != 0);
    assert(image->color_buffer[237] == na16_light_grey);
}

//-----------------------------------------------------------------------------
void test_rasterization(image_buffers* image)
{
    rasterize_disc(image, (vec2){0.5f, 0.25f}, 0.25f, na16_pink, 0, image->height);
    rasterize_line(image, (vec2) {0.1f, 0.1f}, (vec2) {0.3f, 0.3f}, 0.05f, na16_orange, 0, image->height);

    int result = stbi_write_png("test_rasterization.png", image->width, image->height, 4, image->color_buffer, sizeof(uint32_t) * image->width);

    assert(result != 0);
}
