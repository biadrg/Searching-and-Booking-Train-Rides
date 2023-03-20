#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <ctime>
#include <cctype>
#include <stdio.h>
#include <cstdlib>

/*i know goto is bad practice, but for the sake of this whole thing wroking 
the way my mind works, let's go along with it*/

/*parola operatorului e deja excriptata, psss! the password is "bug"*/

using namespace std;

string credentials[50][2];
string encrypted_credentials[50][2];
string rides[50][3];
string booking[50][6];

int userCount = 0;
int rideCount = -1;
int bookCount = -1;
int key = 991;
//991 is a great prime number so

string encrypt(string str)
{
	for(int i = 0; i < str.length(); i ++)
	{
		str[i] = str[i] + key;
		
	}
	return str;
}

string decrypt(string str)
{
	for(int i = 0; i < str.length(); i ++)
	{
		str[i] = str[i] - key;
	}
	return str;
}

void readOp()
{
	ifstream file;
    file.open("Account.csv");
    if(file.fail())
    {
        cout << "error" << endl;
        exit(1);
	}
	
	for(int i = 0; i < userCount + 1; i ++)
	{
		getline(file, credentials[i][0], ',');
		getline(file, encrypted_credentials[i][1], '\n');
		credentials[i][1] = decrypt(encrypted_credentials[i][1]);
	}
    file.close();
}

int stoi(string date)
{
	int number;
	stringstream container(date);
	container >> number;
	return number;
}

bool validEmail(string email)
{
	int i;
	int at, dot;
	int atCount = 0, dotCount = 0, afterAtCount = 0, afterDotCount = 0;
	//email must start with an alphabet character
	if(!(email[0] >= 'a' && email[0] <= 'z') || (email[0] >= 'A' && email[0] <= 'Z'))
	{
		return 0;
	}
	//counting the @
	for(i = 0; i < email.length(); i ++)
	{
		if(email[i] == '@')
		{
			at = i;
			atCount ++;
		}
	}
	//there must be only one @
	if(atCount != 1)
	{
		return 0;
	}
	//counting the dots
	for(i = email.length(); i > at; i --)
	{
		if(email[i] == '.')
		{
			dot = i;
			dotCount ++;
		}
	}
	//there must be at least one dot
	if(dotCount < 1)
	{
		return 0;
	}
	//there must be at least two characters between @ and the last dot
	for(i = at; i < dot; i ++)
	{
		afterAtCount ++;
	}	
	//there must be at least two characters after the last dot
	for(i = dot; i < email.length(); i ++)
	{
		afterDotCount ++;
	}
	//checking whether the last two happen or not
	if(!(afterDotCount > 1 && afterAtCount > 1))
	{
		return 0;
	}
	return 1;
}

bool strongPassword(string password)
{
	int lowercase = 0, uppercase = 0, number = 0, special = 0;
	if(password.length() < 12)
	{
		return 0;
	}
	for(int i = 0; i < password.length(); i ++)
	{
		if(password[i] >= 'a' && password[i] <= 'z')
		{
			lowercase = 1;
		}
		if(password[i] >= 'A' && password[i] <= 'Z')
		{
			uppercase = 1;
		}
		if((int)(password[i])  >= 48 && (int)(password[i]) <= 57)
		{
			number = 1;
		}
		if((int)(password[i])  >= 32 && (int)(password[i]) <= 47)
		{
			special = 1;
		}
		if((int)(password[i])  >= 58 && (int)(password[i]) <= 64)
		{
			special = 1;
		}
		if((int)(password[i])  >= 91 && (int)(password[i]) <= 96)
		{
			special = 1;
		}
		if((int)(password[i])  >= 123 && (int)(password[i]) <= 126)
		{
			special = 1;
		}
	}
	if(lowercase == 0 || uppercase == 0 || number == 0 || special == 0)
	{
		return 0;
	}
	return 1;
}

