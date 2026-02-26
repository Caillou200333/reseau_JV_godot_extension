#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "example_class.h"
#include "summator.h"
#include "gdexample.h"
#include "Network/GDNetworkManager.h"

#include "Network/Messages/GDBaseMessage.h"
#include "Network/Messages/GDHELOMessage.h"
#include "Network/Messages/GDGameplayMessage.h"
#include "Network/Messages/GDTextMessage.h"

#include "Entities/GDEntity.h"
#include "Entities/GDEntityManager.h"

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(ExampleClass);
	GDREGISTER_CLASS(Summator);
	GDREGISTER_RUNTIME_CLASS(GDExample);
	GDREGISTER_RUNTIME_CLASS(GDNetworkManager);

	GDREGISTER_CLASS(GDBaseMessage);
	GDREGISTER_CLASS(GDHELOMessage);
	GDREGISTER_CLASS(GDGameplayMessage);
	GDREGISTER_CLASS(GDTextMessage);
	
	GDREGISTER_RUNTIME_CLASS(GDEntity);
	GDREGISTER_RUNTIME_CLASS(GDEntityManager);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT GDNetwork_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}