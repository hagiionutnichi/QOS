#pragma once

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