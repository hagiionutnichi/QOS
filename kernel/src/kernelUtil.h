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

struct RSDP {
	char signature[8];
	uint8_t checksum;
	char oemid[6];
	uint8_t revision;
	uint32_t rsdt_address;
} __attribute__((packed));

struct RSDP_Ext {
 RSDP rsdp;
 
 uint32_t length;
 uint64_t xsdt_address;
 uint8_t ext_checksum;
 uint8_t reserved[3];
}  __attribute__ ((packed));

struct SDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
}  __attribute__ ((packed));

// struct XSDT {
//   SDTHeader header;
//   uint32_t sigint() const {
// return *(uint32_t*) header.Signature;
//     }
// };

struct BootInfo {
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	RSDP_Ext* rsdp_ext;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo {
    PageTableManager* pageTableManager;
};

KernelInfo InitialiseKernel(BootInfo* bootInfo);