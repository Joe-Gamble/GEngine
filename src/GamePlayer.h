#pragma once
#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

class GameTeam;

class GamePlayer
{
public:
	GamePlayer(GameTeam& team);
	inline GameTeam* GetTeam() { return team; }
private:
	GameTeam* team = nullptr;
};
#endif
