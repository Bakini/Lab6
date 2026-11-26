#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

std::vector<std::vector<int>> create_matrix(size_t n){
    std::vector<std::vector<int>> matrix(n,std::vector<int>(n,0));

    for(size_t i=0;i<n;i++){
        for(size_t j=i+1;j<n;j++){
            int value=rand()%2;
            matrix[i][j]=value;
            matrix[j][i]=value;
        }
    }
    return matrix;
}

// void print_matrix(const std::vector<std::vector<int>>& matrix){
//     std::cout<<"  ";

//     for(size_t j=0;j<matrix.size();j++){
//         std::cout<<j+1<<" ";
//     }
//     std::cout<<"\n";

//     for(size_t i=0;i<matrix.size();i++){
//         std::cout<<i+1<<" ";

//         for(size_t j=0;j<matrix[i].size();j++){
//             std::cout<<matrix[i][j]<<" ";
//         }
//         std::cout<<"\n";
//     }
// }

void print_matrix(const std::vector<std::vector<int>>& matrix,const std::vector<int>& labels={}){
    if(matrix.empty()){
        std::cout<<"ГРАФ НЕ СОДЕРЖИТ ВЕРШИН!\n";
        return;
    }

    std::cout<<"  ";

    for(size_t j=0;j<matrix.size();j++){
        int number=labels.empty()?static_cast<int>(j+1):labels[j];
        std::cout<<number<<" ";
    }
    std::cout<<"\n";

    for(size_t i=0;i<matrix.size();i++){
        int number=labels.empty()?static_cast<int>(i+1):labels[i];
        std::cout<<number<<" ";

        for(size_t j=0;j<matrix[i].size();j++){
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}

std::vector<std::vector<int>> cartesian_product(const std::vector<std::vector<int>>& matrix1,const std::vector<std::vector<int>>& matrix2){
    size_t n1=matrix1.size();
    size_t n2=matrix2.size();
    size_t n=n1*n2;

    std::vector<std::vector<int>> result(n,std::vector<int>(n,0));

    for(size_t i=0;i<n;i++){
        size_t a=i/n2;
        size_t b=i%n2;

        for(size_t j=i+1;j<n;j++){
            size_t c=j/n2;
            size_t d=j%n2;

            if((a==c&&matrix2[b][d]==1)||(b==d&&matrix1[a][c]==1)){
                result[i][j]=1;
                result[j][i]=1;
            }
        }
    }
    return result;
}

std::vector<std::vector<int>> create_spisok(const std::vector<std::vector<int>>& matrix){
    std::vector<std::vector<int>> neighbors(matrix.size());

    for(size_t i=0;i<matrix.size();i++){
        for(size_t j=0;j<matrix[i].size();j++){
            if(matrix[i][j]==1)neighbors[i].push_back(j+1);
        }
    }
    return neighbors;
}

void print_spisok(const std::vector<std::vector<int>>& neighbors){
    for(size_t i=0;i<neighbors.size();i++){
        std::cout<<i+1<<": ";

        for(int schet:neighbors[i]){
            std::cout<<schet<<" ";
        }
        std::cout<<"\n";
    }
}

std::vector<std::vector<int>> merge_vertices(const std::vector<std::vector<int>>& matrix,size_t u,size_t v,bool keep_loops){
    size_t n=matrix.size();
    std::vector<std::vector<int>> new_matrix(n-1,std::vector<int>(n-1,0));

    for(size_t i=0;i<n-1;i++){
        for(size_t j=0;j<n-1;j++){
            size_t a=(i<v)?i:i+1;
            size_t b=(j<v)?j:j+1;

            if(a==b&&!keep_loops)continue;

            if(a==u&&b==u){
                new_matrix[i][j]=matrix[u][u]||matrix[v][v]||matrix[u][v];
            }
            else if(a==u){
                new_matrix[i][j]=matrix[u][b]||matrix[v][b];
            }
            else if(b==u){
                new_matrix[i][j]=matrix[a][u]||matrix[a][v];
            }
            else{
                new_matrix[i][j]=matrix[a][b];
            }
        }
    }
    return new_matrix;
}

std::vector<std::vector<int>> split_vertex(const std::vector<std::vector<int>>& matrix,size_t v){
    size_t n=matrix.size();
    std::vector<std::vector<int>> new_matrix(n+1,std::vector<int>(n+1,0));
    std::vector<size_t> old_to_new(n);

    size_t index=0;

    for(size_t i=0;i<n;i++){
        if(i!=v){
            old_to_new[i]=index;
            index++;
        }
    }

    for(size_t i=0;i<n;i++){
        for(size_t j=i+1;j<n;j++){
            if(i!=v&&j!=v&&matrix[i][j]==1){
                size_t a=old_to_new[i];
                size_t b=old_to_new[j];

                new_matrix[a][b]=1;
                new_matrix[b][a]=1;
            }
        }
    }

    size_t first=n-1;
    size_t second=n;
    size_t count=0;

    for(size_t i=0;i<n;i++){
        if(matrix[v][i]==1){
            size_t neighbor=old_to_new[i];
            size_t target=(count%2==0)?first:second;

            new_matrix[target][neighbor]=1;
            new_matrix[neighbor][target]=1;
            count++;
        }
    }

    new_matrix[first][second]=1;
    new_matrix[second][first]=1;

    return new_matrix;
}

std::vector<std::vector<int>> merge_vertices_spisok(const std::vector<std::vector<int>>& spisok,int u,int v,bool keep_loops){
    size_t n=spisok.size();
    std::vector<std::vector<int>> new_spisok(n-1);
    std::vector<int> old_to_new(n);

    for(size_t i=0;i<n;i++){
        int number=static_cast<int>(i+1);

        if(number==v)number=u;
        if(number>v)number--;

        old_to_new[i]=number;
    }

    for(size_t i=0;i<n;i++){
        int a=old_to_new[i];

        for(int neighbor:spisok[i]){
            int b=old_to_new[neighbor-1];

            if(a!=b||keep_loops)new_spisok[a-1].push_back(b);//петля
        }
    }

    for(std::vector<int>& row:new_spisok){
        std::sort(row.begin(),row.end());
        row.erase(std::unique(row.begin(),row.end()),row.end());
    }

    return new_spisok;
}

std::vector<std::vector<int>> split_vertex_spisok(const std::vector<std::vector<int>>& spisok,int v){
    int n=static_cast<int>(spisok.size());
    std::vector<std::vector<int>> new_spisok(n+1);

    int first=n;
    int second=n+1;

    for(int i=1;i<=n;i++){
        if(i==v)continue;

        int a=i;
        if(a>v)a--;

        for(int neighbor:spisok[i-1]){
            if(neighbor==v)continue;
            if(neighbor>v)neighbor--;

            new_spisok[a-1].push_back(neighbor);
        }
    }

    int count=0;

    for(int neighbor:spisok[v-1]){
        if(neighbor>v)neighbor--;

        int target=(count%2==0)?first:second;

        new_spisok[target-1].push_back(neighbor);
        new_spisok[neighbor-1].push_back(target);
        count++;
    }

    new_spisok[first-1].push_back(second);
    new_spisok[second-1].push_back(first);

    return new_spisok;
}

int main(){
    srand(static_cast<unsigned>(time(nullptr)));

    int n1,n2;

    std::cout<<"Введите количество вершин для первого графа: ";
    if(!(std::cin>>n1)||n1<=0){
        std::cout<<"НЕВЕРНЫЙ ВВОД.\nРАЗМЕР ДОЛЖЕН БЫТЬ ЦЕЛЫМ ЧИСЛОМ БОЛЬШЕ НУЛЯ\n";
        return 1;
    }

    std::cout<<"Введите количество вершин для второго графа: ";
    if(!(std::cin>>n2)||n2<=0){
        std::cout<<"НЕВЕРНЫЙ ВВОД.\nРАЗМЕР ДОЛЖЕН БЫТЬ ЦЕЛЫМ ЧИСЛОМ БОЛЬШЕ НУЛЯ\n";
        return 1;
    }

    std::vector<std::vector<int>> matrix1=create_matrix(n1);
    std::vector<std::vector<int>> matrix2=create_matrix(n2);

    if(matrix1==matrix2&&n2>1){
        matrix2[0][1]=1-matrix2[0][1];
        matrix2[1][0]=matrix2[0][1];
    }

    std::cout<<"\n";
    std::cout<<"ПЕРВЫЙ ГРАФ:\n";
    print_matrix(matrix1);

    std::cout<<"\n";
    std::cout<<"ВТОРОЙ ГРАФ:\n";
    print_matrix(matrix2);

    std::vector<std::vector<int>> spisok1=create_spisok(matrix1);
    std::vector<std::vector<int>> spisok2=create_spisok(matrix2);

    std::cout<<"\n";
    std::cout<<"СПИСОК СМЕЖНОСТИ ПЕРВОГО ГРАФА:\n";
    print_spisok(spisok1);

    std::cout<<"\n";
    std::cout<<"СПИСОК СМЕЖНОСТИ ВТОРОГО ГРАФА:\n";
    print_spisok(spisok2);
    std::cout<<"\n";

    int op;
    std::cout<<"1 - Объединение графов\n";
    std::cout<<"2 - Пересечение графов\n";
    std::cout<<"3 - Кольцевая сумма графов\n";
    std::cout<<"4 - Декартово произведение графов\n";
    std::cout<<"0 - Перейти к операциям с вершинами\n";
    std::cout<<"Выберите операцию: ";

    if(!(std::cin>>op)||op<0||op>4){
        std::cout<<"НЕВЕРНЫЙ ВВОД!\n";
        return 1;
    }

    if(op==4){
        std::cout<<"МАТРИЦА ДЕКАРТОВА ПРОИЗВЕДЕНИЯ:\n";
        print_matrix(cartesian_product(matrix1,matrix2));
        return 0;
    }

    if(op!=0){
        int n;

        if(op==2)n=std::min(n1,n2);
        else n=std::max(n1,n2);

        std::vector<std::vector<int>> result(n,std::vector<int>(n,0));

        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                int a=0;
                int b=0;

                if(i<n1&&j<n1)a=matrix1[i][j];
                if(i<n2&&j<n2)b=matrix2[i][j];

                if(op==1)result[i][j]=a||b;
                else if(op==2)result[i][j]=a&&b;
                else result[i][j]=(a!=b);
            }
        }

        // std::cout<<"РЕЗУЛЬТАТ ОПЕРАЦИИ:\n";
        // print_matrix(result);
        // return 0;

        std::vector<int> labels;

        for(int i=0;i<n;i++){
            bool has_edge=false;

            for(int j=0;j<n;j++){
                if(result[i][j]==1){
                    has_edge=true;
                    break;
                }
            }

            if(op!=3||has_edge){
                labels.push_back(i+1);
            }
        }

        std::vector<std::vector<int>> new_result(labels.size(),std::vector<int>(labels.size(),0));

        for(size_t i=0;i<labels.size();i++){
            for(size_t j=0;j<labels.size();j++){
                new_result[i][j]=result[labels[i]-1][labels[j]-1];
            }
        }

        std::cout<<"РЕЗУЛЬТАТ ОПЕРАЦИИ:\n";
        print_matrix(new_result,labels);
        return 0;

    }

    int choice,representation,operation;

    std::cout<<"ВЫБЕРИТЕ ГРАФ(1 или 2): ";
    if(!(std::cin>>choice)||(choice!=1&&choice!=2)){
        std::cout<<"НЕВЕРНЫЙ ВВОД!\n";
        return 1;
    }

    std::vector<std::vector<int>>& selected_matrix=(choice==1)?matrix1:matrix2;
    std::vector<std::vector<int>>& selected_spisok=(choice==1)?spisok1:spisok2;
    int n=static_cast<int>(selected_matrix.size());

    std::cout<<"ВЫБЕРИТЕ ПРЕДСТАВЛЕНИЕ ГРАФА:\n";
    std::cout<<"1 - Матрица смежности\n";
    std::cout<<"2 - Список смежности\n";

    if(!(std::cin>>representation)||(representation!=1&&representation!=2)){
        std::cout<<"НЕВЕРНЫЙ ВВОД!\n";
        return 1;
    }

    std::cout<<"ВЫБЕРИТЕ ОПЕРАЦИЮ:\n";
    std::cout<<"1 - Отождествление вершин\n";
    std::cout<<"2 - Стягивание ребра\n";
    std::cout<<"3 - Расщепление вершины\n";

    if(!(std::cin>>operation)||operation<1||operation>3){
        std::cout<<"НЕВЕРНЫЙ НОМЕР ОПЕРАЦИИ!\n";
        return 1;
    }

    if(operation==1||operation==2){
        int a,b;
        std::cout<<"Введите номера двух вершин: ";

        if(!(std::cin>>a>>b)||a<1||b<1||a>n||b>n||a==b){
            std::cout<<"НЕВЕРНЫЕ НОМЕРА ВЕРШИН!\n";
            return 1;
        }

        if(representation==1){
            if(operation==2&&selected_matrix[a-1][b-1]==0){
                std::cout<<"МЕЖДУ ВЕРШИНАМИ НЕТ РЕБРА\n";
                return 1;
            }

            selected_matrix=merge_vertices(selected_matrix,a-1,b-1,operation==1);//петля
        }
        else{
            const std::vector<int>& neighbors=selected_spisok[a-1];

            if(operation==2&&std::find(neighbors.begin(),neighbors.end(),b)==neighbors.end()){
                std::cout<<"МЕЖДУ ВЕРШИНАМИ НЕТ РЕБРА\n";
                return 1;
            }

            selected_spisok=merge_vertices_spisok(selected_spisok,a,b,operation==1);
        }
    }
    else{
        int v;
        std::cout<<"Введите номер вершины: ";

        if(!(std::cin>>v)||v<1||v>n){
            std::cout<<"НЕВЕРНЫЙ НОМЕР ВЕРШИНЫ\n";
            return 1;
        }

        if(representation==1){
            selected_matrix=split_vertex(selected_matrix,v-1);
        }
        else{
            selected_spisok=split_vertex_spisok(selected_spisok,v);
        }
    }

    std::cout<<"РЕЗУЛЬТАТ ОПЕРАЦИИ:\n";

    if(representation==1){
        print_matrix(selected_matrix);
    }
    else{
        print_spisok(selected_spisok);
    }

    return 0;
}