#pragma once

void SafeWriteBuf(uintptr_t addr, void * data, size_t len);
void SafeWrite8(uintptr_t addr, UInt8 data);
void SafeWrite16(uintptr_t addr, UInt16 data);
void SafeWrite32(uintptr_t addr, UInt32 data);
void SafeWrite64(uintptr_t addr, UInt64 data);

// ### warning: if you try to branch more than +/- 2GB with these, they will fail and return false
// ### this is a limitation of the 'jmp' instruction and more generally the x64 ISA
// 5 bytes written to src
bool SafeWriteJump(uintptr_t src, uintptr_t dst);
bool SafeWriteCall(uintptr_t src, uintptr_t dst);
