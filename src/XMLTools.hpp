#ifndef XML_TOOLS_HPP
#define XML_TOOLS_HPP

#include <exception>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
namespace rxml = rapidxml;


//Custom exception to be thrown when an XML node is not found when searching by id.
class XMLNodeNotFoundException : public std::exception{
	public:
		XMLNodeNotFoundException(const std::string& id):
			nodeId("Node not found: " + id){};

		~XMLNodeNotFoundException() throw(){};

		const char* what() const throw() {return nodeId.c_str();};

	private:
		std::string nodeId;
};


//Find a child node, searching by "id" attribute (not node name).
rxml::xml_node<>* getChildNode(rxml::xml_node<>* parent, const std::string id);


#endif
