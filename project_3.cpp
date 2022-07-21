
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
    cout << "Or anythings else to quit" << endl;
    TestPassword(mapp, "nfi9682k0sy582md", weights);
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
    cout << "The strength of this password was: " << sum << endl;


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




