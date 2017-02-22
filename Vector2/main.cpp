#include <iostream>
#include <cmath>
#include "Vector2.h"

int main() {
	Vector2 a(1, 0), b(3, 4);
	//std::cin >> a;
	//std::cin >> b;
	std::cout << "a = " << a << "\nb = " << b << "\n";

	std::cout << "-b = " << -b << "\n";
	std::cout << "a * 5 = " << a * 5 << "\n";
	std::cout << "5 * b = " << 5 * b << "\n";
	std::cout << "b / 5 = " << b / 5 << "\n";
	std::cout << "a + b = " << a + b << "\n";
	std::cout << "a - b = " << a - b << "\n";

	a += b;
	std::cout << "a += b = " << a << "\n";
	a -= b;
	std::cout << "a -= b = " << a << "\n";

	std::cout << "a * b = " << a * b << "\n";
	std::cout << "a ^ b = " << (a ^ b) << "\n";

	std::cout << "b.len = " << b.len() << "\n";
	std::cout << "b.squareLen = " << b.squareLen() << "\n";
	std::cout << "b.perpendicular = " << b.perpendicular() << "\n";
	std::cout << "b.norm = " << b.norm() << "\n";
	std::cout << "a.getRotated(30) = " << a.getRotated(30) << "\n";
	std::cout << "a.rotate(30) = " << a.rotate(30) << "\n";

	return 0;
}