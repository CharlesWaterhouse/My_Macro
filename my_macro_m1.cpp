//20180830
//from WATERHOUSE CLARILES


#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <direct.h>
#include <io.h>
#include "my_macro_h1.h"
#include "waterhouse_function.h"

using namespace std;

int g_wibsite_delay_time = 3000;
string g_the_year;
string g_start_number;
string g_next_number = "33333";
string g_frame_name;
string g_row = "1";
string g_grid = "1";
int g_bit_delay_time = 500;
int g_macro[10];
/*
int g_macro1;
int g_macro2;
int g_macro3;
int g_macro4;
*/
vector<vector<string>> g_all_position_data;
int g_which_row_be_enter=-1;
int g_data_least_row = 0;


void initialoze_globa_paramater();

bool IsDataOK();
void ArrangeData();

void CountRow();
void CountGrid();
void UpdateCurrentRowDate(int input_which_row);
void MacroAction0(string mode);
void MacroAction1(string mode);
void MacroAction2(string mode);
void MacroAction3(string mode);
void MacroAction4(string mode);
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardHookProcWithDATA(int nCode, WPARAM wParam, LPARAM lParam);



int main() {
	cout << "Welcome to my Keying Helper! This is developed from Waterhouse!" << endl;
	cout << "Thanks for your using! Care about some limit!" << endl;
	initialoze_globa_paramater();
	while (1) {
		
		cout << "e/1/2/3/4 ?" << endl;
		cout << "e to exit the program!" << endl;
		cout << "1 to read datas from a csv file." << endl;
		cout << "2 is a mode with a bit easiler to macro some action." << endl;
		cout << "3 is a mode with a auto import one data from csv file at a time." << endl;
		cout << "4 to auto keyboard all data from read datas. " << endl;
		string mode_type;
		cin >> mode_type;
		if (mode_type == "e") { exit(0); }
		else if (mode_type == "1") {
			/*to choose the read_file*/
			string working_file_name;
			ChooseFile(working_file_name);
			//vector<vector<string>> g_all_position_data;
			if (!ReadCsvTo2dString(working_file_name, g_all_position_data)) {
				cout << "Reading is not successful!" << endl;
				cout << "Maybe that file isn't exist or it's opening!" << endl;
				continue;
			}
			
			Appear2DString(g_all_position_data);
			ArrangeData();
			Appear2DString(g_all_position_data);
			
			cout << "do u want to save the arraged Data to csv file? y/n" << endl;
			cin >> mode_type;
			if (mode_type=="y") {
				string output_csv_name;
				while (1) {
					cout << "keyboard the file name? ex: file123" << endl;
					cin >> output_csv_name;
					if (IsAllAlphabet(output_csv_name)&& !IsFileExist(output_csv_name +".csv")) {
						output_csv_name = output_csv_name + ".csv";
						cout << output_csv_name << endl;
						Write2dStringToCsv(output_csv_name, g_all_position_data);
						cout << output_csv_name << "...OK" << endl;
						break;
					}
					cout << "Maybe the file have existed " << endl;
					cout << "The output csv file name must be component with all Alphabet!" << endl;
				}
				
			}
		}
		else if (mode_type == "2") {
			// Set windows hook
			HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
			MessageBox(NULL, "Press OK to stop logging.", "Information", MB_OK);
			UnhookWindowsHookEx(keyboardHook);
		}
		else if (mode_type == "3") {
			if (g_all_position_data.empty()) {
				cout << "Notice! Need to read data first!" << endl;
				cout << "Choose action again!" << endl;
				continue;
			}
			cout << "which row form csv will be keyin ?" << endl;
			while (1) {
				SafeCinInt(g_which_row_be_enter);
				g_which_row_be_enter = g_which_row_be_enter - 1;
				if (g_which_row_be_enter < g_all_position_data.size()) { break; }
			}
			HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProcWithDATA, NULL, 0);
			MessageBox(NULL, "Press OK to stop logging.", "Information", MB_OK);
			UnhookWindowsHookEx(keyboardHook);
		}
		else if (mode_type == "4") {
			if (g_all_position_data.empty()) {
				cout << "Notice! Need to read data first!" << endl;
				cout << "Choose action again!" << endl;
				continue;
			}
			cout << "which row form csv will be keyin ?" << endl;
			while (1) {
				SafeCinInt(g_which_row_be_enter);
				g_which_row_be_enter = g_which_row_be_enter - 1;
				if (g_which_row_be_enter < g_all_position_data.size() || g_which_row_be_enter >= 1) { break; }
			}

			CountDownTimeSec(5);
			while (1) {
				UpdateCurrentRowDate(g_which_row_be_enter);
				if (g_the_year.empty() || g_start_number.empty() || g_next_number.empty() || g_frame_name.empty() || g_row.empty() || g_grid.empty()) {
					cout << "something is empty, stop keyboarding" << endl;
					system("pause");
					break;
				}
				MacroAction0("NOBACK");
				Kcout(g_start_number);
				MacroAction1("NOBACK");
				cout << "the row." << g_which_row_be_enter + 1 << ": " << g_the_year << ", " << g_start_number << ", " << g_next_number << ", " << g_frame_name << ", " << g_row << ", " << g_grid;
				cout << "...OK" << endl;
				g_which_row_be_enter = g_which_row_be_enter + 1;

				Sleep(g_wibsite_delay_time);

				MacroAction2("NOBACK");
				Sleep(g_wibsite_delay_time);
			}
		}
	}


	/*
	Sleep(5000);
	keybd_event(0xd, 0, 0, 0);
	keybd_event(VK_SHIFT, 0x10, 0, 0); // shift depress
	keybd_event(VkKeyScan('a'), 0, 0, 0); //depress
	keybd_event(VkKeyScan('a'), 0, KEYEVENTF_KEYUP, 0); // release!
	keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);// shift release
	Sleep(1000);
	keybd_event(0xd, 0, 0, 0);
	*/
	cout << "line 51" << endl;
	system("pause");
}


