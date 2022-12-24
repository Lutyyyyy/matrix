# matrix - a project which implements matrix class, basic operations with matrices and Bareiss determinant computing algorithm 

### Installing and running

Prerequisites:
1) gtest should be installed on your system
2) cmake should be installed on your system

1. Create an empty folder where you want to build your project (f.e. it is called build)
2. `cd build`
3. run `cmake .. `
4. run `make` or `cmake --build`

After following this simple set of instructions all apps are now in `build/apps` in each separate directory. All unit tests are in `build/tests`. Test generation lies in another folder in apps called `gen_test` 

### Generating tests

1. Run `./gen-test/gen-test` in build directory

After that program creates 200 random square matrices of size 100 with determinant equal to 42. Then program automatically adds random rows, then swaps random rows and then subtracts random rows of matrix (each operation -> 100 times). Thes it compares determinant values of mixed matrices wirh initial value (42). If values don't match, it writes down input and output in file called 'gen_test_errors_log.txt', which is located in the same folder as 'gen_test' binary file.