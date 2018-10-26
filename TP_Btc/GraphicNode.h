#pragma once
#include <vector>
#include <string>

struct pos {
	unsigned int xCoord;
	unsigned int yCoord;
};

template<typename T> class GraphicNode
{
public:
	GraphicNode(T& info, pos position_) {
		nodeMejorado = info;
		nodPos.xCoord = position_.xCoord;
		nodPos.yCoord = position_.yCoord;
		nodeName.clear();
		selected = false;
	};

	~GraphicNode() {};

	T& getUtil() {
		return nodeMejorado;
	};

	void setPos(unsigned int x_,unsigned int y_) {
		nodPos.xCoord = x_; // despues ver de limitar con el tamaño del display
		nodPos.yCoord = y_;
	};

	unsigned int getX(void) {
		return nodPos.xCoord;
	};

	unsigned int getY(void) {
		return nodPos.yCoord;
	};

	void toggelSelected(void) {
		selected = !selected;
	};

	bool isSelected(void) {
		return selected;
	}

private:
	
	T nodeMejorado;
	
	pos nodPos;
	bool selected;
	std::string nodeName;
};

