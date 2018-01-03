#include <iostream>
#include <stack>
#include <ctime>
#include <cmath>
#include <cstdlib>
using namespace std;

struct Point
{
    int x, y;
};

// A global point needed for  sorting points with reference
// to  the first point Used in compare function of qsort()
Point p0;

// A utility function to find next to top in a stack
Point nextToTop(stack<Point> &S)
{
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}
// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) {
        float a = (r.y - p.y) * (q.x - p.x);
        float b = (q.y - p.y) * (r.x - p.x);
        if(a == b) {
            return true;
        }
    }
    return false;
}
// A utility function to swap two points
int swap(Point &p1, Point &p2)
{
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

// A utility function to return square of distance
// between p1 and p2
int distSq(Point p1, Point p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
           (p1.y - p2.y)*(p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void *vp1, const void *vp2)
{
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;

    // Find orientation
    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;

    return (o == 2)? -1: 1;
}

// Find convex hull of a set of n points using Graham Scan Algorithm.
stack<Point> convexHull(Point points[], int n)
{
    // Find the bottommost point
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++)
    {
        int y = points[i].y;

        // Pick the bottom-most or chose the left
        // most point in case of tie
        if ((y < ymin) || (ymin == y &&
                           points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }

    // Place the bottom-most point at first position
    swap(points[0], points[min]);

    // Sort n-1 points with respect to the first point.
    // A point p1 comes before p2 in sorted ouput if p2
    // has larger polar angle (in counterclockwise
    // direction) than p1
    p0 = points[0];
    qsort(&points[1], n-1, sizeof(Point), compare);

    // If two or more points make same angle with p0,
    // Remove all but the one that is farthest from p0
    // Remember that, in above sorting, our criteria was
    // to keep the farthest point at the end when more than
    // one points have same angle.
    int m = 1; // Initialize size of modified array
    for (int i=1; i<n; i++)
    {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while (i < n-1 && orientation(p0, points[i],
                                      points[i+1]) == 0)
            i++;


        points[m] = points[i];
        m++;  // Update size of modified array
    }
    stack<Point> S;
    // If modified array of points has less than 3 points,
    // convex hull is not possible
    if (m < 3) {
        for(int i = 0;i < m;i++){
            S.push(points[i]);
        }
        return S;
    }

    // Create an empty stack and push first three points
    // to it.
    //stack<Point> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);

    // Process remaining n-3 points
    for (int i = 3; i < m; i++)
    {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and points[i] makes
        // a non-left turn
        while (orientation(nextToTop(S), S.top(), points[i]) != 2)
            S.pop();
        S.push(points[i]);
    }


    return S;
}

Point* printHullPoints(stack<Point> S){
    // Now stack has the output points, print contents of stack
    Point *robots = new Point[S.size()];
    int i=0;
    while (!S.empty())
    {
        Point p = S.top();
        robots[i] = p;
        cout << "(" << p.x << ", " << p.y <<")" << endl;
        S.pop();
        i++;
    }
}

//generate random coordinate
Point generateCoord(int range){
    Point a;
    a.x = rand()% range;
    a.y = rand()% range;
    return a;
}

//return new set of points after removing hull points
Point* removeHullPoints(Point *a, Point *b, int x, int y){
    Point *robots = new Point[x];
    int flag=0, z = 0;
    for(int i = 0;i < x;i++){
        Point d = a[i];
        for(int j = 0;j<y;j++){
            if(d.x == b[j].x && d.y == b[j].y){
                flag = 1;
                break;
            }
        }
        if(flag == 1){
            //continue;
            flag = 0;
        }
        else{
            robots[z] = d;
            flag = 0;
            z++;
        }
    }
    return robots;
}

//check if neighbor
int check_neighbor(Point a, Point b){
    if(a.x == b.x + 1 && a.y == b.y){
        return 1;
    }
    else if(a.x == b.x + 1 && a.y == b.y + 1){
        return 2;
    }
    else if(a.x == b.x && a.y == b.y + 1){
        return 3;
    }
    else if(a.x == b.x - 1 && a.y == b.y + 1){
        return 4;
    }
    else if(a.x == b.x - 1 && a.y == b.y){
        return 5;
    }
    else if(a.x == b.x - 1 && a.y == b.y - 1){
        return 6;
    }
    else if(a.x == b.x && a.y == b.y - 1){
        return 7;
    }
    else if(a.x == b.x + 1 && a.y == b.y - 1){
        return 8;
    }
    else{
        return 0;
    }
}

//find nearest internal robot
Point find_nearest_int(Point a, Point* bound, int tot_b, Point* internals, int tot_int){
    Point* int_robots = new Point[tot_int];
    int k = 0, m = 0;
    for(int j = 0; j < tot_int;j++){
        Point q;
        q.x = internals[j].x;
        q.y = internals[j].y;
        m = 0;
        for(int l = 0; l < tot_b; l++){
            Point p = bound[l % tot_b];
            Point r = bound[(l+1) % tot_b];
            if(onSegment(p,q,r)){
                m = 1;
            }
        }
        if(m == 0){
            int_robots[k].x = q.x;
            int_robots[k].y = q.y;
            k++;
        }
    }
    //cout<<"Internal robots: ";
    //for(int i = 0;i<k;i++){
    //    cout<<"("<<int_robots[i].x<<", "<<int_robots[i].y<<"), ";
    //}
    //cout<<endl;

    Point near;
    near.x = int_robots[0].x;
    near.y = int_robots[0].y;
    int n1=0,n2=0,n3=0,n4=0,n5=0,n6=0,n7=0,n8=0;
    int dist = ((a.x - int_robots[0].x) * (a.x - int_robots[0].x)) + ((a.y - int_robots[0].y) * (a.y - int_robots[0].y)), dist1 = 0;
    for(int i =0;i< k; i++){
        if(i>0) {
            dist1 = ((a.x - int_robots[i].x) * (a.x - int_robots[i].x)) +
                    ((a.y - int_robots[i].y) * (a.y - int_robots[i].y));
            if (dist1 < dist) {
                near.x = int_robots[i].x;
                near.y = int_robots[i].y;
                dist = dist1;
            }
        }
        if(int_robots[i].x == a.x +1 && int_robots[i].y == a.y){
            n1=1;
        }
        else if(int_robots[i].x == a.x +1 && int_robots[i].y == a.y + 1){
            n2=1;
        }
        else if(int_robots[i].x == a.x && int_robots[i].y == a.y + 1){
            n3=1;
        }
        else if(int_robots[i].x == a.x -1 && int_robots[i].y == a.y + 1){
            n4=1;
        }
        else if(int_robots[i].x == a.x -1 && int_robots[i].y == a.y){
            n5=1;
        }
        else if(int_robots[i].x == a.x - 1 && int_robots[i].y == a.y - 1){
            n6=1;
        }
        else if(int_robots[i].x == a.x && int_robots[i].y == a.y - 1){
            n7=1;
        }
        else if(int_robots[i].x == a.x + 1 && int_robots[i].y == a.y - 1){
            n8=1;
        }
    }
    if(n1 == 1 && n3 == 1){
        near.x = a.x + 1;
        near.y = a.y + 1;
    }
    else if(n3 == 1 && n5 == 1){
        near.x = a.x - 1;
        near.y = a.y + 1;
    }
    else if(n5 == 1 && n7 == 1){
        near.x = a.x - 1;
        near.y = a.y - 1;
    }
    else if(n7 == 1 && n1 == 1){
        near.x = a.x + 1;
        near.y = a.y - 1;
    }
    return near;
}

//move
Point move(Point a, Point *b, Point *c, int tot_b, int tot_c, Point *internals, int intsize){
    int dir1 = 0, dir2 =0, dir3 = 0, dir4 = 0;
    int n1=0,n2=0,n3=0,n4=0,n5=0,n6=0,n7=0,n8=0;
    for(int i =0;i<tot_b;i++){
        //find direction to move
        if(b[i].x < a.x && b[i].y>a.y){
            dir2 = 1;
        }
        if(b[i].x > a.x && b[i].y>a.y){
            dir1 = 1;
        }
        if(b[i].x < a.x && b[i].y<a.y){
            dir3 = 1;
        }
        if(b[i].x > a.x && b[i].y<a.y){
            dir4 = 1;
        }

        //find neighbors
        if(b[i].x == a.x +1 && b[i].y == a.y){
            n1=1;
        }
        else if(b[i].x == a.x +1 && b[i].y == a.y + 1){
            n2=1;
        }
        else if(b[i].x == a.x && b[i].y == a.y + 1){
            n3=1;
        }
        else if(b[i].x == a.x -1 && b[i].y == a.y + 1){
            n4=1;
        }
        else if(b[i].x == a.x -1 && b[i].y == a.y){
            n5=1;
        }
        else if(b[i].x == a.x - 1 && b[i].y == a.y - 1){
            n6=1;
        }
        else if(b[i].x == a.x && b[i].y == a.y - 1){
            n7=1;
        }
        else if(b[i].x == a.x + 1 && b[i].y == a.y - 1){
            n8=1;
        }
    }
    for(int i =0;i<intsize;i++){
        if(internals[i].x == a.x +1 && internals[i].y == a.y){
            n1=1;
        }
        else if(internals[i].x == a.x +1 && internals[i].y == a.y + 1){
            n2=1;
        }
        else if(internals[i].x == a.x && internals[i].y == a.y + 1){
            n3=1;
        }
        else if(internals[i].x == a.x -1 && internals[i].y == a.y + 1){
            n4=1;
        }
        else if(internals[i].x == a.x -1 && internals[i].y == a.y){
            n5=1;
        }
        else if(internals[i].x == a.x - 1 && internals[i].y == a.y - 1){
            n6=1;
        }
        else if(internals[i].x == a.x && internals[i].y == a.y - 1){
            n7=1;
        }
        else if(internals[i].x == a.x + 1 && internals[i].y == a.y - 1){
            n8=1;
        }
    }

    if(n1==1 && n5 == 1){
        if(dir1 == 1 || dir2 == 1){
            a.y = a.y - 1;
        }
        else if(dir3 == 1 || dir4 == 1){
            a.y = a.y + 1;
        }
    }
    else if(n3==1 && n7 == 1){
        if(dir1 == 1 || dir4 == 1){
            a.x = a.x - 1;
        }
        else if(dir2 == 1 || dir3 == 1){
            a.x = a.x + 1;
        }
    }
    else if(n2 == 1 && n6 == 1){
        if(dir2 == 0){
            a.x = a.x - 1;
            a.y = a.y + 1;
        }
        else if(dir4 == 0){
            a.x = a.x + 1;
            a.y = a.y - 1;
        }
    }
    else if(n4 == 1 && n8 == 1){
        if(dir1 == 0){
            a.x = a.x + 1;
            a.y = a.y + 1;
        }
        else if(dir3 == 0){
            a.x = a.x - 1;
            a.y = a.y - 1;
        }
    }
    else{
        Point near = find_nearest_int(a, b, tot_b, internals, intsize);
        //cout<<"Nearest internal of ("<<a.x<<", "<<a.y<<") is ("<<near.x<<", "<<near.y<<")"<<endl;
        //getchar();
        int neighbor = check_neighbor(near, a);
        //cout<<"nbr = "<<neighbor;
        if(neighbor == 1){
            a.x = a.x -1;
        }
        else if(neighbor == 2){
            a.x = a.x - 1;
            a.y = a.y - 1;
        }
        else if(neighbor == 3){
            a.y = a.y - 1;
        }
        else if(neighbor == 4){
            a.x = a.x + 1;
            a.y = a.y - 1;
        }
        else if(neighbor == 5){
            a.x = a.x + 1;
        }
        else if(neighbor == 6){
            a.x = a.x + 1;
            a.y = a.y + 1;
        }
        else if(neighbor == 7){
            a.y = a.y + 1;
        }
        else if(neighbor == 8){
            a.x = a.x - 1;
            a.y = a.y + 1;
        }
        else if(neighbor == 0){
            int tot_neighbors = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8;
            if(tot_neighbors>=2){
                if(n1 == 1 & n6 == 1){
                    a.y = a.y + 1;
                }
                else if(n1 == 1 && n4 == 1){
                    a.y = a.y - 1;
                }
                else if(n1 == 1 && n7 == 1){
                    a.x = a.x - 1;
                    a.y = a.y + 1;
                }
                else if(n1 == 1 && n3 == 1){
                    a.x = a.x - 1;
                    a.y = a.y - 1;
                }
                else if(n5 == 1 & n8 == 1){
                    a.y = a.y + 1;
                }
                else if(n5 == 1 && n2 == 1){
                    a.y = a.y - 1;
                }
                else if(n5 == 1 && n7 == 1){
                    a.x = a.x + 1;
                    a.y = a.y + 1;
                }
                else if(n5 == 1 && n3 == 1){
                    a.x = a.x + 1;
                    a.y = a.y - 1;
                }
                else if(n3 == 1 && n8 == 1){
                    a.x = a.x - 1;
                }
                else if(n3 == 1 && n6 == 1){
                    a.x = a.x + 1;
                }
                else if(n7 == 1 && n2 == 1){
                    a.x = a.x - 1;
                }
                else if(n7 == 1 && n4 == 1){
                    a.x = a.x + 1;
                }
                else if(n1 == 1 && n2 == 1){
                    a.x = a.x - 1;
                }
                else if(n2 == 1 && n3 == 1){
                    a.y = a.y - 1;
                }
                else if(n3 == 1 && n4 == 1){
                    a.y = a.y - 1;
                }
                else if(n4 == 1 && n5 == 1){
                    a.x = a.x + 1;
                }
                else if(n5 == 1 && n6 == 1){
                    a.x = a.x + 1;
                }
                else if(n6 == 1 && n7 == 1){
                    a.y = a.y + 1;
                }
                else if(n7 == 1 && n8 == 1){
                    a.y = a.y + 1;
                }
                else if(n8 == 1 && n1 == 1){
                    a.x = a.x - 1;
                }
            }
        }
    }

    return a;
}
int main() {
    int count = 0, tot_round = 0, tot_h = 0, totalrobots = 0, n=0, tests = 0,space = 1;
    cout<<"----------------------------------------------------------------------"<<endl<<endl;
    std::cout << "\tTIME ANALYSIS OF COMPLETE VISITABILITY PROBLEM " << std::endl<<std::endl;
    cout<<"----------------------------------------------------------------------"<<endl<<endl;
    std::cout << "Enter the number of robots: ";
    std::cin >> totalrobots;
    again:std::cout << "Select the grid space for robot configuration:"<<endl;
    std::cout << "1. Compact square (sqrt(n) * sqrt(n))"<<endl;
    std::cout << "2. Tight square ((sqrt(n)*2) * (sqrt(n)*2))"<<endl;
    std::cout << "3. Loose square ((sqrt(n)*3) * (sqrt(n)*3))"<<endl;
    std::cout << "Choose Option (1, 2 or 3): ";
    std::cin >> space;
    if(space !=1 && space !=2 && space !=3){
        std::cout<<"Wrong choice"<<endl;
        goto again;
    }
    std::cout << "\nHow many tests do you want to perform: ";
    std::cin >> tests;
    cout<<"----------------------------------------------------------------------"<<endl;
    cout<<"\nTotal number of robots: "<<totalrobots<<endl<<endl;
    cout <<"Tests \t\tConvex hull layers (h)\t\tRounds (Outward Wave)" << endl;
    cout<<"----------------------------------------------------------------------"<<endl;
    srand(time(0));


    while(count < tests) {
        n = totalrobots;
        int h = 0;
        int *totalPoints = new int[n];
        int *internalsize = new int[n];


        int range = (int)(ceil (sqrt(n))) * space;
        Point *robots = new Point[n];
        Point **ch_layers = new Point *[n];
        Point **internals = new Point *[n];
        for (int i = 0; i < n; i++) {
            ch_layers[i] = new Point[n];
            internals[i] = new Point[n];
            totalPoints[i] = 0;
        }
        //cout << "The robots points are:" << endl;
        for (int i = 0; i < n; i++) {
            label:
            Point newpoint = generateCoord(range);
            for (int j = i - 1; j >= 0; j--) {
                if (robots[j].x == newpoint.x && robots[j].y == newpoint.y) {
                    goto label;
                }
            }
            robots[i].x = newpoint.x;
            robots[i].y = newpoint.y;
            //cout << "(" << robots[i].x << ", " << robots[i].y << ") ";
        }
        //cout << endl;

        int isize = totalrobots;
        internalsize[0] = isize;
        while (n > 0) {

            //Point *oldrobots = new Point[n];
            internals[h] = new Point[n];
            for (int i = 0; i < n; i++) {
                //oldrobots[i].x = robots[i].x;
                //oldrobots[i].y = robots[i].y;
                internals[h][i].x = robots[i].x;
                internals[h][i].y = robots[i].y;
            }

            //cout << "The convex hull points are (out of total " << n << "): Layer " << h << endl;

            int h_count = 0;

            stack<Point> s = convexHull(robots, n);

            while (!s.empty()) {
                Point p = s.top();

                ch_layers[h][h_count].x = p.x;
                ch_layers[h][h_count].y = p.y;
                s.pop();
                h_count++;
            }
            //for (int i = 0; i < h_count; i++) {
            //std::cout << "(" << ch_layers[h][i].x << ", " << ch_layers[h][i].y << ")";
            //}
            isize = isize - h_count;
            internalsize[h + 1] = isize;
            totalPoints[h] = h_count;
            //cout << endl;

            Point *newrobots = new Point[n - h_count];
            int flag = 0, k = 0;
            for (int i = 0; i < n; i++) {
                //Point p = oldrobots[i];
                Point p = internals[h][i];
                for (int j = 0; j < h_count; j++) {
                    Point q = ch_layers[h][j];
                    if (p.x == q.x && p.y == q.y) {
                        flag = 1;
                        //break;
                    }
                }
                if (flag == 1) {
                    flag = 0;
                } else {
                    newrobots[k].x = p.x;
                    newrobots[k].y = p.y;
                    flag = 0;
                    k++;
                }
            }
            //if((n - h_count) > 0) {
            //    cout << "New set of points:" << endl;
            //}
            robots = new Point[n - h_count];
            for (int i = 0; i < n - h_count; i++) {
                robots[i].x = newrobots[i].x;
                robots[i].y = newrobots[i].y;
                //    cout<<"("<<robots[i].x<<", "<<robots[i].y<<") ";
            }

            //cout<<endl;
            //ch_layers[x] = printHullPoints(s);
            //robots = removeHullPoints(robots, ch_layers[x],n,h_count);

            n = n - h_count;
            h++;
        }
        tot_h += h;
        //cout << endl;
        //cout << "Convex hull layers, h = " << h << endl << endl;
        //for (int i = 0; i < h; i++) {
        //    cout << "size of internal layer " << i << ": " << internalsize[i] << endl;
        //}

        //inward wave result, innermost layer terminate
        int terminate = h - 1;

        //outward wave
        int round = 0, iteration = 1;
        while (terminate > 0) {
            //  if (totalrobots <= 250)
            //      cout << "iteration " << iteration << endl;
            for (int i = 0; i < terminate; i++) {
                bool mov = false;
                //    if (totalrobots <= 250) {
                //      cout << "Before move, Layer " << i << ": ";
                //    for (int j = 0; j < totalPoints[i]; j++) {
                //      cout << "(" << ch_layers[i][j].x << ", " << ch_layers[i][j].y << ") ";
                //}
                //}
                Point* ch_layers1 = new Point[totalPoints[i]];
                for (int j = 0; j < totalPoints[i]; j++) {
                    ch_layers1[j].x = ch_layers[i][j].x;
                    ch_layers1[j].y = ch_layers[i][j].y;
                }

                for (int j = 0; j < totalPoints[i]; j++) {
                    Point move_point = move(ch_layers[i][j], ch_layers[i], ch_layers[i + 1], totalPoints[i],
                                            totalPoints[i + 1], internals[i + 1], internalsize[i + 1]);
                    if (move_point.x != ch_layers[i][j].x || move_point.y != ch_layers[i][j].y) {
                        ch_layers1[j].x = move_point.x;
                        ch_layers1[j].y = move_point.y;
                        mov = true;
                    }
                }
                for (int j = 0; j < totalPoints[i]; j++) {
                    ch_layers[i][j].x = ch_layers1[j].x;
                    ch_layers[i][j].y = ch_layers1[j].y;

                    internals[i][j].x = ch_layers1[j].x;
                    internals[i][j].y = ch_layers1[j].y;
                }
                for (int k = 0; k < internalsize[i + 1]; k++) {
                    internals[i][k + totalPoints[i]].x = internals[i + 1][k].x;
                    internals[i][k + totalPoints[i]].y = internals[i + 1][k].y;
                }
                /*
                cout << "After Move, Layer " << i << ": ";
                for (int j = 0; j < totalPoints[i]; j++) {
                    cout << "(" << ch_layers[i][j].x << ", " << ch_layers[i][j].y << ") ";
                }
                cout << endl;
                */

                if (mov) {
                    round++;
                }
                /*if (mov) {
                    round++;

                    for (int k = 0; k < totalPoints[i]; k++) {
                        internals[i][k].x = ch_layers[i][k].x;
                        internals[i][k].y = ch_layers[i][k].y;
                    }
                    for (int k = 0; k < internalsize[i + 1]; k++) {
                        internals[i][k + totalPoints[i]].x = internals[i + 1][k].x;
                        internals[i][k + totalPoints[i]].y = internals[i + 1][k].y;
                    }

                }*/
                //cout << internalsize[i + 1] << endl;
            }
            for (int i = terminate-2;i>=0;i--){
                for (int j = 0; j<totalPoints[i]; j++) {
                    internals[i][j].x = ch_layers[i][j].x;
                    internals[i][j].y = ch_layers[i][j].y;
                }
                for (int k = 0; k < internalsize[i + 1]; k++) {
                    internals[i][k + totalPoints[i]].x = internals[i + 1][k].x;
                    internals[i][k + totalPoints[i]].y = internals[i + 1][k].y;
                }
            }

            terminate--;
            iteration++;
            // if (totalrobots <= 250)
            //cout << endl;
        }

        cout <<"Test "<<count+1<<"\t\t\t h = "<<h<< "\t\t\t\t" << round << endl;
        tot_round = tot_round + round;
        count++;
    }

    cout<<endl;
    cout<<"----------------------------------------------------------------------"<<endl;
    cout<<"Total rounds: \t\t\t\t\t\t"<<tot_round<<endl;
    cout<<"\nAverage of "<< tests <<" tests:\t\th = "<<tot_h/tests<<"\t\trounds = "<<(double)tot_round/(double)tests<<endl<<endl<<endl;
    return 0;
}
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
           (p1.y - p2.y)*(p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void *vp1, const void *vp2)
{
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;

    // Find orientation
    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;

    return (o == 2)? -1: 1;
}

// Find convex hull of a set of n points using Graham Scan Algorithm.
stack<Point> convexHull(Point points[], int n)
{
    // Find the bottommost point
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++)
    {
        int y = points[i].y;

        // Pick the bottom-most or chose the left
        // most point in case of tie
        if ((y < ymin) || (ymin == y &&
                           points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }

    // Place the bottom-most point at first position
    swap(points[0], points[min]);

    // Sort n-1 points with respect to the first point.
    // A point p1 comes before p2 in sorted ouput if p2
    // has larger polar angle (in counterclockwise
    // direction) than p1
    p0 = points[0];
    qsort(&points[1], n-1, sizeof(Point), compare);

    // If two or more points make same angle with p0,
    // Remove all but the one that is farthest from p0
    // Remember that, in above sorting, our criteria was
    // to keep the farthest point at the end when more than
    // one points have same angle.
    int m = 1; // Initialize size of modified array
    for (int i=1; i<n; i++)
    {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while (i < n-1 && orientation(p0, points[i],
                                      points[i+1]) == 0)
            i++;


        points[m] = points[i];
        m++;  // Update size of modified array
    }
    stack<Point> S;
    // If modified array of points has less than 3 points,
    // convex hull is not possible
    if (m < 3) {
        for(int i = 0;i < m;i++){
            S.push(points[i]);
        }
        return S;
    }

    // Create an empty stack and push first three points
    // to it.
    //stack<Point> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);

    // Process remaining n-3 points
    for (int i = 3; i < m; i++)
    {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and points[i] makes
        // a non-left turn
        while (orientation(nextToTop(S), S.top(), points[i]) != 2)
            S.pop();
        S.push(points[i]);
    }


    return S;
}

Point* printHullPoints(stack<Point> S){
    // Now stack has the output points, print contents of stack
    Point *robots = new Point[S.size()];
    int i=0;
    while (!S.empty())
    {
        Point p = S.top();
        robots[i] = p;
        cout << "(" << p.x << ", " << p.y <<")" << endl;
        S.pop();
        i++;
    }
}

//generate random coordinate
Point generateCoord(int range){
    Point a;
    a.x = rand()% range;
    a.y = rand()% range;
    return a;
}

//return new set of points after removing hull points
Point* removeHullPoints(Point *a, Point *b, int x, int y){
    Point *robots = new Point[x];
    int flag=0, z = 0;
    for(int i = 0;i < x;i++){
        Point d = a[i];
        for(int j = 0;j<y;j++){
            if(d.x == b[j].x && d.y == b[j].y){
                flag = 1;
                break;
            }
        }
        if(flag == 1){
            //continue;
            flag = 0;
        }
        else{
            robots[z] = d;
            flag = 0;
            z++;
        }
    }
    return robots;
}

//check if neighbor
int check_neighbor(Point a, Point b){
    if(a.x==b.x + 1 && a.y == b.y){
        return 1;
    }
    else if(a.x==b.x + 1 && a.y == b.y + 1){
        return 2;
    }
    else if(a.x==b.x && a.y == b.y + 1){
        return 3;
    }
    else if(a.x==b.x - 1 && a.y == b.y + 1){
        return 4;
    }
    else if(a.x==b.x - 1 && a.y == b.y){
        return 5;
    }
    else if(a.x==b.x - 1 && a.y == b.y - 1){
        return 6;
    }
    else if(a.x==b.x && a.y == b.y - 1){
        return 7;
    }
    else if(a.x==b.x + 1 && a.y == b.y - 1){
        return 8;
    }
    else{
        return 0;
    }
}

//find nearest internal robot
Point find_nearest_int(Point a, Point* bound, int tot_b, Point* internals, int tot_int){
    Point* int_robots = new Point[tot_int];
    int k = 0, m = 0;
    for(int j = 0; j < tot_int;j++){
        Point q;
        q.x = internals[j].x;
        q.y = internals[j].y;
        m = 0;
        for(int l = 0; l < tot_b; l++){
            Point p = bound[l % tot_b];
            Point r = bound[(l+1) % tot_b];
            if(onSegment(p,q,r)){
                m = 1;
            }
        }
        if(m == 0){
            int_robots[k].x = q.x;
            int_robots[k].y = q.y;
            k++;
        }
    }
    //cout<<"Internal robots: ";
    //for(int i = 0;i<k;i++){
    //    cout<<"("<<int_robots[i].x<<", "<<int_robots[i].y<<"), ";
    //}
    //cout<<endl;

    Point near;
    near.x = int_robots[0].x;
    near.y = int_robots[0].y;
    int dist = ((a.x - int_robots[0].x) * (a.x - int_robots[0].x)) + ((a.y - int_robots[0].y) * (a.y - int_robots[0].y)), dist1 = 0;
    for(int i =1;i< k; i++){
        dist1 = ((a.x - int_robots[i].x) * (a.x - int_robots[i].x)) + ((a.y - int_robots[i].y) * (a.y - int_robots[i].y));
        if(dist1 < dist){
            near.x = int_robots[i].x;
            near.y = int_robots[i].y;
            dist = dist1;
        }
    }
    return near;
}

//move
Point move(Point a, Point *b, Point *c, int tot_b, int tot_c, Point *internals, int intsize){
    int dir1 = 0, dir2 =0, dir3 = 0, dir4 = 0;
    int n1=0,n2=0,n3=0,n4=0,n5=0,n6=0,n7=0,n8=0;
    for(int i =0;i<tot_b;i++){
        //find direction to move
        if(b[i].x < a.x && b[i].y>a.y){
            dir2 = 1;
        }
        if(b[i].x > a.x && b[i].y>a.y){
            dir1 = 1;
        }
        if(b[i].x < a.x && b[i].y<a.y){
            dir3 = 1;
        }
        if(b[i].x > a.x && b[i].y<a.y){
            dir4 = 1;
        }

        //find neighbors
        if(b[i].x == a.x +1 && b[i].y == a.y){
            n1=1;
        }
        else if(b[i].x == a.x +1 && b[i].y == a.y + 1){
            n2=1;
        }
        else if(b[i].x == a.x && b[i].y == a.y + 1){
            n3=1;
        }
        else if(b[i].x == a.x -1 && b[i].y == a.y + 1){
            n4=1;
        }
        else if(b[i].x == a.x -1 && b[i].y == a.y){
            n5=1;
        }
        else if(b[i].x == a.x - 1 && b[i].y == a.y - 1){
            n6=1;
        }
        else if(b[i].x == a.x && b[i].y == a.y - 1){
            n7=1;
        }
        else if(b[i].x == a.x + 1 && b[i].y == a.y - 1){
            n8=1;
        }
    }
    for(int i =0;i<intsize;i++){
        if(internals[i].x == a.x +1 && internals[i].y == a.y){
            n1=1;
        }
        else if(internals[i].x == a.x +1 && internals[i].y == a.y + 1){
            n2=1;
        }
        else if(internals[i].x == a.x && internals[i].y == a.y + 1){
            n3=1;
        }
        else if(internals[i].x == a.x -1 && internals[i].y == a.y + 1){
            n4=1;
        }
        else if(internals[i].x == a.x -1 && internals[i].y == a.y){
            n5=1;
        }
        else if(internals[i].x == a.x - 1 && internals[i].y == a.y - 1){
            n6=1;
        }
        else if(internals[i].x == a.x && internals[i].y == a.y - 1){
            n7=1;
        }
        else if(internals[i].x == a.x + 1 && internals[i].y == a.y - 1){
            n8=1;
        }
    }

    if(n1==1 && n5 == 1){
        if(dir1 == 1 || dir2 == 1){
            a.y = a.y - 1;
        }
        else if(dir3 == 1 || dir4 == 1){
            a.y = a.y + 1;
        }
    }
    else if(n3==1 && n7 == 1){
        if(dir1 == 1 || dir4 == 1){
            a.x = a.x - 1;
        }
        else if(dir2 == 1 || dir3 == 1){
            a.x = a.x + 1;
        }
    }
    else if(n2 == 1 && n6 == 1){
        if(dir2 == 0){
            a.x = a.x - 1;
            a.y = a.y + 1;
        }
        else if(dir4 == 0){
            a.x = a.x + 1;
            a.y = a.y - 1;
        }
    }
    else if(n4 == 1 && n8 == 1){
        if(dir1 == 0){
            a.x = a.x + 1;
            a.y = a.y + 1;
        }
        else if(dir3 == 0){
            a.x = a.x - 1;
            a.y = a.y - 1;
        }
    }
    else{
        Point near = find_nearest_int(a, b, tot_b, internals, intsize);
        //cout<<"Nearest internal of ("<<a.x<<", "<<a.y<<") is ("<<near.x<<", "<<near.y<<")"<<endl;
        //getchar();
        int neighbor = check_neighbor(near, a);
        if(neighbor == 1){
            a.x = a.x -1;
        }
        else if(neighbor == 2){
            a.x = a.x - 1;
            a.y = a.y - 1;
        }
        else if(neighbor == 3){
            a.y = a.y - 1;
        }
        else if(neighbor == 4){
            a.x = a.x + 1;
            a.y = a.y - 1;
        }
        else if(neighbor == 5){
            a.x = a.x + 1;
        }
        else if(neighbor == 6){
            a.x = a.x + 1;
            a.y = a.y + 1;
        }
        else if(neighbor == 7){
            a.y = a.y + 1;
        }
        else if(neighbor == 8){
            a.x = a.x - 1;
            a.y = a.y + 1;
        }
        else if(neighbor == 0){
            int tot_neighbors = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8;
            if(tot_neighbors>=2){
                if(n1 == 1 & n6 == 1){
                    a.y = a.y + 1;
                }
                else if(n1 == 1 && n4 == 1){
                    a.y = a.y - 1;
                }
                else if(n1 == 1 && n7 == 1){
                    a.x = a.x - 1;
                    a.y = a.y + 1;
                }
                else if(n1 == 1 && n3 == 1){
                    a.x = a.x - 1;
                    a.y = a.y - 1;
                }
                else if(n5 == 1 & n8 == 1){
                    a.y = a.y + 1;
                }
                else if(n5 == 1 && n2 == 1){
                    a.y = a.y - 1;
                }
                else if(n5 == 1 && n7 == 1){
                    a.x = a.x + 1;
                    a.y = a.y + 1;
                }
                else if(n5 == 1 && n3 == 1){
                    a.x = a.x + 1;
                    a.y = a.y - 1;
                }
                else if(n3 == 1 && n8 == 1){
                    a.x = a.x - 1;
                }
                else if(n3 == 1 && n6 == 1){
                    a.x = a.x + 1;
                }
                else if(n7 == 1 && n2 == 1){
                    a.x = a.x - 1;
                }
                else if(n7 == 1 && n4 == 1){
                    a.x = a.x + 1;
                }
                else if(n1 == 1 && n2 == 1){
                    a.x = a.x - 1;
                }
                else if(n2 == 1 && n3 == 1){
                    a.y = a.y - 1;
                }
                else if(n3 == 1 && n4 == 1){
                    a.y = a.y - 1;
                }
                else if(n4 == 1 && n5 == 1){
                    a.x = a.x + 1;
                }
                else if(n5 == 1 && n6 == 1){
                    a.x = a.x + 1;
                }
                else if(n6 == 1 && n7 == 1){
                    a.y = a.y + 1;
                }
                else if(n7 == 1 && n8 == 1){
                    a.y = a.y + 1;
                }
                else if(n8 == 1 && n1 == 1){
                    a.x = a.x - 1;
                }
            }
        }
    }

    return a;
}
int main() {
    int count = 0, tot_round = 0, totalrobots = 0, n=0, tests = 0,space = 1;
    cout<<"----------------------------------------------------------------------"<<endl<<endl;
    std::cout << "\tTIME ANALYSIS OF COMPLETE VISITABILITY PROBLEM " << std::endl<<std::endl;
    cout<<"----------------------------------------------------------------------"<<endl<<endl;
    std::cout << "Enter the number of robots: ";
    std::cin >> totalrobots;
    again:std::cout << "Select the grid space for robot configuration:"<<endl;
    std::cout << "1. Compact square (sqrt(n) * sqrt(n))"<<endl;
    std::cout << "2. Tight square ((sqrt(n)*2) * (sqrt(n)*2))"<<endl;
    std::cout << "3. Loose square ((sqrt(n)*3) * (sqrt(n)*3))"<<endl;
    std::cout << "Choose Option (1, 2 or 3): ";
    std::cin >> space;
    if(space !=1 && space !=2 && space !=3){
        std::cout<<"Wrong choice"<<endl;
        goto again;
    }
    std::cout << "\nHow many tests do you want to perform: ";
    std::cin >> tests;
    cout<<"----------------------------------------------------------------------"<<endl;
    cout<<"\nTotal number of robots: "<<totalrobots<<endl<<endl;
    cout <<"Tests \t\tConvex hull layers (h)\t\tRounds (Outward Wave)" << endl;
    cout<<"----------------------------------------------------------------------"<<endl;
    srand(time(0));


    while(count < tests) {
        n = totalrobots;
        int h = 0;
        int *totalPoints = new int[n];
        int *internalsize = new int[n];


        int range = (int) sqrt(n) * space;
        Point *robots = new Point[n];
        Point **ch_layers = new Point *[n];
        Point **internals = new Point *[n];
        for (int i = 0; i < n; i++) {
            ch_layers[i] = new Point[n];
            internals[i] = new Point[n];
            totalPoints[i] = 0;
        }
        //cout << "The robots points are:" << endl;
        for (int i = 0; i < n; i++) {
            label:
            Point newpoint = generateCoord(range);
            for (int j = i - 1; j >= 0; j--) {
                if (robots[j].x == newpoint.x && robots[j].y == newpoint.y) {
                    goto label;
                }
            }
            robots[i].x = newpoint.x;
            robots[i].y = newpoint.y;
            //cout << "(" << robots[i].x << ", " << robots[i].y << ") ";
        }
        //cout << endl;

        int isize = totalrobots;
        internalsize[0] = isize;
        while (n > 0) {

            //Point *oldrobots = new Point[n];
            internals[h] = new Point[n];
            for (int i = 0; i < n; i++) {
                //oldrobots[i].x = robots[i].x;
                //oldrobots[i].y = robots[i].y;
                internals[h][i].x = robots[i].x;
                internals[h][i].y = robots[i].y;
            }

            //cout << "The convex hull points are (out of total " << n << "): Layer " << h << endl;

            int h_count = 0;

            if (h == 7) {
                stack<Point> s = convexHull(robots, n);
            }
            stack<Point> s = convexHull(robots, n);

            while (!s.empty()) {
                Point p = s.top();

                ch_layers[h][h_count].x = p.x;
                ch_layers[h][h_count].y = p.y;
                s.pop();
                h_count++;
            }
            //for (int i = 0; i < h_count; i++) {
            //std::cout << "(" << ch_layers[h][i].x << ", " << ch_layers[h][i].y << ")";
            //}
            isize = isize - h_count;
            internalsize[h + 1] = isize;
            totalPoints[h] = h_count;
            //cout << endl;

            Point *newrobots = new Point[n - h_count];
            int flag = 0, k = 0;
            for (int i = 0; i < n; i++) {
                //Point p = oldrobots[i];
                Point p = internals[h][i];
                for (int j = 0; j < h_count; j++) {
                    Point q = ch_layers[h][j];
                    if (p.x == q.x && p.y == q.y) {
                        flag = 1;
                        //break;
                    }
                }
                if (flag == 1) {
                    flag = 0;
                } else {
                    newrobots[k].x = p.x;
                    newrobots[k].y = p.y;
                    flag = 0;
                    k++;
                }
            }
            //if((n - h_count) > 0) {
            //    cout << "New set of points:" << endl;
            //}
            robots = new Point[n - h_count];
            for (int i = 0; i < n - h_count; i++) {
                robots[i].x = newrobots[i].x;
                robots[i].y = newrobots[i].y;
                //    cout<<"("<<robots[i].x<<", "<<robots[i].y<<") ";
            }

            //cout<<endl;
            //ch_layers[x] = printHullPoints(s);
            //robots = removeHullPoints(robots, ch_layers[x],n,h_count);

            n = n - h_count;
            h++;
        }
        //cout << endl;
        //cout << "Convex hull layers, h = " << h << endl << endl;
        //for (int i = 0; i < h; i++) {
        //    cout << "size of internal layer " << i << ": " << internalsize[i] << endl;
        //}

        //inward wave result, innermost layer terminate
        int terminate = h - 1;

        //outward wave
        int round = 0, iteration = 1;
        while (terminate > 0) {
            //  if (totalrobots <= 250)
            //      cout << "iteration " << iteration << endl;
            for (int i = 0; i < terminate; i++) {
                bool mov = false;
                //    if (totalrobots <= 250) {
                //      cout << "Before move, Layer " << i << ": ";
                //    for (int j = 0; j < totalPoints[i]; j++) {
                //      cout << "(" << ch_layers[i][j].x << ", " << ch_layers[i][j].y << ") ";
                //}
                //}
                Point* ch_layers1 = new Point[totalPoints[i]];
                for (int j = 0; j < totalPoints[i]; j++) {
                    ch_layers1[j].x = ch_layers[i][j].x;
                    ch_layers1[j].y = ch_layers[i][j].y;
                }

                for (int j = 0; j < totalPoints[i]; j++) {
                    Point move_point = move(ch_layers[i][j], ch_layers[i], ch_layers[i + 1], totalPoints[i],
                                            totalPoints[i + 1], internals[i + 1], internalsize[i + 1]);
                    if (move_point.x != ch_layers[i][j].x || move_point.y != ch_layers[i][j].y) {
                        ch_layers1[j].x = move_point.x;
                        ch_layers1[j].y = move_point.y;
                        mov = true;
                    }
                }
                for (int j = 0; j < totalPoints[i]; j++) {
                    ch_layers[i][j].x = ch_layers1[j].x;
                    ch_layers[i][j].y = ch_layers1[j].y;

                    internals[i][j].x = ch_layers1[j].x;
                    internals[i][j].y = ch_layers1[j].y;
                }
                for (int k = 0; k < internalsize[i + 1]; k++) {
                    internals[i][k + totalPoints[i]].x = internals[i + 1][k].x;
                    internals[i][k + totalPoints[i]].y = internals[i + 1][k].y;
                }
                /*if (totalrobots <= 250) {
                    cout << "After Move, Layer " << i << ": ";
                    for (int j = 0; j < totalPoints[i]; j++) {
                        cout << "(" << ch_layers[i][j].x << ", " << ch_layers[i][j].y << ") ";
                    }
                    cout << endl;
                }*/
                if (mov) {
                    round++;
                }
                /*if (mov) {
                    round++;

                    for (int k = 0; k < totalPoints[i]; k++) {
                        internals[i][k].x = ch_layers[i][k].x;
                        internals[i][k].y = ch_layers[i][k].y;
                    }
                    for (int k = 0; k < internalsize[i + 1]; k++) {
                        internals[i][k + totalPoints[i]].x = internals[i + 1][k].x;
                        internals[i][k + totalPoints[i]].y = internals[i + 1][k].y;
                    }

                }*/
                //cout << internalsize[i + 1] << endl;
            }
            for (int i = terminate-2;i>=0;i--){
                for (int j = 0; j<totalPoints[i]; j++) {
                    internals[i][j].x = ch_layers[i][j].x;
                    internals[i][j].y = ch_layers[i][j].y;
                }
                for (int k = 0; k < internalsize[i + 1]; k++) {
                    internals[i][k + totalPoints[i]].x = internals[i + 1][k].x;
                    internals[i][k + totalPoints[i]].y = internals[i + 1][k].y;
                }
            }

            terminate--;
            iteration++;
            // if (totalrobots <= 250)
            //cout << endl;
        }

        cout <<"Test "<<count+1<<"\t\t\t h = "<<h<< "\t\t\t\t" << round << endl;
        tot_round = tot_round + round;
        count++;
    }

    cout<<endl;
    cout<<"----------------------------------------------------------------------"<<endl;
    cout<<"Total rounds: \t\t\t\t\t\t"<<tot_round<<endl;
    cout<<"\nAverage rounds of "<< tests <<" rounds: \t\t\t\t"<<(double)tot_round/(double)tests<<endl<<endl<<endl;
    return 0;
}
