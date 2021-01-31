#pragma once
#include <stddef.h>
#include <stdint.h>
#include "EfiMemory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapSize, uint64_t mMapDescSize);