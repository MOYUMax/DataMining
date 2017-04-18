#include "stdafx.h"
#include "Apriori.h"
#include <sstream>

//打印事物项数据库，数据项和数据单词
void Apriori::printMapSet(map_s & mapSet)
{
	map_s::iterator it = mapSet.begin();
	cout << "Support\t" << "ItemSet\n";
	while (it != mapSet.end()){
		set_s::iterator itSet = it->first.begin();
		cout << "#" << it->second << "\t";
		cout << "[";
		while (itSet != it->first.end()){
			cout << *itSet << ",";
			++itSet;
		}
		cout << "]\n";
		++it;
	}
}
void Apriori::printsetSet(set<set_s> & set_set_str)
{
	set<set_s>::iterator itSet = set_set_str.begin();

	while (itSet != set_set_str.end()){
		set<string>::iterator itSetStr = (*itSet).begin();
		cout << "[";
		while (itSetStr != (*itSet).end()){
			cout << *itSetStr << ",";
			++itSetStr;
		}
		cout << "]" << endl;
		++itSet;
	}
}
void Apriori::printSet(set<string> & set_str)
{
	set<string>::iterator itSet = set_str.begin();
	cout << "[";
	while (itSet != set_str.end()){
		cout << *itSet << ",";
		++itSet;
	}
	cout << "]" << endl;
}
/* 将文本数据存入multimap中,产生事务数据库D,即textDataBase
 * 
 */
int Apriori::buildData(double threshold)
{
	//打开文本文件
	ifstream inFile;
	inFile.open(dataFileName.c_str());
	if (!inFile){
		cerr << "open " << dataFileName << " failed!" << endl;
		return EXIT_FAILURE;
	}
	//读取文本行
	string textline;
	vector<string> lines_of_text;
	while (getline(inFile, textline))
		lines_of_text.push_back(textline);

	//产生事务数据库
	int line_num;
	for (line_num = 0; line_num != lines_of_text.size(); ++line_num){
		istringstream line(lines_of_text[line_num]);
		string word;//单个字符，即由空格分割开的数字
		while (line >> word){
			//将每一行的每个单词（即数字）插入到事务数据库 textDatabase的每一行
			//（单词之间是同一行，但是没有了空格，便于查找）
			textDatabase[line_num].insert(word); //textDatabase[i]代表value即 set<string>,将值word插入 set_s中
		}		
	}
	textDatabaseLine = textDatabase.size();
	minSup = int(textDatabaseLine*threshold);
	cout << "textDatabaseLine:" << textDatabaseLine << "\t MinSupport" << minSup << endl;
	return EXIT_SUCCESS;
}
/* 计算数据库D中每一项（即每个word）出现的频率
 */
map_s Apriori::getTextDatabaseFre()
{
	map_s textDatabaseFre;
	map<long, set<string>>::iterator textDataIt = textDatabase.begin();//数据库行迭代器
	while (textDataIt != textDatabase.end()){
		//扫描数据库，使用 ret 存储 map_s的返回值，如果 Fre中已经存在 set_s了，则insert失败， 
		pair<map_s::iterator, bool> ret = textDatabaseFre.insert(make_pair(textDataIt->second, 1));
		if (!ret.second)
			++ret.first->second;//将 map_s 的值（即int）加 1
		++textDataIt;//下一行
	}
	return textDatabaseFre;
}
/* 获取数据库 D 中每一项（行）的支持度，即统计数据集每一项（行）作为子集出现的频率
 * 注：set<string> 中的string是按从小到大的顺序排列的
 */
map_s Apriori::getTextDatabaseSurpport()
{
	map_s textDatabaseSurpport;
	map_s textDatabaseFre = getTextDatabaseFre(); //获得频繁项集
	map<long, set<string>>::iterator textdataIt1 = textDatabase.begin();
	while (textdataIt1 != textDatabase.end()){
		textDatabaseSurpport.insert(make_pair(textdataIt1->second, 0));//将数据库中第二项 set_s 作为键初始化 Support
		map<long, set_s>::iterator textdataIt2 = textDatabase.begin();
		while (textdataIt2 != textDatabase.end()){
			//扫描一次数据库，取 support 和 数据库中的交集
			if (retainAll(textdataIt1->second, textdataIt2->second) == textdataIt1->second)
				++textDatabaseSurpport[textdataIt1->second];//增加 set_s 的计数
			++textdataIt2;//取 support 中的下一个 set_s
		}
		++textdataIt1;//取第二行
	}
	
	map_s::iterator textDatabaseFreIt = textDatabaseFre.begin();//频繁项集迭代器
	while (textDatabaseFreIt != textDatabaseFre.end()){
		//扫描频繁项集，求频繁项的支持度：即
		textDatabaseSurpport[textDatabaseFreIt->first] = textDatabaseSurpport[textDatabaseFreIt->first] / textDatabaseFreIt->second;
		++textDatabaseFreIt;
	}
	return textDatabaseSurpport;
}
/* 计算获取长度为 1 的候选项集
 */
