#include <iostream>
#include <queue>
#include <string>

using namespace std;


const int num = 9;      //number of puzzle slots, i.e. 9 slots for the 8-puzzle; allows user to easily change to bigger/smaller puzzles.
int goalPuzzle[num] = {1, 2, 3, 4, 5, 6, 7, 8, 0};      //array contents need to be updated to match the value of num whenever the user updates num.

//int defaultPuzzle[num] = {1, 2, 3, 4, 5, 6, 7, 8, 0};     //depth 0       8 different possible test cases w/ different depths
//int defaultPuzzle[num] = {1, 2, 3, 4, 5, 6, 0, 7, 8};     //depth 2
//int defaultPuzzle[num] = {1, 2, 3, 5, 0, 6, 4, 7, 8};     //depth 4
//int defaultPuzzle[num] = {1, 3, 6, 5, 0, 2, 4, 7, 8};     //depth 8
//int defaultPuzzle[num] = {1, 3, 6, 5, 0, 7, 4, 8, 2};     //depth 12
//int defaultPuzzle[num] = {1, 6, 7, 5, 0, 3, 4, 8, 2};     //depth 16
//int defaultPuzzle[num] = {7, 1, 2, 4, 8, 5, 6, 3, 0};     //depth 20
int defaultPuzzle[num] = {0, 7, 2, 4, 6, 1, 3, 5, 8};       //depth 24

int totalNodesExpanded = 0;     //total number of nodes expanded to reach goal node.
int maxNodesInQueue = 0;        //maximum number of nodes in the queue at any one time.

struct node {
    int puzzle[num];    //puzzle state
    int h_n = 0;        //h(n) value
    int g_n = 0;        //g(n) value
    int f_n = 0;        //f(n) value
    int up = 0;         //"move up" operator
    int down = 0;       //"move down" operator
    int left = 0;       //"move left" operator
    int right = 0;      //"move right" operator
    int g_nTracker = 0; //used to update a node's cost g(n)
    bool operator < (const node &test) const {  //used to implement the priority queue in GSA, node's w/ lowest f(n) value at front of queue
        return f_n > test.f_n;
    }
    int parentPuzzle[num] = {9, 9, 9, 9, 9, 9, 9, 9, 9};    //keeps track of a node's parent's puzzle state
};

vector<node> childrenNode;  //stores child nodes of a node being expanded in the expand function.


bool goalTest(int nodePuzzle[]) {       //tests if current node's puzzle matches goal puzzle.
    for (int i = 0; i < num; ++i) {
        if (nodePuzzle[i] != goalPuzzle[i]) {
            return false;
        }
    }
    return true;
}

