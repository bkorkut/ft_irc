#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
private:
    const int id;
    std::string nick;
    std::string username;
    std::string realname;
    std::string password;
    bool isAuthenticated;
    bool isRegistered;

public:
    User(void) : id(-1), isAuthenticated(false), isRegistered(false) {}
    User(int fd) : id(fd), isAuthenticated(false), isRegistered(false) {}

    // Getters
    const std::string& getNick() const { return nick; }
    const std::string& getUsername() const { return username; }
    const std::string& getRealname() const { return realname; }
    bool getIsAuthenticated() const { return isAuthenticated; }
    bool getIsRegistered() const { return isRegistered; }
    int getId() const { return id; }

    // Setters
    void setNick(std::string newNick) { nick = newNick; }
    void setUsername(std::string newUsername) { username = newUsername; }
    void setRealname(std::string newRealname) { realname = newRealname; }
    void setPassword(std::string newPassword) { password = newPassword; }
    
    // Authentication methods
    bool authenticate(const std::string& serverPassword) {
        if (password == serverPassword) {
            isAuthenticated = true;
            return true;
        }
        return false;
    }

    // Registration check
    bool checkRegistration() {
        if (!nick.empty() && !username.empty() && !realname.empty() && isAuthenticated) {
            isRegistered = true;
            return true;
        }
        return false;
    }
};

#endif