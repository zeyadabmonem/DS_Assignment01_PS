#include <iostream>

using namespace std;

//  Book Structure
struct Book {
    int    id;
    string title;
    string author;

    Book() : id(0) {}
    Book(const string& title, int id, const string& author)
        : id(id), title(title), author(author) {
    }

    bool operator<(const Book& o) const { return id < o.id; }
    bool operator>(const Book& o) const { return id > o.id; }
    bool operator==(const Book& o) const { return id == o.id; }

    friend ostream& operator<<(ostream& out, const Book& b) {
        out << "  [ID=" << b.id << "] \"" << b.title << "\" by " << b.author;
        return out;
    }
};

//  AVL Node
struct AVLNode {
    Book     data;
    AVLNode* left = nullptr;
    AVLNode* right = nullptr;
    int      height = 1;

    AVLNode(const Book& b) : data(b) {}
};
//  AVL Class
class AVL {
public:
    AVL() : root(nullptr) {}
    ~AVL() { destroy(root); }

    // Insert a book
    void insert(const Book& b) {
        root = insert(root, b);
    }

    // Delete a book by ID
    void remove(int id) {
        root = remove(root, id);
    }

    // Search by ID
    bool search(int id) const {
        return search(root, id);
    }

    // In-order traversal (sorted by ID)
    void inorder() const {
        inorder(root);
        cout << "\n";
    }

    // Find all books with IDs in [lo, hi]
    void searchByRange(int lo, int hi) const {
        searchByRange(root, lo, hi);
        cout << "\n";
    }

    // Find closest book ID to a given value
    Book closestID(int id) const {
        if (!root) throw runtime_error("Tree is empty");
        AVLNode* best = nullptr;
        AVLNode* cur = root;
        while (cur) {
            if (!best || abs(cur->data.id - id) < abs(best->data.id - id))
                best = cur;
            if (id < cur->data.id) cur = cur->left;
            else if (id > cur->data.id) cur = cur->right;
            else break; // exact match
        }
        return best->data;
    }

    // Tree height
    int height() const { return nodeH(root); }

private:
    AVLNode* root;

    // Height helpers
    int nodeH(AVLNode* n) const { return n ? n->height : 0; }

    void updateH(AVLNode* n) {
        if (n) n->height = 1 + max(nodeH(n->left), nodeH(n->right));
    }

    int balanceFactor(AVLNode* n) const {
        return n ? nodeH(n->left) - nodeH(n->right) : 0;
    }

    // Rotations

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateH(y);
        updateH(x);
        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateH(x);
        updateH(y);
        return y;
    }

    // Balance
    AVLNode* balance(AVLNode* node) {
        updateH(node);
        int bf = balanceFactor(node);

        // Left heavy
        if (bf > 1) {
            if (balanceFactor(node->left) < 0)       // Left-Right case
                node->left = rotateLeft(node->left);
            return rotateRight(node);                 // Right case
        }

        // Right heavy
        if (bf < -1) {
            if (balanceFactor(node->right) > 0)      // Right-Left case
                node->right = rotateRight(node->right);
            return rotateLeft(node);                  // Left case
        }

        return node; // already balanced
    }

    // Core operations
    AVLNode* insert(AVLNode* node, const Book& b) {
        if (!node) return new AVLNode(b);

        if (b < node->data) node->left = insert(node->left, b);
        else if (b > node->data) node->right = insert(node->right, b);
        else return node; // duplicate ID: ignore

        return balance(node);
    }

    AVLNode* minNode(AVLNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    AVLNode* remove(AVLNode* node, int id) {
        if (!node) return nullptr;

        if (id < node->data.id) node->left = remove(node->left, id);
        else if (id > node->data.id) node->right = remove(node->right, id);
        else {
            // Node found
            if (!node->left || !node->right) {
                AVLNode* child = node->left ? node->left : node->right;
                delete node;
                return child;
            }
            // Two children: replace with in-order successor
            AVLNode* successor = minNode(node->right);
            node->data = successor->data;
            node->right = remove(node->right, successor->data.id);
        }

        return balance(node);
    }

    bool search(AVLNode* node, int id) const {
        if (!node) return false;
        if (id == node->data.id) return true;
        return (id < node->data.id)
            ? search(node->left, id)
            : search(node->right, id);
    }

    void inorder(AVLNode* node) const {
        if (!node) return;
        inorder(node->left);
        cout << node->data << "\n";
        inorder(node->right);
    }

    void searchByRange(AVLNode* node, int lo, int hi) const {
        if (!node) return;
        if (node->data.id > lo) searchByRange(node->left, lo, hi);
        if (node->data.id >= lo && node->data.id <= hi) cout << node->data << "\n";
        if (node->data.id < hi) searchByRange(node->right, lo, hi);
    }

    void destroy(AVLNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
};

int main() {
    AVL lib;

    lib.insert(Book("Clean Code", 105, "Robert Martin"));
    lib.insert(Book("The Pragmatic Prog.", 203, "Hunt & Thomas"));
    lib.insert(Book("CLRS Algorithms", 150, "Cormen et al."));
    lib.insert(Book("Design Patterns", 178, "GoF"));
    lib.insert(Book("Refactoring", 120, "Fowler"));
    lib.insert(Book("SICP", 101, "Abelson"));
    lib.insert(Book("Domain-Driven Design", 230, "Eric Evans"));

    cout << "\nIn-order traversal (sorted by ID)\n";
    lib.inorder();

    cout << "\nSearch\n";
    cout << "Search ID=150: " << boolalpha << lib.search(150) << "\n";
    cout << "Search ID=999: " << boolalpha << lib.search(999) << "\n";

    cout << "\nDelete ID=150, then search again\n";
    lib.remove(150);
    cout << "Search ID=150: " << boolalpha << lib.search(150) << "\n";

    cout << "\nIn-order after delete\n";
    lib.inorder();

    cout << "\nRange search [100, 200]\n";
    lib.searchByRange(100, 200);

    cout << "\nClosest ID to 160\n";
    cout << lib.closestID(160) << "\n";

    cout << "\nTree height\n";
    cout << "Height = " << lib.height() << "\n";

    return 0;
}
