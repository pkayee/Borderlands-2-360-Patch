#pragma once
#include <xtl.h>

namespace hooks {
    class PowerPCPatching {
    public:
        int Int24ToInt32(int Value) {
            byte bValue = (Value >> 24) & 0xFF;
            Value &= 0x00FFFFFF;
            if (Value & 0x800000 && bValue != 0x48)
                Value |= 0xFF000000;
            if (Value & 1)
                Value -= 1;
            return Value;
        }

        int GetBranchCall(int Address) {
            int Dest, Temp;
            Dest = *(int *)Address;
            Temp = Dest;
            Dest = Temp & 0x03FFFFFC;
            if (Temp & 0x02000000) Dest |= 0xFC000000;
            Dest = Address + Dest;
            return Dest;
        }

        VOID PatchInJump(DWORD *Address, void *Dest, BOOL Linked) {
            DWORD Bytes[4];
            DWORD Destination = (DWORD)Dest;
            Bytes[0] = 0x3C000000 + ((Destination >> 16) & 0xFFFF);
            Bytes[1] = 0x60000000 + (Destination & 0xFFFF);
            Bytes[2] = 0x7C0903A6;
            Bytes[3] = 0x4E800420;
            if (Linked)
                Bytes[3] += 1;
            *(__int64 *)((DWORD)Address + 0) = *(__int64 *)&Bytes[0];
            *(__int64 *)((DWORD)Address + 8) = *(__int64 *)&Bytes[2];
            __dcbst(0, Address);
            __sync();
        }
    };
}