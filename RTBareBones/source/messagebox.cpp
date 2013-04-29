#include "PlatformPrecomp.h"
#include "messagebox.h"
#include "game.h"

messagebox *g_messagebox;

messagebox::messagebox()
{

}

void messagebox::init()
{
	entity_ = GetEntityRoot()->AddEntity(new Entity("messagebox"));
	//entity_->GetFunction("next_char")->sig_function.connect(boost::bind(&typewriter::next_char, this, _1));
	msg_box_ = new image_object_("box_message.rttex",false);
	msg_box_->set_click_function("OnTouchEnd",boost::bind(&messagebox::click_box, this, _1));

	msg_box_->set_position(182,113);
	on_ = false;
	//set_message("titulo", "Esta es una super prueba de texto en donde vemos si podemos poner tdo el texto que queramos");
}

void messagebox::set_message(std::string title, std::string text)
{
	title_ = title;
	text_ = text;
	on_ = true;
	clicks_ = 0;
	tut_ = true;
	just_added_ = true;
}

void messagebox::set_message_no_tutorial(std::string title, std::string text)
{
	title_ = title;
	text_ = text;
	on_ = true;
	clicks_ = 0;
	tut_ = false;
	just_added_ = true;
}


void messagebox::clean()
{
	//current_text_ ="";
}

void messagebox::click_box(VariantList *pVList)
{
	if (just_added_ == true)
		return;
	if (on_ == false)
		return;

	clicks_++;
	if (clicks_ == 1)
	{
		on_ = false;
		clicks_ = 0;
		if (tut_ == true)
		{
			g_game->tutorial_++;
		}

	}
}

void messagebox::draw()
{
	if (on_ == false)
		return;
	/*if (current_text_.length() == 0)
		return;*/
	
	msg_box_->draw();

	rtRect wrapArea(193,124,713, 185);
	//DrawFilledRect(wrapArea,MAKE_RGBA(255,255,255,255));
	GetBaseApp()->GetFont(FONT_SMALL)->DrawWrapped(wrapArea, title_, true, false, MAKE_RGBA(255,255,255,255), 1.5f);
	rtRect wrapArea2(193,163,713, 389);
	GetBaseApp()->GetFont(FONT_SMALL)->DrawWrapped(wrapArea2, text_, true, false, MAKE_RGBA(255,255,255,255), 1.0f);

	
	//msg_box_->draw();
}

void messagebox::update()
{
	just_added_ = false;
}