#include "ComponentInformer.hpp"

#include "ComponentManager.hpp"
#include "CommandInterpreter.hpp"
#include "Property.hpp"

#include <sstream>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

namespace Core {

ComponentInformer::ComponentInformer(ComponentManager & cm) : m_component_manager(cm) {
}

void ComponentInformer::registerHandlers(CommandInterpreter & ci) {
	ci.addHandler("listProperties", boost::bind(&ComponentInformer::listProperties,  this, _1));
	ci.addHandler("getProperty", boost::bind(&ComponentInformer::getProperty,  this, _1));
	ci.addHandler("setProperty", boost::bind(&ComponentInformer::setProperty,  this, _1));

	ci.addHandler("getPropertyType", boost::bind(&ComponentInformer::getPropertyType,  this, _1));
	ci.addHandler("getPropertyToolTip", boost::bind(&ComponentInformer::getPropertyToolTip,  this, _1));

	ci.addHandler("getPropertyConstraints", boost::bind(&ComponentInformer::getPropertyConstraints,  this, _1));

	ci.addHandler("listHandlers", boost::bind(&ComponentInformer::listHandlers,  this, _1));
	ci.addHandler("triggerHandler", boost::bind(&ComponentInformer::triggerHandler,  this, _1));

	ci.addHandler("getMetaInfo", boost::bind(&ComponentInformer::getMetaInfo,  this, _1));
	ci.addHandler("setBump", boost::bind(&ComponentInformer::setBump,  this, _1));
}

std::string ComponentInformer::listProperties(std::vector<std::string> args) {
	if (args.empty()) {
		return "No component name specified.";
	}

	Base::Component * component;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	std::string rep = component->listProperties();

	if (rep.empty())
		rep = "\n";

	return rep;
}

std::string ComponentInformer::getPropertyConstraints(std::vector<std::string> args) {
	if (args.size() != 2) {
		return "No component name specified.";
	}

	Base::Component * component;
	Base::PropertyInterface * prop;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	prop = component->getProperty(args[1]);
	if (!prop) {
		return "Unknown property";
	}

	std::string rep;

	int cc = prop->countConstraints();
	for (int i = 0; i < cc; ++i) {
		rep += prop->getConstraint(i) + "\n";
	}

	if (rep.empty())
		rep = "\n";

	return rep;
}

std::string ComponentInformer::getProperty(std::vector<std::string> args) {
	if (args.size() != 2) {
		return "No component name specified.";
	}

	Base::Component * component;
	Base::PropertyInterface * prop;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	prop = component->getProperty(args[1]);
	if (!prop) {
		return "Unknown property";
	}

	return prop->store();
}

std::string ComponentInformer::getPropertyType(std::vector<std::string> args) {
	if (args.size() != 2) {
		return "No component name specified.";
	}

	Base::Component * component;
	Base::PropertyInterface * prop;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	prop = component->getProperty(args[1]);
	if (!prop) {
		return "Unknown property";
	}

	return prop->type();
}

std::string ComponentInformer::getPropertyToolTip(std::vector<std::string> args) {
	if (args.size() != 2) {
		return "No component name specified.";
	}

	Base::Component * component;
	Base::PropertyInterface * prop;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	prop = component->getProperty(args[1]);
	if (!prop) {
		return "Unknown property";
	}

	return prop->toolTip();
}

std::string ComponentInformer::setProperty(std::vector<std::string> args) {
	if (args.size() != 3) {
		return "No component name specified.";
	}

	Base::Component * component;
	Base::PropertyInterface * prop;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	prop = component->getProperty(args[1]);
	if (!prop) {
		return "Unknown property";
	}

	prop->retrieve(args[2]);

	return prop->store();
}


std::string ComponentInformer::listHandlers(std::vector<std::string> args) {
	if (args.empty()) {
		return "No component name specified.";
	}

	Base::Component * component;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	std::string rep = component->listHandlers();

	if (rep.empty())
		rep = "\n";

	return rep;
}

std::string ComponentInformer::triggerHandler(std::vector<std::string> args) {
	if (args.size() != 2) {
		return "No component name specified.";
	}

	Base::Component * component;
	Base::EventHandlerInterface * handler;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	handler = component->getHandler(args[1]);
	if (!handler) {
		return "Unknown handler";
	}

	handler->execute();
	return "OK";
}

std::string ComponentInformer::getMetaInfo(std::vector<std::string> args) {
	if (args.size() != 1) {
		return "No component name specified.";
	}

	Base::Component * component;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	std::stringstream ss;
	ss << component->getType() << "\n";
	ss << component->getPriority() << "\n";
	ss << component->getBump() << "\n";

	return ss.str();
}

std::string ComponentInformer::setBump(std::vector<std::string> args) {
	if (args.size() != 2) {
		return "Invalid command syntax.";
	}

	Base::Component * component;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	/// \todo sprawdzic rezultat (czy udal sie lexical cast), sprawdzic, czy wlasciwy poziom zostal ustawiony itp.
	component->setBump(boost::lexical_cast<int>(args[1]));

	return "OK";
}


}
