#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define getcx getchar_unlocked

// letter struct

typedef struct letter {

    struct letter *parent;

    unsigned int level;

    char character;
    char *road;

    int csum;

} letter_t;

// leaves struct

typedef struct leaf {

    letter_t *letter;

    struct leaf_t *next;

} leaf_t;

// leaves operands

letter_t *pop(leaf_t *first) {

    leaf_t *current = first;
    leaf_t *before = current;

    while (current->next != NULL) {
        before = current;
        current = current->next;
    }


    letter_t *leaf_letter = current->letter;

    before->next = NULL;

    return leaf_letter;

}

void push(leaf_t *first, letter_t *insert) {

    leaf_t *current = first;

    while (current->next != NULL) {
        current = current->next;
    }

    leaf_t *leaf = malloc(sizeof(leaf_t));
    leaf->next = NULL;
    leaf->letter = insert;
    current->next = leaf;


}


letter_t **cs_by_level(letter_t **array, size_t size, unsigned int max) {

    int *counting_array = calloc(max + 1, sizeof(int));

    letter_t **sorted_array = calloc(size, sizeof(letter_t *));

    for (int i = 0; i < size; ++i) {
        ++counting_array[array[i]->level];
    }

    for (int j = 1; j < max + 1; j++) {
        counting_array[j] = counting_array[j] + counting_array[j - 1];
    }

    for (int k = size - 1; k > -1; k--) {
        sorted_array[counting_array[array[k]->level] - 1] = array[k];
        --counting_array[array[k]->level];
    }

    free(counting_array);

    return sorted_array;

}

unsigned int findMaxLevel(letter_t **array, size_t size) {

    unsigned int max = array[0]->level;

    for (int i = 1; i < size; ++i) {
        if (array[i]->level > max) {
            max = array[i]->level;
        }
    }

    return max;

}

unsigned int lr_sort(letter_t **array, unsigned int level, unsigned int level_marker, unsigned int n) {

    unsigned int TMP[] = {0, 0};

    letter_t **array_cpy = calloc(n - level_marker - 1, sizeof(letter_t *));
    int l = 0;

    for (int i = level_marker + 1; i < n; i++) {
        array_cpy[l] = array[i];
        l++;
    }

    for (int j = level_marker + 1; j < n; j++) {
        if (array[j]->road[level] == 'L') {
            ++TMP[0];
        } else {
            ++TMP[1];
        }
    }


    TMP[0] = TMP[0] + level_marker;
    unsigned int border = TMP[0] + 1;
    TMP[1] = TMP[1] + TMP[0];

    l = n - level_marker - 2;

    for (l; l > -1; l--) {
        if (array_cpy[l]->road[level] == 'L') {
            array[TMP[0]--] = array_cpy[l];
        } else {
            array[TMP[1]--] = array_cpy[l];
        }
    }


    free(array_cpy);

    return border;

}

void findParent(letter_t **array, unsigned int level, unsigned int level_marker, unsigned int n, unsigned int border) {
    if (level_marker + 1 != border) {
        array[level_marker + 1]->parent = array[level_marker];
    }

    if (border != n) {
        array[border]->parent = array[level_marker];
    }
}


void setNewMaxLetter(letter_t *const *array, letter_t **max_p, letter_t *letter) {
    letter_t *max = max_p[0];
    letter_t *parent1 = letter->parent;
    letter_t *parent2 = max->parent;
    letter_t *first = array[0];
    int i = 0;

    if (letter->level > max->level) {

        while (parent1->character == parent2->character && i < letter->level) {
            parent1 = parent1->parent;
            parent2 = parent2->parent;
            i++;
        }

        if (i == letter->level) {
            max_p[0] = letter;
        } else if (parent1->character > parent2->character) {
            max_p[0] = letter;
        }
    } else if (letter->level < max->level) {
        while (parent1->character == parent2->character && i < max->level) {
            parent1 = parent1->parent;
            parent2 = parent2->parent;
            i++;
        }

        if (parent1->character > parent2->character) {
            max_p[0] = letter;
        }
    } else {

        while (parent1->character == parent2->character && i < max->level) {
            parent1 = parent1->parent;
            parent2 = parent2->parent;
            i++;
        }

        if (i != letter->level) {
            if (parent1->character > parent2->character) {
                max_p[0] = letter;
            }
        }
    }
}


void process(letter_t **array, unsigned int level, unsigned int level_marker, unsigned int n, letter_t **max_p) {
    if (n - level_marker == 0) {
        return;
    }
    if (n - level_marker == 1) {

        letter_t *letter = array[level_marker];

        if (letter->character > max_p[0]->character) {
            max_p[0] = array[level_marker];
        } else if (letter->character == max_p[0]->character) {
            setNewMaxLetter(array, max_p, letter);
        }


    } else {
        unsigned int border = lr_sort(array, level, level_marker, n);
        findParent(array, level, level_marker, n, border);
        process(array, level + 1, level_marker + 1, border, max_p); // Left side
        process(array, level + 1, border, n, max_p); // Right side
    }
}

letter_t **read(int *size, unsigned int *csum) {


    int i, level;

    int counter = 0;
    char *line;
    int read;

    leaf_t *head = malloc(sizeof(leaf_t));
    head->next = NULL;

    while ((read = getcx()) != EOF) {
        line = calloc(66, sizeof(char));
        i = 0;
        while (read != 10) {
            line[i] = read;
            read = getcx();
            i++;
        }

        letter_t *letter = malloc(sizeof(letter_t));

        letter->character = line[0];
        letter->road = line + 2;
        level = strlen(line + 2);
        letter->level = level;

        *csum = *csum + level + 1;

        push(head, letter);

        counter++;
    }


    letter_t **array = calloc(counter, sizeof(letter_t *));

    *size = counter;

    for (int l = 0; l < counter; l++) {
        array[l] = pop(head);
    }

    return array;
}


int main() {

    clock_t start = clock();

    int size = 0;
    int csum = 0;

    letter_t **array = read(&size, &csum);
    array = cs_by_level(array, size, findMaxLevel(array, size));

    letter_t *max = malloc(sizeof(letter_t));
    max->character = 0;
    letter_t *max_p[1] = {max};

    process(array, 0, 0, size, max_p);

    letter_t *letter = max_p[0];

    while (letter != array[0]) {
        printf("%c", letter->character);
        letter = letter->parent;
    }
    printf("%c%c%i", array[0]->character, 10, csum);

    // TEN FRAGMENT POZNIEJ USUN!!

    clock_t end = clock();

    printf("\n%f",end-start);


    return 0;
}