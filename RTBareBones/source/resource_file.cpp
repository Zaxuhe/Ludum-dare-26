#include "PlatformPrecomp.h"
#include "resource_file.h"
#include <string.h>

resource_file g_resources;

resource_file::resource_file()
{
    //ctor
	animations_load();
	count_images_ = 0;
	base_app_deleted_ = false;
}

resource_file::~resource_file()
{
    //dtor
}

/* we return a texture id containing the image */
texture_anim* resource_file::get_image(std::string _name)
{

	//buscamos en los frames data (los datos de las animaciones), si no existe nos regresa final_name_ vacio, si existe entonces lo usamos
	if (frames_data_[_name].final_name_ == "")
    {
        frames_data_[_name].final_name_ = _name;
    }

	//usamos el nombre con la animacion
    std::string new_name = frames_data_[_name].final_name_;

	//news es el nombre del archivo que vamos a usar
    std::string news= "interface/"+new_name;

	if (new_name.length() == 0)
    {
        return 0;
    }

	//revisamos que la imagen este cargada (que exista)
    if (images_loaded_[new_name] != 0)
	{
		//si la animacion no esta cargada la cargamos
		if (!images_loaded_[new_name]->animation->IsLoaded())
		{
			images_loaded_[new_name]->animation->LoadFile(news.c_str());
		}
		
		//a la imagen cargada le agregamos el frame data
		images_loaded_[new_name]->set_frame(&frames_data_[_name]);
		return images_loaded_[new_name];

    }

	//como no existe tenemos que cargar los datos
	SurfaceAnim* _new_surface = new SurfaceAnim();
	if (!_new_surface->IsLoaded())
	{
		_new_surface->LoadFile(news.c_str());
	}

	texture_anim *_new_surface_struct = new texture_anim();
	_new_surface_struct->animation = _new_surface;
	_new_surface_struct->setup_anim(1,1);

	images_loaded_[new_name] = _new_surface_struct;

	images_loaded_[new_name]->set_frame(&frames_data_[_name]);
	return images_loaded_[new_name];


}

void resource_file::animations_load()
{

//characters
frames_data_["char_n_1.rttex"] = frames(45,39,"characters.rttex",5);

frames_data_["char_n_1.rttex"].tiles_data.push_back(tile_data(0,0));
frames_data_["char_n_1.rttex"].tiles_data.push_back(tile_data(46,0));
frames_data_["char_n_1.rttex"].tiles_data.push_back(tile_data(0,40));
frames_data_["char_n_1.rttex"].tiles_data.push_back(tile_data(46,40));
frames_data_["char_n_1.rttex"].tiles_data.push_back(tile_data(0,80));
frames_data_["char_n_1.rttex"].tiles_data.push_back(tile_data(46,80));

//explo big
frames_data_["exp_b.rttex"] = frames(47,47,"exp_b.rttex",8);

frames_data_["exp_b.rttex"].tiles_data.push_back(tile_data(0,0));
frames_data_["exp_b.rttex"].tiles_data.push_back(tile_data(47,0));
frames_data_["exp_b.rttex"].tiles_data.push_back(tile_data(47*2,0));
frames_data_["exp_b.rttex"].tiles_data.push_back(tile_data(47*3,0));
frames_data_["exp_b.rttex"].tiles_data.push_back(tile_data(0,47));
frames_data_["exp_b.rttex"].tiles_data.push_back(tile_data(47,47));

//explo small
frames_data_["exp_small.rttex"] = frames(23,23,"exp_small.rttex",8);

frames_data_["exp_small.rttex"].tiles_data.push_back(tile_data(0,0));
frames_data_["exp_small.rttex"].tiles_data.push_back(tile_data(23,0));
frames_data_["exp_small.rttex"].tiles_data.push_back(tile_data(23*2,0));
frames_data_["exp_small.rttex"].tiles_data.push_back(tile_data(0,23));
frames_data_["exp_small.rttex"].tiles_data.push_back(tile_data(23,23));
frames_data_["exp_small.rttex"].tiles_data.push_back(tile_data(23*2,23));
frames_data_["exp_small.rttex"].tiles_data.push_back(tile_data(0,46));
frames_data_["exp_small.rttex"].tiles_data.push_back(tile_data(23,46));

//teleport
frames_data_["teleportout.rttex"] = frames(76,76,"teleportout.rttex",12);

frames_data_["teleportout.rttex"].tiles_data.push_back(tile_data(0,0));
frames_data_["teleportout.rttex"].tiles_data.push_back(tile_data(76,0));
frames_data_["teleportout.rttex"].tiles_data.push_back(tile_data(76*2,0));
frames_data_["teleportout.rttex"].tiles_data.push_back(tile_data(0,76));
frames_data_["teleportout.rttex"].tiles_data.push_back(tile_data(76,76));
frames_data_["teleportout.rttex"].tiles_data.push_back(tile_data(76*2,76));
frames_data_["teleportout.rttex"].tiles_data.push_back(tile_data(0,76*2));
frames_data_["teleportout.rttex"].tiles_data.push_back(tile_data(76,76*2));
frames_data_["teleportout.rttex"].tiles_data.push_back(tile_data(76,76*2));

//loading
frames_data_["anim_loading.rttex"] = frames(64,64,"anim_loading.rttex",12);

frames_data_["anim_loading.rttex"].tiles_data.push_back(tile_data(0,0));
frames_data_["anim_loading.rttex"].tiles_data.push_back(tile_data(64,0));
frames_data_["anim_loading.rttex"].tiles_data.push_back(tile_data(64*2,0));
frames_data_["anim_loading.rttex"].tiles_data.push_back(tile_data(0,64));
frames_data_["anim_loading.rttex"].tiles_data.push_back(tile_data(64,64));
frames_data_["anim_loading.rttex"].tiles_data.push_back(tile_data(64*2,64));
frames_data_["anim_loading.rttex"].tiles_data.push_back(tile_data(0,64*2));
frames_data_["anim_loading.rttex"].tiles_data.push_back(tile_data(64,64*2));

//stars glow / notification
frames_data_["anim_stars.rttex"] = frames(128,78,"anim_stars.rttex",1);
frames_data_["anim_stars.rttex"].tiles_data.push_back(tile_data(0,0));
frames_data_["anim_stars.rttex"].tiles_data.push_back(tile_data(128,0));

}

