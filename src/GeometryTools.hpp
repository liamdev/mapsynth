#ifndef GEOMETRY_TOOLS_HPP
#define GEOMETRY_TOOLS_HPP

//Convenience function for adding 3 elements to a vector.
template <class T>
void addVertex(std::vector<T>& vec, T x, T y, T z){
	vec.push_back(x);
	vec.push_back(y);
	vec.push_back(z);
};

#endif
