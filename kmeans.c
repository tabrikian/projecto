#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define EPS 0.001


static int d;
static int k;
static int n;
static int iter = 200;


typedef struct{
    double* cordinates;
}point;


typedef struct{
    int size_of_points;
    point* points;
    point mean;
} cluster;


double distance(point p, point q){
    double squared_dis = 0;
    for(int i = 0; i < d; i++){
        squared_dis += (p[i]-q[i])**2;
    }
    return sqrt(squared_dis);
}

int set_parameters(){
    int num_of_args = __argc;
    if (num_of_args < 4 || num_of_args > 5){
        printf("An Error Has Occurred\n");
        return 1;
    }
    k = atoi(__argv[1]);
    n = atoi(__argv[2]);
    if (n <= k || 1 >= k){
        printf("Invalid number of clusters!\n");
        return 1;
    }
    if (n <= 1){
        printf("Invalid number of points!\n");
        return 1;
    }
    d = atoi(__argv[3]);
    if (d < 1){
        printf("Invalid dimension of point!\n");
        return 1;
    }
    if (num_of_args == 5){
        iter = atoi(__argv[4]);
        if (1000 <= iter || 1 >= iter){
            printf("Invalid maximum iteration!\n");
            return 1;
        }
    }
    return 0;
}

point* get_points(){
    double val;
    point* points = (point*) malloc(sizeof(point)*n);
    if (points == NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for(int i = 0; i < n; i++){
        points[i].cordinates = (double*) malloc(sizeof(double)*d);
        if (points[i].cordinates == NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
        for(int cord = 0; cord < d; cord++){
            if (scanf("%lf", &val) == 1){
                printf("An Error Has Occurred\n");
                return NULL;
            }
            points[i].cordinates[cord] = val;
            getchar();
        }
    }
    return points;
}

cluster* create_clasters(point* points){ 
    cluster* clusters = (cluster*) malloc(sizeof(cluster)*k)
    if (clusters == NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for(int i = 0; i < k; i ++){
        clusters[i].size_of_points = 0;
        clusters[i].points = (point*) malloc(sizeof(point)*n);
        clusters[i].mean = (double*) malloc(sizeof(double)*d);
        if (clusters[i].points == NULL || clusters[i].mean == NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
        for (int j = 0; j < d; i++){
            clusters[i].mean.cordinates[j] = points[i].cordinates[j];
        }
    }
    return clusters;
}

void add_point_to_cluster(cluster c, point p){
    c.points[c.size_of_points] = p;
    c.size_of_points++;
}

double update_mean_in_cluster(cluster c){
    double squared_dis = 0;
    for (int cord = 0; cord < k; cord++){
        double avg = 0;
        for(int i = 0; i < c.size_of_points; i++){
            avg += c.points[i].cordinates[cord];
        }
        avg /= c.size_of_points;
        squared_dis += (c.mean[cord]-avg)**2;
        c.mean.cordinates[cord] = avg;
    }
    return sqrt(squared_dis);
}

void clear_cluster(cluster c){
    c.size_of_points = 0;
}

double distance_from_cluster(cluster c, point p){
    return distance(p, c.mean);
}

cluster find_closeset_cluster(cluster* clusters, point p){
    cluster closest;
    for(int i = 0; i < k; i++){
        if (distance_from_cluster(closest, p) > distance_from_cluster(clusters+i, p))
            closest = clusters+i;
    }
    return closest;
}

void cleanup(point* points, cluster* clusters){
    for(int i = 0; i < n; i++){
        free(points[i].cordinates);
    }
    free(points);

    for(int i = 0; i < k; i++){
        free(clusters[i].points);
        free(clusters[i].mean);
    }
    free(clusters);
}

int main(){
    if (set_parameters()){
        return 1;
    }
    point points[] = get_points();
    if(points == NULL){
        return 1;
    }
    cluster clusters[] = create_clasters(points);
    if(clusters == NULL){
        return 1;
    }

    for(int i = 0; i < iter; i++){
        /*step 3 in the algorithem*/
        for(int j = 0; j < n; j++){
            cluster closest = find_closeset_cluster(clusters, points[j]);
            add_point_to_cluster(closest, points[j]);
        }

        /*step 4 in the algorithem*/
        double max_change = 0;

        for(int j = 0; j < k; j++){
            double change = update_mean_in_cluster(clusters[j]);
            clear_cluster(clusters[j]);

            if (change > max_change)
                max_change = change;
        }
        
        /*step 5 in the algorithem*/
        if (max_change < EPS)
            break;
    }
    /*print results*/
    for(int i = 0;  i < k; i++){
        printf("%.4f", clusters[i].mean.cordinates[0]);
        for(int j = 1; j < d; j++){
            printf(",%.4f", clusters[i].mean.cordinates[j]);
        }
        printf("\n");
    }
    cleanup();
    return 0;
}

