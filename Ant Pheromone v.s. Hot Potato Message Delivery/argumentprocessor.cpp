#include "constants.h"
#include <vector>
#include <string>
#include <iostream>
#include "mailboxhelpers.h"

using namespace std;

bool processArguments(int argc, char *argv[])
{
    int count_success = 0;
    vector<bool> all_good;
    all_good.resize(3, false);
    bool success = false;
    for (int i = 1; i < argc; i++)
    {
        string curr_arg = argv[i];
        if (curr_arg == "-d")
        {
            if (i + 1 < argc)
            {
                string d_arg = argv[i + 1];
                try
                {
                    FIXED_INTERVAL = stoi(d_arg);
                    if (FIXED_INTERVAL < 100)
                    {
                        cout << "Interval may not be less than 100." << endl;
                    }
                    else if (FIXED_INTERVAL >= 100)
                    {
                        all_good[0] = true;
                        i++;
                    }
                }
                catch (const exception &e)
                {

                    cout << "ERROR: -d: ";
                    cerr << e.what() << endl;
                }
            }
        }
        else if (curr_arg == "-r")
        {
            if (i + 1 < argc)
            {
                string r_arg = argv[i + 1];
                if (r_arg == "hot" || r_arg == "ant")
                {
                    all_good[1] = true;
                    HOT_OR_ANT = r_arg;
                    i++;
                }
                else if (r_arg != "hot" || r_arg != "ant")
                {
                    cout << "-r ARGUMENT MUST BE HOT OR ANT" << endl;
                }
            }
        }
        else
        {
            try
            {
                FILE_NUMBER = stoi(curr_arg);
                if (10 <= FILE_NUMBER <= 50)
                {
                    all_good[2] = true;
                }
            }
            catch (exception &e)
            {
                cout << "FILE NUMBER MUST BE AN INTEGER [10,50]" << endl;
            }
        }
    }
    for (int i = 0; i < all_good.size(); i++)
    {
        if (all_good[i] == true)
        {
            count_success++;
        }
    }
    if (count_success == 3)
    {
        success = true;
        cout << topSeparatorGen("ARGUMENT PARSING SUCCESSFUL!", '*');
        cout << "-d -> " << FIXED_INTERVAL << endl;
        cout << "-r -> " << HOT_OR_ANT << endl;
        cout << "FILE NUMBER: " << FILE_NUMBER << endl;
        cout << "File number results in: ./graph/A" << FILE_NUMBER << ".dat" << endl;
        cout << bottomSeparatorGen('*');
    }
    return success;
}