bool actualCity(string city)
{
	for(int i = 0; i < city.length(); i ++)
	{
		if(!isalpha(city[i]))
		{
			if(!isspace(city[i]) && city[i] != '-')
			{
				return 0;				
			}
		}
	}
	return 1;
}

bool correctDate(string date)
{
	int day, month, year;
	string temp;
	
	if(date.length() != 10)
	{
		return 0;
	}
	
	day = stoi(date.substr(0, 2));
	month = stoi(date.substr(3, 5));
	year = stoi(date.substr(6, 10));
	
	if(year < 1 || month < 1 || day < 1)
	{
		return 0;
	}
	if(month >= 1 && month <= 12)
	{
		if(month == 2)
		{
			if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
			{
				if(day > 29)
				{
					return 0;
				}
			}
			else
			{
				if(day > 28)
				{
					return 0;
				}
			}
		}
		else
		{
			if(month == 4 || month == 6 || month == 9 || month == 11)
			{
				if(day > 30)
				{
					return 0;
				}
			}
			else
			{
				if(day > 31)
				{
					return 0;
				}
			}
		}
	}
	else
	{
		return 0;
	}
	return 1;
}

bool futureDate(string date)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	
	int currentDay = ltm->tm_mday;
	int currentMonth = 1 + ltm->tm_mon;
	int currentYear = 1900 + ltm->tm_year;
	
	int day = stoi(date.substr(0, 2));
	int month = stoi(date.substr(3, 5));
	int year = stoi(date.substr(6, 10));

	if(year > currentYear)
	{
		return 1;
	}
	else
	{
		if(year < currentYear)
		{
			return 0;
		}
		else
		{
			if(month > currentMonth)
			{
				return 1;
			}
			else
			{
				if(month < currentMonth)
				{
					return 0;
				}
				else
				{
					if(day > currentDay)
					{
						return 1;
					}
					else
					{
						if(day < currentDay)
						{
							return 0;
						}
						else
						{
							return 0;
						}
					}
				}
			}
		}
	}
}

bool correctCnp(string cnp)
{
	if(cnp.length() != 13)
	{
		return 0;
	}
	
	int birthYear = stoi(cnp.substr(1, 2));
	int birthMonth = stoi(cnp.substr(3, 2));
	int birthDay = stoi(cnp.substr(5, 2));
	
	if(birthMonth < 1 || birthMonth > 12)
	{
		return 0;
	}
	if(birthDay < 1 || birthDay > 31)
	{
		return 0;
	}
	if(cnp[0] != '1' && cnp[0] != '2' && cnp[0] != '5' && cnp[0] != '6')
	{
		return 0;
	}

	return 1;	
}

