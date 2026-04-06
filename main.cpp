//DOMINIK GURCZYŃSKI 203854 ACIR 2A ZADANIE_DOMOWE_V2

#include <iostream>
#include <cctype>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

const int CC_BOARD_SIZE = 3;
const int CHECKERS_BOARD_SIZE = 8;

//Funkcja sprawdzająca czy figury nie wychodzą poza obszar roboczy, ani nie nachodzą na siebie, zwraca wartość true, jeśli żaden z wymienionych warunków nie został spełniony
bool is_fitting(int cx, int cy, int cr, int rx, int ry, int rwidth, int rheight, int lx1, int ly1, int lx2, int ly2) {
	if ((cx - cr) < 0 || (cx + cr) > 1200 || (cy - cr) < 0 || (cy + cr) > 800) { return false; }
	if (rx > 1200 || rx < 0 || ry > 800 || ry < 0 || (rx + rwidth) > 1200 || (ry + rheight) > 800) { return false; }
	if (lx1 < 0 || lx1 > 1200 || ly1 < 0 || ly1 > 800 || lx2 < 0 || lx2 > 1200 || ly2 < 0 || ly2 > 800) { return false; }
	if (lx1 >= rx && lx1 <= rx + rwidth && ly1 >= ry && ly1 <= ry + rheight) { return false; }
	if (lx2 >= rx && lx2 <= rx + rwidth && ly2 >= ry && ly2 <= ry + rheight) { return false; }

	int x_close = cx;
	int y_close = cy;
	if (cx < rx) { x_close = rx; }
	else if (cx > rx + rwidth) { x_close = rx + rwidth; }
	if (cy < ry) { y_close = ry; }
	else if (cy > ry + rheight) { y_close = ry + rheight; }
	if (pow(cx - x_close, 2) + pow(cy - y_close, 2) <= (pow(cr, 2))) { return false; }

	return true;
}

//Funkcja tworząca pusty lub wypełniony plik SVG, zależnie od trybu wybranego przez użytkownika
void create_SVG(string file_name, int mode) {
	ofstream file(file_name);
	file << "<?xml version='1.0' encoding='UTF-8'?>" << endl;
	file << "<svg width='1200' height='800' xmlns='http://www.w3.org/2000/svg'>" << endl;
	if (mode == 1) {
		string cfill, cstroke, rfill, rstroke, lfill;
		int cx, cy, cr;
		int rx, ry, rwidth, rheight;
		int lx1, lx2, ly1, ly2;
		bool fitting = false;
		do {
			cout << "Okrag - Podaj wartosci liczbowe XY, promien, kolor wypelnienia i obwodu: ";
			cin >> cx >> cy >> cr >> cfill >> cstroke;
			cout << "Prostokat - Podaj szerokosc i wysokosc, wartosci liczbowe XY, kolor wypelnienia i obwodu: ";
			cin >> rwidth >> rheight >> rx >> ry >> rfill >> rstroke;
			cout << "Linia - Podaj startowe i koncowe wartosci liczbowe XY i kolor wypelnienia: ";
			cin >> lx1 >> ly1 >> lx2 >> ly2 >> lfill;

			fitting = is_fitting(cx, cy, cr, rx, ry, rwidth, rheight, lx1, ly1, lx2, ly2);
			if (!fitting) { cout << "Podane dane sa nieprawidlowe, figura wychodzi poza pole robocze lub nachodzi na inna!" << endl; }
		} while (!fitting);

		file << "<circle r='" << cr << "' cx='" << cx << "' cy='" << cy << "' fill='" << cfill << "' stroke='" << cstroke << "' />" << endl;
		file << "<rect width='" << rwidth << "' height='" << rheight << "' x='" << rx << "' y='" << ry << "' fill='" << rfill << "' stroke='" << rstroke << "' />" << endl;
		file << "<line x1='" << lx1 << "' y1='" << ly1 << "' x2='" << lx2 << "' y2='" << ly2 << "' style='stroke:" << lfill << ";stroke-width:5' />" << endl;
	}
	file << "</svg>" << endl;
	file.close();
}

