#include <iostream>
#include <cmath>
#include "Vector2.h"

int main() {
	Vector2 a(1, 0), c(2, 3);
	std::cout << a.getRotated(30);
	std::cout << "\n";

	return 0;
}