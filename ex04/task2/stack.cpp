
#include <iostream>

template<typename StackType>
class Stack {

    struct Node  {
        StackType data;
        Node* next;
    };

    Node* head;
    int length;
    int count;

public:
    Stack(int length) : head(nullptr), length(length), count(0){ }

    ~Stack(){
        Node* tmp = head;

        while(tmp){
            Node* next = tmp->next;
            delete tmp;
            tmp = next;
        }

        head = NULL;
    }

    void push(StackType element){
        if(isFull()){
            std::cout << "Stack if full\n";
            return;
        }

        Node* newTop = new Node();
        if(isEmpty()){
            newTop->data = element;
            newTop->next = nullptr;
            head = newTop;
            count++;
        } else{
            newTop->data = element;
            newTop->next = head;
            head = newTop;
            count++;
        }
    }

    bool isEmpty() const{
        return head == nullptr;
    }

    bool isFull() const{
        return length == count;
    }

    StackType pop(){
        if(isEmpty()){
            std::cout << "Stack is empty\n";
            return NULL;
        }
        Node* toPop = head;
        head = head->next;
        count--;
        StackType ret = toPop->data;
        delete(toPop);
        return ret;
    }

    void print() const {
        Node* tmp;
        tmp = head;
        while(tmp != nullptr){
            std::cout << tmp->data << "\n";
            tmp = tmp->next;
        }
    }

    int size() const{
        return count;
    }

    StackType top() const{
        return head->data;
    }
};

int main(){
    Stack<int> stackTestInt(3);
    stackTestInt.push(1);
    stackTestInt.push(2);
    stackTestInt.push(3);
    stackTestInt.push(4);

    stackTestInt.print();

    Stack<double> stackTestDouble(3);
    stackTestDouble.push(1.5);
    stackTestDouble.push(2.668);
    stackTestDouble.push(3.25);
    double test = stackTestDouble.pop();
    stackTestDouble.push(4.455);

    std::cout << test << "\n";

    stackTestDouble.print();
}