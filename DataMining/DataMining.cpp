// DataMining.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Apriori2.h"

int _tmain(int argc, char* argv[])
{
	Apriori2 myapriori;
	double threashold[5] = { 0.35, 0.30, 0.25, 0.20, 0.15 };
	string filename = string("mushroom.dat");
//	string filename = string("kosarak.dat");
	myapriori.buildDataBase(filename, threashold[0]);
	bool ret = myapriori.LItemSetMap();
	if (ret)
		myapriori.printFreqKSetMap(1);
	system("PAUSE");
	return 0;
}

//#include "Apriori.h"
//
//
//int _tmain(int argc, char* argv[])
//{
//	/*if (argc != 4){
//		cout << "usage: apriori.exe [min_sup] [input-i.txt] [output-i].txt" << endl;
//		return 0;
//	}
//	int min_sup = atoi(argv[1]);*/
//	Apriori apriori("input.txt");
//	double threashold[5] = { 0.3, 0.20, 0.15, 0.10, 0.05 };
//
////	Apriori apriori("mushroom.dat");
//	apriori.buildData(threashold[0]);										//获取文本文件中原始数据
//
//
//	map<int, map_s> L;
//	map_s freq1ItemSetMap = apriori.findFreq1Itemset();			//获取 长度为 1 的频繁项集
//	set<set_s> freqKItemSet = apriori.keySet(freq1ItemSetMap);	//从 1 频繁项集中获取键值
//	L.insert(make_pair(1, freq1ItemSetMap));					//将 1 频繁项集存入 L
//	//for循环退出条件为： 得到频繁 k 项集为空集时
//	for (int k = 2;; ++k){
//		cout << "k = " << k << endl;
//		map_s freqKItemSetMap = apriori.getFreqKItemset(k, freqKItemSet);
//		L.insert(make_pair(k, freqKItemSetMap));
//		if (freqKItemSetMap.size() != 0){
//			set<set_s> freqKItemSetTemp = apriori.keySet(freqKItemSetMap);
//			freqKItemSet = apriori.keySet(freqKItemSetMap);
//		}
//		else{
//		//	cout << "k = " << k << endl;
//			break;
//		}
//	}
//	//打印所有满足 min_sup 的频繁集,长度从 1 到 k
//	map<int, map_s>::iterator allLIt = L.begin();
//	while (allLIt != L.end()){
//		if (allLIt->second.size() != 0){
//			cout << "频繁 " << allLIt->first << " 项集：" << endl;
//			apriori.printMapSet(allLIt->second);
//		}
//		++allLIt;
//	}
//	//将由 input.txt 原始数据计算得到的最大频繁项集存入 output.txt文件
//	ofstream maxFstream;
//	maxFstream.open("output.txt", fstream::out);
//	map<int, map_s>::iterator maxLIt = (--L.end());
//	maxLIt--;
//	map_s::iterator mapIt = maxLIt->second.begin();
//	while (mapIt != maxLIt->second.end()){
//		set_s::iterator maxItSet = mapIt->first.begin();
//		maxFstream << "#" << mapIt->second << "\t" << "[";
//		while (maxItSet != mapIt->first.end()){
//			maxFstream << *maxItSet << ",";
//			++maxItSet;
//		}
//		maxFstream << "]" << endl;
//		++mapIt;
//	}
//	maxFstream.close();
//
//	system("PAUSE");
//	return 0;
//}

