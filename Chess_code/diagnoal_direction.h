#pragma once
enum DIAGONAL { UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
void move_position_info_diag(int* row, int* column, DIAGONAL direction, int count);