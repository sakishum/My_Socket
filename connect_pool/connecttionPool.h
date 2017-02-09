/**
 * @file connecttionPool.h
 * @Synopsis  用来分配连接对象的，这样不会有内存泄漏的问题
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2015-11-02
 */

#pragma once 

#include <stdio.h>
#include <deque>
#include <set>

template<typename DataType>
class ConnectionPool {
public:
	ConnectionPool(int max_size) 
	: max_size_(max_size), aliveCount_(0), deadCount_(0) {
	}

	~ConnectionPool(void) { }

	DataType *alloc(void) {
		DataType *obj = NULL;
		if (deadCount_ > 0) {
			obj = deadObjs_.front();
			deadObjs_.pop_front();
			--deadCount_;
		} else {
			if (max_size_ >= 0 && getCount() >= max_size_) {
				printf("overflow.\n");
				return NULL;
			}
			obj = new DataType();
		}
		aliveObjs_.insert(obj);
		++aliveCount_;

		return obj;
	}

	void recycle(DataType *obj) {
		obj->release();
		aliveObjs_.erase(obj);
		--aliveCount_;
		deadObjs_.push_back(obj);
		++deadCount_;
	}

	int getCount(void) const {
		return aliveCount_ + deadCount_;
	}
	int getMaxSize(void) const {
		return max_size_;
	}
	int getAliveCount(void) const {
		return aliveCount_;
	}
	int getDeadCount(void) const {
		return deadCount_;
	}
	std::set<DataType*> &getAliveObjs(void) {
		return aliveObjs_;
	}

private:
	int max_size_;
	int aliveCount_;
	int deadCount_;

	std::deque<DataType*> deadObjs_;
	std::set<DataType*> aliveObjs_;
};  // ConnectionPool

