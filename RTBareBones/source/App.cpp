/*
 *  App.cpp
 *  Created by Seth Robinson on 3/6/09.
 *  For license info, check the license.txt file that should have come with this.
 *
 */
#include "PlatformPrecomp.h"
#include "App.h"

#include "Entity/CustomInputComponent.h" //used for the back button (android)
#include "Entity/FocusInputComponent.h" //needed to let the input component see input messages
#include "Entity/ArcadeInputComponent.h" 
//#include "include/resource_img.h"
#include "script_enviroment.h"
#include "resource_file.h"
#include "fade_out_in.h"
#include "typewriter.h"
#include "game.h"
#include "spacecraft.h"
#include "nodes.h"
#include "messagebox.h"

MessageManager g_messageManager;
MessageManager * GetMessageManager() {return &g_messageManager;}

FileManager g_fileManager;
FileManager * GetFileManager() {return &g_fileManager;}

#include "Audio/AudioManager.h"
AudioManager g_audioManager; //to disable sound, this is a dummy

AudioManager * GetAudioManager(){return &g_audioManager;}

App *g_pApp = NULL;

BaseApp * GetBaseApp() 
{
	if (!g_pApp)
	{
		g_pApp = new App;
	}
	return g_pApp;
}

App * GetApp() 
{
	assert(g_pApp && "GetBaseApp must be called used first");
	return g_pApp;
}

App::App()
{
	m_bDidPostInit = false;
}

App::~App()
{
}

bool App::Init()
{
	g_game = 0;
	if (m_bInitted)	
	{
		return true;
	}
	
	if (!BaseApp::Init()) return false;
	
	if (GetEmulatedPlatformID() == PLATFORM_ID_IOS || GetEmulatedPlatformID() == PLATFORM_ID_WEBOS)
	{
		//SetLockedLandscape( true); //if we don't allow portrait mode for this game
		//SetManualRotationMode(true); //don't use manual, it may be faster (33% on a 3GS) but we want iOS's smooth rotations
	}

	LogMsg("The Save path is %s", GetSavePath().c_str());
	LogMsg("Region string is %s", GetRegionString().c_str());

#ifdef _DEBUG
	LogMsg("Built in debug mode");
#endif
#ifndef C_NO_ZLIB
	//fonts need zlib to decompress.  When porting a new platform I define C_NO_ZLIB and add zlib support later sometimes
	if (!GetFont(FONT_SMALL)->Load("interface/font_trajan.rtfont")) return false;
#endif

	SetupFakePrimaryScreenSize(960,640);
	//GetBaseApp()->SetFPSVisible(true);
	GetBaseApp()->SetFPSLimit(30);
	return true;
}

void App::Kill()
{
	BaseApp::Kill();
}

void App::OnExitApp(VariantList *pVarList)
{
	LogMsg("Exiting the app");
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_FINISH_APP;
	GetBaseApp()->AddOSMessage(o);
}

#define kFilteringFactor 0.1f
#define C_DELAY_BETWEEN_SHAKES_MS 500

//testing accelerometer readings. To enable the test, search below for "ACCELTEST"
//Note: You'll need to look at the  debug log to see the output. (For android, run PhoneLog.bat from RTBareBones/android)
void App::OnAccel(VariantList *pVList)
{
	
	if ( int(pVList->m_variant[0].GetFloat()) != MESSAGE_TYPE_GUI_ACCELEROMETER) return;

	CL_Vec3f v = pVList->m_variant[1].GetVector3();

	LogMsg("Accel: %s", PrintVector3(v).c_str());

	v.x = v.x * kFilteringFactor + v.x * (1.0f - kFilteringFactor);
	v.y = v.y * kFilteringFactor + v.y * (1.0f - kFilteringFactor);
	v.z = v.z * kFilteringFactor + v.z * (1.0f - kFilteringFactor);

	// Compute values for the three axes of the acceleromater
	float x = v.x - v.x;
	float y = v.y - v.x;
	float z = v.z - v.x;

	//Compute the intensity of the current acceleration 
	if (sqrt(x * x + y * y + z * z) > 2.0f)
	{
		Entity *pEnt = GetEntityRoot()->GetEntityByName("jumble");
		if (pEnt)
		{
			//GetAudioManager()->Play("audio/click.wav");
            VariantList vList(CL_Vec2f(), pEnt);
			pEnt->GetFunction("OnButtonSelected")->sig_function(&vList);
		}
		LogMsg("Shake!");
	}
}


