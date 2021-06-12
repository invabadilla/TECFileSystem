#ifndef LIST_H
#define LIST_H

#include <iostream>
#include "Node.h"


using namespace std;


template <class T>
//! Class that handles the list logic
class List {

private:

    //! head pointer indicates the first position of the list
    Node<T> *head;

    //! indicates the len of the list
    int size;

public:

    /*!
     * \brief Construct a new List object and initialize the head and the size in zero
     *
     */
    List(){
        size = 0;
        head = NULL;
    };

    /*!
     * \brief destructor
     */
    ~List(){};

    /*!
     * \brief Indicates if the list is empty
     * \return bool True if the list is empty
     */
    bool isEmpty(){
        return head == NULL;
    };

    /*!
     * \brief add a new node to the tail of the list
     * \param Template <T> data that will be stored
     */
    void insertLast(T data_){
        Node<T> *new_node = new Node<T> (data_);
        Node<T> *temp = head;

        if (isEmpty()) {
            head = new_node;
        } else {
            while (temp->getNext() != NULL) {
                temp = temp->getNext();
            }
            temp->setNext(new_node);
        }
        size++;
    };

    /*!
     * \brief add a new node to the head of the list
     * \param Template <T> data that will be stored
     */
    void insertFirst(T data_){
        Node<T> *new_node = new Node<T> (data_);
        Node<T> *temp = head;

        if (isEmpty()) {
            head = new_node;
        } else {
            new_node->setNext(head);
            head = new_node;

            while (temp) {
                temp = temp->getNext();
            }
        }
        size++;
    };

    /*!
     * \brief finds a node given and deletes it
     * \param Template <T> data that will be deleted
     */
    void deleteNode(T data_){
        Node<T> *temp = head;
        Node<T> *temp1 = head->getNext();

        if (head->getValue() == data_) {
            head = temp->getNext();
            size--;
        } else {
            while (temp1) {
                if (temp1->getValue() == data_) {
                    Node<T> *aux_node = temp1;
                    temp->setNext(temp1->getNext());
                    delete aux_node;
                    size--;
                }
                temp = temp->getNext();
                temp1 = temp1->getNext();
            }
        }
    };

    /*!
     * \brief prints the list on console
     */
    void printList(){
        Node<T> *temp = head;
        if (isEmpty()) {
            cout << "La Lista está vacía " << endl;
        } else {
            while (temp) {
                temp->printNode();
                if (!temp->getNext()) cout << "NULL";
                temp = temp->getNext();
            }
        }
        cout << endl << endl;
    };

    /*!
     * \brief find the node pointer with the given index
     * \param int with the index of the node
     * \return Node<T>* node pointer
     */
    Node<T>* find(int pos){
        Node<T> *current = head;
        int count = 0;
        while (current != NULL){
            if (count == pos){
                return current;
            } else {
                count++;
                current = current->getNext();
            }
        }
    };

    /*!
     * \brief add a new node to the tail of the list
     * \param Template <T> data that will be stored
     * \return bool indicating the existance of the value
     */
    bool findValue(T value_){
        Node<T> *current = head;
        bool result = false;
        while (current != NULL){
            if (value_ == current->getValue()){
                result = true;
                return result;
            } else {
                result = false;
                current = current->getNext();
            }
        }
    };

    /*!
     * \brief indicates the len of the list
     * \return int with the len of the list
     */
    int getSize(){
        return size;
    };

    /*!
     * \brief deletes all the list
     */
    void delete_all(){
        Node<T>* current = head;
        while (current) {
            Node<T>* temp = current;
            current = current->getNext();
            delete temp;
        }
        head = 0;
        size = 0;
    };
};



#endif //LIST_H
