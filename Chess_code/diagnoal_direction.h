#pragma once
enum DIAGONAL { UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
void calculate_diag_mv(int* row, int* column, DIAGONAL direction, int count);