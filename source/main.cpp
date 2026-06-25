#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <shlwapi.h>
#include <sunset.hpp>
#include <d3d9.h>
#include <dwmapi.h>
#include "pentane.hpp"
#include "config.hpp"
#include "Game/GameSpecificFlashImpl.hpp"
#include "Game/Genie/String.hpp"
#include "Game/GameProgressionManager.hpp"
#include "Game/Stage/StageEntity.hpp"
#include "Game/Components/ActiveMoves.hpp"
#include "Game/Components/CarsReactionMonitor.hpp"
#include "Game/Stage/Cars2VehicleDBlock.hpp"
#include "Patch/Input/KeyControllerInputDriver.hpp"
#include "Patch/Input/WindowsSystemInputDriver.hpp"
#include "Patch/Input/WindowsControllerInputDriver.hpp"
#include "Patch/Input/XInputInputDriver.hpp"
#include "Patch/OptionFlashCallbacks.hpp"

extern "C" void __stdcall Pentane_Main() {
	// FIXME: link against Pentane.lib properly instead of this bullshit!!!!
	Pentane_LogUTF8 =
		reinterpret_cast<void(*)(PentaneCStringView*)>(
			GetProcAddress(GetModuleHandleA("Pentane.dll"),
				"Pentane_LogUTF8"));

	Pentane_IsWindowedModeEnabled =
		reinterpret_cast<int(*)()>(
			GetProcAddress(GetModuleHandleA("Pentane.dll"),
				"Pentane_IsWindowedModeEnabled"));

	sunset::inst::nop(reinterpret_cast<void*>(0x00e27caf), 2);
	DefineInlineHook(LangSelectOverride) {
		static void __cdecl callback(sunset::InlineCtx & ctx) {
			uintptr_t ebp = ctx.ebp.unsigned_integer;
			void* strObj = reinterpret_cast<void*>(ebp - 0x7c);

			using FormatFn = void(__cdecl*)(void*, const char*, ...);
			FormatFn Format = reinterpret_cast<FormatFn>(0x005df990);

			Format(strObj, "LangSelectNames\\LangNames_win32_PC1.langnames");
		}
	};

	LangSelectOverride::install_at_ptr(0x00e0bc15);
}