#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <set>
#include <queue>
#include <list>


void CheckFile(std::ifstream& file){
	if(!file) throw std::runtime_error("file is exist");
	if(!file.is_open()) throw std::runtime_error("file doest open");
	if(file.peek() == EOF) throw std::runtime_error("file is empty");
}





//hashTable
template<class Key, class Value>
class HashMap{
private:
	struct Node{
		Key k;
		Value v;
		Node* next;	
		size_t hash;
		Node(const Key& key, const Value& val) : k(key), v(val), next(nullptr) {}
	};
	std::vector<Node*> buckets;
	size_t current_cap;
	size_t size;
	double load_factor = 0.75;
public:
	HashMap() : current_cap(8), buckets(current_cap, nullptr) {}
	HashMap(size_t capacity) : current_cap(capacity), buckets(current_cap, nullptr) {}
	~HashMap(){
		for (auto node : buckets){
			while (node){
				Node* next = node->next;
				delete node;
				node = next;
			}
		}
	}
	
	void insert(const Key& key, const Value& value){
		if(size/current_cap >= load_factor) resize(current_cap*2);

		size_t h = std::hash<Key>{}(key);
		size_t indx = h % current_cap;
		Node* cur = buckets[indx];
		while (cur != nullptr){
			if (cur->k == key){
				cur->v = value;
				return;
			}
			cur = cur->next;
		}

		Node* n = new Node(key, value);
		n->next = buckets[indx];
		buckets[indx] = n;
		n->hash = h;
		++size;
	}


	Value* find(const Key& key){
		size_t indx = std::hash<Key>{}(key) % current_cap;
		for (Node* cur{buckets[indx]}; cur != nullptr; cur = cur->next){
			if (cur->k == key) return &cur->v;
		}
		return nullptr;
	}


	void erase(const Key& key){
		size_t indx = std::hash<Key>{}(key) % current_cap;
		Node* cur = buckets[indx];
		Node* temp = nullptr;
		while (cur != nullptr){
			if (cur->k == key){
				if (temp == nullptr){
					buckets[indx] = cur->next;
				} else{
					temp->next = cur->next;
				}
				delete cur;
				--size;
				return;
			}
			temp = cur;
			cur = cur->next;
		}
	}

	void resize(const size_t& newsize){
		std::vector<Node*> newbuckets(newsize, nullptr);
		for(auto i: buckets){
			Node* cur = i;
			while (cur != nullptr){
				size_t indx = cur->hash % newsize;
				Node* temp = cur->next;
				cur->next = newbuckets[indx];
				newbuckets[indx] = cur;
				cur = temp;
			}
		}
		buckets = std::move(newbuckets);
		current_cap = newsize;
	}
};






void PerevarotAnagram() {
	std::ifstream file("text.txt");
	std::string str;
	std::stringstream k;
	std::vector<std::string> tokens;
	while(std::getline(file, str)){		
		std::string delimiters = ",;|";
		for (char &ch : str){
			if (delimiters.find(ch) != std::string::npos) {
				ch = ' ';
			}
		}

		std::stringstream line(str);
		std::string token;
		while(line >> token){
			tokens.push_back(token);
		}
	}
	std::cout << tokens.size() << '\n';
	for(const auto& word: tokens){
		std::cout << word << " ";
	}
	std::cout << '\n';

	

	std::unordered_map<std::string, std::vector<std::string*>> anagram_groups;
	for (auto& word : tokens){
		std::string sorted_word = word;
		std::sort(sorted_word.begin(), sorted_word.end());
		anagram_groups[sorted_word].push_back(&word);
	}

	for (auto& [key, group] : anagram_groups){
		if (group.size() > 1){
			for (auto& word_ptr : group){
				std::reverse((*word_ptr).begin(), (*word_ptr).end());
			}
		}
	}
	
	for(const auto& word: tokens){
		std::cout << word << " ";
	}
	std::cout << '\n';
}







