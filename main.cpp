#include "/Users/huron/stdc++.h"

using namespace std;

class Point
{
public:
    int x;
    int y;
    /*bool operator == (const Point &p){
        return ((this->x == p.x) && (this->y == p.y));
    }*/
};
typedef pair<float,vector<Point>> PAIR;
struct cmp{
    bool operator()(const PAIR& P1,const PAIR& P2){
        return P1.first<P2.first;
    }
};

Point GenerateRandomPoint();
float AverageDistance(vector<vector<Point>>);
vector<Point> GenerateRandomPoints(int);
float CalculateDistanceForTwoPoints(Point,Point);
vector<Point> BuildBestPath(int n,int population_size,float survive_rate,int iteration);// n is the number of points,and the population_size set the number of lives in every generation and iteration is the number of generations, the survive rate determine the rate of lives to survive and reproduce for the next generation
vector<vector<Point> > GenerateFristGeneration(int,int,vector<Point>);
float CalculateDistancePerformence(vector<Point>);
vector<vector<Point> > ChooseWhoCanSurvive(vector<vector<Point>>,float);
vector<vector<Point> > SortByDistancePerformance(map<vector<Point>,float>);
bool cmp(const pair<vector<Point>,float>, const pair<vector<Point>,float>);
vector<vector<Point> > ReproduceNextGeneration(vector<vector<Point>>,int,float);
vector<Point> GenerateRandomLife(vector<Point>);
vector<vector<Point> > GenerateRandomlives(vector<Point>,int);
vector<Point> ReproduceChild(vector<Point>,vector<Point>);
bool CheckTwoPointsSame(Point,Point);
bool CheckPointInPoints(vector<Point>,Point);
void RandomVariation(vector<vector<Point>> &,int);
vector<vector<Point> > BuildBestPath(int n,int population_size,int iteration,float variation_rate,int select_num);
void PrintPaths(vector<vector<Point> > paths);
void PrintPoints(vector<Point>);
int FindMinIndex(vector<float>&);
float CalculateSurviveRate(int);
void RandomVariation(vector<vector<Point>> &,float);
void Variation(vector<Point> &);

Point GenerateRandomPoint(){
    Point point;
    point.x=rand() % 100;
    point.y=rand() % 100;
    return point;
}

void PrintPoint(Point point){
    cout<<"( "<<point.x<<","<<point.y<<" )";
}

bool IsTwoPointsSame(Point point_1,Point point_2){
    if((point_1.x == point_2.x) && (point_1.y == point_2.y))
        return true;
    return false;
}

vector<Point> GenerateRandomPoints(int n){
    // generate n random points using GenerateRandomPoint
    // in order to avoid generating same points, we use x_list and y_list to record previous points
    vector<Point> points;
    int  i;
    int x_list[100];
    int y_list[100];
    for(i=0;i<100;i++){
        x_list[i]=i;
        y_list[i]=i;
    }
    for(i=0;i<n;i++){
        Point point=GenerateRandomPoint();
        while((x_list[point.x] == -1 ) && (y_list[point.y] == -1)){
            point.x=(point.x+1) % 100;
            point.y=(point.y+1) % 100;
        }
        points.push_back(point);
        x_list[point.x] = -1;
        y_list[point.y] = -1;
    }
    return points;
}

float CalculateDistanceForTwoPoints(Point point_1,Point point_2){
    // calculate the distance for two points
    float distance=sqrt(pow(point_1.x-point_2.x,2) + pow(point_1.y-point_2.y,2));
    return distance;
}

float CalculateDistancePerformence(vector<Point> life){
    // calculate the distance performance for one life. Obviously, the smaller the distance, the better the performance.
    float total_distance=0;
    for(int i=0;i<life.size()-1;i++){
        total_distance += CalculateDistanceForTwoPoints(life[i], life[i+1]);
    }
    return total_distance;
}

