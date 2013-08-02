#pragma once

extern "C" PVOID __stdcall GetKiSystemCall64Address();
extern "C" void __stdcall SetSoftBreakPoint();

extern "C" void __stdcall ClearWriteProtect();
extern "C" void __stdcall SetWriteProtect();
