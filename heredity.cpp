
#include <vector>
#include <iostream>
#include <map>
#include <math.h>

using namespace std;

 class Point
{
public:
    int x;
    int y;
    bool operator == (const Point &p){
        return ((this->x == p.x) && (this->y == p.y));
    }
};

Point GenerateRandomPoint();
vector<Point> GenerateRandomPoints(int);
float CalculateDistanceForTwoPoints(Point,Point);
vector<Point> BuildBestPath(int n,int population_size,float survive_rate,int iteration);// n is the number of points,and the population_size set the number of lives in every generation and iteration is the number of generations, the survive rate determine the rate of lives to survive and reproduce for the next generation
vector<vector<Point>> GenerateFristGeneration(int,int,vector<Point>);
float CalculateDistancePerformence(vector<Point>);
vector<vector<Point>> ChooseWhoCanSurvive(vector<vector<Point>>,float);
vector<vector<Point>> SortByDistancePerformance(map<vector<Point>,float>);
bool cmp(const pair<vector<Point>,float>, const pair<vector<Point>,float>);
vector<vector<Point>> ReproduceNextGeneration(vector<vector<Point>>,int,int);
vector<Point> GenerateRandomLife(vector<Point>);
vector<vector<Point>> GenerateRandomlives(vector<Point>,int);
vector<Point> ReproduceChild(vector<Point>,vector<Point>);
bool CheckTwoPointsSame(Point,Point);
bool CheckPointInPoints(vector<Point>,Point);
void RandomVariation(vector<vector<Point>> &,int);
vector<vector<Point>> BuildBestPath(int n,int population_size,int iteration,int variation_rate,float survive_rate,int select_num);
void PrintPaths(vector<vector<Point>> paths);
void PrintPoints(vector<Point>);

Point GenerateRandomPoint(){
    Point point;
    point.x=rand() % 1000;
    point.y=rand() % 1000;
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
    int x_list[1000];
    int y_list[1000];
    for(i=0;i<1000;i++){
        x_list[i]=i;
        y_list[i]=i;
    }
    for(i=0;i<n;i++){
        Point point=GenerateRandomPoint();
        while((x_list[point.x] == -1 ) && (y_list[point.y] == -1)){
            point.x=(point.x+1) % 1000;
            point.y=(point.y+1) % 1000;
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

bool cmp(const pair<vector<Point>,float> &p1, const pair<vector<Point>,float> &p2){
    return p1.second < p2.second;
}

vector<vector<Point>> SortByDistancePerformance(map<vector<Point>,float> live_distance){
    // sort the lives by their distance, the begin part of the lives would survive
    vector<vector<Point>> sort_lives;// the lives that sorted by distance performance
    sort(live_distance.begin(),live_distance.end(),cmp);
    for(map<vector<Point>,float> :: iterator it=live_distance.begin();it != live_distance.end();it++){
        sort_lives.push_back(it->first);
    }
    return sort_lives;
}

vector<vector<Point>> ChooseWhoCanSurvive(vector<vector<Point>> lives, float survive_rate){
    // the return vector contains the survive lives who can reproduce the next generation
    int n = lives.size();
    int survive_num= n * survive_rate;
    map<float,vector<Point>> live_distance;
    for(int i=0;i<lives.size();i++){
        float distance_performance=CalculateDistancePerformence(lives[i]);
        live_distance.insert(make_pair(distance_performance, lives[i]));
    }
    sort(live_distance.begin(),live_distance.end());
    int i=0;
    vector<vector<Point>> survive_lives;
    for(map<float,vector<Point>>::iterator it=live_distance.begin();it != live_distance.end();++it){
        survive_lives.push_back(it->second);
        i++;
        if(i>survive_num)
            break;
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
    int father_size=parents_size/2;
    for(int i=0;i<father_size;i++){
        child.push_back(father[i]);
    }
    for(int i=0;i<parents_size;){
        if(CheckPointInPoints(child, mother[i])){
            i++;
            continue;
        }
        child.push_back(mother[i]);
        i++;
    }
    return child;
}
               
void RandomVariation(vector<vector<Point>> &lives,int variation_rate){
    int lives_size=lives.size();
    int variation_size = lives_size/variation_rate;
    for(int i=0;i<variation_size;i++){
        int random_select = rand() % lives_size;
        swap(lives[random_select][1],lives[random_select][3]);
    }
}

vector<vector<Point>> ReproduceNextGeneration(vector<vector<Point>> survive_lives,int population_size,int variation_rate)
    {
    // the function generates next generation whose size is still population, if the number of lives reproduced by parents are not enough, we will generate random lives to fill the population. It also include the process of variation
        vector<vector<Point>> children;
        int parents_size=survive_lives.size();
        if(parents_size %2 != 0){
            parents_size -= 1;
            survive_lives.pop_back();
        }
        int father_size=parents_size/2;
        vector<vector<Point>> fathers;
        vector<vector<Point>> mothers;
        for(int i=0;i<father_size;){
            fathers.push_back(survive_lives[i]);
        }
        for(int i=father_size;i<parents_size;i++)
            mothers.push_back(survive_lives[i]);
        int j=0;
        while((j<mothers.size()) && (j<fathers.size())){
            vector<Point> child = ReproduceChild(fathers[j], mothers[j]);
            children.push_back(child);
        }
        while(children.size()<population_size){
            vector<Point> child = GenerateRandomLife(survive_lives[0]);
            children.push_back(child);
        }
        //RandomVariation(children,variation_rate);
        return children;
}
    

vector<vector<Point>> GenerateFirstGeneration(int n,int population_size,vector<Point> points){
    // gernating the first generation whose population is population_size
    vector<vector<Point>> lives;
    for(int i=0;i<population_size;i++){
        vector<Point> live = GenerateRandomPoints(n);
        lives.push_back(live);
    }
    return lives;
}

vector<vector<Point>> BuildBestPath(int n,int population_size,int iteration,int variation_rate,float survive_rate,int select_num){
    vector<Point> points=GenerateRandomPoints(n);
    vector<vector<Point>> children;
    vector<vector<Point>> lives;
    lives = GenerateFirstGeneration(n, population_size, points);
    for(int i=1;i<=iteration;i++){
        children = ReproduceNextGeneration(lives, population_size, variation_rate);
        lives=children;
    }
    vector<vector<Point>> best_paths(children.begin(),children.begin()+select_num);
    return best_paths;
}

void PrintPaths(vector<vector<Point>> paths){
    for(int i=0;i<paths.size();i++){
        cout<<"the "<<i<<"th path is: ";
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
    //vector<vector<Point>> best_paths=BuildBestPath(10, 20, 90, 8, 0.2, 5);
    //PrintPaths(best_paths);
    //vector<Point> points=GenerateRandomPoints(10);
    //PrintPoints(points);
    cout<<"hello,world"<<endl;
    return 0;
}