/*bool cmp(const pair<vector<Point>,float> &p1, const pair<vector<Point>,float> &p2){
    return p1.second < p2.second;
}*/
// some problems in the following two functions
 /*vector<vector<Point>> SortByDistancePerformance(map<vector<Point>,float> live_distance){
    // sort the lives by their distance, the begin part of the lives would survive
    vector<vector<Point>> sort_lives;// the lives that sorted by distance performance
    sort(live_distance.begin(),live_distance.end(),cmp);
    for(map<vector<Point>,float> :: iterator it=live_distance.begin();it != live_distance.end();it++){
        sort_lives.push_back(it->first);
    }
    return sort_lives;
}*/

/*vector<vector<Point>> SortedByDistance(map<float,vector<Point>> live_distane,int survive_num){
    vector<vector<Point>> lives_sorted_by_distance;
    int num=0;
    float min=10000000000000;
    while(num<survive_num){
        int min_index;
        for(int i=0;i<live_distane.size();i++){
            if(live_distane[i].first<min)
        }
    }
}*/

int FindMinIndex(vector<float> &distances){
    int min_index=0;
    int min_value=INT_MAX;
    for(int i=0;i<distances.size();i++){
        if(distances[i]<min_value){
            min_value=distances[i];
            min_index=i;
        }
    }
    return min_index;
}

vector<vector<Point>> ChooseWhoCanSurvive(vector<vector<Point>> lives, float survive_rate){
    // the return vector contains the survive lives who can reproduce the next generation
    int n = lives.size();
    int survive_num=n * survive_rate;
    vector<vector<Point>> survive_lives;
    vector<float> distances;
    for(int i=0;i<lives.size();i++){
        float distance=CalculateDistancePerformence(lives[i]);
        distances.push_back(distance);
    }
    int num=0;
    while(num <= survive_num){
        int min_index=FindMinIndex(distances);
        distances.erase(distances.begin()+min_index);
        survive_lives.push_back(lives[min_index]);
        lives.erase(lives.begin()+min_index);
        num++;
    }
    return survive_lives;
}

vector<Point> GenerateRandomLife(vector<Point> points){
    random_shuffle(points.begin(),points.end());
    vector<Point> life=points;
    return life;
}

vector<vector<Point>> GenerateRandomlives(vector<Point> points,int population_size){
    vector<vector<Point>> lives;
    for(int i=0;i<population_size;i++){
        vector<Point> life=GenerateRandomLife(points);
        lives.push_back(life);
    }
    return lives;
}

bool CheckTwoPointsSame(Point point_1,Point point_2){
    return ((point_1.x == point_2.x) && (point_1.y == point_2.y));
}

bool CheckPointInPoints(vector<Point> points,Point point){
    for(int i=0;i<points.size();i++){
        if(CheckTwoPointsSame(point, points[i]))
            return true;
    }
    return false;
}

vector<Point> ReproduceChild(vector<Point> father,vector<Point> mother){
    vector<Point> child;
    int parents_size=father.size();
    int mother_size=mother.size();
    int father_size=parents_size/2;
    for(int i=0;i<father_size;i++){
        child.push_back(father[i]);
    }
    for(int i=0;i<mother_size;){
        if(CheckPointInPoints(child, mother[i])){
            i++;
            continue;
        }
        child.push_back(mother[i]);
        i++;
    }
    if(child.size() != parents_size)
        cout<<"here is an error"<<endl;
    return child;
}

void Variation(vector<Point> &life){
    int size = life.size();
    int random_index_1 = rand() % size;
    int random_index_2 = rand() % size;
    Point temp = life[random_index_2];
    life[random_index_2] = life[random_index_1];
    life[random_index_1] = temp;
}
               
void RandomVariation(vector<vector<Point>> &lives,float variation_rate){
    int lives_size=lives.size();
    int variation_num = lives_size * variation_rate;
    for(int i=0;i<variation_num;i++){
        int variation_index = rand() % lives_size;
        Variation(lives[variation_index]);
    }
}

