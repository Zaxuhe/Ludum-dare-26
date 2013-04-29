#include "PlatformPrecomp.h"
#include "spacecraft.h"
#include "typewriter.h"
#include "fade_out_in.h"
#include "game.h"
#include "nodes.h"
#include "boost/lexical_cast.hpp"
#include "messagebox.h"

spacecraft *g_spacecraft;

void unit::random_path()
{
	int position = RandomRange(0,g_spacecraft->points_free_.size());
	create_path(g_spacecraft->points_free_[position].x,g_spacecraft->points_free_[position].y);
}

void unit::create_path(int _to_x, int _to_y)
{
	bool entered_ = false;
	int walking_x = current_x_;
	int walking_y = current_y_;
	if (current_path_.size() > 0)
	{
		entered_ = true;
		walking_x = current_path_[current_path_.size()-1].x;
		walking_y = current_path_[current_path_.size()-1].y;
	}
	current_path_.clear();
	current_path_ = g_spacecraft->aStar(walking_x, walking_y, _to_x, _to_y);
	if (entered_ == true) 
	{
		current_path_.push_back(CL_Vec2f(walking_x,walking_y));
	}
	/*steps_ = 0;
	exact_x_ = current_x_ *16+8;
	exact_y_ = current_y_ *16+8;*/
	//LogMsg("PATH:");
	/*if (current_path_.size() == 0)
	{
		//LogMsg("No path found :P");
		
	}
	else
	{
		for (int i = 0; i < current_path_.size(); i++)
		{
			//LogMsg("x: %d, y: %d", (int)end_path[i].x,(int)end_path[i].y);
			r_batch_chars->BlitEx(char_,rtRectf(end_path[i].x*16,end_path[i].y*16,end_path[i].x*16+8,end_path[i].y*16+8),rtRectf(0,0,8,8));
		}
	}*/
}

void unit::draw_to_batch()
{
	if (walk_dir_ == 0 || walk_dir_ == 4)
	{
		g_spacecraft->r_batch_chars4->BlitEx(g_spacecraft->char_4_,rtRectf(exact_x_-4,exact_y_-4,exact_x_+4,exact_y_+4),rtRectf(0,0,8,8));
	}
	else if (walk_dir_ == 1)
	{
		g_spacecraft->r_batch_chars1->BlitEx(g_spacecraft->char_1_,rtRectf(exact_x_-4,exact_y_-4,exact_x_+4,exact_y_+4),rtRectf(0,0,8,8));
	}
	else if (walk_dir_ == 2)
	{
		g_spacecraft->r_batch_chars2->BlitEx(g_spacecraft->char_2_,rtRectf(exact_x_-4,exact_y_-4,exact_x_+4,exact_y_+4),rtRectf(0,0,8,8));
	}
	else if (walk_dir_ == 3)
	{
		g_spacecraft->r_batch_chars3->BlitEx(g_spacecraft->char_3_,rtRectf(exact_x_-4,exact_y_-4,exact_x_+4,exact_y_+4),rtRectf(0,0,8,8));
	}

	if (current_path_.size() == 0 && working_ == true)
	{
		if (work_type_ == 1)
		{
			g_spacecraft->r_batch_tooltips->BlitEx(g_spacecraft->icon_repair_,rtRectf(exact_x_-4,exact_y_-4,exact_x_+4,exact_y_+4),rtRectf(0,0,8,8));
		}
		else if (work_type_ == 2)
		{
			g_spacecraft->r_batch_tooltips->BlitEx(g_spacecraft->icon_work_,rtRectf(exact_x_-4,exact_y_-4,exact_x_+4,exact_y_+4),rtRectf(0,0,8,8));
		}
	}

	/*for (int i = 0; i < current_path_.size(); i++)
	{
		//LogMsg("x: %d, y: %d", (int)end_path[i].x,(int)end_path[i].y);
		r_batch_chars->BlitEx(char_,rtRectf(current_path_[i].x*16,current_path_[i].y*16,current_path_[i].x*16+8,current_path_[i].y*16+8),rtRectf(0,0,8,8));
	}*/
}

Point* spacecraft::getPointFromCoord(int x, int y)
{
	return &tile_set_[x][y];
}

Point* spacecraft::getPoint(int x, int y)
{
	return &tile_set_[x][y];
}

