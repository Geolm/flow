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

    int result = stbi_write_png("test_clear.png", image->width, image->height, 4, image->color_buffer, 4);
    assert(result != 0);
}

//-----------------------------------------------------------------------------
void test_rasterization(image_buffers* image)
{
    
}
