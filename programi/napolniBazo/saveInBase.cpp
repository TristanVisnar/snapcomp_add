
#include <iostream>
#include <mysql/mysql.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

char table[] = "aeiou";

int getRandomLength(int min, int max){
	//min 3 max 10 za besede
	return (rand() % (max - min)) + min;

}

int getTrueFalse() {
	return rand() % 2;
}

string ustvariBesedo() {
	string beseda ="" ;
	int ln = getRandomLength(3, 10);
	int vrsta = getTrueFalse();
	int crka = 0;
	for (int i = 0; i < ln; i++) {
		if (vrsta == 0) {
			if (i % 2 == 0) {
				crka = getRandomLength(0, 5);
				beseda +=table[crka];
			}
			else {
				crka = getRandomLength(97, 123);
				beseda += (char)crka;
			}
		}
		else {
			if (i % 2 == 1) {
				crka = getRandomLength(0, 5);
				beseda += table[crka];
			}
			else {
				crka = getRandomLength(97, 123);
				beseda += (char)crka;
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

string ustvariCas(){
	stringstream ss;
	int tmp = getRandomLength(0,25);
	if (tmp < 10)
		ss << "0" << ":";
	ss << tmp;
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

string ustvariDatum() {
	stringstream ss;
	ss << getRandomLength(1970, 2018) << "-";
	int tmp = getRandomLength(1, 13);
	if (tmp < 10)
		ss << "0";
	ss << tmp <<"-";
	tmp = getRandomLength(0, 29);
	if (tmp < 10)
		ss << "0";
	ss << tmp;
	return ss.str();
}

string ustvariTimestamp() {
	return ustvariDatum() + " " + ustvariCas();
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



int main(int argc, char ** argv ) {
	if (argc != 1) {
		return -1;
	}
	int stSlik = atoi(argv[1]);
	int stUporabnikov = stSlik / 4;
	srand(time(NULL));


	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	char *server = "localhost";
	char *user = "user";
	//set the password for mysql server here
	char *password = "joomladb"; /* set me first */
	char *database = "snapcomp";

	conn = mysql_init(NULL);

	/* Connect to database */
	if (!mysql_real_connect(conn, server,
		user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}




	sql::PreparedStatement  *prep_stmt;

	prep_stmt = conn->prepareStatement("INSERT into UPORABNIK (EMAIL,ACCNAME,USERNAME,PASS,DATEOFBIRTH,FIRSTNAME,SURNAME,LANG,GENDER,NUMOFPOSTS,NUMOFWINS) Values (?,?,?,?,?,?,?,?,?,?,?);");

	for (int i = 0; i < stUporabnikov; i++) {
		prep_stmt->setString(1, ustvariMail());
		string user = ustvariUsername();
		prep_stmt->setString(2, user);
		prep_stmt->setString(3, user);
		prep_stmt->setString(4, ustvariBesedo());

		prep_stmt->setString(5, ustvariDatum());

		prep_stmt->setString(6, ustvariBesedo());
		prep_stmt->setString(7, ustvariBesedo());

		prep_stmt->setString(8, izberiLang());
		prep_stmt->setInt(9, izberiGender());

		prep_stmt->setInt(10, getRandomLength(0, 1000));
		prep_stmt->setInt(11, getRandomLength(0, 1000));

		prep_stmt->execute();
	}
	
	delete prep_stmt;

	sql::PreparedStatement  *prep_stmt;

	prep_stmt = conn->prepareStatement("INSERT INTO PICTURE(DESCRIPTION,CONTENT,DATEOFUPLOAD,ID_USER,ID_SUGGESTION,LIKES,DISLIKES,NSFW) VALUES (?,?,?,?, ?,?,?,?)");

	ofstream file;
	string ime;
	for (int i = 0; i < stSlik; i++) {
		prep_stmt->setString(1, ustvariBesedilo());

		//blob od slike
		stringstream ss;
		ss<< "png/image"<< i <<".png";
		ime = ss.str();
		file.open(ime, ios::binary | ios::in );
		//stringstream ss;
		//ss << file.rdbuf();
		prep_stmt->setString(2, file.rdbuf());


		prep_stmt->setString(3, ustvariTimestamp());

		prep_stmt->setint(4, getRandomLength(0, stUporabnikov));
		prep_stmt->setint(5, getRandomLength(0,100));		///////   VSTAVI KOLIKO SUGGESTIONOV BO
		prep_stmt->setint(6, getRandomLength(0, 1000));
		prep_stmt->setint(7, getRandomLength(0, 1000));
		prep_stmt->setint(8, getTrueFalse());
		prep_stmt->execute();

	}
	
	delete prep_stmt;
	

	/*
	res = mysql_use_result(conn);

	// output table name 
	printf("MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s \n", row[0]);
	*/

	mysql_close(conn);
	delete conn;
}