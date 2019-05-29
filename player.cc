#include <ncurses.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <random>

#include <cstring>

//---------------------------------------------------------
// xy[0] = y ; [1] = x

class Thing {
	public:
		int x, y;
	protected:
		int generate_random() {
			struct timeval t;
			gettimeofday(&t, NULL);
			pid_t pid = getpid();
			srand(t.tv_usec * t.tv_sec * pid);

			return rand() % 5;
		}

		//---------------------------------------------------------

		int itox(int x) noexcept {
			if (x == 0) {
				return 1;
				
			}
			if (x == 1) {
				return 5;
			}
			if (x == 2) {
				return 9;
			}
			if (x == 3) {
				return 13;
			}
			if (x == 4) {
				return 17;
			}
			return 0;
		}

		//-----------------------------------------------------

		Thing() {
			this->x = generate_random();
			this->y = generate_random();
		}
};

//---------------------------------------------------------
// define player class (asterisk)

class Player : public Thing {
	public:
		void draw_player() {
			mvprintw(y * 2, this->itox(x), "*"); 
		}

		//-----------------------------------------------------

		// 1 = top
		// 2 = right
		// 3 = bottom
		// 4 = left
		void move(int dir) {
			switch (dir) {
				// top
				case 1:
					if (y != 0) {
						y--;
					}
					break;
				// right
				case 2:
					if (x != 4) {
						x++;
					}
					break;
				// bottom
				case 3:
					if (y != 4) {
						y++;
					}
					break;
				// left
				case 4:
					if (x != 0) {
						x--;
					}
					break;
				default:
					break;
			}
		}

		//-----------------------------------------------------

		int parse_input_move(int ch) {
			refresh();
			switch (ch) {
				case KEY_UP:
					return 1;
					break;
				case KEY_RIGHT:
					return 2;
					break;
				case KEY_DOWN:
					return 3;
					break;
				case KEY_LEFT:
					return 4;
					break;
				default:
					return 0;
					break;
			}
		}

		//-----------------------------------------------------
		
		int parse_input_shoot(int ch) {
			switch (ch) {
				case (int) 'w':
					return 6;
					break;
				case (int) 'd':
					return 7;
					break;
				case (int) 's':
					return 8;
					break;
				case (int) 'a':
					return 9;
					break;
				default:
					return 0;
					break;
			}
		}

		//-----------------------------------------------------
		
		void randomize_player_pos() {
			this->x = generate_random();
			this->y = generate_random();
		}

		//-----------------------------------------------------

		Player() {
			draw_player();
		}

		//-----------------------------------------------------
		
		bool shoot(int wumpus_y, int wumpus_x, int dir) {
			// top
			if (dir == 6) {
				if (wumpus_y == this->y - 1 && wumpus_x == this->x) {
					return true;
				}
			}
			
			// right
			else if (dir == 7) {
				if (wumpus_y == this->y && wumpus_x == this->x + 1) {
					return true;	
				}
			}

			// bottom
			else if (dir == 8) {
				if (wumpus_y == this->y + 1 && wumpus_x == this->x) {
					return true;	
				}
			}

			// left
			else if (dir == 9) {
				if (wumpus_y == this->y && wumpus_x == this->x - 1) {
					return true;	
				}
			}
			return false;
		}
};

//---------------------------------------------------------
// define your enemy's class and hit checker

class Wumpus : public Thing {
	public:
		bool check_if_hit_player(int player_y, int player_x) {
			if (player_y == this->y && player_x == this->x) {
				attron(A_BOLD);
				init_pair(1, COLOR_RED, COLOR_BLACK);
				attron(COLOR_PAIR(1));

				const char *msg = "You lost! Press any key to leave.";

				mvprintw(LINES / 8, (COLS / 2) - strlen(msg) + 5, msg);

				attroff(COLOR_PAIR(1));
				return true;
			}
			return false;
		}	

		//-----------------------------------------------------
		// o.O this checks if the wumpus is near the player
		void check_if_wumpus_nearby(int player_y, int player_x) {
			if ((player_x + 1 == x && player_y == y) || \
					(player_x - 1 == x && player_y == y) || \
					(player_x == x && player_y + 1 == y) || \
					(player_x == x && player_y - 1 == y) || \
					(player_x - 1 == x && player_y - 1 == y) || \
					(player_x - 1 == x && player_y + 1 == y) || \
					(player_x + 1 == x && player_y - 1 == y) || \
					(player_x + 1 == x && player_y + 1 == y))	{
				mvprintw(LINES / 4 + 5, 0, "You smell a Wumpus.");
			}
		}
};

//---------------------------------------------------------
// Define bats (random teleporters)
class Bat : public Thing {
public:
	void teleport_player(Player player) {
		if (this->x == player.x && this->y == player.x) {
			player.randomize_player_pos();	
		}
	}
};
