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

template <class K, class V> void LRU<K, V>::put(K key, V val){
	auto it = item_map.find(key);

	if(it != item_map.end()){
		item_list.erase(it->second);
		item_map.erase(it);
	}
	item_list.push_front(std::make_pair(key, val));
	item_map.insert(make_pair(key, item_list.begin()));

	if(item_map.size() > size){
		auto it = item_list.end();
		item_map.erase((--it)->first);
		item_list.pop_back();
	}
}

template <class K, class V> V LRU<K, V>::get(K key){
	ASSERT(exist(key));

	auto it = item_map[key];
	item_list.splice(item_list.begin(), item_list, it);

	return it->second;
}

#endif
