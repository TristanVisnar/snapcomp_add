/*	Generiranje podatkov za:
-UPORABNIK
-ROOM
-PICTURE
-ENDOFSESSION
*/


#include <iostream>

//#include <mysql/mysql.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <exception>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;


char table[] = "aeiou";
char ostale[] = "bcdfghjklmnprstvz";
string countryList[] = { "Andorra", "Austria" , "Belgium" , "China" , "Egipt" , "Finland", "France", "Germany" , "Greece" , "Hungary", "India", "India","Iraq","Japan","Italy","Libya","Mexico","Niger","Poland","Peru","Russa","Slovenia","Spani","Sweden","United States of America","United Kingdom" };


int getRandomLength(int min, int max) {
	//min 3 max 10 za besede
	return (rand() % (max - min)) + min;

}

int getTrueFalse() {
	return rand() % 2;
}

string ustvariBesedo() {
	string beseda = "";
	int ln = getRandomLength(3, 10);
	int vrsta = getTrueFalse();
	int crka = 0;
	for (int i = 0; i < ln; i++) {
		if (vrsta == 0) {
			if (i % 2 == 0) {
				crka = getRandomLength(0, 5);
				beseda += table[crka];
			}
			else {
				crka = getRandomLength(0, 21);
				beseda += ostale[crka];
			}
		}
		else {
			if (i % 2 == 1) {
				crka = getRandomLength(0, 5);
				beseda += table[crka];
			}
			else {
				crka = getRandomLength(0, 21);
				beseda += ostale[crka];
			}
		}
	}
	return beseda;
}

string ustvariMail() {
	string beseda = "";
	beseda += ustvariBesedo() + "@generated.bot";
	return beseda;
}

string ustvariUsername() {
	return ustvariBesedo();
}

string ustvariBesedilo() {
	string besedilo = "";
	int stBesed = getRandomLength(5, 18);
	for (int i = 0; i < stBesed; i++) {
		besedilo += ustvariBesedo() + " ";
	}
	return besedilo;
}


string ustvariCas() {
	stringstream ss;
	int tmp = getRandomLength(0, 24);
	if (tmp < 10)
		ss << "0";
	ss << tmp << ":";
	tmp = getRandomLength(0, 60);
	if (tmp < 10)
		ss << "0";
	ss << tmp << ":";
	tmp = getRandomLength(0, 60);
	if (tmp < 10)
		ss << "0";
	ss << tmp;
	return ss.str();
}

int getSessionLength() {
	return 30 * (getRandomLength(2, 21)); // od 1 min do 10 min 
}

