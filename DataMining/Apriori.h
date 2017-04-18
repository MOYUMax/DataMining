#ifndef _APRIORI_H
#define _APRIORI_H

#include <iostream>	//标准输入输出
#include <cstdlib>	//
#include <map>		//map 关联容器模板库
#include <set>		//set 关联容器模板库
#include <fstream>	//文件IO
#include <list>		//list容器模板
#include <utility>	//
#include <vector>	//向量容器模板
#include <string>	//字符串函数库

using namespace std;

/* Apriori 类用于实现 apriori 频繁模式挖掘算法
 * @author MaxWang
 * @qq 1716766991
 * @mail 1716766991@qq.com
 * 
*/
//typedef map< set< set<string> >, int> map_ss;	//用于存储所有数据集DataSet（N行）
typedef map< set<string>, int> map_s;			//用于存储一个数据项Data（一行）

typedef set<string> set_s;						//用于存储一个数据（中间值）
//struct set_s{
//	set<string> Element;
//};
//struct map_s{
//	map<set<string>, int> Element;
//};

class Apriori{
private:
	string dataFileName;						//data输入数据文件
	int minSup;								//最小支持度
	map<long,set<string>> textDatabase;			//事务数据库D(即存不带空格的一行事物项）
	long textDatabaseLine;						//事务数据库数据行数
	//map_ss CandiK;								//候选集 Ck，长度为 k
	//map_ss FreaqK;								//频繁项集 Lk，长度为 k

	//私有函数
	void removeAll(set_s & set1, set_s &set2);	//删除所有 set<string> 项
	set_s addAll(set_s & set1, set_s &set2);	//加入所有 set<string> 项
public:
	//带参构造函数，参数data_filename:文件路径；minSup:支持度
	Apriori(string data_filename) :dataFileName(data_filename){}
	
	int getMinSup();
	//方法函数
	void printMapSet(map_s & map_set);			//打印事务数据库 map_s
	void printsetSet(set<set_s> & set_set_str);	//打印 set<set<string>> 数据项
	void printSet(set_s & set_str);				//打印 set<string>
	
	int buildData(double threshold);							//读取数据库、生成基础数据
	map_s getTextDatabaseFre();					//获得事务数据库中每一项（每个数字）出现的频率
	map_s getTextDatabaseSurpport();			//读取数据库支持度
	map<string, int> getCandi1ItemSet();			//读取候选集
	map_s findFreq1Itemset();					//从数据库查找频繁集

	set<set_s> aprioriGen(int m, set<set_s> & set_set_str);		//生成长度为 m 的候选集
	bool hasFreqKSubset(set_s & s1, set<set_s> & set_set_str);	//判读数据库单行是否包含数据项 s1
	map_s getFreqKItemset(int k, set<set_s> freqMItemset);		//读取长度为 k 的频繁项
	set<set_s> keySet(map_s & mapSet);			//返回 mapSet 中的键（即set<set<string>> 值
	set_s retainAll(set_s set1,set_s set2);		//
//	list<set_s> getSubSets(set_s setK);			//返回 set_s 的 k-1项子集
};

#endif