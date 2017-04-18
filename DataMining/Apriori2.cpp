#include "stdafx.h"
#include <vector>
#include <sstream>
#include "Apriori2.h"

/* 打印 set<string>集合
 */
bool Apriori2::printSetStr(const set_s & set_str)
{
	cout << "[";

	/*
	set_s::const_iterator cs_It = set_str.cbegin();
	while (cs_It != set_str.cend()){
		cout << *cs_It << ",";
	}
	*/

	for (const auto & sets_elem : set_str){
		cout << sets_elem << ", ";
	}

	cout << "]\n";

	return true;
}
/* 打印 map_s 集合
 */
bool Apriori2::printMapSetStr(map_s & map_set_str)
{
	/*for (const auto & map_elem : map_set_str){
		cout << "[";
		for (const auto & set_elem : map_elem.first){
			cout << set_elem << ", ";
		}
		cout << "]\n";
	}*/
	cout << "Support:\t" << "ItemSet" << endl;
	for (const auto & map_elem : map_set_str){
		cout << "#" << map_elem.second << "\t";
		printSetStr(map_elem.first);
		
	}
	return true;
}
bool Apriori2::printFreqKSetMap(int k){
	map_s & FreqKSetMap = AllFreqItemSetMap[k];
	return (printMapSetStr(FreqKSetMap));
}
bool Apriori2::isFreqItemSet(int k, set_s & set_str)
{
	if (minSup > 0 ){
		if (getSupportFromDataBase(k, set_str) > minSup)
			return true;
		else
			return true;
	}
	else{
		cout << "isFreqItemSet error, minSupport is null\n";
		return false;
	}
	return false;
}

int Apriori2::getSupportFromDataBase(int k, set_s & set_str)
{
	//for (const auto & multimap_elem : DataBase){ //map pair
	//	for (const auto & set_elem : multimap_elem.second){//set 
	//		for (const auto & str_elem : set_elem){//string
	//			for (const auto & setstr_elem : set_str){

	//			}
	//		}
	//	}
	//}

	return 0;
}
/* 读取 input 文件数据，创建数据库 DataBase
 * 根据 threashold 和数据库行数 dataBaseLine 计算最小支持度
 * minSup = dataBaseLine * threashold
 */
long Apriori2::buildDataBase(string input, double threashold)
{
	dataBaseLine = 0;
	ifstream ifs;
	ifs.open(input.c_str());
	if (!ifs){
		cerr << "File " << input << " open error!\n	";
		return -1;
	}
	string textline;
	vector<string> vec_line;
	cout << "Read data from file:" << input << endl;
	while (getline(ifs, textline))							//读取文件中一行
		vec_line.push_back(textline);						//用向量存每一行
	
	//分割一行中每个单词存入 setsTemp,然后构造 line_num/setsTemp 键值对存入 DataBase
	set_s setsTemp;
	int line_num;
	for (line_num = 0; line_num != vec_line.size(); ++line_num){
		istringstream line(vec_line[line_num]);
		string word;
		setsTemp.clear();
		while (line >> word){
			setsTemp.insert(word);			
		}
		DataBase.insert(make_pair((long)line_num, setsTemp));//插入到数据库
//		++dataBaseLine;										//对行计数
		/*
		cout << "Line :" << dataBaseLine;
		copy(setsTemp.cbegin(), setsTemp.cend(), ostream_iterator<string>(cout," "));
		cout << endl;*/
	}
	dataBaseLine = line_num;
	cout << "Total line :" << dataBaseLine << endl;
	minSup = int(dataBaseLine * threashold);
	cout << "Min Support :" << minSup << endl;
	return dataBaseLine;
}
/* 由数据库扫描，对每一个单词进行计数
 * 生成的 1 项候选集存入 AllFreqItemSetMap的键值 0 中
 */
bool Apriori2::LItemSetMap()
{
	//中间 map 变量
	map<string, int> strItemSetMapTemp;
	map<set<string>, int> cand1Map;
	//从数据中扫描
	if (!DataBase.empty()){		
		for (const auto & data_elem : DataBase){				//数据库中每一个 key/value 元素
			for (const auto & set_elem : data_elem.second){		//每个set<string>集合中的 string 元素 
				//返回值 ret 的 second 成员 表示是否成功将一对 （set_elem) 插入到 strItemSetMapTemp 中
				pair<map<string, int>::iterator, bool> ret = strItemSetMapTemp.insert(make_pair(set_elem, 1));
				if (!ret.second)//失败，表示再次遇到该单词，则增加计数
					++ret.first->second;
			}
		}
	}
	else{
		cerr << "getLItemSetMap failed, DataBase is empty!\n";
		return false;
	}
	//将计数结果转化为 map_s 结构，然后存入 AllFreqItemSetMap 中
	set_s setsTemp;//中间变量
	for (const auto & strMap_elem : strItemSetMapTemp){
		if (strMap_elem.second >= minSup){						//检验支持度
			setsTemp.clear();//清空
			setsTemp.insert(strMap_elem.first);//string 插入到 setsTemp中
			cand1Map.insert(make_pair(setsTemp, strMap_elem.second));// 构造 map_s 结构并插入到 cand1Map中
		}
	}
	AllFreqItemSetMap.clear();
	pair<map<int, map_s>::iterator, bool> ret;
	ret = AllFreqItemSetMap.insert(make_pair(1, cand1Map));//将 1 频繁项集插入到 AllFreqItemSetMap 键值为 1
	if (!ret.second)//如果插入失败，返回 false
		return false;
	return true;//成功存入返回
}