float getFloat(float min, float max) {
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

string ustvariDatum(int letoStart) {
	stringstream ss;
	ss << getRandomLength(letoStart, 2018) << "-";
	int tmp = getRandomLength(1, 13);
	if (tmp < 10)
		ss << "0";
	ss << tmp << "-";
	tmp = getRandomLength(1, 29);
	if (tmp < 10)
		ss << "0";
	ss << tmp;
	return ss.str();
}

string ustvariTimestamp(int leto) {
	return ustvariDatum(leto) + " " + ustvariCas();
}

string izberiLang() {
	if (getTrueFalse() == 0) {
		return "SLO";
	}
	else {
		return "ENG";
	}
}

int izberiGender() {
	return getTrueFalse() + 1;
}



int main(int argc, char ** argv) {
	if (argc != 2) {
		return -1;
	}
	int stSlik = atoi(argv[1]);
	int stUporabnikov = stSlik / 4 + 2;
	srand(time(NULL));

	sql::Driver *driver;
	try {
		driver = get_driver_instance();
	}
	catch (int &e) {
		cout << "ERROR IN DRIVER" << endl;
	}


	cout << "driver ... ok" << endl;

	sql::Connection *conn;

	string server = "localhost";
	string user = "user";
	//set the password for mysql server here
	string password = "joomladb"; /* set me first */
	string database = "snapcomp";



	/* Connect to database */
	try {
		conn = driver->connect(server, user, password);
	}
	catch (int &e) {
		cout << "ERROR IN MYSQL CONNECT" << endl;
	}

	cout << "mysql connection ... ok" << endl;
	try {
		conn->setSchema(database);
	}
	catch (int &e) {
		cout << "ERROR IN DATABASE CONNECTION" << endl;
	}
	cout << "database connection ... ok" << endl;

	/*		GENERIRANJE UPORABNIKOV		*/

	sql::PreparedStatement *prep_stmt;

	try {
		prep_stmt = conn->prepareStatement("INSERT into UPORABNIK (EMAIL,ACCNAME,USERNAME,PASS,DATEOFBIRTH,FIRSTNAME,SURNAME,LANG,GENDER,NUMOFPOSTS,NUMOFWINS,COUNTRY) Values (?,?,?,?,?,?,?,?,?,?,?,?);");
		cout << "Start adding users" << endl;
		for (int i = 0; i < stUporabnikov; i++) {
			prep_stmt->setString(1, ustvariMail());
			string user = ustvariUsername();
			prep_stmt->setString(2, user);
			prep_stmt->setString(3, user);
			prep_stmt->setString(4, ustvariBesedo());

			prep_stmt->setString(5, ustvariDatum(1980));

			prep_stmt->setString(6, ustvariBesedo());
			prep_stmt->setString(7, ustvariBesedo());

			prep_stmt->setString(8, izberiLang());
			prep_stmt->setInt(9, izberiGender());

			prep_stmt->setInt(10, getRandomLength(0, 1000));
			prep_stmt->setInt(11, getRandomLength(0, 1000));

			prep_stmt->setString(12, countryList[getRandomLength(0, 26)]);

			prep_stmt->execute();
		}
		cout << "End adding users ... ok" << endl;
	}
	catch (int &e) {
		cout << "ERROR IN PREPARE STATEMENT" << endl;
	}
	delete prep_stmt;




	/*		GENERIRANJE SOB		*/


	try {
		prep_stmt = conn->prepareStatement("INSERT into ROOM (NAME,DATEOFCREATION,PRIVATEROOM,NSFWROOM,ID_CREATOR) Values (?,NOW(),?,?,?);");
		cout << "Start adding rooms" << endl;
		for (int i = 0; i < 30; i++) {
			prep_stmt->setString(1, ustvariBesedo());

			prep_stmt->setInt(2, 0);
			prep_stmt->setInt(3, 0);

			prep_stmt->setInt(4, getRandomLength(1, stUporabnikov));

			prep_stmt->execute();
		}
		cout << "End adding rooms ... ok" << endl;
	}
	catch (int &e) {
		cout << "ERROR IN PREPARE STATEMENT" << endl;
	}
	delete prep_stmt;




	string *date = (string*)malloc(stSlik);
	for (int i = 0; i<stSlik; i++) {
		date[i] = ustvariTimestamp(2017);
	}

	/*			GENERIRANJE SLIK		*/


	prep_stmt = conn->prepareStatement("INSERT INTO PICTURE(DESCRIPTION,CONTENT,DATEOFUPLOAD,ID_USER,ID_SUGGESTION,LIKES,DISLIKES,NSFW,LATITUDE,LONGITUDE) VALUES (?,?,?,?,?,?,?,?,?,?)");
	cout << "Start adding picture " << endl;
	ifstream file;
	string ime;
	for (int i = 0; i < stSlik; i++) {
		prep_stmt->setString(1, ustvariBesedilo());


		//blob od slike
		stringstream ss;
		ss << "png/image" << i << ".png";
		ime = ss.str();
		file.open(ime.c_str(), ios::binary | ios::in);

		//stringstream sd;
		//sd << file.rdbuf();
		prep_stmt->setBlob(2, &file);



		prep_stmt->setString(3, date[i]);

		prep_stmt->setInt(4, getRandomLength(1, stUporabnikov));

		prep_stmt->setInt(5, getRandomLength(1, 301));		///////   VSTAVI KOLIKO SUGGESTIONOV BO

		prep_stmt->setInt(6, getRandomLength(0, 1000));

		prep_stmt->setInt(7, getRandomLength(0, 1000));

		prep_stmt->setInt(8, getTrueFalse());

		prep_stmt->setFloat(9, getFloat(-60,60));
		prep_stmt->setFloat(10, getFloat(-179, 179));


		prep_stmt->execute();

		file.close();

	}

	cout << "End adding picture" << endl;




	/*			GENERIRANJE KONCEV SEJE			*/


	try {
		prep_stmt = conn->prepareStatement("INSERT into ENDOFSESSION (SESSIONDURATION ,DATEOFSTART,ID_WINNER,ID_SELECTOR,ID_WINNING_PIC,ID_ROOM) Values (?,?,?,?,?,?);");
		cout << "Start adding end_session_state" << endl;
		for (int i = 0; i < stSlik; i++) {
			prep_stmt->setInt(1, getSessionLength());
			prep_stmt->setString(2, date[i]);

			prep_stmt->setInt(3, getRandomLength(1, stUporabnikov));
			prep_stmt->setInt(4, getRandomLength(1, stUporabnikov));

			prep_stmt->setInt(5, getRandomLength(1, i+1));
			prep_stmt->setInt(6, getRandomLength(1, 31));

			prep_stmt->execute();
		}
		cout << "End adding end_session_state ... ok" << endl;
	}
	catch (int &e) {
		cout << "ERROR IN PREPARE STATEMENT" << endl;
	}
	delete prep_stmt;


	cout << "Adding completed succesfuly" << endl;

	delete prep_stmt;

	delete conn;
}
