#include "stdafx.h"
#include <vector>
#include <sstream>
#include "Apriori2.h"

/* ��ӡ set<string>����
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
/* ��ӡ map_s ����
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
/* ��ȡ input �ļ����ݣ��������ݿ� DataBase
 * ���� threashold �����ݿ����� dataBaseLine ������С֧�ֶ�
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
	while (getline(ifs, textline))							//��ȡ�ļ���һ��
		vec_line.push_back(textline);						//��������ÿһ��
	
	//�ָ�һ����ÿ�����ʴ��� setsTemp,Ȼ���� line_num/setsTemp ��ֵ�Դ��� DataBase
	set_s setsTemp;
	int line_num;
	for (line_num = 0; line_num != vec_line.size(); ++line_num){
		istringstream line(vec_line[line_num]);
		string word;
		setsTemp.clear();
		while (line >> word){
			setsTemp.insert(word);			
		}
		DataBase.insert(make_pair((long)line_num, setsTemp));//���뵽���ݿ�
//		++dataBaseLine;										//���м���
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
/* �����ݿ�ɨ�裬��ÿһ�����ʽ��м���
 * ���ɵ� 1 ���ѡ������ AllFreqItemSetMap�ļ�ֵ 0 ��
 */
bool Apriori2::LItemSetMap()
{
	//�м� map ����
	map<string, int> strItemSetMapTemp;
	map<set<string>, int> cand1Map;
	//��������ɨ��
	if (!DataBase.empty()){		
		for (const auto & data_elem : DataBase){				//���ݿ���ÿһ�� key/value Ԫ��
			for (const auto & set_elem : data_elem.second){		//ÿ��set<string>�����е� string Ԫ�� 
				//����ֵ ret �� second ��Ա ��ʾ�Ƿ�ɹ���һ�� ��set_elem) ���뵽 strItemSetMapTemp ��
				pair<map<string, int>::iterator, bool> ret = strItemSetMapTemp.insert(make_pair(set_elem, 1));
				if (!ret.second)//ʧ�ܣ���ʾ�ٴ������õ��ʣ������Ӽ���
					++ret.first->second;
			}
		}
	}
	else{
		cerr << "getLItemSetMap failed, DataBase is empty!\n";
		return false;
	}
	//���������ת��Ϊ map_s �ṹ��Ȼ����� AllFreqItemSetMap ��
	set_s setsTemp;//�м����
	for (const auto & strMap_elem : strItemSetMapTemp){
		if (strMap_elem.second >= minSup){						//����֧�ֶ�
			setsTemp.clear();//���
			setsTemp.insert(strMap_elem.first);//string ���뵽 setsTemp��
			cand1Map.insert(make_pair(setsTemp, strMap_elem.second));// ���� map_s �ṹ�����뵽 cand1Map��
		}
	}
	AllFreqItemSetMap.clear();
	pair<map<int, map_s>::iterator, bool> ret;
	ret = AllFreqItemSetMap.insert(make_pair(1, cand1Map));//�� 1 Ƶ������뵽 AllFreqItemSetMap ��ֵΪ 1
	if (!ret.second)//�������ʧ�ܣ����� false
		return false;
	return true;//�ɹ����뷵��
}

