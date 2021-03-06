/* Author: Tobi Vollebregt */

#include "StdAfx.h"
#include "GlobalStuff.h"
#include "LogOutput.h"
#include "Messages.h"
#include "TdfParser.h"
#include "mmgr.h"
#include <algorithm>
#include <assert.h>
#include <locale>
#include <cctype>

CMessages::CMessages(): loaded(false) {}

/** Return a pointer to the single instance of CMessages. */
CMessages* CMessages::GetInstance()
{
	static CMessages instance;
	return &instance;
}

/** Load the messages from gamedata/messages.tdf into memory. */
void CMessages::Load()
{
	try {	
		TdfParser tdfparser("gamedata/messages.tdf");
		// Grab a list of messages.  Each message is one section.
		std::vector<std::string> section_list = tdfparser.GetSectionList("messages");
		// Load the possible translations for every message.
		for (std::vector<std::string>::const_iterator sit = section_list.begin(); sit != section_list.end(); ++sit) {
			const std::map<std::string, std::string>& allvalues = tdfparser.GetAllValues("messages\\" + *sit);
			for (std::map<std::string, std::string>::const_iterator it = allvalues.begin(); it != allvalues.end(); ++it) {
				tr[*sit].push_back(it->second);
			}
		}
	} catch (const TdfParser::parse_error& e) {
		// Show parse errors in the infolog.
		logOutput.Print("%s:%d: %s", e.get_filename().c_str(), e.get_line(), e.what());
	} catch (const content_error& e) {
		// Ignore non-existant file.
	}
	loaded = true;
}

/** Translate \p msg. If multiple messages are available it picks one at random.
Returns \p msg if no messages are available. */
std::string CMessages::Translate(const std::string& msg) const
{
	// TdfParser puts everything in lowercase.
	std::string lowerd = StringToLower(msg);
	message_map_t::const_iterator it = tr.find(lowerd);
	if (it == tr.end())
		return msg;
	return Pick(it->second);
}

/** Returns a message from \p vec at random. */
std::string CMessages::Pick(const message_list_t& vec) const
{
	assert(!vec.empty());
	if (vec.size() == 1)
		return vec[0];
	return vec[gu->usRandInt() % vec.size()];
}