//Funkcja wyświetlająca zawartosć tablicy planszy kółka i krzyżyk
void CC_display_board(char board[CC_BOARD_SIZE][CC_BOARD_SIZE]) {
	char col = char(186);
	char row = char(205);
	cout << "  1   2   3" << endl;

	for (int i = 0; i < CC_BOARD_SIZE; i++) {
		cout << i + 1;
		for (int j = 0; j < CC_BOARD_SIZE; j++) {
			if (j != 2) { cout << " " << board[i][j] << " " << col; }
			else { cout << " " << board[i][j] << " "; }
		}
		cout << endl;
		if (i != 2) {
			cout << " " << row << row << row << char(206) << row << row << row << char(206) << row << row << row << endl;
		}
	}
}

//Funkcja umożliwiająca ruch graczy, a także sprawdzająca warunki poprawności i czas ruchu
void CC_move_players(char board[CC_BOARD_SIZE][CC_BOARD_SIZE], int& turn, bool& time_ended) {
	int x, y;
	cout << "Gracz " << (turn % 2 == 0 ? "O" : "X") << ", podaj wspolrzedne ruchu: ";
	time_t start_timer = time(0);
	cin >> x >> y;
	time_t end_timer = time(0);
	if (difftime(end_timer, start_timer) <= 20) {
		if (x >= 1 && x <= 3 && y >= 1 && y <= 3) {
			if (board[x - 1][y - 1] == ' ') {
				if (turn % 2 != 0) {
					board[x - 1][y - 1] = 'X';
				}
				else if (turn % 2 == 0) {
					board[x - 1][y - 1] = 'O';
				}
				turn++;
			}
			else { cout << "Pole jest juz zajete!" << endl; }
		}
		else { cout << "Podane wspolrzedne sa bledne!" << endl; }
	}
	else {
		time_ended = true;
		cout << "Czas tury zostal przekroczony!" << endl;
		return;
	}
}

//Funkcja sprawdzająca warunki wygranej, zwraca wartość true, jeśli wygrana została wykryta
bool CC_check_win(char board[CC_BOARD_SIZE][CC_BOARD_SIZE], char& winner) {
	if (board[0][0] != ' ' && board[0][0] == board[0][1] && board[0][1] == board[0][2]) { winner = board[0][0]; return true; }
	else if (board[1][0] != ' ' && board[1][0] == board[1][1] && board[1][1] == board[1][2]) { winner = board[1][0]; return true; }
	else if (board[2][0] != ' ' && board[2][0] == board[2][1] && board[2][1] == board[2][2]) { winner = board[2][0]; return true; }
	else if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) { winner = board[0][0]; return true; }
	else if (board[2][0] != ' ' && board[2][0] == board[1][1] && board[1][1] == board[0][2]) { winner = board[2][0]; return true; }
	else if (board[0][0] != ' ' && board[0][0] == board[1][0] && board[1][0] == board[2][0]) { winner = board[0][0]; return true; }
	else if (board[0][1] != ' ' && board[0][1] == board[1][1] && board[1][1] == board[2][1]) { winner = board[0][1]; return true; }
	else if (board[0][2] != ' ' && board[0][2] == board[1][2] && board[1][2] == board[2][2]) { winner = board[0][2]; return true; }

	return false;
}

