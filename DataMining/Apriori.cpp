#include "stdafx.h"
#include "Apriori.h"
#include <sstream>

//��ӡ���������ݿ⣬����������ݵ���
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
/* ���ı����ݴ���multimap��,�����������ݿ�D,��textDataBase
 * 
 */
int Apriori::buildData(double threshold)
{
	//���ı��ļ�
	ifstream inFile;
	inFile.open(dataFileName.c_str());
	if (!inFile){
		cerr << "open " << dataFileName << " failed!" << endl;
		return EXIT_FAILURE;
	}
	//��ȡ�ı���
	string textline;
	vector<string> lines_of_text;
	while (getline(inFile, textline))
		lines_of_text.push_back(textline);

	//�����������ݿ�
	int line_num;
	for (line_num = 0; line_num != lines_of_text.size(); ++line_num){
		istringstream line(lines_of_text[line_num]);
		string word;//�����ַ������ɿո�ָ������
		while (line >> word){
			//��ÿһ�е�ÿ�����ʣ������֣����뵽�������ݿ� textDatabase��ÿһ��
			//������֮����ͬһ�У�����û���˿ո񣬱��ڲ��ң�
			textDatabase[line_num].insert(word); //textDatabase[i]����value�� set<string>,��ֵword���� set_s��
		}		
	}
	textDatabaseLine = textDatabase.size();
	minSup = int(textDatabaseLine*threshold);
	cout << "textDatabaseLine:" << textDatabaseLine << "\t MinSupport" << minSup << endl;
	return EXIT_SUCCESS;
}
/* �������ݿ�D��ÿһ���ÿ��word�����ֵ�Ƶ��
 */
map_s Apriori::getTextDatabaseFre()
{
	map_s textDatabaseFre;
	map<long, set<string>>::iterator textDataIt = textDatabase.begin();//���ݿ��е�����
	while (textDataIt != textDatabase.end()){
		//ɨ�����ݿ⣬ʹ�� ret �洢 map_s�ķ���ֵ����� Fre���Ѿ����� set_s�ˣ���insertʧ�ܣ� 
		pair<map_s::iterator, bool> ret = textDatabaseFre.insert(make_pair(textDataIt->second, 1));
		if (!ret.second)
			++ret.first->second;//�� map_s ��ֵ����int���� 1
		++textDataIt;//��һ��
	}
	return textDatabaseFre;
}
/* ��ȡ���ݿ� D ��ÿһ��У���֧�ֶȣ���ͳ�����ݼ�ÿһ��У���Ϊ�Ӽ����ֵ�Ƶ��
 * ע��set<string> �е�string�ǰ���С�����˳�����е�
 */
map_s Apriori::getTextDatabaseSurpport()
{
	map_s textDatabaseSurpport;
	map_s textDatabaseFre = getTextDatabaseFre(); //���Ƶ���
	map<long, set<string>>::iterator textdataIt1 = textDatabase.begin();
	while (textdataIt1 != textDatabase.end()){
		textDatabaseSurpport.insert(make_pair(textdataIt1->second, 0));//�����ݿ��еڶ��� set_s ��Ϊ����ʼ�� Support
		map<long, set_s>::iterator textdataIt2 = textDatabase.begin();
		while (textdataIt2 != textDatabase.end()){
			//ɨ��һ�����ݿ⣬ȡ support �� ���ݿ��еĽ���
			if (retainAll(textdataIt1->second, textdataIt2->second) == textdataIt1->second)
				++textDatabaseSurpport[textdataIt1->second];//���� set_s �ļ���
			++textdataIt2;//ȡ support �е���һ�� set_s
		}
		++textdataIt1;//ȡ�ڶ���
	}
	
	map_s::iterator textDatabaseFreIt = textDatabaseFre.begin();//Ƶ���������
	while (textDatabaseFreIt != textDatabaseFre.end()){
		//ɨ��Ƶ�������Ƶ�����֧�ֶȣ���
		textDatabaseSurpport[textDatabaseFreIt->first] = textDatabaseSurpport[textDatabaseFreIt->first] / textDatabaseFreIt->second;
		++textDatabaseFreIt;
	}
	return textDatabaseSurpport;
}
/* �����ȡ����Ϊ 1 �ĺ�ѡ�
 */
map<string, int> Apriori::getCandi1ItemSet()
{
	map<string, int> candi1ItemSet;
	map<long, set<string>>::iterator textDatabaseIt = textDatabase.begin();
	set_s::iterator setIt;

	while (textDatabaseIt != textDatabase.end()){//ɨ�����ݿ� D
		setIt = textDatabaseIt->second.begin();//���׵��ʵ�����
		while (setIt != textDatabaseIt->second.end()){//ɨ�����ݿ���һ���һ�У��е�ÿ������
			//ͨ�� insert �����жϸõ����Ƿ���
			pair<map<string, int>::iterator, bool> ret = candi1ItemSet.insert(make_pair(*setIt, 1));
			if (!ret.second)
				++ret.first->second;
			++setIt;
		}
		++textDatabaseIt;//���ݿ�D����һ��
	}
	return candi1ItemSet;
}
/* �����ȡ����Ϊ 1 ��Ƶ���
 */
