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

#include "bedrock/entity/weak_entity_ref.h"
#include "endstone/actor/actor.h"
#include "endstone/core/permissions/permissible_base.h"

class Actor;

namespace endstone::core {

class EndstoneLevel;
class EndstoneServer;
class EndstoneActor;

struct EndstoneActorComponent {
    std::shared_ptr<EndstoneActor> actor;
};

class EndstoneActor : public Actor, public std::enable_shared_from_this<EndstoneActor> {
protected:
    EndstoneActor(EndstoneServer &server, ::Actor &actor);

public:
    // CommandSender
    void sendMessage(const Message &message) const override;
    void sendErrorMessage(const Message &message) const override;
    [[nodiscard]] Server &getServer() const override;
    [[nodiscard]] std::string getName() const override;

    // Permissible
    [[nodiscard]] bool isPermissionSet(std::string name) const override;
    [[nodiscard]] bool isPermissionSet(const Permission &perm) const override;
    [[nodiscard]] bool hasPermission(std::string name) const override;
    [[nodiscard]] bool hasPermission(const Permission &perm) const override;
    Result<PermissionAttachment *> addAttachment(Plugin &plugin, const std::string &name, bool value) override;
    Result<PermissionAttachment *> addAttachment(Plugin &plugin) override;
    Result<void> removeAttachment(PermissionAttachment &attachment) override;
    void recalculatePermissions() override;
    [[nodiscard]] std::unordered_set<PermissionAttachmentInfo *> getEffectivePermissions() const override;
    [[nodiscard]] bool isOp() const override;
    void setOp(bool value) override;

    // Actor
    [[nodiscard]] std::string getType() const override;
    [[nodiscard]] std::uint64_t getRuntimeId() const override;
    [[nodiscard]] Location getLocation() const override;
    [[nodiscard]] Vector<float> getVelocity() const override;
    [[nodiscard]] bool isOnGround() const override;
    [[nodiscard]] bool isInWater() const override;
    [[nodiscard]] bool isInLava() const override;
    [[nodiscard]] Level &getLevel() const override;
    [[nodiscard]] Dimension &getDimension() const override;
    void setRotation(float yaw, float pitch) override;
    void teleport(Location location) override;
    void teleport(Actor &target) override;
    [[nodiscard]] std::int64_t getId() const override;
    void remove() override;
    [[nodiscard]] bool isDead() const override;
    [[nodiscard]] bool isValid() const override;
    [[nodiscard]] int getHealth() const override;
    [[nodiscard]] Result<void> setHealth(int health) const override;
    [[nodiscard]] int getMaxHealth() const override;
    [[nodiscard]] std::vector<std::string> getScoreboardTags() const override;
    [[nodiscard]] bool addScoreboardTag(std::string tag) const override;
    [[nodiscard]] bool removeScoreboardTag(std::string tag) const override;
    [[nodiscard]] bool isNameTagVisible() const override;
    void setNameTagVisible(bool visible) override;
    [[nodiscard]] bool isNameTagAlwaysVisible() const override;
    void setNameTagAlwaysVisible(bool visible) override;
    [[nodiscard]] std::string getNameTag() const override;
    void setNameTag(std::string name) override;
    [[nodiscard]] std::string getScoreTag() const override;
    void setScoreTag(std::string score) override;

    ::Actor &getActor() const;

    static std::shared_ptr<EndstoneActor> create(EndstoneServer &server, ::Actor &actor);

protected:
    template <typename T>
    T &getHandle() const
    {
        auto *ptr = actor_.tryUnwrap<T>(/*include_removed*/ true);
        if (!ptr) {
            throw std::runtime_error("Trying to access an actor that is no longer valid.");
        }
        return *ptr;
    }

private:
protected:
    EndstoneServer &server_;

private:
    WeakEntityRef actor_;
    static PermissibleBase &getPermissibleBase();
};

}  // namespace endstone::core
