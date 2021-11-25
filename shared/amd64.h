#pragma once

#include <intrin.h>

#define KGDT64_NULL 0x00
#define KGDT64_R0_CODE 0x10
#define KGDT64_R0_DATA 0x18
#define KGDT64_R3_CMCODE 0x20
#define KGDT64_R3_DATA 0x28
#define KGDT64_R3_CODE 0x30
#define KGDT64_SYS_TSS 0x40
#define KGDT64_R3_CMTEB 0x50

#pragma pack(push, 1)

namespace ss
{
    struct Segment_Selector
    {
        unsigned short RPL : 2;
        unsigned short TI : 1;
        unsigned short Index : 13;
    };

    struct Segment_Descriptor
    {
        unsigned short Limit0;
        unsigned short Base0;

        unsigned char Base1;

        unsigned char Type : 4;

        unsigned char S : 1;
        unsigned char DPL : 2;
        unsigned char P : 1;

        unsigned char Limit1 : 4;

        unsigned char AVL : 1;
        unsigned char L : 1;
        unsigned char DB : 1;
        unsigned char G : 1;

        unsigned char Base2;

        DWORD32 BaseAddress()
        {
            return Base0 | (Base1 << 16) | (Base2 << 24);
        }

        DWORD32 Limit()
        {
            return Limit0 | (Limit1 << 16);
        }

        void Print()
        {
            KdPrint(("Base:[%x] Limit:[%x]\n", BaseAddress(), Limit()));
        }
    };

    struct Gate_Descriptor
    {
        unsigned short Offset0;
        Segment_Selector Selector;

        unsigned char Reserved0;

        unsigned char Type : 4;
        unsigned char S : 1;
        unsigned char DPL : 2;
        unsigned char P : 1;

        unsigned short Offset1;

        DWORD32 Offset()
        {
            return Offset0 | (Offset1 << 16);
        }
    };

    struct Gate_Descriptor_64 : Gate_Descriptor
    {
        unsigned int Offset2;
        unsigned int Reserved2;

        DWORD64 Offset()
        {
            return Gate_Descriptor::Offset() | ((DWORD64)Offset2 << 32);
        }
    };

    struct GDTR
    {
        unsigned short Limit;
        Segment_Descriptor *BaseAddress;

        void SGDT() { _sgdt(this); }
        void LGDT() { _lgdt(this); }

        size_t Size()
        {
            return ((Limit + 1) / 8);
        }

        void Print()
        {
            for (size_t i = 0; i < Size(); i++)
            {
                BaseAddress[i].Print();
            }
        }
    };

    struct IDTR
    {
        unsigned short Limit;
        void *BaseAddress;

        void SIDT() { __sidt(this); }
        void LIDT() { __lidt(this); }

        size_t Size()
        {
            return ((Limit + 1) / 8);
        }
    };
}

#pragma pack(pop)
