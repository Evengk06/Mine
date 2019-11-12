#pragma once

#include "BPInterface.h"

namespace SML {
	namespace Paks {
		void initSBL();
		
		class UDebugLogger : public Objects::UObject {
		public:
			void logInfo(Objects::FFrame& stack, void* retVals);
			void logWarning(Objects::FFrame& stack, void* retVals);
			void logError(Objects::FFrame& stack, void* retVals);
			void logDebug(Objects::FFrame& stack, void* retVals);
		};

		class UModHandler : public Objects::UObject {
		public:
			void isDebugMode(Objects::FFrame& stack, void* retVals);
			void broadcastEvent(Objects::FFrame& stack, void* retVals);
			void doesPakExist(Objects::FFrame& stack, void* retVals);
			void isDllLoaded(Objects::FFrame& stack, void* retVals);
		};
	}
}