void expand(node parentNode) {          //expand's a given node - must slightly update if num is updated (different operator and swap values).
    int blank;                          //keeps track of what position the blank spot is in.
    
    if (parentNode.puzzle[0] == 0) {    //indicate the operators that are available for parentNode based on where blank position is.
        parentNode.up = 0;              //blank top left
        parentNode.down = 1;
        parentNode.left = 0;
        parentNode.right = 1;
        blank = 0;
    }
    else if (parentNode.puzzle[1] == 0) {   //blank top middle
        parentNode.up = 0;
        parentNode.down = 1;
        parentNode.left = 1;
        parentNode.right = 1;
        blank = 1;
    }
    else if (parentNode.puzzle[2] == 0) {   //blank top right
        parentNode.up = 0;
        parentNode.down = 1;
        parentNode.left = 1;
        parentNode.right = 0;
        blank = 2;
    }
    else if (parentNode.puzzle[3] == 0) {   //blank middle left
        parentNode.up = 1;
        parentNode.down = 1;
        parentNode.left = 0;
        parentNode.right = 1;
        blank = 3;
    }
    else if (parentNode.puzzle[4] == 0) {   //blank middle
        parentNode.up = 1;
        parentNode.down = 1;
        parentNode.left = 1;
        parentNode.right = 1;
        blank = 4;
    }
    else if (parentNode.puzzle[5] == 0) {   //blank middle right
        parentNode.up = 1;
        parentNode.down = 1;
        parentNode.left = 1;
        parentNode.right = 0;
        blank = 5;
    }
    else if (parentNode.puzzle[6] == 0) {   //blank bottom left
        parentNode.up = 1;
        parentNode.down = 0;
        parentNode.left = 0;
        parentNode.right = 1;
        blank = 6;
    }
    else if (parentNode.puzzle[7] == 0) {   //blank bottom middle
        parentNode.up = 1;
        parentNode.down = 0;
        parentNode.left = 1;
        parentNode.right = 1;
        blank = 7;
    }
    else if (parentNode.puzzle[8] == 0) {   //blank bottom right
        parentNode.up = 1;
        parentNode.down = 0;
        parentNode.left = 1;
        parentNode.right = 0;
        blank = 8;
    }
    
    int i;
    
    node childNode = parentNode;        //childNode that represents one of the possible puzzle states based on
                                        //the operators available for the current parentNode
    ++childNode.g_nTracker;
    for (i = 0; i < num; ++i) {         //stores a node's parent's puzzle
        childNode.parentPuzzle[i] = parentNode.puzzle[i];
    }
    
    if (childNode.up == 1) {            //swap blank spot with puzzle piece above the blank spot's position.
        childNode.puzzle[blank] = parentNode.puzzle[blank - 3];
        childNode.puzzle[blank - 3] = 0;
        childNode.g_n = childNode.g_nTracker;
        for (i = 0; i < num; ++i) {
            if (childNode.puzzle[i] != parentNode.parentPuzzle[i]) {    //if childNode's puzzle is equal to its parent's parent's puzzle node
                childrenNode.push_back(childNode);                      //then it is a duplicate node that should not be pushed to the queue
                break;
            }
        }
    }

    childNode = parentNode;     //reinitialize childNode for next possible operator
    ++childNode.g_nTracker;
    for (i = 0; i < num; ++i) {
        childNode.parentPuzzle[i] = parentNode.puzzle[i];
    }
    
    if (childNode.down == 1) {          //swap blank spot with puzzle piece below the blank spot's position.
        childNode.puzzle[blank] = parentNode.puzzle[blank + 3];
        childNode.puzzle[blank + 3] = 0;
        childNode.g_n = childNode.g_nTracker;
        for (i = 0; i < num; ++i) {
            if (childNode.puzzle[i] != parentNode.parentPuzzle[i]) {    //if childNode's puzzle is equal to its parent's parent's puzzle node
                childrenNode.push_back(childNode);                      //then it is a duplicate node that should not be pushed to the queue
                break;
            }
        }
    }

    childNode = parentNode;     //reinitialize childNode for next possible operator
    ++childNode.g_nTracker;
    for (i = 0; i < num; ++i) {
        childNode.parentPuzzle[i] = parentNode.puzzle[i];
    }
    
    if (childNode.left == 1) {          //swap blank spot with puzzle piece to the left of the blank spot's position.
        childNode.puzzle[blank] = parentNode.puzzle[blank - 1];
        childNode.puzzle[blank - 1] = 0;
        childNode.g_n = childNode.g_nTracker;
        for (i = 0; i < num; ++i) {
            if (childNode.puzzle[i] != parentNode.parentPuzzle[i]) {    //if childNode's puzzle is equal to its parent's parent's puzzle node
                childrenNode.push_back(childNode);                      //then it is a duplicate node that should not be pushed to the queue
                break;
            }
        }
    }
    
    childNode = parentNode;     //reinitialize childNode for next possible operator
    ++childNode.g_nTracker;
    for (i = 0; i < num; ++i) {
        childNode.parentPuzzle[i] = parentNode.puzzle[i];
    }
    
    if (childNode.right == 1) {         //swap blank spot with puzzle piece to the right of the blank spot's position.
        childNode.puzzle[blank] = parentNode.puzzle[blank + 1];
        childNode.puzzle[blank + 1] = 0;
        childNode.g_n = childNode.g_nTracker;
        for (i = 0; i < num; ++i) {
            if (childNode.puzzle[i] != parentNode.parentPuzzle[i]) {    //if childNode's puzzle is equal to its parent's parent's puzzle node
                childrenNode.push_back(childNode);                      //then it is a duplicate node that should not be pushed to the queue
                break;
            }
        }
    }
    
    return;
}

void uniformCostSearch() {      //assigns a node's g(n) value to its f(n) value; h(n) = 0 in Uniform Cost Search
    for (int i = 0; i < childrenNode.size(); ++i) {
        childrenNode[i].f_n = childrenNode[i].g_n;
    }
    return;
}

void aStarMisplaceTile() {      //Increments a node's h(n) for every misplaced tile. Returns childrenNode with updated f(n) values.
    for (int h = 0; h < childrenNode.size(); ++h) {     //reinitializes child nodes' h(n) values
        childrenNode[h].h_n = 0;
    }
    for (int i = 0; i < childrenNode.size(); ++i) {     //increment h(n) by 1 for each misplaced tile
        for (int j = 0; j < num - 1; ++j) {
            if (childrenNode[i].puzzle[j] != j + 1) {
                ++childrenNode[i].h_n;
            }
        }
    }
    for (int k = 0; k < childrenNode.size(); ++k) {
        childrenNode[k].f_n = childrenNode[k].g_n + childrenNode[k].h_n;    //f(n) = g(n) + h(n)
    }
    return;
}

