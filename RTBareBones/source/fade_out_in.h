#ifndef FADE_OUT_IN_H
#define FADE_OUT_IN_H

#include "PlatformPrecomp.h"
#include "BaseApp.h"
#include "Entity/EntityUtils.h"
#include "resource_file.h"
#include "sqrat/sqrat.h"
#include <bitset>

class fade_out_in
{
public:
	fade_out_in();
	void init();

	Surface *bg_1_;
	Surface *bg_2_;
	Surface *bg_3_;
	Surface *bg_4_;
	Surface *bg_5_;
	Surface *bg_6_;

	Entity *entity_;

	void update();
	void draw();
	void fade_out(int _in_frames, boost::function<void(VariantList *pVList)> functor);
	void fade_in(int _in_frames, boost::function<void(VariantList *pVList)> functor);
	int frames_;
	int start;
	bool fade_out_;
	bool fade_in_;
};

extern fade_out_in *g_fader;

#endif // FADE_OUT_IN_H