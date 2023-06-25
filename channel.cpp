#include "Channel.hpp"

Channel::Channelstd::string channelName, std::string key, Client *client)
{
    this->name = channelName;
    this->topic = "";
    this->mode = "";
    this->key = "";
    this->inviteOnly = false;
    this->userLimit = -1;
    operators[0] = client->getName();
}

Channel::~Channel() {}

void Channel::addClient(std::string nickname)
{
    this->clients.push_back(nickname);
}

void Channel::removeClient(std::string nickname)
{
    for (int i = 0; i < this->clients.size(); i++) {
        if (this->clients[i] == nickname) {
            this->clients.erase(this->clients.begin() + i);
            break;
        }
    }
}

bool Channel::isClientInChannel(std::string nickname)
{
    for (int i = 0; i < this->clients.size(); i++) {
        if (this->clients[i] == nickname) {
            return true;
        }
    }
    return false;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

std::string Channel::getTopic()
{
    return this->topic;
}

void Channel::setMode(std::string mode)
{
    this->mode = mode;
}

std::string Channel::getMode()
{
    return this->mode;
}

void Channel::setKey(std::string key)
{
    this->key = key;
}

std::string Channel::getKey()
{
    return this->key;
}

void Channel::setInviteOnly(bool inviteOnly)
{
    this->inviteOnly = inviteOnly;
}

bool Channel::isInviteOnly()
{
    return this->inviteOnly;
}

void Channel::setOperator(std::string nickname, bool isOperator)
{
    if (isOperator) {
        this->operators.push_back(nickname);
    } else {
        for (int i = 0; i < this->operators.size(); i++) {
            if (this->operators[i] == nickname) {
                this->operators.erase(this->operators.begin() + i);
                break;
            }
        }
    }
}

bool Channel::isOperator(std::string nickname)
{
    for (int i = 0; i < this->operators.size(); i++) {
        if (this->operators[i] == nickname) {
            return true;
        }
    }
    return false;
}

void Channel::setUserLimit(int limit)
{
    this->userLimit = limit;
}

int Channel::getUserLimit()
{
    return this->userLimit;
}