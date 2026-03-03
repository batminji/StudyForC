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
	// std::promise
	// 연산을 수행 후에, 돌려줄 객체의 타입을
	// 템플릿 인자로 받는다
	std::promise<std::string> p;

	//  미래에 string 데이터를 돌려 주겠다는 약속
	// 연산이 끝난 다음에 promise 객체는
	// 자신이 가지고 있는 future 객체에 값을 넣어주게 된다.
	std::future<std::string> data = p.get_future();

	std::thread t (Worker, std::ref(p));

	// 미래에 약속된 데이터를 받을 때까지 기다린다.
	data.wait();

	// get 함수를 바로 호출하더라도
	// 알아서 promise가 future에 객체를 전달할 때까지
	// 기다린 다음에 리턴한다.

	// 주의!
	// future 객체에서 get을 호출하면 설정된 객체가 이동된다.
	// => get을 두 번 호출하면 안된다.
	std::cout << "받은 데이터 : " << data.get() << std::endl;

	t.join();
}