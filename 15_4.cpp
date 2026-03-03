// 동기와 비동기 실행
// 동기적
//		- 한 번에 하나씩 순차적으로 실행되는 작업
//		- 한 작업이 끝날 때까지 다음 작업으로 이동하지 않는다.
// 비동기적
//		- 한 갈래가 아니라 여러 갈래로 갈라져서 동시에 진행되는 것
// 
// C++11 펴즌 라이브러리를 통해 간단히 비동기적 실행을
// 할 수 있게 해주는 도구를 제공하고 있다.
// 
// 쓰레드에서 비동기적으로 값을 받아내겠다는 의미
// => 미래에 쓰레드가 원하는 데이터를 돌려 주겠다는 약속이라고 볼 수 있다.
// =>=> std::promise / std::future
//

#include <iostream>
#include <future>
#include <string>
#include <thread>

void Worker(std::promise<std::string>& p)
{
	p.set_value("some data");
}

int main()
{
	std::promise<std::string> p;
	//  미래에 string 데이터를 돌려 주겠다는 약속
	std::future<std::string> data = p.get_future();

	std::thread t (Worker, std::ref(p));

	// 미래에 약속된 데이터를 받을 때까지 기다린다.
	data.wait();

	// wait 
	std::cout << "받은 데이터 : " << data.get() << std::endl;

	t.join();
}