map<string, int> Apriori::getCandi1ItemSet()
{
	map<string, int> candi1ItemSet;
	map<long, set<string>>::iterator textDatabaseIt = textDatabase.begin();
	set_s::iterator setIt;

	while (textDatabaseIt != textDatabase.end()){//扫描数据库 D
		setIt = textDatabaseIt->second.begin();//行首单词迭代器
		while (setIt != textDatabaseIt->second.end()){//扫描数据库中一项（即一行）中的每个单词
			//通过 insert 方法判断该单词是否在
			pair<map<string, int>::iterator, bool> ret = candi1ItemSet.insert(make_pair(*setIt, 1));
			if (!ret.second)
				++ret.first->second;
			++setIt;
		}
		++textDatabaseIt;//数据库D中下一行
	}
	return candi1ItemSet;
}
/* 计算获取长度为 1 的频繁项集
 */
map_s Apriori::findFreq1Itemset()
{
	set_s freq1Key;//频繁项关键字(一行）
	map_s freq1ItemSetMap;//频繁项集 （多行）
	map<string, int> candidate1ItemSet = getCandi1ItemSet();//获取长度为 1 的候选项集（一个单词）
	map<string, int>::iterator candIt = candidate1ItemSet.begin();//候选项（一个单词）迭代器
	while (candIt != candidate1ItemSet.end()){//扫描候选集
		if (candIt->second >= minSup){//过滤低于支持度的候选项（一个单词）
			//高于支持度的候选项，插入到频繁项集中
			freq1Key.erase(freq1Key.begin(), freq1Key.end());
			freq1Key.insert(candIt->first);
			freq1ItemSetMap[freq1Key] = candIt->second;
		}
		++candIt;//下一个候选项
	}
	return freq1ItemSetMap;//返回长度为 1 （即一行就一个单词）的频繁项集
}
/* 根据长度为 k-1的频繁项集计算长度为 k 的频繁项集
 * k > 1
 */
map_s Apriori::getFreqKItemset(int k, set< set_s> freqMItemSet)
{
	map_s freqKitemSetMap;//存储 K 频繁项集（多行）
	map_s candFreqKItemSetMap;//存储 K 候选集（多行）
	set< set_s> candFreqKItemSet = aprioriGen(k - 1, freqMItemSet);//长度为 k-1 频繁集生成长度为 k 候选集
	int i = 1;
	// 支持度 minSup 值越大， 效率越高， 值越小反之
	map<long, set<string>>::iterator mapIter = textDatabase.begin();//数据库迭代器
	//下面的while循环效率很低
	while (mapIter != textDatabase.end()){
		set_s wordLine = mapIter->second;
		set< set_s>::iterator kIt = candFreqKItemSet.begin();//
		cout << "Line " << i++ << endl;
		printSet(wordLine);
		while (kIt != candFreqKItemSet.end()){
			set_s kSet = *kIt;
			set_s setTemp(kSet.begin(), kSet.end());
			removeAll(setTemp, wordLine);//将setTemp中的 wordLine删除
			if (setTemp.size() == 0){
				pair< map_s::iterator, bool> ret = candFreqKItemSetMap.insert(make_pair(kSet, 1));
				if (!ret.second)
					++ret.first->second;
			}
			++kIt;
		}
		++mapIter;
	}

	map_s::iterator candIt = candFreqKItemSetMap.begin();
	while (candIt != candFreqKItemSetMap.end()){
		if (candIt->second >= minSup){//过滤低于支持度的频繁项（一行）
			freqKitemSetMap[candIt->first] = candIt->second;
		}
		++candIt;
	}
	return freqKitemSetMap;
}
/* 取两项的交集,返回 set1 N set2
 */
