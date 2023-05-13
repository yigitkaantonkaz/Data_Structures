#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;;
template <class T>
class node {
public:
    T full_name;
    T number;
    T city;
    node * left;
    node * right;
    int height;
    
    node(){
        full_name = "";
        number = "";
        city = "";
        left = nullptr;
        right = nullptr;
        height = 1;
    }
    
    node(T line_text){
        vector<string> temp;
        string s;
        stringstream ss(line_text);
        while(getline (ss,s,' ')){
            temp.push_back(s);
        }
        full_name = temp[0] + " " + temp[1];
        number = temp[2];
        city = temp[3];
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};
template <typename obj>
void insert_tree(node<obj> * &root, obj line){
    node<obj> * temp = new node<obj>(line);
    
    if(!root){
        root = temp;
        return;
    }
    
    node<obj>* prev = nullptr;
    node<obj>* next = root;
    
    while(next){
        if(next->full_name < line){ // means alphabetically bigger
            prev = next;
            next = next->right;
        }
        else if(next->full_name > line){ //alphabetically smaller
            prev = next;
            next = next->left;
        }
    }
    if(prev->full_name > line){prev->left = temp;}
    else{prev->right = temp;}
}

int return_height(node<string> * m){
    if(!m) return 0;
    return m->height;
}
template<typename K2>
node<K2> * change_to_right(node<K2> * m);
template<typename K1>
node<K1> * change_to_left(node<K1> * m);

template <typename K3>
K3 return_full_name(K3 line){
    vector<K3> v;
    K3 s;
    stringstream ss(line);
    while(getline (ss,s,' ')){
        v.push_back(s);
    }
    s = v[0] + " " + v[1];
    return s;
}

string return_name(string s){
    stringstream ss(s);
    string k;
    while(getline(ss,k,' ')){
        return k;
    }
    return s;
}

template <typename obj>
node<obj> * insert_avl(node<obj> * root, obj line){
    node<obj> * temp = new node(line);
    
    if(!root){
        return temp;
    }
    
    if(root->full_name > return_full_name(line)){
        root->left = insert_avl<string>(root->left, line);
    }
    else if(root->full_name < return_full_name(line)){
        root->right = insert_avl<string>(root->right, line);
    }
    else {
        cout << "The given contact full name already exists in the database." << endl;
        return root;
    }
    
    root->height = max(return_height(root->left),return_height(root->right)) + 1;
    
    int counter;
    
    if(!root){
        counter = 0;
    }
    else{
        counter = return_height(root->left) - return_height(root->right);
    }
    
    if(counter > 1 && root->left->full_name > return_full_name(line)) return change_to_right(root);
    if(counter < -1 && root->right->full_name < return_full_name(line)) return change_to_left(root);
    if(counter < -1 && root->right->full_name > return_full_name(line)){
        root->right = change_to_right(root->right);
        return change_to_left(root);
    }
    if(counter > 1 && root->left->full_name < return_full_name(line)){
        root->left = change_to_left(root->left);
        return change_to_right(root);
    }
    
    return root;
}
template<typename K2>
node<K2> * change_to_right(node<K2> * m){
    node<K2> * n1 = m->left;
    node<K2> * n2 = n1->right;
    
    //moving m to down
    n1->right = m;
    m->left = n2;
    
    m->height = max(return_height(m->left),return_height(m->right)) + 1;
    n1->height = max(return_height(n1->left),return_height(n1->right)) + 1;
    
    return n1;
}
template<typename K1>
node<K1> * change_to_left(node<K1> * m){
    node<K1> * n1 = m->right;
    node<K1> * n2 = n1->left;
    
    n1->left = m;
    m->right = n2;
    
    m->height = max(return_height(m->left), return_height(m->right)) + 1;
    n1->height = max(return_height(n1->left), return_height(n1->right)) + 1;
    
    return n1;
}

template <typename T2>
void add_contact(T2 full_name, T2 number, T2 city, node<T2> * &BST, node<T2> * &AVL){
    T2 combined = full_name + " " + number + " " + city;
    
    cout << "Adding an item to the phonebook BST . . ." << endl;
    cout << "====================================" << endl;
    auto start1 = std::chrono::high_resolution_clock::now();
    insert_tree<T2>(BST, combined);
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1).count();
    cout << "Contact has been added successfully to the BST" << endl;
    cout << endl;
    cout << "Adding an item to the phonebook (AVL) . . ." << endl;
    cout << "====================================" << endl;
    auto start2 = std::chrono::high_resolution_clock::now();
    AVL = insert_avl<T2>(AVL, combined);
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count();
    cout << "Contact has been added successfully to the AVL Tree" << endl;
    cout << endl;
    cout << "Adding a contact to the Binary Tree took " << duration1 << " nanoseconds. . ." << endl;
    cout << "Adding a contact to the AVL Tree took " << duration2 << " nanoseconds. . ." << endl;
}
template <typename T1>
void search_contact(node<T1> * root, T1 full_name){
    if(root) for (auto & c: root->full_name) c = toupper(c);
    for (auto & k: full_name) k = toupper(k);
    if(root){
        T1 str = return_name(root->full_name);
        size_t f = str.find(full_name);
        
        if(root->full_name == full_name){
            T1 ss;
            ss = root->full_name + " " + root->number + " " + root->city;
            cout << ss << endl;
        }
        else if(f != T1::npos){
            cout << root->full_name << " "<< root->number << " "<< root->city << endl;
        }
        search_contact(root->left, full_name);
        search_contact(root->right, full_name);
    }
}
template <typename T3>
node<T3> * delete_contact(node<T3> * root, T3 full_name, int &check){
    
    if(!root){
        return root;
    }
    
    if(full_name < root->full_name){
        root->left = delete_contact(root->left, full_name, check);
        return root;
    }
    else if(full_name > root->full_name){
        root->right = delete_contact(root->right, full_name, check);
        return root;
    }
    
    if(!root->left){
        node<T3> * t = root->right;
        delete root;
        check = 1;
        return t;
    }
    else if(!root->right){
        node<T3> * t = root->left;
        delete root;
        check = 1;
        return t;
    }
    else{
        node<T3> * p = root;
        node<T3> * k = root->right;
        while(k->left != NULL){
            p = k;
            k = k->left;
        }
        
        if(p != root){
            p->left = k->right;
        }
        else{
            p->right = k->right;
        }
        
        root->full_name = k->full_name;
        root->number = k->number;
        root->city = k->city;
        
        delete k;
        check = 1;
        return root;
    }
}

