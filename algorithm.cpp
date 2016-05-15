#include "basic.h"

int* number_list;
int total_number = 0;
vector<vector<int>> outputInt;
int outputLength = 0;

void generateNextList(int* next_list, graph_node& node, int attr_num) {
	int index = 0;
	int temp;
	for (int i = 0; i < attr_num; i++) {
		temp = node.get_this_node();
		temp &= number_list[i];
		if (!temp) {
			next_list[index] = node.get_this_node() | number_list[i];
			index += 1;
		}
	}
}
//generate next level nodes
void generateNextLevel(unordered_map<int, int>& next_layer, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int l, int attr_num) {
	int* next_list = new int[attr_num - l];
	last_layer.clear();
	for (auto val : current_layer) {
		if (!val.get_delete()) {
			last_layer.push_back(val);
			generateNextList(next_list, val, attr_num);
			for (int i = 0; i < attr_num - l; i++) {
				if (next_layer.find(next_list[i]) == next_layer.end()) {
					next_layer[next_list[i]] = 1;
				}
				else {
					next_layer[next_list[i]] += 1;
				}
			}
		}
	}
	current_layer.clear();
	for (auto val : next_layer) {
		if (val.second == (l + 1)) {
			graph_node temp_node = graph_node();
			temp_node.set_this_node(val.first);
			current_layer.push_back(temp_node);

		}
	}
	next_layer.clear();
	delete[] next_list;
}

void tane(data_node data[], int attr_num) {
	vector<graph_node> last_layer = vector<graph_node>();
	vector<graph_node> current_layer = vector<graph_node>();
	unordered_map<int, int> node_to_rhs;
	unordered_map<int, graph_node> node_index;
	unordered_map<int, int> next_layer;
	unordered_map<int, vector<vector<int>>> partition;
	unordered_map<int, vector<int>> mark;
	number_list = new int[attr_num];
	for (int i = 0; i < attr_num; i++) {
		number_list[i] = (1 << i);
	}
	//first layer partition
	getFirstLayerPartition(data, partition, mark, attr_num);
	partition[0] = vector<vector<int>>(1);
	partition[0][0] = vector<int>(total_number);
	mark[0] = vector<int>(total_number);
	int l = 1;
	last_layer.resize(1);
	node_index[0] = last_layer[0];
	node_to_rhs[0] = 0xfff;
	current_layer.resize(attr_num);
	for (int i = 0; i < attr_num; i++) {
		current_layer[i].set_bit(i);
	}
	while ((!current_layer.empty()) && l != 12) {
		computeDependency(current_layer, node_to_rhs, l, partition, mark);
		generateNextLevel(next_layer, current_layer, last_layer, l, attr_num);
		l++;
	}
	outputAllResult();
}
//get current layer rhs based on last layer
void get_rhs(unordered_map<int, int>& node_to_rhs, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int layer, int attr_num) {
	for (auto val : current_layer) {
		int sum = 0xfff;
		//get rhs
		for (int i = 0; i < attr_num; i++) {
			if (val.get_this_node() & number_list[i]) {
				sum &= (node_to_rhs[val.get_this_node() ^ number_list[i]]);
			}
		}
		node_to_rhs[val.get_this_node()] = sum;
	}
}

void getPartition(unordered_map<int, vector<vector<int>>>& partition, unordered_map<int, vector<int>>& mark, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int layer, int attr_num) {
	if (layer == 1) return;
	for (auto val : current_layer) {
		vector<vector<int>> tempPartition;
		vector<int> tempMark = vector<int>(total_number);
		for (int i = 0; i < attr_num; i++) {
			int temp = val.get_this_node() & number_list[i];
			int temp1;
			if (temp) {
				temp1 = val.get_this_node() ^ number_list[i];
				calculatePartition(partition[temp], partition[temp1], mark[temp], mark[temp1], tempPartition, tempMark);
				break;
			}
		}
		partition[val.get_this_node()] = tempPartition;
		mark[val.get_this_node()] = tempMark;
	}
}

void computeDependency(vector<graph_node>& current_layer, unordered_map<int, int>& node_to_rhs, int layer, unordered_map<int, vector<vector<int>>>& partition, unordered_map<int, vector<int>>& mark) {
	get_rhs(node_to_rhs, current_layer, current_layer, layer, 12);
	getPartition(partition, mark, current_layer, current_layer, layer, 12);
	int len = current_layer.size();
	for (int i = 0; i < len; i++) {
		auto val = current_layer[i];
		int rhs_x = node_to_rhs[val.get_this_node()];
		for (int i = 0; i < 12; i++) {
			if ((val.get_this_node() & number_list[i]) != 0) {
				if (rhs_x & number_list[i]) {
					int pre_layer_node = val.get_this_node() ^ number_list[i];
					if ((judgeValid(partition[pre_layer_node], partition[pre_layer_node | number_list[i]]))) {
						output(pre_layer_node, i);
						//remove A from C
						rhs_x ^= number_list[i];
						//remove all B in R\X from C
						rhs_x &= val.get_this_node();
						if (!rhs_x) break;
					}
				}
			}
			node_to_rhs[val.get_this_node()] = rhs_x;
		}
		if (!rhs_x) {
			current_layer[i].set_delete();
		}
	}
}

