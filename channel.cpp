/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkaratae <dkaratae@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:41:28 by dkaratae          #+#    #+#             */
/*   Updated: 2023/06/25 17:50:10 by dkaratae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

std::string Channel::getTopic()
{
    return this->topic;
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

void Channel::setOperator(std::string nickName, bool isOperator)
{
    if (isOperator)
    {
        this->operators.push_back(nickName);
    }
    else
    {
        for (int i = 0; i < this->operators.size(); i++)
        {
            if (this->operators[i] == nickName)
            {
                this->operators.erase(this->operators.begin() + i);
                break;
            }
        }
    }
}

bool Channel::isOperator(std::string nickName)
{
    for (int i = 0; i < this->operators.size(); i++)
    {
        if (this->operators[i] == nickName)
            return true;
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

void Channel::addClient(std::string nickName)
{
    this->clients.push_back(nickName);
}

void Channel::removeClient(std::string nickName)
{
    for (int i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i] == nickName)
        {
            this->clients.erase(this->clients.begin() + i);
            break;
        }
    }
}

bool Channel::isClientInChannel(std::string nickName)
{
    for (int i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i] == nickName)
            return true;
    }
    return false;
}