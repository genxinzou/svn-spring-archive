/**
 * @file ConfigHandler.h
 * @brief config base
 * @author Christopher Han <xiphux@gmail.com>
 *
 * Definition of config structure class
 * Copyright (C) 2005.  Licensed under the terms of the
 * GNU GPL, v2 or later.
 */
#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <string>
#include <sstream>
#include <map>
#include <stdio.h>

#ifdef __FreeBSD__
#include <sys/stat.h>
#endif

/**
 * @brief config handler base
 *
 * This is the abstract configuration handler class used
 * for polymorphic configuration.  Platform-specifics should derive
 * from this.
 */
class ConfigHandler
{
public:
	/**
	* @brief set string
	* @param name name of key to set
	* @param value string value to set
	 */
	void SetString(std::string name, std::string value);

	/**
	 * @brief get string
	 * @param name name of key to get
	 * @param def default string value to use if key is not found
	 * @return string value
	 */
	std::string GetString(std::string name, std::string def);
	
	template<typename T>
	void Set(const std::string& name, const T& value)
	{
		
		std::ostringstream buffer;
		buffer << value;
		SetString(name, buffer.str());
	}
	
	template<typename T>
	T Get(const std::string& name, const T& def)
	{
		std::map<std::string, std::string>::iterator pos = data.find(name);
		if (pos == data.end()) {
			Set(name, def);
			return def;
		}
		std::istringstream buf(pos->second);
		T temp;
		buf >> temp;
		return temp;
	}

	/**
	 * @brief instantiate global configHandler
	 * @return name of the configfile used
	 */
	static std::string Instantiate(std::string configSource);

	std::string GetConfigFile() const
	{
		return filename;
	}
	/**
	 * @brief deallocate
	 */
	static void Deallocate();

private:
	ConfigHandler(const std::string& configFile);
	~ConfigHandler();
	
	/**
	 * @brief config file name
	 */
	std::string filename;

	/**
	 * @brief data map
	 *
	 * Map used to internally cache data
	 * instead of constantly rereading from the file
	 */
	std::map<std::string, std::string> data;

	/**
	 * @brief Get the name of the default configuration file
	 */
	static std::string GetDefaultConfig();
	
	// helper functions
	void Read(FILE* file);
	void Write(FILE* file);
	char* Strip(char* begin, char* end);
	void AppendLine(char* buf);
};

extern ConfigHandler* _configHandler;

// quick hack so I don't have to change . to -> in the entire project
#define configHandler (*_configHandler)

#endif /* CONFIGHANDLER_H */
