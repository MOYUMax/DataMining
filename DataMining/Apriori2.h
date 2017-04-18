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
	string inFile;							//�����ļ�
	string outFile;							//�������ļ�
	int minSup;								//��С֧�ֶ�
	multimap<long, set<string> > DataBase;	//���ݿ� ���������ļ����벢�������룬һ�ж�Ӧ�����ļ�һ��
	long dataBaseLine;						//
	map<int, map_s> AllFreqItemSetMap;		//����Ƶ����洢�ĵط�
	//map_s CKItemSetMap;						// K ��ѡ��
	//map_s LKItemSetMap;						// K Ƶ����

public:
	bool printSetStr(const set_s & set_str);						//��ӡ������ set<string> 
	bool printMapSetStr(map_s & map_set_str_int);			//��ӡ���ݼ� map<set<string>>
	bool printFreqKSetMap(int k);							//��ӡ K Ƶ�������AllFreqItemSetMap

	long buildDataBase(string input,double threashold);		//�������ݿ�,������С֧�ֶ�
	int getSupportFromDataBase(int k, set_s & set_str);		//�������ݿ��м���֧�ֶ�
	bool isFreqItemSet(int k, set_s & set_str);				//�������ݿ��ж� k ��ѡ���Ƿ���Ƶ�������֧�ֶȣ��ж��Ƿ������С֧�ֶ�
	
//	bool getCItemSetMap();					//�������ݿ��ȡ 1 ��ѡ��
//	bool getLItemSetMap();						//���� 1 ��ѡ����ȡ 1 Ƶ���
	bool LItemSetMap();						//���� ���ݿ� ��ȡ 1 Ƶ���������AllFreqItemSetMap(key=1)
	bool CKItemSetMap(map_s & ck_map, map_s & lk_1_map);	//***���� k-1��ѡ������ k Ƶ���***
	bool LKItemSetMap(map_s & lk_map, map_s ck_map);		//***����  k ��ѡ������ k Ƶ���***
	bool isCutable(int k, set_s & set_str);					//�жϸ� k ��ѡ���Ƿ���Լ�֦
	bool isLinkable(int k, set_s & set1, set_s & set2);		//�ж����� k Ƶ�����Ƿ�������� k+1 ��ѡ��
	
};

#endif