vector<CL_Vec2f> spacecraft::aStar(int x1, int y1, int x2, int y2)
{
    vector<CL_Vec2f> path;

    // Define points to work with
    Point *start = getPointFromCoord(x1, y1);
    Point *end = getPointFromCoord(x2, y2);
    Point *current;
    Point *child;

    // Define the open and the close list
    list<Point*> openList;
    list<Point*> closedList;
    list<Point*>::iterator i;

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
		
        for (int i = 0; i < 4; i ++)
        {
            // If it's current point then pass
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
            // Get this point
            child = getPoint(current->getX() + x, current->getY() + y);

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

bool spacecraft::is_valid_ship()
{
	tile_set_.clear();
	for (int i = 0; i < 36; i++)
	{
		for (int z = 0; z < 36; z++)
		{
				tile_set_[i][z] = Point(i,z,true);
				tile_set_[i][z].x = i;
				tile_set_[i][z].y = z;
				tile_set_[i][z].walkable = false;
		}
	}

	for (int i = 0; i < enemies_list_.size(); i++)
	{
		delete(enemies_list_[i]);
	}
	enemies_list_.clear();
	enemies_list_.push_back(new enemy_ship(0));
	/*enemies_list_.push_back(new enemy_ship(1));
	enemies_list_.push_back(new enemy_ship(2));
	enemies_list_.push_back(new enemy_ship(3));*/

	for(std::map<int, std::map<int, tile_map> >::iterator it = block_list_.begin(); it != block_list_.end(); it++) 
	{
		for(std::map<int, tile_map>::iterator it2 =(*it).second.begin(); it2 != (*it).second.end(); it2++) 
		{
			if ((*it2).second.value_ == 1)
			{
				//blocked_tile_->Blit(200+(*it).first*16,(*it2).first*16+10);
				tile_set_[(*it).first][(*it2).first].walkable = true;
			}
		}
	}

	int energy_num = 0;
	//laser_list_.clear();//
	for (int i = 0; i < module_list_.size(); i++)
	{
		if (module_list_[i]->type_ == 0 || module_list_[i]->type_ == 1 || module_list_[i]->type_ == 2 || module_list_[i]->type_ == 3)
		{
			energy_num+=module_list_[i]->type_+1;
		}
	}

	int laser_num = 0;
	laser_list_.clear();
	for (int i = 0; i < module_list_.size(); i++)
	{
		if (module_list_[i]->type_ == 4 || module_list_[i]->type_ == 5 || module_list_[i]->type_ == 6 || module_list_[i]->type_ == 7)
		{
			laser_list_.push_back(module_list_[i]);
			laser_num++;
		}
	}

	if (energy_num<laser_num)
	{
		g_messagebox->set_message_no_tutorial("No energy","Not enough energy for all the lasers");

		return false; //not enough energy for all the lasers
	}

	//son demasiados lasers
	
	if (laser_num >= 11)
	{
		g_messagebox->set_message_no_tutorial("Too much lasers","Sorry but the ship is limited to 10 lasers");

		return false;
	}

	if (module_list_.size() > 0)
	{
		ship_module *m = module_list_[0];
		for (int i = 1; i < module_list_.size(); i++)
		{
			if (aStar(m->walk_x_,m->walk_y_,module_list_[i]->walk_x_,module_list_[i]->walk_y_).size() == 0) //no path found, means that all are not connected
			{
				g_messagebox->set_message_no_tutorial("Connected","It seems that all the modules are not connected");
				return false;
			}
		}
	}
	else
	{
		g_messagebox->set_message_no_tutorial("Nothing","It seems like there is no ship!");

		return false;
	}


	//tile_set_.clear();
	return true;
}

spacecraft::spacecraft()
{

}

//map size = 36x36

//0 = walkable
//1 = wall
//2 = empty / void
//3 = work spot
int current_size_x_;
int current_size_y_;

//we block the tiles that are already filled by the module
bool spacecraft::block_tiles(ship_module *m)
{
	//first we check if its going to be posible to add it, then we add it
	for (int i = 0; i < m->current_size_x_; i+=2)
	{
		for (int z = 0; z < m->current_size_y_; z+=2)
		{
			//we grab the first tile
			if (m->part_data_[i][z] == 2 && m->part_data_[i+1][z] == 2 && m->part_data_[i+1][z+1] == 2 && m->part_data_[i][z+1] == 2)
			{
				//soy vacio asi que no importa lo que sea puedo ignorarme siempre
				continue;
			}
			if (m->part_data_[i][z] == 1 || m->part_data_[i+1][z] == 1 || m->part_data_[i+1][z+1] == 1 || m->part_data_[i][z+1] == 1)
			{
				//soy barrera, solo puedo ponerme en lugares vacios o con barrera, no en tiles
				if (block_list_[i/2+m->start_x_][z/2+m->start_y_].value_ == 1)
					return false;
				//block_list_[i/2+m->start_x_][z/2+m->start_y_] = 2; //only wall can be put in here
			}
			else
			{
				if (block_list_[i/2+m->start_x_][z/2+m->start_y_].value_ != 0)
					return false;
			}
		}
	}

	m->work_spots_.clear();
	m->repair_spots_.clear();

	for (int i = 0; i < m->current_size_x_; i+=2)
	{
		for (int z = 0; z < m->current_size_y_; z+=2)
		{
			//we grab the first tile
			if (m->part_data_[i][z] == 2 && m->part_data_[i+1][z] == 2 && m->part_data_[i+1][z+1] == 2 && m->part_data_[i][z+1] == 2)
			{
				//empty :/
				continue;
			}
			if (m->part_data_[i][z] == 1 || m->part_data_[i+1][z] == 1 || m->part_data_[i+1][z+1] == 1 || m->part_data_[i][z+1] == 1)
			{
				block_list_[i/2+m->start_x_][z/2+m->start_y_].value_ = 2; //only wall can be put in here
			}
			else
			{
				block_list_[i/2+m->start_x_][z/2+m->start_y_].value_ = 1; //walkable place :)
				block_list_[i/2+m->start_x_][z/2+m->start_y_].m = m; //walkable place :)
				m->walk_x_ = i/2+m->start_x_;
				m->walk_y_ = z/2+m->start_y_;

				//we check that it is a work spot
				if (m->part_data_[i][z] == 3 && m->part_data_[i+1][z] == 3 && m->part_data_[i+1][z+1] == 3 && m->part_data_[i][z+1] == 3)
				{
					m->work_spots_.push_back(work_pos(i/2+m->start_x_,z/2+m->start_y_));
				}
				else
				{
					m->repair_spots_.push_back(work_pos(i/2+m->start_x_,z/2+m->start_y_));
				}
			}
		}
	}
	return true;
}

std::map<int, std::map<int, int> > spacecraft::rotate_tiles(std::map<int, std::map<int, int> > part_data_)
{
	std::map<int, std::map<int, int> > new_data_;

	/*for (int i = 0; i < current_size_y_; i++)
	{
		for (int z = 0; z < current_size_x_; z++)
		{
			new_data_[i][z] = 0; //walkable
		}
	}*/
	if (rotation_ == 0)
	{
		for (std::map<int, std::map<int, int> >::iterator it = part_data_.begin(); it != part_data_.end(); it++)
		{
			for (std::map<int, int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
			{
				new_data_[(*it2).first][(*it).first] = part_data_[(*it).first][(*it2).first]; //change
			}
		}
		rotation_ = 1;
		int temp = current_size_x_;
		current_size_x_ = current_size_y_;
		current_size_y_ = temp;
	}
	else if (rotation_ == 1)
	{
		//we reset back to up and flip it after that
		for (std::map<int, std::map<int, int> >::iterator it = part_data_.begin(); it != part_data_.end(); it++)
		{
			for (std::map<int, int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
			{
				new_data_[(*it2).first][(*it).first] = part_data_[(*it).first][(*it2).first]; //change
			}
		}
		part_data_ = new_data_;
		new_data_.clear();
		int temp = current_size_x_;
		current_size_x_ = current_size_y_;
		current_size_y_ = temp;

		int t_x_ = current_size_x_;

		for (std::map<int, std::map<int, int> >::iterator it = part_data_.begin(); it != part_data_.end(); it++)
		{
			int t_y_ = current_size_y_;
			for (std::map<int, int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
			{
				new_data_[t_x_][t_y_--] = part_data_[(*it).first][(*it2).first]; //change
			}
			t_x_--;
		}

		rotation_ = 2;
		
	}
	else if (rotation_ == 2)
	{
		//we flip it back
		int t_x_ = current_size_x_;
		for (std::map<int, std::map<int, int> >::iterator it = part_data_.begin(); it != part_data_.end(); it++)
		{
			int t_y_ = current_size_y_;
			for (std::map<int, int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
			{
				new_data_[t_x_][t_y_--] = part_data_[(*it).first][(*it2).first]; //change
			}
			t_x_--;
		}
		part_data_ = new_data_;
		new_data_.clear();
		//we rotate it
		for (std::map<int, std::map<int, int> >::iterator it = part_data_.begin(); it != part_data_.end(); it++)
		{
			for (std::map<int, int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
			{
				new_data_[(*it2).first][(*it).first] = part_data_[(*it).first][(*it2).first]; //change
			}
		}
		part_data_ = new_data_;
		new_data_.clear();
		int temp = current_size_x_;
		current_size_x_ = current_size_y_;
		current_size_y_ = temp;
		//we flip again
		t_x_ = current_size_x_;
		for (std::map<int, std::map<int, int> >::iterator it = part_data_.begin(); it != part_data_.end(); it++)
		{
			int t_y_ = current_size_y_;
			for (std::map<int, int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
			{
				new_data_[t_x_][t_y_--] = part_data_[(*it).first][(*it2).first]; //change
			}
			t_x_--;
		}
		
		rotation_ = 3;
	}
	else if (rotation_ == 3)
	{
		rotation_ = 0;
		//we flip it back
		//we flip it back
		int t_x_ = current_size_x_;
		for (std::map<int, std::map<int, int> >::iterator it = part_data_.begin(); it != part_data_.end(); it++)
		{
			int t_y_ = current_size_y_;
			for (std::map<int, int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
			{
				new_data_[t_x_][t_y_--] = part_data_[(*it).first][(*it2).first]; //change
			}
			t_x_--;
		}
		part_data_ = new_data_;
		new_data_.clear();
		//we rotate it
		for (std::map<int, std::map<int, int> >::iterator it = part_data_.begin(); it != part_data_.end(); it++)
		{
			for (std::map<int, int>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
			{
				new_data_[(*it2).first][(*it).first] = part_data_[(*it).first][(*it2).first]; //change
			}
		}
		int temp = current_size_x_;
		current_size_x_ = current_size_y_;
		current_size_y_ = temp;

	}

	return new_data_;
}

void spacecraft::init()
{
	finished_levels_ = 0;
	entity_ = GetEntityRoot()->AddEntity(new Entity("game"));
	floor_16 = new Surface("interface/floor_16.rttex");
	floor_16_selected = new Surface("interface/floor_16_selected.rttex");
	hp_bg_ = new image_object_("hp_bg.rttex",false);

	work_free_16 = new Surface("interface/work_free.rttex");

	wall_16 = new Surface("interface/wall_16.rttex");
	blocked_tile_ = new Surface("interface/repeat_tile.rttex");

	bg_editor_ = new image_object_("background_editor.rttex",false);
	bg_editor_->set_position(200,10);
	bg_editor_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_board, this, _1));

	cancel_btn_ = new image_object_("btn_cancel.rttex",false);
	cancel_btn_->set_position(40,400);
	cancel_btn_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_cancel, this, _1));

	add_btn_ = new image_object_("add_part.rttex",false);
	add_btn_->set_position(40,200);
	add_btn_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_add, this, _1));

	save_btn_ = new image_object_("btn_save.rttex",false);
	save_btn_->set_position(800,40);
	save_btn_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_save, this, _1));

	del_btn_ = new image_object_("btn_del.rttex",false);
	del_btn_->set_position(40,500);
	del_btn_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_del, this, _1));

	rotate_btn_ = new image_object_("rotate_part.rttex",false);
	rotate_btn_->set_position(40,300);
	rotate_btn_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_rotate, this, _1));

	battle_right_ = new image_object_("battle_right.rttex",false);
	battle_right_->set_position(600,0);

	btn_add_worker_ = new image_object_("add_worker.rttex",false);
	btn_add_worker_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_add_worker, this, _1));
	btn_add_worker_->set_position(607,361);

	btn_remove_worker_ = new image_object_("remove_worker.rttex",false);
	btn_remove_worker_->set_position(607,487);
	btn_remove_worker_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_remove_worker, this, _1));

	btn_add_repair_ = new image_object_("add_repair.rttex",false);
	btn_add_repair_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_add_repair, this, _1));
	btn_add_repair_->set_position(607,61);

	btn_remove_repair_ = new image_object_("remove_repair.rttex",false);
	btn_remove_repair_->set_position(607,187);
	btn_remove_repair_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_remove_repair, this, _1));

	char_1_ = new Surface("interface/unit_1.rttex");
	char_2_ = new Surface("interface/unit_2.rttex");
	char_3_ = new Surface("interface/unit_3.rttex");
	char_4_ = new Surface("interface/unit_4.rttex");

	fire_s_ = new Surface("interface/tiny_fire.rttex");
	fire_m_ = new Surface("interface/med_fire.rttex");
	fire_b_ = new Surface("interface/huge_fire.rttex");
	icon_repair_ = new Surface("interface/work_repair.rttex");
	icon_work_ = new Surface("interface/work_work.rttex");

	enemy_1_ = new image_object_("enemy_1.rttex",false);
	enemy_2_ = new image_object_("enemy_2.rttex",false);
	enemy_3_ = new image_object_("enemy_3.rttex",false);
	
	r_batch_chars1 = new RenderBatcher();
	r_batch_chars2 = new RenderBatcher();
	r_batch_chars3 = new RenderBatcher();
	r_batch_chars4 = new RenderBatcher();
	r_batch_tooltips = new RenderBatcher();
	r_batch_fire = new RenderBatcher();

	btn_border_1_ = new image_object_("border_attack.rttex",false);
	btn_border_2_ = new image_object_("border_attack.rttex",false);
	btn_border_3_ = new image_object_("border_attack.rttex",false);
	btn_border_4_ = new image_object_("border_attack.rttex",false);
	btn_border_5_ = new image_object_("border_attack.rttex",false);
	btn_border_6_ = new image_object_("border_attack.rttex",false);
	btn_border_7_ = new image_object_("border_attack.rttex",false);
	btn_border_8_ = new image_object_("border_attack.rttex",false);
	btn_border_9_ = new image_object_("border_attack.rttex",false);
	btn_border_10_ = new image_object_("border_attack.rttex",false);

	btn_border_1_->set_position(730,30+58*0);
	btn_border_2_->set_position(730,30+58*1);
	btn_border_3_->set_position(730,30+58*2);
	btn_border_4_->set_position(730,30+58*3);
	btn_border_5_->set_position(730,30+58*4);
	btn_border_6_->set_position(730,30+58*5);
	btn_border_7_->set_position(730,30+58*6);
	btn_border_8_->set_position(730,30+58*7);
	btn_border_9_->set_position(730,30+58*8);
	btn_border_10_->set_position(730,30+58*9);

	btn_border_1_e_ = new image_object_("border_attack.rttex",false);
	btn_border_2_e_ = new image_object_("border_attack.rttex",false);
	btn_border_3_e_ = new image_object_("border_attack.rttex",false);
	btn_border_4_e_ = new image_object_("border_attack.rttex",false);
	btn_border_5_e_ = new image_object_("border_attack.rttex",false);
	btn_border_6_e_ = new image_object_("border_attack.rttex",false);
	btn_border_7_e_ = new image_object_("border_attack.rttex",false);
	btn_border_8_e_ = new image_object_("border_attack.rttex",false);
	btn_border_9_e_ = new image_object_("border_attack.rttex",false);
	btn_border_10_e_ = new image_object_("border_attack.rttex",false);

	btn_border_1_e_->set_position(730+122,30+58*0);
	btn_border_2_e_->set_position(730+122,30+58*1);
	btn_border_3_e_->set_position(730+122,30+58*2);
	btn_border_4_e_->set_position(730+122,30+58*3);
	btn_border_5_e_->set_position(730+122,30+58*4);
	btn_border_6_e_->set_position(730+122,30+58*5);
	btn_border_7_e_->set_position(730+122,30+58*6);
	btn_border_8_e_->set_position(730+122,30+58*7);
	btn_border_9_e_->set_position(730+122,30+58*8);
	btn_border_10_e_->set_position(730+122,30+58*9);

	selected_laser_ = 0;

	btn_border_1_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_1, this, _1));
	btn_border_2_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_2, this, _1));
	btn_border_3_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_3, this, _1));
	btn_border_4_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_4, this, _1));
	btn_border_5_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_5, this, _1));
	btn_border_6_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_6, this, _1));
	btn_border_7_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_7, this, _1));
	btn_border_8_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_8, this, _1));
	btn_border_9_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_9, this, _1));
	btn_border_10_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_laser_10, this, _1));

	btn_border_1_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_1, this, _1));
	btn_border_2_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_2, this, _1));
	btn_border_3_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_3, this, _1));
	btn_border_4_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_4, this, _1));
	btn_border_5_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_5, this, _1));
	btn_border_6_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_6, this, _1));
	btn_border_7_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_7, this, _1));
	btn_border_8_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_8, this, _1));
	btn_border_9_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_9, this, _1));
	btn_border_10_e_->set_click_function("OnTouchEnd",boost::bind(&spacecraft::click_eship_10, this, _1));



	for (int i = 0; i < 16; i++)
	{
		avaible_ship_modules_[i] = 0;
	}

	adding_object_ = 0;
	selected_object_ = 0;

	for (int i = 0; i < 36; i++)
	{
		for (int z = 0; z < 36; z++)
		{
			block_list_[i][z].value_ = 0; //walkable
		}
	}

	r_batch_walls = new RenderBatcher();
	r_batch_floor = new RenderBatcher();
	r_batch_work_floor = new RenderBatcher();
	r_batch_wall_lock_air = new RenderBatcher();
}

