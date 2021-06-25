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

static int preIndex = 0; //Number to use in the method BuildTree

/**
 * Function to allocate a new tree node
 * @param ch Caracter to insert
 * @param freq Frequence of the character
 * @param left node left of the node
 * @param right node right of the node
 * @return the node
 */
Tree_Node* getNode(string ch, int freq, Tree_Node* left, Tree_Node* right)
{
    Tree_Node* node = new Tree_Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

/**
 * Comparison object to be used to order the heap
 */
struct comp
{
    bool operator()(Tree_Node* l, Tree_Node* r)
    {
        // highest priority item has lowest frequency
        return l->freq > r->freq;
    }
};

/**
 * traverse the Huffman Tree and store Huffman Codes in a map.
 * @param root of the Huffman tree
 * @param str Code of the huffman code for each node in the tree
 * @param huffmanCode Dictionary
 */
void encode(Tree_Node* root, string str,
            unordered_map<string, string> &huffmanCode)
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

/**
 * traverse the Huffman Tree and decode the encoded string
 * @param root of the huffman tree
 * @param index to go through the text
 * @param str String to decode
 * @param strDecode Result to decode method
 */
void decode(Tree_Node* root, int &index, string str, string*strDecode)
{
    if (root == nullptr) {
        return;
    }

    // found a leaf node
    if (!root->left && !root->right)
    {
        *strDecode += root->ch;
        return;
    }

    index++;

    if (str[index] =='0')
        decode(root->left, index, str, strDecode);
    else
        decode(root->right, index, str, strDecode);
}

/**
 * Determinate the preorden expresion of the huffman tree
 * @param node root of the tree
 * @param str String result
 */
void preOrden(Tree_Node* node, string *str)
{
    if (node == nullptr){
        return;
    }
    *str += node->ch;
    *str += "$";
    preOrden(node->left, str);
    preOrden(node->right,str);
}

/**
 * Determinate the inorden expresion of the huffman tree
 * @param node root of the tree
 * @param str String result
 */
void inOrden(Tree_Node* node, string *str)
{
    if (node == nullptr){
        return;
    }
    inOrden(node->left, str);
    *str += node->ch;
    *str += "$";
    inOrden(node->right,str);
}

/**
 * Determines the position of a value from the preorder list within the inorder list
 * @param arr Inorden list
 * @param strt start of the list
 * @param end end of the list
 * @param value to find
 * @return position
 */
int search(List<string>  arr, int strt, int end, string value)
{
    int i;
    for (i = strt; i <= end; i++)
    {
        if (arr.find(i)->getValue() == value)
            return i;
    }
}

/**
 * Rebuild the Huffman tree with the preorden and inorden expression
 * @param in inorden expression
 * @param pre preorden expression
 * @param inStrt start position
 * @param inEnd end position
 * @return root to the rebuild tree
 */
Tree_Node* buildTree(List<string> in, List<string> pre, int inStrt, int inEnd)
{
    if (inStrt > inEnd)
        return NULL;

    Tree_Node* tNode = getNode(pre.find(preIndex++)->getValue(), 0, nullptr, nullptr);

    if (inStrt == inEnd)
        return tNode;

    int inIndex = search(in, inStrt, inEnd, tNode->ch);

    tNode->left = buildTree(in, pre, inStrt, inIndex - 1);
    tNode->right = buildTree(in, pre, inIndex + 1, inEnd);

    return tNode;
}

/**
 * Builds Huffman Tree and decode given input text
 * @param text to build a tree
 * @return the expression of the preorden and inorden
 */
List<string> buildHuffmanTree(string text)
{
    unordered_map<string, int> freq;
    for (int i = 0; i< text.size(); i++) {
        string ch="";
        ch.push_back(text[i]);
        freq[ch]++;
    }
    List<Tree_Node *> pq;

    for (auto pair: freq) {
        pq.insertPriority(getNode(pair.first, pair.second, nullptr, nullptr));
    }
    int chr = 0;
    while (pq.getSize() != 1) {
        Tree_Node *left = pq.getHead()->getValue();
        pq.delete_first();
        Tree_Node *right = pq.getHead()->getValue();
        pq.delete_first();
        int sum = left->freq + right->freq;
        string char_ = "N"+to_string(chr);
        pq.insertPriority(getNode(char_, sum, left, right));
        chr++;
    }

    Tree_Node *root = pq.getHead()->getValue();

    unordered_map<string, string> huffmanCode;
    encode(root, "", huffmanCode);

    string str = "";
    for (int i = 0; i< text.size(); i++) {
        string ch="";
        ch.push_back(text[i]);
        str += huffmanCode[ch];
    }
    List <string>toSend;

    string pre;
    preOrden(root, &pre);

    string in;
    inOrden(root, &in);

    toSend.insertLast(str);
    toSend.insertLast(pre);
    toSend.insertLast(in);

    return toSend;
}

#endif //CONTROLLERNODE_HUFFMAN_H
