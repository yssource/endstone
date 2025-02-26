

# File item\_stack.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**item\_stack.h**](item__stack_8h.md)

[Go to the documentation of this file](item__stack_8h.md)


```C++
#include <utility>

#include <fmt/format.h>

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

#include <memory>

namespace endstone {

namespace core {
class EndstoneItemStack;
}

class ItemStack : public std::enable_shared_from_this<ItemStack> {
public:
    ItemStack() = default;
    explicit ItemStack(std::string type, int amount = 1) : type_(std::move(type)), amount_(amount) {}

    virtual ~ItemStack() = default;

    [[nodiscard]] virtual std::string getType() const
    {
        return type_;
    }

    virtual void setType(std::string type)
    {
        // TODO(item): clear item components when the type is changed
        type_ = std::move(type);
    }

    [[nodiscard]] virtual int getAmount() const
    {
        return amount_;
    }

    virtual void setAmount(int amount)
    {
        amount_ = amount;
    }

protected:
    friend class core::EndstoneItemStack;

    virtual const core::EndstoneItemStack *asEndstoneItemStack() const
    {
        return nullptr;
    }

    virtual core::EndstoneItemStack *asEndstoneItemStack()
    {
        return nullptr;
    }

private:
    std::string type_ = "minecraft:air";
    int amount_ = 0;
};

}  // namespace endstone

namespace fmt {
template <>
struct formatter<endstone::ItemStack> : formatter<string_view> {
    using Type = endstone::ItemStack;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "ItemStack({} x {})", val.getType(), val.getAmount());
    }
};
}  // namespace fmt
```