//Funkcja tworząca i aktualizująca plik SVG zawierający plansze kólka i krzyżyk
void CC_to_SVG(char board[CC_BOARD_SIZE][CC_BOARD_SIZE], string file_name) {
	ofstream file(file_name);
	file << "<?xml version='1.0' encoding='UTF-8'?>" << endl;
	file << "<svg width='1200' height='800' xmlns='http://www.w3.org/2000/svg'>" << endl;
	file << "<line x1='100' y1='0' x2='100' y2='300' style='stroke:black;stroke-width:5'/>" << endl;
	file << "<line x1='200' y1='0' x2='200' y2='300' style='stroke:black;stroke-width:5'/>" << endl;
	file << "<line x1='0' y1='100' x2='300' y2='100' style='stroke:black;stroke-width:5'/>" << endl;
	file << "<line x1='0' y1='200' x2='300' y2='200' style='stroke:black;stroke-width:5'/>" << endl;

	for (int i = 0; i < CC_BOARD_SIZE; i++) {
		for (int j = 0; j < CC_BOARD_SIZE; j++) {
			if (board[i][j] == 'O') {
				file << "<circle r='40' cx='" << 50 + (j * 100) << "' cy='" << 50 + (i * 100) << "' fill='white' stroke='black' stroke-width='5' />" << endl;
			}
			if (board[i][j] == 'X') {
				file << "<line x1='" << 15 + (j * 100) << "' y1='" << 15 + (i * 100) << "' x2='" << 85 + (j * 100) << "' y2='" << 85 + (i * 100) << "' style='stroke:black;stroke-width:5' />" << endl;
				file << "<line x1='" << 85 + (j * 100) << "' y1='" << 15 + (i * 100) << "' x2='" << 15 + (j * 100) << "' y2='" << 85 + (i * 100) << "' style='stroke:black;stroke-width:5' />" << endl;
			}
		}
	}
	file << "</svg>" << endl;
	file.close();
}

//Funkcja operująca ruchem bota, sprawdzająca czy możliwy z warunków ruchu jest spełniony. Wykonuje sprawdzenie według zadanej logiki:
// -> stawia pionek na srodku jesli to mozliwe -> blokuje 2 pionki gracza -> stawia kolejny pionek obok swojego -> stawia losowo pionek
void CC_bot_move(char board[CC_BOARD_SIZE][CC_BOARD_SIZE], int& turn) {
	if (turn == 0) {
		if (board[1][1] == ' ') { board[1][1] = 'O'; turn++; return; }
	}
	for (int i = 0; i < CC_BOARD_SIZE; i++) {
		for (int j = 0; j < CC_BOARD_SIZE; j++) {
			if (i + 2 < 3 && board[i][j] == 'X' && board[i + 1][j] == 'X' && board[i + 2][j] == ' ') { board[i + 2][j] = 'O'; turn++; return; }
			if (i - 2 >= 0 && board[i][j] == 'X' && board[i - 1][j] == 'X' && board[i - 2][j] == ' ') { board[i - 2][j] = 'O'; turn++; return; }
			if (i + 2 < 3 && board[i][j] == 'X' && board[i + 2][j] == 'X' && board[i + 1][j] == ' ') { board[i + 1][j] = 'O'; turn++; return; }
			if (j + 2 < 3 && board[i][j] == 'X' && board[i][j + 1] == 'X' && board[i][j + 2] == ' ') { board[i][j + 2] = 'O'; turn++; return; }
			if (j - 2 >= 0 && board[i][j] == 'X' && board[i][j - 1] == 'X' && board[i][j - 2] == ' ') { board[i][j - 2] = 'O'; turn++; return; }
			if (j + 2 < 3 && board[i][j] == 'X' && board[i][j + 2] == 'X' && board[i][j + 1] == ' ') { board[i][j + 1] = 'O'; turn++; return; }
			if (i + 2 < 3 && j + 2 < 3 && board[i][j] == 'X' && board[i + 1][j + 1] == 'X' && board[i + 2][j + 2] == ' ') { board[i + 2][j + 2] = 'O'; turn++; return; }
			if (i + 2 < 3 && j + 2 < 3 && board[i][j] == 'X' && board[i + 2][j + 2] == 'X' && board[i + 1][j + 1] == ' ') { board[i + 1][j + 1] = 'O'; turn++; return; }
			if (i + 2 < 3 && j - 2 >= 0 && board[i][j] == 'X' && board[i + 1][j - 1] == 'X' && board[i + 2][j - 2] == ' ') { board[i + 2][j - 2] = 'O'; turn++; return; }
			if (i + 2 < 3 && j - 2 >= 0 && board[i][j] == 'X' && board[i + 2][j - 2] == 'X' && board[i + 1][j - 1] == ' ') { board[i + 1][j - 1] = 'O'; turn++; return; }
			if (i + 2 < 3 && j + 2 < 3 && board[i + 1][j + 1] == 'X' && board[i + 2][j + 2] == 'X' && board[i][j] == ' ') { board[i][j] = 'O'; turn++; return; }
			if (i + 2 < 3 && j - 2 >= 0 && board[i + 1][j - 1] == 'X' && board[i + 2][j - 2] == 'X' && board[i][j] == ' ') { board[i][j] = 'O'; turn++; return; }
		}
	}

	for (int i = 0; i < CC_BOARD_SIZE; i++) {
		for (int j = 0; j < CC_BOARD_SIZE; j++) {
			if (i + 1 < 3 && board[i][j] == 'O' && board[i + 1][j] == ' ') { board[i + 1][j] = 'O'; turn++; return; }
			if (i - 1 >= 0 && board[i][j] == 'O' && board[i - 1][j] == ' ') { board[i - 1][j] = 'O'; turn++; return; }
			if (j + 1 < 3 && board[i][j] == 'O' && board[i][j + 1] == ' ') { board[i][j + 1] = 'O'; turn++; return; }
			if (j - 1 >= 0 && board[i][j] == 'O' && board[i][j - 1] == ' ') { board[i][j - 1] = 'O'; turn++; return; }
			if (i + 1 < 3 && j + 1 < 3 && board[i][j] == 'O' && board[i + 1][j + 1] == ' ') { board[i + 1][j + 1] = 'O'; turn++; return; }
			if (i + 1 < 3 && j - 1 >= 0 && board[i][j] == 'O' && board[i + 1][j - 1] == ' ') { board[i + 1][j - 1] = 'O'; turn++; return; }
			if (i - 1 >= 0 && j + 1 < 3 && board[i][j] == 'O' && board[i - 1][j + 1] == ' ') { board[i - 1][j + 1] = 'O'; turn++; return; }
			if (i - 1 >= 0 && j - 1 >= 0 && board[i][j] == 'O' && board[i - 1][j - 1] == ' ') { board[i - 1][j - 1] = 'O'; turn++; return; }
		}
	}

	int rand_x, rand_y;
	do {
		rand_x = rand() % 3;
		rand_y = rand() % 3;
	} while (board[rand_x][rand_y] != ' ');
	board[rand_x][rand_y] = 'O';
	turn++;
}

