// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Rosetta/Commons/Macros.hpp>
#include <Rosetta/Loaders/AccountLoader.hpp>

#if defined(ROSETTASTONE_WINDOWS)
#include <filesystem>
#elif defined(ROSETTASTONE_LINUX)
#include <experimental/filesystem>
#elif defined(ROSETTASTONE_MACOSX)
#include <stdlib.h>
#endif
#include <fstream>
#include <iomanip>
#include <iostream>

#if defined(ROSETTASTONE_WINDOWS)
namespace filesystem = std::filesystem;
#elif defined(ROSETTASTONE_LINUX)
namespace filesystem = std::experimental::filesystem;
#endif

namespace RosettaStone
{
AccountInfo* AccountLoader::Load(std::string email) const
{
    // Read account data from JSON file
    std::ifstream playerFile("Datas/" + email + ".json");
    nlohmann::json j;

    if (!playerFile.is_open())
    {
        return nullptr;
    }

    AccountInfo* account;

    try
    {
        playerFile >> j;

        auto nickname = j["nickname"].get<std::string>();

        std::vector<DeckInfo*> decks;
        decks.reserve(j["decks"].size());

        if (!j["decks"].is_null())
        {
            for (auto& deck : j["decks"])
            {
                const CardClass deckClass = CardClass::_from_string(
                    deck["class"].get<std::string>().c_str());
                const std::string deckName = deck["name"].get<std::string>();

                DeckInfo* d = new DeckInfo(deckName, deckClass);
                for (auto& card : deck["cards"])
                {
                    const std::string cardID = card["id"].get<std::string>();
                    const int numOfCard = card["num"].get<int>();

                    d->AddCard(cardID, numOfCard);
                }

                decks.emplace_back(d);
            }
        }

        account = new AccountInfo(std::move(email), std::move(nickname), decks);
    }
    catch (...)
    {
        playerFile.close();

        return nullptr;
    }

    playerFile.close();

    return account;
}

void AccountLoader::Save(AccountInfo* account) const
{
    // Store account data to JSON file
#if defined(ROSETTASTONE_WINDOWS) || defined(ROSETTASTONE_LINUX)
    filesystem::create_directory("Datas");
#elif defined(ROSETTASTONE_MACOSX)
    system("mkdir Datas");
#endif
    std::ofstream playerFile("Datas/" + account->GetEmail() + ".json");

    nlohmann::json j;

    if (!playerFile.is_open())
    {
        std::cout << "An error occurred while saving player data.\n";
    }

    try
    {
        j["nickname"] = account->GetNickname();

        j["decks"] = nlohmann::json::array();

        for (std::size_t deckIdx = 0; deckIdx < account->GetNumOfDeck();
             ++deckIdx)
        {
            j["decks"].emplace_back(nlohmann::json::object(
                { { "class",
                    account->GetDeck(deckIdx)->GetClass()._to_string() },
                  { "name", account->GetDeck(deckIdx)->GetName() },
                  { "cards", nlohmann::json::array() } }));

            for (std::size_t cardIdx = 0;
                 cardIdx < account->GetDeck(deckIdx)->GetUniqueNumOfCards();
                 ++cardIdx)
            {
                j["decks"].at(deckIdx)["cards"].emplace_back(
                    nlohmann::json::object(
                        { { "id",
                            account->GetDeck(deckIdx)->GetCard(cardIdx).first },
                          { "num", account->GetDeck(deckIdx)
                                       ->GetCard(cardIdx)
                                       .second } }));
            }
        }

        playerFile << std::setw(4) << j << "\n";
    }
    catch (...)
    {
        std::cout << "An error occurred while saving player data.\n";

        playerFile.close();

        return;
    }

    playerFile.close();
}
}  // namespace RosettaStone
