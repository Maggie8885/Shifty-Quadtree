/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
        :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
        :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
    avg = s.getAvg(ul,w,h);
    var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
    clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
    copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
    // Your code here.

    stats *s = new stats(imIn);
    pair<int, int> ul = make_pair(0, 0);
    int w = imIn.width();
    int h = imIn.height();
    //buildTree
    root = buildTree(*s, ul, w, h, tol);

}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul, int w, int h, double tol) {
    
    RGBAPixel a = s.getAvg(ul, w, h);
    double v = s.getVar(ul, w, h);
    Node * n = new Node(s,ul,w,h);
    double min_var = s.getVar(ul, w, h);
    int min_x;
    int min_y;
    //-1 var for no partition
    double ne_var = -1;
    double nw_var = -1;
    double se_var = -1;
    double sw_var = -1;
    
    
    //if base case
     if((w == 1 && h == 1) || v <= tol){
         return new Node(ul,w,h,a,v);
     } else {
         for (int j = 0; j < h; j++){
             for(int i = 0; i < w; i++) {
                 if (i == 0 && j == 0){
                     //no need have partitions
                     continue;
                 }
                 
                 if (i == 0){
                     //only have nw, sw
                     pair<int, int> sw_ul = make_pair(ul.first, ul.second + j);
                     //update var
                     nw_var = s.getVar(ul, w, j);
                     sw_var = s.getVar(sw_ul, w, h - j);
                 } else if (j == 0) {
                     // only have ne, nw
                     pair<int, int> ne_ul = make_pair(ul.first + i, ul.second);
                     //update var
                     nw_var = s.getVar(ul, i, h);
                     ne_var = s.getVar(ne_ul, w - i, h);
                     
                 } else {
                     //four partitions
                     pair<int, int> ne_ul = make_pair(ul.first + i, ul.second);
                     pair<int, int> sw_ul = make_pair(ul.first, ul.second + j);
                     pair<int, int> se_ul = make_pair(ul.first + i, ul.second + j);
                     //update var
                     nw_var = s.getVar(ul, i, j);
                     ne_var = s.getVar(ne_ul, w-i, j);
                     sw_var = s.getVar(sw_ul, i, h-j);
                     se_var = s.getVar(se_ul, w-i, h-j);
                     
                 }
                 
                 //get max var over 4 partitions
                 double max_n = max(nw_var,ne_var);
                 double max_s = max(sw_var,se_var);
                 double max_var = max(max_n,max_s);
                 
                 //update max_var
                 if (min_var > max_var) {
                     min_var = max_var;
                     min_x = i;
                     min_y = j;
                 }
             }
         }
     }
    
    

    if (min_x == 0){
        // two partitions: nw,sw
        pair<int, int> sw_ul = make_pair(ul.first, ul.second + min_y);
        // update nw,sw of the node
        n -> NW = buildTree(s, ul, w, min_y, tol);
        n -> SW = buildTree(s, sw_ul, w, h - min_y, tol);
        
    } else if (min_y == 0){
        //two partitions: nw, ne
        pair<int, int> se_ul = make_pair(ul.first + min_x, ul.second);
        // update nw,ne of the node
        n -> NW = buildTree(s, ul, min_x, h, tol);
        n -> NE = buildTree(s, se_ul, w - min_x, h, tol);
        
    } else {
        // four partitions
        pair<int, int> ne_ul = make_pair(ul.first + min_x, ul.second);
        pair<int, int> sw_ul = make_pair(ul.first, ul.second + min_y);
        pair<int, int> se_ul = make_pair(ul.first + min_x, ul.second + min_y);
        // // update nw,ne,sw,se of the node
        n -> NW = buildTree(s, ul, min_x, min_y, tol);
        n -> NE = buildTree(s, ne_ul, w - min_x, min_y, tol);
        n -> SW = buildTree(s, sw_ul, min_x, h - min_y, tol);
        n -> SE = buildTree(s, se_ul, w - min_x, h - min_y, tol);
    }
    
    return n;
   
}


