#include "PlatformPrecomp.h"
#include "fade_out_in.h"

fade_out_in *g_fader;

fade_out_in::fade_out_in()
{

}

void fade_out_in::init()
{
	frames_ = 0;
	bg_1_ = new Surface("interface/fade_1.rttex");
	bg_2_ = new Surface("interface/fade_2.rttex");
	bg_3_ = new Surface("interface/fade_3.rttex");
	bg_4_ = new Surface("interface/fade_4.rttex");
	bg_5_ = new Surface("interface/fade_5.rttex");
	bg_6_ = new Surface("interface/fade_6.rttex");
	entity_ = GetEntityRoot()->AddEntity(new Entity("fade_out_in"));
	start = 100;
	fade_out_ = false;
	fade_in_ = false;
}


/*
_in_frames: number of frames before start fading out (30 frames = 1 second)
functor: Function to call when we finish the fadeout
*/
void fade_out_in::fade_out(int _in_frames, boost::function<void(VariantList *pVList)> functor)
{
	fade_out_ = true;
	start = _in_frames;
	frames_ = 0;
	entity_->GetFunction("fade_out_end")->sig_function.connect(functor);
}

void fade_out_in::fade_in(int _in_frames, boost::function<void(VariantList *pVList)> functor)
{
	fade_in_ = true;
	start = _in_frames;
	frames_ = 0;
	entity_->GetFunction("fade_in_end")->sig_function.connect(functor);

}

void fade_out_in::draw()
{
	int mod = 2;
	if (fade_out_ == true)
	{
		//fade in fade out
		if (frames_>=start+mod*0 && frames_<= start+mod*1)
		{
			bg_1_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*1 && frames_< start+mod*2)
		{
			bg_2_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*2 && frames_< start+mod*3)
		{
			bg_3_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*3 && frames_< start+mod*4)
		{
			bg_4_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*4 && frames_< start+mod*5)
		{
			bg_5_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*5 && frames_< start+mod*6)
		{
			bg_6_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if  (frames_ >= start+mod*6)
		{
			g_globalBatcher.Flush();
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//DrawRect(0,0,GetScreenSizeX(),GetScreenSizeY());
		}
		if (frames_>=start+mod*10)
		{
			fade_out_ = false;
			entity_->GetFunction("fade_out_end")->sig_function(0);
		}
	}
	if (fade_in_ == true)
	{

		if (frames_>=start+mod*0 && frames_<= start+mod*1)
		{
			bg_6_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*1 && frames_< start+mod*2)
		{
			bg_5_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*2 && frames_< start+mod*3)
		{
			bg_4_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*3 && frames_< start+mod*4)
		{
			bg_3_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*4 && frames_< start+mod*5)
		{
			bg_2_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_>= start+mod*5 && frames_< start+mod*6)
		{
			bg_1_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,32,32));
		}
		else if (frames_ >= start+mod*6)
		{
			fade_in_ = false;
			//if (entity_->GetFunction("fade_in_end")->sig_function.)
			//	entity_->GetFunction("fade_in_end")->sig_function(0);
		}
	}
}

void fade_out_in::update()
{
	frames_++;
}