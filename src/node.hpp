#include <cmath>

namespace node {
	class Node {
		public:
			int x;
			int y;
			int id;
			Node();
			Node(int arg_x, int arg_y);
			Node(int arg_x, int arg_y, int arg_id);
			friend float abs(Node n);
			friend float distance(Node n1, Node n2);
			Node operator - (Node node);
	};
}
