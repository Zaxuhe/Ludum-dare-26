#ifndef SCRIPT_ENVIROMENT_H
#define SCRIPT_ENVIROMENT_H

#include "PlatformPrecomp.h"
#include "BaseApp.h"
#include "Entity/EntityUtils.h"
#include "resource_file.h"
#include "sqrat/sqrat.h"
#include <bitset>

enum unit_event_type
{
	UNIT_EVENT_NONE,
	UNIT_EVENT_LOGIN,
	UNIT_EVENT_LOGOUT,
	UNIT_EVENT_CHANNEL_JOIN,
	UNIT_EVENT_CHANNEL_LEAVE,
	UNIT_EVENT_ADVANCE,
	UNIT_EVENT_LOOK,
	UNIT_EVENT_DIRECTION,
	UNIT_EVENT_OUTFIT,
	UNIT_EVENT_MAIL_SEND,
	UNIT_EVENT_MAIL_RECEIVE,
	UNIT_EVENT_TRADE_REQUEST,
	UNIT_EVENT_TRADE_ACCEPT,
	UNIT_EVENT_TEXTEDIT,
	UNIT_EVENT_REPORTBUG,
	UNIT_EVENT_THINK,
	UNIT_EVENT_STATSCHANGE,
	UNIT_EVENT_COMBAT_AREA,
	UNIT_EVENT_PUSH,
	UNIT_EVENT_TARGET,
	UNIT_EVENT_FOLLOW,
	UNIT_EVENT_COMBAT,
	UNIT_EVENT_ATTACK,
	UNIT_EVENT_CAST,
	UNIT_EVENT_KILL,
	UNIT_EVENT_DEATH,
	UNIT_EVENT_ENEMY_RESPAWN,
	UNIT_EVENT_PREPAREDEATH
};

struct unit_event
{
	unit_event()
	{
		script_= 0;
		func_ = 0;
	};
	unit_event_type event_id_;
	Sqrat::Script *script_;
	Sqrat::Function *func_;
};

/**
* we declare the console here
*/

class console_manager
{
public:
	console_manager()
	{

	};

	void init()
	{
		entity_ = GetEntityRoot()->AddEntity(new Entity("comsole_manager"));
		entity_->AddComponent(new FocusUpdateComponent());

		entity_->GetFunction(std::string("f_console_manager"))->sig_function.connect(boost::bind(&console_manager::next_message, this, _1));
	}

	void add_message(std::string msg)
	{
		return;
		if (messages_.size() == 0)
		{
			GetMessageManager()->CallEntityFunction(entity_, 1000, std::string("f_console_manager"), 0);
		}
		messages_.push_back(msg);
	}

	void next_message(VariantList *pVList)
	{
		messages_.erase(messages_.begin());
		if (messages_.size() != 0)
		{
			GetMessageManager()->CallEntityFunction(entity_, 1000, std::string("f_console_manager"), 0);
		}

	}

	void draw()
	{
		int height = GetBaseApp()->GetFont(FONT_SMALL)->MeasureText("Test",1.0f).y;
		for (int i = 0; i < messages_.size(); i++)
		{
			GetBaseApp()->GetFont(FONT_SMALL)->DrawScaled(15,GetScreenSizeYf() - 55 - (i+1)*height, messages_[i],1);
			if (i*height+55 > GetScreenSizeY()) //we check that we don't go too much up and keep drawing
			{
				return;
			}
		}
	}



private:
	std::vector<std::string> messages_;
	Entity* entity_;

};

extern console_manager *g_console;

class script_enviroment
{
public:
	script_enviroment();
	void init();

	/*
	* We set the function for the event if we already have a script
	*/
	void set_function(unit_event* _e);

	void load_scripts();

	/**
	* We get the event by the name for example "nextwave", returns the ID for next wave
	*/
	unit_event_type get_event_by_name(std::string str);

	//Event Type, Script
	std::vector<unit_event*> enemy_scripts_;

	//Event Type, Script
	std::vector<unit_event*> unit_scripts_;

	void BindSquirrel();

	HSQUIRRELVM vm_squirrel_;

	std::vector<Sqrat::Script*> sq_scripts_;

	Sqrat::Function *funcA2;
};

extern script_enviroment *g_script_enviroment;

#endif // SCRIPT_ENVIROMENT_H