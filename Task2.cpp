#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// Student Record Structure
struct Student {
    int studentID;
    string name;
    float GPA;
    Student* left;
    Student* right;

    Student(int id, string n, float g)
        : studentID(id), name(n), GPA(g), left(nullptr), right(nullptr) {}
};

class StudentBST {
private:
    Student* root;

    // Insert Record
    Student* insert(Student* node, int id, string name, float gpa) {
        if (!node) {
            return new Student(id, name, gpa);
        }
        if (id < node->studentID) {
            node->left = insert(node->left, id, name, gpa);
        } else if (id > node->studentID) {
            node->right = insert(node->right, id, name, gpa);
        } else {
            cout << "Warning: Student ID " << id << " already exists.\n";
        }
        return node;
    }

    // Find Minimum Value Node
    Student* findMin(Student* node) {
        while (node && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // Delete Record
    Student* remove(Student* node, int id) {
        if (!node) return nullptr;

        if (id < node->studentID) {
            node->left = remove(node->left, id);
        } else if (id > node->studentID) {
            node->right = remove(node->right, id);
        } else {
            // no child or single child
            if (!node->left) {
                Student* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Student* temp = node->left;
                delete node;
                return temp;
            }

            //  two children - copy inorder successor, then delete it
            Student* temp = findMin(node->right);
            node->studentID = temp->studentID;
            node->name = temp->name;
            node->GPA = temp->GPA;
            node->right = remove(node->right, temp->studentID);
        }
        return node;
    }

    // Search Record
    Student* search(Student* node, int id) {
        if (!node || node->studentID == id) return node;
        if (id < node->studentID) return search(node->left, id);
        return search(node->right, id);
    }

    // Inorder Traversal
    void inorder(Student* node) {
        if (!node) return;
        inorder(node->left);
        cout << "ID: " << node->studentID
             << " | Name: " << node->name
             << " | GPA: " << node->GPA << "\n";
        inorder(node->right);
    }

    // Count Students
    int countNodes(Student* node) {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    // Track High/Low GPA
    void findGPAExtremes(Student* node, Student*& minStudent, Student*& maxStudent) {
        if (!node) return;

        if (!minStudent || node->GPA < minStudent->GPA) minStudent = node;
        if (!maxStudent || node->GPA > maxStudent->GPA) maxStudent = node;

        findGPAExtremes(node->left, minStudent, maxStudent);
        findGPAExtremes(node->right, minStudent, maxStudent);
    }

    // GPA Threshold Filter
    void filterByGPA(Student* node, float threshold, int& found) {
        if (!node) return;
        filterByGPA(node->left, threshold, found);
        if (node->GPA >= threshold) {
            cout << "ID: " << node->studentID
                 << " | Name: " << node->name
                 << " | GPA: " << node->GPA << "\n";
            found++;
        }
        filterByGPA(node->right, threshold, found);
    }

    // Free all nodes 
    void destroy(Student* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    StudentBST() : root(nullptr) {}
    ~StudentBST() { destroy(root); }

    // Prevent shallow copies 
    StudentBST(const StudentBST&) = delete;
    StudentBST& operator=(const StudentBST&) = delete;

    void Insert(int id, string name, float gpa) {
        root = insert(root, id, name, gpa);
    }

    void Search(int id) {
        Student* res = search(root, id);
        if (res) {
            cout << "\n[Found Student] ID: " << res->studentID
                 << " | Name: " << res->name
                 << " | GPA: " << res->GPA << "\n";
        } else {
            cout << "\n[Search Error] Student ID " << id << " not found.\n";
        }
    }

    void Delete(int id) {
        if (search(root, id)) {
            root = remove(root, id);
            cout << "Student ID " << id << " successfully deleted.\n";
        } else {
            cout << "Cannot delete: Student ID " << id << " not found.\n";
        }
    }

    void DisplayInorder() {
        cout << "\n Student Records (Sorted by ID)\n";
        if (!root) {
            cout << "No records available.\n";
            return;
        }
        inorder(root);
    }

    void DisplayTotalCount() {
        cout << "\nTotal Number of Students: " << countNodes(root) << "\n";
    }

    void DisplayGPAExtremes() {
        if (!root) {
            cout << "No records available to compute GPA extremes.\n";
            return;
        }
        Student* minGpaStudent = nullptr;
        Student* maxGpaStudent = nullptr;
        findGPAExtremes(root, minGpaStudent, maxGpaStudent);

        cout << "\n GPA Analytics\n";
        cout << "Highest GPA: " << maxGpaStudent->GPA << " (Name: " << maxGpaStudent->name << ", ID: " << maxGpaStudent->studentID << ")\n";
        cout << "Lowest GPA:  " << minGpaStudent->GPA << " (Name: " << minGpaStudent->name << ", ID: " << minGpaStudent->studentID << ")\n";
    }

    void DisplayAboveThreshold(float threshold) {
        cout << "\n Students with GPA >= " << threshold << " \n";
        int found = 0;
        filterByGPA(root, threshold, found);
        if (found == 0) cout << "No students found.\n";
    }
};

int main() {
    StudentBST bst;

    cout << " STUDENT RECORD MANAGEMENT SYSTEM (BST) \n";

    // Insert student records
    bst.Insert(103, "Ali", 3.8);
    bst.Insert(101, "Ahmed", 2.9);
    bst.Insert(105, "Aras", 3.4);
    bst.Insert(102, "Sara", 3.1);
    bst.Insert(104, "Minahil", 3.95);

    // Display sorted records
    bst.DisplayInorder();

    // Total Count
    bst.DisplayTotalCount();

    // Search Operation
    bst.Search(102);
    bst.Search(999);

    // Analytical Features
    bst.DisplayGPAExtremes();

    float threshold;
    cout << "\nEnter GPA threshold: ";
    if (!(cin >> threshold)) threshold = 3.0f; // fallback if input is invalid/empty
    bst.DisplayAboveThreshold(threshold);

    // Delete Operation - all three cases
    cout << "\nDeleting Record (ID: 102 - Leaf Node)\n";
    bst.Delete(102);
    bst.DisplayInorder();

    cout << "\nDeleting Record (ID: 105 - Node with one child)\n";
    bst.Delete(105);
    bst.DisplayInorder();

    cout << "\nDeleting Record (ID: 103 - Root Node, two children)\n";
    bst.Delete(103);
    bst.DisplayInorder();

    cout << "\nDeleting Record (ID: 999 - Does not exist)\n";
    bst.Delete(999);

    bst.DisplayTotalCount();

    return 0;
}