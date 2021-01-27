
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

typedef struct {
    unsigned int X;
    unsigned int Y;
} Point;

typedef struct __attribute__ ((packed)){
	unsigned char magic[2];
	unsigned int fileSize;
	unsigned char res[4];
	unsigned int pixelAddress;
} BMP_HEADER;

typedef struct __attribute__ ((packed)){
	unsigned int dibSize;
	signed int width;
	signed int height;
	unsigned short colorPlanes;
	unsigned short bitsPerPixel;
	unsigned int compressionMethod;
	unsigned int rawSize;
	signed int horizontalResolution;
	signed int verticalResolution;
	unsigned int numberOfColors;
	unsigned int numberOfImportantColors;

} BMP_DIB_HEADER;

typedef struct {
	BMP_HEADER* bmp_header;
	BMP_DIB_HEADER* bmp_dib_header;
	unsigned int* pixels;
} BMP_IMAGE;

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

void putPixel(FrameBuffer* framebuffer, unsigned int colour, unsigned int x, unsigned int y)
{
    unsigned int* base = (unsigned int*)framebuffer->BaseAddress;
    *(unsigned int*)(base + x + (y * framebuffer->PixelsPerScanLine)) = colour;
}

Point CursorPosition;
void Print(FrameBuffer* framebuffer, PSF1_FONT* psf1_font, unsigned int colour, char* str)
{
    
    char* chr = str;
    while(*chr != 0){
        putChar(framebuffer, psf1_font, colour, *chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;
        if(CursorPosition.X + 8 > framebuffer->Width)
        {
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }
        chr++;
    }
}

void Draw(FrameBuffer* framebuffer, BMP_IMAGE* image, unsigned int xOff, unsigned int yOff) {
    unsigned int x = xOff;
    unsigned int y = yOff + image->bmp_dib_header->height;
    unsigned int* col;
    size_t count = image->bmp_dib_header->width * image->bmp_dib_header->height;
    for(col = image->pixels; col <= image->pixels + count; col++) {
        putPixel(framebuffer, *col, x, y);
        x++;
        if(x - xOff + 1 > image->bmp_dib_header->width)
        {
            x = xOff;
            y -= 1;
        }
    }
}

void _start(FrameBuffer* framebuffer, PSF1_FONT* font, BMP_IMAGE* albie)  {
    CursorPosition.X = 0;
    CursorPosition.Y = 0;
    for (int t = 0; t < 50; t+=1){
        Print(framebuffer, font, 0xffffffff, "Hello Kernel Hello Kernel");
    }
    
    Draw(framebuffer, albie, 320, 200);
    // putPixel(framebuffer, *((unsigned int*)(__UINTPTR_TYPE__)albie->bmp_header->pixelAddress + 0), 0, 0);
    // putPixel(framebuffer, *((unsigned int*)(__UINTPTR_TYPE__)albie->bmp_header->pixelAddress + 1), 0, 1);
    // putPixel(framebuffer, *((unsigned int*)(__UINTPTR_TYPE__)albie->bmp_header->pixelAddress + 2), 0, 2);
    // putPixel(framebuffer, *((unsigned int*)(__UINTPTR_TYPE__)albie->bmp_header->pixelAddress + 3), 0, 3);
    // putPixel(framebuffer, *((unsigned int*)(__UINTPTR_TYPE__)albie->bmp_header->pixelAddress + 4), 0, 4);
    return ;
}