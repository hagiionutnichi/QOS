#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stdint.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04
#define BMP_MAGIC0 0x42
#define BMP_MAGIC1 0x4D

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

FrameBuffer framebuffer;
FrameBuffer* InitializeGOP() {
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;

	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
	if(EFI_ERROR(status)) {
		Print(L"Could not locate GOP\n\r");
		return NULL;
	} else {
		Print(L"GOP located\n\r");
	}

	framebuffer.BaseAddress = (void*) gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return &framebuffer;
}

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* Result;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	if(Directory == NULL) {
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS status = Directory->Open(Directory, &Result, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

	if(status != EFI_SUCCESS) {
		return NULL;
	}

	return Result;
}

PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if(font == NULL) return NULL;

	PSF1_HEADER* fontHeader;
	UINTN size = sizeof(PSF1_HEADER);
	SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&fontHeader);
	font->Read(font, &size, fontHeader);
	if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1)
		return NULL;

	UINTN glyphBufferSize = fontHeader->mode == 1 ? 512 : 256;
	glyphBufferSize *= fontHeader->charsize;

	void* glyphBuffer;
	{
		font->SetPosition(font, size);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	PSF1_FONT* loadedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&loadedFont);
	loadedFont->glyphBuffer = glyphBuffer;
	loadedFont->psf1_Header = fontHeader;
	return loadedFont;
}

BMP_IMAGE* LoadBMP(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* bmp = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if(bmp == NULL) return NULL;

	BMP_HEADER* bmpHeader;
	UINTN size = 14;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&bmpHeader);
	bmp->Read(bmp, &size, bmpHeader);
	if (bmpHeader->magic[0] != BMP_MAGIC0 || bmpHeader->magic[1] != BMP_MAGIC1)
		return NULL;

	BMP_DIB_HEADER* bmpDibHeader;
	size = 40;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&bmpDibHeader);
	bmp->Read(bmp, &size, bmpDibHeader);
	
	unsigned int* pixels;
	size = bmpDibHeader->width * bmpDibHeader->height * (bmpDibHeader->bitsPerPixel << 3);
	SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&pixels);
	bmp->SetPosition(bmp, bmpHeader->pixelAddress);
	bmp->Read(bmp, &size, pixels);

	BMP_IMAGE* image;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(BMP_IMAGE), (void**)&image);
	image->bmp_dib_header = bmpDibHeader;
	image->bmp_header = bmpHeader;
	image->pixels = pixels;

	return image;
}

int memcmp(const void* a, const void* b, size_t s) {
	const unsigned char* aa = a, *bb = b;
	for(size_t i = 0; i < s; i++) {
		if(aa[i] < bb[i]) return -1;
		else if(aa[i] > bb[i]) return 1;
	}
	return 0;
}

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	Print(L"Initializing Kernel...\n\r");

	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);

	if(Kernel == NULL) {
		Print(L"Kernel couldn't be loaded\n\r");
	} else {
		Print(L"Kernel Loaded!\n\r");
	}

	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid,  &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid,  &FileInfoSize, &FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);

	}

	if(memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0||
				header.e_ident[EI_CLASS] != ELFCLASS64 ||
				header.e_ident[EI_DATA] != ELFDATA2LSB ||
				header.e_type != ET_EXEC ||
				header.e_machine != EM_X86_64 ||
				header.e_version != EV_CURRENT)
	{
		Print(L"Header is valid\n\r");
	} else {
		Print(L"Header is invalid\n\r");
	}

	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}
	for (
			Elf64_Phdr* phdr = phdrs;
			(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
			phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
		)
	{
		switch (phdr->p_type){
			case PT_LOAD:
			{
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

				Kernel->SetPosition(Kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				Kernel->Read(Kernel, &size, (void*)segment);
				break;
			}
		}
	}

	int (*KernelStart)() = ((__attribute__((sysv_abi)) int (*)() ) header.e_entry);
	FrameBuffer* buffer = InitializeGOP();

	Print(L"%dx%d@0x%x PPSL:%d\n\r", buffer->Width, buffer->Height, buffer->BaseAddress, buffer->PixelsPerScanLine);

	PSF1_FONT* font = LoadPSF1Font(NULL, L"zap-ext-vga16.psf", ImageHandle, SystemTable);
	if(font == NULL)
		Print(L"Could not load font\n\r");
	else
		Print(L"Font loaded successfully\n\r");

	BMP_IMAGE* image = LoadBMP(NULL, L"albie.bmp", ImageHandle, SystemTable);
	if(image == NULL) {
		Print(L"Could not load BMP");
	} else {
		Print(L"BMP loaded successfully\n\r");
		Print(L"Hex size: 0x%x\n\rDec size: %d\n\rPixel address: 0x%x\n\r", image->bmp_header->fileSize, image->bmp_header->fileSize, image->bmp_header->pixelAddress);
		Print(L"Width: %d\n\r",image->bmp_dib_header->width);
		Print(L"Height: %d\n\r",image->bmp_dib_header->height);
		Print(L"Bits Per Pixel: %d\n\r",image->bmp_dib_header->bitsPerPixel);
		Print(L"HR: %d\n\r",image->bmp_dib_header->horizontalResolution);
		Print(L"VR: %d\n\r",image->bmp_dib_header->verticalResolution);
		Print(L"RawSize: %d\n\r",image->bmp_dib_header->rawSize);
		unsigned int* p = image->pixels;
		Print(L"First colour: %x\n\r", p[0]);
		Print(L"Second colour: %x\n\r", p[1]);
		Print(L"Third colour: %x\n\r", p[2]);
		Print(L"Fourth colour: %x\n\r", p[3]);
	}
	Print(L"%d\r\n", KernelStart(buffer, font, image));
	
	return EFI_SUCCESS; // Exit the UEFI application
}
