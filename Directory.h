#pragma once
#include <iostream>
#include <vector>
#include "Node.h"


class Directory : public TreeNode 
{
public:
	vector<TreeNode*> children;

	Directory(const string& Name, const string& Owner, bool ReadOnly, TreeNode* Parent = nullptr) : TreeNode(Name, Owner, ReadOnly, Parent) {}

	~Directory() 
	{
		for (TreeNode* i : children) 
		{
			delete i;
		}
	}

	void addDirectory(TreeNode* newDir) 
	{
		newDir->parent = this;
		this->children.push_back(newDir);
	}

	void displayInformation() const override 
	{
		cout << "Directory Name: " << name << endl;
		cout << "Owner: " << owner << endl;
		if (readOnly == true) 
		{
			cout << "Read-Only: Yes" << endl;
			cout << "Read-Write: No" << endl;
		}
		else 
		{
			cout << "Read-Only : No" << endl;
			cout << "Read-Write: Yes" << endl;
		}
		cout << "Creation Time: " << creationTime << endl;
		for (const TreeNode* i : children) 
		{
			i->displayInformation();
		}
	}
};