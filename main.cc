// Import libs
#include <ncurses.h>

#include <iostream>

#include "player.cc"

// Preload functions
void print_grid();

// Here we go boys
int main() {
	int ch;

	initscr();
	curs_set(FALSE); // no cursor
	keypad(stdscr, TRUE);
	raw(); // no line buffering (no need to press return after input)
	noecho(); // no weird chars
	start_color();

	print_grid();

	Player player;
	Wumpus wumpus;
	Bat bat1;
	Bat bat2;

	// loop until the user press q
	while  ((ch = getch()) != (int) 'q') {
		// move

		clear();
		print_grid();

		player.move(player.parse_input_move(ch));
		
		// debug info
		// printw("wumpus x: %c | y: %c\n", (char) wumpus.x + 48, (char) wumpus.y + 48);
		// printw("player x: %c | y: %c\n", (char) player.x + 48, (char) player.y + 48);
		// printw("bat1 x:   %c | y: %c\n", (char) bat1.x + 48, (char) bat1.y + 48);
		// printw("bat2 x:   %c | y: %c\n", (char) bat2.x + 48, (char) bat2.y + 48);

		/*
		if (player.x == bat1.x && player.y == bat1.y) {
			player.randomize_player_pos();
		}
		if (player.x == bat2.x && player.y == bat2.y) {
			do {
				player.randomize_player_pos();
				// impossible to be carried to the wumpus' cell
			} while (wumpus.x == player.x && wumpus.y == player.y);
		}*/


		player.draw_player();
		refresh();

		if (player.shoot(wumpus.y, wumpus.x, player.parse_input_shoot(ch))) {
			attron(A_BOLD);
			init_pair(1, COLOR_GREEN, COLOR_BLACK);

			attron(COLOR_PAIR(1));
			mvprintw(12, 0, "You won! Congratulations.");
			attroff(COLOR_PAIR(1));
			break;
		}

		wumpus.check_if_wumpus_nearby(player.y, player.x);
		if (wumpus.check_if_hit_player(player.y, player.x)) {
			break;
		}
		// wumpus.check_if_hit_player(player.x, player.y) ? break : continue;
	}

	refresh(); // show the things on screen
	getch();
	endwin(); // finish the program and exit TUI mode
}

//-------------------------------------------------------------------
// Print out the base grid.

void print_grid() {
	for (int c = 0; c < 4; c++) {
		printw("   |   |   |   |\n");
		printw("-------------------\n");
	}
	printw("   |   |   |   |\n");
}
