#ifndef CLUSTERING_HPP
#define CLUSTERING_HPP
#include <iostream>

#ifndef TOL
        #define TOL     1.e-9
#endif


template<class T>
class NuagePoint
{
    int M_n; // nombre de points
    T * point; // tableau contenant des points
    double (*distance)(T, T); // permet de calculer la dist entre 2 pts

public:
    NuagePoint();
    NuagePoint(int n, double (*pf)(T, T));
    NuagePoint(int n, T * pts, double (*pf)(T, T));
    NuagePoint(const NuagePoint & np);
    ~NuagePoint();

    //getter setter
    int get_n() const;
    T * get_point() const; // tout le tableau 
    double get_distance(T a, T b) const; // evalue distance entre les param donnés
    void set_n(int n);
    void set_point(int index, T val);
    void set_distance(double (*newdist)(T, T) );

    // surcharge d'operateurs
    NuagePoint & operator = (const NuagePoint & np);
    T & operator [] (int index); // renvoie le i-ème pt
    NuagePoint operator + (const NuagePoint & np); // concaténation

    // autres méthode
    void resize_point(int size);

    // Tests
    int testu_1(); // constru par default
    int testu_2(); // constru normal
    int testu_3(); // constru par copie
    int testu_4(); // destructeur, il existe des outils plus adapter pour check le fuite de mem
    int testu_5(); // get n
    int testu_6(); // get point, prend le ptr ie tout le tableau
    int testu_7(); // get distance, eval la fct
    int testu_8(); // set n
    int testu_9(); // set point, change une donné dans le tableau point
    int testu_10(); // set nouvelle fonction distance
    int testu_11(); // overload operator =
    int testu_12(); // overload operator []
    int testu_13(); // overload operator + 
    static int all_testsu();

};

// constructeur par default
template<class T>
NuagePoint < T >::NuagePoint()
{
    M_n = 0;
    point = new T [0];
    distance = nullptr; 
}
// constructeur par copie
template<class T>
NuagePoint < T >::NuagePoint(const NuagePoint & np)
{
    M_n = np.M_n;
    point = new T [M_n];
    for (int i=0; i<M_n; ++i)
        point[i] = np.point[i]; 
    distance = np.distance; 
}
// constructeur normal
template<class T>
NuagePoint < T > ::NuagePoint(int n, T * pts, double (*pf)(T, T))
{
    M_n = n;
    point = pts; // si le tableau original est détruit ?
    distance = pf;
}
template<class T>
NuagePoint<T>::NuagePoint(int n, double (*pf)(T, T))
{
    M_n = n;
    point = new T [n];
    distance = pf;
}

// destructeur
template<class T>
NuagePoint < T >::~NuagePoint()
{
    delete [] point;
}
// getter & setter
template<class T>
int NuagePoint<T>::get_n() const { return M_n; }

template<class T>
T * NuagePoint<T>:: get_point() const { return point; }

template<class T> // evalue distance entre les param donnés
double NuagePoint<T>::get_distance(T a, T b) const 
{ return distance(a, b); }

template<class T>
void NuagePoint<T>::set_n(int n)
{ 
    M_n = n; 
    resize_point(n);
    /*for (int i=0; i<(2*n); ++i)
        std::cout<< point[i]<<std::endl;*/
}

template<class T>
void NuagePoint<T>::set_point(int index, T val)
{
    if ( index > (M_n -1) )
    {
        std::cout<< "Erreur : le nuage ne contient pas au temps de points !" << std::endl;
        std::cout<< "Veuillez d'abord augmenter le nombre de point du nuage." << std::endl;
        exit(0);
    }
    point[index] = val; 
}

template<class T>
void NuagePoint<T>::set_distance(double (*newdist)(T, T) )
{ distance = newdist; }

