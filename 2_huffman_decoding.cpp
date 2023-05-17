/*Восстановите строку по её беспрефиксному коду символов. 

В первой строке входного файла заданы два целых числа k и l через пробел — 
количество различных букв, встречающихся в строке, и размер получившейся закодированной строки, соответственно. 
В следующих k строках записаны коды букв в формате "letter: code". Ни один код не является префиксом другого. 
Буквы могут быть перечислены в любом порядке. В качестве букв могут встречаться лишь строчные буквы латинского алфавита; 
каждая из этих букв встречается в строке хотя бы один раз. 
Наконец, в последней строке записана закодированная строка. Исходная строка и коды всех букв непусты. 
Заданный код таков, что закодированная строка имеет минимальный возможный размер.

В первой строке выходного файла выведите строку s. Она должна состоять из строчных букв латинского алфавита. 
Гарантируется, что длина правильного ответа не превосходит 10^4 символов.*/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin; using std::cout;
using std::endl; using std::string;
using std::vector; 

struct Symbol
{
	char letter;
	string code;
};

struct Tree
{
	Symbol leaft;
	Tree * left;
	Tree * right;
};

bool comp_code(Tree * a, Tree * b)
{
	string a_code = a->leaft.code;
	string b_code = b->leaft.code;
	if (a_code.size() == b_code.size())
		return a_code > b_code;
	return a_code.size() > b_code.size();
}
	

int main(int arg, char *files[])
{
	//чтение исходных данных
	int amount = 0, size = 0;
	cin >> amount >> size;
	vector<Tree> tree(2*amount);
	for (int i = 0; i < amount; ++i)
	{
		string code;
		char letter;
		cin >> letter;
		cin.get();
		cin.get();
		cin >> code;
		
		tree[i] = Tree{{letter, code}, nullptr, nullptr};
	}
	string chipher;
	cin >> chipher;
	
	//массив указателей на вершины
	int count = 0;
	vector<Tree *> pt_tree;
	for (count = 0; count < amount; ++count)
		pt_tree.push_back(&tree[count]);
	
	//построение дерева	
	while (pt_tree.size() > 1)
	{
		sort(pt_tree.begin(), pt_tree.end(), comp_code);
		Symbol s {'*', pt_tree[0]->leaft.code.substr(0, pt_tree[0]->leaft.code.size() - 1)};
		tree[count] = {s, pt_tree[1], pt_tree[0]};

		pt_tree.erase(pt_tree.begin());
		pt_tree.erase(pt_tree.begin());
		pt_tree.push_back(&tree[count]);
		
		count++;
	}
	
	//расшифровка по дереву
	int line = 0;
	string dechipher;
	if (chipher.size() == 1)
		dechipher = pt_tree[0] -> leaft.letter;
	while (chipher.size() > line && chipher.size() != 1)
	{
		Tree * tr_temp = pt_tree[0];
		while (tr_temp->left && tr_temp->right) //пока не лист дерева
		{
			string chi_code = chipher.substr(line++, 1);
			tr_temp = (chi_code == "0") ? tr_temp->left : tr_temp->right;
		}
		dechipher += tr_temp -> leaft.letter;
	}

	cout << dechipher << endl;

	
	return 0;
}