void  spacecraft::click_eship_1(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 1)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[0]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_eship_2(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 2)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[1]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_eship_3(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 3)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[2]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_eship_4(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 4)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[3]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_eship_5(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 5)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[4]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_eship_6(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 6)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[5]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_eship_7(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 7)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[6]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_eship_8(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 8)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[7]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_eship_9(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 9)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[8]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_eship_10(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && enemies_list_.size() >= 10)
	{
		g_game->already_clicked_ = true;
		if (selected_laser_ != 0)
		{
			//we now add this as the shooter for the ship
			//we clear all the enemies this could be shooting at
			for (int z =0; z < enemies_list_.size(); z++)
			{
				for (int i = 0; i < enemies_list_[z]->laser_ids_.size(); i++)
				{
					if (enemies_list_[z]->laser_ids_[i] == selected_laser_-1)
					{
						enemies_list_[z]->laser_ids_.erase(enemies_list_[z]->laser_ids_.begin()+i);
						break;
					}
				}
			}
			enemies_list_[9]->laser_ids_.push_back(selected_laser_-1);
		}
	}
}

void  spacecraft::click_laser_1(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 1)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 1;
	}
}

void  spacecraft::click_laser_2(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 2)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 2;
	}
}

void  spacecraft::click_laser_3(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 3)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 3;
	}
}

void  spacecraft::click_laser_4(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 4)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 4;
	}
}

void  spacecraft::click_laser_5(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 5)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 5;
	}
}

void  spacecraft::click_laser_6(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 6)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 6;
	}
}

void  spacecraft::click_laser_7(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 7)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 7;
	}
}

void  spacecraft::click_laser_8(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 8)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 8;
	}
}

void  spacecraft::click_laser_9(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 9)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 9;
	}
}

void  spacecraft::click_laser_10(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == BATTLE_SHIP && laser_list_.size() >= 10)
	{
		g_game->already_clicked_ = true;
		selected_laser_ = 10;
	}
}

void spacecraft::click_board(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == CREATE_SHIP && adding_object_ != 0)
	{
		g_game->already_clicked_ = true;
		CL_Vec2f click_pos = pVList->Get(0).GetVector2();
		click_pos.x-=200;
		click_pos.y-=10;
		//LogMsg("%f %f",click_pos.x,click_pos.y);
		//LogMsg("%d, %d",(int)floorf(click_pos.x/16.0f),(int)floorf(click_pos.y/16.0f));
		adding_object_->current_size_x_ = current_size_x_;
		adding_object_->current_size_y_ = current_size_y_;
		adding_object_->start_x_ = (int)floorf(click_pos.x/16.0f);
		adding_object_->start_y_ = (int)floorf(click_pos.y/16.0f);
		bool res = block_tiles(adding_object_);
		if (res == true)
		{
			//agregamos el objecto
			avaible_ship_modules_[adding_object_->type_]--;
			module_list_.push_back(adding_object_);
		}
		else
		{
			delete(adding_object_);
		}
		//we reset the walking list
		for (int i = 0; i < 36; i++)
		{
			for (int z = 0; z < 36; z++)
			{
				block_list_[i][z].value_ = 0; //walkable
			}
		}

		for (int i = 0; i < module_list_.size(); i++)
		{
			block_tiles(module_list_[i]);
		}
		adding_object_ = 0;
	}
	else if(g_game->state_ == CREATE_SHIP && adding_object_ == 0)
	{
		g_game->already_clicked_ = true;
		//we are picking a ship
		CL_Vec2f click_pos = pVList->Get(0).GetVector2();
		click_pos.x-=200;
		click_pos.y-=10;
		if (block_list_[(int)floorf(click_pos.x/16.0f)][(int)floorf(click_pos.y/16.0f)].value_ == 1 && block_list_[(int)floorf(click_pos.x/16.0f)][(int)floorf(click_pos.y/16.0f)].m != 0)
		{
			selected_object_ = block_list_[(int)floorf(click_pos.x/16.0f)][(int)floorf(click_pos.y/16.0f)].m;
		}
		else 
		{
			selected_object_ = 0;
		}
	}
	else if(g_game->state_ == BATTLE_SHIP)
	{
		g_game->already_clicked_ = true;
		//we are picking a ship
		CL_Vec2f click_pos = pVList->Get(0).GetVector2();
		click_pos.x-=10;
		click_pos.y-=10;
		if (block_list_[(int)floorf(click_pos.x/16.0f)][(int)floorf(click_pos.y/16.0f)].m != 0)
		{
			selected_object_ = block_list_[(int)floorf(click_pos.x/16.0f)][(int)floorf(click_pos.y/16.0f)].m;
			if (selected_object_->type_ >= 8 && selected_object_->type_ <= 11)
			{
				selected_object_ = 0;
			}
		}
		else
		{
			selected_object_ = 0;
		}
	}
}

void spacecraft::click_save(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (g_game->state_ == CREATE_SHIP)
	{
		g_game->already_clicked_ = true;
		if (is_valid_ship())
		{
			//LogMsg("Yay I have created a valid ship");
			//We have a valid ship
			g_game->enable_main_menu(true);
			g_game->state_ = MAIN_MENU;
			selected_object_ = 0;
			if (g_game->tutorial_ == 3)
			{
				g_messagebox->set_message_no_tutorial("Let's continue","Now its time to add some wokers, go to the job centre to do so");
			}
		}
		else
		{
			LogMsg("Damn I have not created a valid ship");
		}
	}
}

void spacecraft::click_del(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (g_game->state_ == CREATE_SHIP && selected_object_ != 0)
	{
		avaible_ship_modules_[selected_object_->type_]++;
		delete(selected_object_);
		for (int i = 0; i < module_list_.size(); i++)
		{
			if (module_list_[i] == selected_object_)
			{
				module_list_.erase(module_list_.begin()+i);
				break;
			}
		}
		selected_object_ = 0;
	}

	//we reset the walking list
	for (int i = 0; i < 36; i++)
	{
		for (int z = 0; z < 36; z++)
		{
			block_list_[i][z].value_ = 0; //walkable
		}
	}

	for (int i = 0; i < module_list_.size(); i++)
	{
		block_tiles(module_list_[i]);
	}

}

void spacecraft::click_cancel(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == CREATE_SHIP && adding_object_ != 0)
	{
		g_game->already_clicked_ = true;
		//adding_object_->part_data_ = rotate_tiles(adding_object_->part_data_);
		delete( adding_object_);
		 adding_object_ = 0;

	}
	else if (g_game->state_ == CREATE_SHIP && selected_object_ != 0)
	{
		g_game->already_clicked_ = true;
		selected_object_ = 0;
	}
}


void spacecraft::click_rotate(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == CREATE_SHIP && adding_object_ != 0)
	{
		g_game->already_clicked_ = true;
		adding_object_->part_data_ = rotate_tiles(adding_object_->part_data_);

	}
}

