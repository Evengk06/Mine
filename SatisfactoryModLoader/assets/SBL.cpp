#include "stdafx.h"
#include "SBL.h"

#include <HookLoader.h>
#include <mod/ModFunctions.h>

using namespace SML;
using namespace Mod;
using namespace SML::Objects;

namespace SML {
	namespace Paks {
		template <typename T>
		FunctionBuilder logFunc(std::string s, void(T::*p)(FFrame&,void*)) {
			struct Params { FString s; bool b; };
			return FunctionBuilder::staticFunc(s).native(p).param(PropertyBuilder::param(Objects::EPropertyClass::Str, "log")).param(PropertyBuilder::param(Objects::EPropertyClass::Bool, "ignoreDebugMode").helpBool<Params,&Params::b>());
		}

		void initSBL() {
			struct ParamsSB { FString s; bool b; };
			struct ParamsB { bool b; };
			::subscribe<&UGameEngine::Start>([](void* ret, void* _) {
				ClassBuilder<UDebugLogger>::Basic()
					.extend<Objects::UObject>()
					.func(logFunc("logInfo", &UDebugLogger::logInfo))
					.func(logFunc("logWarning", &UDebugLogger::logWarning))
					.func(logFunc("logError", &UDebugLogger::logError))
					.func(logFunc("logDebug", &UDebugLogger::logDebug))
					.build();
				ClassBuilder<UModHandler>::Basic()
					.extend<Objects::UObject>()
					.func(Paks::FunctionBuilder::staticFunc("isDebugMode").native(&UModHandler::isDebugMode).addFuncFlags(FUNC_BlueprintPure).param(Paks::PropertyBuilder::retVal(EPropertyClass::Bool, "retVal").helpBool<ParamsB, &ParamsB::b>()))
					.func(Paks::FunctionBuilder::staticFunc("broadcastEvent").native(&UModHandler::broadcastEvent).param(Paks::PropertyBuilder::param(EPropertyClass::Str, "event")))
					.func(Paks::FunctionBuilder::staticFunc("doesPakExist").native(&UModHandler::doesPakExist).param(Paks::PropertyBuilder::param(EPropertyClass::Str, "pak")).param(Paks::PropertyBuilder::retVal(EPropertyClass::Bool, "retVal").helpBool<ParamsSB, &ParamsSB::b>()))
					.func(Paks::FunctionBuilder::staticFunc("isDllLoaded").native(&UModHandler::isDllLoaded).param(Paks::PropertyBuilder::param(EPropertyClass::Str, "dll")).param(Paks::PropertyBuilder::retVal(EPropertyClass::Bool, "retVal").helpBool<ParamsSB, &ParamsSB::b>()))
					.build();
			});
		}

		void log(Objects::FFrame& stack, Utility::LogType type) {
			Objects::FString str;
			bool ignore = false;
			stack.stepCompIn(&str);
			stack.stepCompIn(&ignore);
			stack.code += !!stack.code;

			if (debugOutput || ignore) Utility::log(type, str.toStr());
		}

		void UDebugLogger::logInfo(Objects::FFrame & stack, void* retVals) {
			log(stack, Utility::Info);
		}

		void UDebugLogger::logWarning(Objects::FFrame & stack, void* retVals) {
			log(stack, Utility::Warning);
		}

		void UDebugLogger::logError(Objects::FFrame & stack, void* retVals) {
			log(stack, Utility::Error);
		}

		void UDebugLogger::logDebug(Objects::FFrame & stack, void* retVals) {
			log(stack, Utility::Debug);
		}

		void UModHandler::isDebugMode(Objects::FFrame & stack, void * retVals) {
			stack.code += !!stack.code;
			*((bool*)retVals) = debugOutput;
		}

		void UModHandler::broadcastEvent(Objects::FFrame & stack, void * retVals) {
			FString s;
			stack.stepCompIn(&s);
			stack.code += !!stack.code;
			Functions::broadcastEvent(s.toStr());
		}

		void UModHandler::doesPakExist(Objects::FFrame & stack, void * retVals) {
			FString p;
			stack.stepCompIn(&p);
			stack.code += !!stack.code;
			*((bool*)retVals) = Functions::doesPakExist(p.toStr());
		}

		void UModHandler::isDllLoaded(Objects::FFrame & stack, void * retVals) {
			FString p;
			stack.stepCompIn(&p);
			stack.code += !!stack.code;
			*((bool*)retVals) = Functions::isModLoaded(p.toStr());
		}
	}
}