	#include <iostream>
	#include <fstream>
	#include <cstring>
	#include <cstdlib>
	#include <iomanip>
	using namespace std;
	
	const char* CARS_FILE = "cars.dat";
	const char* RECYCLE_FILE = "recycle.dat";
	
	struct Car {
	    int id;
	    char model[50];
	    char brand[30];
	    char category[30];
	    char registrationNo[30];
	    char insurance[50];
	    double price;
	    int year;
	};
	
	struct Node {
	    Car data;
	    Node* next;
	    Node(const Car &c) : data(c), next(NULL) {}
	};
	
	struct StackNode {
	    Car data;
	    StackNode* next;
	    StackNode(const Car &c) : data(c), next(NULL) {}
	};
	
	struct BSTNode {
	    Car data;
	    BSTNode* left;
	    BSTNode* right;
	    BSTNode(const Car &c) : data(c), left(NULL), right(NULL) {}
	};
	
	Node* head = NULL;
	StackNode* recycleTop = NULL;
	
	BSTNode* bstByID = NULL;
	BSTNode* bstByPrice = NULL;
	
	void printCar(const Car &c) {
	    cout << left << setw(6) << c.id
	         << setw(12) << c.year
	         << setw(12) << c.price
	         << setw(15) << c.brand
	         << setw(18) << c.model
	         << setw(12) << c.category
	         << setw(16) << c.registrationNo
	         << setw(18) << c.insurance << "\n";
	}
	
	void printHeader() {
	    cout << left << setw(6) << "ID"
	         << setw(12) << "Year"
	         << setw(12) << "Price"
	         << setw(15) << "Brand"
	         << setw(18) << "Model"
	         << setw(12) << "Category"
	         << setw(16) << "RegNo"
	         << setw(18) << "Insurance" << "\n";
	    cout << "--------------------------------------------------------------------------------------------------------\n";
	}
	
	void insertAtEnd(const Car &c) {
	    Node* nn = new Node(c);
	    if(!head) { head = nn; return; }
	    Node* cur = head;
	    while(cur->next) cur = cur->next;
	    cur->next = nn;
	}
	
	Node* findNodeById(int id) {
	    Node* cur = head;
	    while(cur) {
	        if(cur->data.id == id) return cur;
	        cur = cur->next;
	    }
	    return NULL;
	}
	
	bool removeById(int id, Car &outCar) {
	    if(!head) return false;
	    if(head->data.id == id) {
	        Node* tmp = head;
	        head = head->next;
	        outCar = tmp->data;
	        delete tmp;
	        return true;
	    }
	    Node* prev = head;
	    Node* cur = head->next;
	    while(cur) {
	        if(cur->data.id == id) {
	            prev->next = cur->next;
	            outCar = cur->data;
	            delete cur;
	            return true;
	        }
	        prev = cur;
	        cur = cur->next;
	    }
	    return false;
	}
	
	void pushRecycle(const Car &c) {
	    StackNode* sn = new StackNode(c);
	    sn->next = recycleTop;
	    recycleTop = sn;
	}
	
	bool popRecycle(Car &outCar) {
	    if(!recycleTop) return false;
	    StackNode* tmp = recycleTop;
	    recycleTop = recycleTop->next;
	    outCar = tmp->data;
	    delete tmp;
	    return true;
	}
	
	BSTNode* insertBSTByID(BSTNode* root, const Car &c) {
	    if(!root) return new BSTNode(c);
	    if(c.id < root->data.id) root->left = insertBSTByID(root->left, c);
	    else root->right = insertBSTByID(root->right, c);
	    return root;
	}
	
	BSTNode* insertBSTByPrice(BSTNode* root, const Car &c) {
	    if(!root) return new BSTNode(c);
	    if(c.price < root->data.price) root->left = insertBSTByPrice(root->left, c);
	    else root->right = insertBSTByPrice(root->right, c);
	    return root;
	}
	
	BSTNode* minValueNode(BSTNode* node) {
	    BSTNode* current = node;
	    while(current && current->left) current = current->left;
	    return current;
	}
	
	BSTNode* deleteBSTByID(BSTNode* root, int id) {
	    if(!root) return root;
	    if(id < root->data.id) root->left = deleteBSTByID(root->left, id);
	    else if(id > root->data.id) root->right = deleteBSTByID(root->right, id);
	    else {
	        if(!root->left) {
	            BSTNode* temp = root->right;
	            delete root;
	            return temp;
	        } else if(!root->right) {
	            BSTNode* temp = root->left;
	            delete root;
	            return temp;
	        }
	        BSTNode* temp = minValueNode(root->right);
	        root->data = temp->data;
	        root->right = deleteBSTByID(root->right, temp->data.id);
	    }
	    return root;
	}
	
	BSTNode* deleteBSTByPrice(BSTNode* root, double price, int idFallback) {
	    if(!root) return root;
	    if(price < root->data.price) root->left = deleteBSTByPrice(root->left, price, idFallback);
	    else if(price > root->data.price) root->right = deleteBSTByPrice(root->right, price, idFallback);
	    else {
	        if(root->data.id != idFallback) {
	            root->left = deleteBSTByPrice(root->left, price, idFallback);
	            root->right = deleteBSTByPrice(root->right, price, idFallback);
	            return root;
	        }
	        if(!root->left) {
	            BSTNode* temp = root->right;
	            delete root;
	            return temp;
	        } else if(!root->right) {
	            BSTNode* temp = root->left;
	            delete root;
	            return temp;
	        }
	        BSTNode* temp = minValueNode(root->right);
	        root->data = temp->data;
	        root->right = deleteBSTByPrice(root->right, temp->data.price, temp->data.id);
	    }
	    return root;
	}
	
	BSTNode* searchBSTByID(BSTNode* root, int id) {
	    if(!root) return NULL;
	    if(id == root->data.id) return root;
	    if(id < root->data.id) return searchBSTByID(root->left, id);
	    return searchBSTByID(root->right, id);
	}
	
	void saveAll() {
	    ofstream ofs(CARS_FILE, ios::binary);
	    Node* cur = head;
	    while(cur) {
	        ofs.write((char*)&cur->data, sizeof(Car));
	        cur = cur->next;
	    }
	    ofs.close();
	
	    ofstream ofs2(RECYCLE_FILE, ios::binary);
	    StackNode* s = recycleTop;
	    while(s) {
	        ofs2.write((char*)&s->data, sizeof(Car));
	        s = s->next;
	    }
	    ofs2.close();
	}
	
	void clearDataStructures() {
	    while(head) {
	        Node* t = head;
	        head = head->next;
	        delete t;
	    }
	    while(recycleTop) {
	        StackNode* t = recycleTop;
	        recycleTop = recycleTop->next;
	        delete t;
	    }
	    while(bstByID) bstByID = deleteBSTByID(bstByID, bstByID->data.id);
	    while(bstByPrice) bstByPrice = deleteBSTByPrice(bstByPrice, bstByPrice->data.price, bstByPrice->data.id);
	}
	
	void loadAll() {
	    clearDataStructures();
	    ifstream ifs(CARS_FILE, ios::binary);
	    if(ifs) {
	        Car c;
	        while(ifs.read((char*)&c, sizeof(Car))) {
	            insertAtEnd(c);
	            bstByID = insertBSTByID(bstByID, c);
	            bstByPrice = insertBSTByPrice(bstByPrice, c);
	        }
	        ifs.close();
	    }
	    ifstream ifs2(RECYCLE_FILE, ios::binary);
	    if(ifs2) {
	        Car c;
	        Car arr[1000];
	        int count = 0;
	        while(ifs2.read((char*)&c, sizeof(Car))) {
	            if(count < 1000) arr[count++] = c;
	        }
	        for(int i = count-1; i >=0; --i)
	            pushRecycle(arr[i]);
	        ifs2.close();
	    }
	}
	
	int generateNewID() {
	    int maxid = 0;
	    Node* cur = head;
	    while(cur) {
	        if(cur->data.id > maxid) maxid = cur->data.id;
	        cur = cur->next;
	    }
	    StackNode* s = recycleTop;
	    while(s) {
	        if(s->data.id > maxid) maxid = s->data.id;
	        s = s->next;
	    }
	    return maxid + 1;
	}
	
	void addNewCarInteractive() {
	    Car c;
	    c.id = generateNewID();
	    cout << "Enter model: "; cin.ignore(); cin.getline(c.model, 50);
	    cout << "Enter brand: "; cin.getline(c.brand, 30);
	    cout << "Enter category: "; cin.getline(c.category,30);
	    cout << "Enter registration number: "; cin.getline(c.registrationNo, 30);
	    cout << "Enter insurance: "; cin.getline(c.insurance, 50);
	    cout << "Enter price: "; cin >> c.price;
	    cout << "Enter year: "; cin >> c.year;
	
	    insertAtEnd(c);
	    bstByID = insertBSTByID(bstByID, c);
	    bstByPrice = insertBSTByPrice(bstByPrice, c);
	
	    cout << "Car added with ID: " << c.id << "\n";
	}
	
	void displayAll() {
	    if(!head) { cout << "No cars available.\n"; return; }
	    printHeader();
	    Node* cur = head;
	    while(cur) {
	        printCar(cur->data);
	        cur = cur->next;
	    }
	}
	
	void searchByIDInteractive() {
	    int id; cout << "Enter ID: "; cin >> id;
	    BSTNode* found = searchBSTByID(bstByID, id);
	    if(found) {
	        printHeader();
	        printCar(found->data);
	    } else cout << "No car found.\n";
	}
	
	int strcasecmp(const char *a,const char *b){
	    for(; *a && *b; ++a, ++b){
	        char x = *a, y = *b;
	        if(x >= 'A' && x <= 'Z') x += 32;
	        if(y >= 'A' && y <= 'Z') y += 32;
	        if(x != y) return x - y;
	    }
	    return (unsigned char)*a - (unsigned char)*b;
	}
	
	void searchByModelOrBrand() {
	    cout << "Search by: 1) Model  2) Brand\nChoose: ";
	    int ch; cin >> ch;
	    cin.ignore();
	    if(ch == 1) {
	        char q[50]; cout << "Model: "; cin.getline(q,50);
	        bool found=false;
	        Node* cur=head;
	        printHeader();
	        while(cur) {
	            if(strcasecmp(cur->data.model, q) == 0) {
	                printCar(cur->data); found=true;
	            }
	            cur = cur->next;
	        }
	        if(!found) cout << "No results.\n";
	    } else {
	        char q[30]; cout << "Brand: "; cin.getline(q,30);
	        bool found=false;
	        Node* cur=head;
	        printHeader();
	        while(cur) {
	            if(strcasecmp(cur->data.brand, q) == 0) {
	                printCar(cur->data); found=true;
	            }
	            cur = cur->next;
	        }
	        if(!found) cout << "No results.\n";
	    }
	}
	
	void updateCarInteractive() {
	    int id; cout << "Enter ID to update: "; cin >> id;
	    Node* n = findNodeById(id);
	    if(!n) { cout << "Car not found.\n"; return; }
	
	    cout << "Existing details:\n";
	    printHeader(); printCar(n->data);
	
	    cout << "Enter new details (press ENTER to keep old value):\n";
	    cin.ignore();
	
	    char buf[120];
	
	    cout << "Model [" << n->data.model << "]: "; cin.getline(buf,120);
	    if(strlen(buf)) strncpy(n->data.model, buf, sizeof(n->data.model));
	
	    cout << "Brand [" << n->data.brand << "]: "; cin.getline(buf,120);
	    if(strlen(buf)) strncpy(n->data.brand, buf, sizeof(n->data.brand));
	
	    cout << "Category [" << n->data.category << "]: "; cin.getline(buf,120);
	    if(strlen(buf)) strncpy(n->data.category, buf, sizeof(n->data.category));
	
	    cout << "Registration [" << n->data.registrationNo << "]: "; cin.getline(buf,120);
	    if(strlen(buf)) strncpy(n->data.registrationNo, buf, sizeof(n->data.registrationNo));
	
	    cout << "Insurance [" << n->data.insurance << "]: "; cin.getline(buf,120);
	    if(strlen(buf)) strncpy(n->data.insurance, buf, sizeof(n->data.insurance));
	
	    cout << "Price [" << n->data.price << "]: ";
	    string priceStr; getline(cin, priceStr);
	    if(priceStr.size()) n->data.price = atof(priceStr.c_str());
	
	    cout << "Year [" << n->data.year << "]: ";
	    string yearStr; getline(cin, yearStr);
	    if(yearStr.size()) n->data.year = atoi(yearStr.c_str());
	
	    while(bstByID) bstByID = deleteBSTByID(bstByID, bstByID->data.id);
	    while(bstByPrice) bstByPrice = deleteBSTByPrice(bstByPrice, bstByPrice->data.price, bstByPrice->data.id);
	
	    Node* cur = head;
	    while(cur) {
	        bstByID = insertBSTByID(bstByID, cur->data);
	        bstByPrice = insertBSTByPrice(bstByPrice, cur->data);
	        cur = cur->next;
	    }
	
	    cout << "Car updated.\n";
	}
	
	void deleteCarInteractive() {
	    int id; cout << "Enter ID: "; cin >> id;
	    Car removed;
	    if(removeById(id, removed)) {
	        pushRecycle(removed);
	        bstByID = deleteBSTByID(bstByID, removed.id);
	        bstByPrice = deleteBSTByPrice(bstByPrice, removed.price, removed.id);
	        cout << "Car moved to recycle bin.\n";
	    } else cout << "Car not found.\n";
	}
	
	void viewRecycleBin() {
	    if(!recycleTop) { cout << "Recycle bin is empty.\n"; return; }
	    printHeader();
	    StackNode* s = recycleTop;
	    while(s) {
	        printCar(s->data);
	        s = s->next;
	    }
	}
	
	void restoreFromRecycleInteractive() {
	    if(!recycleTop) { cout << "Recycle bin empty.\n"; return; }
	    Car c;
	
	    cout << "Restore: 1) Top  2) By ID\nChoose: ";
	    int ch; cin >> ch;
	
	    if(ch == 1) {
	        popRecycle(c);
	        insertAtEnd(c);
	        bstByID = insertBSTByID(bstByID, c);
	        bstByPrice = insertBSTByPrice(bstByPrice, c);
	        cout << "Car restored.\n";
	    } else {
	        int id; cout << "Enter ID: "; cin >> id;
	
	        StackNode* cur = recycleTop;
	        StackNode* prev = NULL;
	
	        while(cur) {
	            if(cur->data.id == id) break;
	            prev = cur;
	            cur = cur->next;
	        }
	
	        if(!cur) { cout << "ID not found.\n"; return; }
	
	        Car found = cur->data;
	
	        if(prev == NULL) recycleTop = cur->next;
	        else prev->next = cur->next;
	
	        delete cur;
	
	        insertAtEnd(found);
	        bstByID = insertBSTByID(bstByID, found);
	        bstByPrice = insertBSTByPrice(bstByPrice, found);
	
	        cout << "Car restored.\n";
	    }
	}
	
	void sortAndDisplayByPrice() {
	    int n=0; Node* cur=head;
	    while(cur) { n++; cur = cur->next; }
	    if(n==0) { cout << "No cars.\n"; return; }
	
	    Car* arr = new Car[n];
	    cur = head; int i=0;
	    while(cur) { arr[i++] = cur->data; cur = cur->next; }
	
	    for(int a=0;a<n-1;a++)
	        for(int b=0;b<n-1-a;b++)
	            if(arr[b].price > arr[b+1].price) {
	                Car tmp = arr[b]; arr[b]=arr[b+1]; arr[b+1]=tmp;
	            }
	
	    printHeader();
	    for(int k=0;k<n;k++) printCar(arr[k]);
	
	    delete [] arr;
	}
	
	void sortAndDisplayByBrand() {
	    int n=0; Node* cur=head;
	    while(cur) { n++; cur = cur->next; }
	    if(n==0) { cout << "No cars.\n"; return; }
	
	    Car* arr = new Car[n];
	    cur = head; int i=0;
	    while(cur) { arr[i++] = cur->data; cur = cur->next; }
	
	    for(int a=0;a<n-1;a++)
	        for(int b=0;b<n-1-a;b++)
	            if(strcasecmp(arr[b].brand, arr[b+1].brand) > 0) {
	                Car tmp = arr[b]; arr[b]=arr[b+1]; arr[b+1]=tmp;
	            }
	
	    printHeader();
	    for(int k=0;k<n;k++) printCar(arr[k]);
	
	    delete [] arr;
	}
	
	bool adminLogin() {
	    const char adminUser[] = "admin";
	    const char adminPass[] = "admin123";
	
	    char user[50], pass[50];
	
	    cout << "Admin Username: "; cin >> user;
	    cout << "Password: "; cin >> pass;
	
	    if(strcmp(user, adminUser)==0 && strcmp(pass, adminPass)==0)
	        return true;
	
	    cout << "Invalid admin credentials.\n";
	    return false;
	}
	
	struct SimpleUser {
	    char username[40];
	    char password[40];
	    bool isAdmin;
	    SimpleUser* next;
	
	    SimpleUser(const char *u, const char *p, bool a=false) : next(NULL), isAdmin(a) {
	        strncpy(username, u, sizeof(username));
	        strncpy(password, p, sizeof(password));
	    }
	};
	
	SimpleUser* users = NULL;
	
	void addDefaultUsers() {
	    if(!users) {
	        users = new SimpleUser("user", "user123", false);
	        SimpleUser* a = new SimpleUser("admin", "admin123", true);
	        users->next = a;
	    }
	}
	
	SimpleUser* findUser(const char *u) {
	    SimpleUser* cur = users;
	    while(cur) {
	        if(strcmp(cur->username, u) == 0) return cur;
	        cur = cur->next;
	    }
	    return NULL;
	}
	
	void registerUserInteractive() {
	    cout << "Register new user\n";
	    char u[40], p[40];
	
	    cout << "Username: "; cin >> u;
	    if(findUser(u)) { cout << "User exists.\n"; return; }
	
	    cout << "Password: "; cin >> p;
	
	    SimpleUser* nu = new SimpleUser(u,p,false);
	    nu->next = users;
	    users = nu;
	
	    cout << "User registered.\n";
	}
	
	bool userLoginInteractive(SimpleUser* &outUser) {
	    char u[40], p[40];
	    cout << "Username: "; cin >> u;
	    cout << "Password: "; cin >> p;
	
	    SimpleUser* fu = findUser(u);
	    if(fu && strcmp(fu->password, p) == 0) {
	        outUser = fu;
	        return true;
	    }
	
	    cout << "Invalid credentials.\n";
	    return false;
	}
	
	void userMenu() {
	    SimpleUser* su = NULL;
	    if(!userLoginInteractive(su)) return;
	
	    int ch;
	    do {
	        cout << "\n--- User Menu ---\n";
	        cout << "1. View All Cars\n2. Search by ID\n3. Search by Model/Brand\n4. Sort by Price\n5. Sort by Brand\n0. Logout\nChoose: ";
	        cin >> ch;
	
	        switch(ch) {
	            case 1: displayAll(); break;
	            case 2: searchByIDInteractive(); break;
	            case 3: searchByModelOrBrand(); break;
	            case 4: sortAndDisplayByPrice(); break;
	            case 5: sortAndDisplayByBrand(); break;
	            case 0: break;
	            default: cout << "Invalid choice.\n";
	        }
	
	    } while(ch != 0);
	}
	
	void adminMenu() {
	    if(!adminLogin()) return;
	
	    int ch;
	    do {
	        cout << "\n--- Admin Menu ---\n";
	        cout << "1. Add New Car\n2. Display All Cars\n3. Update Car\n4. Delete Car\n5. View Recycle Bin\n6. Restore From Recycle Bin\n7. Save Data\n8. Load Data\n9. Sort by Price\n10. Sort by Brand\n11. Search by ID\n12. Search by Model/Brand\n0. Exit to Main\nChoose: ";
	        cin >> ch;
	
	        switch(ch) {
	            case 1: addNewCarInteractive(); break;
	            case 2: displayAll(); break;
	            case 3: updateCarInteractive(); break;
	            case 4: deleteCarInteractive(); break;
	            case 5: viewRecycleBin(); break;
	            case 6: restoreFromRecycleInteractive(); break;
	            case 7: saveAll(); break;
	            case 8: loadAll(); break;
	            case 9: sortAndDisplayByPrice(); break;
	            case 10: sortAndDisplayByBrand(); break;
	            case 11: searchByIDInteractive(); break;
	            case 12: searchByModelOrBrand(); break;
	            case 0: break;
	            default: cout << "Invalid choice.\n";
	        }
	
	    } while(ch != 0);
	}
	
	int main() {
	    addDefaultUsers();
	    loadAll();
	
	    cout << " Car Sale Management System \n";
	
	    int ch;
	    do {
	        cout << "\nMain Menu:\n1. Admin Login\n2. User Login\n3. Register (User)\n4. Save Data\n5. Load Data\n0. Exit\nChoose: ";
	        cin >> ch;
	
	        switch(ch) {
	            case 1: adminMenu(); break;
	            case 2: userMenu(); break;
	            case 3: registerUserInteractive(); break;
	            case 4: saveAll(); break;
	            case 5: loadAll(); break;
	            case 0: cout << "Exiting...\n"; break;
	            default: cout << "Invalid choice.\n";
	        }
	
	    } while(ch != 0);
	
	    saveAll();
	
	    clearDataStructures();
	
	    while(users) {
	        SimpleUser* t = users;
	        users = users->next;
	        delete t;
	    }
	
	    return 0;
	}