// surcharges d'operateurs
template<class T>
NuagePoint<T> & NuagePoint<T>::operator = (const NuagePoint & np)
{
    if ( this != &np )
    {
        if (point != nullptr)
            delete [] point;
        M_n =  np.M_n;
        point = new T [M_n];
        for (int i=0; i<M_n; ++i)
            point[i] = np.point[i];
        distance = np.distance;
    }
    return *this;
}
template<class T>  // renvoie le i-ème pt
T & NuagePoint<T>::operator [] (int index)
{
    if ( index > M_n )
    {
        std::cout << "erreur : l'index donné est supérieur à la taille du tableau de points"
            << std::endl;;
    }
    return point[index];
}
template<class T>  // concaténation
NuagePoint<T> NuagePoint<T>::operator + (const NuagePoint & np)
{
    NuagePoint ret;
    ret.M_n = M_n + np.M_n;

    ret.point = new T [ret.M_n];
    for (int i=0; i<M_n; ++i)
        ret.point[i] = point[i];
    for (int i=0; i<np.M_n; ++i)
        ret.point[M_n + i] = np.point[i]; // attention a bien tester 
    
    ret.distance = distance;

    return ret;
}
template<class T> 
void NuagePoint<T>::resize_point(int size)
{
    T * resize = new T[size];
    for ( int i=0; i<size; ++i)
        resize[i] = point[i];
    delete [] point;
    point = resize;
}


/////////////////////////////////////////////////////////////////////
//                         TESTS
//
//////////////////////////////////////////////////////////////////////

// distance dans R1 pour test, en pratique T complex ?
double dist(double a , double b){ return std::abs(a-b); }
double dist2(double i, double j) { return i + j; }

template<class T>
int NuagePoint<T>::testu_1() // test constru par default
{
    NuagePoint<double> n;
    int ret = 0;
    if (n.M_n == 0)
        ret += 1;
    if (n.point != nullptr) // meh
        ret += 1;
    if (n.distance == nullptr)
        ret += 1;
    return ret/3;
    
}

template<class T>
int NuagePoint<T>::testu_3() // test constructeur par copy
{
    double * arr = new double [5];
    NuagePoint<double> n1(5, arr, dist);
    NuagePoint<double> n2(n1);

    int ret = 0;
    if ( n1.M_n == n2.M_n )
        ret += 1;
    if ( n1.point != n2.point)
        ret += 1;
    for (int i=0; i<5; ++i)
    {
        if ( n1.point[i] == n2.point[i] ) 
        ret += 1;
    }
    if ( n1.distance == n2.distance )
        ret += 1;
    return ret/8;
}
template<class T>
int NuagePoint<T>::testu_2() // test constru normal
{
    double * arr = new double[3];
    NuagePoint n(2, arr, dist);
    int ret = 0;
    if (n.M_n == 2 && n.point == arr && n.distance == dist )
        ret += 1;
    return ret;
}
/*
template<class T>
int NuagePoint<T>::testu_4() // test destructeur, il existe des outils plus adapter pour check le fuite de mem
{
    double * arr = new double[5];
    NuagePoint<double> n(5, arr, dist);
    double * p1 = n.point;
    double * p2 = n.distance;
    NuagePoint<double> n;
    int ret = 0;
    if (p1 != n.point && p2 != n.distance)
        ret += 1;
    return ret;
}
*/
template<class T>
int NuagePoint<T>:: testu_5() // test get n
{
    double * arr = new double[3];
    NuagePoint<double> n(3, arr, dist);
    int ret =0;
    if (n.get_n() == 3)
        ret += 1;
    return ret;
}
template<class T>
int NuagePoint<T>:: testu_6() // test get point, prend le ptr ie tout le tableau
{
    int ret = 0;
    double * arr = new double[3];
    NuagePoint<double> n(3, arr, dist);
    if (n.get_point() == n.point)
        ret += 1;
    return ret;
}
template<class T>
int NuagePoint<T>:: testu_7() // test get distance, eval la fct
{
    int ret = 0;
    double * arr = new  double[3];
    NuagePoint<double> n(3, arr, dist);
    if (n.get_distance(3.1, 5.1) - 2. < TOL )
        ret += 1;
    return ret;
}
template<class T>
int NuagePoint<T>:: testu_8() // test set n
{
    int ret = 0;
    double * arr = new  double[6] {1, 2, 3, 4, 5, 6};
    NuagePoint<double> n(6, arr, dist);
    n.set_n(8);
    if (n.M_n == 8)
        ret += 1;

    n.set_n(5);
    if (n.M_n == 5)
        ret += 1;
    return ret/2;
}
template<class T>
int NuagePoint<T>:: testu_9() // test set point, change une donné dans le tableau point
{
    int ret = 0;
    double * arr = new  double[10];
    NuagePoint<double> n(10, arr, dist);
    n.set_point(1, 10);
    if ( n.point[1] == 10 )
        ret += 1;
    return ret;
}

