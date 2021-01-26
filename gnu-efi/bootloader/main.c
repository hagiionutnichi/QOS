#include <efi.h>
#include <efilib.h>
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

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	Print(L"Initializing Kernel...\n\r");

	if(LoadFile(NULL, L"kernel.elm", ImageHandle, SystemTable) == NULL) {
		Print(L"Kernel couldn't be loaded\n\r");
	} else {
		Print(L"Kernel Loaded!\n\r");
	}
	
	return EFI_SUCCESS; // Exit the UEFI application
}
