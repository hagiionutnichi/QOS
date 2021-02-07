#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;
BasicRenderer::BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* psf1_font)
{
    this->Framebuffer = framebuffer;
    this->font = psf1_font;
}

void BasicRenderer::PutChar(unsigned int colour, char chr, unsigned int xOff, unsigned int yOff)
{
    unsigned int* base = (unsigned int*)Framebuffer->BaseAddress;
    char* fontPtr = (char *) font->glyphBuffer + (chr * font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + 16; y++){
        for (unsigned long x = xOff; x < xOff+8; x++){
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0){
                    *(unsigned int*)(base + x + (y * Framebuffer->PixelsPerScanLine)) = colour;
                }

        }
        fontPtr++;
    }
}

void BasicRenderer::PutPixel(unsigned int colour, unsigned int x, unsigned int y)
{
    unsigned int* base = (unsigned int*)Framebuffer->BaseAddress;
    *(unsigned int*)(base + x + (y * Framebuffer->PixelsPerScanLine)) = colour;
}

void BasicRenderer::Print(const char* str, unsigned int colour)
{
    
    char* chr = (char *) str;
    while(*chr != 0){
        PutChar(colour, *chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;
        if(CursorPosition.X + 8 > Framebuffer->Width)
        {
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }
        chr++;
    }
}

void BasicRenderer::PrintChar(char chr, unsigned int colour)
{
    PutChar(colour, chr, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X+=8;
    if(CursorPosition.X + 8 > Framebuffer->Width)
    {
        CursorPosition.X = 0;
        CursorPosition.Y += 16;
    }
}

void BasicRenderer::DrawBMP(BMP_IMAGE* image, unsigned int xOff, unsigned int yOff) {
    unsigned int x = xOff;
    unsigned int y = yOff + image->bmp_dib_header->height;
    unsigned int* col;
    size_t count = image->bmp_dib_header->width * image->bmp_dib_header->height;
    for(col = image->pixels; col < image->pixels + count; col++) {
        PutPixel(*col, x, y);
        x++;
        if(x - xOff + 1 > image->bmp_dib_header->width)
        {
            x = xOff;
            y -= 1;
        }
    }
}

void BasicRenderer::SetCursor(unsigned int x, unsigned int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
}
void BasicRenderer::Clear(uint32_t colour) {
    uint64_t fbBase = (uint64_t)Framebuffer->BaseAddress;
    uint64_t fbBytesPerScanline = Framebuffer->PixelsPerScanLine * 4;
    uint64_t fbHeight = Framebuffer->Height;
    uint64_t fbSize = Framebuffer->BufferSize;
    for(int vsc = 0; vsc < fbHeight; vsc++) {
        uint64_t pixPtrBase = fbBase + (fbBytesPerScanline * vsc);
        for(uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + fbBytesPerScanline); pixPtr++) {
            *pixPtr = colour;
        }
    }

}
void BasicRenderer::NewLine(){
    CursorPosition.X = 0;
    CursorPosition.Y += 16;
}

void BasicRenderer::Backspace() {
    if(CursorPosition.X == 0) {
        CursorPosition.X = Framebuffer->Width;
        CursorPosition.Y -= 16;
        if(CursorPosition.Y < 0) CursorPosition.Y = 0;
    }

    //Clear 8x16
    unsigned int* base = (unsigned int*)Framebuffer->BaseAddress;
    for (unsigned long y = CursorPosition.Y; y < CursorPosition.Y + 16; y++)
        for (unsigned long x = CursorPosition.X - 8; x < CursorPosition.X; x++)
            *(unsigned int*)(base + x + (y * Framebuffer->PixelsPerScanLine)) = 0x00000000;
    
    CursorPosition.X -= 8;

    if(CursorPosition.X < 0) {
        CursorPosition.X = Framebuffer->Width;
        CursorPosition.Y -= 16;
        if(CursorPosition.Y < 0) CursorPosition = {0, 0};
    }
}