
typedef unsigned long long size_t;

typedef struct {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} FrameBuffer;

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;

typedef struct {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

void putChar(FrameBuffer* framebuffer, PSF1_FONT* psf1_font, unsigned int colour, char chr, unsigned int xOff, unsigned int yOff)
{
    unsigned int* base = (unsigned int*)framebuffer->BaseAddress;
    char* fontPtr = psf1_font->glyphBuffer + (chr * psf1_font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + 16; y++){
        for (unsigned long x = xOff; x < xOff+8; x++){
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0){
                    *(unsigned int*)(base + x + (y * framebuffer->PixelsPerScanLine)) = colour;
                }

        }
        fontPtr++;
    }
}

void _start(FrameBuffer* framebuffer, PSF1_FONT* font)  {
    putChar(framebuffer, font, 0xFFFFFF00, 'H', 0, 10);
    putChar(framebuffer, font, 0xFFFFFF00, 'e', 1, 10);
    putChar(framebuffer, font, 0xFFFFFF00, 'l', 2, 10);
    putChar(framebuffer, font, 0xFFFFFF00, 'l', 3, 10);
    putChar(framebuffer, font, 0xFFFFFF00, 'o', 4, 10);

    return;
}