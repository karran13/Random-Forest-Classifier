/*
 * datautil.h
 *
 *  Created on: Sep 25, 2017
 *      Author: Binki
 */

#ifndef DATAUTIL_H_
#define DATAUTIL_H_

#include <string>
#include <vector>

#define AGE 0
#define WORKCLASS 1
#define FNLWGT 2
#define EDUCATION 3
#define EDUCATION_NUM 4
#define MARITAL_STATUS 5
#define OCCUPATION 6
#define RELATIONSHIP 7
#define RACE 8
#define SEX 9
#define CAPITAL_GAIN 10
#define CAPITAL_LOSS 11
#define HOURS_PER_WEEK 12
#define NATIVE_COUNTRY 13
#define GREATER_THAN_50 14

enum class Workclass : short {
	Private = 1, Self_emp_not_inc, Self_emp_inc, Federal_gov, Local_gov, State_gov, Without_pay, Never_worked
};

enum class Education : short {
	Bachelors = 1, Some_college, Eleventh, HS_grad, Prof_school, Assoc_acdm, Assoc_voc, Nineth, Seventh_eighth, Twelveth, Masters, First_fourth, Tenth, Doctorate, Fifth_sixth, Preschool
};

enum class Marital_status : short {
	Married_civ_spouse = 1, Divorced, Never_married, Separated, Widowed, Married_spouse_absent, Married_AF_spouse
};

enum class Occupation : short {
	Tech_support = 1, Craft_repair, Other_service, Sales, Exec_managerial, Prof_specialty, Handlers_cleaners, Machine_op_inspct, Adm_clerical, Farming_fishing, Transport_moving, Priv_house_serv, Protective_serv, Armed_Forces
};

enum class Relationship : short {
	Wife = 1, Own_child, Husband, Not_in_family, Other_relative, Unmarried
};

enum class Race : short{
 	White = 1, Asian_Pac_Islander, Amer_Indian_Eskimo, Other, Black
};

enum class Sex : short {
	Male = 1, Female
};

enum class Native_country : short {
	United_States = 1, Cambodia, England, Puerto_Rico, Canada, Germany, Outlying_US, India, Japan, Greece, South, China, Cuba, Iran, Honduras, Philippines, Italy, Poland, Jamaica, Vietnam, Mexico, Portugal, Ireland, France, Dominican_Republic, Laos, Ecuador, Taiwan, Haiti, Columbia, Hungary, Guatemala, Nicaragua, Scotland, Thailand, Yugoslavia, El_Salvador, Trinadad_and_Tobago, Peru, Hong, Holand_Netherlands
};

enum class Dage {
	less41 = 1, less61, less81, less91
};

enum class Dfnlwgt {
	less84228 = 1, less189778, less295828, less1484705
};

enum class Den {
	less8 = 1, less10, less12, less16
};

enum class Dcg {
	less1078 = 1, less8462, less99999
};

enum class Dcl {
	less87 = 1, less490, less4365
};

enum class Dhpw {
	less28 = 1, less40, less53, less99
};

struct Data{
	int age;
	enum Dage dage;

	enum Workclass workclass;

	int fnlwgt;
	enum Dfnlwgt dfnlwgt;

	enum Education education;

	int education_num;
	enum Den den;

	enum Marital_status marital_status;
	enum Occupation occupation;
	enum Relationship relationship;
	enum Race race;
	enum Sex sex;

	int capital_gain;
	enum Dcg dcg;

	int capital_loss;
	enum Dcl dcl;

	int hours_per_week;
	enum Dhpw dhpw;

	enum Native_country native_country;
	bool greater_than_50;
};

std::vector<struct Data> &get_dataset(std::string filename);

int get_attr_value(const int attr_id, const struct Data &d);




#endif /* DATAUTIL_H_ */
