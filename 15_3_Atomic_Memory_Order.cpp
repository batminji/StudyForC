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

int main()
{
	std::vector<std::thread> Workers;

	std::atomic<int> a(0);
	std::atomic<int> b(0);

	Workers.emplace_back(Worker1, std::ref(a), std::ref(b));
	Workers.emplace_back(Worker2, std::ref(a), std::ref(b));

	Workers[0].join();
	Workers[1].join();
}