void spacecraft::click_add(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if(g_game->state_ == CREATE_SHIP)
	{

		g_game->already_clicked_ = true;
		/*selected_object_ = 0;
		if (adding_object_ != 0)
		{
			delete (adding_object_);
		}
		adding_object_ = new ship_module();
		adding_object_->part_data_ = get_part(0);
		adding_object_->type_ = 0;*/

		selected_object_ = 0;
		if (adding_object_ != 0)
		{
			delete (adding_object_);
			adding_object_ = 0;
		}

		g_game->state_ = SELECT_MODULE;
	}
}

void spacecraft::add_people_to_ship()
{


	for (int i = 0; i < module_list_.size(); i++)
	{
		module_list_[i]->hitpoints_ = 100;
		module_list_[i]->work_points_ = 0;
		module_list_[i]->working_people_ = 0;
		module_list_[i]->repearing_people_ = 0;
		for (int z = 0; z < module_list_[i]->work_spots_.size();z++)
		{
			module_list_[i]->work_spots_[z].worker_ = 0;
		}
		for (int z = 0; z < module_list_[i]->repair_spots_.size();z++)
		{
			module_list_[i]->repair_spots_[z].worker_ = 0;
		}

	}
	working_people_ = 0;

	unit_list_.clear();
	for (int i = 0; i < 36; i++)
	{
		for (int z = 0; z < 36; z++)
		{
			if (tile_set_[i][z].walkable == true)
			{
				points_free_.push_back(CL_Vec2f(tile_set_[i][z].x,tile_set_[i][z].y));
			}
		}
	}

	for (int i = 0; i < g_game->number_of_employes_; i++)
	{
		int position = RandomRange(0,points_free_.size());
		unit_list_.push_back(unit());
		unit_list_[unit_list_.size()-1].set_current_pos(points_free_[position].x,points_free_[position].y);
		//unit_list_[unit_list_.size()-1].create_path(29,29);
	}
	selected_laser_ = 0;
	selected_object_ = 0;

	for (int i = 0; i < enemies_list_.size(); i++)
	{
		delete(enemies_list_[i]);
	}
	enemies_list_.clear();

	//levels
	if (finished_levels_ == 0)
	{
		enemies_list_.push_back(new enemy_ship(0));
	}
	else if (finished_levels_ == 1)
	{
		enemies_list_.push_back(new enemy_ship(0));
		enemies_list_.push_back(new enemy_ship(1));
	}
	else if (finished_levels_ == 2)
	{
		enemies_list_.push_back(new enemy_ship(2));
	}
	else if (finished_levels_ == 3)
	{
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
	}
	else if (finished_levels_ == 4)
	{
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
	}
	else if (finished_levels_ == 5)
	{
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(2));
	}
	else if (finished_levels_ == 6)
	{
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(1));
	}
	else if (finished_levels_ == 7)
	{
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(3));
		//enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(1));
	}
	else if (finished_levels_ == 8)
	{
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(1));
	}
	else if (finished_levels_ == 9)
	{
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
	}
	else if (finished_levels_ == 10)
	{
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(2));
	}
	else if (finished_levels_ == 11)
	{
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(2));
	}
	else if (finished_levels_ == 12)
	{
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(1));
	}
	else if (finished_levels_ == 13)
	{
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(3));
		enemies_list_.push_back(new enemy_ship(1));
		enemies_list_.push_back(new enemy_ship(2));
		enemies_list_.push_back(new enemy_ship(3));
	}


	/*enemies_list_.push_back(new enemy_ship(1));
	enemies_list_.push_back(new enemy_ship(2));
	enemies_list_.push_back(new enemy_ship(3));*/
	if (g_game->tutorial_ == 4)
	{
		g_messagebox->set_message("Battle!", "You are on battle! against 1 ship (at the right), click on your laser and then on the enemy to target it, OMG! a module is on FIRE! click on it and click on 'Add Repair' to make someone repair it!, also your laser is not loaded click on it and then on 'Add worker' to put someone to load it (the more the faster it loads)");
	}

}

void spacecraft::attack_random_module(float damage)
{
	std::vector <ship_module*> possible_list_;
	for (int i = 0; i <  module_list_.size(); i++)
	{
		if (module_list_[i]->type_ <8)
		{
			possible_list_.push_back(module_list_[i]);
		}
	}

	int attack_to = RandomRange(0,possible_list_.size());
	possible_list_[attack_to]->hitpoints_-=damage;
	if (possible_list_[attack_to]->hitpoints_ <= 0)
	{
		g_game->state_ = SHIP_NODES;
		g_nodes->spaceships_[0].set_current_pos(g_nodes->main_station_node_);
	}

}

void spacecraft::draw_battle()
{
	bg_editor_->set_position(10,10);
	//bg_editor_->draw();
	battle_right_->draw();


	for (int i = 0; i < module_list_.size(); i++)
	{
		for(std::map<int, std::map<int, int> >::iterator it = module_list_[i]->part_data_.begin(); it != module_list_[i]->part_data_.end(); it++) 
		{
			for(std::map<int, int>::iterator it2 =(*it).second.begin(); it2 != (*it).second.end(); it2++) 
			{

				if ((*it2).second == 0 || (*it2).second == 3)
				{
					//we draw it at the position
					if (selected_object_ == module_list_[i])
					{
						//floor_16_selected->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
						r_batch_floor->BlitEx(floor_16_selected,rtRectf(10+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,10+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));

					}
					else
					{
						if ((*it2).second == 3)
						{
							//work_free_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
							r_batch_work_floor->BlitEx(work_free_16,rtRectf(10+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,10+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));

						}
						else
						{
							//floor_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
							r_batch_floor->BlitEx(floor_16,rtRectf(10+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,10+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));

						}
					}


					if (module_list_[i]->hitpoints_ < 100 && module_list_[i]->hitpoints_ > 66)
					{
						//we only render this outside the editor
						r_batch_fire->BlitEx(fire_s_,rtRectf(10+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,10+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,16,16));
						//wall_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
					}
					else if (module_list_[i]->hitpoints_ <= 66 && module_list_[i]->hitpoints_ > 33)
					{
						//we only render this outside the editor
						r_batch_fire->BlitEx(fire_m_,rtRectf(10+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,10+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,16,16));
						//wall_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
					}
					if (module_list_[i]->hitpoints_ < 33)
					{
						//we only render this outside the editor
						r_batch_fire->BlitEx(fire_b_,rtRectf(10+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,10+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,16,16));
						//wall_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
					}
				}
				else if ((*it2).second == 1)
				{
					r_batch_walls->BlitEx(wall_16,rtRectf(10+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,10+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));
					//wall_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
				}
				else if ((*it2).second == -1)
				{
					//we only render this outside the editor
					r_batch_wall_lock_air->BlitEx(wall_16,rtRectf(10+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,10+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));
					//wall_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
				}


			}
		}
	}

	r_batch_wall_lock_air->Flush();
	r_batch_walls->Flush();
	r_batch_work_floor->Flush();
	r_batch_floor->Flush();
	r_batch_fire->Flush();

	glPushMatrix();
		glTranslatef(10,10,0);

		for (int i = 0; i < unit_list_.size(); i++)
		{
			unit_list_[i].walk();
			unit_list_[i].draw_to_batch();
		}
		r_batch_chars1->Flush();
		r_batch_chars2->Flush();
		r_batch_chars3->Flush();
		r_batch_chars4->Flush();
		r_batch_tooltips->Flush();
	glPopMatrix();

	//we draw the enemies
	//we draw the lasers
	int enemy_x = 859;
	int enemy_y = 35;

	for (int i = 0; i < enemies_list_.size(); i++)
	{

		float current_hp_size = (enemies_list_[i]->hp_ * 69)/ enemies_list_[i]->max_hp_;
	
		//hp_bg_->set_position(615,35);
		hp_bg_->set_position(enemy_x,enemy_y);
		hp_bg_->draw();
		DrawLine(MAKE_RGBA(175,45,19,255),enemy_x+5,enemy_y+7,enemy_x+5+current_hp_size,enemy_y+7,5.0f);
		DrawLine(MAKE_RGBA(119,32,13,255),enemy_x+5,enemy_y+12,enemy_x+5+current_hp_size,enemy_y+12,5.0f);

		//GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(enemy_x,enemy_y+28, "4 Lasers",0.7f,MAKE_RGBA(255,255,255,255));
		if (enemies_list_[i]->type_ == 0 || enemies_list_[i]->type_ == 1)
		{
			enemy_1_->set_position(enemy_x+25,enemy_y+20);
			enemy_1_->draw();
		}
		else if (enemies_list_[i]->type_ == 2)
		{
			enemy_2_->set_position(enemy_x+25,enemy_y+20);
			enemy_2_->draw();
		}
		else if (enemies_list_[i]->type_ == 3)
		{
			enemy_3_->set_position(enemy_x+25,enemy_y+20);
			enemy_3_->draw();
		}

		for (int z = 0; z < enemies_list_[i]->laser_ids_.size(); z++)
		{
			DrawLine(MAKE_RGBA(255,239,25,255),enemy_x+23,enemy_y+36,737+69,enemies_list_[i]->laser_ids_[z]*58+35+10);
		}

		enemies_list_[i]->update();
		
		//we shoot
		if (enemies_list_[i]->type_ == 0 && enemies_list_[i]->attack_speed_ > 1000000) //the tutorial ship should only shoot once!
		{
			attack_random_module(50);
			enemies_list_[i]->attack_speed_ = 0;
		}
		else if (enemies_list_[i]->type_ == 1 && enemies_list_[i]->attack_speed_ > 1000)
		{
			attack_random_module(10);
			enemies_list_[i]->attack_speed_ = 0;
		}
		else if (enemies_list_[i]->type_ == 2 && enemies_list_[i]->attack_speed_ > 900)
		{
			attack_random_module(15);

			enemies_list_[i]->attack_speed_ = 0;
		}
		else if (enemies_list_[i]->type_ == 3 && enemies_list_[i]->attack_speed_ > 800)
		{
			attack_random_module(20);
			enemies_list_[i]->attack_speed_ = 0;
		}

		enemy_y+= 58;

	}


	//we draw the lasers
	int laser_x = 737;
	int laser_y = 35;
	for (int i = 0; i < laser_list_.size(); i++)
	{
		float req_to_shoot_ =0;
		if (laser_list_[i]->type_ == 4)
		{
			req_to_shoot_ = 50;
		}
		if (laser_list_[i]->type_ == 5)
		{
			req_to_shoot_ = 220;
		}
		if (laser_list_[i]->type_ == 6)
		{
			req_to_shoot_ = 320;
		}
		if (laser_list_[i]->type_ == 7)
		{
			req_to_shoot_ = 420;
		}

		float current_hp_size = (laser_list_[i]->work_points_ * 69)/ req_to_shoot_;
		//float current_hp_size = (30 * 69)/ req_to_shoot_;
	
		//hp_bg_->set_position(615,35);
		hp_bg_->set_position(laser_x,laser_y);
		hp_bg_->draw();
		DrawLine(MAKE_RGBA(75,180,96,255),laser_x+5,laser_y+7,laser_x+5+current_hp_size,laser_y+7,5.0f);
		DrawLine(MAKE_RGBA(49,130,62,255),laser_x+5,laser_y+12,laser_x+5+current_hp_size,laser_y+12,5.0f);
		if (laser_list_[i]->type_ == 4)
		{
			GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(laser_x,laser_y+28, "1 Laser",0.7f,MAKE_RGBA(255,255,255,255));
		}
		if (laser_list_[i]->type_ == 5)
		{
			GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(laser_x,laser_y+28, "2 Lasers",0.7f,MAKE_RGBA(255,255,255,255));
		}
		if (laser_list_[i]->type_ == 6)
		{
			GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(laser_x,laser_y+28, "3 Lasers",0.7f,MAKE_RGBA(255,255,255,255));
		}
		if (laser_list_[i]->type_ == 7)
		{
			GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(laser_x,laser_y+28, "4 Lasers",0.7f,MAKE_RGBA(255,255,255,255));
		}

		laser_y+= 58;
	}
	if (selected_object_ != 0)
	{
		
		float current_hp_size = (selected_object_->hitpoints_ * 69)/ 100;
		hp_bg_->set_position(615,35);
		hp_bg_->draw();
		DrawLine(MAKE_RGBA(175,45,19,255),620,42,620+current_hp_size,42,5.0f);
		DrawLine(MAKE_RGBA(119,32,13,255),620,47,620+current_hp_size,47,5.0f);

		

		if (selected_object_->hitpoints_ == 100)
		{

			GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(608,150, "This \nmodule \ndosen't \nneed \nrepairs",0.95f,MAKE_RGBA(255,255,255,255));
		}
		else
		{
			if (selected_object_->repearing_people_ != selected_object_->repair_spots_.size())
			{
				btn_add_repair_->draw();
			}
			if (selected_object_->repearing_people_ != 0)
			{
				btn_remove_repair_->draw();
			}
		}
		if (selected_object_->work_spots_.size()==0)
		{
			GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(608,400, "This \nmodule \ndosen't \nneed \nworkers",0.95f,MAKE_RGBA(255,255,255,255));
		}
		else
		{
			GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(620,330, boost::lexical_cast<std::string>(selected_object_->working_people_)+" of "+ boost::lexical_cast<std::string>(selected_object_->work_spots_.size()),0.95f,MAKE_RGBA(255,255,255,255));
			if (selected_object_->working_people_ != selected_object_->work_spots_.size())
			{
				btn_add_worker_->draw();
			}
			if (selected_object_->working_people_ != 0)
			{
				btn_remove_worker_->draw();
			}
		}
	}

	if (selected_laser_ == 1)
		btn_border_1_->draw();
	if (selected_laser_ == 2)
		btn_border_2_->draw();
	if (selected_laser_ == 3)
		btn_border_3_->draw();
	if (selected_laser_ == 4)
		btn_border_4_->draw();
	if (selected_laser_ == 5)
		btn_border_5_->draw();
	if (selected_laser_ == 6)
		btn_border_6_->draw();
	if (selected_laser_ == 7)
		btn_border_7_->draw();
	if (selected_laser_ == 8)
		btn_border_8_->draw();
	if (selected_laser_ == 9)
		btn_border_9_->draw();
	if (selected_laser_ == 10)
		btn_border_10_->draw();

	/*btn_border_1_e_->draw();
	btn_border_2_e_->draw();
	btn_border_3_e_->draw();
	btn_border_4_e_->draw();
	btn_border_5_e_->draw();
	btn_border_6_e_->draw();
	btn_border_7_e_->draw();
	btn_border_8_e_->draw();
	btn_border_9_e_->draw();
	btn_border_10_e_->draw();*/
}

