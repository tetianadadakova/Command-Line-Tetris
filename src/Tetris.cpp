
/*
 
 The following code is a derivative work of the code from Javidx9 (http://www.onelonecoder.com/) which is licensed GPLv3. This code therefore is also licensed under the terms of the GPLv3 license.
 
The original code here: https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_Tetris.cpp
The corresponding YouTube tutorial "Code-It-Yourself! Tetris - Programming from Scratch (Quick and Simple C++)" here: https://www.youtube.com/watch?v=8OK8_tHeCIA&t=1806s
 In addition, I used the following pull request for the example use of ncurses: https://github.com/OneLoneCoder/CommandLineFPS/pull/1, specifically the following code: https://github.com/rohanliston/CommandLineFPS/blob/linux-port/linux/CommandLineFPS.cpp
 
 */


#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <thread>
#include <vector>


// ===== Global vars =====

// Tetromino
std::wstring tetromino[7];

// Tetris field
int n_field_width = 12; // field columns
int n_field_height = 18; // field rows
// p_field will be allocated dynamically, 0 - empty space, 1-7 - different tetrominos shapes, 9 - border/boundary walls
unsigned char* p_field = nullptr; // to store all the map imformation

// Screen
int n_screen_width = 80; // columns
int n_screen_height = 30; // rows


// ===== Global functions =====

int rotate(int px, int py, int r) {
    
    // Returns an index in tetromino array after rotation for each case
    
    switch(r % 4) {
        case 0: return py * 4 + px;        // r % 4 == 0: 0 degrees rotation
        case 1: return 12 + py - (px * 4); // r % 4 == 1: 90 degrees
        case 2: return 15 - (py * 4) - px; // r % 4 == 2: 180 degrees
        case 3: return 3 - py + (px * 4);  // r % 4 == 3: 270 degrees
    }
    
    return 0;
}

bool does_piece_fit(int n_tetromino, int n_rotation, int n_pos_x, int n_pos_y) {
    
    // All Field cells >0 are occupied
    for (int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++) {
            // Get index into piece
            int pi = rotate(px, py, n_rotation);

            // Get index into field
            int fi = (n_pos_y + py) * n_field_width + (n_pos_x + px);

            // Check that test is in bounds. Note out of bounds does
            // not necessarily mean a fail, as the long vertical piece
            // can have cells that lie outside the boundary, so we'll
            // just ignore them
            if (n_pos_x + px >= 0 && n_pos_x + px < n_field_width) {
                if (n_pos_y + py >= 0 && n_pos_y + py < n_field_height) {
                    // In Bounds so do collision check
                    // p_field[fi] != 0 - means there is already something in the field
                    if (tetromino[n_tetromino][pi] != L'.' && p_field[fi] != 0) // or if (tetromino[n_tetromino][pi] == L'X'
                        return false; // fail on first hit
                }
            }
        }
    
    return true;
}

