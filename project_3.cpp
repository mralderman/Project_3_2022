#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

void makeMap(map<string, int>& mappy);
void makeVector(vector<pair<int, string>>& vect, map<string, int>& mappy);
void PrintTopValue(vector < pair<int, string>>& vect, int number);
void Search4Value(map<string, int>& mappy, string val);
void TestPassword(map<string, int>& mappy, string key, vector<float>& wt);
int FindRepeats(string key);
string Strength(float val);

int main()
{
    map<string, int> mapp;
    vector<pair<int, string>> vec;
    vector<float> weights = { 1.0f,1.0f,1.0f,1.0f,1.0f,.8f,.8f,.75f,.65f,.65f,.5f,.5f,.4f,.3f,.2f,.15f };
    makeMap(mapp);
    makeVector(vec, mapp);
    //PrintTopValue(vec, 10);
    cout << "Please enter a value: " << endl;
    cout << "1: To test the strength of a password." << endl;
    cout << "2: To print the top most common values." << endl;
    cout << "3. To search for a two letter combination and see how many times it appears in the top 100K passwords." << endl;
    cout << "Or anythings else to quit." << endl;
    string choice = "";
    getline(cin, choice);

    while (choice == "1" || choice == "2" || choice == "3")
    {
        if (choice == "1")
        {
            cout << "Please enter a password to test that is between 4 and 16 characters." << endl;
            getline(cin, choice);
            if (choice.size() > 16 || choice.size() < 4)
            {
                cout << "You have entered an invalid password." << endl;
            }
            else
            {
                TestPassword(mapp, choice, weights);
            }

        }
        else if (choice == "2")
        {
            cout << "Please enter how many top letter combinations you would like to see." << endl;
            getline(cin, choice);
            int sender = 0;
            try {
                sender = stoi(choice);
            }
            catch (invalid_argument& e)
            {
                cout << "That is not a valid number." << endl;
            }
            PrintTopValue(vec, sender);

        }
        else if (choice == "3")
        {
            cout << "Please enter a TWO character combination that you would like to check for matches in most common passwords." << endl;
            getline(cin, choice);
            if (choice.size() < 2)
            {
                cout << "That is not enough letters." << endl;
            }
            else if (choice.size() > 2)
            {
                string index = choice.substr(0, 2);
                Search4Value(mapp, index);
            }
            else
            {
                Search4Value(mapp, choice);
            }
        }
        cout << "Please enter a value: " << endl;
        cout << "1: To test the strength of a password." << endl;
        cout << "2: To print the top most common values." << endl;
        cout << "3. To search for a two letter combination and see how many times it appears in the top 100K passwords." << endl;
        cout << "Or anythings else to quit." << endl;
        
        getline(cin, choice);
    }
    cout << "Thank you for using the password strength checker!" << endl;

    return 0;
}

void makeMap(map<string, int>& mappy)
{
    string line = "";
    ifstream file;
    string bin = "";
    file.open("PwnedPasswordsTop100k.txt");
    clock_t time_req = clock();
    while (getline(file, line))
    {
        // if size = 0
        if (line.size() == 0)
        {
            bin = " ";
            map<string, int>::iterator iter0 = mappy.find(bin);
            if (iter0 == mappy.end())
            {
                mappy[bin] = 1;
            }
            else
            {
                iter0->second = iter0->second + 1;
            }

        }
        else if (line.size() == 1)
        {
            bin = line;
            map<string, int>::iterator iter1 = mappy.find(bin);
            if (iter1 == mappy.end())
            {
                mappy[bin] = 1;
            }
            else
            {
                iter1->second = iter1->second + 1;
            }

        }
        else
        {
            for (int i = 0; i < line.size() - 1; i++)
            {
                bin = line.substr(i, 2);
                map<string, int>::iterator iter = mappy.find(bin);
                if (iter == mappy.end())
                {
                    mappy[bin] = 1;
                }
                else
                {
                    iter->second = iter->second + 1;
                }
            }

        }
    }
    time_req = clock() - time_req;
    cout <<"Making the map took " << (float)time_req / CLOCKS_PER_SEC << " seconds" << endl;
    file.close();
}

void makeVector(vector<pair<int, string>>& vect, map<string, int>& mappy)
{
    auto it = mappy.begin();
    for (it; it != mappy.end(); it++)
    {
        vect.push_back(make_pair(it->second, it->first));
    }
    sort(vect.begin(), vect.end());
}

void PrintTopValue(vector<pair<int, string>>& vect, int number)
{
    if (number >= vect.size() - 1)
    {
        number = vect.size() - 1;
    }
    cout << "Printing top " << number << " most common letter combinations" << endl;
    int i = vect.size() - 1;
    int j = 0;
    cout << "Letter combinations:" << " " << "Number of times they appear in most common passwords:" << endl;
    while (j < number)
    {
        
        cout << vect[i].second << setw(24) << vect[i].first << endl;
        j++;
        i = i - 1;
    }



}

void Search4Value(map<string, int>& mappy, string val)
{
    auto itter = mappy.find(val);
    if (itter == mappy.end())
    {
        cout << val <<" not found in most common two letter combinations." << endl;
    }
    else
    {
        cout << "The letter combination " << val << " was found " << itter->second << " times the 100k most common passwords" << endl;
    }

}

void TestPassword(map<string, int>& mappy, string key, vector<float>& wt)
{
    string bin = "";
    float sum = 0.0f;
    int repeats = FindRepeats(key);
    for (int i = 0; i < key.size() - 1; i++)
    {
        bin = key.substr(i, 2);
        auto finder = mappy.find(bin);
        if (finder != mappy.end())
        {
            sum = (sum + (float)finder->second * wt.at(i)) /((float)(key.size() - 1) - repeats);
        }

    }
    cout << "The strength of the password " << key <<" was " << sum << endl;
    cout << "This password is " << Strength(sum) << endl;

}

int FindRepeats(string key)
{
    int value = 0;
    string temp = "";
    for (int j = 0; j < key.size() - 2; j++)
    {
        temp = key.substr(j, 2);
        size_t found = key.find(temp, j + 1);
        if (found != string::npos)
        {
            value++;
        }

    }
    return value;
}

string Strength(float val)
{
    if (val >= 500)
    {
        return "very weak";
    }
    else if (val < 500 && val >= 150)
    {
        return "weak";
    }
    else if (val < 150 && val >= 100)
    {
        return "neutral strength";
    }
    else if (val < 100 && val >= 40)
    {
        return "strong";
    }
    else if(val < 40 && val >= 18)
    {
        return "very strong";
    }
    else {
        return "exceptionally strong";
    }
}








