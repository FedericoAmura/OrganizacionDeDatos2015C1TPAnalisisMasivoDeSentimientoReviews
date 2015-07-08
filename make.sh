clear

cp dist/Debug/GNU-Linux-x86/popcorn popcorn

valgrind --leak-check=full ./popcorn

