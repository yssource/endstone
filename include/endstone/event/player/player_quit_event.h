// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * @brief Called when a player leaves a server.
 */
class PlayerQuitEvent : public PlayerEvent {
public:
    explicit PlayerQuitEvent(Player &player, std::string quit_message)
        : PlayerEvent(player), quit_message_(std::move(quit_message))
    {
    }
    ~PlayerQuitEvent() override = default;

    inline static const std::string NAME = "PlayerQuitEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    /**
     * @brief Gets the quit message to send to all online players.
     *
     * @return Message to appear to other players on the server.
     */
    [[nodiscard]] std::string getQuitMessage() const
    {
        return quit_message_;
    }

    /**
     * @brief Sets the quit message to send to all online players.
     *
     * @param message Message to appear to other players on the server.
     */
    void setQuitMessage(std::string message)
    {
        quit_message_ = std::move(message);
    }

private:
    std::string quit_message_;
};

}  // namespace endstone