void spacecraft::click_add_repair(VariantList *pVList)
{
	if (g_game->state_ == BATTLE_SHIP && selected_object_ != 0)
	{
		if (selected_object_->repearing_people_ < selected_object_->repair_spots_.size()) //we can add a new worker
		{
			//we look for free people
			if (g_game->number_of_employes_-working_people_ > 0)// we have enough people, we look for someone that is not working
			{
				int unit_num_;
				for (int i = 0; i < unit_list_.size(); i++)
				{
					if (unit_list_[i].working_ == false)
					{
						unit_num_ = i; //we found a non working person
						break;
					}
				}

				unit_list_[unit_num_].working_ = true;
				for (int i = 0; i < selected_object_->repair_spots_.size(); i++)
				{
					if (selected_object_->repair_spots_[i].worker_ == 0) //nobody is working here :P
					{
						unit_list_[unit_num_].create_path(selected_object_->repair_spots_[i].x_,selected_object_->repair_spots_[i].y_);
						unit_list_[unit_num_].work_type_ = 1;
						selected_object_->repair_spots_[i].worker_ = &unit_list_[unit_num_];

						break;
					}
				}


				//we add the free person to a free spot on the selcted_boject
				working_people_++;
				selected_object_->repearing_people_++;
			}
		}
	}

}

void spacecraft::click_remove_repair(VariantList *pVList)
{
	if (g_game->state_ == BATTLE_SHIP && selected_object_ != 0)
	{
		if (selected_object_->repearing_people_ > 0) //we can remove one
		{
			//we search for a worker
			for (int i = 0; i < selected_object_->repair_spots_.size(); i++)
			{
				if (selected_object_->repair_spots_[i].worker_ != 0) //somebody is working here :P
				{
					//unit_list_[unit_num_].create_path(selected_object_->work_spots_[i].x_,selected_object_->work_spots_[i].y_);
					selected_object_->repair_spots_[i].worker_->working_ = false;
					selected_object_->repair_spots_[i].worker_->work_type_ = 0;
					selected_object_->repair_spots_[i].worker_ = 0;

					break;
				}
			}
			
			working_people_--;
			selected_object_->repearing_people_--;
		}
	}

}

void spacecraft::click_add_worker(VariantList *pVList)
{
	if (g_game->state_ == BATTLE_SHIP && selected_object_ != 0)
	{
		if (selected_object_->working_people_ < selected_object_->work_spots_.size()) //we can add a new worker
		{
			//we look for free people
			if (g_game->number_of_employes_-working_people_ > 0)// we have enough people, we look for someone that is not working
			{
				int unit_num_;
				for (int i = 0; i < unit_list_.size(); i++)
				{
					if (unit_list_[i].working_ == false)
					{
						unit_num_ = i; //we found a non working person
						break;
					}
				}

				unit_list_[unit_num_].working_ = true;
				for (int i = 0; i < selected_object_->work_spots_.size(); i++)
				{
					if (selected_object_->work_spots_[i].worker_ == 0) //nobody is working here :P
					{
						unit_list_[unit_num_].create_path(selected_object_->work_spots_[i].x_,selected_object_->work_spots_[i].y_);
						unit_list_[unit_num_].work_type_ = 2;
						selected_object_->work_spots_[i].worker_ = &unit_list_[unit_num_];

						break;
					}
				}


				//we add the free person to a free spot on the selcted_boject
				working_people_++;
				selected_object_->working_people_++;
			}
		}
	}

}

void spacecraft::click_remove_worker(VariantList *pVList)
{
	if (g_game->state_ == BATTLE_SHIP && selected_object_ != 0)
	{
		if (selected_object_->working_people_ > 0) //we can remove one
		{
			//we search for a worker
			for (int i = 0; i < selected_object_->work_spots_.size(); i++)
			{
				if (selected_object_->work_spots_[i].worker_ != 0) //somebody is working here :P
				{
					//unit_list_[unit_num_].create_path(selected_object_->work_spots_[i].x_,selected_object_->work_spots_[i].y_);
					selected_object_->work_spots_[i].worker_->working_ = false;
					selected_object_->work_spots_[i].worker_->work_type_ = 0;
					selected_object_->work_spots_[i].worker_ = 0;

					break;
				}
			}
			
			working_people_--;
			selected_object_->working_people_--;
		}
	}

}

