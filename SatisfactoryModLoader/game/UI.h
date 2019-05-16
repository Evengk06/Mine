#pragma once

#pragma warning (push)
#pragma warning (disable: 4667)

namespace SML {
	namespace Objects {
		class UButton {
		public:
			bool IsPressed();
			void SlateHandlePressed();
		};

		class UCheckBox {
		public:
			bool IsPressed();
		};

		class SButton {
		public:
			bool IsPressed();
		};

		class UAkCheckBox {
		public:
			bool IsPressed();
		};

	}
}


#pragma warning (pop)
