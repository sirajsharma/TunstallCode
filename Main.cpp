#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <queue>
#include <cmath>

std::vector<std::string> binary_vec;

struct Node
{
	std::string data;
	float prob;
	std::vector<Node*> child;
};


Node* compare (std::vector<Node*> child)
{
	Node* temp = child[0];

	for (int i=1; i<child.size() ; i++)
	{
		if ((child[i]->prob) > (temp->prob))
		{
			temp = child[i];
		}
	}

	return temp;
}


Node* insert_node (std::string data, float prob)
{
	Node* new_node = new Node ();
	new_node->data = data;
	new_node->prob = prob;
	return new_node;
}


std::unordered_map<char, int> total_count_per_char (std::string data)
{
	std::unordered_map<char, int> char_counts;

	for (int i = 0; i < data.length(); i++)
	{
		char_counts[data[i]]++;
	}

	return char_counts;
}


std::unordered_map<char, float> prob_per_char (std::string data, std::unordered_map<char, int> char_counts)
{
	std::unordered_map<char, float> char_prob;
	int string_len = data.length ();

	for (auto pair : char_counts)
	{
		float prob = (float) pair.second / (float) string_len;
		char_prob[pair.first] = prob;
	}

	return char_prob;
}


std::string max_char (std::unordered_map<char, float> probs)
{
	char ch;
	float max = 0.0;
	
	for (auto it = probs.begin (); it != probs.cend (); ++it)
	{
		if (it->second > max)
		{
			ch = it->first;
			max = it->second;
		}
	}

	std::string max_char_strg (1, ch);
	return max_char_strg;
}


std::vector<std::string> chars_in_arr (std::unordered_map<char, float> probs, std::string max_char_strg)
{
	std::vector<std::string> strings_to_encode;

	for (auto pair : probs)
	{
		std::string new_strg (1, pair.first);
		if (new_strg != max_char_strg)
		{
			strings_to_encode.push_back (new_strg);
		}
	}

	return strings_to_encode;
}


void insert_to_vector (std::vector<int> arr, int n)
{
	std::string binary_string = "";

	for (int i = 0; i < n; i++)
	{
		binary_string += std::to_string (arr[i]);
	}

	binary_vec.push_back (binary_string);
}


void generate_binary (int n, std::vector<int> arr, int i)
{
	if (i == n)
	{
		insert_to_vector (arr, n);
		return;
	}

	arr[i] = 0;
	generate_binary (n, arr, i + 1);

	arr[i] = 1;
	generate_binary (n, arr, i + 1);
}


std::map<std::string, std::string> encode (std::vector<std::string> strings_to_encode)
{
	std::map<std::string, std::string> encoded_map;

	for (int i = 0; i < strings_to_encode.size (); i++)
	{
		encoded_map.insert ({strings_to_encode[i], binary_vec[i]});
	}

	return encoded_map;
}


void tunstall_tree (std::string data, int n)
{
	Node* root = new Node ();
	Node* temp1 = root;
	Node* temp2 = root;
	std::unordered_map<char, int> char_counts;
	std::unordered_map<char, float> probs;

	char_counts = total_count_per_char (data);
	probs = prob_per_char (data, char_counts);

	for (auto pair : probs)
	{
		std::string chat_to_string (1, pair.first);
		root->child.push_back(insert_node (chat_to_string, pair.second));
	}

	std::string max_char_strg = max_char (probs);
	std::vector<std::string> strings_to_encode = chars_in_arr (probs, max_char_strg);

	int iterations = floor (((pow(2, n)) - probs.size ()) / (probs.size () - 1));

	temp1 = compare (temp1->child);

	while (iterations > 0)
	{
		std::vector<Node*> childs = temp2->child;

		for (int i = 0; i < childs.size (); i++)
		{
			std::string new_string = "";
			new_string.append (temp1->data);
			new_string.append (root->child[i]->data);
			strings_to_encode.push_back (new_string);

			float prob = temp1->prob * childs[i]->prob;
			temp1->child.push_back (insert_node (new_string, prob));
		}

		temp2 = temp1;
		temp1 = compare (temp1->child);
		iterations--;
	}

	int size_of_string_vec = strings_to_encode.size ();
	std::vector<int> arr(probs.size());
	generate_binary (n, arr, 0);
	
	
	std::map<std::string, std::string> encoded_map = encode (strings_to_encode);

	for (auto pair : encoded_map)
	{
		std::cout << pair.first << "\t\t" << pair.second << std::endl;
	}

	std::cout << "Binary string sequence: ";

	for (auto str : encoded_map)
	{
		std::cout << str.second << "";
	}

	std::cout << std::endl;
	// std::cout << "Highest Probability is: " << temp1->prob << " of " << temp1->data << " character." << std::endl;
}


int main (int argc, char* argv[])
{
	int n;
	std::string data;
	std::cout << "Enter character sequence: ";
	std::cin >> data;
	std::cout << "Enter bit lenght: ";
	std::cin >> n;
	tunstall_tree (data, n);
	return 0;
}