// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Rosetta/Policies/BasicPolicy.hpp>

namespace RosettaStone
{
TaskMeta BasicPolicy::Next(const Game& game)
{
    (void)game;
    return TaskMeta();
}

TaskMeta BasicPolicy::Require(Player& player, TaskID id)
{
    if (auto iter = m_require.find(id); iter != m_require.end())
    {
        return iter->second(*this, player);
    }
    return TaskMeta(id);
}

void BasicPolicy::Notify(const TaskMeta& meta)
{
    if (auto iter = m_notify.find(meta.GetID()); iter != m_notify.end())
    {
        return iter->second(*this, meta);
    }
}

TaskMeta BasicPolicy::RequireMulligan(Player& player)
{
    (void)player;
    return TaskMeta();
}

TaskMeta BasicPolicy::RequirePlayCard(Player& player)
{
    (void)player;
    return TaskMeta();
}

TaskMeta BasicPolicy::RequireAttack(Player& player)
{
    (void)player;
    return TaskMeta();
}

void BasicPolicy::NotifyOverDraw(const TaskMeta& meta)
{
    (void)meta;
}
}  // namespace RosettaStone
