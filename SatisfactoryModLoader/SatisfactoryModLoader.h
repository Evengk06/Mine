#pragma once

#include <tuple>
#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <mod/ModHandler.h>
#include "HookLoader.h"

namespace SML {
	inline static HookLoader* hookLoader = new HookLoader();
	inline static const std::string modLoaderVersion = "1.0.0-pr4";
	inline static const std::string targetVersion[2] = { "99133", "98994" };
	inline static bool loadConsole = true;
	inline static bool debugOutput = false;
	inline static bool supressErrors = false;

	extern Mod::ModHandler modHandler;

	void mod_loader_entry();
	void read_config();
	void cleanup();
}