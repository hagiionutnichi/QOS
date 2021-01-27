#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

typedef struct {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} FrameBuffer;

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
		Print(L"GOP located");
	}

	framebuffer.BaseAddress = (void*) gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.Width = gop->Mode->Info->PixelsPerScanLine;

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

	Print(L"%dx%d@0x%x", buffer->Width, buffer->Height, buffer->BaseAddress);
	Print(L"%d\r\n", KernelStart());
	
	return EFI_SUCCESS; // Exit the UEFI application
}
