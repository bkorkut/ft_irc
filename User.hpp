#ifndef USER_HPP
#define USER_HPP

# include <string>

typedef unsigned char uint8_t;  // C++98 uyumlu tanım
typedef unsigned short uint16_t;  // C++98 uyumlu tanım

enum UserModes
{
	INVISIBLE	= 'i',
	SERVNOTICE	= 's',
	WALLOPS		= 'w',
	OPERATOR	= 'o',
	AWAY		= 'a',
	RESTRICTED	= 'r'
};

enum	UserFlags
{
	B_INVISIBLE		= 1 << 0,	// (00000001) marks a users as invisible
	B_SERVNOTICE	= 1 << 1,	// (00000010) marks a user for receipt of server notices
	B_WALLOPS		= 1 << 2,	// (00000100) user receives wallops
	B_OPERATOR		= 1 << 3,	// (00001000) operator flag
	B_AWAY			= 1 << 4,	// (00010000) user is flagged as away
	B_RESTRICTED	= 1 << 5	// (00100000) restricted user connection
};

class User {
private:
    const int   id;
    std::string nick;
    std::string username;
    std::string realname;
    std::string password;
    bool        isAuthenticated;
    bool        isRegistered;
    uint8_t     modeFlags;

public:
    std::string buffer;

    User(void);
    User(int fd);

    // Getters
    const std::string&  getNick() const;
    const std::string&  getUsername() const;
    const std::string&  getRealname() const;
    bool               getIsAuthenticated() const;
    bool               getIsRegistered() const;
    int                getId() const;
    uint8_t            getFlags(UserFlags flag);
    bool               hasFlag(UserFlags flag);

    // Setters
    void               setNick(std::string newNick);
    void               setUsername(std::string newUsername);
    void               setRealname(std::string newRealname);
    void               setPassword(std::string newPassword);
    void               setFlag(UserFlags flag);
    void               unsetFlag(UserFlags flag);

    // Authentication methods
    bool               authenticate(const std::string& serverPassword);
    bool               checkRegistration();
};

#endif
