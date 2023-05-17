/* Построение кода Хафмана.

По данной непустой строке s длины не более 10^4, состоящей из строчных букв латинского алфавита, постройте оптимальный беспрефиксный код.
1) В первой строке выведите количество различных букв k, встречающихся в строке, и размер получившейся закодированной строки. 
2) В следующих k строках запишите коды букв в формате "letter: code". 
3) В последней строке выведите закодированную строку.*/

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

using std::cin; using std::cout;
using std::endl; using std::string;
using std::vector; using std::map;

struct Symbol
{
	char letter;
	int freq;
	string code;
};

struct Tree
{
	Symbol leaft;
	Tree * left;
	Tree * right;
};

int main()
{
	string str;
	cin >> str;
	
	// обработка входной строки - создаём массив структур Tree
	// каждый элемент этого массива содержит букву из строки и кол-во её повторений (частоту)
	vector<Tree> tr_sym;
	auto p = str.begin();
	while (p != str.end())
	{
		auto fs = find_if(tr_sym.begin(), tr_sym.end(), [p] (Tree tr) {return tr.leaft.letter == *p;});
		if (fs != tr_sym.end())
			++(fs->leaft.freq);
		else
			tr_sym.push_back({Symbol{*p, 1}, nullptr , nullptr}); 
		++p;
	}
	
	sort(tr_sym.begin(), tr_sym.end(), [] (Tree a, Tree b) {return a.leaft.freq < b.leaft.freq;});
	auto amount = tr_sym.size(); // количество различных букв во входной строке
	tr_sym.resize(2*amount);
	
	int count = 0;
	vector<Tree *> pt_tree;
	for (count = 0; count < amount; ++count)
		pt_tree.push_back(&tr_sym[count]);
	
	//построение дерева
	if (pt_tree.size() == 1)
		pt_tree[0] -> leaft.code = "0";
	while (pt_tree.size() > 1) //пока в pt_tree не будет одной единственной ссылки 
	{
		//сортируем pt_tree по частоте от наименьшей 
		sort(pt_tree.begin(), pt_tree.end(), [] (Tree * a, Tree * b) { return a->leaft.freq < b->leaft.freq; });
				
		//берем два наименьших объекта и создаем новый элемент в tr_sym
		Symbol s {'*', (pt_tree[0]->leaft.freq + pt_tree[1]->leaft.freq), ""};
		tr_sym[count] = {s, pt_tree[1], pt_tree[0]};
		
		//удаляем два наименьших объекта и добавляем ссылку на новый объект в pt_tree
		pt_tree.erase(pt_tree.begin());
		pt_tree.erase(pt_tree.begin());
		pt_tree.push_back(&tr_sym[count]);
		
		++count;
	}
	
	//обход дерева и создание кодов для каждого символа
	while (!(pt_tree.empty()))
	{
		vector<Tree *> left;
		vector<Tree *> right;
		for (int x = 0; x < pt_tree.size(); ++x)
		{
			if ((pt_tree[x] -> left))
			{
				left.push_back(pt_tree[x] -> left);
				pt_tree[x] -> left -> leaft.code = pt_tree[x] -> leaft.code;
			}
			if ((pt_tree[x] -> right))
			{
				right.push_back(pt_tree[x] -> right);
				pt_tree[x] -> right -> leaft.code = pt_tree[x] -> leaft.code;
			}
		}
		
		pt_tree.clear();
		
		for (Tree * lf: left)
		{
			lf->leaft.code += "1";
			pt_tree.push_back(lf);
		}
		for (Tree * rt: right)
		{
			rt->leaft.code += "0";
			pt_tree.push_back(rt);
		}
	}
	
	tr_sym.erase(tr_sym.begin() + amount, tr_sym.end()); //удаляем вспомог. вершины
	sort(tr_sym.begin(), tr_sym.end(), [] (Tree a, Tree b) { return a.leaft.code < b.leaft.code; });
	
	// составления словаря буква-код
	string res;
	map<char, string> dic;
	for (auto & x : tr_sym)
		dic[x.leaft.letter] = x.leaft.code;
	// составление закодированной строки 
	for (auto x : str)
		res += dic[x];
	
	
	//вывод результата
	cout << amount << " " << res.size() << endl;
	for (auto & x : tr_sym)
		cout << x.leaft.letter << ": " << x.leaft.code << endl;
	cout << res << endl;


	return 0;
}