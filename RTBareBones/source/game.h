#ifndef GAME_H
#define GAME_H

#include "PlatformPrecomp.h"
#include "BaseApp.h"
#include "Entity/EntityUtils.h"
#include "resource_file.h"
#include "sqrat/sqrat.h"
#include <bitset>

enum game_states{SPLASH_SCREEN, MAIN_MENU, CREATE_SHIP, MARKET, SELECT_MODULE,JOBS,SHIP_NODES,BATTLE_SHIP, GAME_OVER};

class game
{
public:
	game();
	void init();

	Entity *entity_;

	void update();
	void draw();

	//Fade callbacks
	void finish_splash_cb(VariantList *pVList);

	//Click callbacks
	void menu_click_build(VariantList *pVList);
	void menu_click_market(VariantList *pVList);
	void menu_click_jobs(VariantList *pVList);
	void menu_click_travel(VariantList *pVList);
	void menu_click_market_back(VariantList *pVList);
	void market_click_1(VariantList *pVList);
	void market_click_2(VariantList *pVList);
	void market_click_3(VariantList *pVList);
	void market_click_4(VariantList *pVList);
	void market_click_5(VariantList *pVList);
	void market_click_6(VariantList *pVList);
	void market_click_7(VariantList *pVList);
	void market_click_8(VariantList *pVList);
	void market_click_9(VariantList *pVList);
	void market_click_10(VariantList *pVList);
	void market_click_11(VariantList *pVList);
	void market_click_12(VariantList *pVList);
	void market_click_13(VariantList *pVList);
	void market_click_14(VariantList *pVList);
	void market_click_15(VariantList *pVList);
	void market_click_16(VariantList *pVList);
	void click_hire(VariantList *pVList);

	void buy_market(int num);

	//enable / disables
	void enable_main_menu(bool new_state);

	image_object_ *menu_click_build_;
	image_object_ *menu_click_market_;
	image_object_ *menu_click_jobs_;
	image_object_ *menu_click_travel_;
	image_object_ *market_bg_;
	image_object_ *hire_bg_;
	image_object_ *hire_click_;

	image_object_ *select_module_bg_;
	
	image_object_ *market_click_1_;
	image_object_ *market_click_2_;
	image_object_ *market_click_3_;
	image_object_ *market_click_4_;
	image_object_ *market_click_5_;
	image_object_ *market_click_6_;
	image_object_ *market_click_7_;
	image_object_ *market_click_8_;
	image_object_ *market_click_9_;
	image_object_ *market_click_10_;
	image_object_ *market_click_11_;
	image_object_ *market_click_12_;
	image_object_ *market_click_13_;
	image_object_ *market_click_14_;
	image_object_ *market_click_15_;
	image_object_ *market_click_16_;
	image_object_ *btn_back_;

	int number_of_employes_;

	bool already_clicked_;

	game_states state_;
	int tutorial_;//state of the tutorial

	int cash_;
};

extern game *g_game;

#endif // FADE_OUT_IN_H