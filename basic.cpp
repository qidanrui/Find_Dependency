#include <iostream>
using namespace std;
#include <fstream>
#include "basic.h"

data_node m_data[100000];

int readFromFile(string filename)
{
    fstream fin;
    string buffer;
    int number;
    int length;
    try{
        fin.open(filename, ios::in);
    }
    catch (exception& e){
        cout << e.what() << '\n';
        return -1;
    }
    number = 0;
    while (true)
    {
        getline(fin, buffer);
        length = buffer.length();
        if(length <= 2){
            break;
        }
        m_data[number] = spiltline(buffer, length);
        number++;
        if(!fin.good()){
            break;
        }
    }
    fin.close();
    return number;
}

data_node spiltline(string buffer, int length)
{
	data_node Node;
	string temp;
	int number;
	int i, start, n_item;
	start = 0;
	n_item = 1;
	for (i = 0; i<length; i++)
	{
		if (buffer[i] == ',')
		{
			temp = buffer.substr(start, (i - start));
			switch (n_item)
			{
			case 1:
				number = atoi(temp.c_str());
				Node.item1 = number;
				break;
			case 2:
				number = atoi(temp.c_str());
				Node.item2 = number;
				break;
			case 3:
				number = atoi(temp.c_str());
				Node.item3 = number;
				break;
			case 4:
				Node.item4 = temp;
				break;
			case 5:
				Node.item5 = temp[0];
				break;
			case 6:
				Node.item6 = temp;
				break;
			case 7:
				number = atoi(temp.c_str());
				Node.item7 = number;
				break;
			case 8:
				Node.item8 = temp;
				break;
			case 9:
				Node.item9 = temp;
				break;
			case 10:
				Node.item10 = temp;
				break;
			case 11:
				Node.item11 = temp;
				break;
			default:
				break;
			}
			start = i + 1;
			n_item++;
		}
	}
	temp = buffer.substr(start, (i - start));
	number = atoi(temp.c_str());
	Node.item12 = number;


	return Node;
}