void aStarManhattanDistance() {     //Manhattan Distance calculated by: |(x.start - x.goal)| + |(y.start - y.goal)|
    int xStart;     //starting x-coordinate position
    int yStart;     //starting y-coordinate position
    int xGoal;      //goal x-coordinate position
    int yGoal;      //goal y-coordinate position
    int x;          //stores x.start - x.goal
    int y;          //stores y.start - y.goal
    
    for (int h = 0; h < childrenNode.size(); ++h) {     //reinitialize h(n) value for each node in childrenNode
        childrenNode[h].h_n = 0;
    }
    for (int i = 0; i < childrenNode.size(); ++i) {     //check each node in childrenNode
        for (int j = 0; j < num - 1; ++j) {             //check each of these nodes' puzzles tile by tile
            if (childrenNode[i].puzzle[j] != j + 1) {   //if a tile is misplaced, need to store starting position and goal position; j = misplaced tile's value
                if (j == 0) {       //goal tile top left
                    xGoal = 0;
                    yGoal = 2;
                }
                else if (j == 1) {  //goal tile top middle
                    xGoal = 1;
                    yGoal = 2;
                }
                else if (j == 2) {  //goal tile top right
                    xGoal = 2;
                    yGoal = 2;
                }
                else if (j == 3) {  //goal tile middle left
                    xGoal = 0;
                    yGoal = 1;
                }
                else if (j == 4) {  //goal tile middle
                    xGoal = 1;
                    yGoal = 1;
                }
                else if (j == 5) {  //goal tile middle right
                    xGoal = 2;
                    yGoal = 1;
                }
                else if (j == 6) {  //goal tile bottom left
                    xGoal = 0;
                    yGoal = 0;
                }
                else if (j == 7) {  //goal tile bottom middle
                    xGoal = 1;
                    yGoal = 0;
                }
                else {              //goal tile bottom right
                    xGoal = 2;
                    yStart = 0;
                }
                for (int k = 0; k < num - 1; ++k) {     //find where misplaced tile is
                    if (childrenNode[i].puzzle[k] == j && k == 0) {         //starting tile top left
                        xStart = 0;
                        yStart = 2;
                    }
                    else if (childrenNode[i].puzzle[k] == j && k == 1) {    //starting tile top middle
                        xStart = 1;
                        yStart = 2;
                    }
                    else if (childrenNode[i].puzzle[k] == j && k == 2) {    //starting tile top right
                        xStart = 2;
                        yStart = 2;
                    }
                    else if (childrenNode[i].puzzle[k] == j && k == 3) {    //starting tile middle left
                        xStart = 0;
                        yStart = 1;
                    }
                    else if (childrenNode[i].puzzle[k] == j && k == 4) {    //starting tile middle
                        xStart = 1;
                        yStart = 1;
                    }
                    else if (childrenNode[i].puzzle[k] == j && k == 5) {    //starting tile middle right
                        xStart = 2;
                        yStart = 1;
                    }
                    else if (childrenNode[i].puzzle[k] == j && k == 6) {    //starting tile bottom left
                        xStart = 0;
                        yStart = 0;
                    }
                    else if (childrenNode[i].puzzle[k] == j && k == 7) {    //starting tile bottom middle
                        xStart = 1;
                        yStart = 0;
                    }
                    else {              //starting tile bottom right
                        xStart = 2;
                        yStart = 0;
                    }
                }
                x = xStart - xGoal;
                if (x < 0) {
                    x = x * -1;
                }
                y = yStart - yGoal;
                if (y < 0) {
                    y = y * -1;
                }
                childrenNode[i].h_n = childrenNode[i].h_n + x + y;  //|(x.start - x.goal)| + |(y.start - y.goal)|
            }
        }
    }
    for (int a = 0; a < childrenNode.size(); ++a) {
        childrenNode[a].f_n = childrenNode[a].g_n + childrenNode[a].h_n;    //f(n) = g(n) + h(n)
    }
    return;
}
    