//Funkcja zarządzająca grą kółko i krzyżyk, dopasowuje tryb gry (bot lub drugi gracz), zależnie od wyboru użytkownika
void CC_manager(char board[CC_BOARD_SIZE][CC_BOARD_SIZE], int mode, int& turn) {
	turn = 0;
	bool game_ended = false, time_ended = false;
	char winner = ' ';

	while (!game_ended && turn < 9 && !time_ended) {
		CC_display_board(board);
		if (turn % 2 != 0) { CC_move_players(board, turn, time_ended); }
		else {
			if (mode == 0) { CC_bot_move(board, turn); }
			else { CC_move_players(board, turn, time_ended); }
		}
		CC_to_SVG(board, "kolko_i_krzyzyk.svg");
		game_ended = CC_check_win(board, winner);
	}

	if (time_ended) {
		cout << "Wygrywa gracz: " << (turn % 2 == 0 ? 'X' : 'O') << "!" << endl;
	}
	else if (game_ended) {
		cout << "Wygral gracz: " << winner << "!" << endl;
		CC_display_board(board);
		CC_to_SVG(board, "kolko_i_krzyzyk.svg");
	}
	else if (turn == 9) {
		cout << "Remis!" << endl;
		CC_display_board(board);
		CC_to_SVG(board, "kolko_i_krzyzyk.svg");
	}
}

//Funkcja wyświetlająca zawartosć tablicy planszy warcab
void CHECKERS_display_board(char board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE]) {
	char white_tile = char(219);
	cout << "    A  B  C  D  E  F  G  H" << "\n" << endl;
	for (int i = 0; i < CHECKERS_BOARD_SIZE; i++) {
		cout << i + 1 << "  ";
		for (int j = 0; j < CHECKERS_BOARD_SIZE; j++) {
			if (board[i][j] != ' ') {
				cout << " " << (board[i][j] == 'B' ? 'O' : '@') << " ";
			}
			else {
				if ((i + j) % 2 == 0) {
					cout << white_tile << white_tile << white_tile;
				}
				else { cout << "   "; }
			}
		}
		cout << endl;
	}
}

