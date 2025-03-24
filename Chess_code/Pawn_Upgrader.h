#pragma once
#include "Team.h"
bool is_pawn(Piece* shouldbepawn);
TYPE get_valid_upgrade_type();
TYPE upgrade_pawn_if_needed(Pawn* pawn_to_upgrade, Team* team_owner, Board* mainboard, TYPE upgradeTo = EMPTY);
void really_perform_upgrade(Pawn* to_upgrade, TYPE new_class, Team* team_owner, Board* mainboard);
void place_upgraded_piece(Team* team_owner, Pawn* pawn_i_was, const char* newpiece_type, Piece* upgraded_piece, Board* mainboard);