class User
{
    private:
        string email, username, password, password2;
    public:
        void userSignup()
        {
			cout << "------------------------------\n";
			cout << "\tSignup details:";
			cout << "\n------------------------------\n";
			cout << "\nEmail address: ";
			cin >> email;
			if(!validEmail(email))
			{
				throw "Invalid email.\nCriteria to be checked:\n1) Email must start with alphabet char;\n2) There must be only one (at);\n3) There msut be at least one (dot);\n4) At least two characters between (at) and (dot) as well as after (dot).\n";
			}
			cout << "Username: ";
			cin >> username;
			cout << "Password: ";
			cin >> password;
			if(!strongPassword(password))
			{
				throw "You'll need a password stronger than that.\nCriteria to be checked:\n1) min. 12 characters;\n2) at least one lowercase, uppercase, number & special character.\n";
			}
			cout << "Repeat password: ";
			cin >> password2;
			if(password != password2)
			{
				throw "Passwords do not match.\n";
			}

			readOp();
			userCount++;
			
			ofstream file;
		    file.open("Account.csv");
		    if (file.fail())
		    {
		        cout << "error" << endl;
		        exit(1);
		    }
			
			credentials[userCount][0] = username;
			credentials[userCount][1] = password;
			encrypted_credentials[userCount][1] = encrypt(password);

			for(int i = 0; i < userCount + 1; i ++)
			{
				file << credentials[i][0] << ",";
				file << encrypted_credentials[i][1] << "\n";
			}
			file.close();
			cout << "\nAccount succesfully created.\n\n";
        }
        void userLogin()
        {
			cout << "------------------------------\n";
			cout << "\tLogin details:";
			cout << "\n------------------------------\n";
			cout << "\nUsername: ";
			cin >> username;
			cout << "Password: ";
			cin >> password;

			int done = 0;
			for(int i = 1; i < userCount + 1; i ++)
			{
				if(username == credentials[i][0] && password == credentials[i][1])
				{
					done = 1;
					cout << "\nLogin was succesful.\n";
				}
			}
			if(!done)
			{
				throw "Incorrect. Try again.\n";
			}
			cout << endl;			
        }
        void searchRide()
        {
        	string start, stop, date;
			cout << "-----------------------------------\n";
			cout << "\tTrain rides details:";
			cout << "\n-----------------------------------\n";
			cout << "\nDe la: ";
			getline(cin >> ws, start);
			if(!actualCity(start))
			{
				throw "That place does not exist.\n";
			}
			cout << "Pana la: ";
			getline(cin >> ws, stop);
			if(!actualCity(stop))
			{
				throw "That place does not exist.\n";
			}
			cout << "Data in format ZZ.LL.AAAA: ";
			getline(cin >> ws, date);
			if(!correctDate(date))
			{
				throw "Date format is incorrect.\n";
			}
			if(!futureDate(date))
			{
				throw "Date is in the past, dingus.\n";
			}

			int done = 0;
			for(int i = 0; i < rideCount + 1; i ++)
			{
				if(start == rides[i][0] && stop == rides[i][1] && date == rides[i][2])
				{
					done = 1;
					cout << "\nTrain ride was found.\n";
					string response;
					cout << "\nDo you wish to book it?";
					cout << "\n\"1\" for no";
					cout << "\n\"2\" for yes";
					cout << "\nYour response: ";
					cin >> response;
					
					int answer = stoi(response);
					if(answer != 1 && answer != 2)
					{
						cout << "\nInvalid response, it's out of bounds.\n";
						sleep(1);
						return;				
					}
					
					switch(answer)
					{
						case 1:
							cout << "\nAight then.";
							sleep(1);
							break;
						case 2:
							bookRide(i);
							break;				
					}	
				}
			}
			if(!done)
			{
				throw "Train ride does not exist.\n";
			}
        }
        void bookRide(int index)
        {
        	string name, type, cnp;
            cout << "\nWe'll need some more details from you then.\n\n";
            cnp_was_wrong:
  			cout << "-----------------------------------\n";
			cout << "\tPurchaser details:";
			cout << "\n-----------------------------------\n";
			cout << "\nFull Name: ";
			getline(cin >> ws, name);
			cout << "CNP: ";
			cin >> cnp;
			if(!correctCnp(cnp))
			{
				goto cnp_was_wrong;
			}
			cout << "Tip calator (adult/elev/student): ";
			cin >> type;
			
			bookCount ++;
			booking[bookCount][0] = rides[index][0];
			booking[bookCount][1] = rides[index][1];
			booking[bookCount][2] = rides[index][2];
			booking[bookCount][3] = name;
			booking[bookCount][4] = cnp;
			booking[bookCount][5] = type;
			
			sleep(1);
			system("cls");
			
			cout << "Printing your ticket ...\n\n";
			sleep(1);
			
			cout << "-----------------------------------\n";
			cout << "\t**Ticket details**\n";
			cout << "-----------------------------------\n\n";
			cout << booking[bookCount][0] << "---->" << booking[bookCount][1];
			cout << "\nDate of ride: " << booking[bookCount][2];
			cout << "\n\n-----------------------------------\n";
			cout << "\nBooked by:\n" << booking[bookCount][3] << ", " << booking[bookCount][5];
			cout << "\nCNP: " << booking[bookCount][4];
			cout << "\n\n-----------------------------------\n\n";
			
			ofstream ticket;
		    ticket.open("Tickets.csv");
		    if (ticket.fail())
		    {
		        cout << "error" << endl;
		        exit(1);
		    }
			for(int i = 0; i < bookCount + 1; i ++)
            {
				ticket << booking[i][0] << ",";
				ticket << booking[i][1] << ",";
				ticket << booking[i][2] << ",";
				ticket << booking[i][3] << ",";
				ticket << booking[i][4] << ",";
				ticket << booking[i][5] << "\n";            	
			}
			ticket.close();
        }
};

