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