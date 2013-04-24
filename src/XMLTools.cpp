#include "XMLTools.hpp"

// Search for child node by "id" attribute instead of name.
rxml::xml_node<>* getChildNode(rxml::xml_node<>* parent, const std::string id){
	size_t childCount = rxml::count_children(parent);
	
	if(childCount == 0)
		throw XMLNodeNotFoundException(id);

	bool found = false;

	rxml::xml_node<>* current = parent->first_node();
	for(size_t i = 0; i < childCount; ++i){
		size_t attrCount = rxml::count_attributes(current);
		
		if(attrCount == 0){
			current = current->next_sibling();
			continue;
		}

		if(current->first_attribute("id")->value() == id){
			found = true;
			break;
		}

		current = current->next_sibling();
	}
	
	if(!found)
		throw XMLNodeNotFoundException(id);

	return current;
};