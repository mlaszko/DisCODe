#include "ComponentProxy.hpp"
#include "DisCODeClient.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace DisCODe {

ComponentProxy::ComponentProxy(Client * client, const std::string & name) : m_client(client), m_name(name) {
	m_priority = -1;
	m_bump = -1;
}

void ComponentProxy::refreshPropertyConstraints(int i) {
	std::string request;
	request += "getPropertyConstraints";
	request += ":";
	request += m_name + ":";
	request += properties[i];

	std::vector<std::string> consts;

	std::string str = m_client->send(request);

	size_t start = 0, end = 0;
	const std::string separator = "\n";

	std::string tmp;

	while (end != std::string::npos) {
		end = str.find(separator, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		tmp = str.substr(start, (end == std::string::npos) ? std::string::npos : end - start);

		if (tmp.empty())
			break;

		consts.push_back(tmp);

		std::cout << tmp << "\n";


		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = ((end > (std::string::npos - separator.size())) ? std::string::npos : end + separator.size());
	}

	constraints.push_back(consts);
}

void ComponentProxy::refresh() {
	std::string request;
	request += "listProperties";
	request += ":";
	request += m_name;

	std::string str = m_client->send(request);

	size_t start = 0, end = 0;
	std::string separator = "\n";

	std::string tmp;

	properties.clear();
	constraints.clear();

	while (end != std::string::npos) {
		end = str.find(separator, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		tmp = str.substr(start, (end == std::string::npos) ? std::string::npos : end - start);

		if (tmp.empty())
			break;

		properties.push_back(tmp);

		refreshPropertyConstraints(properties.size()-1);

		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = ((end > (std::string::npos - separator.size())) ? std::string::npos : end + separator.size());
	}



	request = "listHandlers";
	request += ":";
	request += m_name;

	str = m_client->send(request);

	start = 0, end = 0;
	separator = "\n";

	handlers.clear();
	while (end != std::string::npos) {
		end = str.find(separator, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		tmp = str.substr(start, (end == std::string::npos) ? std::string::npos : end - start);

		if (tmp.empty())
			break;

		handlers.push_back(tmp);


		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = ((end > (std::string::npos - separator.size())) ? std::string::npos : end + separator.size());
	}

	request = "getMetaInfo";
	request += ":";
	request += m_name;

	str = m_client->send(request);

	separator = "\n";
	std::vector<std::string> strs;
	boost::split(strs,str,boost::is_any_of(separator));

	m_type = strs[0];
	m_priority = boost::lexical_cast<int>(strs[1]);
	m_bump = boost::lexical_cast<int>(strs[2]);
}

void ComponentProxy::print(int indent) {
	std::string pref;
	for (int i = 0; i < indent; ++i)
		pref += "\t";

	std::cout << pref << "[C] " << m_name << "\n";

	pref += "\t";

	BOOST_FOREACH(std::string pp, properties) {
		std::cout << pref << "[P] " << pp << std::endl;
	}

	BOOST_FOREACH(std::string h, handlers) {
		std::cout << pref << "[H] " << h << std::endl;
	}
}

void ComponentProxy::triggerHandler(const std::string & handler) {
	for (size_t i = 0; i < handlers.size(); ++i) {
		if (handlers[i] == handler) {
			triggerHandler(i);
			break;
		}
	}
}

void ComponentProxy::triggerHandler(int i) {
	std::string req;

	req += "triggerHandler:";
	req += m_name + ":";
	req += handlers[i];

	m_client->send(req);
}

std::string ComponentProxy::getPropertyValue(int i) {
	std::string req = "getProperty:";
	req += m_name + ":";
	req += properties[i];

	std::string val = m_client->send(req);

	return val;
}

std::string ComponentProxy::setPropertyValue(int i, const std::string & val) {
	return setPropertyValue(properties[i], val);
}

std::string ComponentProxy::setPropertyValue(const std::string & name, const std::string & val) {
	std::string req = "setProperty:";
	req += m_name + ":";
	req += name + ":";
	req += val;

	std::string ret = m_client->send(req);

	return ret;
}

std::string ComponentProxy::getPropertyType(int i) {
	std::string req = "getPropertyType:";
	req += m_name + ":";
	req += properties[i];

	std::string val = m_client->send(req);

	return val;
}

std::string ComponentProxy::getPropertyToolTip(int i) {
	std::string req = "getPropertyToolTip:";
	req += m_name + ":";
	req += properties[i];

	std::string val = m_client->send(req);

	return val;
}

void ComponentProxy::setBump(int bump) {
	std::string req = "setBump:";
	req += m_name + ":";
	req += boost::lexical_cast<std::string>(bump);

	m_client->send(req);

	m_bump = bump;
}

}
