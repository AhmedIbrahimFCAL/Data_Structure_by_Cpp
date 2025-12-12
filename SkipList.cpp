#include<iostream>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<iomanip>

template<typename T>
class SkipListNode{
public:
    T value;
    std::vector<SkipListNode<T>*> pointers;
    SkipListNode(T value, SkipListNode<T>* node = nullptr):value(value){
        pointers.push_back(node);
    }

};
int C;
template<typename T>
struct SkipList{
    std::vector<SkipListNode<T>*> *head;
    unsigned int len;
public:
    SkipList():len(0){
        head = new std::vector<SkipListNode<T>*>(1,nullptr);
    }
    unsigned int max_height()const{
        return head->size();
    }
    unsigned int size()const{
        return len;
    }
    bool insert(T value){
        SkipListNode<T>* new_node = new SkipListNode<T>(value);
        int max_expeced_height = (33-__builtin_clz(len))<<1;
        int i = 0;

        std::vector<SkipListNode<T>*> *current = head;
        std::vector<std::vector<SkipListNode<T>*> *> ancestors;
        while(current != nullptr){
            for(i = current->size()-1 ;i+1; i--){
                if((*current)[i]!=nullptr){
                    if((*current)[i]->value == value){
                        delete new_node;
                        return false;
                    }
                    if((*current)[i]->value < value){
                        ancestors.push_back(current);
                        current = &(*current)[i]->pointers;
                        break;
                    }
                }
            }
            if(i==-1){
                len++;
                ancestors.push_back(current);
                new_node->pointers[0] = (*current)[0];
                // Randomize the node
                while(i<max_expeced_height && rand()&1){
                    new_node->pointers.push_back(nullptr);
                    i++;
                }
                // make sure that hieght the first node the maximum height
                while(head->size() < new_node->pointers.size()){
                    head->push_back(nullptr);
                }
                (*current)[0] = new_node;
                break;
            }
        }
        // linking part, back-track
        int p = ancestors.size()-1;
        current = ancestors[p];
        SkipListNode<T>* temp;
        for(i=1;i<new_node->pointers.size();i++){
            while(i+1 > current->size()){
                current = ancestors[--p];
            }
            temp = (*current)[i];
            new_node->pointers[i] = temp;
            (*current)[i] = new_node;
        }
        return true;
    }
    T* search(T value){
        std::vector<SkipListNode<T>*> *current = head;
        int i = head->size()-1;
        while(current != nullptr && 0<=i){
            C++;
            if((*current)[i]!=nullptr){
                if((*current)[i]->value == value)
                    return &(*current)[i]->value;
                if((*current)[i]->value < value){
                    current = &(*current)[i]->pointers;
                    continue;
                }
            }
            i--;
        }
        return nullptr;
    }
    friend std::ostream& operator<<(std::ostream& out,const SkipList &skipList){
        int n = skipList.max_height();
        int m = skipList.size();
        out<<m<<'\n';
        std::pair<int,int> grid[m];
        int i, j;
        SkipListNode<T>* current = (*skipList.head)[0];
        i = 0;
        int max_num_digits=0;
        int num_digits;
        // fill grid
        while(current != nullptr){
            num_digits = std::to_string(current->value).size();
            max_num_digits = max_num_digits<num_digits ? num_digits : max_num_digits;
            grid[i] = {current->value, current->pointers.size()};
            current = current->pointers[0];
            i+=1;
        }
        out << std::left;
        bool is_print_value;
        for(i=n-1;i+1;i--){
            out << "S";
            is_print_value = false;
            for(j=-1;j<m;++j){
                out<<std::setfill('-')<<std::setw(max_num_digits);
                if(!is_print_value || grid[j].second <= i){
                    out<<"-";
                }else{
                    out<<grid[j].first;
                }
                if(is_print_value){
                    j--;
                }
                is_print_value = !is_print_value;
            }
            out << "E";
            if(i)
                out << '\n';
        }
        out << std::right;
        return out;
    }
    void print_heights(std::ostream& out, bool is_print_label = true, bool is_print_len_sumHeights = false){
        std::vector<SkipListNode<T>*> *current = head;
        int C = 0;
        if(is_print_label){
            out<<"Heights: ";
        }
        out << "[";
        while(current != nullptr){
            C += current->size();
            out << current->size();
            if((*current)[0] == nullptr){
                break;
            }
            out << ", ";
            current = &((*current)[0]->pointers);
        }
        out << "]";
        if(is_print_len_sumHeights)
            out << " (" << len << ", " << C << ")";
    }
    void print_pointers(std::ostream& out){
        if(this->size() == 0){
            out<<"{}";
            return;
        }
        out << "{\n";
        SkipListNode<T>* current = (*head)[0];
        out <<std::setfill(' ')<<std::setw(6) << "head"<<" -> [";
        for(int i=0;i<(*head).size();i++){
            if((*head)[i] != nullptr){
                out << (*head)[i]->value;
            }else{
                out << "N";
            }
            if(i+1 < (*head).size()){
                out<<", ";
            }
        }
        out<<"]\n";
        while(current != nullptr){
            out << std::setfill(' ')<<std::setw(6) << current->value << " -> [";
            for(int i=0;i<current->pointers.size();i++){
                if(current->pointers[i])
                    out << current->pointers[i]->value;
                else
                    out << "N";

                if(i+1<current->pointers.size())
                    out<<", ";
            }
            out<<"]\n";
            current = current->pointers[0];
        }
        out<<"}\n";
    }
    
