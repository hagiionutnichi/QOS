#include "BasicRenderer.h"

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