//test for arcade keys.  To enable this test, search for TRACKBALL/ARCADETEST: below and uncomment the stuff under it.
//Note: You'll need to look at the debug log to see the output.  (For android, run PhoneLog.bat from RTBareBones/android)

void App::OnArcadeInput(VariantList *pVList)
{

	int vKey = pVList->Get(0).GetUINT32();
	eVirtualKeyInfo keyInfo = (eVirtualKeyInfo) pVList->Get(1).GetUINT32();
	
	string pressed;

	switch (keyInfo)
	{
		case VIRTUAL_KEY_PRESS:
			pressed = "pressed";
			break;

		case VIRTUAL_KEY_RELEASE:
			pressed = "released";
			break;

		default:
			LogMsg("OnArcadeInput> Bad value of %d", keyInfo);
	}
	

	string keyName = "unknown";

	switch (vKey)
	{
		case VIRTUAL_KEY_DIR_LEFT:
			keyName = "Left";
			break;

		case VIRTUAL_KEY_DIR_RIGHT:
			keyName = "Right";
			break;

		case VIRTUAL_KEY_DIR_UP:
			keyName = "Up";
			break;

		case VIRTUAL_KEY_DIR_DOWN:
			keyName = "Down";
			break;

	}
	
	LogMsg("Arcade input: Hit %d (%s) (%s)", vKey, keyName.c_str(), pressed.c_str());
}


void AppInput(VariantList *pVList)
{

	//0 = message type, 1 = parent coordinate offset, 2 is fingerID
	eMessageType msgType = eMessageType( int(pVList->Get(0).GetFloat()));
	CL_Vec2f pt = pVList->Get(1).GetVector2();
	//pt += GetAlignmentOffset(*m_pSize2d, eAlignment(*m_pAlignment));

	
	uint32 fingerID = 0;
	if ( msgType != MESSAGE_TYPE_GUI_CHAR && pVList->Get(2).GetType() == Variant::TYPE_UINT32)
	{
		fingerID = pVList->Get(2).GetUINT32();
	}

	CL_Vec2f vLastTouchPt = GetBaseApp()->GetTouch(fingerID)->GetLastPos();

	switch (msgType)
	{
	case MESSAGE_TYPE_GUI_CLICK_START:
		LogMsg("Touch start: X: %.2f YL %.2f (Finger %d)", pt.x, pt.y, fingerID);
		break;
	case MESSAGE_TYPE_GUI_CLICK_MOVE:
		LogMsg("Touch mode: X: %.2f YL %.2f (Finger %d)", pt.x, pt.y, fingerID);
		break;
	case MESSAGE_TYPE_GUI_CLICK_END:
		LogMsg("Touch end: X: %.2f YL %.2f (Finger %d)", pt.x, pt.y, fingerID);
		if (g_game->state_ == SHIP_NODES)
		{
			g_nodes->click_on_node(pt.x, pt.y);
		}
		break;
	}	


}

