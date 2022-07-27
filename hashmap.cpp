#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include<string>
#include<iostream>
using namespace std;

int FindRepeats(string key);
string Strength(float val);


template<class t1, class t2>
class Node
{

public:

    //pair
    t1 pair;

    // pair count
    t2 count;

    bool is_max;

    Node* next;

    Node(t1 p)
    {
        count = 1;
        next = nullptr;
        pair = p;
        is_max = false;
    }


};

template<class t1, class t2>
class unordered_map
{
    Node<t1, t2>* store;

    Node<t1, t2>* top_blocks;


public:

    /****  Constructor for hashmap class    ****/
    unordered_map()
    {
        store = nullptr;

        top_blocks = nullptr;


    }


    /****  Function responsible for adding new value and maintaining frequency in the hashmap    ****/
    void insert(t1 data)
    {

        if (store == nullptr)
        {
            store = new Node<t1, t2>(data);

        }
        else
        {
            Node<t1, t2>* curr = store;

            bool is_found = false;

            while (curr != nullptr)
            {
                if (curr->pair == data)
                {
                    is_found = true;
                    curr->count++;
                }
                curr = curr->next;
            }

            if (!is_found)
            {
                Node<t1, t2>* new_node = new Node<t1, t2>(data);

                new_node->next = store;

                store = new_node;

            }
        }
    }

    /****  returns total number of occurences of a pair   ****/
    int get_count(t1 data)
    {


        if (store == nullptr)
            return 0;

        Node<string, int>* curr = store;

        bool is_found = false;

        while (curr != nullptr)
        {
            if (curr->pair == data)
                return curr->count;
            curr = curr->next;

        }

        return 0;

    }

    /****  returns total number of occurences of a pair   ****/
    int get_max10_count()
    {


        if (top_blocks == nullptr)
        {

            for (int pass = 1; pass <= 10; pass++)
            {
                string max_block;

                int max_count = -100;

                Node<string, int>* curr = store;

                while (curr != nullptr)
                {
                    if (!curr->is_max && curr->count > max_count)
                    {

                        max_count = curr->count;

                        max_block = curr->pair;

                    }
                    curr = curr->next;

                }


                curr = store;



                while (curr != nullptr)
                {
                    if (curr->pair == max_block && curr->count == max_count)
                    {
                        curr->is_max = true;
                        break;
                    }
                    curr = curr->next;
                }
                Node<string, int>* temp = top_blocks;

                Node<string, int>* max_node = new Node<string, int>(max_block);

                max_node->count = max_count;

                if (temp == nullptr)
                    top_blocks = max_node;
                else
                {
                    while (temp->next != nullptr)
                        temp = temp->next;
                    temp->next = max_node;
                }

            }
        }

        cout << "\n::: Top 10 most occuring blocks ::: \n\n";

        Node<string, int>* curr = top_blocks;

        while (curr != nullptr)
        {

            cout << "max block: " << curr->pair << "  | max count: " << curr->count << endl;
            curr = curr->next;
        }

        cout << endl << endl;

        return 0;

    }


    void print_hashmap()
    {

        Node* curr = store;

        while (curr != nullptr)
        {
            cout << "(" << curr->pair << " , " << curr->count << ")  ";
            curr = curr->next;

        }
        cout << endl;


    }
};

// Reading the data from files and builds the map
void load_map(unordered_map<string, int>& map)
{
    ifstream file;

    string temp = "";

    string line = "";

    file.open("PwnedPasswordsTop100k.txt", ios::in);

    clock_t time_req = clock();

    cout << "Loading Map...";

    while (getline(file, line))
    {

        if (line.size() == 0 || line.size() == 1)
        {
            temp = "";
            temp.push_back('\0');
            temp.push_back('\0');

            map.insert(temp);

        }
        else
        {
            for (int i = 0; i < line.length() - 1; i++)
            {
                temp = line.substr(i, 2);

                map.insert(temp);
            }

        }

    }
    file.close();
    time_req = clock() - time_req;
    cout << "\n\nMap has been Loaded\nCreation of the unordered map took " << (float)time_req / CLOCKS_PER_SEC << " seconds" << endl;


}

void test_password_using_map(unordered_map<string, int>& map, string password, vector<float> wt)
{
    clock_t timeReqMap = clock();

    string temp;

    float sum = 0.0f;

    int repeats = FindRepeats(password);



    for (int i = 0; i < password.length() - 1; i++)
    {
        temp = password.substr(i, 2);

        int count = map.get_count(temp);

        sum = sum + (float)count * wt.at(i) / ((float)password.size() - repeats);


    }

    timeReqMap = clock() - timeReqMap;

    cout << "Strength of the password " << password << " was: " << sum << endl;

    cout << "This password is " << Strength(sum) << endl;

    cout << "The unordered map took " << (float)timeReqMap / CLOCKS_PER_SEC << " seconds to find the sum." << endl<<endl;
}



int main()
{

    vector<float> weights = { 1.0f,1.0f,1.0f,1.0f,1.0f,.8f,.8f,.75f,.65f,.65f,.5f,.5f,.4f,.3f,.2f,.15f };
     
    unordered_map<string, int> map;

    load_map(map);

    // start main
    cout << "\n\nPlease enter a value: " << endl;
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
            string input;
            cout << "Please enter a password to test that is between 4 and 16 characters." << endl;
            getline(cin, input);
            if (input.size() > 16 || input.size() < 4)
            {
                cout << "You have entered an invalid password." << endl;
            }
            else
            {
                cout << "\n\n --- Unordered Map Result ---- \n\n";

                test_password_using_map(map, input, weights);

             
            }

        }
        else if (choice == "2")
        {
            cout << "\n --- Unordered Map Result ---- \n";

            map.get_max10_count();

        }
        else if (choice == "3")
        {
            string input;
            
            cout << "Please enter a TWO character combination that you would like to check for matches in most common passwords." << endl;
            
            getline(cin, input);
            
            if (input.size() < 2)
            {
                cout << "That is not enough letters." << endl;
            }
            
            else if (input.size() > 2)
            {

                string index = input.substr(0, 2);

                cout << "\n --- Unordered Map Result ---- \n";

                cout << "\nBlock: " << index << " | count: " << map.get_count(input);

              
            }
            else
            {
                cout << "\n --- Unordered Map Result ---- \n";

                cout << "\nBlock: " << input << " | count: " << map.get_count(input);

            }
        }
        cout << "\n\nPlease enter a value: " << endl;
        cout << "1: To test the strength of a password." << endl;
        cout << "2: To print the top most common values." << endl;
        cout << "3. To search for a two letter combination and see how many times it appears in the top 100K passwords." << endl;
        cout << "Or anythings else to quit." << endl;

        getline(cin, choice);
    }
    cout << "Thank you for using the password strength checker!" << endl;


   
    return 0;
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
    else if (val < 40 && val >= 18)
    {
        return "very strong.";
    }
    else {
        return "exceptionally strong.";
    }
}
