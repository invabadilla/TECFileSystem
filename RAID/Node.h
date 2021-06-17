#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

template <class T>
//! Class that handles the Node logic
class Node {
private:

    //! pointer that indicates the next position
    Node *next;

    //! contains the data of the node
    T value;

public:

    /*!
     * \brief Construct a new Node object and initialize the value and the next pointer in NULL
     */
    Node(){
        value = NULL;
        next = NULL;
    };

    /*!
     * \brief Construct a new Node object and initialize the next pointer in NULL and receives the data
     *
     * \param value_ data that will be stored in the node
     */
    Node(T value_){
        value = value_;
        next = NULL;
    };

    /*!
     * \brief destructor
     */
    ~Node(){};

    /*!
     * \brief get the next pointer
     *
     * \return Node pointer
     */
    Node *getNext() const{
        return next;
    };

    /*!
     * \brief sets the new pointer
     *
     * \param next_ Node new pointer
     */
    void setNext(Node *next_){
        Node::next = next_;
    };

    /*!
     * \brief gets the value of the node
     *
     * \return T value
     */
    T getValue() const{
        return value;
    };

    /*!
     * \brief sets the new data
     *
     * \param value_ new data
     */
    void setValue(T value_){
        Node::value = value_;
    };

    /*!
     * \brief prints the node value on console
     */
    void printNode(){
        //cout << "Node-> " << "Dato: " << dato << " Direcion: " << this << " Siguiente: " << next << endl;
        cout << value << "-> ";
    };

    /*!
     * \brief delete all the nodes
     */
    void delete_all(){
        if (next)
            next->delete_all();
        delete this;
    };
};


#endif //NODE_H
