#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>

class Channel {
public:
    Channel(std::string name);
    ~Channel();

    void addClient(std::string nickname);
    void removeClient(std::string nickname);
    bool isClientInChannel(std::string nickname);

    void setTopic(std::string topic);
    std::string getTopic();

    void setKey(std::string key);
    std::string getKey();

    void setInviteOnly(bool inviteOnly);
    bool isInviteOnly();

    void setOperator(std::string nickname, bool isOperator);
    bool isOperator(std::string nickname);

    void setUserLimit(int limit);
    int getUserLimit();

private:
    std::string name;
    std::string topic;
    std::string mode;
    std::string key;
    bool inviteOnly;
    std::vector<std::string> clients;
    std::vector<std::string> operators;
    int userLimit;
};

#endif