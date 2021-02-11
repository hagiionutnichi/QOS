#include "acpi.h"

void listACPIHeaders(XSDT* xsdt) {
    uint64_t entries = (xsdt->header.Length - sizeof(SDTHeader)) / 8;
    for(int i = 0; i < entries; i++) {
        XSDT *h = ((XSDT *) xsdt->entry[i]);

        GlobalRenderer->PrintChar(h->header.Signature[0]);
        GlobalRenderer->PrintChar(h->header.Signature[1]);
        GlobalRenderer->PrintChar(h->header.Signature[2]);
        GlobalRenderer->PrintChar(h->header.Signature[3]);
        GlobalRenderer->NewLine();
    }
}

XSDT* findHeader(XSDT* xsdt, const char* identifier) {
    uint64_t entries = (xsdt->header.Length - sizeof(SDTHeader)) / 8;
    for(int i = 0; i < entries; i++) {
        XSDT *h = ((XSDT *) xsdt->entry[i]);
        if(strncmp(h->header.Signature, identifier, 4) == 0) {
            return h;
        }
    }
    return NULL;
}

void listMADTEntries(MADT* madt) {
    uint32_t totalLength = madt->h.Length;
    totalLength -= sizeof(MADT);

    MADT_Entry_Header* entry = (MADT_Entry_Header *) (madt + 1);
    while(totalLength > 0) {
        GlobalRenderer->Print("Entry type: ");
        GlobalRenderer->PrintLn(to_string((uint64_t)entry->type));
        switch(entry->type) {
            case 0:
            {
                MADT_Entry_Type_0* t0 = (MADT_Entry_Type_0 *) entry;
                GlobalRenderer->Print("Processor ID: ");
                GlobalRenderer->PrintLn(to_string((uint64_t)t0->processor_id));
                totalLength -= sizeof(MADT_Entry_Type_0);
                // while(true);
                entry = ((MADT_Entry_Header *)(t0 + 1));
            }
            break;
            case 1:
            {
                MADT_Entry_Type_1* t1 = (MADT_Entry_Type_1 *) entry;
                GlobalRenderer->Print("IO ID: ");
                GlobalRenderer->PrintLn(to_string((uint64_t)t1->io_id));
                GlobalRenderer->Print("IO Address: ");
                GlobalRenderer->PrintLn(to_hstring(t1->io_address));
                totalLength -= sizeof(MADT_Entry_Type_1);
                // while(true);
                entry = ((MADT_Entry_Header *)(t1 + 1));
            }
            break;
            case 2:
            {
                MADT_Entry_Type_2* t2 = (MADT_Entry_Type_2 *) entry;
                GlobalRenderer->Print("Bus source: ");
                GlobalRenderer->PrintLn(to_string((uint64_t)t2->bus_source));
                totalLength -= sizeof(MADT_Entry_Type_2);
                // while(true);
                entry = ((MADT_Entry_Header *)(t2 + 1));
            }
            break;
            case 4:
            {
                MADT_Entry_Type_4* t4 = (MADT_Entry_Type_4 *) entry;
                GlobalRenderer->Print("Proc ID(T4): ");
                GlobalRenderer->PrintLn(to_string((uint64_t)t4->processor_id));
                totalLength -= sizeof(MADT_Entry_Type_4);
                // while(true);
                entry = ((MADT_Entry_Header *)(t4 + 1));
            }
            break;
            case 5:
            {
                MADT_Entry_Type_5* t5 = (MADT_Entry_Type_5 *) entry;
                GlobalRenderer->Print("Local APIC Address: 0x");
                GlobalRenderer->PrintLn(to_hstring(t5->local_apic_address));
                totalLength -= sizeof(MADT_Entry_Type_5);
                // while(true);
                entry = ((MADT_Entry_Header *)(t5 + 1));
            }
            break;
            default:
            {
                totalLength = 0;
            }
            break;
        }

    }

}

void InitialiseXSDT(RSDP_Ext* rsdp_ext) {
    GlobalRenderer->SetCursor(0, 0);

    if(rsdp_ext->rsdp.revision != 2) {
        GlobalRenderer->PrintLn("Unable to initialise XSDT");
    }

    XSDT* xsdt = (XSDT *) rsdp_ext->xsdt_address;

    listACPIHeaders(xsdt);
      
    XSDT* facp = findHeader(xsdt, "FACP");
    if(facp) {
        GlobalRenderer->Print("Found FACP at address 0x");
        GlobalRenderer->PrintLn(to_hstring((uint64_t) facp));
        FADT* fadt = (FADT *) facp;
        GlobalRenderer->Print("FADT PreferredPowerManagementProfile: ");
        GlobalRenderer->PrintLn(to_string((uint64_t) fadt->PreferredPowerManagementProfile));
    } else {
        GlobalRenderer->Print("Could not find FACP");
    }

    XSDT* apic = findHeader(xsdt, "APIC");
    if(apic) {
        GlobalRenderer->Print("Found APIC at address 0x");
        GlobalRenderer->PrintLn(to_hstring((uint64_t) apic));
        MADT* madt = (MADT *) apic;
        listMADTEntries(madt);
    } else {
        GlobalRenderer->Print("Could not find FACP");
    }

}