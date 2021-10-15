#pragma once

template <typename T>
void printVector(const std::vector<T> &vec, std::string sep="\n") {
	for (auto elem : vec) {
		std::cout << elem << sep;
	}
}
