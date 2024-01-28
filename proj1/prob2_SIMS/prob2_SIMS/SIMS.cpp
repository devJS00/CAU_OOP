#include <iostream>
#include <fstream>
#include <string>
#include<iomanip>
#include <algorithm>
using namespace std;

typedef struct Student {
	string student_name, student_id, student_birth_year, student_dept, student_tel;
	string student_admission_year;
} Student;

void Sort(Student *stdt, int count, int sorting_option) { // sorting fuction
	if (sorting_option == 1)
	{
		for (int i = 0; i < count; i++)
		{
			for (int j = i; j < count; j++)
			{
				if (stdt[i].student_name > (stdt[j].student_name)) 
				{
					Student temp;
					temp = stdt[i];
					stdt[i] = stdt[j];
					stdt[j] = temp;

				}
			}
		}
	}

	else if (sorting_option == 2)
	{
		for (int i = 0; i < count; i++)
		{
			for (int j = i; j < count; j++)
			{
				if (stdt[i].student_id > (stdt[j].student_id))
				{
					Student temp;
					temp = stdt[i];
					stdt[i] = stdt[j];
					stdt[j] = temp;

				}
			}
		}
	}

	else if (sorting_option == 3)
	{
		for (int i = 0; i < count; i++)
		{
			for (int j = i; j < count; j++)
			{
				if (stdt[i].student_admission_year > (stdt[j].student_admission_year))
				{
					Student temp;
					temp = stdt[i];
					stdt[i] = stdt[j];
					stdt[j] = temp;

				}
			}
		}
	}

	else if (sorting_option == 4)
	{
		for (int i = 0; i < count; i++)
		{
			for (int j = i; j < count; j++)
			{
				if (stdt[i].student_dept > (stdt[j].student_dept))
				{
					Student temp;
					temp = stdt[i];
					stdt[i] = stdt[j];
					stdt[j] = temp;

				}
			}
		}
	}
}


