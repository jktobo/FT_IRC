/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkaratae <dkaratae@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:41:22 by dkaratae          #+#    #+#             */
/*   Updated: 2023/06/25 17:44:53 by dkaratae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

class Channel
{
    public:
        Channel(std::string channelName, std::string key, Client *client);
        ~Channel();

        void setTopic(std::string topic);
        void setKey(std::string key);

        std::string getTopic();
        std::string getKey();

        void setInviteOnly(bool inviteOnly);
        bool isInviteOnly();

        void setOperator(std::string nickname, bool isOperator);
        bool isOperator(std::string nickName);

        void setUserLimit(int limit);
        int getUserLimit();

        void addClient(std::string nickName);
        void removeClient(std::string nickName); 
        bool isClientInChannel(std::string nickName);
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