#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

void Worker(std::atomic<int>& Counter)
{
	for (int i = 0; i < 10000; ++i)
	{
		++Counter;
		// Counter는 atomic 객체이기 때문에,
		// 뮤텍스로 보호하지 않았음에도 불구하고
		// 정확히 계산이 된다. 

		// 이는 어셈블리어 명령어를 확인하면,
		// lock add DWORD PTR [rdi], 1
		// 와 같이 명령어 한 줄로 생성이 되는데
		// lock add : rdi에 위치한 메모리를 읽고, 1을 더하고,
		// 다시 rdi 메모리에 쓰기를 모두 한다.
	}
}

int main()
{
	std::atomic<int> Counter(0);
	// atomic의 템플릿 인자 : 원자적으로 만들고 싶은 타입을 전달

	std::vector<std::thread> Workers;
	for (int i = 0; i < 4; ++i)
	{
		Workers.push_back(std::thread(Worker, std::ref(Counter)));
	}

	for (int i = 0; i < 4; ++i)
	{
		Workers[i].join();
	}

	std::cout << "Counter 최종 값 : " << Counter << std::endl;
}