// NIKU_CSV_CORRECTION.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include <stdio.h>
#include <vector>
#include <iterator>
#include <regex>
#include "rapidcsv.h"


using namespace std;

string formatDate(tm*, const char*);
string getFileName(int, int, int);
int CSVCorrections(tm,tm*);
tm makeDate(int, int, int);
int CreateDataCollection(tm, tm*);
int insertDataToCollection(vector<vector<string>>*, string);
string path = "C:\\Users\\edward\\Downloads\\";
string filenameExtension = ".csv";


const char* mmddyyyy = "%m-%d-%Y";          //// Format: "MM-DD-YYYY" e.g.12-31-2021 "%m-%d-%Y"

int main()
{
    // Intro
    cout << "Hello World!\n";

    // ask for range

    //hardcode applies
    tm startDate = makeDate(2021, 9, 15);
    std::time_t now = std::time(NULL);
    tm* endDate = localtime(&now);

    // correction
    CSVCorrections(startDate,endDate); //doen

    //CreateDataCollection(startDate,endDate);


    //system("pause");


}

/*formatDate return a string of a ptm after formatted*/
string formatDate(tm* ptm, const char* format){
    char buffer[32];
    strftime(buffer, 32, format, ptm);
    return buffer;
}

string getFileName(int year, int month, int day) {
    string filename = "item-sales-summary-";
    string date;
    if (month < 10) {
        if (day < 10) {
            date = to_string(year) + "-0" + to_string(month) + "-0" + to_string(day);
        }
        date = to_string(year) + "-0" + to_string(month) + "-" + to_string(day);
    }
    else {date = to_string(year) + "-" + to_string(month) + "-" + to_string(day); }
    filename.append(date).append("-").append(date);
    return filename;
}

int CSVCorrection(string filename) {
    {
        std::ifstream       file(path + filename + filenameExtension);
        std::vector< std::vector<std::string> >   matrix;
        std::vector<std::string>   row;
        std::string                line;
        std::string                cell="";
        vector<string> target_array = {
            "1-3 Tongue Root",
            "2-1 Angus Skirt",
            "2-4 Angus Finger Meat",
            "2-5 Angus Beef Cube",
            "1-7 B.L. Wagyu Chuck Rib",
            "1-9 Gold Label Wagyu Zabuton",
            "Calpico",
            "2-10 Chicken Cartilage",
            "2-11 Duck Breast",
            "2-9 Chicken Thigh",
            "3-1 A5 Wagyu Rib-Eye Slice",
            "1-23 Salmon Ochazuke",
            "1-25 Unagi Don",
            "Hakushika Tanuki Junmai",
            //"Sho Chiku Bai",                        //bug: cannot identify sake/nigori
            "3-6 Shrimp",
            "0-4 Miso Soup",
            "4-4 King Oyster Mushroom",
            "4-7 Zucchini",
            "4-9 Enoki Mushroom",
            "Kurosawa Junmai"
        };

        int correction = 0;

        if (!file.is_open()) { return 1; }

        while (file)
        {
            std::getline(file, line);
            std::stringstream lineStream(line);
            row.clear();

            while (std::getline(lineStream, cell, ','))
                    row.push_back(cell);

            if (!row.empty())
                
                matrix.push_back(row);
        }

        // manipulate data
        for (int i = 0; i<int(matrix.size()); i++)
        {
            for (int j = 0; j<int(matrix[i].size()); j++) {
                if (matrix[i][j].size() != 0) {

                    for (int k = 0; k < target_array.size();k++) {
                        string target = target_array[k];                            //search items in matrix needs to be fixed

                        //correction starts here
                        size_t found = matrix[i][j].find(target);
                        if (matrix[i][j][0] != '*') {                               //safety feature, prevent multiple edit
                            while (found != std::string::npos) {
                                matrix[i][j].insert(matrix[i][j].size(), ",");
                                found = matrix[i][j].find(target, found + 1);
                                matrix[i][j].insert(0, "*");                        // mark items in matrix that has been edited
                                correction++;
                            }
                        }
                    }
                }
            }
        }

        // write data to destination
        string newpath = path + filename + "_2" + filenameExtension;
        std::ofstream out(newpath);

        for (auto& row : matrix) {
            for (auto col : row)
                out << col << ',';
            out << '\n';
        }
    }
    return 0;
}