int main() {
	int sorting_option = 1; // default sorting option is sort by name

	while (1)
	{
		int selected_num;

		cout << "1. Insertion\n2. Search\n3. Sorting Option\n4. Exit" << endl;
		cout << "> ";
		cin >> selected_num;
		cout << "\n";

		if (selected_num == 1) // selected Insertion
		{
			string name, department, id, student_birth_year, tel;

			cout << "Name ? ";
			cin.ignore();
			getline(cin, name);

			cout << "Student ID (10 digits) ? ";
			cin >> id;

			cout << "Birth Year (4 digits) ? ";
			cin >> student_birth_year;

			cout << "Department ? ";
			cin.ignore();
			getline(cin, department);

			cout << "Tel ? ";
			cin >> tel;
			cout << "\n";

			int offset;
			string line;
			bool already_inserted = false;

			ifstream myfile; // stream class to read from files
			myfile.open("file1.txt");
			if (myfile.is_open())
			{
				while (!myfile.eof())
				{
					getline(myfile, line);
					if ((offset = line.find(id, 0)) != string::npos) // check whether the same student id is provided
					{
						cout << "Error : Already inserted" << endl;
						cout << "\n";
						already_inserted = true;
						myfile.close();
						break;
					}
				}
			}

			if (already_inserted == false) // if inserted student id doesn't exists 
			{
				ofstream myfile("file1.txt", ios::app);  // stream class to write on files
				if (myfile.is_open())
				{
					myfile << std::setw(17) << left << name;
					myfile << std::setw(12) << left << id;
					myfile << std::setw(22) << left << department;
					myfile << std::setw(11) << left << student_birth_year;
					myfile << std::setw(13) << left << tel << endl;
					myfile.close();
				}
				else cout << "Unable to open file" << endl;
			}

		}

		else if (selected_num == 2) // selected Search
		{
			int selected_num2;
			string search_keyword;
			cout << "- Search -\n1. Search by name\n2. Search by student ID(10 numbers)\n3. Search by admission year(4 numbers)\n4. Search by department name\n5. List All" << endl;
			cout << "> ";
			cin >> selected_num2;
			cout << "\n";

			if (selected_num2 == 1) // selected Search by name
			{
				cout << "Name keyword? ";
				cin.ignore();
				getline(cin, search_keyword);
				cout << "\n";

				int offset;
				string line;
				Student stdt[100];
				int count = 0;
				ifstream myfile;
				myfile.open("file1.txt");
				if (myfile.is_open())
				{
					while (!myfile.eof())
					{
						getline(myfile, line);
						if ((offset = line.find(search_keyword, 0)) != string::npos) // if inserted name is found in the file
						{
							/* store the student's name. id, admission year, etc. in array of structures 'Student' respectively */
							stdt[count].student_name = line.substr(0, 17);
							stdt[count].student_id = line.substr(17, 12);
							stdt[count].student_admission_year = line.substr(17, 4);
							stdt[count].student_dept = line.substr(29, 22);
							stdt[count].student_birth_year = line.substr(51, 11);
							stdt[count].student_tel = line.substr(62, 13);
							count++;
						}
					}
					myfile.close();
				}
				else cout << "Unable to open file";

				Sort(stdt, count, sorting_option); // sort the student records stored in array of structures 'Student'

				cout << "Name             Student ID  Dept                  Birth Year Tel          " << endl;
				for (int i = 0; i < count; i++)
				{
					cout << std::setw(17) << left << stdt[i].student_name;
					cout << std::setw(12) << left << stdt[i].student_id;
					cout << std::setw(22) << left << stdt[i].student_dept;
					cout << std::setw(11) << left << stdt[i].student_birth_year;
					cout << std::setw(13) << left << stdt[i].student_tel << endl;
				}
				cout << "\n";
			}

			else if (selected_num2 == 2) // selected Search by student id
			{
				cout << "Student ID? ";
				cin >> search_keyword;
				cout << "\n";

				int offset;
				string line;
				ifstream myfile;
				myfile.open("file1.txt");
				if (myfile.is_open())
				{
					while (!myfile.eof())
					{
						getline(myfile, line);
						if ((offset = line.find(search_keyword, 0)) != string::npos) // if inserted student id is found in the file
						{
							cout << "Name             Student ID  Dept                  Birth Year Tel          " << endl;
							cout << line << endl; // just print out the line that contains that student id
						}
					}
					myfile.close();
				}
				else cout << "Unable to open file";
				cout << "\n";
			}

			else if (selected_num2 == 3) // selected Search by admission year
			{
				cout << "Admission year? ";
				cin >> search_keyword;
				cout << "\n";

				int offset;
				string line;
				Student stdt[100];
				int count = 0;
				ifstream myfile;
				myfile.open("file1.txt");
				if (myfile.is_open())
				{
					while (!myfile.eof())
					{
						getline(myfile, line);
						if ((offset = line.find(search_keyword, 0)) != string::npos) // if inserted admission year is found in the file
						{
							/* store the student's name. id, admission year, etc. in array of structures 'Student' respectively */
							stdt[count].student_name = line.substr(0, 17);
							stdt[count].student_id = line.substr(17, 12);
							stdt[count].student_admission_year = line.substr(17, 4);
							stdt[count].student_dept = line.substr(29, 22);
							stdt[count].student_birth_year = line.substr(51, 11);
							stdt[count].student_tel = line.substr(62, 13);
							count++;
						}
					}
					myfile.close();
				}
				else cout << "Unable to open file";

				Sort(stdt, count, sorting_option); // sort the student records stored in array of structures 'Student'

				cout << "Name             Student ID  Dept                  Birth Year Tel          " << endl;

				for (int i = 0; i < count; i++)
				{
					cout << std::setw(17) << left << stdt[i].student_name;
					cout << std::setw(12) << left << stdt[i].student_id;
					cout << std::setw(22) << left << stdt[i].student_dept;
					cout << std::setw(11) << left << stdt[i].student_birth_year;
					cout << std::setw(13) << left << stdt[i].student_tel << endl;
				}
				cout << "\n";
			}


			else if (selected_num2 == 4) // selected Search by department name
			{
				cout << "Department name keyword? ";
				cin.ignore();
				getline(cin, search_keyword);
				cout << "\n";

				int offset;
				string line;
				Student stdt[100];
				int count = 0;
				ifstream myfile;
				myfile.open("file1.txt");
				if (myfile.is_open())
				{
					while (!myfile.eof())
					{
						getline(myfile, line);
						if ((offset = line.find(search_keyword, 0)) != string::npos) // if inserted department name is found in the file
						{
							/* store the student's name. id, admission year, etc. in array of structures 'Student' respectively */
							stdt[count].student_name = line.substr(0, 17);
							stdt[count].student_id = line.substr(17, 12);
							stdt[count].student_admission_year = line.substr(17, 4);
							stdt[count].student_dept = line.substr(29, 22);
							stdt[count].student_birth_year = line.substr(51, 11);
							stdt[count].student_tel = line.substr(62, 13);
							count++;
						}
					}
					myfile.close();
				}
				else cout << "Unable to open file";

				Sort(stdt, count, sorting_option); // sort the student records stored in array of structures 'Student'

				cout << "Name             Student ID  Dept                  Birth Year Tel          " << endl;
				for (int i = 0; i < count; i++)
				{
					cout << std::setw(17) << left << stdt[i].student_name;
					cout << std::setw(12) << left << stdt[i].student_id;
					cout << std::setw(22) << left << stdt[i].student_dept;
					cout << std::setw(11) << left << stdt[i].student_birth_year;
					cout << std::setw(13) << left << stdt[i].student_tel << endl;
				}
				cout << "\n";
			}

			else if (selected_num2 == 5) // selected List All
			{
				string line;
				Student stdt[100];
				int count = 0;
				ifstream myfile;
				myfile.open("file1.txt");
				if (myfile.is_open())
				{
					while (getline(myfile, line))
					{
						/* store every student's name. id, admission year, etc. in array of structures 'Student' respectively */
						stdt[count].student_name = line.substr(0, 17);
						stdt[count].student_id = line.substr(17, 12);
						stdt[count].student_admission_year = line.substr(17, 4);
						stdt[count].student_dept = line.substr(29, 22);
						stdt[count].student_birth_year = line.substr(51, 11);
						stdt[count].student_tel = line.substr(62, 13);
						count++;
					}
					myfile.close();
				}
				else cout << "Unable to open file";

				Sort(stdt, count, sorting_option); // sort the student records stored in array of structures 'Student'

				cout << "Name             Student ID  Dept                  Birth Year Tel          " << endl;
				for (int i = 0; i < count; i++)
				{
					cout << std::setw(17) << left << stdt[i].student_name;
					cout << std::setw(12) << left << stdt[i].student_id;
					cout << std::setw(22) << left << stdt[i].student_dept;
					cout << std::setw(11) << left << stdt[i].student_birth_year;
					cout << std::setw(13) << left << stdt[i].student_tel << endl;
				}
				cout << "\n";
			}

		}

		else if (selected_num == 3) // selected Sorting Option
		{
			cout << "- Sorting Option\n1. Sort by Name\n2. Sort by Student ID\n3. Sort by Admission Year\n4. Sort by Department name" << endl;
			cout << "> ";
			cin >> sorting_option;
			cout << "\n";
		}

		else if (selected_num == 4) // selected Quit
			return 0;
	}
}