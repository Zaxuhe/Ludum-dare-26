#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include "PlatformPrecomp.h"
#include "BaseApp.h"
#include "Entity/EntityUtils.h"
#include "resource_file.h"
#include "sqrat/sqrat.h"
#include <bitset>


class messagebox
{
public:
	messagebox();
	void init();

	Entity *entity_;

	void update();
	void draw();
	void clean();
	image_object_ *msg_box_;
	bool on_;
	bool tut_;
	std::string title_;
	std::string text_;
	void set_message(std::string title, std::string text);
	void set_message_no_tutorial(std::string title, std::string text); //the same but not addnig tutorial points

	void click_box(VariantList *pVList);
	bool just_added_;

	int clicks_;


};

extern messagebox *g_messagebox;

#endif // FADE_OUT_IN_H