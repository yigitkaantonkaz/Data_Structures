#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

class Cell{
public:
    bool visited;
    //walls information
    bool l,r,d,u;
    //connection with neighbours
    vector<Cell*> neighbours;
    vector<Cell*> static_neighbours;
    Cell * path = nullptr;
    //location in maze
    int x_cordinate;
    int y_cordinate;
    
    Cell (int x, int y){
        this->x_cordinate = x;
        this->y_cordinate = y;
        visited = false;
        this->l = 1;
        this->r = 1;
        this->u = 1;
        this->d = 1;
    }
    
};

template <typename T>
class Stack{
public:
    Cell * top;
    
    Stack(){
        top = nullptr;
    };
    
    ~Stack(){
        
    };
    bool isEmpty(){
        return (top == nullptr);
    };
    
    void pop(Cell * ptr){
        if(!isEmpty()){
            top = ptr->path;
            ptr->path = nullptr;
        }
    };
    
    void push(Cell * ptr){
        if(isEmpty()){
            top = ptr;
        }
        else{
            ptr->path = top;
            top = ptr;
        }
    };
};


class Maze{
public:
    int MazeID;
    Cell*** base;
    int rows, cols;
    
    Maze(int rows, int cols){
        this->MazeID = 0;
        this->rows = rows;
        this->cols = cols;
        
        base = new Cell** [rows];
        for (int i = 0; i < rows; ++i) {
            base[i] = new Cell* [cols];
            for (int j = 0; j < cols; ++j) {
                base[i][j] = new Cell(i,j);
            }
        }
        
        // connecting cells that are neighbours
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (i > 0) {//check down
                    base[i][j]->neighbours.push_back(base[i-1][j]);
                    base[i][j]->static_neighbours.push_back(base[i-1][j]);
                }
                if (j > 0){ // check left
                    base[i][j]->neighbours.push_back(base[i][j-1]);
                    base[i][j]->static_neighbours.push_back(base[i][j-1]);
                }
                if (i < rows-1){//check up
                    base[i][j]->neighbours.push_back(base[i+1][j]);
                    base[i][j]->static_neighbours.push_back(base[i+1][j]);
                }
                if (j < cols-1){//check right
                    base[i][j]->neighbours.push_back(base[i][j+1]);
                    base[i][j]->static_neighbours.push_back(base[i][j+1]);
                }
            }
        }
    }
    
    
    Cell* rnd_func(Cell* current){
        if(current->neighbours.size() != 0){
            int a = rand() % current->neighbours.size();
            Cell * neighbour = current->neighbours[a];
            if(!(neighbour->visited)){ // check if it's first time visiting
                if((neighbour->y_cordinate)-(current->y_cordinate) == 1){ // right
                    current->r = 0;
                    neighbour->l = 0;
                }
                else if ((neighbour->x_cordinate) - (current->x_cordinate) == 1){ // up
                    current->u = 0;
                    neighbour->d = 0;
                }
                else if ((neighbour->y_cordinate)-(current->y_cordinate) == -1){ // left
                    current->l = 0;
                    neighbour->r = 0;
                }
                else if ((neighbour->x_cordinate) - (current->x_cordinate) == -1){ // down
                    current->d = 0;
                    neighbour->u = 0;
                }
                neighbour->visited = true;
                return neighbour;
            }
            // neighbour have been visited before
            // pop neighbour from the vector
            for(int i = 0; i < current->neighbours.size(); i++){
                if(current->neighbours[i] == neighbour){
                    current->neighbours.erase(current->neighbours.begin()+i);
                }
            }
        }
        return nullptr;
    }
    
    void creating_maze() {
        Cell* current = base[0][0];
        current->visited = true;
        Stack<Cell> stack;
        stack.push(current);
        
        while (true) {
            Cell* next = rnd_func(current);
            if(current->neighbours.size() == 0){ // means blocked
                stack.pop(current);
                current = stack.top;
            }
            if (next){
                stack.push(next);
                current = next;
            }
            if(stack.top == nullptr){break;}
        }
        
    }
    
    Cell* rnd_path(Cell* current){
        int b = rand() % current->static_neighbours.size();
        return current->static_neighbours[b];
    }
    
    void remove_from_neighbour(Cell* current, Cell* next){
        for(int i = 0; i < current->static_neighbours.size(); i++){
            if(current->static_neighbours[i] == next){
                current->static_neighbours.erase(current->static_neighbours.begin()+i);
            }
        }
    }
    
    
    Stack<Cell> path_maze(int entryX, int entryY, int exitX, int exitY){
        Stack<Cell> s_path;
        Cell* current = base[entryY][entryX];// starting cell
        Cell* finish = base[exitY][exitX];// end cell
        s_path.push(current);
        current->visited = false;
        
        while(true){
            Cell* next = rnd_path(current);
            
            if(next->visited){
                if((next->y_cordinate)-(current->y_cordinate) == 1){ // right
                    if(!(current->r && next->l)){// means open (no wall) right side
                        s_path.push(next);
                        current->visited = false;
                        current = next;
                    }
                    else{
                        remove_from_neighbour(current, next);
                    }
                }
                else if ((next->x_cordinate) - (current->x_cordinate) == 1){ // up
                    if(!(current->u && next->d)){// no wall
                        s_path.push(next);
                        current->visited = false;
                        current = next;
                    }
                    else{
                        remove_from_neighbour(current, next);
                    }
                }
                else if ((next->y_cordinate)-(current->y_cordinate) == -1){ // left
                    if(!(current->l && next->r)){
                        s_path.push(next);
                        current->visited = false;
                        current = next;
                    }
                    else{
                        remove_from_neighbour(current, next);
                    }
                }
                else if ((next->x_cordinate) - (current->x_cordinate) == -1){ // down
                    if (!(current->d && next->u)){
                        s_path.push(next);
                        current->visited = false;
                        current = next;
                    }
                    else{
                        remove_from_neighbour(current, next);
                    }
                }
            }
            else{// visited condition
                if(current->static_neighbours.size() == 1){// means block, only have previous visited cell
                    s_path.pop(current);
                    remove_from_neighbour(s_path.top, current);
                    current = s_path.top;
                    
                }
            }
            if(current == finish){break;}
        }
        
        return s_path;
        
    }
};


