#include "node.hpp"

Node::Node(){};
Node::Node(int arg_x, int arg_y): x{arg_x}, y{arg_y} {};
Node::Node(int arg_x, int arg_y, int arg_id) : x{arg_x}, y{arg_y}, id{arg_id} {};

float abs(Node n){
	return std::sqrt(std::pow(n.x, 2) + std::pow(n.y, 2));
}

float distance(Node n1, Node n2){
	return abs(n1 - n2);
}

Node Node::operator - (Node node){
	return Node(this->x - node.x, this->y - node.y);
}