int counter_return(node<string> * root){
    if(!root){
        return 0;
    }
    return return_height(root->left) - return_height(root->right);
}
template <typename N>
node<N> * delete_avl(node<N> * root, N full_name, int &check){
    
    if(!root){
        return root;
    }
    
    if(full_name < root->full_name){
        root->left = delete_avl(root->left, full_name, check);
    }
    else if (full_name > root->full_name){
        root->right = delete_avl(root->right, full_name, check);
    }
    else{
        if((root->left == NULL) || (root->right == NULL)){
            node<N> * t;
            if(root->left) t = root->left;
            else t = root->right;
            
            if(!t){
                t = root;
                root = NULL;
            }
            else{
                root->full_name = t->full_name;
                root->number = t->number;
                root->city = t->city;
                free(t);
                check = 1;
            }
        }
        else {
            node<N> * t_ptr;
            node<N> * c_ptr = root->right;
            
            while(c_ptr->left){
                c_ptr = c_ptr->left;
            }
            t_ptr = c_ptr;
            root->full_name = t_ptr->full_name;
            root->number = t_ptr->number;
            root->city = t_ptr->city;
            
            root->right = delete_avl(root->right, t_ptr->full_name, check);
        }
    }
    
    if(!root){
        return root;
    }
    
    root->height = max(return_height(root->left), return_height(root->right)) + 1;
    
    int counter = counter_return(root);
        
    if(counter > 1 && counter_return(root->left) >= 0) return change_to_right(root);
    if(counter < -1 && counter_return(root->right) <= 0) return change_to_left(root);
    if(counter < -1 && counter_return(root->right) > 0){
        root->right = change_to_right(root->right);
        return change_to_left(root);
    }
    if(counter > 1 && counter_return(root->left) < 0){
        root->left = change_to_left(root->left);
        return change_to_right(root);
    }
    
    return root;
}

