#include <iostream>

#include "generator.hpp"

int main() {
    Genetator gen(-100.0, 100.0);

    for (size_t i = 0; i != 10; ++i) {
        gen.GenerateAndSave(100 * (i + 1), "./tests/e2e/test_data/" + std::to_string(i + 1) + ".dat");
    }

    return 0;
}
