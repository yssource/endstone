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

#include <any>
#include <map>
#include <string>

#include "endstone/command/command.h"
#include "endstone/command/command_sender.h"

namespace endstone {
/**
 * Represents a class which contains a single method for executing commands
 */
class CommandExecutor {
public:
    virtual ~CommandExecutor() noexcept = default;

    /**
     * Executes the given command, returning its success.
     *
     * @param sender Source of the command
     * @param command Command which was executed
     * @param label Alias of the command which was used
     * @param args Passed command arguments
     * @return true if a valid command, otherwise false
     */
    virtual bool onCommand(const CommandSender &sender, const Command &command, const std::string &label,
                           const std::map<std::string, std::string> &args) = 0;
};
}  // namespace endstone
