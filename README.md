# Matrix calculator
A command line matrix calculator written in C. Supports basic arithmetic operations: addition, subtraction, multiplication.

## Download and build
```sh
git clone https://github.com/anna-ib/matrix-calc.git
cmake .
make
```

## Usage
```sh
./matrix-calc [-i] [-o]

-i Alternative input data format
-o Alternative output data format
```

## Usage examples

        [ 3000 ]        [ 1 5 1 ]        [ 123 ]        [ 48 ]        
    A = [ 3000 ]    B = [ 3 4 1 ]    C = [ 456 ]    D = [ 25 ]
        [ 3000 ]        [ 7 8 1 ]        [ 172 ]        [ 13 ]

                        [  377  ]
        A - B * C + D = [  610  ]
                        [ -1694 ]

#### Default:
```sh
./matrix-calc
```
Input:
```
3 1
3000
3000
3000 
- 
3 3
1 5 1
3 4 1
7 8 1
* 
3 1
123
456
172
+
1 3
48
25
13
^D
```
Output:
```
3 1
377
610
-1694
```

#### Alternative input/output format

```sh
./matrix-calc -i -o
```
Input:
```
A=[3000; 3000; 3000]
B=[1 5 1; 3 4 1; 7 8 1]
C=[123; 456; 172]
D=[48; 25; 13]

A-B*C+D
```
Output:
```
[377; 610; -1694]
```

## License
- [MIT](https://github.com/anna-ib/matrix-calc/blob/main/LICENSE)