vector<vector<Point>> ReproduceNextGeneration(vector<vector<Point>> survive_lives,int population_size,float variation_rate)
    {
    // the function generates next generation whose size is still population, if the number of lives reproduced by parents are not enough, we will generate random lives to fill the population. It also include the process of variation
        vector<vector<Point>> children;
        int parents_size=survive_lives.size();
        
        int i=0;
        while((children.size()<=population_size) && (i<survive_lives.size())){
            for(int j=0;j<survive_lives.size();j++){
                vector<Point> child = ReproduceChild(survive_lives[i], survive_lives[j]);
                children.push_back(child);
            }
            i++;
        }
        
        while(children.size()<population_size){
            vector<Point> child = GenerateRandomLife(survive_lives[0]);
            children.push_back(child);
        }
        //cout<<"I am here 3"<<endl;
        
        RandomVariation(children,variation_rate);
        return children;
}

float AverageDistance(vector<vector<Point>> lives){
    int size = lives.size();
    float total_distance = 0;
    for(int i=0;i<size;i++){
        total_distance += CalculateDistancePerformence(lives[i]);
    }
    return total_distance/size;
}

vector<vector<Point>> GenerateFirstGeneration(int n,int population_size,vector<Point> points){
    // gernating the first generation whose population is population_size
    vector<vector<Point>> lives;
    lives=GenerateRandomlives(points, population_size);
    return lives;
}

float CalculateSurviveRate(int n){
    float survive_rate = 1/sqrt(n);
    return survive_rate;
}

vector<vector<Point>> BuildBestPath(int n,int population_size,int iteration,float variation_rate,int select_num){
    float survive_rate = CalculateSurviveRate(n);
    vector<Point> points=GenerateRandomPoints(n);
    vector<vector<Point>> children;
    vector<vector<Point>> lives;
    lives = GenerateFirstGeneration(n, population_size, points);
    //PrintPoints(points);
    //PrintPaths(lives);
    for(int i=1;i<=iteration;i++){
        vector<vector<Point>> survive_lives = ChooseWhoCanSurvive(lives, survive_rate);
        children = ReproduceNextGeneration(survive_lives, population_size, variation_rate);
        lives=children;
        
        if(i<20){
            cout<<"The average distance for "<<i<<"th is " <<AverageDistance(children)<<endl;
        }
    }
    vector<vector<Point>> best_paths(children.begin(),children.begin()+select_num);
    return best_paths;
}

void PrintPaths(vector<vector<Point>> paths){
    for(int i=0;i<paths.size();i++){
        cout<<"the "<<i<<"th path is: "<<endl;
        for(int j=0;j<paths[i].size()-1;j++){
            cout<<"( "<<paths[i][j].x<<","<<paths[i][j].y<<" )"<<endl;
        }
    }
}


void PrintPoints(vector<Point> points){
    for(int i=0;i<points.size();i++){
        cout<<"( "<<points[i].x<<","<<points[i].y<<" )"<<endl;
    }
}


int main(int argc, const char * argv[]) {
    vector<vector<Point>> best_paths;
    best_paths=BuildBestPath(80, 1000, 100, 0.1, 4);
    cout<<endl;
    //PrintPoints(best_paths[0]);
    cout<<endl;
    cout<<"the final distance is "<<AverageDistance(best_paths)<<endl;
    /*for(int i=0;i<best_paths.size();i++){
        float distance=CalculateDistancePerformence(best_paths[i]);
        cout<<distance<<endl;
    }*/
    /*vector<Point> points=GenerateRandomPoints(10);
    PrintPoints(points);
    float survive_rate = CalculateSurviveRate(10);
    cout<<CalculateDistancePerformence(points)<<endl;
    cout<<endl;
    vector<vector<Point>> lives=GenerateFirstGeneration(10, 5, points);
    PrintPaths(lives);
    vector<vector<Point>> survive_lives=ChooseWhoCanSurvive(lives, survive_rate);
    PrintPaths(survive_lives);
    vector<vector<Point>> children;
    //cout<<"the break"<<endl;
    children=ReproduceNextGeneration(lives, 5, 5);
    cout<<endl<<"the children:"<<endl;
    cout<<"I am here 1"<<endl;
    
    PrintPaths(children);
    //cout<<"hello,world"<<endl;*/
    return 0;
}
