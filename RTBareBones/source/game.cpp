#include "PlatformPrecomp.h"
#include "game.h"
#include "typewriter.h"
#include "fade_out_in.h"
#include "spacecraft.h"
#include "boost/lexical_cast.hpp"
#include "messagebox.h"

game *g_game;

game::game()
{

}

void game::init()
{
	entity_ = GetEntityRoot()->AddEntity(new Entity("game"));
	//entity_->GetFunction("next_char")->sig_function.connect(boost::bind(&typewriter::next_char, this, _1));
	state_ = SPLASH_SCREEN;
	g_fader->fade_out(120,boost::bind(&game::finish_splash_cb, this, _1));
	g_typewriter->type_text(rtRect (355,310,606,485), "By Zaxuhe", 80,1.0f,MAKE_RGBA(47,51,43,255));

	menu_click_build_ = new image_object_("menu_click.rttex",false);
	menu_click_jobs_ = new image_object_("menu_click.rttex",false);
	menu_click_market_ = new image_object_("menu_click.rttex",false);
	menu_click_travel_ = new image_object_("menu_click.rttex",false);

	menu_click_build_->set_position(213,334);
	menu_click_market_->set_position(232,108);
	menu_click_jobs_->set_position(587,116);
	menu_click_travel_->set_position(609,409);

	menu_click_build_->set_click_function("OnTouchEnd",boost::bind(&game::menu_click_build, this, _1));
	menu_click_market_->set_click_function("OnTouchEnd",boost::bind(&game::menu_click_market, this, _1));
	menu_click_jobs_->set_click_function("OnTouchEnd",boost::bind(&game::menu_click_jobs, this, _1));
	menu_click_travel_->set_click_function("OnTouchEnd",boost::bind(&game::menu_click_travel, this, _1));
	market_bg_ = new image_object_("market.rttex",false);
	hire_bg_ = new image_object_("contract.rttex",false);
	hire_click_ = new image_object_("hire_click.rttex",false);
	hire_click_->set_position(297,247);

	hire_click_->set_click_function("OnTouchEnd",boost::bind(&game::click_hire, this, _1));

	select_module_bg_ = new image_object_("module.rttex",false);

	btn_back_ = new image_object_("btn_back.rttex",false);
	btn_back_->set_position(5,5);
	btn_back_->set_click_function("OnTouchEnd",boost::bind(&game::menu_click_market_back, this, _1));


	market_click_1_ = new image_object_("market_click.rttex",false);
	market_click_2_ = new image_object_("market_click.rttex",false);
	market_click_3_ = new image_object_("market_click.rttex",false);
	market_click_4_ = new image_object_("market_click.rttex",false);
	market_click_5_ = new image_object_("market_click.rttex",false);
	market_click_6_ = new image_object_("market_click.rttex",false);
	market_click_7_ = new image_object_("market_click.rttex",false);
	market_click_8_ = new image_object_("market_click.rttex",false);
	market_click_9_ = new image_object_("market_click.rttex",false);
	market_click_10_ = new image_object_("market_click.rttex",false);
	market_click_11_ = new image_object_("market_click.rttex",false);
	market_click_12_ = new image_object_("market_click.rttex",false);
	market_click_13_ = new image_object_("market_click.rttex",false);
	market_click_14_ = new image_object_("market_click.rttex",false);
	market_click_15_ = new image_object_("market_click.rttex",false);
	market_click_16_ = new image_object_("market_click.rttex",false);

	market_click_1_->set_transparency(100);
	market_click_2_->set_transparency(100);
	market_click_3_->set_transparency(100);
	market_click_4_->set_transparency(100);
	market_click_5_->set_transparency(100);
	market_click_6_->set_transparency(100);
	market_click_7_->set_transparency(100);
	market_click_8_->set_transparency(100);
	market_click_9_->set_transparency(100);
	market_click_10_->set_transparency(100);
	market_click_11_->set_transparency(100);
	market_click_12_->set_transparency(100);
	market_click_13_->set_transparency(100);
	market_click_14_->set_transparency(100);
	market_click_15_->set_transparency(100);
	market_click_16_->set_transparency(100);

	market_click_1_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_1, this, _1));
	market_click_2_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_2, this, _1));
	market_click_3_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_3, this, _1));
	market_click_4_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_4, this, _1));
	market_click_5_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_5, this, _1));
	market_click_6_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_6, this, _1));
	market_click_7_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_7, this, _1));
	market_click_8_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_8, this, _1));
	market_click_9_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_9, this, _1));
	market_click_10_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_10, this, _1));
	market_click_11_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_11, this, _1));
	market_click_12_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_12, this, _1));
	market_click_13_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_13, this, _1));
	market_click_14_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_14, this, _1));
	market_click_15_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_15, this, _1));
	market_click_16_->set_click_function("OnTouchEnd",boost::bind(&game::market_click_16, this, _1));

	market_click_1_->set_position(17,75);
	market_click_2_->set_position(250,75);
	market_click_3_->set_position(486,75);
	market_click_4_->set_position(722,75);

	market_click_5_->set_position(17,216);
	market_click_6_->set_position(250,216);
	market_click_7_->set_position(486,216);
	market_click_8_->set_position(722,216);

	market_click_9_->set_position(17,355);
	market_click_10_->set_position(250,355);
	market_click_11_->set_position(486,355);
	market_click_12_->set_position(722,355);

	market_click_13_->set_position(17,497);
	market_click_14_->set_position(250,497);
	market_click_15_->set_position(486,497);
	market_click_16_->set_position(722,497);

	enable_main_menu(false); // we disable on the menu

	cash_ = 2000;
	number_of_employes_ = 0;

}

