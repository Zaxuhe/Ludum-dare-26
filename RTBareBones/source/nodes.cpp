#include "PlatformPrecomp.h"
#include "nodes.h"
#include "game.h"
#include "spacecraft.h"
nodes *g_nodes;

nodes::nodes()
{

}

void spaceship::walk()
{
	if (current_path_.size() == 0)
	{
		//Logs
		return;		
	}

	int p = current_path_.size() -1;

		
	walk_rads_ = atan2(current_path_[p].y - current_y_, current_path_[p].x - current_x_);
	current_x_ += cosf(walk_rads_) * ((8.0f/100.0f)*(float)GetBaseApp()->GetGameDeltaTick());
	current_y_ += sinf(walk_rads_) * ((8.0f/100.0f)*(float)GetBaseApp()->GetGameDeltaTick());

	//check if its inside the next point
	if (abs(current_x_  - current_path_[p].x) <= 2 && abs(current_y_  - current_path_[p].y) <= 2)
	{
		current_x_ = current_path_[p].x;
		current_y_ = current_path_[p].y;
		my_node = g_nodes->get_node_by_pos(current_x_,current_y_);
		current_path_.erase(current_path_.begin()+p);

		if (my_node->type == 0)
		{
			g_game->state_ = BATTLE_SHIP;
			g_spacecraft->add_people_to_ship();
			//my_node->type = 1;
			g_nodes->r_batch_nodes->clean();
			g_nodes->generate_node_batcher();
			current_path_.clear();
		}
		else if (my_node->type == 2)
		{
			current_path_.clear();
			g_game->state_ = MAIN_MENU;
			g_game->enable_main_menu(true);
		}
	}
}

void spaceship::draw()
{
	g_nodes->ship_->Blit(current_x_-16,current_y_-16,MAKE_RGBA(255,255,255,255),(walk_rads_*180.0f)/PI,CL_Vec2f(current_x_,current_y_));
}

void spaceship::create_path(Node* _to_node)
{
	bool was_walking = false;
	float was_x_ = 0;
	float was_y_ = 0;
	if (current_path_.size() > 0)
	{
		int p = current_path_.size() -1;
		was_walking = true;
		was_x_ = current_path_[p].x;
		was_y_ = current_path_[p].y;
	}
	current_path_.clear();
	current_path_ = g_nodes->aStarNodeVersion(my_node, _to_node);
	if (current_path_.size() == 0 || was_walking)//no encontre camino, es porque es el nodo en el que estoy)
	{
		//we check that we were not walkng there :P
		if (was_walking && current_path_.size() > 0)
		{
			int p = current_path_.size() -1;
			if (was_x_ == current_path_[p].x && was_y_ == current_path_[p].y)
			{
				return;
			}
		}
		current_path_.push_back(CL_Vec2f(my_node->x,my_node->y));
	}
}

void nodes::init()
{
	entity_ = GetEntityRoot()->AddEntity(new Entity("typewriter"));
	//entity_->GetFunction("next_char")->sig_function.connect(boost::bind(&typewriter::next_char, this, _1));
	r_batch_nodes = new RenderBatcher();
	node_ = new Surface("interface/node.rttex");
	node_visited_ = new Surface("interface/node_clear.rttex");
	node_station_ = new Surface("interface/eco_node.rttex");
	ship_ = new Surface("interface/ship.rttex");
	set_nodes(0);
}

Node* nodes::add_new_node(int _type, int x, int y, Node* parent)
{
	g_node_list.node_list_.push_back(new Node());
	g_node_list.node_list_[g_node_list.node_list_.size()-1]->x = x;
	g_node_list.node_list_[g_node_list.node_list_.size()-1]->y = y;
	g_node_list.node_list_[g_node_list.node_list_.size()-1]->type = _type;
	if (parent != 0)
	{
		parent->addChild(g_node_list.node_list_[g_node_list.node_list_.size()-1]);
		//LogMsg("%d",parent);
		//LogMsg("%d",parent->x);
		g_node_list.node_list_[g_node_list.node_list_.size()-1]->addChild(parent);
	}
	return g_node_list.node_list_[g_node_list.node_list_.size()-1];
}

