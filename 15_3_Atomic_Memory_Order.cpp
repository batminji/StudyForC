#include <atomic>
#include <cstdio>
#include <thread>
#include <vector>

// atomic 객체는 원자적 연산 시
// 메모리에 접근할 때 어떤 방식으로 접근할지 지정할 수 있다.

// memory_order_relaxed - 가장 느슨한 조건
// 메모리에서 읽거나 쓸 경우,
// 주위의 다른 메모리 접근들과 순서가 바뀌어도 된다.
// CPU에서 메모리 연산 순서에 대해 자유를 주는 것과 같다.
// => CPU에서 매우 빠른 속도로 실행할 수 있게 된다.
// => 대신 예상치 못한 결과를 나을 수 있다.

// memory_order_release
// 해당 명령 이전의 모든 메모리 명령들이
// 해당 명령 이후로 재배치 되는 것을 금지한다.
// 만약에 같은 변수를 memory_order_acquired로 읽는 쓰레드가 있다면
// memory_order_release 이전에 오는 모든 메모리 명령들이
// 해당 쓰레드에 의해서 관찰될 수 있어야 한다.

// memory_order_acquire
// release와는 반대로,
// 해당 명령 이후에 오는 모든 메모리 명령들이
// 해당 명령 이전으로 재배치 되는 것을 금지한다.

// => release오와 acquire를 통해서
// 동기화를 수행하는 것을 볼 수 있다.

// store(), load() 함수
// atomic 객체들에 대해서 원자적으로 쓰기와 읽기를 지원해주는 함수
// store() : 쓰기
// load() : 읽기
// 이때 추가적인 인자로 memory_order을 지정해 전달할 수 있다.

void Worker1(std::atomic<int>& a, std::atomic<int>& b)
{
	b.store(1, std::memory_order_relaxed);
	int x = a.load(std::memory_order_relaxed);

	printf("x : %d\n", x);
}

void Worker2(std::atomic<int>& a, std::atomic<int>& b)
{
	a.store(1, std::memory_order_relaxed);
	int y = b.load(std::memory_order_relaxed);

	printf("y : %d\n", y);
}

// ------------------------------------------------------

void Producer(std::atomic<bool>& bIsReady, int& Data)
{
	Data = 10;
	bIsReady.store(true, std::memory_order_release);
}

void Consumer(std::atomic<bool>& bIsReady, int& Data)
{
	while (!bIsReady.load(std::memory_order_acquire))
	{

	}
	printf("Data : %d\n", Data);
}

int main()
{
	std::vector<std::thread> Workers;

	std::atomic<bool> bIsReady(false);
	int Data = 0;

	Workers.emplace_back(Producer, std::ref(bIsReady), std::ref(Data));
	Workers.emplace_back(Consumer, std::ref(bIsReady), std::ref(Data));

	Workers[0].join();
	Workers[1].join();
}