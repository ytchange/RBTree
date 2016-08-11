#include <iostream>
#include <windows.h>
using namespace std;

enum colour
{
	Black,
	Red
};
template<class K,class V>
struct RBTreeNode
{
	K _key;
	V _value;
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
	colour _col;//节点颜色
	RBTreeNode(const K&key,const V&value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _col(Red)
	{}
};
template<class K,class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}
	bool Insert(const K&key,const V&value)
	{
		if (_root == NULL)
		{
			Node*tmp = new Node(key, value);
			_root = tmp;
			_root->_col = Black;
		}
		else
		{
			Node *cur = _root;
			Node *parent = NULL;
			while (cur)
			{
				if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key>key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else
				{
					break;
				}
			}
			if (cur != NULL)
			{
				return false;
			}
			//以上找到要插入的地方
			if (parent->_key>key)
			{
				Node *tmp = new Node(key, value);
				parent->_left = tmp;
				tmp->_parent = parent;
				cur = tmp;
			}
			else 
			{
				Node *tmp = new Node(key, value);
				parent->_right = tmp;
				tmp->_parent = parent;
				cur = tmp;
			}
			//以上是将节点插入进去
			//调整颜色
			Node *gradfather = parent->_parent;
			
			while (gradfather)
			{
				Node *uncle = NULL;
				if (parent == gradfather->_left)
				{
					uncle = gradfather->_right;
				}
				else
				{
					uncle = gradfather->_left;
				}
				//1.cur为红，p为红，g为黑，u存在且为红则将p,u改为黑，g改为红，然后把g当成cur，
				//继续向上调整。
				if (uncle != NULL && uncle->_col == Red && parent->_col == Red &&  gradfather->_col == Black)
				{
					uncle->_col = Black;
					parent->_col = Black;
					gradfather->_col = Red;
					cur = gradfather;
					parent = cur->_parent;
					if (parent == NULL)
					{
						break;
					}
					gradfather = parent->_parent;
				}
				//2.cur为红，p为红，g为黑，u不存在/u为黑,p为g的左孩子，cur为p的左孩子，
				//则进行右单旋转；相反，p为g的右孩子，cur为p的右孩子，则进行左单旋转	p、g变色--p变黑，g变红
				else if ((uncle == NULL || uncle->_col == Black) && parent->_col == Red && gradfather->_col == Black)
				{
					if (gradfather->_left == parent&&cur == parent->_left)
					{
						RotateR(gradfather);
						parent->_col = Black;
						gradfather->_col = Red;
						break;
					}
					else if (parent == gradfather->_right && cur == parent->_right)
					{
						RotateL(gradfather);
						parent->_col = Black;
						gradfather->_col = Red;
						break;
					}
					else if (parent == gradfather->_left&&cur == parent->_right)
					{
						RotateLR(gradfather);
						cur->_col = Black;
						gradfather->_col = Red;
						break;
					}
					else if (parent == gradfather->_right&&cur == parent->_left)
					{
						RotateRL(gradfather);
						cur->_col = Black;
						gradfather->_col = Red;
						break;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
				//3.cur为红，p为红，g为黑，u不存在/u为黑p为g的左孩子，cur为p的右孩子，则针对p做左单旋转；
				//相反，p为g的右孩子，cur为p的左孩子，则针对p做右单旋转则转换成了情况2
			}
			_root->_col = Black;
		}
		
	}

	void InOrder()
	{
		_InOrder(_root);
	}

	bool IsBalance()
	{
		if (_root&&_root->_col == Red)
		{
			return false;
		}
		//计算一条路径上黑色节点的个数
		int blackNum = 0;
		Node *cur = _root;
		while (cur)
		{
			if (cur->_col == Black)
			{
				blackNum++;
			}
			cur = cur->_left;
		}
		int curNum = 0;
		return _IsBalance(_root, blackNum, curNum);
	}
	
protected:
	bool _IsBalance(Node *root, int blackNum, int curNum)
	{
		if (root == NULL)
		{
			return true;
		}
		if (root->_col == Black)
		{
			curNum++;
		}
		else 
		{
			if (root->_parent&&root->_parent->_col == Red)
			{
				cout << "连续红色:" << root->_key << "," << root->_parent->_key << endl;
				return false;
			}
		}
		if (root->_left == NULL&&root->_right == NULL)
		{
			if (blackNum == curNum)
			{
				curNum = 0;
				return true;
			}
			return false;
		}
		return _IsBalance(root->_left, blackNum, curNum) && _IsBalance(root->_right, blackNum, curNum);
	}
	void _InOrder(Node *root)
	{
		if (root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}
	void RotateR(Node* parent)
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;
		parent->_left = subLR;
		if (subLR)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		Node *ppNode = parent->_parent;

		parent->_parent = subL;

		if (ppNode == NULL)
		{
			_root = subL;
			subL->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subL;
				subL->_parent = ppNode;
			}
			else
			{
				ppNode->_right = subL;
				subL->_parent = ppNode;
			}
		}
	}
	void RotateL(Node* parent)
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}
		subR->_left = parent;
		Node *ppNode = parent->_parent;
		parent->_parent = subR;
		if (ppNode == NULL)
		{
			_root = subR;
			subR->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subR;
				subR->_parent = ppNode;
			}
			else
			{
				ppNode->_right = subR;
				subR->_parent = ppNode;
			}
		}
	}
	void RotateRL(Node* parent)
	{
		RotateR(parent->_right);
		RotateL(parent);
	}
	void RotateLR(Node* parent)
	{
		RotateL(parent->_left);
		RotateR(parent);
	}
private:
	Node *_root;
};

void test()
{
	int arr[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	//int arr[] = { 1, 2, 3, 4 };
	int len = sizeof(arr) / sizeof(arr[0]);
	RBTree<int, int> rbt;
	for (int i = 0; i < len; i++)
	{
		rbt.Insert(arr[i], i);
	}
	rbt.InOrder();
	cout << endl;
	bool ret= rbt.IsBalance();
	if (ret == true)
	{
		cout << "平衡";
	}
	else
	{
		cout << "不平衡";
	}
	cout << endl;
}