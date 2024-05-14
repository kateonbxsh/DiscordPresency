enum eAchievementMessages{
    
    e_achievement_our_info=1002,
    e_achievement_friends_info=1003,
    e_achievement_leaderboard_info=1004,
    e_achievement_achievement_info=1005,
    e_achievement_pic_loaded=1006,
	e_achievement_challenge_completed=1007,
	e_achievement_challenge_completed_by_remote=1008,
	e_achievement_challenge_received=1009,
	e_achievement_challenge_list_received=1010,
	e_achievement_challenge_launched=1011,
	e_achievement_player_info=1012,
	e_achievement_purchase_info=1013
};
enum eAchievementShowTypes{
    
	e_achievement_show_ui=0,
    e_achievement_show_profile,
    e_achievement_show_leaderboard,
    e_achievement_show_achievement,
	e_achievement_show_bank,
	e_achievement_show_friend_picker,
	e_achievement_show_purchase_prompt
};


const int EVENT_OTHER_SOCIAL = 70;
extern UIView *g_glView;

extern "C" NSString* findOption( const char* _key );
extern bool F_DsMapAdd_Internal(int _index, char* _pKey, double _value);
extern bool F_DsMapAdd_Internal(int _index, char* _pKey, char* _pValue);
extern int CreateDsMap( int _num, ... );
extern void CreateAsynEventWithDSMap(int dsmapindex, int event_index);





