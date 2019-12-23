#include "stdafx.h"
#include "FText.h"
#include <Windows.h>
#include <detours.h>

using namespace SML;
using namespace SML::Objects;

FText FText::fromString(FString&& s) {
	static FText(*fptr)(FString&&) = nullptr;
	if (fptr) fptr = (FText(*)(FString&&)) DetourFindFunction("FactoryGame-Win64-Shipping.exe", "FText::FromString");
	return fptr(std::move(s));
}
