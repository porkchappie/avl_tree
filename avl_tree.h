#pragma once
#include <cstdio>
#include <vector>

struct avl_tree {
	inline static constexpr size_t invalid = -1;
	struct node {
		int data;
		size_t parent, left, right;
		int height;
	};

	std::vector<node> nodes;
	size_t head{ invalid }, free{ invalid };

	inline size_t add(int data) {
		auto i = free;
		if (i == invalid) {
			i = nodes.size();
			auto& node = nodes.emplace_back();
			node.data = data; // TODO
		}
		else {
			nodes[i].data = data; // TODO
			free = nodes[i].parent;
		}
		auto& node = nodes[i];
		node.left = node.right = invalid;
		node.height = 0;
		if (head == invalid) {
			head = i;
			node.parent = invalid;
			return i;
		}
		// TODO int compare(lhs, rhs) -1, 0, 1
		auto root = head;
		while (true) {
			if (nodes[root].data > data) {
				auto& left = nodes[root].left;
				if (left == invalid) {
					left = i;
					node.parent = root;
					break;
				}
				root = left;
			}
			else if (nodes[root].data < data) {
				auto& right = nodes[root].right;
				if (right == invalid) {
					right = i;
					node.parent = root;
					break;
				}
				root = right;
			}
			else {
				// TODO equals
				return i;
			}
		}

		auto j = i;
		do {
			auto& parent = nodes[root];
			if (parent.left == j) {
				if (--parent.height == 0) break;
				if (parent.height == -2) {
					auto& child = nodes[j];
					if (child.height == -1) { // ll
						parent.height = child.height = 0;
						if ((child.parent = parent.parent) == invalid) head = j;
						else nodes[child.parent].left = j;
						if ((parent.left = child.right) != invalid) nodes[parent.left].parent = root;
						nodes[child.right = root].parent = j;
					}
					else { // lr
						const auto k = child.right;
						auto& grandchild = nodes[k];
						parent.height = 1;
						child.height = grandchild.height = 0;
						if ((grandchild.parent = parent.parent) == invalid) head = k;
						else nodes[grandchild.parent].right = k;
						if ((child.right = grandchild.left) != invalid) nodes[child.right].parent = j;
						nodes[grandchild.left = j].parent = k;
						nodes[grandchild.right = root].parent = k;
						parent.left = invalid;
					}
					break;
				}
			}
			else {
				if (++parent.height == 0) break;
				if (parent.height == 2) {
					auto& child = nodes[j];
					if (child.height == -1) { // rl
						const auto k = child.left;
						auto& grandchild = nodes[k];
						parent.height = -1;
						child.height = grandchild.height = 0;
						if ((grandchild.parent = parent.parent) == invalid) head = k;
						else nodes[grandchild.parent].left = k;
						if ((child.left = grandchild.right) != invalid) nodes[child.left].parent = j;
						nodes[grandchild.left = root].parent = k;
						nodes[grandchild.right = j].parent = k;
						parent.right = invalid;
					}
					else { // +1 // rr
						parent.height = child.height = 0;
						if ((child.parent = parent.parent) == invalid) head = j;
						else nodes[child.parent].right = j;
						if ((parent.right = child.left) != invalid) nodes[parent.right].parent = root;
						nodes[child.left = root].parent = j;
					}
					break;
				}
			}
			j = root;
		} while ((root = nodes[root].parent) != invalid);


		return i;
	}

	void padding(char ch, int n) {
		int i;

		for (i = 0; i < n; i++)
			putchar(ch);
	}

	void structure(size_t root, int level = 0) {
		if (root == invalid) {
			padding('\t', level);
			puts("~ X");
		}
		else {
			structure(nodes[root].right, level + 1);
			padding('\t', level);
			printf("%d) %d (%d) p:%d\n", root, nodes[root].height, nodes[root].data, nodes[root].parent);
			structure(nodes[root].left, level + 1);
		}
	}

	void print() {
		printf("---------------- AVL TREE START ----------------\n\n");
		structure(head);
		printf("\n---------------- AVL TREE END ------------------\n\n");
	}

};