void spacecraft::draw()
{
	bg_editor_->set_position(200,10);
	//we print the selected object
	if (adding_object_ != 0)
	{
		for(std::map<int, std::map<int, int> >::iterator it = adding_object_->part_data_.begin(); it != adding_object_->part_data_.end(); it++) 
		{
			for(std::map<int, int>::iterator it2 =(*it).second.begin(); it2 != (*it).second.end(); it2++) 
			{
				if ((*it2).second == 0)
				{
					//we draw it at the position
					r_batch_floor->BlitEx(floor_16,rtRectf(10+(*it).first*8,(*it2).first*8+30,10+(*it).first*8+8,(*it2).first*8+30+8),rtRectf(0,0,8,8));

					//floor_16->Blit(10+(*it).first*8,(*it2).first*8+30);
				}
				else if ((*it2).second == 3)
				{
					//we draw it at the position
					r_batch_work_floor->BlitEx(work_free_16,rtRectf(10+(*it).first*8,(*it2).first*8+30,10+(*it).first*8+8,(*it2).first*8+30+8),rtRectf(0,0,8,8));

					//work_free_16->Blit(10+(*it).first*8,(*it2).first*8+30);
				}
				else if ((*it2).second == 1)
				{
					//wall_16->Blit(10+(*it).first*8,(*it2).first*8+30);
					r_batch_walls->BlitEx(wall_16,rtRectf(10+(*it).first*8,(*it2).first*8+30,10+(*it).first*8+8,(*it2).first*8+30+8),rtRectf(0,0,8,8));

				}
			}
		}
	}

	

	bg_editor_->draw();
	/*for(std::map<int, std::map<int, tile_map> >::iterator it = block_list_.begin(); it != block_list_.end(); it++) 
	{
		for(std::map<int, tile_map>::iterator it2 =(*it).second.begin(); it2 != (*it).second.end(); it2++) 
		{
			if ((*it2).second.value_ == 2)
			{
				blocked_tile_->Blit(200+(*it).first*16,(*it2).first*16+10);
			}
		}
	}*/

	for (int i = 0; i < module_list_.size(); i++)
	{
		for(std::map<int, std::map<int, int> >::iterator it = module_list_[i]->part_data_.begin(); it != module_list_[i]->part_data_.end(); it++) 
		{
			for(std::map<int, int>::iterator it2 =(*it).second.begin(); it2 != (*it).second.end(); it2++) 
			{

				if ((*it2).second == 0 || (*it2).second == 3)
				{
					//we draw it at the position
					if (selected_object_ == module_list_[i])
					{
						//floor_16_selected->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
						r_batch_floor->BlitEx(floor_16_selected,rtRectf(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,200+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));

					}
					else
					{
						if ((*it2).second == 3)
						{
							//work_free_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
							r_batch_work_floor->BlitEx(work_free_16,rtRectf(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,200+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));

						}
						else
						{
							//floor_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
							r_batch_floor->BlitEx(floor_16,rtRectf(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,200+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));

						}
					}
				}
				else if ((*it2).second == 1)
				{
					r_batch_walls->BlitEx(wall_16,rtRectf(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,200+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));
					//wall_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
				}
				else if ((*it2).second == -1 && g_game->state_ != CREATE_SHIP)
				{
					//we only render this outside the editor
					r_batch_wall_lock_air->BlitEx(wall_16,rtRectf(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16,200+(*it).first*8+module_list_[i]->start_x_*16+8,(*it2).first*8+10+module_list_[i]->start_y_*16+8),rtRectf(0,0,8,8));
					//wall_16->Blit(200+(*it).first*8+module_list_[i]->start_x_*16,(*it2).first*8+10+module_list_[i]->start_y_*16);
				}
			}
		}
	}

	r_batch_wall_lock_air->Flush();
	r_batch_walls->Flush();
	r_batch_work_floor->Flush();
	r_batch_floor->Flush();
	GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(5,5, "Adding:",1.0f,MAKE_RGBA(255,255,255,255));

	add_btn_->draw();
	save_btn_->draw();
	if (adding_object_ != 0)
	{
		rotate_btn_->draw();
	}
	if (adding_object_ != 0 || selected_object_ != 0)
	{
		cancel_btn_->draw();
	}
	if (selected_object_ != 0)
	{
		del_btn_->draw();
	}
}

void spacecraft::update()
{
	if (g_game->state_ == BATTLE_SHIP)
	{
		//for each module we search for people repearing them
		for (int i = 0; i < module_list_.size(); i++)
		{
			if (module_list_[i]->repearing_people_ > 0)
			{
				//we add hp as the number of repearing people
				//TODO: fix this, it dosen't check if the unit is already stepping on the position
				module_list_[i]->hitpoints_ += module_list_[i]->repearing_people_ * 0.05;
				if (module_list_[i]->hitpoints_ > 100)
				{
					module_list_[i]->hitpoints_ = 100;
					//we remove all the workers because we have 100 hp now :)
					for (int z = 0; z < module_list_[i]->repair_spots_.size(); z++)
					{
						if (module_list_[i]->repair_spots_[z].worker_ == 0)
							continue;
						module_list_[i]->repair_spots_[z].worker_->working_ = false;
						module_list_[i]->repair_spots_[z].worker_->work_type_ = 0;
						module_list_[i]->repair_spots_[z].worker_ = 0;
						working_people_--;
						module_list_[i]->repearing_people_--;
					}
				}
			}
			if (module_list_[i]->working_people_ > 0)
			{
				//we add hp as the number of repearing people
				//TODO: fix this, it dosen't check if the unit is already stepping on the position
				module_list_[i]->work_points_ += module_list_[i]->working_people_ * 0.25;
				//here we shoot or do what we must
				float req_to_shoot_ =0;
				if (module_list_[i]->type_ == 4)
				{
					req_to_shoot_ = 50;
				}
				if (module_list_[i]->type_ == 5)
				{
					req_to_shoot_ = 220;
				}
				if (module_list_[i]->type_ == 6)
				{
					req_to_shoot_ = 320;
				}
				if (module_list_[i]->type_ == 7)
				{
					req_to_shoot_ = 420;
				}
				if (req_to_shoot_ < module_list_[i]->work_points_)
				{
					module_list_[i]->work_points_ = req_to_shoot_;
					//we can shoot now (if thre is a target of course)
					//we search on the laser list for our id
					int laser_id = 0;
					for (int z = 0; z < laser_list_.size(); z++)
					{
						if (laser_list_[z] ==module_list_[i])
						{
							laser_id = z;
							break;
						}
					}

					bool has_shoot_ = false;
					//now we search on the enemies to who are we shooting
					for (int z = 0; z < enemies_list_.size(); z++)
					{
						for (int c = 0; c < enemies_list_[z]->laser_ids_.size(); c++)
						{
							if (enemies_list_[z]->laser_ids_[c] == laser_id)
							{
								//we have finally found to who I'm shooting, so lets shoot!
								module_list_[i]->work_points_ = 0;
								if (module_list_[i]->type_ == 4)
								{
									enemies_list_[z]->hp_-= 20;
								}
								if (module_list_[i]->type_ == 5)
								{
									enemies_list_[z]->hp_-= 40;
								}
								if (module_list_[i]->type_ == 6)
								{
									enemies_list_[z]->hp_-= 60;
								}
								if (module_list_[i]->type_ == 7)
								{
									enemies_list_[z]->hp_-= 80;
								}
								has_shoot_ = true;
								break;
							}
						}
						if (has_shoot_ == true && enemies_list_[z]->hp_ <= 0)
						{
							enemies_list_.erase(enemies_list_.begin()+z);
							if (enemies_list_.size() == 0)
							{
								finished_levels_++;
								g_game->cash_+=5000;
								g_nodes->spaceships_[0].my_node->type = 1;
								g_game->state_ = SHIP_NODES;
								g_nodes->r_batch_nodes->clean();
								g_nodes->generate_node_batcher();
							}
							break;
						}
					}

				}
			}
		}
	}
	//we do all the battle stuff
}