set_s Apriori::retainAll(set_s set1, set_s set2)
{
	set_s::iterator setIt = set1.begin();
	set_s ret;
	while (setIt != set1.end()){
		set_s::iterator set2It = set2.begin();
		while (set2It != set2.end()){
			if ((*setIt) == (*set2It)){
				ret.insert(*setIt);
				break;
			}
			++set2It;
		}
		++setIt;
	}
	return ret;
}
/* 取两项的差集,返回 set1 - set2
 */
void Apriori::removeAll(set_s &set1, set_s &set2)
{
	set_s::iterator set2It = set2.begin();
	while (set2It != set2.end()){
		set1.erase(*set2It);
		++set2It;
		if (set1.size() == 0)
			break;
	}
	return;
}
/* 取两项的并集，返回 set1 U set2
 */
set_s Apriori::addAll(set_s &set1, set_s &set2)
{
	set_s::iterator set1It = set1.begin();
	set_s::iterator set2It = set2.begin();
	set_s retSet(set1.begin(), set1.end());//初始化retSet，并将set1复制到里面
	while (set2It != set2.end()){
		retSet.insert(*set2It);
		++set2It;
	}
	return retSet;
}
/* 由长度为 k-1(m) 频繁项集获取长度为 k(m+1)候选项集
 * freqMItemSet: 长度 k-1(m) 频繁项集
 */
set<set_s> Apriori::aprioriGen(int m, set<set_s> & freqMItemSet)
{
	cout << " M 频繁项集： " << endl;
	printsetSet(freqMItemSet);
	int line = 1;
	int word = 1;

	set<set_s> candFreqKItemSet;//频繁项集（多行）
	set<set_s>::iterator freqIt = freqMItemSet.begin();//迭代器： m 频繁项集（多行，一行有 m 个单词）
	set_s originItemSet;//原始项(一行，长度为 k-1(m))
	set_s identiItemSet;//目标项(一行，长度为 k(m+1))
	
	cout << "aprioriGen start" << endl;
	while (freqIt != freqMItemSet.end()){		
		originItemSet = *freqIt;						//使原始项 等于freqIt所指行开始的 m 频繁项集
		cout << "Line ：" << line++ << endl;		
		printSet(originItemSet);//测试打印一行 m 频繁项

		set<set_s>::iterator freqItNext = ++freqIt;		//指向 originItemSet 的下一行开始的 m 频繁项集
		while (freqItNext != freqMItemSet.end()){
			
			set_s identicalSet(originItemSet.begin(), originItemSet.end());		//复制 原始项集 所指部分到 identicalSet
			set_s setS(*freqItNext);											//复制 原始项集下一行所指部分到 setS
			identiItemSet.erase(identiItemSet.begin(), identiItemSet.end());	//清空目标项集
			identiItemSet = addAll(identicalSet, setS);							// 取 identicalSet 与 setS 的并集
			//当 目标项集 长度 达到 k（m+1）时
			if (identiItemSet.size() == m + 1){
				//判断 m 频繁项集是否包含 目标项 identiItemSet,如果不包含，就插入到候选项集 candFreqKItemSet
				if (!hasFreqKSubset(identiItemSet, freqMItemSet)){
					candFreqKItemSet.insert(identiItemSet);
					/*cout << "word:" << word++ << endl;
					printSet(identiItemSet);*/
				}
			}
			++freqItNext;	// m 频繁项集 下一行
		}
	}
	return candFreqKItemSet;
}
/* 根据先验知识，剪枝，判断频繁项集中是否存在 k项集的长度为k-1项的所有子集
 * 返回 freqMItemSet 是否包含 candKItemSet的真子集, m = k
 */
bool Apriori::hasFreqKSubset(set_s & candKItemSet, set<set_s> & freqMItemSet)
{
	int occurse = 0;

	if (freqMItemSet.count(candKItemSet))
		return true;
	return false;
}

/* 获取 mapSet 的键值（即一行，每行1或者多个单词，每个单词用一个 set_s 存储
 */
set<set_s> Apriori::keySet(map_s & mapSet)
{
	map_s::iterator mapIt = mapSet.begin();
	set<set_s> retSet;
	while (mapIt != mapSet.end()){
		retSet.insert(mapIt->first);
		++mapIt;
	}
	return retSet;
}