void initialoze_globa_paramater() {
	vector<vector<string>> initialize_data;
	string initialize_data_file_name = "mymacro.csv";
	ReadCsvTo2dString(initialize_data_file_name, initialize_data);
	
	g_wibsite_delay_time = atoi(initialize_data[0][1].c_str());
	g_the_year = initialize_data[1][1];
	g_next_number = initialize_data[2][1];
	g_frame_name = initialize_data[3][1];
	g_row = initialize_data[4][1];
	g_grid = initialize_data[5][1];
	g_bit_delay_time = atoi(initialize_data[6][1].c_str());
	
	for (int index1 = 0; index1 < 10 && !initialize_data[7 + index1][1].empty(); index1++) {
		if (IsInt(initialize_data[7 + index1][1]) && stoi(initialize_data[7 + index1][1]) > 31 && stoi(initialize_data[7 + index1][1]) < 128) { g_macro[index1] = stoi(initialize_data[7 + index1][1]); }
		else if (initialize_data[7 + index1][1][0] >= 20 && initialize_data[7 + index1][1][0] < 128) { g_macro[index1] = initialize_data[7 + index1][1][0]; }
		cout << "macro" << index1 << " : " << initialize_data[7 + index1][1] << endl;
	}
	/*
	if (IsInt(initialize_data[7][1]) && stoi(initialize_data[7][1]) > 31 && stoi(initialize_data[7][1]) < 128) { g_macro1 = stoi(initialize_data[7][1]); }
	else if (initialize_data[7][1][0] >= 20 && initialize_data[7][1][0] < 128) { g_macro1 = initialize_data[7][1][0]; }
	
	if (IsInt(initialize_data[8][1]) && stoi(initialize_data[8][1]) > 31 && stoi(initialize_data[8][1]) < 128) { g_macro2 = stoi(initialize_data[8][1]); }
	else if (initialize_data[8][1][0] >= 20 && initialize_data[8][1][0] < 128) { g_macro2 = initialize_data[8][1][0]; }
	
	if (IsInt(initialize_data[9][1]) && stoi(initialize_data[9][1]) > 31 && stoi(initialize_data[9][1]) < 128) { g_macro3 = stoi(initialize_data[9][1]); }
	else if (initialize_data[9][1][0] >= 20 && initialize_data[9][1][0] < 128) { g_macro3 = initialize_data[9][1][0]; }
	
	if (IsInt(initialize_data[10][1]) && stoi(initialize_data[10][1]) > 31 && stoi(initialize_data[10][1]) < 128) { g_macro4 = stoi(initialize_data[10][1]); }
	else if (initialize_data[10][1][0] >= 20 && initialize_data[10][1][0] < 128) { g_macro4 = initialize_data[10][1][0]; }
	*/

	for (int index1 = 0; index1 < initialize_data.size(); index1++) {
		if (initialize_data[index1][0] == "macro0") { break; }
		cout << initialize_data[index1][0] << ", " << initialize_data[index1][1] << endl;
	}
}

