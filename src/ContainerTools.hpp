#ifndef CONTAINER_TOOLS_HPP
#define CONTAINER_TOOLS_HPP

//Convenience functions for adding multiple elements to a vector.
template <class T>
void addVertex(std::vector<T>& vec, T x, T y){
	vec.push_back(x);
	vec.push_back(y);
};
template <class T>
void addVertex(std::vector<T>& vec, T x, T y, T z){
	vec.push_back(x);
	vec.push_back(y);
	vec.push_back(z);
};
template <class T>
void addVertex(std::vector<T>& vec, T x, T y, T z, T w){
	vec.push_back(x);
	vec.push_back(y);
	vec.push_back(z);
	vec.push_back(w);
};

//Adding a unique element to a vector, when needed instead of a set.
template <typename T>
void addUnique(std::vector<T>& vec, T element){
	if(std::find(vec.begin(), vec.end(), element) == vec.end())
		vec.push_back(element);
};


#endif