double foo(double i, double j) 
    {return i + j; }


template<class T>
int NuagePoint<T>:: testu_10() // test set nouvelle fonction distance
{
    int ret = 0;
    double (*pf)(double, double);
    pf = &foo;
    NuagePoint<double> n(3, nullptr, dist);
    n.set_distance(pf);
    if ( n.get_distance(5, 2) == 7)
        ret += 1;
    return ret;
}
template<class T>
int NuagePoint<T>:: testu_11() // test overload operator =
{
    int ret = 0;
    //double * arr1 = new double[3] {1.1, 2.2, 3.3};
    double * arr2 = new double[2] {4.4, 5.5};
    NuagePoint<double> n1(3, nullptr, dist2);
    NuagePoint<double> n2(2, arr2, dist);
    n1 = n2;
    if ( n1.M_n == 2 && n1.distance(1, 2) == dist(1, 2) )
        ret += 1;
    if ( n1.point[0] == 4.4 && n1.point[1] == 5.5 )
        ret += 1;
    return ret/2;


}
template<class T>
int NuagePoint<T>:: testu_12() // test overload operator []
{
    int ret = 0;
    double * arr = new double[3] {4., 5., 6.};
    NuagePoint<double> n(3, arr, dist);
    if ( n[0] == 4. && n[1] == 5. && n[2] == 6.)
        ret += 1;
    return ret;
}
template<class T>
int NuagePoint<T>:: testu_13() // test overload operator + 
{
    int ret = 0;
    double * arr1 = new double[3] {1., 2., 3.};
    double * arr2 = new double[5] {4.1, 5.1, 6.1, 7.7, 8.8};
    NuagePoint<double> n1(3, arr1, nullptr);
    NuagePoint<double> n2(5, arr2, dist);
    NuagePoint<double> n3;
    n3 = (n1 + n2); // comment metter n1 + n2 dans n3 ?
    if (n3.M_n == 8 && n3[0] == 1. && n3[7] == 8.8 && n1.distance == nullptr)
        ret += 1;
    return ret;
}
template<class T>
int NuagePoint<T>::all_testsu()
{
    int ret = 0;
    NuagePoint<double> np;
    ret += np.testu_1();
    ret += np.testu_2();
    ret += np.testu_3();
    //ret += np.testu_4(); // rip
    ret += np.testu_5();
    ret += np.testu_6();
    ret += np.testu_7(); 
    ret += np.testu_8();
    ret += np.testu_9();
    ret += np.testu_10();
    ret += np.testu_11();
    ret += np.testu_12();
    ret += np.testu_13(); 
    std::cout << ret << "/12 tests de NuagePoint passés !" << std::endl;
        return 12 - ret;
}


template<class T>
class Clustering 
{
    int M_nb_cluster; // nombre de cluster
    NuagePoint<T> * M_nuage; // pointeur sur un nuage de points
    int * M_num_cluster; // tableau numero de point - numero de cluster

public:
    // constructeur, destructeur
    Clustering();
    Clustering(const Clustering & c);
    ~Clustering();

    // getter, setter
    int get_nb_cluster();
    NuagePoint<T> * get_nuage();
    int * get_num_cluster();
    void set_nb_cluster(int n);
    void set_nuage(const NuagePoint<T> & n);
    void set_num_cluster(int index, int val);

