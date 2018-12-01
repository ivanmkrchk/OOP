#pragma once
#include <list>
using namespace std;


struct Node {
	int data;
	list<Node> children;

public:
	Node(int data, list<Node> children) {
		this->data = data;
		this->children = children;
	}
};


class Tree {
	Node *root;

public:
	Tree(Node *root) {
		this->root = root;
	}
};

