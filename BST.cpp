#include "bits\stdc++.h"
using namespace std;

class Book
{
public:
    int id;
    string title;
    string author;

    Book(){}

    Book(const string& name, const int& id, const string& author)
    {
        this->id = id;
        this->author = author;
        this->title = name;
    }

    friend bool operator<(Book& book1, Book& book2)
    {
        return book1.id < book2.id;
    }
    friend bool operator>(Book& book1, Book& book2)
    {
        return book1.id > book2.id;
    }

    friend bool operator<(const Book& book1, const Book& book2)
    {
        return book1.id < book2.id;
    }
    friend bool operator>(const Book& book1, const Book& book2)
    {
        return book1.id > book2.id;
    }

    friend bool operator<(const int& key, const Book& book2)
    {
        return key < book2.id;
    }
    friend bool operator>(const int& key, const Book& book2)
    {
        return key > book2.id;
    }

    friend bool operator==(const Book& book1, const Book& book2)
    {
        return book1.id == book2.id;
    }
    friend bool operator==(const int& key, const Book& book2)
    {
        return key == book2.id;
    }

    friend ostream& operator<<(ostream& out, const Book& book)
    {
        out << "Book Title: " << book.title << "\nID: " << book.id << "\nAuthor: " << book.author << endl;
        return out;
    }
};

struct Node
{
    Book data;
    Node* left;
    Node* right;

    Node(Book value)
    {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

class LibraryManagementSystemBST
{
private:
    // Insert
    Node* insert(Node* node, const Book& data)
    {
        if (node == nullptr)
            return new Node(data);

        if (data < node->data)
            node->left = insert(node->left, data);
        else if (data > node->data)
            node->right = insert(node->right, data);

        return node;
    }

    // Find minimum value
    Node* minValue(Node* node)
    {
        while (node && node->left != nullptr)
            node = node->left;

        return node;
    }

    // Delete node
    Node* deleteNode(Node* node, const int& id)
    {

        if (node == nullptr)
            return node;

        // search for the node
        if (id < node->data)
            node->left = deleteNode(node->left, id);

        else if (id > node->data)
            node->right = deleteNode(node->right, id);

        else
        {
            // Case 1: no child
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }

            // Case 2: one child
            else if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Case 3: two children
            Node* temp = minValue(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.id);
        }

        return node;
    }

    // Search by key
    bool Search(Node* node, const int& key)
    {
        if (node == nullptr)
            return false;

        if (key == node->data)
            return true;

        if (key < node->data)
        {
            return Search(node->left, key);
        }
        else if (key > node->data)
        {
            return Search(node->right, key);
        }

        return false;
    }

    // DFS Traversals
    void inorder(Node* node)
    {
        if (node != nullptr)
        {
            inorder(node->left);
            cout << node->data;
            inorder(node->right);
        }
    }

    void preorder(Node* node)
    {
        if (node != nullptr)
        {
            cout << node->data;
            preorder(node->left);
            preorder(node->right);
        }
    }

    void postorder(Node* node)
    {
        if (node != nullptr)
        {
            postorder(node->left);
            postorder(node->right);
            cout << node->data;
        }
    }

    // BFS
    void bfs(Node* node)
    {

        if (node == nullptr)
            return;

        queue<Node*> q;
        q.push(node);

        while (!q.empty())
        {

            Node* current = q.front();
            q.pop();

            cout << current->data;

            if (current->left)
                q.push(current->left);

            if (current->right)
                q.push(current->right);
        }
    }

    // delete tree in destractor
    void DeleteTree(Node* root)
    {
        if (root == nullptr)
            return;

        DeleteTree(root->left);
        DeleteTree(root->right);
        delete root;
    }

public:
    LibraryManagementSystemBST() {
        root = nullptr;
    }

    // Insert
    void insert(const Book& data)
    {
        root = insert(root, data);
    }

    // Find minimum value
    Book minValue()
    {
        return minValue(root)->data;
    }

    // Delete node
    void deleteNode(const int& key)
    {
        root = deleteNode(root, key);
    }

    // Search by key
    bool Search(const int& key)
    {
        return Search(root, key);
    }

    // DFS Traversals
    void inorder()
    {
        inorder(root);
    }

    void preorder()
    {
        preorder(root);
    }

    void postorder()
    {
        postorder(root);
    }

    // BFS
    void bfs()
    {
        bfs(root);
    }

    ~LibraryManagementSystemBST()
    {
        DeleteTree(root);
    }
private:
    Node* root;
};

int main()
{
    LibraryManagementSystemBST tree;

    tree.insert(Book("Yossef", 1, "Ayman"));
    tree.insert(Book("Yossef", 2, "Ayman"));
    tree.insert(Book("Yossef", 3, "Ayman"));

    tree.insert(Book("Yossef", 4, "Ayman"));

    tree.insert(Book("Yossef", 5, "Ayman"));
    tree.insert(Book("Yossef", 6, "Ayman"));
    tree.insert(Book("Yossef", 7, "Ayman"));

    cout << boolalpha << tree.Search(4) << endl;
    tree.deleteNode(4);
    cout << boolalpha << tree.Search(4) << endl;

    tree.inorder();

    return 0;
}