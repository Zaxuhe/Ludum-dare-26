#include "PlatformPrecomp.h"
#include "typewriter.h"

typewriter *g_typewriter;

typewriter::typewriter()
{

}

void typewriter::init()
{
	entity_ = GetEntityRoot()->AddEntity(new Entity("typewriter"));
	entity_->GetFunction("next_char")->sig_function.connect(boost::bind(&typewriter::next_char, this, _1));
}

void typewriter::next_char(VariantList *pVList)
{
	if (text_.length() < num_chars_)
		return;
	current_text_ = text_.substr(0,num_chars_++);
	GetMessageManager()->CallEntityFunction(entity_, time_ms_, "next_char", 0);
}

void typewriter::type_text(rtRect _type_area, std::string _text, int _time_ms, float _scale, unsigned int _color)
{
	type_area_ = _type_area;
	text_ = _text;
	time_ms_ = _time_ms;
	num_chars_ = 0;
	current_text_ = "";
	GetMessageManager()->CallEntityFunction(entity_, time_ms_, "next_char", 0);
	scale_ = _scale;
	color_ = _color;
}

void typewriter::clean()
{
	current_text_ ="";
}
void typewriter::draw()
{
	if (current_text_.length() == 0)
		return;
	/*
	rtRect wrapArea(180,163,180+156, 163+30);
	DrawFilledRect(wrapArea,MAKE_RGBA(255,255,255,255));
	GetBaseApp()->GetFont(FONT_SMALL)->DrawWrapped(wrapArea, t.title_, true, false, MAKE_RGBA(0,0,0,255), 0.6f);
	*/
	rtRect test = type_area_;
	GetBaseApp()->GetFont(FONT_SMALL)->DrawWrapped(test, current_text_, true, true, color_, scale_);
}

void typewriter::update()
{

}