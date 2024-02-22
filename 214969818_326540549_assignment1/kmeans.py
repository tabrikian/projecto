import numpy
import sys
euclid_dst = numpy.linalg.norm
eps = 0.001


def ret_args():
    arg_lst = sys.argv[1:-1]

    if len(arg_lst) != 4:
        if len(arg_lst) == 3:
            arg_lst.append('200')
        else:
            print("too many arguments")

    N = float(arg_lst[1])
    if not N.is_integer() or N <= 1:
        print("Invalid number of points!")
        exit()

    K = float(arg_lst[0])
    if not K.is_integer() or K >= N or K <= 1:
        print("Invalid number of clusters!")
        exit()

    d = int(arg_lst[2])
    if d < 1:
        print("Invalid dimension of point!")
        exit()

    iter_n = float(arg_lst[3])
    if not iter_n.is_integer() or iter_n >= 1000 or iter_n <= 1:
        print("Invalid maximum iteration!")
        exit()

    return int(K), int(N), d, int(iter_n)


def points_to_clusters(k, centroids, points):
    clusters = [[] for _ in range(k)]
    for point in points:
        dst_to_centroids_lst = [euclid_dst(point - centroid) for centroid in centroids]
        clusters[dst_to_centroids_lst.index(min(dst_to_centroids_lst))].append(point)
    return clusters


def update_centroids(clusters):
    return [numpy.add.reduce(cluster) / len(cluster) for cluster in clusters]


def is_converged(centroids, new_centroids):
    for i, centroid in enumerate(centroids):
        if euclid_dst(centroid - new_centroids[i]) > eps:
            return False
    return True


def do_kmeans(k, max_iter, points):
    centroids = [points[i] for i in range(k)]
    for _ in range(max_iter):
        clusters = points_to_clusters(k, centroids, points)
        new_centroids = update_centroids(clusters)
        if is_converged(centroids, new_centroids):
            return new_centroids
        centroids = new_centroids
    return centroids


def main():
    K, N, d, max_iter = ret_args()
    filename = sys.argv[-1]
    if filename[-4:] != ".txt":
        exit()
    f = open(filename, 'r')
    p = f.read().split('\n')
    points = [numpy.array([float(x) for x in p[i].split(",")]) for i in range(N)]
    points = [point[:d] for point in points]

    centroids = do_kmeans(K, max_iter, points)

    for centroid in centroids:
        sys.stdout.write('%.4f' % centroid[0])
        for j in range(1, d):
            sys.stdout.write(',')
            sys.stdout.write('%.4f' % centroid[j])
        sys.stdout.write('\n')


if __name__ == '__main__':
    main()