void App::Update()
{
	if (g_game != 0 && g_messagebox != 0 && g_messagebox->on_ == false)
		g_game->already_clicked_ = false;
	else if (g_game != 0)
		g_game->already_clicked_ = true;

	//game can think here.  The baseApp::Update() will run Update() on all entities, if any are added.  The only one
	//we use in this example is one that is watching for the Back (android) or Escape key to quit that we setup earlier.
	BaseApp::Update();

	if (!m_bDidPostInit)
	{
		//stuff I want loaded during the first "Update"
		m_bDidPostInit = true;
		
		//for android, so the back key (or escape on windows) will quit out of the game
		Entity *pEnt = GetEntityRoot()->AddEntity(new Entity);
		EntityComponent *pComp = pEnt->AddComponent(new CustomInputComponent);
		//tell the component which key has to be hit for it to be activated
		pComp->GetVar("keycode")->Set(uint32(VIRTUAL_KEY_BACK));
		//attach our function so it is called when the back key is hit
		pComp->GetFunction("OnActivated")->sig_function.connect(1, boost::bind(&App::OnExitApp, this, _1));

		//nothing will happen unless we give it input focus
		pEnt->AddComponent(new FocusInputComponent);

		//ACCELTEST:  To test the accelerometer uncomment below: (will print values to the debug output)
		//SetAccelerometerUpdateHz(25); //default is 0, disabled
		//GetBaseApp()->m_sig_accel.connect(1, boost::bind(&App::OnAccel, this, _1));

		//TRACKBALL/ARCADETEST: Uncomment below to see log messages on trackball/key movement input
		pComp = pEnt->AddComponent(new ArcadeInputComponent);
		GetBaseApp()->m_sig_arcade_input.connect(1, boost::bind(&App::OnArcadeInput, this, _1));
	
		//these arrow keys will be triggered by the keyboard, if applicable
		AddKeyBinding(pComp, "Left", VIRTUAL_KEY_DIR_LEFT, VIRTUAL_KEY_DIR_LEFT);
		AddKeyBinding(pComp, "Right", VIRTUAL_KEY_DIR_RIGHT, VIRTUAL_KEY_DIR_RIGHT);
		AddKeyBinding(pComp, "Up", VIRTUAL_KEY_DIR_UP, VIRTUAL_KEY_DIR_UP);
		AddKeyBinding(pComp, "Down", VIRTUAL_KEY_DIR_DOWN, VIRTUAL_KEY_DIR_DOWN);
		AddKeyBinding(pComp, "Fire", VIRTUAL_KEY_CONTROL, VIRTUAL_KEY_GAME_FIRE);

		//INPUT TEST - wire up input to some functions to manually handle.  AppInput will use LogMsg to
		//send them to the log.  (Each device has a way to view a debug log in real-time)
		GetBaseApp()->m_sig_input.connect(&AppInput);

		/*
		//file handling test, if TextScanner.h is included at the top..

		TextScanner t;
		t.m_lines.push_back("Testing 123");
		t.m_lines.push_back("Fuck ya'll!");
		t.m_lines.push_back("Whoopsopsop!");

		LogMsg("Saving file...");
		t.SaveFile("temp.txt");


		TextScanner b;
		b.LoadFile("temp.txt");
		b.DumpToLog();
		*/

		//init_resource_img();
		//g_resource_images->add_to_load_queve("final.rttex", "interface/particle.bin");

		g_console = new console_manager();
		g_console->init();
		/*g_console->add_message("test 1");
		g_console->add_message("`2test 2");
		g_console->add_message("test 3");
		g_console->add_message("test 4");
		g_console->add_message("test 5");*/
		g_script_enviroment = new script_enviroment();
		g_script_enviroment->init();

		g_fader = new fade_out_in();
		g_fader->init();

		g_typewriter = new typewriter();
		g_typewriter->init();

		g_spacecraft = new spacecraft();
		g_spacecraft->init();


		g_game = new game();
		g_game->init();

		g_nodes = new nodes();
		g_nodes->init();

		g_messagebox = new messagebox();
		g_messagebox->init();
	}
	//game is thinking.  

	//we set the position of the game
	//we get the screen width and height
	//LogMsg("%d %d",GetScreenSizeX(),GetScreenSizeY());
	g_fader->update();
	g_typewriter->update();
	g_messagebox->update();
	g_spacecraft->update();
}
/*


void start_up_map()
{
	
	static bool once = false;
	if (once == false) //we allocate only once
	{
		//g_nodes->set_nodes();
		for (int i = 0; i < 30; i++)
		{
			for (int z = 0; z < 30; z++)
			{
					tile_set_[i][z] = Point(i,z,true);
					tile_set_[i][z].x = i;
					tile_set_[i][z].y = z;
					tile_set_[i][z].walkable = true;
			}
		}
		char_1_ = new Surface("interface/unit_1.rttex");
		char_2_ = new Surface("interface/unit_2.rttex");
		char_3_ = new Surface("interface/unit_3.rttex");
		char_4_ = new Surface("interface/unit_4.rttex");
		tile_set_[1][4].walkable =false;

		tile_set_[3][0].walkable =false;
		tile_set_[3][1].walkable =false;
		tile_set_[3][2].walkable =false;
		tile_set_[3][3].walkable =false;
		tile_set_[3][4].walkable =false;
		tile_set_[3][5].walkable =false;
		tile_set_[3][6].walkable =false;
		tile_set_[2][6].walkable =false;
		tile_set_[3][6].walkable =false;
		tile_set_[4][6].walkable =false;
		tile_set_[5][6].walkable =false;
		tile_set_[0][6].walkable =false;
		tile_set_[0][8].walkable =false;
		tile_set_[1][8].walkable =false;
		tile_set_[2][8].walkable =false;
		tile_set_[3][8].walkable =false;
		tile_set_[4][8].walkable =false;
		tile_set_[5][8].walkable =false;
		tile_set_[6][8].walkable =false;
		tile_set_[7][8].walkable =false;
		tile_set_[7][7].walkable =false;
		tile_set_[7][6].walkable =false;
		tile_set_[7][5].walkable =false;
		tile_set_[7][4].walkable =false;
		//tile_set_[7][3].walkable =false;
		tile_set_[7][2].walkable =false;
		tile_set_[7][1].walkable =false;
		tile_set_[7][0].walkable =false;
		tile_set_[8][20].walkable =false;
		tile_set_[5][4].walkable =false;
		tile_set_[6][4].walkable =false;

		tile_set_[5][2].walkable =false;
		tile_set_[5][3].walkable =false;
		tile_set_[5][1].walkable =false;

		unit_list_.push_back(unit());
		unit_list_[0].set_current_pos(1,0);
		unit_list_[0].create_path(29,29);

		unit_list_.push_back(unit());
		unit_list_[1].set_current_pos(29,0);
		unit_list_[1].create_path(29,29);

		unit_list_.push_back(unit());
		unit_list_[2].set_current_pos(0,29);
		unit_list_[2].create_path(29,29);
		//g_fader->fade_out(100,fade_out_callback);
		//g_typewriter->type_text(rtRect (355,310,606,485), "By Zaxuhe", 80,1.0f,MAKE_RGBA(47,51,43,255));
	}

	once = true;
}*/