/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
    // Your code here.
    int w = root -> width;
    int h = root -> height;
    PNG p = PNG(w, h);

    //call recursive helper function
    renders(root,p);
    return p;
}

//helper function for render
void SQtree::renders(Node * n, PNG &p) {

    int uf = n -> upLeft.first;
    int us = n -> upLeft.second;
    int nw = n -> width;
    int nh = n -> height;
    //set default node to have leaves
    int l = 1;


    //if there is NE in node
    if (n->NE != NULL) {
        renders(n->NE, p);
        l = 0;
    }

    //if there is NE in node
    if (n->SE != NULL) {
        renders(n->SE, p);
        l = 0;
    }


    //if there is NE in node
    if (n->NW != NULL) {
        renders(n->NW, p);
        l = 0;
    }


    //if there is NE in node
    if (n->SW != NULL) {
        renders(n->SW, p);
        l = 0;
    }

    //render leaf
    if (l == 1) {
        for (int i = uf; i < nw + uf; i++) {
            for(int j = us; j < nh + us; j++) {
                RGBAPixel * pixel = p.getPixel(i, j);
                pixel -> r = n -> avg.r;
                pixel -> g = n -> avg.g;
                pixel -> b = n -> avg.b;
            }
        }
    }
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
    // Your code here.
    //call recursive helper function
    clearSub(root);

}

void SQtree::clearSub(Node * n) {
    //check if there is more NE in the node
    if (n -> NE != NULL){
        //do recursion
        clearSub(n -> NE);
    }

    //check if there is more SE in the node
    if (n -> SE != NULL){
        //do recursion
        clearSub(n -> SE);
    }

    //check if there is more NW in the node
    if (n -> NW != NULL){
        //do recursion
        clearSub(n -> NW);
    }

    //check if there is more SW in the node
    if (n -> SW != NULL){
        //do recursion
        clearSub(n -> SW);
    }

    //delete node
    delete n;



}

void SQtree::copy(const SQtree & other) {
    // Your code here.
    //call recursive helper function
    //current root update
    root = copyOther(other.root);
}

SQtree::Node * SQtree::copyOther(Node * n) {
    pair<int, int> other_ul = n -> upLeft;
    int other_w = n -> width;
    int other_h = n -> height;
    RGBAPixel other_avg = n -> avg;
    double other_var = n -> var;

    Node * newNode = new Node(other_ul,other_w,other_h,other_avg,other_var);

    //check if there is more NE in the node
    if (n -> NE != NULL){
        //copy NE to the new node
        newNode -> NE = copyOther(n -> NE);
    }

    //check if there is more SE in the node
    if (n -> SE != NULL){
        //copy SE to the new node
        newNode -> SE = copyOther(n -> SE);
    }

    //check if there is more NW in the node
    if (n -> NW != NULL){
        //copy NW to the new node
        newNode -> NW = copyOther(n -> NW);
    }

    //check if there is more SW in the node
    if (n -> SW != NULL){
        //copy SW to the new node
        newNode -> SW = copyOther(n -> SW);
    }

    //return node
    return newNode;
}



int SQtree::size() {
    // Your code here.
    //call recursive function
    return subSize(root) + 1;
}

//helper function for size()
int SQtree::subSize(Node * n) {
    int size = 0;

    //check if there is more NE in the node
    if (n -> NE != NULL){
        //do recursion
        size = size + subSize(n -> NE);
    }

    //check if there is more SE in the node
    if (n -> SE != NULL){
        //do recursion
        size = size + subSize(n -> SE);
    }

    //check if there is more NW in the node
    if (n -> NW != NULL){
        //do recursion
        size = size + subSize(n -> NW);
    }

    //check if there is more SW in the node
    if (n -> SW != NULL){
        //do recursion
        size = size + subSize(n -> SW);
    }

    //return size
    return size;
}

    
    
    
