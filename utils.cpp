#include <vector>
#include <string>

// Converts string toLower
std::string	toLower(std::string str)
{
	for(std::string::iterator c = str.begin(); c != str.end(); c++)
		*c = std::tolower(*c);
	return str;
}

// Split using vectors in C++
std::vector<std::string>	vecSplit(std::string toSplit, std::string septor)
{
	size_t						pos = 0;
	size_t						foundPos;
	std::string					tmp;
	std::vector<std::string>	retval;

	do
	{
		foundPos = toSplit.find(septor, pos);
		if (foundPos != std::string::npos)
			retval.push_back(toSplit.substr(pos, foundPos - pos));
		else
			retval.push_back(toSplit.substr(pos, toSplit.size()));
		pos = foundPos + septor.size();
	} while (foundPos != std::string::npos);
	return (retval);
}
