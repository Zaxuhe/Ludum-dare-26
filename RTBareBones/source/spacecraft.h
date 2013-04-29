#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "PlatformPrecomp.h"
#include "App.h"
#include "Entity/EntityUtils.h"
#include "resource_file.h"
#include "sqrat/sqrat.h"
#include <bitset>

//Tengo cada modulo con sus datos, y posion inicial y final
//Estos modulos los puedo rotar y borrar y posicionar
//Todos los modulos deben pder llegar a todos los modulos (podemos saber mediante a*?)
//Solo podemos rotar un modulo antes de ponerlo
//Si el modulo colisiona con otro modulo entonces quitarlo de escena y volver a empezar
//Agregar borde simple? a los modulos para distinguirlos



class unit
{
public:

	unit()
	{
		walk_dir_ = 0;
		current_x_ = 0;
		current_y_ = 0;
		update_exact_pos();
		working_ = false;
		work_type_ = 0;
	}
	unit(int _x, int _y)
	{
		walk_dir_ = 0;
		current_x_ = _x;
		current_y_ = _y;
		update_exact_pos();
		working_ = false;
		work_type_ = 0;
	}

	void update_exact_pos()
	{
		exact_x_ = 16*current_x_+8;
		exact_y_ = 16*current_y_+8;
	}

	void set_current_pos(int _x, int _y)
	{
		walk_dir_ = 0;
		current_x_ = _x;
		current_y_ = _y;
		update_exact_pos();
	}

	void draw_to_batch();

	void create_path(int _to_x, int _to_y);

	void where_to_walk()
	{
		int p = current_path_.size();
		if (p == 0)
		{
			walk_dir_ = 0;
			return;
		}
		if (current_x_ < current_path_[p-1].x)
		{
			walk_dir_ = 1;
		}
		else if (current_x_ > current_path_[p-1].x)
		{
			walk_dir_ = 2;
		}
		else if (current_y_ > current_path_[p-1].y)
		{
			walk_dir_ = 3;
		}
		else if (current_y_ < current_path_[p-1].y)
		{
			walk_dir_ = 4;
		}
		steps_ = 0;
	}
	void random_path();
	void walk()
	{
		if (walk_dir_ == 0 || steps_ == 16)
		{
			where_to_walk();
			if (walk_dir_ == 0)
			{
				//we have no path :( we will create one!
				if (working_ == false)
					random_path();
				return;
			}
		}
		if (walk_dir_ == 1)
		{
			//we move to the right
			exact_x_++;
		}
		if (walk_dir_ == 2)
		{
			//we move to the right
			exact_x_--;
		}
		if (walk_dir_ == 3)
		{
			exact_y_--;
			//we move to the right
		}
		if (walk_dir_ == 4)
		{
			//we move to the right
			exact_y_++;
		}
		steps_++;
		int p = current_path_.size();
		if (steps_ == 16) //we have reached another walk area
		{

			current_x_ = current_path_[p-1].x;
			current_y_ = current_path_[p-1].y;
			exact_x_ = current_x_ *16+8;
			exact_y_ = current_y_ *16+8;
			current_path_.erase(current_path_.begin()+p-1);
		}
	}

	int steps_;
	int walk_dir_;
	int exact_x_; //pixel
	int exact_y_; //pixel 
	int current_x_;//on the map
	int current_y_;//on the map
	int work_type_; //0 no trabaja, 1 repara, 2 trabaja
	bool working_; //if its working or not
	std::vector<CL_Vec2f> current_path_; // the path to follow

};

struct work_pos
{
	work_pos(int _x, int _y)
	{
		x_ = _x;
		y_ = _y;
		worker_ = 0;
	};
	int x_;
	int y_;
	unit *worker_;
};

class ship_module
{
public:
	std::map<int, std::map<int, int> > part_data_;
	int current_size_x_;
	int current_size_y_;
	int start_x_;
	int start_y_;
	int type_; //tipo de modulo
	int walk_x_; //a walkable tile from here
	int walk_y_; // a walkable tile from heres
	float hitpoints_; //100
	float work_points_; //the number of work points for next action
	int working_people_;//how much people is working here
	std::vector <work_pos> repair_spots_;
	std::vector <work_pos> work_spots_;
	int repearing_people_;
};

struct tile_map
{
	tile_map()
	{
		m = 0;
	};
	int value_;
	ship_module* m;
};

 
class enemy_ship
{
public:
	enemy_ship(int _type)
	{
		type_ = _type;
		if (type_ == 0)
			hp_ = 50;
		if (type_ == 1)
			hp_ = 100;
		if (type_ == 2)
			hp_ = 150;
		if (type_ == 3)
			hp_ = 200;
		attack_speed_ = 0;
		max_hp_ = hp_;

		if (type_ == 0)
		{
			attack_speed_ = 1000000;
		}
	}

	void update()
	{
		attack_speed_++;
	};

	unsigned int attack_speed_; //shoots per second :P
	float hp_;
	float max_hp_;
	int type_; //0 tutorial, 1 slow, 2 med, 3 fast
	std::vector<int> laser_ids_; //the lasers I'm being shooted by
};

