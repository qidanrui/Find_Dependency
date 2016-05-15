#ifndef BASIC_H
#define BASIC_H
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include "datastructure.h"
#include <unordered_map>
#include <vector>
#include <algorithm>

extern data_node m_data[100000];
extern int total_number;

extern int readFromFile(string filename);
extern data_node spiltline(string buffer, int length);
extern void get_rhs(unordered_map<int, int>& node_to_rhs, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int layer, int attr_num);
extern void tane(data_node data[], int attr_num);
extern void generateNextLevel(unordered_map<int, int>& next_layer, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int l, int attr_num);
extern void generateNextList(int* next_list, graph_node& node, int attr_num);
extern void computeDependency(vector<graph_node>& current_layer, unordered_map<int, int>& node_to_rhs, int layer, unordered_map<int, vector< vector<int> > >& partition, unordered_map<int, vector<int> >& mark);
extern void getPartition(unordered_map<int, vector<vector<int>>>& partition, unordered_map<int, vector<int>>& mark, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int layer, int attr_num);
extern void getFirstLayerPartition(data_node data[], unordered_map<int, vector<vector<int>>>& partition, unordered_map<int, vector<int>>& mark, int attr_num);
extern void calculatePartition(vector<vector<int>> node1_partition, vector<vector<int>> node2_partition, vector<int> node1_mark, vector<int> node2_mark, vector<vector<int>>& tempPartition, vector<int>& tempMark);
extern bool judgeValid(vector<vector<int>> node1, vector<vector<int>> node2);
extern void output(int X_except_A, int A);
extern bool cmp(vector<int> node1, vector<int> node2);
extern void outputAllResult();
#endif
