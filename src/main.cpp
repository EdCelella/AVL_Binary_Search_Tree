#include "avl.h"

#include <fstream>
#include <sstream>

template<typename K, typename I>
void readFileNodes(AVL<K,I>& bt, std::string filename);

int main(){
	
	AVL<int,std::string> bt;
	readFileNodes(bt, "../names.txt");

	std::cout << bt << std::endl;
	bt.displayTree();

	return 0;
}

template<typename K, typename I>
void readFileNodes(AVL<K,I>& bt, std::string filename){

	std::string line, key, item;
  	std::ifstream file (filename);

  	if (file.is_open()){

    	while ( getline (file,line) ){
      		
      		std::istringstream iss(line);
		    iss >> key;
		    iss >> item;

		    bt.insert(std::stoi(key), item);
		   	
    	}
    	file.close();

  	}
	else{
		std::cout << "Unable to open file." << std::endl;
	} 

}