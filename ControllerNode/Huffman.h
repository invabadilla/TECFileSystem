//
// Created by ingrid on 6/22/21.
//

#ifndef CONTROLLERNODE_HUFFMAN_H
#define CONTROLLERNODE_HUFFMAN_H

#include <iostream>
#include <string>
#include <queue>
#include "List.h"
#include <unordered_map>
using namespace std;

// Function to allocate a new tree node
Tree_Node* getNode(char ch, int freq, Tree_Node* left, Tree_Node* right)
{
    Tree_Node* node = new Tree_Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

// Comparison object to be used to order the heap
struct comp
{
    bool operator()(Tree_Node* l, Tree_Node* r)
    {
        // highest priority item has lowest frequency
        return l->freq > r->freq;
    }
};

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(Tree_Node* root, string str,
            unordered_map<char, string> &huffmanCode)
{
    if (root == nullptr)
        return;

    // found a leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

// traverse the Huffman Tree and decode the encoded string
void decode(Tree_Node* root, int &index, string str)
{
    if (root == nullptr) {
        return;
    }

    // found a leaf node
    if (!root->left && !root->right)
    {
        cout << root->ch;
        return;
    }

    index++;

    if (str[index] =='0')
        decode(root->left, index, str);
    else
        decode(root->right, index, str);
}


void preOrden(Tree_Node* node, string *str)
{
    if (node == nullptr){
        return;
    }
    *str += node->ch;
    preOrden(node->left, str);
    preOrden(node->right,str);
}
void inOrden(Tree_Node* node, string *str)
{
    if (node == nullptr){
        return;
    }
    inOrden(node->left, str);
    *str += node->ch;
    inOrden(node->right,str);
}
int search(string arr, int strt, int end, char value)
{
    int i;
    for (i = strt; i <= end; i++)
    {
        if (arr[i] == value)
            return i;
    }
}

Tree_Node* buildTree(string in, string pre, int inStrt, int inEnd)
{
    static int preIndex = 0;

    if (inStrt > inEnd)
        return NULL;

    Tree_Node* tNode = getNode(pre[preIndex++], 0, nullptr, nullptr);

    if (inStrt == inEnd)
        return tNode;

    int inIndex = search(in, inStrt, inEnd, tNode->ch);

    tNode->left = buildTree(in, pre, inStrt, inIndex - 1);
    tNode->right = buildTree(in, pre, inIndex + 1, inEnd);

    return tNode;
}

// Builds Huffman Tree and decode given input text
List<string> buildHuffmanTree(string text)
{
    // count frequency of appearance of each character
    // and store it in a map
    unordered_map<char, int> freq;
    for (char ch: text) {
        freq[ch]++;
    }

    // Create a priority queue to store live nodes of
    // Huffman tree;
    List<Tree_Node *> pq;
    //priority_queue<Tree_Node*, vector<Tree_Node*>, comp> pq;

    // Create a leaf node for each character and add it
    // to the priority queue.
    for (auto pair: freq) {
        pq.insertPriority(getNode(pair.first, pair.second, nullptr, nullptr));
    }
    // do till there is more than one node in the queue

    while (pq.getSize() != 1) {
        // Remove the two nodes of highest priority
        // (lowest frequency) from the queue
        Tree_Node *left = pq.getHead()->getValue();
        pq.delete_first();
        Tree_Node *right = pq.getHead()->getValue();
        pq.delete_first();
        // Create a new internal node with these two nodes
        // as children and with frequency equal to the sum
        // of the two nodes' frequencies. Add the new node
        // to the priority queue.
        int sum = left->freq + right->freq;
        pq.insertPriority(getNode('\0', sum, left, right));
    }

    // root stores pointer to root of Huffman Tree
    Tree_Node *root = pq.getHead()->getValue();

    // traverse the Huffman Tree and store Huffman Codes
    // in a map. Also prints them
    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    // print encoded string
    string str = "";
    for (char ch: text) {
        str += huffmanCode[ch];
    }
    List <string> toSend;


    string pre;
    preOrden(root, &pre);

    string in;
    inOrden(root, &in);

    toSend.insertLast(str);
    toSend.insertLast(pre);
    toSend.insertLast(in);
    return toSend;
}

// Huffman coding algorithm


#endif //CONTROLLERNODE_HUFFMAN_H
}