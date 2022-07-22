#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;
void PrintTopValue(vector < pair<int, string>>& vect, int number);
int FindRepeats(string key);
string Strength(float val);
void TestPasswordGraph(int** arr, string key, vector<float>& wt);
void Search4ValueGraph(int** arr, string val);
int main()
{

    vector<pair<int, string>> vec;
    vector<float> weights = { 1.0f,1.0f,1.0f,1.0f,1.0f,.8f,.8f,.75f,.65f,.65f,.5f,.5f,.4f,.3f,.2f,.15f };
    int** graph = new int* [255];
    for (int i = 0; i < 255; i++)
    {
        graph[i] = new int[255];
    }
    for (int i = 0; i < 255; i++) {
        for (int j = 0; j < 255; j++) {

            
            graph[i][j] = 0;
        }
    }
    // make the graph
    string line = "";
    ifstream file;
    string bin = "";
    file.open("PwnedPasswordsTop100k.txt");
    clock_t time_req = clock();
    
    while (getline(file, line))
    {
        
        if (line.size() == 0)
        {
            graph[0][0] = graph[0][0] + 1;
        }
        else if (line.size() == 1)
        {
            graph[(int)line[0]][0] = graph[(int)line[0]][0] + 1;
        }
        else
        {
            graph[(int)line[0]][(int)line[1]] = graph[(int)line[0]][(int)line[1]] + 1;
        }
        
    }
    file.close();
    time_req = clock() - time_req;
    cout << "Making the graph took " << (float)time_req / CLOCKS_PER_SEC << " seconds" << endl;
    // make the vector of largest values
    for (int i = 0; i < 255; i++)
    {
        for (int j = 0; j < 255; j++)
        {
            if (graph[i][j] > 0)
            {
                string bin1(1, (unsigned char)i);
                string bin2(1, (unsigned char)j);
                string bin3 = bin1 + bin2;
                vec.push_back(make_pair(graph[i][j], bin3));
            }
        }
    }
    sort(vec.begin(), vec.end());
    // start main
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
                TestPasswordGraph(graph, choice, weights);
            }

        }
        else if (choice == "2")
        {
            cout << "Please enter how many top letter combinations you would like to see." << endl;
            getline(cin, choice);
            int sender = 0;
            try {
                sender = stoi(choice);
                PrintTopValue(vec, sender);
            }
            catch (invalid_argument& e)
            {
                cout << "That is not a valid number." << endl;
            }
            

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
                Search4ValueGraph(graph, index);
            }
            else
            {
                Search4ValueGraph(graph, choice);
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


    for (int i = 0; i < 255; i++)
    {
        delete[] graph[i];
    }

    delete[] graph;
    return 0;
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



void TestPassword(map<string, int>& mappy, string key, vector<float>& wt)
{
    string bin = "";
    float sum = 0.0f;
    int repeats = FindRepeats(key);
    clock_t timeReqMap = clock();
    for (int i = 0; i < key.size() - 1; i++)
    {
        bin = key.substr(i, 2);
        auto finder = mappy.find(bin);
        if (finder != mappy.end())
        {
            sum = (sum + (float)finder->second * wt.at(i)) /((float)(key.size() - 1) - repeats);
        }

    }
    timeReqMap = clock() - timeReqMap;
    cout << "The strength of the password " << key <<" was " << sum << endl;
    cout << "This password is " << Strength(sum) << endl;
    cout << "The map took " << (float)timeReqMap / CLOCKS_PER_SEC << " seconds to find the sum." << endl;

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
        return "very weak.";
    }
    else if (val < 500 && val >= 150)
    {
        return "weak.";
    }
    else if (val < 150 && val >= 100)
    {
        return "neutral strength.";
    }
    else if (val < 100 && val >= 40)
    {
        return "strong.";
    }
    else if(val < 40 && val >= 18)
    {
        return "very strong.";
    }
    else {
        return "exceptionally strong.";
    }
}

void TestPasswordGraph(int** arr, string key, vector<float>& wt)
{
    string bin = "";
    float sum = 0.0f;
    int repeats = FindRepeats(key);
    clock_t timeReqMap = clock();
    for (int i = 0; i < key.size() - 1; i++)
    {
        bin = key.substr(i, 2);
        sum = sum + (float)arr[(int)bin[0]][(int)bin[1]] * wt.at(i) / ((float)key.size() - repeats);

    }
    timeReqMap = clock() - timeReqMap;
    cout << "The strength of the password " << key << " was " << sum << endl;
    cout << "This password is " << Strength(sum) << endl;
    cout << "The graph took " << (float)timeReqMap / CLOCKS_PER_SEC << " seconds to find the sum." << endl;
}

void Search4ValueGraph(int** arr, string val)
{
    clock_t searchTime = clock();

    
    
    if (arr[(int)val[0]][(int)val[1]] == 0)
    {
        cout << val << " not found in most common two letter combinations." << endl;
    }
    else
    {
        cout << "The letter combination " << val << " was found " << arr[(int)val[0]][(int)val[1]] << " times the 100k most common passwords" << endl;
    }
    searchTime = clock() - searchTime;
    cout << "The graph took " << (float)searchTime / CLOCKS_PER_SEC << " seconds to look up: " << val << endl;
}