bool judgeValid(vector<vector<int>> node1, vector<vector<int>> node2) {
	if (node1.size() == node2.size())
		return true;
	return false;
}

void getFirstLayerPartition(data_node data[], unordered_map<int, vector<vector<int>>>& partition, unordered_map<int, vector<int>>& mark, int attr_num) {
	int data_length = total_number;
	for (int i = 0; i < attr_num; i++) {
		partition[number_list[i]] = vector<vector<int>>();
		mark[number_list[i]] = vector<int>(data_length);
	}
	for (int i = 0; i < attr_num; i++) {
		if (i == 0) {
			vector<int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item1);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item1);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 1) {
			vector<int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item2);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item2);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 2) {
			vector<int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item3);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item3);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 3) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item4);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item4);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 4) {
			vector<char> temp;
			for (int j = 0; j < data_length; j++) {
				vector<char>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item5);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item5);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 5) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item6);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item6);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 6) {
			vector<short int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<short int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item7);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item7);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 7) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item8);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item8);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 8) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item9);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item9);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 9) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item10);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item10);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 10) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item11);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item11);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
		else if (i == 11) {
			vector<int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item12);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item12);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					mark[number_list[i]][j] = partition[number_list[i]].size() - 1;
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
					mark[number_list[i]][j] = tempIterator - temp.begin();
				}
			}
		}
	}
}

//get current layer partition based on last layer
void calculatePartition(vector<vector<int>> node1_partition, vector<vector<int>> node2_partition, vector<int> node1_mark, vector<int> node2_mark, vector<vector<int>>& tempPartition, vector<int>& tempMark) {
	vector<vector<int>> temp_partition;
	vector<int> temp1_mark;
	unordered_map<int, int> tempResult;
	if (node1_partition.size() >= node2_partition.size()) {
		temp_partition = node1_partition;
		temp1_mark = node2_mark;
	}
	else {
		temp_partition = node2_partition;
		temp1_mark = node1_mark;
	}
	int start_index = 1;
	for (vector<vector<int>>::iterator i = temp_partition.begin(); i < temp_partition.end(); i++) {
		for (vector<int>::iterator j = (*i).begin(); j < (*i).end(); j++) {
			int tmp_j = 0;
			tmp_j = i - temp_partition.begin();
			tmp_j |= (temp1_mark[*j] << 16);
			if (!tempResult[tmp_j]) {
				tempResult[tmp_j] = start_index;
				tempPartition.push_back(vector<int>());
				tempPartition[start_index - 1].push_back(*j);
				tempMark[*j] = start_index - 1;
				start_index += 1;
			}
			else {
				tempPartition[tempResult[tmp_j] - 1].push_back(*j);
				tempMark[*j] = tempResult[tmp_j] - 1;
			}
		}
	}
}

void output(int X_except_A, int A) {
	string tempOutput = "";
	char buf[5];
	bool flag = true;
	vector<int> temp;
	for (int i = 0; i < 12; i++)
	{
		if (flag && (X_except_A & number_list[i])) {
			temp.push_back(i + 1);
			flag = false;
		}
		else if (!flag && (X_except_A & number_list[i])) {
			temp.push_back(i + 1);
		}
	}

	if (flag) {
		return;
	}
	temp.push_back(A + 1);
	outputInt.push_back(temp);
}

bool cmp(const vector<int> node1, const vector<int> node2) {
	int size;

	if (node1.size() >= node2.size()) {
		size = node2.size() - 2;
	}
	else {
		size = node1.size() - 2;
	}

	if (size == 0) {
		if (node1[0] > node2[0]) {
			return true;
		}else
		if (node1[0] < node2[0]) {
			return false;
		}
		else {
			if (node1.size() == node2.size()) {
				if (node1.back() > node2.back()) {
					return true;
				}
				else if (node1.back() < node2.back()) {
					return false;
				}
			}
			else {
				return (node1.size() > node2.size());
			}
		}
	}
	else {
		for (int i = 0; i <= size; i++) {
			if (node1[i] < node2[i]) {
				return false;
			}
			else if (node1[i] > node2[i]) {
				return true;
			}
		}
		if (node1.back() < node2.back()) {
			return false;
		}
		else if (node1.back() > node2.back()) {
			return true;
		}
	}
	return false;
}

void outputAllResult() {
	vector<int> temp;
	for (vector<vector<int>>::iterator j = outputInt.begin(); j < outputInt.end(); j++)
		for (vector<vector<int>>::iterator i = j + 1; i < outputInt.end(); i++){
			if (cmp(*j, *i))
			{
				temp = outputInt[i - outputInt.begin()];
				outputInt[i - outputInt.begin()] = outputInt[j - outputInt.begin()];
				outputInt[j - outputInt.begin()] = temp;
			}
		}
	FILE *outFile;
	char buf[5];
	int j;
    outFile = fopen("../result/result.txt", "w");
	for (auto i : outputInt) {
		for (j = 0; j < i.size() - 2; j++) {
			sprintf(buf, "%d", i[j]);
			fputs(buf, outFile);
			fputs(" ", outFile);
		}
		sprintf(buf, "%d", i[i.size() - 2]);
		fputs(buf, outFile);
		fputs("->", outFile);
		sprintf(buf, "%d", i.back());
		fputs(buf, outFile);
		fputs("\n", outFile);
	}
	fclose(outFile);
}