//Funkcja zwracająca zmieniony znak char na int, który używany jest do nawigacji w tablicy planszy
int get_coordinates(char input) {
	if (isalpha(input)) { return input - 'a'; }
	if (isdigit(input)) { return input - '1'; }
	return -1;
}

//Funkcja sprawdzająca czy zadany ruch spełnia warunki zasad gry, zwraca wartość true, jeśli wykryje taką możliwość
bool CHECKERS_if_movement_correct(char board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE], char player, int x1, int y1, int x2, int y2, bool capture) {
	int mode = (capture == true ? 2 : 1);
	if (board[y2][x2] != ' ') { return false; }
	if (abs(y2 - y1) != mode || abs(x2 - x1) != mode) { return false; }
	if (player == 'W' && y2 - y1 != mode) { return false; }
	else if (player == 'B' && y2 - y1 != -mode) { return false; }
	return true;
}

//Funkcja sprawdzająca czy da sie wykonać bicie, zwraca wartość true, jeśli wykryje taką możliwość
bool CHECKERS_can_capture(char board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE], char player) {
	int move_increment = (player == 'W' ? 1 : -1);
	int directions[] = { -1, 1 };

	for (int i = 0; i < CHECKERS_BOARD_SIZE; i++) {
		for (int j = 0; j < CHECKERS_BOARD_SIZE; j++) {
			if (board[i][j] == player) {
				for (int k = 0; k < 2; k++) {
					int dir = directions[k];
					if (i + 2 * move_increment < 8 && i + 2 * move_increment >= 0 && j + 2 * dir < 8 && j + 2 * dir >= 0) {
						if (board[i + move_increment][j + dir] != ' ' && board[i + move_increment][j + dir] != player) {
							if (board[i + 2 * move_increment][j + 2 * dir] == ' ') { return true; }
						}
					}
				}
			}
		}
	}
	return false;
}

//Funkcja zarządzająca ruchem graczy, która wymusza bicie na aktualnym graczu;
void CHECKERS_move_players(char board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE], int& turn, char player) {
	int x1, y1, x2, y2;
	char move_start[3], move_end[3];

	bool capture = CHECKERS_can_capture(board, player);
	if (capture) {
		cout << "Masz mozliwosc bicia. Musisz zbic pionek przeciwnika!" << endl;
	}

	cout << "Tura: " << (turn % 2 != 0 ? "Czarne" : "Biale") << " - wybierz pionek: ";
	cin >> move_start;
	x1 = get_coordinates(tolower(move_start[0]));
	y1 = get_coordinates(move_start[1]);

	if (x1 >= 0 && x1 < 8 && y1 >= 0 && y1 < 8) {
		if (board[y1][x1] == player) {
			cout << "Gdzie chcesz wykonac ruch: ";
			cin >> move_end;
			x2 = get_coordinates(tolower(move_end[0]));
			y2 = get_coordinates(move_end[1]);

			if (x2 >= 0 && x2 < 8 && y2 >= 0 && y2 < 8) {
				if (capture) {
					if (abs(y2 - y1) != 2 || abs(x2 - x1) != 2) {
						cout << "Musisz zbic pionek przeciwnika!" << endl;
						return;
					}
				}
				if (CHECKERS_if_movement_correct(board, player, x1, y1, x2, y2, capture)) {
					if (abs(y2 - y1) == 2 && abs(x2 - x1) == 2) {
						board[(y1 + y2) / 2][(x1 + x2) / 2] = ' ';
					}
					board[y1][x1] = ' ';
					board[y2][x2] = player;
					turn++;
				}
			}
		}
		else { cout << "Podany pionek nienalezy do tego gracza!" << endl; }
	}
	else { cout << "Podane wspolrzedne sa bledne!" << endl; }
}

