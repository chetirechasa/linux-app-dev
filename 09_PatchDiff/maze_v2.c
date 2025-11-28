#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char WALL = '#';
char PATH = '.';

int SIZE = 6;

int **grid;

typedef struct {
    int x, y;
} Point;

Point *stack;
int stack_size = 0;

void init_grid() {
    for (int i = 0; i < 2*SIZE+1; i++) {
        for (int j = 0; j < 2*SIZE+1; j++) {
            grid[i][j] = (i % 2 == 0 || j % 2 == 0) ? 1 : 0;
        }
    }
}

void push(Point p) {
    stack[stack_size++] = p;
}

Point pop() {
    return stack[--stack_size];
}

int in_bounds(int x, int y) {
    return x > 0 && x < 2*SIZE && y > 0 && y < 2*SIZE;
}

void generate_maze() {
    srand(time(NULL));
    
    Point start = {1, 1};
    grid[start.y][start.x] = 0;
    push(start);
    
    int directions[4][2] = {{2, 0}, {-2, 0}, {0, 2}, {0, -2}};
    
    while (stack_size > 0) {
        Point current = pop();
        
        for (int i = 0; i < 4; i++) {
            int j = rand() % 4;
            int temp[2];
            temp[0] = directions[i][0];
            temp[1] = directions[i][1];
            directions[i][0] = directions[j][0];
            directions[i][1] = directions[j][1];
            directions[j][0] = temp[0];
            directions[j][1] = temp[1];
        }
        
        for (int i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];
            
            if (in_bounds(nx, ny) && grid[ny][nx] == 1) {
                int wx = current.x + directions[i][0]/2;
                int wy = current.y + directions[i][1]/2;
                grid[wy][wx] = 0;
                grid[ny][nx] = 0;
                
                Point next = {nx, ny};
                push(current);
                push(next);
                break;
            }
        }
    }
}

void print_maze() {
    for (int i = 0; i < 2*SIZE+1; i++) {
        for (int j = 0; j < 2*SIZE+1; j++) {
            putchar(grid[i][j] ? WALL : PATH);
        }
        putchar('\n');
    }
}

int main(int argc, char *argv[]) {
    // Обработка аргументов: первый - символы, второй - размер
    if (argc > 1) {
        if (strlen(argv[1]) >= 2) {
            WALL = argv[1][0];
            PATH = argv[1][1];
        }
    }
    if (argc > 2) {
        SIZE = atoi(argv[2]);
        if (SIZE < 2) SIZE = 6;
    }
    
    // Выделение памяти для динамического размера
    grid = malloc((2*SIZE+1) * sizeof(int*));
    for (int i = 0; i < 2*SIZE+1; i++) {
        grid[i] = malloc((2*SIZE+1) * sizeof(int));
    }
    stack = malloc((2*SIZE+1) * (2*SIZE+1) * sizeof(Point));
    
    init_grid();
    generate_maze();
    print_maze();
    
    // Освобождение памяти
    for (int i = 0; i < 2*SIZE+1; i++) {
        free(grid[i]);
    }
    free(grid);
    free(stack);
    
    return 0;
}