    void print_values(std::ostream& out,bool is_print_label = true){
        if(is_print_label){
            out<<"Values: ";
        }
        out << '[';
        SkipListNode<T>* current = (*head)[0];
        while(current != nullptr){
            out << current->value;
            current = current->pointers[0];
            if(current != nullptr){
                out << ", ";
            }
        }
        out<<']';
    }
    bool remove(T value){
        std::vector<std::vector<SkipListNode<T>*> *> ancestors;
        std::vector<SkipListNode<T>*> *current = head;
        int i = this->max_height()-1;
        SkipListNode<T>* del_node = nullptr;
        while(current != nullptr){
            if((*current)[i]!=nullptr){
                if((*current)[i]->value < value){
                    ancestors.push_back(current);
                    current = &(*current)[i]->pointers;
                    continue;
                }
                if(i==0 && (*current)[i]->value == value){
                    del_node = (*current)[i];
                    int p=ancestors.size()-1;
                    // relinking back-track
                    for(int l=0;l < del_node->pointers.size();l++){
                        while(current->size() < l+1){
                            current = ancestors[p--];
                        }
                        (*current)[l] = del_node->pointers[l];                    
                    }
                    delete del_node;
                    len--;
                    return true;
                }
            }
            if(i==0){
                break;
            }
            i--;
        }
        return false;
    }
    void clear(){
        SkipListNode<T>* curr = (*head)[0];
        SkipListNode<T>* temp;
        while(curr!=nullptr){
            temp = curr;
            curr = curr->pointers[0];
            delete temp;
            len--;
        }
        head->clear();
        head->push_back(nullptr);
    }
    ~SkipList(){
        this->clear();
        delete head;
    }
};
class Student{
    int ID;
    std::string name;
public:
    Student(int ID,std::string name):ID(ID),name(name){}
    friend bool operator==(const Student& student1, const Student& student2){
        return student1.ID == student2.ID;
    }
    friend bool operator<(const Student& student1, const Student& student2){
        return student1.ID < student2.ID;
    }
    
    friend bool operator>(const Student& student1, const Student& student2){
        return student1.ID < student2.ID;
    }
    operator int()const{
        return this->ID;
    }
};
int main(){
    std::ios_base::sync_with_stdio(false),std::cout.tie(nullptr);//,std::cin.tie(nullptr);
    std::srand(std::time(0));
    SkipList<Student> skipList;
    int n = rand()%36 + 5;
    int data[n];
    for(int i=0;i<n;++i){
        data[i] = rand()%100;
        if(i<n-5)
            skipList.insert(Student(data[i],""));
    }
    for(int i=0;i<n;++i){
        std::cout << data[i] << ' ' << skipList.search(Student(data[i],"")) << '\n';
    }
    for(int i=0;i<n;++i){
        std::cout << data[i] << ' ' << skipList.insert(Student(data[i],"")) << '\n';
    }
    std::cout<<skipList<<'\n';
    for(int i=0;i<n;++i){
        std::cout << data[i] << ' ' << skipList.remove(Student(data[i],"")) << '\n';
        std::cout<<skipList<<'\n';
    }
    return 0;
}
