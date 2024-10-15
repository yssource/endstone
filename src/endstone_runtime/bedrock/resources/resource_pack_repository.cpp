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

#include "bedrock/resources/resource_pack_repository.h"

#include "endstone/detail/hook.h"
#include "endstone/detail/packs/endstone_pack_source.h"

void ResourcePackRepository::_initializePackSource()
{
    static endstone::detail::EndstonePackSource endstone_pack_source{getResourcePacksPath().getContainer(),
                                                                     PackType::Resources};
    ENDSTONE_HOOK_CALL_ORIGINAL(&ResourcePackRepository::_initializePackSource, this);
    pack_source_->addPackSource(&endstone_pack_source);
}