    // overload operator
    Clustering & operator = (const Clustering & c);
    int & operator [] (int index);

    template<class U>
    friend std::ostream & operator << (std::ostream & out, const Clustering<U> & c);
    
    // autres methodes
    void resize_cluster(int size);

    // virtual Clustering calc_cluster () = 0; d'abord faire les test

    // Tests
    int testu_1(); // construteurs
    int testu_2(); // getter
    int testu_3(); // setter
    int testu_4(); // overload =
    int testu_5(); // overload []
    int testu_6(); // overload <<
    static int all_testsu();
};

// constructeur par default
template<class T>
Clustering<T>::Clustering()
{
    M_nb_cluster = 0;
    NuagePoint<T> * M_nuage;
    int * M_num_cluster = new int[0];
}

// constructeur par copie
template<class T>
Clustering<T>::Clustering(const Clustering & c)
{
    M_nb_cluster = c.M_nb_cluster;
    NuagePoint<T> nuage;
    nuage = c.M_nuage; // = est surchargé donc ca devrai etre ok
    M_nuage = &nuage;
    if (M_num_cluster != nullptr)
        delete [] M_num_cluster;
    M_num_cluster = new int [M_nb_cluster];
    for ( int i=0; i<M_nb_cluster; ++i )
        M_num_cluster[i] = c.M_num_cluster[i];
}
// destructeur
template<class T>
Clustering<T>::~Clustering()
{
    delete [] M_num_cluster; 
    // le destructeur de NuagePoint sera appeler automatiquement
}
// getter
template<class T>
int Clustering<T>::get_nb_cluster() { return M_nb_cluster; }

template<class T>
NuagePoint<T> * Clustering<T>::get_nuage() { return M_nuage; }

template<class T>
int * Clustering<T>::get_num_cluster() { return M_num_cluster; }

// setter
template<class T>
void Clustering<T>::set_nb_cluster(int n)
{
    M_nb_cluster = n;
    resize_cluster;
}
template<class T>
void Clustering<T>::set_nuage(const NuagePoint<T> & n)
{
    M_nuage = &n; 
}
template<class T>
void Clustering<T>:: set_num_cluster(int index, int val)
{
    if ( index > (M_nb_cluster - 1) )
    {
        std::cout << "Erreur : il n'y a pas au temps de points dans le cluster !" <<std::endl;
        exit(0);
    }
    M_num_cluster[index] = val;
}
template<class T> 
void Clustering<T>::resize_cluster(int size)
{
    T * resize = new T[size];
    for ( int i=0; i<size; ++i)
        resize[i] = M_num_cluster[i];
    delete [] M_num_cluster;
    M_num_cluster = resize;
}
template<class T>
Clustering<T> & Clustering<T>::operator = (const Clustering & c)
{
    if ( this != c )
    {
        M_nb_cluster = c.M_nb_cluster;
        M_nuage = c.M_nuage;
        if ( M_num_cluster != nullptr )
            delete M_num_cluster;
        M_num_cluster = new int [M_nuage];
        for (int i=0; i<M_nuage; ++i)
            M_num_cluster[i] = c.M_num_cluster[i];
    }
    return * this;
}
template<class T>
int & Clustering<T>::operator [] (int index)
{
    if ( index > M_nb_cluster)
    {
        std::cout<<"Erreur : l'index est trop grand, il n'y a pas au temps de cluster !"
        << std::endl;
        exit(0);   
    }
    return M_num_cluster[index];
}
template<class U>
std::ostream & operator << (std::ostream & out, const Clustering<U> & c)
{
    out<< "" << std::endl;
    out<< "Nombre de cluster : "<< c.M_nb_cluster;
    out<< " _________________________" ;
    out<< "|Point  |numéro de cluster|" ;
    out<< "|_______|_________________|" ;
    for (int i=0; i<c.M_nb_cluster; ++i)
    {
    out<< "|  " << i <<"   |       "<< c.M_num_cluster[i] << "      |";
    }
    return out;
}




#endif