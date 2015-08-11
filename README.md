## diff
a utility for automatic differentiation

implements a forward automatic differentiation variable (FAD) via operator overloading

### diff.h
* a single header implementation of a FAD variable
* conceptually simple
* uses temporary variables at each operation step
* temporary variables are sloooow

### expr.h
* an additional header that implements FAD with expression templates
* eliminates the need for temporary variables during each operation
* expression templates are fast (usually)

### useful information
* [wikipedia](https://en.wikipedia.org/wiki/Automatic_differentiation) has a good introduction to the subject of automatic differentiation
* [this](http://link.springer.com/article/10.1007%2Fs007910000048#page-1) paper by Aubert et al. discusses FAD with expression templates and an application to computational fluid dynamics
* [this](http://arxiv.org/abs/1205.3506) paper by Phipps and Pawlowski contains a good discussion about FAD with expression templates, some of its limitations, and the potential to improve its performance 