bool IsDataOK() {
	bool is_correct=true;
	for (int index2 = 1; index2 < g_all_position_data.size(); index2++) {
		for (int index3 = 1; index3 <= 4; index3++) {
			if (!g_all_position_data[index2][index3].empty() && !IsPosInt(g_all_position_data[index2][index3])) {
				cout << "data in " << index2 + 1 << "," << index3 + 1 << " of csv file isnot correct!" << endl;
				is_correct = false;
			}
		}
	}
	if (is_correct) { return true; }
	else { return false; }
}
void ArrangeData() {
	if (IsDataOK()) { cout << "data can be arragne" << endl; }
	else { system("pause"); exit(0); }
	for (int index2 = 1; index2 < g_all_position_data.size(); index2++) {
		if (index2 == 1) { 
			if (g_all_position_data[index2][0].empty() || g_all_position_data[index2][1].empty() || g_all_position_data[index2][2].empty() || g_all_position_data[index2][2].empty()) {
				cout << "some data of row.2 is empty" << endl;
				system("pause");
				exit(0);
			}
			if (g_all_position_data[index2][4].empty()) {
				if (!g_all_position_data[index2 + 1][3].empty()) {
					int temp_end_number = stoi(g_all_position_data[index2 + 1][3]) - 1;
					g_all_position_data[index2][4] = to_string(temp_end_number);
				}
				else {
					g_all_position_data[index2][4] = g_all_position_data[index2][3];
				}
			}
			continue; 
		}
		if (g_all_position_data[index2][0].empty()) {
			g_all_position_data[index2][0] = g_all_position_data[index2 - 1][0];
		}
		if (g_all_position_data[index2][1].empty() || g_all_position_data[index2][2].empty()) {
			if (g_all_position_data[index2][0] != g_all_position_data[index2 - 1][0]) {
				g_all_position_data[index2][1] = "1";
				g_all_position_data[index2][2] = "1";
			}
		}
		if (g_all_position_data[index2][2].empty()) {
			
				int temp_grid_number = stoi(g_all_position_data[index2-1][2]) + 1;
				g_all_position_data[index2][2] = to_string(temp_grid_number);
				if (temp_grid_number >= 7) {
					g_all_position_data[index2][2] = "1";
					
					int temp_row_number = stoi(g_all_position_data[index2-1][1]) + 1;
					g_all_position_data[index2][1] = to_string(temp_row_number);
					if (temp_row_number >= 6) {
						g_all_position_data[index2][1] = "1";
					}
				}
		}
		if (g_all_position_data[index2][1].empty()) {
			if (stoi(g_all_position_data[index2][2]) != stoi(g_all_position_data[index2 - 1][2]) + 1) {
				int temp_row_number = stoi(g_all_position_data[index2 - 1][1]) + 1;
				g_all_position_data[index2][1] = to_string(temp_row_number);
				if (temp_row_number > 5) {
					g_all_position_data[index2][1] = "1";
				}
			}
			else {
				g_all_position_data[index2][1] = g_all_position_data[index2 - 1][1];
			}
		}
		if (g_all_position_data[index2][3].empty()) {
			cout << "at " << index2+1 << ",4  (start_number) is empty!" << endl;
		}
		if (g_all_position_data[index2][4].empty()) {
			if (index2 < g_all_position_data.size() - 1 && !g_all_position_data[index2 + 1][3].empty()) {
				int temp_end_number = stoi(g_all_position_data[index2 + 1][3]) - 1;
				if (stoi(g_all_position_data[index2][3]) > temp_end_number) { g_all_position_data[index2][4] = g_all_position_data[index2][3]; }
				else { g_all_position_data[index2][4] = to_string(temp_end_number); }
			}
			else { g_all_position_data[index2][4] = g_all_position_data[index2][3]; }
		}
		g_data_least_row = index2;
	}
}