std::map<int, std::map<int, int> > spacecraft::get_part(int _part)
{
	std::map<int, std::map<int, int> > part_data_;
	if (_part == 0)
	{
		current_size_x_ = 3*2;
		current_size_y_ = 5*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}

		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}

		part_data_[0][0] = 2;
		part_data_[1][0] = 2;
		part_data_[2][0] = 2;
		part_data_[3][0] = 2;
		part_data_[4][0] = 2;
		part_data_[5][0] = 2;

		part_data_[0][1] = 2;
		part_data_[1][1] = 1;
		part_data_[2][1] = 1;
		part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;

		part_data_[0][2] = 2;
		part_data_[1][2] = 1;

		part_data_[0][3] = 1;
		part_data_[1][3] = 1;

		part_data_[0][6] = 1;
		part_data_[1][6] = 1;

		part_data_[0][7] = 2;
		part_data_[1][7] = 1;

		part_data_[0][9] = 2;
		part_data_[1][9] = 2;
		part_data_[2][9] = 2;
		part_data_[3][9] = 2;
		part_data_[4][9] = 2;
		part_data_[5][9] = 2;

		part_data_[0][8] = 2;
		part_data_[1][8] = 1;
		part_data_[2][8] = 1;
		part_data_[3][8] = 1;
		part_data_[4][8] = 1;
		part_data_[5][8] = 1;
		part_data_[5][1] = 2;
		part_data_[5][2] = 2;
		part_data_[5][3] = 1;
		part_data_[4][3] = 1;
		part_data_[4][2] = 1;

		part_data_[5][8] = 2;
		part_data_[5][7] = 2;
		part_data_[5][6] = 1;
		part_data_[4][6] = 1;
		part_data_[4][7] = 1;



		//block air scape
		part_data_[6][3] = -1;
		part_data_[6][4] = -1;
		part_data_[6][5] = -1;
		part_data_[6][6] = -1;

		part_data_[-1][3] = -1;
		part_data_[-1][4] = -1;
		part_data_[-1][5] = -1;
		part_data_[-1][6] = -1;

		//we now block everyting

		//part_data_ = rotate_tiles(part_data_);
	}
	else if (_part == 1)
	{
		current_size_x_ = 4*2;
		current_size_y_ = 5*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}

		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}

		part_data_[0][0] = 2;
		part_data_[1][0] = 2;
		part_data_[2][0] = 2;
		part_data_[3][0] = 2;
		part_data_[4][0] = 2;
		part_data_[5][0] = 2;
		part_data_[6][0] = 2;
		part_data_[7][0] = 2;

		part_data_[0][1] = 2;
		part_data_[1][1] = 1;
		part_data_[2][1] = 1;
		part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;
		part_data_[6][1] = 1;
		part_data_[7][1] = 1;

		part_data_[0][2] = 2;
		part_data_[1][2] = 1;

		part_data_[0][3] = 1;
		part_data_[1][3] = 1;

		part_data_[0][6] = 1;
		part_data_[1][6] = 1;

		part_data_[0][7] = 2;
		part_data_[1][7] = 1;

		part_data_[0][9] = 2;
		part_data_[1][9] = 2;
		part_data_[2][9] = 2;
		part_data_[3][9] = 2;
		part_data_[4][9] = 2;
		part_data_[5][9] = 2;
		part_data_[6][9] = 2;

		part_data_[0][8] = 2;
		part_data_[1][8] = 1;
		part_data_[2][8] = 1;
		part_data_[3][8] = 1;
		part_data_[4][8] = 1;
		part_data_[5][8] = 1;
		part_data_[6][8] = 1;
		part_data_[7][8] = 2;
		part_data_[7][7] = 2;
		part_data_[6][7] = 1;
		part_data_[6][6] = 1;
		part_data_[7][6] = 1;
		part_data_[7][9] = 2;

		part_data_[7][1] = 2;
		part_data_[7][2] = 2;
		part_data_[7][3] = 1;
		part_data_[6][3] = 1;
		part_data_[6][2] = 1;

		//block air scape
		part_data_[8][3] = -1;
		part_data_[8][4] = -1;
		part_data_[8][5] = -1;
		part_data_[8][6] = -1;

		part_data_[-1][3] = -1;
		part_data_[-1][4] = -1;
		part_data_[-1][5] = -1;
		part_data_[-1][6] = -1;

		//we now block everyting

		//part_data_ = rotate_tiles(part_data_);
	}
	else if (_part == 2)
	{
		current_size_x_ = 5*2;
		current_size_y_ = 5*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}

		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}

		part_data_[0][0] = 2;
		part_data_[1][0] = 2;
		part_data_[2][0] = 2;
		part_data_[3][0] = 2;
		part_data_[4][0] = 2;
		part_data_[5][0] = 2;
		part_data_[6][0] = 2;
		part_data_[7][0] = 2;
		part_data_[8][0] = 2;
		part_data_[9][0] = 2;

		part_data_[0][1] = 2;
		part_data_[1][1] = 1;
		part_data_[2][1] = 1;
		part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;
		part_data_[6][1] = 1;
		part_data_[7][1] = 1;
		part_data_[8][1] = 1;
		part_data_[9][1] = 2;

		part_data_[0][2] = 2;
		part_data_[1][2] = 1;

		part_data_[9][2] = 2;
		part_data_[8][2] = 1;

		part_data_[0][3] = 1;
		part_data_[1][3] = 1;

		part_data_[9][3] = 1;
		part_data_[8][3] = 1;

		part_data_[0][6] = 1;
		part_data_[1][6] = 1;

		part_data_[9][6] = 1;
		part_data_[8][6] = 1;

		part_data_[0][7] = 2;
		part_data_[1][7] = 1;

		part_data_[9][7] = 2;
		part_data_[8][7] = 1;

		part_data_[9][8] = 2;
		part_data_[8][8] = 1;
		part_data_[7][8] = 1;
		part_data_[6][8] = 1;
		part_data_[5][8] = 1;
		part_data_[4][8] = 1;
		part_data_[3][8] = 1;
		part_data_[2][8] = 1;
		part_data_[1][8] = 1;
		part_data_[0][8] = 2;
		part_data_[0][9] = 2;
		part_data_[0][9] = 2;
		part_data_[0][9] = 2;
		part_data_[0][9] = 2;
		part_data_[1][9] = 2;
		part_data_[2][9] = 2;
		part_data_[3][9] = 2;
		part_data_[4][9] = 2;
		part_data_[5][9] = 2;
		part_data_[6][9] = 2;
		part_data_[7][9] = 2;
		part_data_[8][9] = 2;
		part_data_[9][9] = 2;

		//work areas
		/*part_data_[4][4] = 3;
		part_data_[4][5] = 3;
		part_data_[5][5] = 3;
		part_data_[5][4] = 3;
		part_data_[4][2] = 3;
		part_data_[4][3] = 3;
		part_data_[5][3] = 3;
		part_data_[5][2] = 3;
		part_data_[4][6] = 3;
		part_data_[4][7] = 3;
		part_data_[5][7] = 3;
		part_data_[5][6] = 3;*/

		//block air scape
		part_data_[10][3] = -1;
		part_data_[10][4] = -1;
		part_data_[10][5] = -1;
		part_data_[10][6] = -1;

		part_data_[-1][3] = -1;
		part_data_[-1][4] = -1;
		part_data_[-1][5] = -1;
		part_data_[-1][6] = -1;

		//we now block everyting

		//part_data_ = rotate_tiles(part_data_);
	}
	else if (_part == 3)
	{
		current_size_x_ = 6*2;
		current_size_y_ = 5*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}

		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}

		part_data_[0][0] = 2;
		part_data_[1][0] = 2;
		part_data_[2][0] = 2;
		part_data_[3][0] = 2;
		part_data_[4][0] = 2;
		part_data_[5][0] = 2;
		part_data_[6][0] = 2;
		part_data_[7][0] = 2;
		part_data_[8][0] = 2;
		part_data_[9][0] = 2;

		part_data_[0][1] = 2;
		part_data_[1][1] = 1;
		part_data_[2][1] = 1;
		part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;
		part_data_[6][1] = 1;
		part_data_[7][1] = 1;
		part_data_[8][1] = 1;
		part_data_[9][1] = 2;

		part_data_[0][2] = 2;
		part_data_[1][2] = 1;

		part_data_[9][2] = 2;
		part_data_[8][2] = 1;

		part_data_[0][3] = 1;
		part_data_[1][3] = 1;

		part_data_[9][3] = 1;
		part_data_[8][3] = 1;

		part_data_[0][6] = 1;
		part_data_[1][6] = 1;

		part_data_[9][6] = 1;
		part_data_[8][6] = 1;

		part_data_[0][7] = 2;
		part_data_[1][7] = 1;

		part_data_[9][7] = 2;
		part_data_[8][7] = 1;

		part_data_[9][8] = 2;
		part_data_[8][8] = 1;
		part_data_[7][8] = 1;
		part_data_[6][8] = 1;
		part_data_[5][8] = 1;
		part_data_[4][8] = 1;
		part_data_[3][8] = 1;
		part_data_[2][8] = 1;
		part_data_[1][8] = 1;
		part_data_[0][8] = 2;
		part_data_[0][9] = 2;
		part_data_[0][9] = 2;
		part_data_[0][9] = 2;
		part_data_[0][9] = 2;
		part_data_[1][9] = 2;
		part_data_[2][9] = 2;
		part_data_[3][9] = 2;
		part_data_[4][9] = 2;
		part_data_[5][9] = 2;
		part_data_[6][9] = 2;
		part_data_[7][9] = 2;
		part_data_[8][9] = 2;
		part_data_[9][9] = 2;

		part_data_[11][0] = 2;
		part_data_[11][1] = 2;
		part_data_[11][2] = 2;
		part_data_[10][0] = 2;
		part_data_[10][1] = 1;
		part_data_[9][1] = 1;
		part_data_[9][2] = 0;
		part_data_[9][3] = 0;
		part_data_[8][2] = 0;
		part_data_[8][3] = 0;
		part_data_[10][2] = 1;
		part_data_[10][3] = 1;
		part_data_[11][3] = 1;
		part_data_[11][6] = 1;
		part_data_[10][6] = 1;
		part_data_[10][7] = 1;
		part_data_[10][8] = 1;
		part_data_[9][8] = 1;
		part_data_[9][8] = 1;

		part_data_[9][7] = 0;
		part_data_[8][7] = 0;
		part_data_[8][6] = 0;
		part_data_[9][6] = 0;

		part_data_[11][7] = 2;
		part_data_[11][8] = 2;
		part_data_[11][9] = 2;
		part_data_[10][9] = 2;

		//work areas
		/*part_data_[4][4] = 3;
		part_data_[4][5] = 3;
		part_data_[5][5] = 3;
		part_data_[5][4] = 3;
		part_data_[4][2] = 3;
		part_data_[4][3] = 3;
		part_data_[5][3] = 3;
		part_data_[5][2] = 3;
		part_data_[4][6] = 3;
		part_data_[4][7] = 3;
		part_data_[5][7] = 3;
		part_data_[5][6] = 3;*/

		//block air scape
		part_data_[12][3] = -1;
		part_data_[12][4] = -1;
		part_data_[12][5] = -1;
		part_data_[12][6] = -1;

		part_data_[-1][3] = -1;
		part_data_[-1][4] = -1;
		part_data_[-1][5] = -1;
		part_data_[-1][6] = -1;

		//we now block everyting

		//part_data_ = rotate_tiles(part_data_);
	}
	else if (_part == 4)
	{
		current_size_x_ = 4*2;
		current_size_y_ = 3*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}
		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}
		part_data_[0][0] = 2;
		part_data_[1][0] = 2;
		part_data_[2][0] = 2;
		part_data_[3][0] = 2;
		part_data_[4][0] = 2;
		part_data_[5][0] = 2;
		part_data_[6][0] = 2;
		part_data_[7][0] = 2;

		part_data_[0][1] = 1;
		part_data_[1][1] = 1;
		part_data_[2][1] = 1;
		part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;
		part_data_[7][1] = 2;
		part_data_[7][2] = 2;
		part_data_[7][3] = 2;
		part_data_[6][1] = 1;
		part_data_[6][2] = 1;
		part_data_[6][3] = 1;

		part_data_[0][5] = 2;
		part_data_[1][5] = 2;
		part_data_[2][5] = 2;
		part_data_[3][5] = 2;
		part_data_[4][5] = 2;
		part_data_[5][5] = 2;
		part_data_[6][5] = 2;
		part_data_[7][5] = 2;
		part_data_[0][4] = 1;
		part_data_[1][4] = 1;
		part_data_[2][4] = 1;
		part_data_[3][4] = 1;
		part_data_[4][4] = 1;
		part_data_[5][4] = 1;
		part_data_[6][4] = 1;
		part_data_[7][4] = 2;

		//work areas
		part_data_[2][2] = 3;
		part_data_[3][2] = 3;
		part_data_[2][3] = 3;
		part_data_[3][3] = 3;
		part_data_[4][2] = 3;
		part_data_[5][2] = 3;
		part_data_[4][3] = 3;
		part_data_[5][3] = 3;

		//block air scape
		part_data_[-1][1] = -1;
		part_data_[-1][2] = -1;
		part_data_[-1][3] = -1;
		part_data_[-1][4] = -1;

	}
	else if (_part == 5)
	{
		current_size_x_ = 5*2;
		current_size_y_ = 3*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}
		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}
		part_data_[0][0] = 2;
		part_data_[1][0] = 2;
		part_data_[2][0] = 2;
		part_data_[3][0] = 2;
		part_data_[4][0] = 2;
		part_data_[5][0] = 2;
		part_data_[6][0] = 2;
		part_data_[7][0] = 2;
		part_data_[8][0] = 2;
		part_data_[9][0] = 2;

		part_data_[0][1] = 1;
		part_data_[1][1] = 1;
		part_data_[2][1] = 1;
		part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;
		part_data_[7][1] = 2;
		part_data_[7][2] = 2;
		part_data_[7][3] = 2;
		part_data_[6][1] = 1;
		part_data_[6][2] = 1;
		part_data_[6][3] = 1;

		part_data_[0][5] = 2;
		part_data_[1][5] = 2;
		part_data_[2][5] = 2;
		part_data_[3][5] = 2;
		part_data_[4][5] = 2;
		part_data_[5][5] = 2;
		part_data_[6][5] = 2;
		part_data_[7][5] = 2;
		part_data_[8][5] = 2;
		part_data_[9][5] = 2;
		part_data_[9][1] = 2;
		part_data_[9][2] = 2;
		part_data_[9][3] = 2;
		part_data_[9][4] = 2;

		part_data_[7][1] = 1;
		part_data_[8][1] = 1;
		part_data_[8][2] = 1;
		part_data_[8][3] = 1;
		part_data_[8][4] = 1;
		part_data_[7][4] = 1;

		part_data_[0][4] = 1;
		part_data_[1][4] = 1;
		part_data_[2][4] = 1;
		part_data_[3][4] = 1;
		part_data_[4][4] = 1;
		part_data_[5][4] = 1;
		part_data_[6][4] = 1;
		//part_data_[7][4] = 2;

		//work areas
		part_data_[2][2] = 3;
		part_data_[3][2] = 3;
		part_data_[2][3] = 3;
		part_data_[3][3] = 3;
		part_data_[4][2] = 3;
		part_data_[5][2] = 3;
		part_data_[4][3] = 3;
		part_data_[5][3] = 3;
		part_data_[6][2] = 3;
		part_data_[7][2] = 3;
		part_data_[6][3] = 3;
		part_data_[7][3] = 3;

		//block air scape
		part_data_[-1][1] = -1;
		part_data_[-1][2] = -1;
		part_data_[-1][3] = -1;
		part_data_[-1][4] = -1;

	}
	else if (_part == 8)
	{
		current_size_x_ = 3*2;
		current_size_y_ = 3*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}
		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}
		part_data_[0][0] = 2;
		part_data_[1][0] = 2;
		part_data_[2][0] = 2;
		part_data_[3][0] = 2;
		part_data_[4][0] = 2;
		part_data_[5][0] = 2;

		part_data_[0][1] = 1;
		part_data_[1][1] = 1;
		part_data_[2][1] = 1;
		part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;

		part_data_[0][0] = 2;
		part_data_[1][0] = 2;
		part_data_[2][0] = 2;
		part_data_[3][0] = 2;
		part_data_[4][0] = 2;
		part_data_[5][0] = 2;

		part_data_[0][4] = 1;
		part_data_[1][4] = 1;
		part_data_[2][4] = 1;
		part_data_[3][4] = 1;
		part_data_[4][4] = 1;
		part_data_[5][4] = 1;

		part_data_[0][5] = 2;
		part_data_[1][5] = 2;
		part_data_[2][5] = 2;
		part_data_[3][5] = 2;
		part_data_[4][5] = 2;
		part_data_[5][5] = 2;

		//block air scape
		part_data_[-1][1] = -1;
		part_data_[-1][2] = -1;
		part_data_[-1][3] = -1;
		part_data_[-1][4] = -1;

		part_data_[6][1] = -1;
		part_data_[6][2] = -1;
		part_data_[6][3] = -1;
		part_data_[6][4] = -1;
	}
	else if (_part == 9)
	{
		current_size_x_ = 3*2;
		current_size_y_ = 3*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}
		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}
		part_data_[0][0] = 2;
		part_data_[1][0] = 2;
		part_data_[2][0] = 2;
		part_data_[3][0] = 2;
		part_data_[4][0] = 2;
		part_data_[5][0] = 2;

		part_data_[0][1] = 2;
		part_data_[0][2] = 2;
		part_data_[0][3] = 2;
		part_data_[0][4] = 2;
		part_data_[0][5] = 2;

		//part_data_[1][1] = 1;
		part_data_[1][2] = 1;
		part_data_[1][3] = 1;
		part_data_[1][4] = 1;
		part_data_[1][5] = 1;

		part_data_[1][1] = 1;
		part_data_[2][1] = 1;
		part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;
		
		part_data_[4][4] = 1;
		part_data_[4][5] = 1;
		part_data_[5][4] = 1;
		part_data_[5][5] = 2;

		//block air scape
		part_data_[1][6] = -1;
		part_data_[2][6] = -1;
		part_data_[3][6] = -1;
		part_data_[4][6] = -1;

		part_data_[6][1] = -1;
		part_data_[6][2] = -1;
		part_data_[6][3] = -1;
		part_data_[6][4] = -1;

	}
	else if (_part == 10)
	{
		current_size_x_ = 3*2;
		current_size_y_ = 3*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}
		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}
		part_data_[0][0] = 2;
		part_data_[5][0] = 2;
		part_data_[1][0] = 1;
		//part_data_[2][0] = 2;
		//part_data_[3][0] = 2;
		part_data_[4][4] = 2;
		part_data_[5][4] = 2;

		part_data_[0][1] = 2;
		part_data_[0][2] = 2;
		part_data_[0][3] = 2;
		part_data_[0][4] = 2;
		part_data_[0][5] = 2;

		part_data_[4][0] = 1;
		part_data_[1][2] = 1;
		part_data_[1][3] = 1;
		part_data_[1][4] = 1;
		part_data_[1][5] = 1;

		part_data_[1][1] = 1;
		//part_data_[2][1] = 0;
		//part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;
		
		part_data_[4][4] = 1;
		part_data_[4][5] = 1;
		part_data_[5][4] = 1;
		part_data_[5][5] = 2;

		//block air scape
		part_data_[1][6] = -1;
		part_data_[2][6] = -1;
		part_data_[3][6] = -1;
		part_data_[4][6] = -1;

		part_data_[6][1] = -1;
		part_data_[6][2] = -1;
		part_data_[6][3] = -1;
		part_data_[6][4] = -1;

		part_data_[1][-1] = -1;
		part_data_[2][-1] = -1;
		part_data_[3][-1] = -1;
		part_data_[4][-1] = -1;
	}
