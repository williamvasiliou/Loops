#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

#define PAIR_NAME  0
#define PAIR_VALUE 1
#define PAIR_AND   2
#define PAIR_OR    3
#define PAIR_XOR   4

#define PAIR 8
#define SIZE 256
#define ITEMS PAIR * SIZE

typedef struct pair pair;

struct pair {
	uint8_t is;
	pair *first;
	pair *second;
	size_t name;
	uint8_t value;
};

uint8_t byte() {
	static uint8_t i[1];
	ssize_t nread = getrandom(i, 1, 0);
	while (nread != 1) {
		nread = getrandom(i, 1, 0);
	}
	return i[0];
}

uintmax_t integer() {
	uintmax_t i = byte();
	for (size_t j = 1; j < sizeof(uintmax_t); ++j) {
		i <<= 8;
		i |= byte();
	}
	return i;
}

void shift(pair *item, size_t *name) {
	if (byte() & 1) {
		item->is = PAIR_NAME;
		item->name = (*name)++;
	} else {
		item->is = PAIR_VALUE;
		item->value = byte();

		while (!item->value) {
			item->value = byte();
		}
	}
}

void create(pair *item, uint8_t first, uint8_t second, size_t *name) {
	if (first == PAIR_XOR && second == PAIR_NAME) {
		shift(item, name);
	} else {
		item->is = PAIR_NAME;
		item->name = (*name)++;
	}
}

void push(pair *first, pair *second, size_t *name) {
	if (second->is == PAIR_NAME) {
		switch (integer() % 3) {
			case 0:
				first->is = PAIR_AND;
				break;
			case 1:
				first->is = PAIR_OR;
				break;
			default:
				first->is = PAIR_XOR;
				break;
		}
	} else {
		first->is = PAIR_XOR;
	}

	if (byte() & 1) {
		first->first = second;
		first->second = (pair *) calloc(1, sizeof(pair));
		create(first->second, first->is, second->is, name);
	} else {
		first->first = (pair *) calloc(1, sizeof(pair));
		first->second = second;
		create(first->first, first->is, second->is, name);
	}
}

void update(pair **first, pair *second, size_t *name) {
	switch (second->is) {
		case PAIR_NAME:
		case PAIR_VALUE:
			*first = (pair *) calloc(1, sizeof(pair));
			push(*first, second, name);
			return;
		default:
			if (byte() & 1) {
				update(&(second->first), second->first, name);
			} else {
				update(&(second->second), second->second, name);
			}
			return;
	}
}

void splice(pair *first, const size_t *in) {
	switch (first->is) {
		case PAIR_NAME:
			first->name = in[first->name];
			return;
		case PAIR_AND:
		case PAIR_OR:
		case PAIR_XOR:
			splice(first->first, in);
			splice(first->second, in);
			return;
		default:
			return;
	}
}

pair *new() {
	pair *first = (pair *) calloc(1, sizeof(pair));
	size_t name = 1;
	shift(first, &name);
	while (name < SIZE) {
		update(&first, first, &name);
	}

	pair *second = (pair *) calloc(1, sizeof(pair));
	second->is = PAIR_XOR;
	if (byte() & 1) {
		second->first = first;
		second->second = (pair *) calloc(1, sizeof(pair));
	} else {
		second->first = (pair *) calloc(1, sizeof(pair));
		second->second = first;
	}

	static size_t in[SIZE];
	for (size_t i = 0; i < SIZE; ++i) {
		in[i] = i;
	}

	while (name > 1) {
		const size_t index = integer() % name;
		const size_t value = in[index];

		--name;
		in[index] = in[name];
		in[name] = value;
	}
	splice(second, in);
	return second;
}

void delete(pair *first) {
	switch (first->is) {
		case PAIR_NAME:
		case PAIR_VALUE:
			free(first);
			return;
		default:
			delete(first->first);
			delete(first->second);
			free(first);
			return;
	}
}

#define PRINT(name)							\
	printf("(");							\
	print(item->first);						\
	printf(" " #name " ");						\
	print(item->second);						\
	printf(")");

void print(const pair *item) {
	switch (item->is) {
		case PAIR_NAME:
			printf("i[%zu]", item->name);
			return;
		case PAIR_VALUE:
			printf("%u", item->value);
			return;
		case PAIR_AND:
			PRINT(&);
			return;
		case PAIR_OR:
			PRINT(|);
			return;
		case PAIR_XOR:
			PRINT(^);
			return;
	}
}

#define POP(name)							\
	pop(item->first, in);						\
	pop(item->second, in);						\
	item->value = item->first->value name item->second->value

void pop(pair *item, const uint8_t *in) {
	switch (item->is) {
		case PAIR_NAME:
			item->value = in[item->name];
			return;
		case PAIR_AND:
			POP(&);
			return;
		case PAIR_OR:
			POP(|);
			return;
		case PAIR_XOR:
			POP(^);
			return;
		default:
			return;
	}
}
