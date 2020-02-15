#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <d3d9.h>

float TempHUDSize = 1.0f;
float TempHUDPos = 1.0f;
float HUDScale;
float HUDSize;
float HUDPos;
float FMVScale;

DWORD HUDSizeCodeCaveExit1 = 0x40BEF6;
DWORD HUDSizeCodeCaveExit2 = 0x40C089;
DWORD FMVSizeCodeCaveExit = 0x5146D6;

void __declspec(naked) HUDSizeCodeCave()
{
	__asm {
		push eax
		mov eax, dword ptr ds : [esp+ 0x04]
		cmp eax, 0x514BC7 // HUD Part 1
		je HUDSizeCodeCaveScale
		cmp eax, 0x590AB4 // HUD Part 2
		je HUDSizeCodeCaveScale
		cmp eax, 0x409FCF // Mirror
		je HUDSizeCodeCaveScale
		cmp eax, 0x4F37CD // Mouse
		je HUDSizeCodeCaveScale
		pop eax
		push ebp
		mov ebp, esp
		and esp, 0xFFFFFFF0
		jmp HUDSizeCodeCaveExit1

	HUDSizeCodeCaveScale:
		///////////////////////////////
		fld dword ptr ds : [TempHUDSize]
		fmul dword ptr ds : [HUDScale]
		fstp dword ptr ds : [HUDSize]
		fld dword ptr ds : [TempHUDPos]
		fsub dword ptr ds : [HUDSize]
		fstp dword ptr ds : [HUDPos]
		///////////////////////////////
		pop eax
		push ebp
		mov ebp, esp
		and esp, 0xFFFFFFF0
		
	HUDSizeCodeCaveFullscreenCheck1:
		// Checks for Splash Screen
		cmp dword ptr ds : [eax + 0x10], 0x41855550
		jne HUDSizeCodeCaveFullscreenCheck2
		cmp dword ptr ds : [eax + 0x14], 0x420C0000
		jne HUDSizeCodeCaveFullscreenCheck2
		cmp dword ptr ds : [eax + 0x20], 0x44512AAA
		jne HUDSizeCodeCaveFullscreenCheck2
		cmp dword ptr ds : [eax + 0x24], 0x420C0000
		jne HUDSizeCodeCaveFullscreenCheck2
		cmp dword ptr ds : [eax + 0x30], 0x44512AAA
		jne HUDSizeCodeCaveFullscreenCheck2
		cmp dword ptr ds : [eax + 0x34], 0x43DE8000
		jne HUDSizeCodeCaveFullscreenCheck2
		cmp dword ptr ds : [eax + 0x40], 0x41855550
		jne HUDSizeCodeCaveFullscreenCheck2
		cmp dword ptr ds : [eax + 0x44], 0x43DE8000
		jne HUDSizeCodeCaveFullscreenCheck2
		jmp HUDSizeCodeCaveExit1

	HUDSizeCodeCaveFullscreenCheck2:
		// Checks for Loading Screen
		cmp dword ptr ds : [eax + 0x10], 0x41855550
		jne HUDSizeCodeCaveNotFullscreen
		cmp dword ptr ds : [eax + 0x14], 0xC32A0000
		jne HUDSizeCodeCaveNotFullscreen
		cmp dword ptr ds : [eax + 0x20], 0x44512AAA
		jne HUDSizeCodeCaveNotFullscreen
		cmp dword ptr ds : [eax + 0x24], 0xC32A0000
		jne HUDSizeCodeCaveNotFullscreen
		cmp dword ptr ds : [eax + 0x30], 0x44512AAA
		jne HUDSizeCodeCaveNotFullscreen
		cmp dword ptr ds : [eax + 0x34], 0x44228000
		jne HUDSizeCodeCaveNotFullscreen
		cmp dword ptr ds : [eax + 0x40], 0x41855550
		jne HUDSizeCodeCaveNotFullscreen
		cmp dword ptr ds : [eax + 0x44], 0x44228000
		jne	HUDSizeCodeCaveNotFullscreen
		jmp HUDSizeCodeCaveExit1

	HUDSizeCodeCaveNotFullscreen:
		sub esp, 0xCC
		xor edx, edx
		fld dword ptr ds : [eax + 0x10]
		mov dh, [eax + 0x83]
		fmul dword ptr ds : [0x6CC914] // Width
		fmul dword ptr ds : [HUDSize] // Size
		fadd dword ptr ds : [HUDPos] // Pos
		push edi
		movzx edi, byte ptr [eax + 0x81]
		fsub dword ptr ds : [0x6CC7BC]
		mov dword ptr ds : [esp + 0x18], 0x3F800000
		mov dword ptr ds : [esp + 0x38], 0x3F800000
		mov dl, [eax + 0x80]
		fstp dword ptr ds : [esp + 0x10]
		mov dword ptr ds : [esp + 0x58], 0x3F800000
		fld dword ptr ds : [eax + 0x14]
		fmul dword ptr ds : [0x6CCBA4] // Height
		fmul dword ptr ds : [HUDSize] // Size
		fadd dword ptr ds : [HUDPos] // Pos
		fsub dword ptr ds : [0x6CC7BC]
		shl edx, 0x08
		or edx, edi
		movzx edi, byte ptr [eax + 0x82]
		shl edx, 0x08
		or edx, edi
		mov dword ptr ds : [esp + 0x1C], edx
		mov edx, dword ptr ds : [eax + 0x50]
		mov dword ptr ds : [esp + 0x20], edx
		mov edx, dword ptr ds : [eax + 0x54]
		mov dword ptr ds : [esp + 0x24], edx
		mov edx, dword ptr ds : [eax + 0x58]
		mov dword ptr ds : [esp + 0x28], edx
		mov dword ptr ds : [esp + 0x2C], edx
		xor edx, edx
		mov dh, [eax + 0x87]
		movzx edi, byte ptr [eax + 0x85]
		fchs
		fstp dword ptr ds : [esp + 0x14]
		fld dword ptr ds : [eax + 0x20]
		mov dl, [eax + 0x84]
		fmul dword ptr ds : [0x6CC914] // Width
		fmul dword ptr ds : [HUDSize] // Size
		fadd dword ptr ds : [HUDPos] // Pos
		fsub dword ptr ds : [0x6CC7BC]
		fstp dword ptr ds : [esp + 0x30]
		fld dword ptr ds : [eax + 0x24]
		shl edx, 0x08
		fmul dword ptr ds : [0x6CCBA4] // Height
		fmul dword ptr ds : [HUDSize] // Size
		fadd dword ptr ds : [HUDPos] // Pos
		or edx, edi
		movzx edi, byte ptr [eax + 0x86]
		shl edx, 0x08
		fsub dword ptr ds : [0x6CC7BC]
		or edx, edi
		mov dword ptr ds : [esp + 0x3C], edx
		mov edx, dword ptr ds : [eax + 0x5C]
		mov dword ptr ds : [esp + 0x40], edx
		mov edx, dword ptr ds : [eax + 0x60]
		movzx edi, byte ptr [eax + 0x89]
		mov dword ptr ds : [esp + 0x44], edx
		mov edx, dword ptr ds : [eax + 0x64]
		mov dword ptr ds : [esp + 0x48], edx
		mov dword ptr ds : [esp + 0x4C], edx
		xor edx, edx
		mov dh, [eax + 0x8B]
		fchs
		fstp dword ptr ds : [esp + 0x34]
		fld dword ptr ds : [eax + 0x30]
		fmul dword ptr ds : [0x6CC914] // Width
		fmul dword ptr ds : [HUDSize] // Size
		fadd dword ptr ds : [HUDPos] // Pos
		mov dl, [eax + 0x88]
		fsub dword ptr ds : [0x6CC7BC]
		fstp dword ptr ds : [esp + 0x50]
		fld dword ptr ds : [eax + 0x34]
		fmul dword ptr ds : [0x6CCBA4] // Height
		fmul dword ptr ds : [HUDSize] // Size
		fadd dword ptr ds : [HUDPos] // Pos
		shl edx, 0x08
		fsub dword ptr ds : [0x6CC7BC]
		or edx, edi
		movzx edi, byte ptr [eax + 0x8A]
		shl edx, 0x08
		or edx, edi
		mov dword ptr ds : [esp + 0x5C], edx
		mov edx, dword ptr ds : [eax + 0x68]
		mov dword ptr ds : [esp + 0x60], edx
		mov edx, dword ptr ds : [eax + 0x6C]
		fchs
		fstp dword ptr ds : [esp + 0x54]
		fld dword ptr ds : [eax + 0x40]
		movzx edi, byte ptr [eax + 0x8D]
		fmul dword ptr ds : [0x6CC914] // Width
		fmul dword ptr ds : [HUDSize] // Size
		fadd dword ptr ds : [HUDPos] // Pos
		mov dword ptr ds : [esp + 0x64], edx
		mov edx, dword ptr ds : [eax + 0x70]
		mov dword ptr ds : [esp + 0x68], edx
		fsub dword ptr ds : [0x6CC7BC]
		mov dword ptr ds : [esp + 0x6C], edx
		xor edx, edx
		mov dh, [eax + 0x8F]
		fstp dword ptr ds : [esp + 0x70]
		mov dword ptr ds : [esp + 0x78], 0x3F800000
		fld dword ptr ds : [eax + 0x44]
		fmul dword ptr ds : [0x6CCBA4] // Height
		fmul dword ptr ds : [HUDSize] // Size
		fadd dword ptr ds : [HUDPos] // Pos
		jmp HUDSizeCodeCaveExit2
	}
}

