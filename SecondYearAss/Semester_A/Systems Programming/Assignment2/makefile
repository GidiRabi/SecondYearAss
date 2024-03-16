# ~ commands ~

.PHONY: all clean

all: my_graph my_Knapsack

clean:
	rm -f *.o my_graph my_Knapsack graph_lib.a

# ~ graph ~
my_graph: my_graph.o graph_lib.a
	gcc -Wall -o my_graph my_graph.o ./graph_lib.a

my_graph.o: my_graph.c 
	gcc -Wall -c my_graph.c -o my_graph.o 


# ~ Knapsack ~
my_Knapsack: my_Knapsack.o
	gcc -Wall -o my_Knapsack my_Knapsack.o

my_Knapsack.o: my_Knapsack.c 
	gcc -Wall -c my_Knapsack.c -o my_Knapsack.o

# ~ common lib ~

my_mat.o: my_mat.c my_mat.h
	gcc -Wall -c my_mat.c -o my_mat.o

graph_lib.a: my_mat.o
	ar rc graph_lib.a my_mat.o
	ranlib graph_lib.a

