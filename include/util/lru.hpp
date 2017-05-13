#ifndef _LRU_H
#define _LRU_H

#include "common.hpp"
#include <list>
#include <unordered_map>

template <class K, class V> class LRU {
	private:
		std::list< std::pair<K, V> > item_list;
		std::unordered_map<K, decltype(item_list.begin()) > item_map;
		uint16_t size;

	public:
		LRU(uint16_t s){ size = s; };
		void put(K key, V val);
		V get(K key);
		bool exist(K key){ return item_map.count(key) > 0;};
};

#endif
