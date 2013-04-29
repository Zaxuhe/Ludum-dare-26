#ifndef NODES_H
#define NODES_H

#include "PlatformPrecomp.h"
#include "BaseApp.h"
#include "Entity/EntityUtils.h"
#include "resource_file.h"
#include "sqrat/sqrat.h"
#include <bitset>


class Node
{
public:
	Node* parent; //not actually the node parent, but the parent when pathfinding (a node can have multiple parents)
	bool closed;
	bool opened;
	int x;
	int y;
	int f;
	int g;
	int h;
	bool walkable;
	int type;

	std::vector<Node*> childs;

	Node::Node()
	{
		parent = NULL;
		closed = false;
		opened = false;
		type = 0;

		x = y = f = g = h = 0;
	}

	Node::Node(int x, int y, bool w)
	{
		parent = NULL;
		closed = false;
		opened = false;

		x = y = f = g = h = 0;

		this->walkable = w;
		this->x = x;
		this->y = y;
		type = 0;

	}

	CL_Vec2f Node::getPosition()
	{
		return CL_Vec2f((float)(x), (float)(y));
	}

	Node* Node::getParent()
	{
		return parent;
	}

	void Node::setParent(Node *p)
	{
		parent = p;
	}

	int Node::getX()
	{
		return x;
	}

	int Node::getY()
	{
		return y;
	}

	float Node::getXf()
	{
		return (float)x;
	}

	float Node::getYf()
	{
		return (float)y;
	}

	int Node::getGScore(Node *p)
	{
		return p->g + ((x == p->x || y == p->y) ? 10 : 14);
	}

	int Node::getHScore(Node *p)
	{
		return (abs(p->x - x) + abs(p->y - y)) * 10;
	}

	int Node::getGScore()
	{
		return g;
	}

	int Node::getHScore()
	{
		return h;
	}

	int Node::getFScore()
	{
		return f;
	}

	void Node::computeScores(Node *end)
	{
		g = getGScore(parent);
		h = getHScore(end);
		f = g + h;
	}

	bool Node::hasParent()
	{
		return parent != NULL;
	}

	void addChild(Node *children)
	{
		if (children == 0)
			return;
		childs.push_back(children);
	}
};

class spaceship
{
public:

	spaceship()
	{
		walk_rads_ = 0;
		current_x_ = 0;
		current_y_ = 0;
		
	}


	void set_current_pos(Node* _new_node)
	{
		my_node = _new_node;
		current_x_ = my_node->x;
		current_y_ = my_node->y;
	}

	void draw();


	void create_path(Node* _to_node);

	void walk();

	float current_x_; //pixel
	float current_y_; //pixel
	float walk_rads_; //radians
	Node* my_node;
	std::vector<CL_Vec2f> current_path_; // the path to follow

};

class node_list
{
public:
	node_list()
	{
	}

	~node_list()
	{
		for (int i = 0; i < node_list_.size(); i++)
		{
			delete(node_list_[i]);
			node_list_.clear();
		}
	}

	std::vector <Node*> node_list_;
};

class nodes
{
public:
	nodes();
	void init();

	Entity *entity_;

	void update();
	void draw();
	void generate_node_batcher();
	void set_nodes(int part_);
	void link_nodes(Node* node_1, Node* node_2);
	Node* add_new_node(int _type, int x, int y, Node* parent = 0);
	vector<CL_Vec2f> aStarNodeVersion(Node* start, Node* end);
	void click_on_node(float _x, float _y);
	Node* get_node_by_pos(float _x, float _y);

	RenderBatcher *r_batch_nodes;
	std::vector<spaceship> spaceships_;

	Surface *node_;
	Surface *node_visited_;
	Surface *node_station_;
	Surface *ship_;

	Node* main_station_node_;

	node_list g_node_list;
	


};

extern nodes *g_nodes;
#endif // FADE_OUT_IN_H