void CountRow() {
	int count_row_number = stoi(g_row);
	count_row_number = count_row_number + 1;
	if (count_row_number >= 6) {
		g_row = "1";
		return;
	}
	g_row = to_string(count_row_number);
}
void CountGrid() {
	int count_grid_number = stoi(g_grid) + 1;
	if (count_grid_number >= 7) {
		CountRow();
		g_grid = "1";
		return;
	}
	IntToString(count_grid_number, g_grid);
}
void UpdateCurrentRowDate(int input_which_row) {
	if (input_which_row > g_data_least_row) { cout << "over the least data" << endl;; system("pause"); exit(0); }
	if (!g_all_position_data[input_which_row][0].empty()) {
		g_frame_name = g_all_position_data[input_which_row][0];
	}
	if (!g_all_position_data[input_which_row][1].empty()) {
		g_row = g_all_position_data[input_which_row][1];
	}
	if (!g_all_position_data[input_which_row][2].empty()) {
		g_grid = g_all_position_data[input_which_row][2];
	}
	if (!g_all_position_data[input_which_row][3].empty()) {
		g_start_number = g_all_position_data[input_which_row][3];
	}
	if (!g_all_position_data[input_which_row][4].empty()) {
		g_next_number = g_all_position_data[input_which_row][4];
	}
}
void MacroAction0(string mode) {
	if (mode == "BACK") {
		Sleep(g_bit_delay_time);
		keybd_event(VK_BACK, 0, 0, 0);
		keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
	}
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	Kcout(g_the_year);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
}
void MacroAction1(string mode) {
	if (mode == "BACK") {
		Sleep(g_bit_delay_time);
		keybd_event(VK_BACK, 0, 0, 0);
		keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
	}
	keybd_event(VK_SHIFT, VK_SHIFT, 0, 0);
	keybd_event(VkKeyScan('`'), 0, 0, 0); //depress
	keybd_event(VkKeyScan('`'), 0, KEYEVENTF_KEYUP, 0); // release!
	keybd_event(VK_SHIFT, VK_SHIFT, KEYEVENTF_KEYUP, 0);
	Kcout(g_next_number);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	Kcout(g_frame_name);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	Kcout(g_row);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	Kcout(g_grid);
	keybd_event(0xd, 0, 0, 0);
	keybd_event(0xd, 0, KEYEVENTF_KEYUP, 0);
}
void MacroAction2(string mode) {
	if (mode == "BACK") {
		Sleep(g_bit_delay_time);
		keybd_event(VK_BACK, 0, 0, 0);
		keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
	}
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_TAB, 0, 0, 0);
	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(0xd, 0, 0, 0);
	keybd_event(0xd, 0, KEYEVENTF_KEYUP, 0);
}
void MacroAction3(string mode) {
	UpdateCurrentRowDate(g_which_row_be_enter);
	if (g_the_year.empty() || g_start_number.empty() || g_next_number.empty() || g_frame_name.empty() || g_row.empty() || g_grid.empty()) {
		cout << "something is empty," << endl;
		system("pause");
		return;
	}
	if (mode == "BACK") {MacroAction0("BACK");}
	else { MacroAction0("NOBACK"); }
	Kcout(g_start_number);
	MacroAction1("NOBACK");
	g_which_row_be_enter = g_which_row_be_enter + 1;
}
void MacroAction4(string mode) {
	UpdateCurrentRowDate(g_which_row_be_enter);
	if (g_the_year.empty() || g_start_number.empty() || g_next_number.empty() || g_frame_name.empty() || g_row.empty() || g_grid.empty()) {
		cout << "something is empty," << endl;
		system("pause");
		return;
	}
	if (mode == "BACK") { MacroAction0("BACK"); }
	else { MacroAction0("NOBACK"); }
	Kcout(g_start_number);
	MacroAction1("NOBACK");
	g_which_row_be_enter = g_which_row_be_enter + 1;

	Sleep(g_wibsite_delay_time);

	MacroAction2("NOBACK");
}