int main() {
        
    // Initialize ncurses screen
    initscr();
    getmaxyx(stdscr, n_screen_height, n_screen_width);
    noecho();
    curs_set(0); // invisible cursor
    timeout(0); // for non-blocking getch()
    keypad(stdscr,TRUE); // to enable KEY_LEFT, KEY_RIGHT, etc
    
    
    // Tetrominos
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    
    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");
    
    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");
    
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");
    tetromino[3].append(L"....");
    
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"....");
    
    tetromino[5].append(L".X..");
    tetromino[5].append(L".X..");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"....");
    
    tetromino[6].append(L"..X.");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L"..X.");
    tetromino[6].append(L"....");
    
    // Field
    p_field = new unsigned char[n_field_width * n_field_height]; // play field buffer
    for (int x = 0; x < n_field_width; x++)
        for (int y = 0; y < n_field_height; y++)
            p_field[y * n_field_width + x] = (x == 0 || x == n_field_width - 1 || y == n_field_height - 1) ? 9 : 0;
    
    
    // ===== Game logic =====
    
    // Tetromino
    int n_current_piece = 3; // first tetromino piece
    int n_current_rotation = 0;
    int n_current_x = n_field_width / 2; // near the middle of the playing field
    int n_current_y = 0; // at the top of the playing field
    
    // Timing
    int n_speed = 20;
    int n_speed_count = 0;
    bool b_force_down = false;
    int piece_count = 0;
    int score = 0;
    
    // To collect the full lines (rows indexes)
    std::vector<int> lines;
    
    // Create Screen Buffer
    auto *screen = new wchar_t[n_screen_width * n_screen_height];
    for (int i = 0; i < n_screen_width * n_screen_height; i++) screen[i] = L' ';
    
    bool game_over = false; // !game_over
    while(!game_over) { // Main game loop
        
        // Timing ============================================================
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Small Step = 1 Game Tick
        n_speed_count++;
        b_force_down = (n_speed_count == n_speed); // move down every n_speed steps
        
        // User input ========================================================
        int key = getch();
        
        

        // Game logic ========================================================
        // e.g. shapes falling, collision detection, scoring
        
        // Handle player movement
        // if key == -1 - do nothing, tetromino keeps falling
        if (key != -1) {
            switch(key) {
                case KEY_LEFT:   // Move left
                    if(does_piece_fit(n_current_piece, n_current_rotation, n_current_x - 1, n_current_y)) {
                        n_current_x--;
                    }
                    break;
                
                case KEY_RIGHT: // Move right
                    if(does_piece_fit(n_current_piece, n_current_rotation, n_current_x + 1, n_current_y)) {
                        n_current_x++;
                    }
                    break;
                
                case KEY_DOWN: // Move down
                    if(does_piece_fit(n_current_piece, n_current_rotation, n_current_x, n_current_y + 1)) {
                        n_current_y++;
                    }
                    break;
                
                case ' ': // Rotate (Space)
                    if(does_piece_fit(n_current_piece, n_current_rotation + 1, n_current_x, n_current_y)) {
                        n_current_rotation++;
                    }
                    break;
            
                default: // Any key to exit
                    game_over = true;
                    break;
            }
        }
        
        // Force the piece down the playfield if it's time
        if (b_force_down) {
            if (does_piece_fit(n_current_piece, n_current_rotation, n_current_x, n_current_y + 1)) {
                // if it fits - keep going
                n_current_y++;
            } else {
                // Lock the piece in the field
                // It can't! Lock the piece in place
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (tetromino[n_current_piece][rotate(px, py, n_current_rotation)] != L'.')
                            p_field[(n_current_y + py) * n_field_width + (n_current_x + px)] = n_current_piece + 1;
                
                // Increase speed after every 10 pieces
                piece_count++;
                if (piece_count % 10 == 0)
                    if (n_speed >= 10)
                        n_speed--;
                
                // Have we created any horizontail lines
                for (int py = 0; py < 4; py++)
                if(n_current_y + py < n_field_height - 1) {
                    bool is_line = true; // assume the line is full
                    for (int px = 1; px < n_field_width - 1; px++)
                        is_line &= (p_field[(n_current_y + py) * n_field_width + px]) != 0;

                    if (is_line)
                    {
                        // Remove Line, set to '='
                        for (int px = 1; px < n_field_width - 1; px++)
                            p_field[(n_current_y + py) * n_field_width + px] = 8;
                        lines.push_back(n_current_y + py);
                    }
                }
                
                score += 25; // increase score for every tetromino
                if(!lines.empty())
                    score += (1 << lines.size()) * 100; // 100 points for every line
                
                // Choose next piece
                n_current_piece = rand() % 7;
                n_current_rotation = 0;
                n_current_x = n_field_width / 2;
                n_current_y = 0;
                
                // If the piece doesn't fit = game over
                if(!does_piece_fit(n_current_piece, n_current_rotation, n_current_x, n_current_y)) {
                    game_over = true;
                }
            }
            n_speed_count = 0;
        }
        
        // Display ===========================================================
        
        // Draw field
        for (int x = 0; x < n_field_width; x++) {
            for (int y = 0; y < n_field_height; y++) {
                // e.g. if p_field[ind] = 0, it will be represented by empty space on the screen
                // if p_field[ind] = 1-7 (one of the tetrominos), it will be represented by one of the letters ABCDEFG
                // if p_field[ind] = 8 (completed line), it will be represented by =
                // if p_field[ind] = 9 (border), it will be represented by #
                screen[(y + 2) * n_screen_width + (x + 2)] = L" ABCDEFG=#"[p_field[y * n_field_width + x]];
            }
        }

        // Draw current piece
        for (int px = 0; px < 4; px++) {
            for (int py = 0; py < 4; py++) {
                if (tetromino[n_current_piece][rotate(px, py, n_current_rotation)] != L'.') {
                    screen[(n_current_y + py + 2) * n_screen_width + (n_current_x + px + 2)] = n_current_piece + 65;
                    // e.g.
                    // for tetromino[0]: n_current_piece + 65 = 0 + 65 = 'A'
                    // for tetromino[1]: n_current_piece + 65 = 1 + 65 = 'B', etc
                }
            }
        }
        
        // Animate line completion
        if (!lines.empty()) {
            
            // Display Frame (cheekily to draw lines)
            mvaddwstr(0, 0, screen);
            refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(400)); // Delay a bit
            
            for (auto &el : lines)
                for (int px = 1; px < n_field_width - 1; px++)
                {
                    for (int py = el; py > 0; py--)
                        // move the pieces above the line down
                        p_field[py * n_field_width + px] = p_field[(py - 1) * n_field_width + px];
                    p_field[px] = 0;
                }

            lines.clear();
        }

        // Display Frame
        mvaddwstr(0, 0, screen);
        
        // Display score
        wchar_t stats[40];
        swprintf(stats, 40, L"SCORE: %8d", score);
        mvaddwstr(0, 0, stats);
        refresh();
        
    }
    
    // Game over
    
    endwin(); // clean up all allocated resources from ncurses and restore the terminal
    
    std::cout << "Game Over!! Score:" << score << std::endl;
    system("pause");
    
    return 0;
}