//Funkcja sprawdzająca warunki wygranej, zwraca true, jeśli jeden z graczy nie ma już pionków lub został zablokowany
bool CHECKERS_check_win(char board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE], char& winner, char player) {
	int whites = 0, blacks = 0;
	for (int i = 0; i < CHECKERS_BOARD_SIZE; i++) {
		for (int j = 0; j < CHECKERS_BOARD_SIZE; j++) {
			if (board[i][j] == 'W') { whites++; }
			else if (board[i][j] == 'B') { blacks++; }
		}
	}
	if (blacks == 0) {
		winner = 'W';
		return true;
	}
	else if (whites == 0) {
		winner = 'B';
		return true;
	}
	if (CHECKERS_can_capture(board, player)) { return false; }

	int move_increment = (player == 'W' ? 1 : -1);
	int directions[] = { -1, 1 };
	bool move_possible = false;

	for (int i = 0; i < CHECKERS_BOARD_SIZE; i++) {
		for (int j = 0; j < CHECKERS_BOARD_SIZE; j++) {
			if (board[i][j] == player) {
				for (int k = 0; k < 2; k++) {
					int dir = directions[k];
					if (i + move_increment < 8 && i + move_increment >= 0 && j + dir < 8 && j + dir >= 0) {
						if (board[i + move_increment][j + dir] == ' ') {
							move_possible = true;
							break;
						}
					}
				}
			}
			if (move_possible) { break; }
		}
		if (move_possible) { break; }
	}
	if (!move_possible) {
		winner = (player == 'W' ? 'B' : 'W');
		return true;
	}

	return false;
}

//Funkcja tworząca i aktualizująca plik SVG zawierający plansze warcab
void CHECKERS_to_SVG(char board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE], string file_name) {
	ofstream file(file_name);
	file << "<?xml version='1.0' encoding='UTF-8'?>" << endl;
	file << "<svg width='1200' height='800' xmlns='http://www.w3.org/2000/svg'>" << endl;
	file << "<rect width='520' height='520' x='20' y='20' fill='white' stroke='black' stroke-width='5' rx='20' ry='20' />";
	file << "<rect width='480' height='480' x='40' y='40' fill='white' stroke='black' stroke-width='4' />";
	for (int i = 0; i < CHECKERS_BOARD_SIZE; i++) {
		for (int j = 0; j < CHECKERS_BOARD_SIZE; j++) {
			if ((i + j) % 2 == 0) {
				file << "<rect width='60' height='60' x='" << 40 + (j * 60) << "' y = '" << 40 + (i * 60) << "' fill='white' stroke='black' />";
			}
			else {
				file << "<rect width='60' height='60' x='" << 40 + (j * 60) << "' y = '" << 40 + (i * 60) << "' fill='black' />";
			}
			if (board[i][j] != ' ') {
				if (board[i][j] == 'B') {
					file << "<circle r='25' cx='" << 70 + (j * 60) << "' cy='" << 70 + (i * 60) << "' fill='black' stroke='white' stroke-width='4' />";
				}
				else {
					file << "<circle r='25' cx='" << 70 + (j * 60) << "' cy='" << 70 + (i * 60) << "' fill='white' stroke='black' stroke-width='4' />";
				}
			}
		}
	}
	file << "</svg>" << endl;
	file.close();
}

//Funkcja tworzy i zapisująca rozgrywke warcab w pliku
void CHECKERS_save_game(char board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE], string file_name, int turn) {
	ofstream file(file_name);
	file << turn << endl;
	for (int i = 0; i < CHECKERS_BOARD_SIZE; i++) {
		for (int j = 0; j < CHECKERS_BOARD_SIZE; j++) {
			if (board[i][j] == ' ') { file << 'P' << endl; }
			else { file << board[i][j] << endl; }
		}
	}
	file.close();
}

//Funkcja odczytuje odczytuje i wypelnia tablice planszy gry z zapisanego pliku
void CHECKERS_read_game(char board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE], string file_name, int& turn) {
	ifstream file(file_name);
	if (!file) { return; }
	file >> turn;
	char placeholder;
	for (int i = 0; i < CHECKERS_BOARD_SIZE; i++) {
		for (int j = 0; j < CHECKERS_BOARD_SIZE; j++) {
			file >> placeholder;
			if (placeholder == 'P') { board[i][j] = ' '; }
			else { board[i][j] = placeholder; }
		}
	}
	file.close();
}