else if (_part == 11)
	{
		current_size_x_ = 3*2;
		current_size_y_ = 3*2;
		for (int i = -1; i < current_size_x_+1; i++)
		{
			for (int z = -1; z < current_size_y_+1; z++)
			{
				part_data_[i][z] = 2; //empty
			}
		}
		for (int i = 0; i < current_size_x_; i++)
		{
			for (int z = 0; z < current_size_y_; z++)
			{
				part_data_[i][z] = 0; //walkable
			}
		}
		part_data_[0][0] = 2;
		part_data_[5][0] = 2;
		part_data_[1][0] = 1;
		//part_data_[2][0] = 2;
		//part_data_[3][0] = 2;
		part_data_[4][4] = 2;
		part_data_[5][4] = 2;

		part_data_[0][1] = 1;
		part_data_[0][4] = 1;
		part_data_[0][5] = 2;

		part_data_[4][0] = 1;
		part_data_[1][2] = 1;
		part_data_[1][3] = 1;
		part_data_[1][4] = 1;
		part_data_[1][5] = 1;

		part_data_[1][1] = 1;
		//part_data_[2][1] = 0;
		//part_data_[3][1] = 1;
		part_data_[4][1] = 1;
		part_data_[5][1] = 1;
		
		part_data_[4][4] = 1;
		part_data_[4][5] = 1;
		part_data_[5][4] = 1;
		part_data_[5][5] = 2;

		part_data_[0][2] = 0;
		part_data_[0][3] = 0;
		part_data_[1][2] = 0;
		part_data_[1][3] = 0;

		//block air scape
		part_data_[1][6] = -1;
		part_data_[2][6] = -1;
		part_data_[3][6] = -1;
		part_data_[4][6] = -1;

		part_data_[6][1] = -1;
		part_data_[6][2] = -1;
		part_data_[6][3] = -1;
		part_data_[6][4] = -1;

		part_data_[1][-1] = -1;
		part_data_[2][-1] = -1;
		part_data_[3][-1] = -1;
		part_data_[4][-1] = -1;

		part_data_[-1][1] = -1;
		part_data_[-1][2] = -1;
		part_data_[-1][3] = -1;
		part_data_[-1][4] = -1;

	}
	rotation_ = 0;
	return part_data_;
}