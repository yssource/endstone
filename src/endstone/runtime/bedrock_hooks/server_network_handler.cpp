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

#include "bedrock/network/server_network_handler.h"

#include <variant>

#include <entt/entt.hpp>
#include <magic_enum/magic_enum.hpp>

#include "bedrock/locale/i18n.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_kick_event.h"
#include "endstone/event/player/player_login_event.h"
#include "endstone/runtime/hook.h"

using endstone::core::EndstonePlayer;
using endstone::core::EndstoneServer;

IncomingPacketFilterResult ServerNetworkHandler::allowIncomingPacketId(const NetworkIdentifierWithSubId &sender,
                                                                       MinecraftPacketIds packet_id,
                                                                       std::size_t packet_size)
{
    // TODO: DataPacketReceiveEvent, the payload is available in network_.receive_buffer_
    return ENDSTONE_HOOK_CALL_ORIGINAL(&NetEventCallback::allowIncomingPacketId, this, sender, packet_id, packet_size);
}

OutgoingPacketFilterResult ServerNetworkHandler::allowOutgoingPacket(const std::vector<NetworkIdentifierWithSubId> &ids,
                                                                     const Packet &packet)
{
    // TODO: DataPacketSendEvent, the payload is available in network_.send_stream_
    return ENDSTONE_HOOK_CALL_ORIGINAL(&NetEventCallback::allowOutgoingPacket, this, ids, packet);
}

void ServerNetworkHandler::disconnectClient(const NetworkIdentifier &network_id, SubClientId sub_client_id,
                                            Connection::DisconnectFailReason reason, const std::string &message,
                                            std::optional<std::string> filtered_message, bool skip_message)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto disconnect_message = message;
    if (auto *endstone_player = server.getPlayer(network_id, sub_client_id)) {
        auto kick_message = getI18n().get(message, nullptr);
        endstone::PlayerKickEvent e{*endstone_player, kick_message};
        server.getPluginManager().callEvent(e);

        if (e.isCancelled()) {
            return;
        }

        if (e.getReason() != disconnect_message) {
            disconnect_message = e.getReason();
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::disconnectClient, this, network_id, sub_client_id, reason,
                                disconnect_message, std::move(filtered_message), skip_message);
}

bool ServerNetworkHandler::trytLoadPlayer(ServerPlayer &server_player, const ConnectionRequest &connection_request)
{
    const auto new_player =
        ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::trytLoadPlayer, this, server_player, connection_request);
    const auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = server_player.getEndstoneActor<EndstonePlayer>();
    endstone_player.initFromConnectionRequest(&connection_request);

    if (server.getBanList().isBanned(endstone_player.getName(), endstone_player.getUniqueId(),
                                     endstone_player.getXuid())) {
        endstone_player.kick("You have been banned from this server.");
        return new_player;
    }

    if (server.getIpBanList().isBanned(endstone_player.getAddress().getHostname())) {
        endstone_player.kick("You have been IP banned from this server.");
        return new_player;
    }

    endstone::PlayerLoginEvent e{endstone_player};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        endstone_player.kick(e.getKickMessage());
    }
    return new_player;
}

ServerPlayer &ServerNetworkHandler::_createNewPlayer(const NetworkIdentifier &network_id,
                                                     const SubClientConnectionRequest &sub_client_connection_request,
                                                     SubClientId sub_client_id)
{
    auto &server_player = ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_createNewPlayer, this, network_id,
                                                      sub_client_connection_request, sub_client_id);
    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = server_player.getEndstoneActor<EndstonePlayer>();
    endstone_player.initFromConnectionRequest(&sub_client_connection_request);

    if (server.getBanList().isBanned(endstone_player.getName(), endstone_player.getUniqueId(),
                                     endstone_player.getXuid())) {
        endstone_player.kick("You have been banned from this server.");
        return server_player;
    }

    if (server.getIpBanList().isBanned(endstone_player.getAddress().getHostname())) {
        endstone_player.kick("You have been IP banned from this server.");
        return server_player;
    }

    endstone::PlayerLoginEvent e{endstone_player};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        endstone_player.kick(e.getKickMessage());
    }
    return server_player;
}

bool ServerNetworkHandler::_isServerTextEnabled(ServerTextEvent const &event) const
{
    if (event == ServerTextEvent::Connection) {
        // Note: return false to prevent BDS from sending the join/quit message
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_isServerTextEnabled, this, event);
}