template <typename S1>
void inorder(node<S1> * root, ofstream& f){
    if(!root) return;
    inorder(root->left, f);
    for (auto & c: root->full_name) c = toupper(c);
    f << root->full_name << " " << root->number << " " << root->city << endl;
    inorder(root->right, f);
}
template <typename S2>
void preorder(node<S2>* root, ofstream& f){
    if(!root) return;
    for (auto & c: root->full_name) c = toupper(c);
    f << root->full_name << " " << root->number << " " << root->city << endl;
    preorder(root->left,f);
    preorder(root->right,f);
}
template <typename B>
void print_(const B& pre, node<B>* root, bool b, ofstream & f)
{
    if(!root) return;
    for (auto & c: root->full_name) c = toupper(c);
    f << pre;
    if(b) {
        f << "|--";
    }
    else {
        f <<"|__";
    }
    f << root->full_name << endl;
    
    print_(pre + (b ? "|   " : "    "), root->left, true, f);
    print_(pre + (b ? "|   " : "    "), root->right, false, f);
    
}

int height_function(node<string>* root)
{
    if (!root) return 0;
    return max(height_function(root->left), height_function(root->right)) + 1;
}
bool balanced(node<string> * root, string type){
    int l, r;
    if(!root) return 1;
    l = height_function(root->left);
    r = height_function(root->right);
    
    if (abs(r- l) <= 1)
        cout << "The tree is balanced" << endl;
    else {
        cout << "The tree is not balanced" << endl;
    }
    cout << "The heights of " << type << " are for left: " << l << " and right: " << r << endl;
    
    return 0;
}

