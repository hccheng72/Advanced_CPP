#pragma once
//#include <mutex>
//#include <condition_variable>
using namespace std;

template <class T>
class LaneThreaded {
public:
	void add(T item);
	T remove();
	int getLaneSize() { return _lane.size(); }
	LaneThreaded() {}
private:
	queue<T> _lane;
	const unsigned int _size = 10;
	mutex laneMu;
	condition_variable laneCV;
};

template <class T>
void LaneThreaded<T>::add(T item) {
	while (true) {
		//add only when carts less than 10 in a lane
		unique_lock<mutex> locker(laneMu);
		laneCV.wait(locker, [this]() {return _lane.size() < _size; });
		_lane.push(item);
		locker.unlock();
		laneCV.notify_all();
		return;
	}
}

template <class T>
T LaneThreaded<T>::remove()
{
	while (true) {
		//checkout only when there're carts in a lane
		unique_lock<mutex> locker(laneMu);
		laneCV.wait(locker, [this]() {return _lane.size() > 0; });
		T back = _lane.front();
		_lane.pop();
		locker.unlock();
		laneCV.notify_all();
		return back;
	}
}