class spacecraft
{
public:
	spacecraft();
	void attack_random_module(float damage);
	void init();

	Entity *entity_;

	void update();
	void draw();

	bool block_tiles(ship_module *m);

	std::map<int, std::map<int, int> > get_part(int _part);
	std::map<int, std::map<int, int> > rotate_tiles(std::map<int, std::map<int, int> > part_data_);

	std::map<int, std::map<int, tile_map> > block_list_; //list of blocked and unblocked tiles (32 x 32)

	void click_add(VariantList *pVList);
	void click_rotate(VariantList *pVList);
	void click_cancel(VariantList *pVList);
	void click_board(VariantList *pVList);
	void click_del(VariantList *pVList);
	void click_save(VariantList *pVList);
	void click_add_worker(VariantList *pVList);
	void click_remove_worker(VariantList *pVList);
	void click_add_repair(VariantList *pVList);
	void click_remove_repair(VariantList *pVList);

	void click_laser_1(VariantList *pVList);
	void click_laser_2(VariantList *pVList);
	void click_laser_3(VariantList *pVList);
	void click_laser_4(VariantList *pVList);
	void click_laser_5(VariantList *pVList);
	void click_laser_6(VariantList *pVList);
	void click_laser_7(VariantList *pVList);
	void click_laser_8(VariantList *pVList);
	void click_laser_9(VariantList *pVList);
	void click_laser_10(VariantList *pVList);

	void click_eship_1(VariantList *pVList);
	void click_eship_2(VariantList *pVList);
	void click_eship_3(VariantList *pVList);
	void click_eship_4(VariantList *pVList);
	void click_eship_5(VariantList *pVList);
	void click_eship_6(VariantList *pVList);
	void click_eship_7(VariantList *pVList);
	void click_eship_8(VariantList *pVList);
	void click_eship_9(VariantList *pVList);
	void click_eship_10(VariantList *pVList);


	int selected_laser_;


	Surface *floor_16;
	Surface *floor_16_selected;
	Surface *wall_16;
	Surface *work_free_16;
	image_object_ *add_btn_;
	image_object_ *rotate_btn_;
	image_object_ *cancel_btn_;
	image_object_ *save_btn_;
	image_object_ *del_btn_;
	image_object_ *bg_editor_;
	Surface *blocked_tile_;
	image_object_ *battle_right_;
	image_object_ *btn_add_repair_;
	image_object_ *btn_remove_repair_;

	image_object_ *btn_border_1_;
	image_object_ *btn_border_2_;
	image_object_ *btn_border_3_;
	image_object_ *btn_border_4_;
	image_object_ *btn_border_5_;
	image_object_ *btn_border_6_;
	image_object_ *btn_border_7_;
	image_object_ *btn_border_8_;
	image_object_ *btn_border_9_;
	image_object_ *btn_border_10_;

	image_object_ *btn_border_1_e_;
	image_object_ *btn_border_2_e_;
	image_object_ *btn_border_3_e_;
	image_object_ *btn_border_4_e_;
	image_object_ *btn_border_5_e_;
	image_object_ *btn_border_6_e_;
	image_object_ *btn_border_7_e_;
	image_object_ *btn_border_8_e_;
	image_object_ *btn_border_9_e_;
	image_object_ *btn_border_10_e_;


	RenderBatcher *r_batch_walls;
	RenderBatcher *r_batch_floor;
	RenderBatcher *r_batch_work_floor;
	RenderBatcher *r_batch_wall_lock_air;
	RenderBatcher *r_batch_chars1;
	RenderBatcher *r_batch_chars2;
	RenderBatcher *r_batch_chars3;
	RenderBatcher *r_batch_chars4;
	RenderBatcher *r_batch_tooltips; //actions the characters are 
	RenderBatcher *r_batch_fire;

	Surface *char_1_;
	Surface *char_2_;
	Surface *char_3_;
	Surface *char_4_;

	image_object_ *enemy_1_;
	image_object_ *enemy_2_;
	image_object_ *enemy_3_;


	Surface *fire_s_;
	Surface *fire_m_;
	Surface *fire_b_;


	Surface *icon_repair_;
	Surface *icon_work_;

	image_object_ *hp_bg_;
	image_object_ *btn_add_worker_;
	image_object_ *btn_remove_worker_;

	void draw_battle();
	void add_people_to_ship();

	std::map <int,int> avaible_ship_modules_;

	ship_module *adding_object_;
	ship_module *selected_object_;
	std::vector <ship_module*> module_list_;
	std::vector <ship_module*> laser_list_;
	std::vector <enemy_ship*> enemies_list_;
	int rotation_;

	//for pathfiding only
	std::map <int, std::map<int,Point> > tile_set_;
	Point* getPointFromCoord(int x, int y);
	Point* getPoint(int x, int y);
	vector<CL_Vec2f> aStar(int x1, int y1, int x2, int y2);
	bool is_valid_ship();
	std::vector<unit> unit_list_;

	std::vector <CL_Vec2f> points_free_; //points where I can walk!

	int working_people_; //people that is working

	int finished_levels_;
};

extern spacecraft *g_spacecraft;

#endif // FADE_OUT_IN_H