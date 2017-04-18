#ifndef _APRIORI2_H
#define _APRIORI2_H
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <fstream>

using namespace std;
typedef map< set<string> , int> map_s;
typedef set<string> set_s;

class Apriori2
{
private:
	string inFile;							//输入文件
	string outFile;							//结果输出文件
	int minSup;								//最小支持度
	multimap<long, set<string> > DataBase;	//数据库 ，从输入文件读入并处理后存入，一行对应输入文件一行
	long dataBaseLine;						//
	map<int, map_s> AllFreqItemSetMap;		//所有频繁项集存储的地方
	//map_s CKItemSetMap;						// K 候选集
	//map_s LKItemSetMap;						// K 频繁集

public:
	bool printSetStr(const set_s & set_str);						//打印行数据 set<string> 
	bool printMapSetStr(map_s & map_set_str_int);			//打印数据集 map<set<string>>
	bool printFreqKSetMap(int k);							//打印 K 频繁项集，从AllFreqItemSetMap

	long buildDataBase(string input,double threashold);		//创建数据库,设置最小支持度
	int getSupportFromDataBase(int k, set_s & set_str);		//根据数据库中计算支持度
	bool isFreqItemSet(int k, set_s & set_str);				//根据数据库判断 k 候选项是否是频繁项（计算支持度，判断是否大于最小支持度
	
//	bool getCItemSetMap();					//根据数据库获取 1 候选集
//	bool getLItemSetMap();						//根据 1 候选集获取 1 频繁项集
	bool LItemSetMap();						//根据 数据库 获取 1 频繁项集，存入AllFreqItemSetMap(key=1)
	bool CKItemSetMap(map_s & ck_map, map_s & lk_1_map);	//***根据 k-1候选集产生 k 频繁项集***
	bool LKItemSetMap(map_s & lk_map, map_s ck_map);		//***根据  k 候选集产生 k 频繁项集***
	bool isCutable(int k, set_s & set_str);					//判断该 k 候选项是否可以剪枝
	bool isLinkable(int k, set_s & set1, set_s & set2);		//判断两个 k 频繁项是否可以生成 k+1 候选项
	
};

#endif