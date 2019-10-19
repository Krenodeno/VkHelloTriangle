#include "WindowedApp.hpp"
#include "Render.hpp"

class ShadowMapExample : public WindowedApp<Render> {
public:

	ShadowMapExample() : WindowedApp("shadowMap", VK_MAKE_VERSION(0, 0, 0)) {

	}

	~ShadowMapExample() {

	}

	void init() {

	}

	void quit() {

	}

	void update() {

	}

	bool draw() {

	}

private:


};

int main(int argc, char* argv[]) {
	try {
		ShadowMapExample app;
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;

}