/**
* Constructor
*
* @param _filename, el nombre del archivo
* @param _draw_itself, si se dibuja autmaticamente, o si el dibujo es manual
*/
image_object_::image_object_(std::string _filename/* = "test.bmp"*/, bool _draw_itself/* = true*/)
{
	//LogMsg("img1");

	/*Entity* pGUIEnt = GetEntityRoot()->AddEntity(new Entity("GUI"));
	pGUIEnt->GetVar("filename")->Set(Variant(_filename));
	LogMsg("img41");
	pGUIEnt->GetVar("filename")->GetSigOnChanged()->connect(boost::bind(&image_object_::on_filename_changed, this, _1));
	LogMsg("img42");
	LogMsg("%s",_filename.c_str());
	pGUIEnt->GetVar("filename")->Set(Variant(_filename));
	LogMsg("%s",pGUIEnt->GetVar("filename")->GetString().c_str());*/

	std::stringstream o_name;
	o_name << g_resources.count_images_++ << "_nextframe";
	object_name_ = o_name.str();
	//LogMsg("img2");

	entity_ = GetEntityRoot()->AddEntity(new Entity(object_name_));
	entity_->AddComponent(new FocusUpdateComponent());
	//LogMsg("img3");

	if (_draw_itself == true)
	{
		entity_->AddComponent(new FocusRenderComponent());
		entity_->GetFunction("OnRender")->sig_function.connect(1, boost::bind(&image_object_::draw, this, _1));
	}
	entity_->GetVar("filename")->Set(Variant(_filename));
	entity_->GetVar("filename")->GetSigOnChanged()->connect(boost::bind(&image_object_::on_filename_changed, this, _1));
	entity_->GetVar("filename")->Set(Variant(_filename));

	entity_->GetVar("enabled")->GetSigOnChanged()->connect(boost::bind(&image_object_::on_enabled_changed, this, _1));
	entity_->GetVar("enabled")->Set(uint32(1));

	entity_->GetVar("rotation")->GetSigOnChanged()->connect(boost::bind(&image_object_::on_rotation_changed, this, _1));
	entity_->GetVar("rotation")->Set(0.0f);

	entity_->GetVar("pos2d")->GetSigOnChanged()->connect(boost::bind(&image_object_::on_position_changed, this, _1));
	pos2d_ = entity_->GetVarWithDefault("pos2d", Variant(0.0f, 0.0f))->GetVector2();
		


	current_frame_ = -1;
	entity_->GetFunction(object_name_)->sig_function.connect(boost::bind(&image_object_::next_frame, this, _1));
	VariantList t_1(entity_);
	entity_->GetFunction(object_name_)->sig_function(&t_1);



	entity_->GetVar("animation_loop")->GetSigOnChanged()->connect(boost::bind(&image_object_::on_animation_loop_changed, this, _1));
	entity_->GetVar("animation_loop")->Set(uint32(1));
	animation_loops_ = true;
	animation_finished_ = false;

	has_input_ = false;

	draw_itself_ =_draw_itself;

	centro_ = false;

	transparency_ = 255;
	entity_->GetVar("transparency")->GetSigOnChanged()->connect(boost::bind(&image_object_::on_transparency_changed, this, _1));
	entity_->GetVar("transparency")->Set(float(transparency_));
	
}

void image_object_::on_filename_changed(Variant *pDataObject)
{
	filename_ = entity_->GetVar("filename")->GetString();
	anim_data_ = g_resources.get_image(filename_);
	entity_->GetVar("current_frame_")->Set(int32(0));
	animation_finished_ = false;
}