//http://www.cplusplus.com/forum/beginner/9149/
//http://www.sharejs.com/codes/cpp/6324
//https://docs.microsoft.com/zh-tw/windows/desktop/inputdev/virtual-key-codes
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);
	// If key is being pressed
	char press_char = 0;
	if (wParam == WM_KEYDOWN) {
		switch (p->vkCode) {
			// Invisible keys
		case VK_NUMPAD0:
		case VK_NUMPAD1:
		case VK_NUMPAD2:
		case VK_NUMPAD3:
		case VK_NUMPAD4:
		case VK_NUMPAD5:
		case VK_NUMPAD6:
		case VK_NUMPAD7:
		case VK_NUMPAD8:
		case VK_NUMPAD9:
			break;
		case VK_LEFT:
		case VK_UP:
		case VK_RIGHT:
		case VK_DOWN:
		case VK_CAPITAL:
		case VK_SHIFT:
		case VK_LCONTROL:
		case VK_RCONTROL:
		case VK_INSERT:
		case VK_END:
		case VK_PRINT:
		case VK_DELETE:
		case VK_BACK:
			if (p->vkCode == g_macro[0]) { MacroAction0("NOBACK"); }
			else if (p->vkCode == g_macro[1]) { MacroAction1("NOBACK"); CountGrid(); }
			else if (p->vkCode == g_macro[2]) { MacroAction2("NOBACK"); }
			break;
			// Visible keys
		default:
			if (p->vkCode >= 0x41 && p->vkCode <= 0x5A) {
				/*
				press_char = char(tolower(p->vkCode));
				cout << press_char << endl;
				if (press_char == g_macro1) {  MacroAction0("Back"); }
				*/
				if(tolower(p->vkCode)== g_macro[0]) { MacroAction0("BACK"); }
				else if (tolower(p->vkCode) == g_macro[1]) {  MacroAction1("BACK"); }
				else if (tolower(p->vkCode) == g_macro[2]) {  MacroAction2("BACK"); }
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyboardHookProcWithDATA(int nCode, WPARAM wParam, LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);
	// If key is being pressed
	char press_char = 0;
	if (wParam == WM_KEYDOWN||p->vkCode >= 0x41 && p->vkCode <= 0x5A) {
		switch (p->vkCode) {
			// Invisible keys
		case VK_NUMPAD0:
		case VK_NUMPAD1:
		case VK_NUMPAD2:
		case VK_NUMPAD3:
		case VK_NUMPAD4:
		case VK_NUMPAD5:
		case VK_NUMPAD6:
		case VK_NUMPAD7:
		case VK_NUMPAD8:
		case VK_NUMPAD9:
			break;
		case VK_LEFT:
		case VK_UP:
		case VK_RIGHT:
		case VK_DOWN:
		case VK_CAPITAL:
		case VK_SHIFT:
		case VK_LCONTROL:
		case VK_RCONTROL:
		case VK_INSERT:
		case VK_END:
		case VK_PRINT:
		case VK_DELETE:
		case VK_BACK:
			if (p->vkCode == g_macro[2]) { MacroAction2("NOBACK"); }
			else if (p->vkCode == g_macro[3]) { MacroAction3("NOBACK"); }
			else if (p->vkCode == g_macro[4]) { MacroAction4("NOBACK"); }

			break;
			// Visible keys
		default:
			if (p->vkCode >= 0x41 && p->vkCode <= 0x5A) {
				press_char = char(tolower(p->vkCode));
				if (tolower(p->vkCode) == g_macro[2]) { MacroAction2("BACK"); }
				else if (tolower(p->vkCode) == g_macro[3]) { MacroAction3("BACK"); }
				else if (tolower(p->vkCode) == g_macro[4]) { MacroAction4("BACK"); }

			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}



