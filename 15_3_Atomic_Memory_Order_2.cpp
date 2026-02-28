#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

void Producer(std::atomic<bool>& bIsReady, std::atomic<int>* Data)
{
	Data[0].store(1, std::memory_order_relaxed);
	Data[1].store(2, std::memory_order_relaxed);
	Data[2].store(3, std::memory_order_relaxed);

	bIsReady.store(true, std::memory_order_release);
}

void Consumer(std::atomic<bool>& bIsReady, std::atomic<int>* Data)
{
	while (!bIsReady.load(std::memory_order_acquire)) {

	}
	printf("Data[0] : %d\n", Data[0].load(std::memory_order_relaxed));
	printf("Data[1] : %d\n", Data[1].load(std::memory_order_relaxed));
	printf("Data[2] : %d\n", Data[2].load(std::memory_order_relaxed));
}

int main()
{
	std::vector<std::thread> Workers;

	std::atomic<bool> bIsReady(false);
	std::atomic<int> Data[3] = { 0, 0, 0 };

	Workers.emplace_back(Producer, std::ref(bIsReady), std::ref(Data));
	Workers.emplace_back(Consumer, std::ref(bIsReady), std::ref(Data));

	Workers[0].join();
	Workers[1].join();
}