void game::enable_main_menu(bool new_state)
{
	menu_click_build_->set_enabled(new_state);
	menu_click_market_->set_enabled(new_state);
	menu_click_jobs_->set_enabled(new_state);
	menu_click_travel_->set_enabled(new_state);
}

void game::draw()
{
	if (state_ == SELECT_MODULE)
	{
		select_module_bg_->draw();
		btn_back_->draw();

		if (g_spacecraft->avaible_ship_modules_[0] == 0)
			market_click_1_->draw();
		if (g_spacecraft->avaible_ship_modules_[1] == 0)
			market_click_2_->draw();
		if (g_spacecraft->avaible_ship_modules_[2] == 0)
			market_click_3_->draw();
		if (g_spacecraft->avaible_ship_modules_[3] == 0)
			market_click_4_->draw();
		if (g_spacecraft->avaible_ship_modules_[4] == 0)
			market_click_5_->draw();
		if (g_spacecraft->avaible_ship_modules_[5] == 0)
			market_click_6_->draw();
		if (g_spacecraft->avaible_ship_modules_[6] == 0)
			market_click_7_->draw();
		if (g_spacecraft->avaible_ship_modules_[7] == 0)
			market_click_8_->draw();
		if (g_spacecraft->avaible_ship_modules_[8] == 0)
			market_click_9_->draw();
		if (g_spacecraft->avaible_ship_modules_[9] == 0)
			market_click_10_->draw();
		if (g_spacecraft->avaible_ship_modules_[10] == 0)
			market_click_11_->draw();
		if (g_spacecraft->avaible_ship_modules_[11] == 0)
			market_click_12_->draw();
		if (g_spacecraft->avaible_ship_modules_[12] == 0)
			market_click_13_->draw();
		if (g_spacecraft->avaible_ship_modules_[13] == 0)
			market_click_14_->draw();
		if (g_spacecraft->avaible_ship_modules_[14] == 0)
			market_click_15_->draw();
		if (g_spacecraft->avaible_ship_modules_[15] == 0)
			market_click_16_->draw();

		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(166,100, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[0]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(406,100, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[1]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(630,100, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[2]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(870,100, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[3]),1.0f,MAKE_RGBA(255,255,255,255));

		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(166,240, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[4]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(406,240, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[5]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(630,240, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[6]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(870,240, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[7]),1.0f,MAKE_RGBA(255,255,255,255));

		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(166,240+140, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[8]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(406,240+140, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[9]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(630,240+140, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[10]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(870,240+140, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[11]),1.0f,MAKE_RGBA(255,255,255,255));

		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(166,240+140*2, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[12]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(406,240+140*2, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[13]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(630,240+140*2, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[14]),1.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(870,240+140*2, ""+boost::lexical_cast<std::string>(g_spacecraft->avaible_ship_modules_[15]),1.0f,MAKE_RGBA(255,255,255,255));
	}
	else if (state_ == MARKET)
	{
		market_bg_->draw();
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(620,20, "Money $"+boost::lexical_cast<std::string>(cash_),1.0f,MAKE_RGBA(255,255,255,255));

		if (cash_ < 900)
			market_click_1_->draw();
		if (cash_ < 1950)
			market_click_2_->draw();
		if (cash_ < 2900)
		market_click_3_->draw();
		if (cash_ < 3800)
		market_click_4_->draw();
		if (cash_ < 900)
			market_click_5_->draw();
		if (cash_ < 2200)
			market_click_6_->draw();
		//if (cash_ < 3300)
			market_click_7_->draw();
		//if (cash_ < 4400)
			market_click_8_->draw();
		if (cash_ < 300)
			market_click_9_->draw();
		if (cash_ < 300)
			market_click_10_->draw();
		if (cash_ < 300)
			market_click_11_->draw();
		if (cash_ < 300)
			market_click_12_->draw();
		market_click_13_->draw();
		market_click_14_->draw();
		market_click_15_->draw();
		market_click_16_->draw();

		btn_back_->draw();
	}
	else if(state_ == JOBS)
	{
		hire_bg_->draw();
		btn_back_->draw();
		//calculate the cash needed to hire someone

		int total = 50*((number_of_employes_+1)*(number_of_employes_+1))-150*(number_of_employes_+1)+200;
		//total*=10;
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(227,200, "People already working for you: "+boost::lexical_cast<std::string>(number_of_employes_),1.0f,MAKE_RGBA(255,255,255,255));

		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(427,405, "$"+boost::lexical_cast<std::string>(total),2.0f,MAKE_RGBA(255,255,255,255));
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(620,20, "Money $"+boost::lexical_cast<std::string>(cash_),1.0f,MAKE_RGBA(255,255,255,255));
		//hire_click_->draw();


	}
	else
	{
		//menu_click_build_->draw();
		//menu_click_market_->draw();
		//menu_click_jobs_->draw();
		//menu_click_travel_->draw();
		GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(5,35, "Money $"+boost::lexical_cast<std::string>(cash_),1.0f,MAKE_RGBA(255,255,255,255));
	}
}

void game::buy_market(int num)
{
	if (tutorial_ <= 2)
	{
		if (num != 5 && num != 1)
			return;
	}
	switch (num)
	{
	case 1:
		if (tutorial_ <= 2 && g_spacecraft->avaible_ship_modules_[num-1] == 1)
		{
			return;
		}
		if (cash_ >= 900)
		{
			cash_ -= 900;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 2:
		if (cash_ >= 1950)
		{
			cash_ -= 1950;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 3:
		if (cash_ >= 2900)
		{
			cash_ -= 2900;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 4:
		if (cash_ >= 3800)
		{
			cash_ -= 3800;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 5:
		if (tutorial_ == 2 && g_spacecraft->avaible_ship_modules_[num-1] == 1)
		{
			return;
		}
		if (cash_ >= 900)
		{
			cash_ -= 900;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 6:
		if (cash_ >= 2200)
		{
			cash_ -= 2200;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 7:
		return;
		if (cash_ >= 3300)
		{
			cash_ -= 3300;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 8:
		return;
		if (cash_ >= 4400)
		{
			cash_ -= 4400;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 9:
		if (cash_ >= 300)
		{
			cash_ -= 300;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 10:
		if (cash_ >= 300)
		{
			cash_ -= 300;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 11:
		if (cash_ >= 300)
		{
			cash_ -= 300;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;
	case 12:
		if (cash_ >= 300)
		{
			cash_ -= 300;
			g_spacecraft->avaible_ship_modules_[num-1]++;
		}
		break;

	}
}

void game::click_hire(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == JOBS)
	{
		g_game->already_clicked_ = true;
		int total = 50*((number_of_employes_+1)*(number_of_employes_+1))-150*(number_of_employes_+1)+200;
		//total*=10;
		if (cash_ >= total)
		{
			//entonces lo podemos comprar
			number_of_employes_++;
			cash_ -= total;
		}
	}
}

void game::market_click_1(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(1);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[0] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(0);
		g_spacecraft->adding_object_->type_ = 0;
		state_ = CREATE_SHIP;
	}
}


void game::market_click_2(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(2);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[1] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(1);
		g_spacecraft->adding_object_->type_ = 1;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_3(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(3);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[2] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(2);
		g_spacecraft->adding_object_->type_ = 2;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_4(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(4);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[3] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(3);
		g_spacecraft->adding_object_->type_ = 3;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_5(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(5);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[4] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(4);
		g_spacecraft->adding_object_->type_ = 4;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_6(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(6);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[5] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(5);
		g_spacecraft->adding_object_->type_ = 5;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_7(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(7);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[6] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(6);
		g_spacecraft->adding_object_->type_ = 6;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_8(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(8);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[7] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(7);
		g_spacecraft->adding_object_->type_ = 7;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_9(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(9);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[8] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(8);
		g_spacecraft->adding_object_->type_ = 8;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_10(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(10);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[9] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(9);
		g_spacecraft->adding_object_->type_ = 9;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_11(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(11);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[10] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(10);
		g_spacecraft->adding_object_->type_ = 10;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_12(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(12);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[11] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(11);
		g_spacecraft->adding_object_->type_ = 11;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_13(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(13);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[12] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(12);
		g_spacecraft->adding_object_->type_ = 12;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_14(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(14);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[13] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(13);
		g_spacecraft->adding_object_->type_ = 13;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_15(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(15);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[14] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(14);
		g_spacecraft->adding_object_->type_ = 14;
		state_ = CREATE_SHIP;
	}
}

void game::market_click_16(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		buy_market(16);
	}
	if (state_ == SELECT_MODULE)
	{
		if (g_spacecraft->avaible_ship_modules_[15] == 0)
			return;
		g_game->already_clicked_ = true;
		g_spacecraft->adding_object_ = new ship_module();
		g_spacecraft->adding_object_->part_data_ = g_spacecraft->get_part(15);
		g_spacecraft->adding_object_->type_ = 15;
		state_ = CREATE_SHIP;
	}
}
void game::menu_click_market_back(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	if (state_ == MARKET)
	{
		g_game->already_clicked_ = true;
		state_ = MAIN_MENU;
		enable_main_menu(true);
		if (tutorial_ == 2)
		{
			g_messagebox->set_message_no_tutorial("Let's continue","Now its time to create your ship, go to the ship hangar to do so");
		}
	}
	if (state_ == JOBS)
	{
		g_game->already_clicked_ = true;
		state_ = MAIN_MENU;
		enable_main_menu(true);
		if (tutorial_ == 4)
		{
			g_messagebox->set_message_no_tutorial("Let's continue","We can go an explore the universe, click on explore to do so (To finish the game you have to clear the universe from enemies)");
		}
	}
	else if (state_ == SELECT_MODULE)
	{
		g_game->already_clicked_ = true;
		state_ = CREATE_SHIP;
		//enable_main_menu(true);
	}
}
void game::menu_click_market(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	g_game->already_clicked_ = true;
	enable_main_menu(false);
	state_ = MARKET;
	if (tutorial_ == 1)
	{
		g_messagebox->set_message("Buy parts of your ship", "You must build your ship to explore the universe, please buy a SMALL REACTOR and a SMALL LASER, after that click on back");
	}
}

void game::menu_click_travel(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	g_game->already_clicked_ = true;
	bool _quit = true;
	if (g_spacecraft->module_list_.size() > 0)
		_quit = false;
	if (number_of_employes_ == 0)
		_quit = true; 
	if (_quit == true)
		return;

	enable_main_menu(false);
	state_ = SHIP_NODES;
	if (tutorial_ == 4)
	{
		g_messagebox->set_message_no_tutorial("Explore","Click on the red nodes to travel to those places, if you want to go back just click on the station");
	}

}

void game::menu_click_jobs(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	g_game->already_clicked_ = true;
	if (tutorial_ <= 2)
	{
		return;
	}
	enable_main_menu(false);
	state_ = JOBS;
	if (tutorial_ == 3)
	{
		g_messagebox->set_message("Hire some workers", "You'r ship needs some people to operate properly, we need to 2 people for now, hire them by clicking on HIRE, after that click on back");
	}
}

void game::menu_click_build(VariantList *pVList)
{
	if (g_game->already_clicked_ == true)
		return;
	g_game->already_clicked_ = true;
	bool _quit = true;
	if (g_spacecraft->module_list_.size() > 0)
		_quit = false;
	for (int i = 0; i < 16; i++)
	{
		if (g_spacecraft->avaible_ship_modules_[i] > 0)
			_quit = false;
	}

	if (_quit == true)
		return;
	//g_spacecraft->get_part();
	enable_main_menu(false);
	state_ = CREATE_SHIP;
	if (tutorial_ == 2)
	{
		g_messagebox->set_message("Create ship", "Create your ship by clicking on the buttons, add a part, rotate it and position it, your ship must be connected to work when you finish click OK, also you need to supply enough energy for the lasers\n\n(Note: its hard to add modules, because the point they are added, please be patient!");
	}
}

void game::update()
{

}

//FADER CALLBACKS

//This finish the splash screen and takes us to the menu
void game::finish_splash_cb(VariantList *pVList)
{
	state_ = MAIN_MENU;
	enable_main_menu(true);
	g_typewriter->clean();
	g_fader->fade_in(0,0);
	tutorial_ = 0; //we startup the tutorial
	g_messagebox->set_message("Welcome", "Please read the instrucctions on this screen and follow them, to start the game click on OK, after that go to the -MARKET-");
}