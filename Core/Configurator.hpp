/*!
 * \file Configurator.hpp
 * \brief File containing the declaration of Configurator class.
 *
 * \author tkornuta
 * \date Apr 8, 2010
 */

#ifndef CONFIGURATOR_HPP_
#define CONFIGURATOR_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <sys/stat.h>
#include <iostream>
#include <string>

#include "Singleton.hpp"
#include "Kernel_Aux.hpp"

namespace Core {

using namespace boost::property_tree;

/*!
 * \class Configurator
 * \brief Class responsible for configuration management.
 * Inherits singleton-behavior from the Singleton class.
 * \date April 8 2010
 * \author tkornuta
 */
class Configurator
{
private:

	typedef std::pair<std::string, ptree> TreeNode;

	/*!
	 * Stored configuration.
	 */
	ptree configuration;

	/*!
	 * Configuration filename.
	 */
	std::string configuration_filename;

public:
	Configurator();

	virtual ~Configurator();

	/*!
	 * Loads configuration from xml file.
	 */
	void loadConfiguration(std::string filename);

	void loadKernels(const ptree * node);
};

}//: namespace Core

#endif /* CONFIGURATOR_HPP_ */