map_s Apriori::findFreq1Itemset()
{
	set_s freq1Key;//Ƶ����ؼ���(һ�У�
	map_s freq1ItemSetMap;//Ƶ��� �����У�
	map<string, int> candidate1ItemSet = getCandi1ItemSet();//��ȡ����Ϊ 1 �ĺ�ѡ���һ�����ʣ�
	map<string, int>::iterator candIt = candidate1ItemSet.begin();//��ѡ�һ�����ʣ�������
	while (candIt != candidate1ItemSet.end()){//ɨ���ѡ��
		if (candIt->second >= minSup){//���˵���֧�ֶȵĺ�ѡ�һ�����ʣ�
			//����֧�ֶȵĺ�ѡ����뵽Ƶ�����
			freq1Key.erase(freq1Key.begin(), freq1Key.end());
			freq1Key.insert(candIt->first);
			freq1ItemSetMap[freq1Key] = candIt->second;
		}
		++candIt;//��һ����ѡ��
	}
	return freq1ItemSetMap;//���س���Ϊ 1 ����һ�о�һ�����ʣ���Ƶ���
}
/* ���ݳ���Ϊ k-1��Ƶ������㳤��Ϊ k ��Ƶ���
 * k > 1
 */
map_s Apriori::getFreqKItemset(int k, set< set_s> freqMItemSet)
{
	map_s freqKitemSetMap;//�洢 K Ƶ��������У�
	map_s candFreqKItemSetMap;//�洢 K ��ѡ�������У�
	set< set_s> candFreqKItemSet = aprioriGen(k - 1, freqMItemSet);//����Ϊ k-1 Ƶ�������ɳ���Ϊ k ��ѡ��
	int i = 1;
	// ֧�ֶ� minSup ֵԽ�� Ч��Խ�ߣ� ֵԽС��֮
	map<long, set<string>>::iterator mapIter = textDatabase.begin();//���ݿ������
	//�����whileѭ��Ч�ʺܵ�
	while (mapIter != textDatabase.end()){
		set_s wordLine = mapIter->second;
		set< set_s>::iterator kIt = candFreqKItemSet.begin();//
		cout << "Line " << i++ << endl;
		printSet(wordLine);
		while (kIt != candFreqKItemSet.end()){
			set_s kSet = *kIt;
			set_s setTemp(kSet.begin(), kSet.end());
			removeAll(setTemp, wordLine);//��setTemp�е� wordLineɾ��
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
		if (candIt->second >= minSup){//���˵���֧�ֶȵ�Ƶ���һ�У�
			freqKitemSetMap[candIt->first] = candIt->second;
		}
		++candIt;
	}
	return freqKitemSetMap;
}
/* ȡ����Ľ���,���� set1 N set2
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
/* ȡ����Ĳ,���� set1 - set2
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
/* ȡ����Ĳ��������� set1 U set2
 */
set_s Apriori::addAll(set_s &set1, set_s &set2)
{
	set_s::iterator set1It = set1.begin();
	set_s::iterator set2It = set2.begin();
	set_s retSet(set1.begin(), set1.end());//��ʼ��retSet������set1���Ƶ�����
	while (set2It != set2.end()){
		retSet.insert(*set2It);
		++set2It;
	}
	return retSet;
}
/* �ɳ���Ϊ k-1(m) Ƶ�����ȡ����Ϊ k(m+1)��ѡ�
 * freqMItemSet: ���� k-1(m) Ƶ���
 */
set<set_s> Apriori::aprioriGen(int m, set<set_s> & freqMItemSet)
{
	cout << " M Ƶ����� " << endl;
	printsetSet(freqMItemSet);
	int line = 1;
	int word = 1;

	set<set_s> candFreqKItemSet;//Ƶ��������У�
	set<set_s>::iterator freqIt = freqMItemSet.begin();//�������� m Ƶ��������У�һ���� m �����ʣ�
	set_s originItemSet;//ԭʼ��(һ�У�����Ϊ k-1(m))
	set_s identiItemSet;//Ŀ����(һ�У�����Ϊ k(m+1))
	
	cout << "aprioriGen start" << endl;
	while (freqIt != freqMItemSet.end()){		
		originItemSet = *freqIt;						//ʹԭʼ�� ����freqIt��ָ�п�ʼ�� m Ƶ���
		cout << "Line ��" << line++ << endl;		
		printSet(originItemSet);//���Դ�ӡһ�� m Ƶ����

		set<set_s>::iterator freqItNext = ++freqIt;		//ָ�� originItemSet ����һ�п�ʼ�� m Ƶ���
		while (freqItNext != freqMItemSet.end()){
			
			set_s identicalSet(originItemSet.begin(), originItemSet.end());		//���� ԭʼ� ��ָ���ֵ� identicalSet
			set_s setS(*freqItNext);											//���� ԭʼ���һ����ָ���ֵ� setS
			identiItemSet.erase(identiItemSet.begin(), identiItemSet.end());	//���Ŀ���
			identiItemSet = addAll(identicalSet, setS);							// ȡ identicalSet �� setS �Ĳ���
			//�� Ŀ��� ���� �ﵽ k��m+1��ʱ
			if (identiItemSet.size() == m + 1){
				//�ж� m Ƶ����Ƿ���� Ŀ���� identiItemSet,������������Ͳ��뵽��ѡ� candFreqKItemSet
				if (!hasFreqKSubset(identiItemSet, freqMItemSet)){
					candFreqKItemSet.insert(identiItemSet);
					/*cout << "word:" << word++ << endl;
					printSet(identiItemSet);*/
				}
			}
			++freqItNext;	// m Ƶ��� ��һ��
		}
	}
	return candFreqKItemSet;
}
/* ��������֪ʶ����֦���ж�Ƶ������Ƿ���� k��ĳ���Ϊk-1��������Ӽ�
 * ���� freqMItemSet �Ƿ���� candKItemSet�����Ӽ�, m = k
 */
bool Apriori::hasFreqKSubset(set_s & candKItemSet, set<set_s> & freqMItemSet)
{
	int occurse = 0;

	if (freqMItemSet.count(candKItemSet))
		return true;
	return false;
}

/* ��ȡ mapSet �ļ�ֵ����һ�У�ÿ��1���߶�����ʣ�ÿ��������һ�� set_s �洢
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