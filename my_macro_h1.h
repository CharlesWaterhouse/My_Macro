#pragma once

#include<iostream>
#include<string>
#include <vector>

using namespace std;

class FilePositionClass;

void IntToString(int & input_int, string & output_string);

void ReadCsvByVectorFilePositionClass(string & reading_csv, vector<FilePositionClass> & output);

class FilePositionClass
{
	//friend ostream & operator<<(ostream &, FilePositionClass &);

public:
	FilePositionClass();
	//~FilePositionClass();

	void Set_frame_name(string input_frame_name);
	void Set_row(int input_int);
	void Set_grid(int input_int);
	void Set_start_number(int input_int);
	void Set_end_number(int input_int);
	string Get_frame_name();
	int Get_row();
	int Get_grid();
	int Get_start_number();
	int Get_end_number();


protected:
private:
	string frame_name_;
	int row_;
	int grid_;
	int start_number_;
	int end_number_;
};




/*===========up for .h======down for .c==================*/

void IntToString(int & input_int, string & output_string) {
	stringstream temp_ss;
	temp_ss << input_int;
	output_string = temp_ss.str();
}

void ReadCsvByVectorFilePositionClass(string & reading_csv, vector<FilePositionClass> & output) {

	FilePositionClass temp_file_position;
	//readfile
	fstream fs_file;
	fs_file.open(reading_csv);
	string temp_one_line;
	vector<string> temp_one_data;
	temp_one_data.clear();
	temp_one_data.resize(5);
	getline(fs_file, temp_one_line, '\n');//ignore first row data
	while (getline(fs_file, temp_one_line, '\n')) {//讀檔讀到跳行字元
		istringstream temp_one_line_iss(temp_one_line); // string to stream
		string temp_grip_dat;
		for (int i = 0;; i++) {
			if (!getline(temp_one_line_iss, temp_grip_dat, ',')) { break; }
			else if (i >= 5 || i < 0) { cout << "extra unexpected data apprear in csv"; system("pause"); break; }
			else if (i == 0 && !temp_grip_dat.empty()) { temp_one_data[0] = temp_grip_dat; }
			else if (i == 1 && !temp_grip_dat.empty()) { temp_one_data[1] = temp_grip_dat; }
			else if (i == 2 && !temp_grip_dat.empty()) { temp_one_data[2] = temp_grip_dat; }
			else if (i == 2 && temp_grip_dat.empty()) {
				cout << "line144" << endl;
				int temp_grip_number = stoi(temp_one_data[2]) + 1;
				if (temp_grip_number > 6) {
					temp_one_data[2] = "1";
					int temp_row_number = stoi(temp_one_data[1]) + 1;
					if (temp_row_number > 5) {
						temp_one_data[1] = "1";
					}
					IntToString(temp_row_number, temp_one_data[1]);
					continue;
				}
				IntToString(temp_grip_number, temp_one_data[2]);
			}
			else if (i == 3 && !temp_grip_dat.empty()) {
				temp_one_data[3] = temp_grip_dat;
				int temp_end_number = stoi(temp_one_data[3]) + 100;
				IntToString(temp_end_number, temp_one_data[4]);
			}
			else if (i == 4 && !temp_grip_dat.empty()) { temp_one_data[4] = temp_grip_dat; }
		}
		for (int i = 0; i < temp_one_data.size(); i++) {
			cout << temp_one_data[i] << endl;
		}
		temp_file_position.Set_frame_name(temp_one_data[0]);
		temp_file_position.Set_row(stoi(temp_one_data[1]));
		temp_file_position.Set_grid(stoi(temp_one_data[2]));
		temp_file_position.Set_start_number(stoi(temp_one_data[3]));
		temp_file_position.Set_end_number(stoi(temp_one_data[4]));
		output.push_back(temp_file_position);
	}
	fs_file.close();
	for (int i = 0; i < output.size(); i++) {
		cout << output[i].Get_frame_name() << ",";
		cout << output[i].Get_row() << ",";
		cout << output[i].Get_grid() << ",";
		cout << output[i].Get_start_number() << ",";
		cout << output[i].Get_end_number() << "," << endl;
	}


}

/*class FilePositionClass
{
	//friend ostream & operator<<(ostream &, FilePositionClass &);

public:
	FilePositionClass();
	//~FilePositionClass();

	void Set_frame_name(string input_frame_name);
	void Set_row(int input_int);
	void Set_grid(int input_int);
	void Set_start_number(int input_int);
	void Set_end_number(int input_int);
	string Get_frame_name();
	int Get_row();
	int Get_grid();
	int Get_start_number();
	int Get_end_number();


protected:
private:
	string frame_name_;
	int row_;
	int grid_;
	int start_number_;
	int end_number_;
};*/
FilePositionClass::FilePositionClass() :frame_name_("-1"), row_(-1), grid_(-1), start_number_(-1), end_number_(-1) {};
void FilePositionClass::Set_frame_name(string input_frame_name) { frame_name_ = input_frame_name; }
void FilePositionClass::Set_row(int input_int) { row_ = input_int; }
void FilePositionClass::Set_grid(int input_int) { grid_ = input_int; }
void FilePositionClass::Set_start_number(int input_int) { start_number_ = input_int; }
void FilePositionClass::Set_end_number(int input_int) { end_number_ = input_int; }
string FilePositionClass::Get_frame_name() { return frame_name_; }
int FilePositionClass::Get_row() { return row_; }
int FilePositionClass::Get_grid() { return grid_; }
int FilePositionClass::Get_start_number() { return start_number_; }
int FilePositionClass::Get_end_number() { return end_number_; }


