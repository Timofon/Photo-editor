#include "application.h"

int main(int argc, char* argv[]) {
    try {
        Application app;
        AppRun(argc, argv, app);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
