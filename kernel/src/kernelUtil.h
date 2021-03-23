#pragma once

#include <stdint.h>
#include "Graphics/BasicRenderer.h"
// #include "cstr.h"
#include "Memory/EfiMemory.h"
#include "Memory/memory.h"
#include "Memory/Bitmap.h"
#include "Memory/PageFrameAllocator.h"
#include "Memory/PageMapIndexer.h"
#include "Memory/paging.h"
#include "Memory/PageTableManager.h"
#include "Keyboard/mouse.h"
#include "acpi.h"
// #include "pci.h"

struct BootInfo {
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
	ACPI::RSDP2* rsdp;
} ;

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo {
    PageTableManager* pageTableManager;
};

KernelInfo InitialiseKernel(BootInfo* BootInfo);