node generalSearch(node problem, string queueFunction) { //queueFunction = algorithm
    priority_queue<node> nodes;          //nodes = MAKE-QUEUE(MAKE-NODE(problem.INITIAL-STATE)) 
    nodes.push(problem);
    int vectorSize;             //used to store childrenNode.size()
    node currNode;
    while(1) {                  //loop do
        if (nodes.empty()) {    //if EMPTY(nodes) then return "failure" (we have proved there is no solution!)
            cout << "Failure";
            return problem;
        }
        
        if (maxNodesInQueue < nodes.size()) {   //store max number of nodes in queue
            maxNodesInQueue = nodes.size();
        }
        currNode = nodes.top();     //node = REMOVE-FRONT(nodes) 
        nodes.pop();
        ++totalNodesExpanded;       //increment number of nodes expanded
        
        cout << "The best state to expand with g(n) = " << currNode.g_n << " and h(n) = " << currNode.h_n << " is:" << endl
        << currNode.puzzle[0] << " " << currNode.puzzle[1] << " " << currNode.puzzle[2] << endl
        << currNode.puzzle[3] << " " << currNode.puzzle[4] << " " << currNode.puzzle[5] << endl
        << currNode.puzzle[6] << " " << currNode.puzzle[7] << " " << currNode.puzzle[8] << endl << "Expanding this node..." << endl;
        
        if (goalTest(currNode.puzzle)) {    // if problem.GOAL-TEST(node.STATE) succeeds then return node
            cout << "Goal!" << endl << endl;
            return currNode;
        }
        expand(currNode);
        if (queueFunction == "uniformCostSearch") {   //    nodes = QUEUEING-FUNCTION(nodes, EXPAND(node, problem.OPERATORS))  
            uniformCostSearch();
        }
        else if (queueFunction == "aStarMisplaceTile") {
            aStarMisplaceTile();
        }
        else if (queueFunction == "aStarManhattanDistance") {
            aStarManhattanDistance();
        }
        vectorSize = childrenNode.size();
        for (int i = vectorSize - 1; i >= 0; --i) {     //store nodes from childrenNode into the queue; empty childrenNode
            nodes.push(childrenNode.back());
            childrenNode.pop_back();
        }
    }
}           


int main()
{
    int userInput = 0;      //stores user input
    string algorithm;       //stores selected algorithm
    node currNode;          //stores default puzzle or custom puzzle
    node goalNode;          //stores the node returned from general search algorithm
    
    cout << "Welcome to the 8-Puzzle Solver" << endl << "Type “1” to use a default puzzle, or “2” to enter your own puzzle. ";
    
    cin >> userInput;
    if (userInput == 1) {           //default puzzle
        for (int i = 0; i < num; ++i) {
            currNode.puzzle[i] = defaultPuzzle[i];
        }
    }
    else if (userInput == 2) {      //custom puzzle; for loops need to be updated whenever num is updated;
        int j = 0;                  //a 15-puzzle, for example, would require 4 numbers per row.
        cout << "Enter your puzzle, use a zero to represent the blank" << endl;
        cout << "Enter the first row, one number at a time" << endl;
        for (j = 0; j < 3; ++j) {
            cin >> userInput;
            currNode.puzzle[j] = userInput;
        }
        cout << "Enter the second row, one number at a time" << endl;
        for (j = 3; j < 6; ++j) {
            cin >> userInput;
            currNode.puzzle[j] = userInput;
        }
        cout << "Enter the third row, one number at a time" << endl;
        for (j = 6; j < 9; ++j) {
            cin >> userInput;
            currNode.puzzle[j] = userInput;
        }
    }
    
    cout << "Enter your choice of algorithm" << endl
    << "1. Uniform Cost Search" << endl
    << "2. A* with the Misplaced Tile heuristic" << endl 
    << "3. A* with the Manhattan distance heuristic" << endl;
    
    cin >> userInput;   //get selected algorithm
    
    if (userInput == 1) {
        algorithm = "uniformCostSearch";
    }
    else if (userInput == 2) {
        algorithm = "aStarMisplaceTile";
    }
    else if (userInput == 3) {
        algorithm = "aStarManhattanDistance";
    }
    else {
        cout << "Unspecified algorithm; Uniform Cost Search set as default algorithm";      //for any input other than 1, 2, or 3
        algorithm = "uniformCostSearch";
    }
    
    goalNode = generalSearch(currNode, algorithm);
    
    cout << "To solve this problem using " << algorithm << ", the search algorithm expanded a total of " << totalNodesExpanded << " nodes." << endl;
    cout << "The maximum number of nodes in the queue at any one time was " << maxNodesInQueue << "." << endl;
    cout << "The depth of the goal node was " << goalNode.g_n << "." << endl;

    return 0;
}