class Operator
{
    private:
        string username, password, usernameOp, passwordOp;
    public:
        void operatorLogin()
        {
			cout << "------------------------------\n";
			cout << "\tLogin details:";
			cout << "\n------------------------------\n";
			cout << "\nUsername: ";
			cin >> username;
			cout << "Password: ";
			cin >> password;
	
			readOp();
			
			usernameOp = credentials[0][0];
			passwordOp = credentials[0][1];
			
			if(username != usernameOp || password != passwordOp)		
			{
				throw "Incorrect. Try again.\n";
			}
			else
			{
				cout << "\nLogin was succesful.\n\n";
			}
        }
        void addRides()
        {
        	string start, stop, date;
			cout << "-----------------------------------\n";
			cout << "\tTrain rides details:";
			cout << "\n-----------------------------------\n";
			cout << "\nDe la: ";
			getline(cin >> ws, start);
			if(!actualCity(start))
			{
				throw "That place does not exist.\n";
			}
			cout << "Pana la: ";
			getline(cin >> ws, stop);
			if(!actualCity(stop))
			{
				throw "That place does not exist.\n";
			}
			cout << "Data in format ZZ.LL.AAAA: ";
			getline(cin >> ws, date);
			if(!correctDate(date))
			{
				throw "Date format is incorrect.\n";
			}
			if(!futureDate(date))
			{
				throw "Date is in the past, dingus.\n";
			}
			
			rideCount ++;
			rides[rideCount][0] = start;
			rides[rideCount][1] = stop;
			rides[rideCount][2] = date;

			ofstream ride;
		    ride.open("Ride.csv");
		    if (ride.fail())
		    {
		        cout << "error" << endl;
		        exit(1);
		    }
			
			for(int i = 0; i < rideCount + 1; i ++)
            {
				ride << rides[i][0] << ",";
				ride << rides[i][1] << ",";
				ride << rides[i][2] << "\n";            	
			}
			
			ride.close();
			
			cout << "\nTrain ride succesfully created.\n";
        }
        void removeRides()
        {
        	system("cls");
        	string choice;
			cout << "-----------------------------------\n";
            cout << "\tTrain rides index:";
			cout << "\n-----------------------------------\n";
            for(int i = 0; i < rideCount + 1; i ++)
            {
            	cout << "\nRide number " << i+1 << ":\n";
            	for(int j = 0; j < 3; j ++)
            	{
            		cout << rides[i][j] << "\t";
				}
				cout << endl;
			}
			cout << "\nWhich train ride number do you want removed?";
			cout << "\nYour response: ";
			cin >> choice;
			
			int response = stoi(choice);
			if(response > rideCount + 1 || response == 0)
			{
				cout << "\nInvalid response, it's out of bounds.\n";
				sleep(1);
				return;
			}
			
			response--;
			rideCount--;
			string newRides[50][3] = rides;
            for(int i = response; i < rideCount + 1; i ++)
            {
            	for(int j = 0; j < 3; j ++)
            	{
            		newRides[i][j] = rides[i+1][j];
				}
			}
			
			memcpy(rides, newRides, sizeof(rides));

			cout << "\n-----------------------------------------\n";
			cout << "\tUpdated train rides index:";
			cout << "\n-----------------------------------------\n";
            for(int i = 0; i < rideCount + 1; i ++)
            {
            	cout << "\nRide number " << i+1 << ":\n";
            	for(int j = 0; j < 3; j ++)
            	{
            		cout << rides[i][j] << "\t";
				}
				cout << endl;
			}
			
			ofstream ride;
		    ride.open("Ride.csv");
		    if (ride.fail())
		    {
		        cout << "error" << endl;
		        exit(1);
		    }
			
			for(int i = 0; i < rideCount + 1; i ++)
            {
				ride << rides[i][0] << ",";
				ride << rides[i][1] << ",";
				ride << rides[i][2] << "\n";
			}
			
			ride.close();	
        }
};

