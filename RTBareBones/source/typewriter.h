#ifndef TYPEWRITER_H
#define TYPEWRITER_H

#include "PlatformPrecomp.h"
#include "BaseApp.h"
#include "Entity/EntityUtils.h"
#include "resource_file.h"
#include "sqrat/sqrat.h"
#include <bitset>


class typewriter
{
public:
	typewriter();
	void init();

	Entity *entity_;

	void update();
	void draw();
	void type_text(rtRect _type_area, std::string _text, int _time_ms, float _scale, unsigned int _color);
	void clean();
	rtRect type_area_;
	std::string text_; //final text to write
	std::string current_text_;
	int num_chars_;
	void next_char(VariantList *pVList);
	int time_ms_; //time to type each char
	float scale_;
	unsigned int color_;


};

extern typewriter *g_typewriter;

#endif // FADE_OUT_IN_H