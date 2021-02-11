#pragma once
#include <stdint.h>
#include "../Graphics/BasicRenderer.h"
#include "../string.h"
#include "../numstring.h"
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

struct XSDT {
  struct SDTHeader header;
  uint64_t entry[];
}  __attribute__ ((packed));

struct GenericAddressStructure
{
  uint8_t AddressSpace;
  uint8_t BitWidth;
  uint8_t BitOffset;
  uint8_t AccessSize;
  uint64_t Address;
}  __attribute__ ((packed));

struct FADT
{
    struct SDTHeader h;
    uint32_t FirmwareCtrl;
    uint32_t Dsdt;
 
    // field used in ACPI 1.0; no longer in use, for compatibility only
    uint8_t  Reserved;
 
    uint8_t  PreferredPowerManagementProfile;
    uint16_t SCI_Interrupt;
    uint32_t SMI_CommandPort;
    uint8_t  AcpiEnable;
    uint8_t  AcpiDisable;
    uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t  PM1EventLength;
    uint8_t  PM1ControlLength;
    uint8_t  PM2ControlLength;
    uint8_t  PMTimerLength;
    uint8_t  GPE0Length;
    uint8_t  GPE1Length;
    uint8_t  GPE1Base;
    uint8_t  CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t  DutyOffset;
    uint8_t  DutyWidth;
    uint8_t  DayAlarm;
    uint8_t  MonthAlarm;
    uint8_t  Century;
 
    // reserved in ACPI 1.0; used since ACPI 2.0+
    uint16_t BootArchitectureFlags;
 
    uint8_t  Reserved2;
    uint32_t Flags;
 
    // 12 byte structure; see below for details
    GenericAddressStructure ResetReg;
 
    uint8_t  ResetValue;
    uint8_t  Reserved3[3];
 
    // 64bit pointers - Available on ACPI 2.0+
    uint64_t                X_FirmwareControl;
    uint64_t                X_Dsdt;
 
    GenericAddressStructure X_PM1aEventBlock;
    GenericAddressStructure X_PM1bEventBlock;
    GenericAddressStructure X_PM1aControlBlock;
    GenericAddressStructure X_PM1bControlBlock;
    GenericAddressStructure X_PM2ControlBlock;
    GenericAddressStructure X_PMTimerBlock;
    GenericAddressStructure X_GPE0Block;
    GenericAddressStructure X_GPE1Block;
} __attribute__((packed));

struct MADT_Entry_Header {
    char type;
    uint8_t length;
} __attribute__((packed));

struct MADT {
    struct SDTHeader h;
    uint32_t localApicAddr;
    uint32_t flags;
} __attribute__((packed));

struct MADT_Entry_Type_0 {
    struct MADT_Entry_Header h;
    uint8_t processor_id;
    uint8_t id;
    uint32_t flags;
} __attribute__((packed));

struct MADT_Entry_Type_1 {
    struct MADT_Entry_Header h;
    uint8_t io_id;
    uint8_t reserved;
    uint32_t io_address;
    uint32_t general_system_interrupt_base;
} __attribute__((packed));

struct MADT_Entry_Type_2 {
    struct MADT_Entry_Header h;
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_system_interrupt;
    uint16_t flags;
} __attribute__((packed));

struct MADT_Entry_Type_4 {
    struct MADT_Entry_Header h;
    uint8_t processor_id;
    uint16_t flags;
    uint8_t lint_number;
} __attribute__((packed));

struct MADT_Entry_Type_5 {
    struct MADT_Entry_Header h;
    uint16_t reserved;
    uint64_t local_apic_address;
} __attribute__((packed));

void listACPIHeaders(XSDT* xsdt);
XSDT* findHeader(XSDT* xsdt, const char* identifier);
void listMADTEntries(MADT* madt);
void InitialiseXSDT(RSDP_Ext* rsdp_ext);