void App::Draw()
{
	//start_up_map();

	static Surface *bg_test_ = new Surface("interface/bg_test.rttex");
	static Surface *bg_go_ = new Surface("interface/game_over.rttex");
	static Surface *station_ = new Surface("interface/eco_station.rttex");
	static Surface *bg_test_space_ = new Surface("interface/bg_stars.rttex");
	/*static Surface *bg_1_ = new Surface("interface/fade_1.rttex");
	static Surface *bg_2_ = new Surface("interface/fade_2.rttex");
	static Surface *bg_3_ = new Surface("interface/fade_3.rttex");
	static Surface *bg_4_ = new Surface("interface/fade_4.rttex");
	static Surface *bg_5_ = new Surface("interface/fade_5.rttex");
	static Surface *bg_6_ = new Surface("interface/fade_6.rttex");*/
	static Surface *wall_ = new Surface("interface/tile_007.rttex");
	static Surface *path_ = new Surface("interface/tile_005.rttex");
	static int frames_ = 0;
	frames_++;

	//Use this to prepare for raw GL calls
	PrepareForGL();
	
	glClearColor(0.509803922f,0.509803922f,0.509803922f,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//draw our game stuff
	//DrawFilledRect(10.0f,10.0f,GetScreenSizeXf()/3,GetScreenSizeYf()/3, MAKE_RGBA(255,255,0,255));
	//DrawFilledRect(0,0,64,64, MAKE_RGBA(0,255,0,100));

	//after our 2d rect call above, we need to prepare for raw GL again. (it keeps it in ortho mode if we don't for speed)
	PrepareForGL();
	static float dis_x = 0;
	static float dis_y = 0;
	/*if (g_resource_images->load_next())
	{
		CL_Vec2f text_size = GetFont(FONT_SMALL)->MeasureText("Loading",1.0f);
		GetFont(FONT_SMALL)->DrawScaled(GetScreenSizeXf()/2-text_size.x/2,GetScreenSizeYf()/2-text_size.y/2, "Loading",1.0f,MAKE_RGBA(255,255,255,255));
		if (g_resource_images->load_queve_.size() == 0) //time to init the game beacuase it has ben laoded already
		{
			//everything has been loaded from the queve we can move to the next state
		}
	}
	else
	{*/

	if (g_game->state_ == SPLASH_SCREEN)
	{
		bg_test_->Blit(0,0);
	}
	else if (g_game->state_ == MAIN_MENU)
	{
		bg_test_space_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,256,256),CL_Vec2f(dis_x,dis_y));
		dis_x+=0.1;
		dis_y+=0.1;
		station_->Blit(GetScreenSizeX()/2.0f-station_->GetWidth()/2.0f,GetScreenSizeY()/2.0f-station_->GetHeight()/2.0f);
		g_game->draw();
	}
	else if (g_game->state_ == CREATE_SHIP)
	{
		bg_test_space_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,256,256),CL_Vec2f(dis_x,dis_y));
		dis_x+=0.1;
		dis_y+=0.1;
		g_spacecraft->draw();
	}
	else if (g_game->state_ == MARKET)
	{
		g_game->draw();
	}
	else if (g_game->state_ == SELECT_MODULE)
	{
		g_game->draw();
	}
	else if (g_game->state_ == JOBS)
	{
		g_game->draw();
	}
	else if (g_game->state_ == SHIP_NODES)
	{
		bg_test_space_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,256,256),CL_Vec2f(dis_x,dis_y));
		dis_x+=0.1;
		dis_y+=0.1;
		g_nodes->draw();
		g_nodes->r_batch_nodes->Flush_no_clean(RenderBatcher::eFlushMode::FLUSH_SETUP_RENDER);

		for (int i = 0; i < g_nodes->spaceships_.size(); i++)
		{
			g_nodes->spaceships_[i].walk();
			g_nodes->spaceships_[i].draw();
		}
	}
	else if (g_game->state_ == BATTLE_SHIP)
	{
		bg_test_space_->BlitEx2_zax(rtRectf(0,0,GetScreenSizeX(),GetScreenSizeY()),rtRectf(0,0,256,256),CL_Vec2f(dis_x,dis_y));
		dis_x-=0.3;
		dis_y+=0.3;
		g_spacecraft->draw_battle();
	}
	else if (g_game->state_ == GAME_OVER)
	{
		bg_go_->Blit(0,0);
	}
	//}
	/*
	static RenderBatcher *r_batch_ = new RenderBatcher();

	static bool once = false;
	if (once == false)
	{
		for (int i = 0; i < 30; i++)
		{
			for (int z = 0; z < 30; z++)
			{
				if (tile_set_[i][z].walkable == false)
				{
					//wall_->Blit(i*16,z*16);
					r_batch_->BlitEx(wall_,rtRectf(i*16,z*16,i*16+16,z*16+16),rtRectf(0,0,16,16));
				}
				else
				{
					//path_->Blit(i*16,z*16);
					r_batch_->BlitEx(path_,rtRectf(i*16,z*16,i*16+16,z*16+16),rtRectf(0,0,16,16));
				}
			}
		}
	}
	once = true;
	glPushMatrix();
		glTranslatef(20,100,0);
		r_batch_->Flush_no_clean(RenderBatcher::eFlushMode::FLUSH_SETUP_RENDER);

		for (int i = 0; i < unit_list_.size(); i++)
		{
			unit_list_[i].walk();
			unit_list_[i].draw_to_batch();
		}
		r_batch_chars1->Flush();
		r_batch_chars2->Flush();
		r_batch_chars3->Flush();
		r_batch_chars4->Flush();
	glPopMatrix();*/

	/*
	draw_nodes();
	r_batch_nodes->Flush_no_clean(RenderBatcher::eFlushMode::FLUSH_SETUP_RENDER);

	for (int i = 0; i < spaceships_.size(); i++)
	{
		spaceships_[i].walk();
		spaceships_[i].draw();
	}
	*/
	g_typewriter->draw();
	g_messagebox->draw();
	g_fader->draw();

	//g_console->draw();
	BaseApp::Draw();
}



void App::OnScreenSizeChange()
{
	BaseApp::OnScreenSizeChange();
}

void App::OnEnterBackground()
{
	//save your game stuff here, as on some devices (Android <cough>) we never get another notification of quitting.
	LogMsg("Entered background");
	BaseApp::OnEnterBackground();
}

void App::OnEnterForeground()
{
	LogMsg("Entered foreground");
	BaseApp::OnEnterForeground();
}

const char * GetAppName() {return "BareBones";}

//the stuff below is for android/webos builds.  Your app needs to be named like this.

//note: these are put into vars like this to be compatible with my command-line parsing stuff that grabs the vars

const char * GetBundlePrefix()
{
	const char * bundlePrefix = "com.rtsoft.";
	return bundlePrefix;
}

const char * GetBundleName()
{
	const char * bundleName = "rtbarebones";
	return bundleName;
}