void __declspec(naked) FMVSizeCodeCave()
{
	__asm {
		mov eax, dword ptr ds : [esp]
		cmp eax, 0x4F1FDD // FMV
		je FMVSizeCodeCaveScale
		push ebp
		mov ebp, esp
		and esp, 0xFFFFFFF0
		jmp FMVSizeCodeCaveExit

	FMVSizeCodeCaveScale:
		///////////////////////////////
		fld dword ptr ds : [esp+ 0x08]
		fdiv dword ptr ds : [HUDSize] // Height
		fmul dword ptr ds : [FMVScale] // Scale
		fstp dword ptr ds : [esp+ 0x08]
		fld dword ptr ds : [esp + 0x0C]
		fdiv dword ptr ds : [HUDSize] // Width
		fmul dword ptr ds : [FMVScale] // Scale
		fstp dword ptr ds : [esp + 0x0C]
		fld dword ptr ds : [esp + 0x10]
		fdiv dword ptr ds : [HUDSize] // Width
		fmul dword ptr ds : [FMVScale] // Scale
		fstp dword ptr ds : [esp + 0x10]
		fld dword ptr ds : [esp + 0x14]
		fdiv dword ptr ds : [HUDSize] // Height
		fmul dword ptr ds : [FMVScale] // Scale
		fstp dword ptr ds : [esp + 0x14]
		///////////////////////////////
		push ebp
		mov ebp, esp
		and esp, 0xFFFFFFF0
		jmp FMVSizeCodeCaveExit
	}
}


void Init()
{
	// Read values from .ini
	CIniReader iniReader("NFSUHUDResizer.ini");

	// General
	HUDScale = iniReader.ReadFloat("GENERAL", "HUDScale", 0.92f);
	FMVScale = iniReader.ReadFloat("GENERAL", "FMVScale", 1.0f);

	{
		// HUD Size
		injector::MakeJMP(0x40BEF0, HUDSizeCodeCave, true);
		// FMV SIze
		injector::MakeJMP(0x5146D0, FMVSizeCodeCave, true);
	}
}
	

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x670CB5) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English speed.exe (3,03 MB (3.178.496 bytes)).", "NFSU HUD Resizer", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;

}