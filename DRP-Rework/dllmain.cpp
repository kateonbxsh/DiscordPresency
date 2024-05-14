// dllmain.cpp : Defines the entry point for the DLL application.

#include "discord-files/discord.h"
#include "iostream"

#define GM extern "C" __declspec(dllexport)

using namespace std;
using namespace discord;

struct DiscordState {
    std::unique_ptr<discord::Core> core;
};

Core* core{};
DiscordState state{};
Activity activity;

void (*CreateAsynEventWithDSMap)(int, int) = NULL;
int (*CreateDsMap)(int _num, ...) = NULL;
bool (*DsMapAddDouble)(int _index, const char* _pKey, double value) = NULL;
bool (*DsMapAddString)(int _index, const char* _pKey, const char* pVal) = NULL;

GM void RegisterCallbacks(char* arg1, char* arg2, char* arg3, char* arg4)
{
    void (*CreateAsynEventWithDSMapPtr)(int, int) = (void (*)(int, int))(arg1);
    int(*CreateDsMapPtr)(int _num, ...) = (int(*)(int _num, ...)) (arg2);
    CreateAsynEventWithDSMap = CreateAsynEventWithDSMapPtr;
    CreateDsMap = CreateDsMapPtr;

    bool (*DsMapAddDoublePtr)(int _index, const char* _pKey, double value) = (bool(*)(int, const char*, double))(arg3);
    bool (*DsMapAddStringPtr)(int _index, const char* _pKey, const char* pVal) = (bool(*)(int, const char*, const char*))(arg4);

    DsMapAddDouble = DsMapAddDoublePtr;
    DsMapAddString = DsMapAddStringPtr;

    OutputDebugString(TEXT("DiscordPresency: Register callbacks called"));
}


GM double InitDiscord(const char* AppID)
{
    int64_t id = (int64_t) strtoll(AppID, NULL, 10);
    auto result = Core::Create(id, DiscordCreateFlags_Default, &core);
    activity.SetType(ActivityType::Playing);
    state.core.reset(core);
     
    state.core->ActivityManager().OnActivityJoin.Connect([](const char* Secret) {
        
        OutputDebugString(TEXT("Someone tried to join :)"));
        
        });

    state.core->ActivityManager().OnActivityJoinRequest.Connect([](const User& u) {
        
        OutputDebugString(TEXT("BRO WANTS TO JOIN!!"));

        });

    return (state.core ? 1 : 0);
    
}

GM double step() {      

    state.core->RunCallbacks();

    return 1;

}



GM double CreateLobby(double maxSize, double type, double locked) {

    discord::Lobby lobby{};
    LobbyTransaction lobbyTransaction{};
    state.core->LobbyManager().GetLobbyCreateTransaction(&lobbyTransaction);
    lobbyTransaction.SetCapacity(10);
    lobbyTransaction.SetType(discord::LobbyType::Public);
    lobbyTransaction.SetLocked(false);
    DiscordState& locstate = state;
    Activity& locact = activity;
    Lobby& loclobby = lobby;

    state.core->LobbyManager().CreateLobby(
        lobbyTransaction, [&locstate, &locact, &loclobby](discord::Result result, discord::Lobby const& lobby_created) {
            loclobby = lobby_created;
            
            int dsmap = CreateDsMap(0);
            DsMapAddDouble(dsmap, "isDiscordCallback", 1);
            DsMapAddString(dsmap, "type", "lobbyCreate");
            DsMapAddDouble(dsmap, "success", (double)result);
            DsMapAddDouble(dsmap, "lobby.id", (double) loclobby.GetId());
            DsMapAddString(dsmap, "lobby.secret", loclobby.GetSecret());
            DsMapAddDouble(dsmap, "lobby.capacity", loclobby.GetCapacity());

            CreateAsynEventWithDSMap(dsmap, 70);

            locact.GetParty().SetId("gfdgdf");
            locact.GetParty().GetSize().SetMaxSize(10);
            locact.GetParty().GetSize().SetCurrentSize(1);
            locact.GetSecrets().SetJoin(loclobby.GetSecret());
            locstate.core->ActivityManager().UpdateActivity(locact, [](Result res) {});


        });

    return 1;

}

GM double UpdatePresence()
{
    state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {

        int dsmap = CreateDsMap(0);
        DsMapAddDouble(dsmap, "isDiscordCallback", 1);
        DsMapAddString(dsmap, "type", "updateActivity");
        DsMapAddDouble(dsmap, "success", (double) result);

        CreateAsynEventWithDSMap(dsmap, 70);

        });

    return 1;
}



GM double SetInfo(const char* details, const char* state) {

    activity.SetState(state);
    activity.SetDetails(details);

    return 1;
}



GM double SetImages(const char* smallImage, const char* largeImage, const char* smallText, const char* largeText) {

    activity.GetAssets().SetSmallImage(smallImage);
    activity.GetAssets().SetLargeImage(largeImage);
    activity.GetAssets().SetSmallText(smallText);
    activity.GetAssets().SetLargeText(largeText);
    
    return 1;

}

GM double SetEndTimestamp(double timestamp) {

    int64_t t = (int64_t)timestamp;
    activity.GetTimestamps().SetEnd(t);
    activity.GetTimestamps().SetStart(0);

    return 1;
}

GM double SetStartTimestamp(double timestamp) {

    int64_t t = (int64_t)timestamp;
    activity.GetTimestamps().SetEnd(0);
    activity.GetTimestamps().SetStart(t);

    return 1;

}

GM double TimestampNow() {

    return (double) time(0);

}

GM double SetSecret(const char* joinSec, const char* spectateSec, const char* matchSec) {
    
    activity.GetSecrets().SetJoin(joinSec);
    //activity.GetSecrets().SetSpectate(joinSec);
    //activity.GetSecrets().SetMatch(matchSec);

    return 1;

}



GM double SetParty(const char* partyId, double pSize, double pMax) {

    activity.GetParty().SetId(partyId);
    activity.GetParty().GetSize().SetCurrentSize((int32_t)pSize);
    activity.GetParty().GetSize().SetMaxSize((int32_t)pMax);

    return 1;

}