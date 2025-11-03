#include "client-app.hpp"

namespace tec {
// Forward declaration for external linkage
void BuildTestVoxelVolume();
} // namespace tec

int main(int argc, char* argv[]) {
	tec::ClientApp app(argc, argv);
	return app.Run();
}
