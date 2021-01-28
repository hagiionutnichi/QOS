#include "BasicRenderer.h"
#include "Framebuffer.h"
#include "PSF_1.h"
#include "BMP.h"
#include "math.h"
#include "numstring.h"

extern "C" void _start(FrameBuffer* framebuffer, PSF1_FONT* font, BMP_IMAGE* albie)  {

    BasicRenderer renderer = BasicRenderer(framebuffer, font);
    renderer.SetCursor(0, 0);
    for (int t = 0; t < 50; t+=1){
        renderer.Print("Hello Kernel");
    }
    
    renderer.DrawBMP(albie, 320, 200);

    renderer.Print(to_hstring((uint32_t) 0x123f456a));
    return ;
}