void nodes::link_nodes(Node* node_1, Node* node_2)
{
	node_1->addChild(node_2);
	node_2->addChild(node_1);
}

void nodes::set_nodes(int part_)
{
	if (part_ == 0)
	{
		//type 0 = nodo rojo, 1 nodo azul, 2 estacion espacial
		Node* n1 = add_new_node(2,184,311);
		Node* n2 = add_new_node(0,167,157,n1);
		Node* n3 = add_new_node(0,348,53,n2);
		Node* n4 = add_new_node(0,348,259,n2);
		Node* n5 = add_new_node(0,450,157,n4);
		link_nodes(n5,n3);
		Node* n6 = add_new_node(0,638,107,n5);
		//Node* n7 = add_new_node(0,885,58,n6);
		//Node* n8 = add_new_node(0,845,345,n6);
		//Node* n9 = add_new_node(0,597,542,n8);
		//Node* n10 = add_new_node(0,807,536,n6);
		//Node* n11 = add_new_node(0,475,280,n9);
		Node* n12 = add_new_node(0,426,590,n6);
		Node* n13 = add_new_node(0,278,439,n12);
		Node* n14 = add_new_node(0,76,568,n13);

		//std::vector<CL_Vec2f> res = aStarNodeVersion(n5,n1);

		spaceships_.push_back(spaceship());
		main_station_node_ = n1;
		spaceships_[0].set_current_pos(main_station_node_);
		//spaceships_[0].create_path(n9);
	}
}

void nodes::generate_node_batcher()
{
	for (int i = 0; i < g_node_list.node_list_.size(); i++)
	{
		if (g_node_list.node_list_[i]->type == 0)
		{
			r_batch_nodes->BlitEx(node_,rtRectf(g_node_list.node_list_[i]->x-8,g_node_list.node_list_[i]->y-8,g_node_list.node_list_[i]->x+8,g_node_list.node_list_[i]->y+8),rtRectf(0,0,16,16));
		}
		else if (g_node_list.node_list_[i]->type == 1)
		{
			r_batch_nodes->BlitEx(node_visited_,rtRectf(g_node_list.node_list_[i]->x-8,g_node_list.node_list_[i]->y-8,g_node_list.node_list_[i]->x+8,g_node_list.node_list_[i]->y+8),rtRectf(0,0,16,16));

		}
		else if (g_node_list.node_list_[i]->type == 2)
		{
			r_batch_nodes->BlitEx(node_station_,rtRectf(g_node_list.node_list_[i]->x-16,g_node_list.node_list_[i]->y-16,g_node_list.node_list_[i]->x+16,g_node_list.node_list_[i]->y+16),rtRectf(0,0,32,32));
		}

	}

	int win_ = true;
	for (int i = 0; i < g_node_list.node_list_.size(); i++)
	{
		if (g_node_list.node_list_[i]->type == 0)
		{
			win_ = false;
			break;
		}
	}
	if (win_ == true)
	{
		g_game->state_ = GAME_OVER;
	}

}

void nodes::draw()
{
	//we draw the nodes
	static bool once = false;
	if (once == false)
	{
		generate_node_batcher();


		once = true;
	}
	
	for (int i = 0; i < g_node_list.node_list_.size(); i++)
	{
		//now we draw the node lines
		for (int z = 0; z < g_node_list.node_list_[i]->childs.size(); z++)
		{
			//DrawLine( GLuint rgba, float ax, float ay, float bx, float by, float lineWidth = 2.0f);
			DrawLine(MAKE_RGBA(254,64,44,255),g_node_list.node_list_[i]->x,g_node_list.node_list_[i]->y,g_node_list.node_list_[i]->childs[z]->x,g_node_list.node_list_[i]->childs[z]->y);
		}
		//node_->Blit(g_node_list.node_list_[i]->x-8,g_node_list.node_list_[i]->y-8);
	}
}

void nodes::update()
{

}