int main() {
    vector<Maze> mazes;
    int M, N, K;
    int mazeID, entryX, entryY, exitX, exitY;// y = row, x = col
    cout << "Enter the number of mazes: ";
    cin >> K;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;
    srand(time(NULL));
    ofstream outfile;
    for(int i = 0; i < K; ++i){
        outfile.open("maze_"+ to_string(i+1) + ".txt");
        Maze maze(M, N);
        maze.creating_maze();
        maze.MazeID = i+1;
        outfile << M << " " << N << endl;
        for(int i = 0; i < maze.rows; i++){
            for(int k = 0; k < maze.cols; k++){
                outfile << "x=" << k <<" y=" << i << " l=" << maze.base[i][k]->l <<" r=" << maze.base[i][k]->r <<" u=" << maze.base[i][k]->u <<" d=" << maze.base[i][k]->d << endl;
            }
        }
        mazes.push_back(maze);
        outfile.close();
    }

    cout << "All mazes are generated." << endl << endl;
    
    cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path: ";
    cin >> mazeID;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> entryX >> entryY;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitX >> exitY;
    
    
    outfile.open("maze_"+to_string(mazeID)+"_path_"+ to_string(entryX)+ "_" + to_string(entryY)+"_"+to_string(exitX)+ "_"+to_string(exitY)+ ".txt");
    for(int l = 0; l<mazes.size();l++){
        if(mazes[l].MazeID == mazeID){
            Stack<Cell> s = mazes[l].path_maze( entryX, entryY, exitX, exitY);
            vector <int> x;
            vector <int> y;
            while(s.top){
                x.push_back(s.top->y_cordinate);
                y.push_back(s.top->x_cordinate);
                s.top = s.top->path;
            }
            reverse(x.begin(), x.end());
            reverse(y.begin(), y.end());
            for(int i= 0; i<x.size();i++){
                outfile << to_string(x[i])+ " " + to_string(y[i]) << endl;
            }
        }
    }
    outfile.close();
    
    return 0;
}

