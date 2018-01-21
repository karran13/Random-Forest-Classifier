/*
 * datautil.cpp
 *
 *  Created on: Sep 25, 2017
 *      Author: Binki
 */

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include "datautil.h"
#include <iostream>
using namespace std;

/**
 * Function to trim string from start
 * @param s: pointer to string
 * @return left trimmed string
 */

static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

/**
 * Function to trim string from end
 * @param s: pointer to string
 * @return right trimmed string
 */

static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

/**
 * Function to trim from both ends
 * @param s: pointer to string
 * @return trimmed string
 */
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

/**
 * Function to split string into vector of strings based on delimiter
 * @param s: Initial string
 * @param delim: Delimiting character
 * @param elems: Vector of split strings
 */
void split(const string &s, char delim, vector<string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

/**
 * Function to take line of data input from file buffer and insert into struct Data appropriately
 * @param inf: input file stream
 * @param d: Data structure made as per attributes
 * @return integer value indicating success or failure of data processing operation
 */

int get_data(ifstream &inf, struct Data &d) {
	string line;
	getline(inf, line);
//	cout << line << endl; //debug

	vector<string> data;
	split(line, ',', data);

	for (unsigned i = 0; i < data.size(); ++i) {
		data[i] = trim(data[i]);

//		cout << data[i] << " "; //debug;
	}
//	cout << endl;

//	struct Data d;
	if(data.size() == 15) {
		// set age
		d.age = stoi(data[0]);

		if(d.age < 41) {
			d.dage = Dage::less41;
		}
		else if(d.age < 61) {
			d.dage = Dage::less61;
		}
		else if(d.age < 81) {
			d.dage = Dage::less81;
		}
		else {
			d.dage = Dage::less91;
		}

		// set workclass
		if(data[1] == "Private")
			d.workclass = Workclass::Private;
		else if (data[1] == "Self-emp-not-inc")
			d.workclass = Workclass::Self_emp_not_inc;
		else if (data[1] == "Self-emp-inc")
			d.workclass = Workclass::Self_emp_inc;
		else if (data[1] == "Federal-gov")
			d.workclass = Workclass::Federal_gov;
		else if (data[1] == "Local-gov")
			d.workclass = Workclass::Local_gov;
		else if (data[1] == "State-gov")
			d.workclass = Workclass::State_gov;
		else if (data[1] == "Without-pay")
			d.workclass = Workclass::Without_pay;
		else if (data[1] == "Never-worked")
			d.workclass = Workclass::Never_worked;
		else
			d.workclass = (Workclass)0;

		// set fnlwgtl
		d.fnlwgt = stoi(data[2]);
		if(d.fnlwgt < 84228)
			d.dfnlwgt = Dfnlwgt::less84228;
		else if(d.fnlwgt < 189778)
			d.dfnlwgt = Dfnlwgt::less189778;
		else if(d.fnlwgt < 1484705)
			d.dfnlwgt = Dfnlwgt::less1484705;
		else
			d.dfnlwgt = Dfnlwgt::less295828;


		// set education
		if(data[3] == "Bachelors")
			d.education = Education::Bachelors;
		else if(data[3] == "Some-college")
			d.education = Education::Some_college;
		else if(data[3] == "11th")
			d.education = Education::Eleventh;
		else if(data[3] == "HS-grad")
			d.education = Education::HS_grad;
		else if(data[3] == "Prof-school")
			d.education = Education::Prof_school;
		else if(data[3] == "Assoc-acdm")
			d.education = Education::Assoc_acdm;
		else if(data[3] == "Assoc-voc")
			d.education = Education::Assoc_voc;
		else if(data[3] == "9th")
			d.education = Education::Nineth;
		else if(data[3] == "7th-8th")
			d.education = Education::Seventh_eighth;
		else if(data[3] == "12th")
			d.education = Education::Twelveth;
		else if(data[3] == "Masters")
			d.education = Education::Masters;
		else if(data[3] == "1st-4th")
			d.education = Education::First_fourth;
		else if(data[3] == "10th")
			d.education = Education::Tenth;
		else if(data[3] == "Doctorate")
			d.education = Education::Doctorate;
		else if(data[3] == "5th-6th")
			d.education = Education::Fifth_sixth;
		else if(data[3] == "Preschool")
			d.education = Education::Preschool;
		else
			d.education = (Education)0;

		// set education-num
		d.education_num = stoi(data[4]);
		if(d.education_num < 8)
			d.den = Den::less8;
		else if(d.education_num < 10)
			d.den = Den::less10;
		else if(d.education_num < 12)
			d.den = Den::less12;
		else
			d.den = Den::less16;


		//set marital status
		if(data[5] == "Married-civ-spouse")
				d.marital_status = Marital_status::Married_civ_spouse;
		else if(data[5] == "Divorced")
				d.marital_status = Marital_status::Divorced;
		else if(data[5] == "Never-married")
				d.marital_status = Marital_status::Never_married;
		else if(data[5] == "Separated")
				d.marital_status = Marital_status::Separated;
		else if(data[5] == "Widowed")
				d.marital_status = Marital_status::Widowed;
		else if(data[5] == "Married-spouse-absent")
				d.marital_status = Marital_status::Married_spouse_absent;
		else if(data[5] == "Married-AF-spouse")
				d.marital_status = Marital_status::Married_AF_spouse;
		else
			d.marital_status = (Marital_status)0;

		// set occupation
		if(data[6] == "Tech-support")
			d.occupation = Occupation::Tech_support;
		else if(data[6] == "Craft-repair")
			d.occupation = Occupation::Craft_repair;
		else if(data[6] == "Other-service")
			d.occupation = Occupation::Other_service;
		else if(data[6] == "Sales")
			d.occupation = Occupation::Sales;
		else if(data[6] == "Exec-managerial")
			d.occupation = Occupation::Exec_managerial;
		else if(data[6] == "Prof-specialty")
			d.occupation = Occupation::Prof_specialty;
		else if(data[6] == "Handlers-cleaners")
			d.occupation = Occupation::Handlers_cleaners;
		else if(data[6] == "Machine-op-inspct")
			d.occupation = Occupation::Machine_op_inspct;
		else if(data[6] == "Adm-clerical")
			d.occupation = Occupation::Adm_clerical;
		else if(data[6] == "Farming-fishing")
			d.occupation = Occupation::Farming_fishing;
		else if(data[6] == "Transport_moving")
			d.occupation = Occupation::Transport_moving;
		else if(data[6] == "Priv-house-serv")
			d.occupation = Occupation::Priv_house_serv;
		else if(data[6] == "Protective-serv")
			d.occupation = Occupation::Protective_serv;
		else if(data[6] == "Armed-Forces")
			d.occupation = Occupation::Armed_Forces;
		else
			d.occupation = (Occupation)0;

		//set relationship
		if(data[7] == "Wife")
			d.relationship = Relationship::Wife;
		else if(data[7] == "Own-child")
			d.relationship = Relationship::Own_child;
		else if(data[7] == "Husband")
			d.relationship = Relationship::Husband;
		else if(data[7] == "Not-in-family")
			d.relationship = Relationship::Not_in_family;
		else if(data[7] == "Other-relative")
			d.relationship = Relationship::Other_relative;
		else if(data[7] == "Unmarried")
			d.relationship = Relationship::Unmarried;
		else
			d.relationship = (Relationship)0;

		// set race
		if(data[8] == "White")
			d.race = Race::White;
		else if(data[8] == "Asian-Pac-Islander")
			d.race = Race::Asian_Pac_Islander;
		else if(data[8] == "Amer-Indian-Eskimo")
			d.race = Race::Amer_Indian_Eskimo;
		else if(data[8] == "Other")
			d.race = Race::Other;
		else if(data[8] == "Black")
			d.race = Race::Black;
		else
			d.race = (Race)0;

		// set sex
		if(data[9] == "Female")
			d.sex = Sex::Female;
		else if(data[9] == "Male")
			d.sex = Sex::Male;
		else
			d.sex = (Sex)0;

		// set capital gain
		d.capital_gain = stoi(data[10]);
		if(d.capital_gain < 1078)
			d.dcg = Dcg::less1078;
		else if(d.capital_gain < 8462)
			d.dcg = Dcg::less8462;
		else
			d.dcg = Dcg::less99999;


		//set capital loss
		d.capital_loss = stoi(data[11]);
		if(d.capital_loss < 87)
			d.dcl = Dcl::less87;
		else if(d.capital_loss < 490)
			d.dcl = Dcl::less490;
		else
			d.dcl = Dcl::less4365;


		// set hours per week
		d.hours_per_week = stoi(data[12]);
		if(d.hours_per_week < 28)
			d.dhpw = Dhpw::less28;
		else if(d.hours_per_week < 40)
			d.dhpw = Dhpw::less40;
		else if(d.hours_per_week < 53)
			d.dhpw = Dhpw::less53;
		else
			d.dhpw = Dhpw::less99;

		// set native country
		if(data[13] == "United-States")
			d.native_country = Native_country::United_States;
		else if(data[13] == "Cambodia")
			d.native_country = Native_country::Cambodia;
		else if(data[13] == "England")
			d.native_country = Native_country::England;
		else if(data[13] == "Puerto-Rico")
			d.native_country = Native_country::Puerto_Rico;
		else if(data[13] == "Canada")
			d.native_country = Native_country::Canada;
		else if(data[13] == "Germany")
			d.native_country = Native_country::Germany;
		else if(data[13] == "Outlying-US(Guam-USVI-etc)")
			d.native_country = Native_country::Outlying_US;
		else if(data[13] == "India")
			d.native_country = Native_country::India;
		else if(data[13] == "Japan")
			d.native_country = Native_country::Japan;
		else if(data[13] == "Greece")
			d.native_country = Native_country::Greece;
		else if(data[13] == "South")
			d.native_country = Native_country::South;
		else if(data[13] == "China")
			d.native_country = Native_country::China;
		else if(data[13] == "Cuba")
			d.native_country = Native_country::Iran;
		else if(data[13] == "Honduras")
			d.native_country = Native_country::Philippines;
		else if(data[13] == "Italy")
			d.native_country = Native_country::Italy;
		else if(data[13] == "Poland")
			d.native_country = Native_country::Poland;
		else if(data[13] == "Jamaica")
			d.native_country = Native_country::Jamaica;
		else if(data[13] == "Vietnam")
			d.native_country = Native_country::Vietnam;
		else if(data[13] == "Mexico")
			d.native_country = Native_country::Mexico;
		else if(data[13] == "Ireland")
			d.native_country = Native_country::Ireland;
		else if(data[13] == "France")
			d.native_country = Native_country::France;
		else if(data[13] == "Dominican-Republic")
			d.native_country = Native_country::Dominican_Republic;
		else if(data[13] == "Laos")
			d.native_country = Native_country::Laos;
		else if(data[13] == "Ecuador")
			d.native_country = Native_country::Ecuador;
		else if(data[13] == "Taiwan")
			d.native_country = Native_country::Taiwan;
		else if(data[13] == "Haiti")
			d.native_country = Native_country::Haiti;
		else if(data[13] == "Columbia")
			d.native_country = Native_country::Columbia;
		else if(data[13] == "Hungary")
			d.native_country = Native_country::Hungary;
		else if(data[13] == "Guatemala")
			d.native_country = Native_country::Guatemala;
		else if(data[13] == "Nicaragua")
			d.native_country = Native_country::Nicaragua;
		else if(data[13] == "Scotland")
			d.native_country = Native_country::Scotland;
		else if(data[13] == "Thailand")
			d.native_country = Native_country::Thailand;
		else if(data[13] == "Yugoslavia")
			d.native_country = Native_country::Yugoslavia;
		else if(data[13] == "El-Salvador")
			d.native_country = Native_country::El_Salvador;
		else if(data[13] == "Trinadad&Tobago")
			d.native_country = Native_country::Trinadad_and_Tobago;
		else if(data[13] == "Peru")
			d.native_country = Native_country::Peru;
		else if(data[13] == "Hong")
			d.native_country = Native_country::Hong;
		else if(data[13] == "Holand-Netherlands")
			d.native_country = Native_country::Holand_Netherlands;
		else
			d.native_country = (Native_country)0;


		if(data[14] == ">50K")
			d.greater_than_50 = true;
		else
			d.greater_than_50 = false;

		return 0;
	}

	return -1;
}

/**
 * Function to return Dataset as a vector of struct Data from file input
 * @param filename: Filepath
 * @return vector of struct data containing dataset as in the input file
 */

vector<struct Data> &get_dataset(string filename) {
	ifstream infile;
	infile.open(filename.c_str(), ios::in);
	vector<struct Data> *v = new vector<struct Data>();
	while(!infile.eof()) {
		struct Data d;
		if (get_data(infile, d) != -1)
			(*v).push_back(d);
	}

	return *v;
}

/**
 * Function to return attribute value given index of attribute and datapoin
 * @param attr_id: index of attribute
 * @param d: Datapoint
 * @return value of required attribute in datapoint
 */

int get_attr_value(const int attr_id, const struct Data &d) {
	switch(attr_id) {
	case AGE:				return (int)d.dage;
	case WORKCLASS:			return (int)d.workclass;
	case FNLWGT:			return (int)d.dfnlwgt;
	case EDUCATION:			return (int)d.education;
	case EDUCATION_NUM:		return (int)d.den;
	case MARITAL_STATUS:	return (int)d.marital_status;
	case OCCUPATION:		return (int)d.occupation;
	case RELATIONSHIP:		return (int)d.relationship;
	case RACE:				return (int)d.race;
	case SEX:				return (int)d.sex;
	case CAPITAL_GAIN:		return (int)d.dcg;
	case CAPITAL_LOSS:		return (int)d.dcl;
	case HOURS_PER_WEEK:	return (int)d.dhpw;
	case NATIVE_COUNTRY:	return (int)d.native_country;
	case GREATER_THAN_50:	return (int)d.greater_than_50;
	default:				return -1;	/// id is not in range
	}
}