vector<CL_Vec2f> nodes::aStarNodeVersion(Node* start, Node* end)
{
    vector<CL_Vec2f> path;

    // Define points to work with
    //Point *start = getPointFromCoord(x1, y1);
    //Point *end = getPointFromCoord(x2, y2);
    Node *current;
    Node *child;

    // Define the open and the close list
    list<Node*> openList;
    list<Node*> closedList;
    list<Node*>::iterator i;

    // Add the start point to the openList
    openList.push_back(start);
    start->opened = true;
	current = start;

	bool skip_cuz_not_found = false; //we had to skip because we didnt found it
	int x,y;
	while (current != end)
    {
		if (openList.size() == 0)
		{
			skip_cuz_not_found = true;
			break;
		}

		i = openList.begin();
		current = (*i);
        // Look for the smallest F value in the openList and make it the current point
        for (i; i != openList.end(); ++ i)
        {
            if ((*i)->getFScore() <= current->getFScore())
            {
                current = (*i);
            }
        }

        // Stop if we reached the end
        if (current == end)
        {
            break;
        }

        // Remove the current point from the openList
        openList.remove(current);
        current->opened = false;

        // Add the current point to the closedList
        closedList.push_back(current);
        current->closed = true;

        // Get all current's adjacent walkable points
		
        for (int i = 0; i < current->childs.size(); i++)
        {
            /*// If it's current point then pass
            if (i==0)
			{
				x=1;
				y=0;
			}
            else if (i==1)
			{
				x=-1;
				y=0;
			}
            else if (i==2)
			{
				x=0;
				y=1;
			}
            else if (i==3)
			{
				x=0;
				y=-1;
			}
            // Get this point*/
            child = current->childs[i];

            // If it's closed or not walkable then pass
            if (child == 0 || child->closed || !child->walkable)
            {
                continue;
            }

            // If it's already in the openList
            if (child->opened)
            {
                // If it has a wroste g score than the one that pass through the current point
                // then its path is improved when it's parent is the current point
                if (child->getGScore() > child->getGScore(current))
                {
                    // Change its parent and g score
                    child->setParent(current);
                    child->computeScores(end);
                }
            }
            else
            {
                // Add it to the openList with current point as parent
                openList.push_back(child);
                child->opened = true;

                // Compute it's g, h and f score
                child->setParent(current);
                child->computeScores(end);
            }
        }
    }

    // Reset
    for (i = openList.begin(); i != openList.end(); ++ i)
    {
        (*i)->opened = false;
    }
    for (i = closedList.begin(); i != closedList.end(); ++ i)
    {
        (*i)->closed = false;
    }

    // Resolve the path starting from the end point
	if (skip_cuz_not_found)
	{
		return path; //we return an empty path :P
	}

    while (current->hasParent() && current != start)
    {
		//LogMsg("%f %f", current->getPosition().x,current->getPosition().y);
        path.push_back(current->getPosition());
        current = current->getParent();
    }

    return path;
}

Node* nodes::get_node_by_pos(float _x, float _y)
{
	for (int i = 0; i < g_node_list.node_list_.size(); i++)
	{
		if (g_node_list.node_list_[i]->x == _x && g_node_list.node_list_[i]->y == _y)
		{
			return g_node_list.node_list_[i];
		}
	}
}

void nodes::click_on_node(float _x, float _y)
{
	if (g_game->already_clicked_ == true)
		return;

	//rtRect(0,0,10,10);
	for (int i = 0; i < g_node_list.node_list_.size(); i++)
	{
		int rect_x_ = g_node_list.node_list_[i]->x-12;
		int rect_y_ = g_node_list.node_list_[i]->y-12;
		int rect_w_ = g_node_list.node_list_[i]->x+12;
		int rect_h_ = g_node_list.node_list_[i]->y+12;

		if (_x >= rect_x_ && _x <= rect_w_)
		{
			if (_y >= rect_y_ && _y <= rect_h_)
			{
				//Clicked on the node :)
				spaceships_[0].create_path(g_node_list.node_list_[i]);
				break;
			}
		}
	}
}