int main()
{
	User user;
	Operator op;
	int done;
	string choice, response;

	action:
	cout << "------------------------------------\n";
	cout << "\tChoose user type:\n";
	cout << "------------------------------------\n";
	cout << "\n\"1\" for operator";
	cout << "\n\"2\" for basic user";
	cout << "\n\nYour choice: ";
	cin >> choice;
	system("cls");
	if(!stoi(choice))
	{
		cout << "Wrong. You did not capiche.\n\n";
		sleep(1);
		goto action;
	}
	switch(stoi(choice))
	{
		case 1:
			//You have chosen operator
			done = 0;
			do
			{
				try
				{
					op.operatorLogin();
					done = 1;
				}
				catch(const char* msg)
				{
					cerr << endl << msg << endl;
				}
				sleep(1);				
			}while(!done);
			op:
			cout << "--------------------------------------\n";
			cout << "\tChoose desired action:";
			cout << "\n--------------------------------------\n";
			cout << "\n\"1\" for adding new train rides.";
			cout << "\n\"2\" for removing train rides.";
			cout << "\n\"3\" for loging out.";
			cout << "\n\nYour response: ";
			cin >> response;
			if(!stoi(response))
			{
				cout << "Wrong. You did not capiche.\n\n";
				sleep(1);
				goto op;
			}
			switch(stoi(response))
			{
				case 1:
					add:
					system("cls");
					done = 0;
					do
					{
						try
						{
							done--;
							op.addRides();
							done = 1;
						}
						catch(const char* msg)
						{
							cerr << endl << msg << endl;
							if(done < -1)
							{
								cout << "Dou you want to give up?\n\"1\" for no.\n\"2\" for yes.";
								cout << "\nYour response: ";
								cin >> choice;
								cout << endl;
								if(!stoi(choice))
								{
									cout << "Wrong. You did not capiche.\n\n";
									sleep(1);
									goto action;
								}
								switch(stoi(choice))
								{
									case 1:
										goto add;
										break;
									case 2:
										cout << "You had a good run, bud.\n\n";
										sleep(1);
										goto us;
										break;
									default:
										cout << "\nWrong. You did not capiche.";
										break;			
								}
							}
						}
						sleep(1);				
					}while(done != 1);
					goto op;
					break;
				case 2:
					remove:
					done = 0;
					do
					{
						try
						{
							done--;
							op.removeRides();
							done = 1;
						}
						catch(const char* msg)
						{
							cerr << endl << msg << endl;
							if(done < -1)
							{
								cout << "Dou you want to give up?\n\"1\" for no.\n\"2\" for yes.";
								cout << "\nYour response: ";
								cin >> choice;
								cout << endl;
								if(!stoi(choice))
								{
									cout << "Wrong. You did not capiche.\n\n";
									sleep(1);
									goto action;
								}
								switch(stoi(choice))
								{
									case 1:
										goto remove;
										break;
									case 2:
										cout << "You had a good run, bud.\n\n";
										sleep(1);
										goto us;
										break;
									default:
										cout << "\nWrong. You did not capiche.";
										break;			
								}
							}
						}
						sleep(1);				
					}while(done != 1);
					goto op;
					break;
				case 3:
					cout << "\nLogged out.";
					sleep(1);
					break;
				default:
					cout << "Wrong. You did not capiche.\n\n";
					sleep(1);
					goto action;
					break;
			}
			break;
		case 2:
			//You have chosen user
			us:
			cout << "--------------------------------------\n";
			cout << "\tChoose desired action:";
			cout << "\n--------------------------------------\n";
			cout << "\n\"1\" for creating new account.";
			cout << "\n\"2\" for loging in.";
			cout << "\n\"3\" for searching a train ride.";
			cout << "\n\"4\" for loging out.";
			cout << "\n\nYour response: ";
			cin >> response;
			system("cls");
			if(!stoi(response))
			{
				cout << "Wrong. You did not capiche.\n\n";
				sleep(1);
				goto us;
			}
			switch(stoi(response))
			{
				case 1:
					signup:
					done = 0;
					do
					{
						try
						{
							done--;
							user.userSignup();
							done = 1;
						}
						catch(const char* msg)
						{
							cout << endl << msg << endl;
							sleep(1);
							if(done < -1)
							{
								cout << "Dou you want to give up?\n\"1\" for no.\n\"2\" for yes.";
								cout << "\nYour response: ";
								cin >> choice;
								cout << endl;
								if(!stoi(choice))
								{
									cout << "Wrong. You did not capiche.\n\n";
									sleep(1);
									goto action;
								}
								switch(stoi(choice))
								{
									case 1:
										goto signup;
										break;
									case 2:
										cout << "You had a good run, bud.\n\n";
										sleep(1);
										goto us;
										break;
									default:
										cout << "\nWrong. You did not capiche.";
										break;			
								}
							}
						}				
					}while(done != 1);
					goto us;
					break;
				case 2:
					login:
					done = 0;
					do
					{
						try
						{
							done--;
							user.userLogin();
							done = 1;
						}
						catch(const char* msg)
						{
							cout << endl << msg << endl;
							sleep(1);
							if(done < -1)
							{
								cout << "Dou you want to give up?\n\"1\" for no.\n\"2\" for yes.";
								cout << "\nYour response: ";
								cin >> choice;
								cout << endl;
								if(!stoi(choice))
								{
									cout << "Wrong. You did not capiche.\n\n";
									sleep(1);
									goto action;
								}
								switch(stoi(choice))
								{
									case 1:
										goto login;
										break;
									case 2:
										cout << "You had a good run, bud.\n\n";
										sleep(1);
										goto us;
										break;
									default:
										cout << "\nWrong. You did not capiche.";
										break;			
								}
							}
						}			
					}while(done != 1);
					goto us;
					break;
				case 3:
					search:
					done = 0;
					do
					{
						try
						{
							done--;
							user.searchRide();
							done = 1;
						}
						catch(const char* msg)
						{
							cout << endl << msg << endl;
							sleep(1);
							if(done < -1)
							{
								cout << "Dou you want to give up?\n\"1\" for no.\n\"2\" for yes.";
								cout << "\nYour response: ";
								cin >> choice;
								cout << endl;
								if(!stoi(choice))
								{
									cout << "Wrong. You did not capiche.\n\n";
									sleep(1);
									goto action;
								}
								switch(stoi(choice))
								{
									case 1:
										goto search;
										break;
									case 2:
										cout << "You had a good run, bud.\n\n";
										sleep(1);
										goto us;
										break;
									default:
										cout << "\nWrong. You did not capiche.";
										break;			
								}
							}
						}				
					}while(done != 1);
					goto us;					
					break;
				case 4:
					cout << "\nLogged out.";
					sleep(1);
					break;
				default:
					cout << "Wrong. You did not capiche.\n\n";
					sleep(1);
					goto us;
					break;
			}
			break;
		default:
			cout << "Wrong. You did not capiche.\n\n";
			sleep(1);
			goto action;
			break;
	}
	
	cout << "\n\nAre you finished?\n\"1\" for no.\n\"2\" for yes.";
	cout << "\nYour response: ";
	cin >> choice;
	if(!stoi(choice))
	{
		cout << "Wrong. You did not capiche.\n\n";
		sleep(1);
		goto action;
	}
	switch(stoi(choice))
	{
		case 1:
			system("cls");
			goto action;
			break;
		case 2:
			cout << "\nNice chatting with you! Take care.";
			break;
		default:
			cout << "\nWrong. You did not capiche.";
			break;			
	}
    return 0;
}
