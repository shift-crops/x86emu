#include <list>
#include <unordered_map>
#include "util/lru.hpp"

template void LRU<uint32_t, uint32_t>::put(uint32_t key, uint32_t val);
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

template uint32_t LRU<uint32_t, uint32_t>::get(uint32_t key);
template <class K, class V> V LRU<K, V>::get(K key){
	ASSERT(exist(key));

	auto it = item_map.find(key)->second;
	item_list.splice(item_list.begin(), item_list, it);

	return it->second;
}
