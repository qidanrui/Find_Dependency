#include "basic.h"
#include <time.h>

int main(int argc, char* argv[])
{
	if(argc > 2){
	    total_number = readFromFile(argv[1]);
	    if(total_number < 0){    	
	    	return 0;
	    }
	    else if(total_number == 0){
            cout << "Empty data file." << endl;
            return 0;
	    }
	}
	else if(argc > 1){
		cout << "Please enter your number of attributes." << endl;
		return 0;
	}
	else{
		cout << "Please enter your file name." << endl;
		return 0;
	}
	clock_t start, finish;
	double duration;
	start = clock();
	int attr_num = 0;
	for (int i = 0; i < strlen(argv[2]); i++) {
		attr_num += (argv[2][i] - '0') * pow(10, 1 - i);
	}
	tane(m_data, attr_num);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "The result is in ./result.txt" << endl;
	cout << "Running time: " << duration << "s" << endl;
	return 0;
}