#include <injector/system.hpp>

namespace INJECTOR_NAMESPACE
{
	System::System(Manager& _manager) :
		manager(_manager)
	{}
	System::~System()
	{}
}
