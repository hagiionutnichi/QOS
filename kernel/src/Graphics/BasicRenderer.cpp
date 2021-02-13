#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;
BasicRenderer::BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* psf1_font)
{
    this->Framebuffer = framebuffer;
    this->font = psf1_font;
}

void memory_copy(uint32_t *source, uint32_t *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void BasicRenderer::SetClearColour(uint32_t colour) {
    this->ClearColour = colour;
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
        PrintChar(*chr, colour);
        // CursorPosition.X+=8;
        // if(CursorPosition.X + 8 > Framebuffer->Width)
        // {
        //     CursorPosition.X = 0;
        //     CursorPosition.Y += 16;
        // }
        chr++;
    }
}

void BasicRenderer::PrintLn(const char* str, unsigned int colour)
{
    Print(str, colour);
    NewLine();
}

void BasicRenderer::PrintChar(char chr, unsigned int colour)
{
    unsigned int* base = (unsigned int*)Framebuffer->BaseAddress;

    PutChar(colour, chr, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X+=8;
    if(CursorPosition.X + 8 > Framebuffer->Width)
    {
        CursorPosition.X = 0;
        CursorPosition.Y += 16;
    }
        /* Check if the offset is over screen size and scroll */
    if (CursorPosition.Y > Framebuffer->Height - 16) {
        uint8_t distance = CursorPosition.Y - Framebuffer->Height + 16;
        int i;
        for (i = distance; i < Framebuffer->Height; i++) 
            memory_copy((uint32_t*)(base + (i * Framebuffer->PixelsPerScanLine)),
                    (uint32_t*)(base + ((i-distance) * Framebuffer->PixelsPerScanLine)),
                    Framebuffer->PixelsPerScanLine);

        /* Blank last line */
        uint32_t *last_line = (uint32_t*)(base + ((Framebuffer->Height-16) * Framebuffer->PixelsPerScanLine));
        for(size_t t = 0; t < 16; t++)
            for (i = t * Framebuffer->PixelsPerScanLine; i < t * Framebuffer->PixelsPerScanLine + Framebuffer->PixelsPerScanLine; i++) last_line[i] = 0;

        CursorPosition.X = 0;
        CursorPosition.Y = Framebuffer->Height - 16;
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
    //Clear screen
    for(size_t y = 0; y < Framebuffer->Height; y++) 
        for(size_t x = 0; x < Framebuffer->PixelsPerScanLine; x++)
            PutPixel(colour, x, y);

    GlobalRenderer->SetCursor(0, 0);
    GlobalRenderer->SetClearColour(colour);

}
void BasicRenderer::NewLine(){
    CursorPosition.X = 0;
    CursorPosition.Y += 16;
    long xOff = CursorPosition.X;
    long yOff = CursorPosition.Y;

    unsigned int* base = (unsigned int*)Framebuffer->BaseAddress;

    /* Check if the offset is over screen size and scroll */
    if (CursorPosition.Y > Framebuffer->Height - 16) {
        uint8_t distance = CursorPosition.Y - Framebuffer->Height + 16;
        int i;
        for (i = distance; i < Framebuffer->Height; i++) 
            memory_copy((uint32_t*)(base + (i * Framebuffer->PixelsPerScanLine)),
                    (uint32_t*)(base + ((i-distance) * Framebuffer->PixelsPerScanLine)),
                    Framebuffer->PixelsPerScanLine);

        /* Blank last line */
        uint32_t *last_line = (uint32_t*)(base + ((Framebuffer->Height-16) * Framebuffer->PixelsPerScanLine));
        for(size_t t = 0; t < 16; t++)
            for (i = t * Framebuffer->PixelsPerScanLine; i < t * Framebuffer->PixelsPerScanLine + Framebuffer->PixelsPerScanLine; i++) last_line[i] = 0;

        CursorPosition.X = 0;
        CursorPosition.Y = Framebuffer->Height - 16;
    }
}

void BasicRenderer::Backspace() {
     if (CursorPosition.X == 0){
        CursorPosition.X = Framebuffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0){
            CursorPosition = {0, 0};
            return;
        }
    }


    unsigned int xOff = CursorPosition.X;
    unsigned int yOff = CursorPosition.Y;

    unsigned int* pixPtr = (unsigned int*)Framebuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + 16; y++){
        for (unsigned long x = xOff - 8; x < xOff; x++){
                    *(unsigned int*)(pixPtr + x + (y * Framebuffer->PixelsPerScanLine)) = ClearColour;
        }
    }

    CursorPosition.X -= 8;

    if (CursorPosition.X < 0){
        CursorPosition.X = Framebuffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0) CursorPosition = {0, 0};
    }

}


void BasicRenderer::ClearMouseCursor(uint8_t* mouseCursor, Point position){
    if (!MouseDrawn) return;

    int xMax = 16;
    int yMax = 16;
    int differenceX = Framebuffer->Width - position.X;
    int differenceY = Framebuffer->Height - position.Y;

    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int y = 0; y < yMax; y++){
        for (int x = 0; x < xMax; x++){
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouseCursor[byte] & (0b10000000 >> (x % 8))))
            {
                if (GetPix(position.X + x, position.Y + y) == MouseCursorBufferAfter[x + y *16]){
                    PutPixel(MouseCursorBuffer[x + y * 16], position.X + x, position.Y + y);
                }
            }
        }
    }
}

void BasicRenderer::DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t colour){


    int xMax = 16;
    int yMax = 16;
    int differenceX = Framebuffer->Width - position.X;
    int differenceY = Framebuffer->Height - position.Y;

    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int y = 0; y < yMax; y++){
        for (int x = 0; x < xMax; x++){
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouseCursor[byte] & (0b10000000 >> (x % 8))))
            {
                MouseCursorBuffer[x + y * 16] = GetPix(position.X + x, position.Y + y);
                PutPixel(colour, position.X + x, position.Y + y);
                MouseCursorBufferAfter[x + y * 16] = GetPix(position.X + x, position.Y + y);

            }
        }
    }

    MouseDrawn = true;
}

uint32_t BasicRenderer::GetPix(uint32_t x, uint32_t y){
    return *(uint32_t*)((uint64_t)Framebuffer->BaseAddress + (x*4) + (y * Framebuffer->PixelsPerScanLine * 4));
}


void BasicRenderer::SetPrimaryColour(uint32_t colour) {
    PrimaryColour = colour;
}

void BasicRenderer::SetSecondaryColour(uint32_t colour) {
    SecondaryColour = colour;
}

uint32_t BasicRenderer::GetPrimaryColour() { return PrimaryColour; }

uint32_t BasicRenderer::GetSecondaryColour() { return SecondaryColour; }