//Funkcja zarządzająca grą warcaby, sprawdza czy należy wczytać akrualna rozgrywkę, a także zarządza cała logiką gry
void CHECKERS_manager(char board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE], int& turn) {
	int choice;
	bool game_ended = false;
	char winner = ' ';

	cout << "1. Nowa gra\n2. Kontynuuj gre\nWybierz opcje: ";
	cin >> choice;

	if (choice == 2) {
		ifstream file("zapis.txt");
		if (file.good()) {
			file.close();
			CHECKERS_read_game(board, "zapis.txt", turn);
		}
	}
	while (!game_ended) {
		char current_player = turn % 2 == 0 ? 'W' : 'B';
		CHECKERS_display_board(board);
		CHECKERS_to_SVG(board, "warcaby.svg");
		CHECKERS_move_players(board, turn, current_player);
		CHECKERS_save_game(board, "zapis.txt", turn);
		game_ended = CHECKERS_check_win(board, winner, current_player);
	}
	if (game_ended) {
		cout << "Wygral gracz: " << winner << "!" << endl;
		CHECKERS_display_board(board);
		CHECKERS_to_SVG(board, "warcaby.svg");
	}
}

int main() {
	srand(time(0));
	char choice;
	int CC_turn = 0;

	cout << "Witaj w programie, oto dostepne opcje: " << endl;
	do {
		cout << "1. Wygeneruj podstawowy plik SVG.\n2. Wygeneruj zaawansowany plik SVG.\n3. Gra w kolko i krzyzyk z drugim graczem\n4. Gra w kolko i krzyzyk z botem\n5. Gra w warcaby\nH. Pomoc\nX. Zamknij program\n" << endl;
		cout << "Wybierz, ktora operacje chcesz wykonac: ";
		cin >> choice;
		cout << endl;

		choice = toupper(choice);
		switch (choice) {
		case '1': {
			create_SVG("plik_graficzny.svg", 0);
			cout << "Pusty plik zostal wygenerowany." << endl;
			break;
		}

		case '2': {
			create_SVG("plik_graficzny.svg", 1);
			cout << "Plik z wpisanymi wartosciami zostal wygenerowany." << endl;
			break;
		}

		case '3': {
			char CC_board[CC_BOARD_SIZE][CC_BOARD_SIZE] = {
				{' ', ' ', ' '},
				{' ', ' ', ' '},
				{' ', ' ', ' '}
			};
			CC_manager(CC_board, 1, CC_turn);
			break;
		}

		case '4': {
			char CC_board[CC_BOARD_SIZE][CC_BOARD_SIZE] = {
				{' ', ' ', ' '},
				{' ', ' ', ' '},
				{' ', ' ', ' '}
			};
			CC_manager(CC_board, 0, CC_turn);
			break;
		}

		case '5': {
			int turn = 0;
			char CHECKERS_board[CHECKERS_BOARD_SIZE][CHECKERS_BOARD_SIZE] = {
				{' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W'},
				{'W', ' ', 'W', ' ', 'W', ' ', 'W', ' '},
				{' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W'},
				{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
				{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
				{'B', ' ', 'B', ' ', 'B', ' ', 'B', ' '},
				{' ', 'B', ' ', 'B', ' ', 'B', ' ', 'B'},
				{'B', ' ', 'B', ' ', 'B', ' ', 'B', ' '}
			};
			CHECKERS_manager(CHECKERS_board, turn);
			break;
		}

		case 'H': {
			cout << char(254) << " Kolko i Krzyzyk - Gra posiada 2 tryby gry, z botem lub przeciwko drugiemu graczowi. Zeby postawic pionek nalezy wpisac wspolrzedne, najpierw rzad, potem kolumne, wszystko oddzielone spacja." << endl;
			cout << char(254) << " Warcaby - Gra ma mozliwosc wczytania poprzedniej rozgrywki, jesli zapis takowej istnieje lub wygenerowanie nowej. Zeby ruszyc pionek nalezy podac pozycje startowa pionka np. B3, a nastepnie pozycje koncowa np. A4" << endl;
			break;
		}

		case 'X': { cout << "Zamykanie programu..." << endl; break; }
		}
		cout << endl;
	} while (choice != 'X');

	return 0;
}
