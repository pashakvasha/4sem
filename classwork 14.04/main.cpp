#include <future>
#include <iostream>
#include <vector>

const int N = 4;
const int pointAmount = 100000;

struct Vector3
{
	float x;
	float y;
	float z;
	float distance()
	{
		return x * x + y * y + z * z;
	}
};

int volume()
{
	int pointCount = 0;
	std::vector<Vector3> points;
	for (int i = 0; i < pointAmount; i++)
	{
		Vector3 point;
		point.x = 2 * (rand() % 32000) / 32000.f - 1;
		point.y = 2 * (rand() % 32000) / 32000.f - 1;
		point.z = 2 * (rand() % 32000) / 32000.f - 1;
		points.push_back(point);
	}

	for (auto & point : points)
	{
		if (point.distance() <= 1)
			pointCount++;
	}
	
	return pointCount;
}

int main()
{
	int vol = 0;

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::future<int>> futures;
	for (int i = 0; i < N; i++)
	{
		futures.push_back(std::async(volume));
	}

	for(auto &e: futures)
	{
		vol += e.get();
	}

	auto diff = std::chrono::high_resolution_clock::now() - start;
	std::cout << diff.count()/1000000 << "\n";
	std::cout << 8 * (float)vol / (N * pointAmount) << "\n";

	return 0;
}