tm makeDate(int year, int month, int day)
{
    tm ttm = { 0 };
    ttm.tm_mday = day;
    ttm.tm_mon = month - 1;
    ttm.tm_year = year - 1900;
    return ttm;
}

int CSVCorrections(tm startDate, tm* endDate) {
    time_t end = mktime(endDate);

    // writing report
    string msg = "\nMissing following csv files between \n";


    for (tm date = startDate;end >= mktime(&date);++date.tm_mday)
    {
        // append file
        
        string filename = getFileName(
            stoi(formatDate(&date, "%Y")),
            stoi(formatDate(&date, "%m")), 
            stoi(formatDate(&date, "%d")));
        
        int edited = CSVCorrection(filename);

        if (edited) msg.append(filename).append("\n");


        std::cout << filename << "\n";
    }
    cout << msg << endl;

    return 0;
}

int CreateDataCollection(tm startDate, tm* endDate) {

    vector<vector<string>> collection;
    time_t end = mktime(endDate);
    string msg;

    // target name array
    vector<string> target_array = {
        "1-4",
        "1-5",
        "1-6",
        "1-7",
        "1-8",
        "1-9",
        "2-6",
        "1-3",
        "1-2",
        "1-1",
        "2-1",
        "2-2",
        "2-4",
        "2-5",
        "2-12",
        "1-10",
        "3-1",
        "3-2",
    };

    // target row - hard coded at 12, should implement an algorithm to find in raw data for future format change
    const int target_row = 12;

    // append first row as item headers
    vector<string> headers = { " ," };  //title
    
    // appending headers
    for (int i = 0; i < target_array.size(); i++) {
        headers.push_back("\n" + target_array[i] + ",");
    }

    collection.push_back(headers); // finish append vertical headers

    //test: [x][y], x = vertical, y = horizontal
    //collection[0][2].push_back('0'); //push horizontal
    

    // for output testing
    for (int i = 0; i < collection.size(); i++) {
        for (int j = 0; j < collection[i].size(); j++) {
            cout << collection[i][j];
        }
        cout << "\n";
    }
    // end of output testing

    

    


    // write data from each day
    //for (tm date = startDate;end >= mktime(&date);++date.tm_mday)
    //{
    //    // append file
    //    string filename = getFileName(stoi(formatDate(&date, "%Y")), stoi(formatDate(&date, "%m")), stoi(formatDate(&date, "%d")));

    //    int edited = insertDataToCollection(collection, filename);

    //    if (edited) msg.append(filename).append("\n");


    //    std::cout << filename << "\n";
    //}


    return 0;
}

int insertDataToCollection(vector<vector<string>>* collection, string filename, vector<string> target_array) {

    std::ifstream       file(path + filename + filenameExtension);
    std::vector< std::vector<std::string> >   matrix;
    std::vector<std::string>   row;
    std::string                line;
    std::string                cell = "";

    // open file
    if (!file.is_open()) { return 1; }

    while (file) {
        // create a new matrix to search data
        while (file)
        {
            std::getline(file, line);
            std::stringstream lineStream(line);
            row.clear();

            while (std::getline(lineStream, cell, ','))
                row.push_back(cell);

            if (!row.empty())

                matrix.push_back(row);
        }
    }
    

    // get value in column x and row y
    for (int i = 0; i < target_array.size(); i++) {
        for (int j = 0; j<int(matrix[i].size()); j++) {
            if (matrix[i][j].size() != 0) {

                for (int k = 0; k < target_array.size();k++) {
                    string target = target_array[k];                            //search items in matrix needs to be fixed

                    //correction starts here
                    size_t found = matrix[i][j].find(target);                           //safety feature, prevent multiple edit
                    while (found != std::string::npos) {
                        vector<string> value = matrix
                        found = matrix[i][j].find(target, found + 1);
                    
                    }
                }
            }
        }
    }


    // insert value









}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
