#include "PlatformPrecomp.h"
#include "script_enviroment.h"

script_enviroment *g_script_enviroment;
console_manager *g_console;

script_enviroment::script_enviroment()
{

}

void script_enviroment::init()
{
	BindSquirrel();
	load_scripts();
}

unit_event_type script_enviroment::get_event_by_name(std::string str)
{
	std::string tmpStr = (str);
	unit_event_type m_type = UNIT_EVENT_NONE;
	if(tmpStr == "login")
		m_type = UNIT_EVENT_LOGIN;
	else if(tmpStr == "logout")
		m_type = UNIT_EVENT_LOGOUT;
	else if(tmpStr == "enemy_respawn")
		m_type = UNIT_EVENT_ENEMY_RESPAWN;

	return m_type;
}

void myFunc() 
{
	//LogMsg("This is a test from squirrell");
	Sqrat::Object slotVal = Sqrat::RootTable().GetSlot("myVal");
	if (slotVal.IsNull() == false)
	{
		//LogMsg("%d", slotVal.Cast<int>());
	}
}

struct global_vars_
{
	int var_test;
};

global_vars_ global_vars = { 4 };

class MyClass {
public:
	void Foo() {}
	int bar;
};

void print_zax(std::string string_to_print)
{
	LogMsg("%s",string_to_print.c_str());
}


void script_enviroment::set_function(unit_event* _e)
{

	if (_e->script_ != 0) //its not null
	{
		_e->script_->Run();

		if(_e->event_id_ == UNIT_EVENT_ENEMY_RESPAWN)
		{
			_e->func_ = new Sqrat::Function(Sqrat::RootTable(vm_squirrel_), "on_respawn_enemy"); // Via the constructor
		}
	}
}

void script_enviroment::load_scripts()
{
	/*//abrimos el archivo de configuracion
	//desencriptamos el archivo
	//leemos todos los scripts con sus tipos
		// type:"enemy_respawn"
		// file:"test.bin"
		//leemos el archivo de script
		//los desencruptamos
		//compilamos el script
		//cargamos la function y todo del script
		//lo agregamos a la lista
		//we open the file
	std::string v = "!zaxuhe_OS_V1.0!";
	ifstream file ("interface/data_s.bin", ios::in|ios::binary);
	std::string file_out;
	if (file.is_open())
	{
		//LogMsg("Encontro el archivo");

		//we load file data
		file.seekg(0, ios::end); // position get-ptr 0 bytes from end
		int size = (int)file.tellg();
		char *memblock = new char [size+1];
		file.seekg (0, ios::beg);
		//we read the file
		file.read (memblock, size);
		memblock[size] = '\0';
		file.close();

		//we decrypt the file :)
		file_out = decrypt_text(v,memblock,size);
		//std::cout << "Decrypt: " << file_out;
		//LogMsg("Decrypt: %s",file_out.c_str());
		//std::cout << " " <<  size << " " << _size;

		delete[] memblock;
	}

	std::string json_test = file_out;
	//LogMsg("Archivo %s",json_test.c_str());
	cJSON * root = cJSON_Parse(json_test.c_str());
	for (int i = 0 ; i < cJSON_GetArraySize(root); i++)
	{
		cJSON * subitem = cJSON_GetArrayItem(root, i);
		// handle subitem.  
			
		cJSON * _event_type= cJSON_GetObjectItem(subitem,"type");
		cJSON * _script_file = cJSON_GetObjectItem(subitem,"script");
		std::string event_type = _event_type->valuestring;
		std::string script_file = _script_file->valuestring;

		LogMsg("Type: %s, Script: %s",event_type.c_str(),script_file.c_str());

		std::string _script_text = decrypt_file(v, "interface/"+script_file);


		//LogMsg("Script: %s",_script_text.c_str());
		Sqrat::Script *temp = new Sqrat::Script();
		temp->CompileString(_script_text);
		unit_event *temp2 = new unit_event();
		temp2->script_ = temp;
		temp2->event_id_ = get_event_by_name(event_type);
		set_function(temp2);

		enemy_scripts_.push_back(temp2);
	}

	cJSON_Delete(root);*/
}

void script_enviroment::BindSquirrel() 
{
	
	vm_squirrel_ = sq_open(1024); //creates a VM with initial stack size 1024

	Sqrat::DefaultVM::Set(vm_squirrel_);

	Sqrat::Table myTable(vm_squirrel_);
	myTable.Func("MyFunc", &myFunc);
	
	Sqrat::RootTable(vm_squirrel_).SetValue("myVal",0);

	//this sets the print function :)
	Sqrat::RootTable(vm_squirrel_).Func<void (*)(std::string)>("print", &print_zax);
		
	/*Sqrat::Class<enemigo> class_enemigo(vm_squirrel_);
	class_enemigo.Var("status",&enemigo::status_);

	*/
	Sqrat::Class<MyClass> myClass(vm_squirrel_);
	myClass.Func("Foo", &MyClass::Foo);
	myClass.Var("bar", &MyClass::bar);

	Sqrat::Class<console_manager> Console_Manager(vm_squirrel_);
	Console_Manager.Func("print", &console_manager::add_message);



	//RootTable(vm).BindValue("other_var", &var_test);

		
	Sqrat::RootTable(vm_squirrel_).Bind("MyTable", myTable);
	Sqrat::RootTable(vm_squirrel_).Bind("MyClass", myClass);
	//Sqrat::RootTable(vm_squirrel_).Bind("enemigo", class_enemigo);
	Sqrat::RootTable(vm_squirrel_).Bind("console_manager", Console_Manager);
	Sqrat::RootTable(vm_squirrel_).SetInstance("console_test", g_console); /* now the variable c in C++ appears as the variable c in the root table in Squirrel */


    Sqrat::Class<global_vars_> _global_vars;
	_global_vars.Var("var_test", &global_vars_::var_test);
    Sqrat::RootTable(vm_squirrel_).Bind("global_vars_", _global_vars);
	Sqrat::RootTable(vm_squirrel_).SetInstance("global_vars", &global_vars); /* now the variable c in C++ appears as the variable c in the root table in Squirrel */


	/*Sqrat::Script *t = new Sqrat::Script();
	sq_scripts_.push_back(t);
		
	//script1->CompileString("MyTable.MyFunc(); myVal = myVal + 1;");
	t->CompileString("global_vars.var_test += 1; print(\"Val: Well this is my script running (I'm testing) \" + global_vars.var_test); function Add_FuncTest(a, b) { return a + b; }");

	Sqrat::Script *script2 = new Sqrat::Script();
	script2->CompileString("function Add_FuncTest(a, b) { return (a * b * 0)+test(); }function test(){ return 15;}");
	script2->Run();
	funcA2 = new Sqrat::Function(Sqrat::RootTable(vm_squirrel_), "Add_FuncTest"); // Via the constructor
	sq_scripts_.push_back(script2);
	//delete(script2);*/

	/*Sqrat::Script *temp = new Sqrat::Script();
	temp->CompileString("console_test.print(\"There was a respawn\"); function test(){ return 25;}console_test.print(\"n:\"+test()); function on_respawn_enemy(enem_){console_test.print(\"There is from the default function \"+enem_.status);enem_.status = 4;}");
	unit_event *temp2 = new unit_event();
	temp2->script_ = temp;
	temp2->event_id_ = get_event_by_name("enemy_respawn");
	set_function(temp2);

	enemy_scripts_.push_back(temp2);*/

}