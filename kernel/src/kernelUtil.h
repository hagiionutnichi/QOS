#pragma once

#include "Graphics/BasicRenderer.h"
#include "Graphics/Framebuffer.h"
#include "FileFormats/PSF_1.h"
#include "FileFormats/BMP.h"
#include "math.h"
#include "numstring.h"
#include "Memory/EfiMemory.h"
#include "Memory/memory.h"
#include "Memory/Bitmap.h"
#include "Memory/PageFrameAllocator.h"
#include "Memory/PageTableManager.h"
#include "Applications/CLI/CLI.h"

struct BootInfo {
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo {
    PageTableManager* pageTableManager;
};

KernelInfo InitialiseKernel(BootInfo* bootInfo);