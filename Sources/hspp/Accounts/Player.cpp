// Copyright (c) 2018 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <hspp/Accounts/Player.h>
#include <hspp/Commons/Constants.h>
#include <hspp/Tasks/TaskMeta.h>

namespace Hearthstonepp
{
Player::Player(const Account* account, const Deck* deck)
    : id(USER_INVALID), email(account->GetEmail())
{
    Cards* cardsInstance = Cards::GetInstance();
    const CardClass cardclass = deck->GetClass();

    const Card* heroCard = cardsInstance->GetHeroCard(cardclass);
    if (heroCard != nullptr)
    {
        hero = new Hero(heroCard);
    }

    const Card* powerCard = cardsInstance->GetDefaultHeroPower(cardclass);
    if (powerCard != nullptr)
    {
        power = new HeroPower(powerCard);
    }

    for (auto& card : deck->GetPrimitiveDeck())
    {
        if (card == nullptr)
        {
            continue;
        }

        Entity* entity = nullptr;
        switch (card->cardType)
        {
            case CardType::MINION:
                entity = new Minion(card);
                break;
            case CardType::WEAPON:
                entity = new Weapon(card);
                break;
            default:
                entity = new Entity(card);
                break;
        }

        cards.emplace_back(entity);
    }
}

Player::~Player()
{
    for (auto& card : cards)
    {
        delete card;
    }
    cards.clear();

    for (auto& minion : field)
    {
        delete minion;
    }
    field.clear();

    for (auto& card : hand)
    {
        delete card;
    }
    hand.clear();

    for (auto& spell : usedSpell)
    {
        delete spell;
    }
    usedSpell.clear();

    for (auto& minion : usedMinion)
    {
        delete minion;
    }
    usedMinion.clear();

    delete hero;
    delete power;
}

Player::Player(const Player& p)
{
    *this = p;
}

Player::Player(Player&& p) noexcept
{
    *this = std::move(p);
}

Player& Player::operator=(const Player& p)
{
    if (this == &p)
    {
        return *this;
    }

    for (auto& card : cards)
    {
        delete card;
    }
    cards.clear();

    for (auto& minion : field)
    {
        delete minion;
    }
    field.clear();

    for (auto& card : hand)
    {
        delete card;
    }
    hand.clear();

    for (auto& spell : usedSpell)
    {
        delete spell;
    }
    usedSpell.clear();

    for (auto& minion : usedMinion)
    {
        delete minion;
    }
    usedMinion.clear();

    delete hero;
    delete power;

    if (p.hero != nullptr)
    {
        hero = p.hero->Clone();        
    }
    if (p.power != nullptr)
    {
        power = p.power->Clone();        
    }

    field.resize(p.field.size());
    std::transform(p.field.begin(), p.field.end(), field.begin(),
                   [](Character* c) -> Character* { return c->Clone(); });

    hand.resize(p.hand.size());
    std::transform(p.hand.begin(), p.hand.end(), hand.begin(),
                   [](Entity* e) -> Entity* { return e->Clone(); });

    usedSpell.resize(p.usedSpell.size());
    std::transform(p.usedSpell.begin(), p.usedSpell.end(), usedSpell.begin(),
                   [](Spell* s) -> Spell* { return s->Clone(); });

    usedMinion.resize(p.usedMinion.size());
    std::transform(p.usedMinion.begin(), p.usedMinion.end(), usedMinion.begin(),
                   [](Character* c) -> Character* { return c->Clone(); });

    return *this;
}

Player& Player::operator=(Player&& p) noexcept
{
    if (this == &p)
    {
        return *this;
    }

    hero = p.hero;
    p.hero = nullptr;
    power = p.power;
    p.power = nullptr;

    field = std::move(p.field);
    hand = std::move(p.hand);
    usedSpell = std::move(p.usedSpell);
    usedMinion = std::move(p.usedMinion);

    return *this;
}
}  // namespace Hearthstonepp
