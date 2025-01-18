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
    // Constructors - sadece deklarasyon
    User(void);
    User(int fd);

    // Getters
    const std::string& getNick() const;
    const std::string& getUsername() const;
    const std::string& getRealname() const;
    bool getIsAuthenticated() const;
    bool getIsRegistered() const;
    int getId() const;

    // Setters
    void setNick(std::string newNick);
    void setUsername(std::string newUsername);
    void setRealname(std::string newRealname);
    void setPassword(std::string newPassword);
    
    // Authentication methods
    bool authenticate(const std::string& serverPassword);
    bool checkRegistration();
};

#endif