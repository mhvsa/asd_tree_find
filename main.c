#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


letter_t *cs_by_level(letter_t *array, size_t size, unsigned int max) {

    int *counting_array = calloc(max + 1, sizeof(int));

    letter_t *sorted_array = calloc(size, sizeof(letter_t));

    for (int i = 0; i < size; ++i) {
        ++counting_array[array[i].level];
    }

    for (int j = 1; j < max + 1; j++) {
        counting_array[j] = counting_array[j] + counting_array[j - 1];
    }

    for (int k = size - 1; k > -1; k--) {
        sorted_array[counting_array[array[k].level] - 1] = array[k];
        --counting_array[array[k].level];
    }

    free(counting_array);
//    free(array);

    return sorted_array;

}

unsigned int findMaxLevel(letter_t *array, size_t size) {

    unsigned int max = array[0].level;

    for (int i = 1; i < size; ++i) {
        if (array[i].level > max) {
            max = array[i].level;
        }
    }

    return max;

}

unsigned int lr_sort(letter_t *array, unsigned int level, unsigned int level_marker, unsigned int n) {

    unsigned int TMP[] = {0, 0};

    letter_t *array_cpy = calloc(n - level_marker - 1, sizeof(letter_t));
    int l = 0;

    for (int i = level_marker + 1; i < n; i++) {
        array_cpy[l] = array[i];
        l++;
    }

    for (int j = level_marker + 1; j < n; j++) {
        if (array[j].road[level] == 'L') {
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
        if (array_cpy[l].road[level] == 'L') {
            array[TMP[0]--] = array_cpy[l];
        } else {
            array[TMP[1]--] = array_cpy[l];
        }
    }

    free(array_cpy);

    return border;

}

void findParent(letter_t *array, unsigned int level, unsigned int level_marker, unsigned int n, unsigned int border) {
    if (level_marker + 1 != border) {
        array[level_marker + 1].parent = &array[level_marker];
    }

    if (border != n) {
        array[border].parent = &array[level_marker];
    }
}

void process(letter_t *array, unsigned int level, unsigned int level_marker, unsigned int n, leaf_t *leaves) {
    if (n - level_marker == 0) {
        return;
    }
    if (n - level_marker == 1) {
        push(leaves, &array[level_marker]);
    } else {
        unsigned int border = lr_sort(array, level, level_marker, n);
        findParent(array, level, level_marker, n, border);
        process(array, level + 1, level_marker + 1, border, leaves); // Left side
        process(array, level + 1, border, n, leaves); // Right side
    }
}

letter_t *read(int size, char **input) {


    int i = 0, j, k = 1;
    letter_t *array = calloc(size, sizeof(letter_t));

    unsigned int level;
    char read;
    int args = size + 1;
    while (k < args) {

        char *test = input[k];

        size_t index = k-1;
        array[index].character = test[0];
        array[]
//        letter_t letter = array[k - 1];
//        letter.character = test[0];
//        letter.level = strlen(test + 2);
//        letter.road = test + 2;

        k++;

    }

    printf("%c",array[0].character);

    return array;
}


int main(int args, char **array) {

    printf("%i\n", args);
    printf("%s\n", array[1]);

    int size = args - 1;

    letter_t *t = read(size, array);

    printf("%c\n", t[0]);

    t = cs_by_level(t, size, findMaxLevel(t, size));
    for (int i = 0; i < size; ++i) {
        printf("%c ", t[i].character);
    }
    return 0;
}