int main() {
    string input_line;
    string input_file;
    node<string> * root_BST = nullptr;
    node<string> * root_AVL = nullptr;
    
    cout << "Please enter the contact file name: ";
    cin >> input_file;
    ifstream File_txt;
    File_txt.open(input_file);
    auto start = std::chrono::high_resolution_clock::now();
    while(getline (File_txt, input_line)){
        insert_tree<string>(root_BST,input_line);
        root_AVL = insert_avl<string>(root_AVL, input_line);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    cout << "Loading the phonebook into a BST." << endl;
    cout << "Phonebook creation in BST took " << dur << " milliseconds. . ." << endl;
    balanced(root_BST, "BST");
    cout << "Loading the phonebook into an AVL" << endl;
    cout << "Phonebook creation in AVL took " << dur << " milliseconds. . ." << endl;
    balanced(root_AVL, "AVL");
    
    int selection = 0;
    while(selection != 6){
        cout << endl;
        cout << "Choose which action to perform from 1 to 6:" << endl;
        cout << "1 - Search a phonebook contact" << endl;
        cout << "2 - Adding a phonebook contact"<< endl;
        cout << "3 - Deleting a phonebook contact"<< endl;
        cout << "4 - Print the phonebook to a file(inorder)"<< endl;
        cout << "    Print the phonebook to a file(preorder)" << endl;
        cout << "5 - Draw the phonebook as a Tree to a file" << endl;
        cout << "6 - Press 6 to exit" << endl;
        cin >> selection;
        cout << endl;
        switch (selection) {
            case 1:{
                cout << "Search for a contact: ";
                string n1;
                getline(cin >> ws, n1);
                cout << endl;
                cout << "Searching an item in the phonebook (BST) . . ." << endl << endl;
                cout << "Phonebook: Searching for: (" << n1 << ")" << endl;
                cout << "====================================" << endl;
                auto start_time1 = std::chrono::high_resolution_clock::now();
                search_contact<string>(root_BST, n1);
                cout << endl;
                auto end_time1 = std::chrono::high_resolution_clock::now();
                auto duration_1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
                cout <<  "Searching an item in the phonebook (AVL) . . ." << endl << endl;
                cout << "Phonebook: Searching for: (" << n1 << ")" << endl;
                cout << "====================================" << endl;
                auto start_time2 = std::chrono::high_resolution_clock::now();
                search_contact<string>(root_AVL, n1);
                cout << endl;
                auto end_time2 = std::chrono::high_resolution_clock::now();
                auto duration_2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();
                for (auto & s: n1) s = toupper(s);
                cout << "The search in BST took " << duration_1 << " nanoseconds. . ." << endl;
                cout << "The search in AVL took " << duration_2 << " nanoseconds. . ." << endl;
                break;}
            case 2:{
                string s1, S1, S2, s2, s3;
                cout << "Enter the information of the contact to be added:" << endl;
                cout << "Name: ";
                cin >> s1 >> S1;
                S2 = s1 + " " + S1;
                cout << endl << "Tel: ";
                cin >> s2;
                cout << endl << "City: ";
                cin >> s3;
                cout << endl;
                add_contact(S2, s2, s3, root_BST, root_AVL);
                break;}
            case 3:{
                cout << "Deleting an item from the phonebook . . ." << endl;
                cout << "Enter the fullname of the contact to be deleted: ";
                string d1 , d2, d3;
                int check1 = 0, check2 = 0;
                cin >> d1 >> d2;
                d3 = d1 + " " + d2;
                
                auto start_time3 = std::chrono::high_resolution_clock::now();
                root_BST = delete_contact(root_BST, d3, check1);
                auto end_time3 = std::chrono::high_resolution_clock::now();
                auto duration_3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time3 - start_time3).count();
                auto start_time4 = std::chrono::high_resolution_clock::now();
                root_AVL = delete_avl(root_AVL, d3, check2);
                auto end_time4 = std::chrono::high_resolution_clock::now();
                auto duration_4 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time4 - start_time4).count();
                if(check1 == 1 || check2 == 1){
                    cout << endl;
                    cout << "Deleted succcessfully. . ." << endl << endl;
                    cout << "Deletion from the Binary Tree took " << duration_3 <<" nanoseconds. . ." <<endl;
                    cout << "Deletion from AVL Tree took " << duration_4 << " nanoseconds. . ." << endl;
                }
                else{
                    cout << "Not found" << endl;
                }
                break;}
            case 4:{
                ofstream File1("phonebookInOrder.txt");
                ofstream File2("phonebookPreOrder.txt");
                auto st1 = std::chrono::high_resolution_clock::now();
                inorder(root_BST, File1);
                auto en1 = std::chrono::high_resolution_clock::now();
                File1 << endl;
                auto dd1 = std::chrono::duration_cast<std::chrono::nanoseconds>(en1 - st1).count();
                auto st2 = std::chrono::high_resolution_clock::now();
                inorder(root_AVL, File1);
                auto en2 = std::chrono::high_resolution_clock::now();
                auto dd2 = std::chrono::duration_cast<std::chrono::nanoseconds>(en2 - st2).count();
                auto st3 = std::chrono::high_resolution_clock::now();
                preorder(root_BST, File2);
                auto en3 = std::chrono::high_resolution_clock::now();
                File2 << endl;
                auto dd3 = std::chrono::duration_cast<std::chrono::nanoseconds>(en3 - st3).count();
                auto st4 = std::chrono::high_resolution_clock::now();
                preorder(root_AVL, File2);
                auto en4 = std::chrono::high_resolution_clock::now();
                auto dd4 = std::chrono::duration_cast<std::chrono::nanoseconds>(en4 - st4).count();
                cout << "Printing in order to file from the Binary Tree took " << dd1 << " nanoseconds. . ." << endl;
                cout << "Printing in order to file from AVL Tree took " << dd2 << " nanoseconds. . ." << endl;
                cout << "Printing pre order to file from the Binary Tree took " << dd3 << " nanoseconds. . ." << endl;
                cout << "Printing pre order to file from AVL Tree took " << dd4 << " nanoseconds. . ." << endl;
                cout << endl;
                File1.close();
                File2.close();
                break;}
            case 5:{
                ofstream F1("phonebookTreeBST.txt");
                ofstream F2("phonebookTreeAVL.txt");
                string v = "";
                bool b = false;
                auto stt3 = std::chrono::high_resolution_clock::now();
                print_(v, root_BST, b, F1);
                auto enn3 = std::chrono::high_resolution_clock::now();
                auto ddd3 = std::chrono::duration_cast<std::chrono::nanoseconds>(enn3 - stt3).count();
                auto stt4 = std::chrono::high_resolution_clock::now();
                print_<string>("", root_AVL, b, F2);
                auto enn4 = std::chrono::high_resolution_clock::now();
                auto ddd4 = std::chrono::duration_cast<std::chrono::nanoseconds>(enn4 - stt4).count();
                cout <<"Drawing tree to file from the Binary Tree took " << ddd3 << " nanoseconds. . ." << endl;
                cout << "Drawing tree to file from AVL Tree took " << ddd4 << " nanoseconds. . ." << endl;
                F1.close();
                F2.close();
                break;}
            default:{
                break;}
        }
    }
    
    File_txt.close();
    return 0;
}
