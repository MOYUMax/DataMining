#ifndef _APRIORI_H
#define _APRIORI_H

#include <iostream>	//��׼�������
#include <cstdlib>	//
#include <map>		//map ��������ģ���
#include <set>		//set ��������ģ���
#include <fstream>	//�ļ�IO
#include <list>		//list����ģ��
#include <utility>	//
#include <vector>	//��������ģ��
#include <string>	//�ַ���������

using namespace std;

/* Apriori ������ʵ�� apriori Ƶ��ģʽ�ھ��㷨
 * @author MaxWang
 * @qq 1716766991
 * @mail 1716766991@qq.com
 * 
*/
//typedef map< set< set<string> >, int> map_ss;	//���ڴ洢�������ݼ�DataSet��N�У�
typedef map< set<string>, int> map_s;			//���ڴ洢һ��������Data��һ�У�

typedef set<string> set_s;						//���ڴ洢һ�����ݣ��м�ֵ��
//struct set_s{
//	set<string> Element;
//};
//struct map_s{
//	map<set<string>, int> Element;
//};

class Apriori{
private:
	string dataFileName;						//data���������ļ�
	int minSup;								//��С֧�ֶ�
	map<long,set<string>> textDatabase;			//�������ݿ�D(���治���ո��һ�������
	long textDatabaseLine;						//�������ݿ���������
	//map_ss CandiK;								//��ѡ�� Ck������Ϊ k
	//map_ss FreaqK;								//Ƶ��� Lk������Ϊ k

	//˽�к���
	void removeAll(set_s & set1, set_s &set2);	//ɾ������ set<string> ��
	set_s addAll(set_s & set1, set_s &set2);	//�������� set<string> ��
public:
	//���ι��캯��������data_filename:�ļ�·����minSup:֧�ֶ�
	Apriori(string data_filename) :dataFileName(data_filename){}
	
	int getMinSup();
	//��������
	void printMapSet(map_s & map_set);			//��ӡ�������ݿ� map_s
	void printsetSet(set<set_s> & set_set_str);	//��ӡ set<set<string>> ������
	void printSet(set_s & set_str);				//��ӡ set<string>
	
	int buildData(double threshold);							//��ȡ���ݿ⡢���ɻ�������
	map_s getTextDatabaseFre();					//����������ݿ���ÿһ�ÿ�����֣����ֵ�Ƶ��
	map_s getTextDatabaseSurpport();			//��ȡ���ݿ�֧�ֶ�
	map<string, int> getCandi1ItemSet();			//��ȡ��ѡ��
	map_s findFreq1Itemset();					//�����ݿ����Ƶ����

	set<set_s> aprioriGen(int m, set<set_s> & set_set_str);		//���ɳ���Ϊ m �ĺ�ѡ��
	bool hasFreqKSubset(set_s & s1, set<set_s> & set_set_str);	//�ж����ݿⵥ���Ƿ���������� s1
	map_s getFreqKItemset(int k, set<set_s> freqMItemset);		//��ȡ����Ϊ k ��Ƶ����
	set<set_s> keySet(map_s & mapSet);			//���� mapSet �еļ�����set<set<string>> ֵ
	set_s retainAll(set_s set1,set_s set2);		//
//	list<set_s> getSubSets(set_s setK);			//���� set_s �� k-1���Ӽ�
};

#endif