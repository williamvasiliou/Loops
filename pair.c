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
	fputc('(', stream);						\
	print(stream, item->first);					\
	fputs(" " #name " ", stream);					\
	print(stream, item->second);					\
	fputc(')', stream)

void print(FILE *stream, const pair *item) {
	switch (item->is) {
		case PAIR_NAME:
			fprintf(stream, "i[%zu]", item->name);
			return;
		case PAIR_VALUE:
			fprintf(stream, "%u", item->value);
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

uint8_t in[SIZE] = {};
uint8_t out[SIZE] = {};
uint8_t first[SIZE] = {};
uint8_t second[SIZE] = {};
uint8_t sum[SIZE] = {};

struct first {
	char *in;
	char *out;
	char *name;
	size_t index;
	size_t next;
	size_t size;
};

struct second {
	uint8_t in[SIZE];
	uint8_t out[SIZE];
	uint8_t first[SIZE];
	uint8_t second[SIZE];
	bool name;
	bool next;
	char *value;
};

bool set(pair **items, const struct first *index, const struct second *next) {
	FILE *stream = fopen(index->in, "r");
	if (stream) {
		size_t i = 0;
		for (size_t j = 0; j < SIZE; ++j) {
			in[j] = next->in[j];
			out[j] = next->out[j];
		}

		if (next->name) {
			size_t j = 0;
			char *name = index->name;
			char value = *name;
			while (value && j < SIZE) {
				first[j] = value;
				++j;

				++name;
				value = *name;
			}

			while (j < SIZE) {
				first[j] = next->first[j];
				++j;
			}

			for (j = 0; j < SIZE; ++j) {
				second[j] = next->second[j];
			}

			j = ITEMS - 1;
			while (i < SIZE) {
				for (size_t k = 0; k < PAIR; ++k) {
					pop(items[j], first);
					second[i] = (second[i] << 1) | (second[i] >> 7);
					second[i] ^= items[j]->value;
					--j;
				}
				++i;
			}

			if (++i >= SIZE) {
				i = 0;
			}

			for (j = 0; j < SIZE; ++j) {
				size_t k = ITEMS - 1;

				const size_t l = i;
				while (i < SIZE) {
					for (size_t l = 0; l < PAIR; ++l) {
						pop(items[k], second);
						first[i] = (first[i] << 1) | (first[i] >> 7);
						first[i] ^= items[k]->value;
						--k;
					}
					++i;
				}

				i = 0;
				while (i < l) {
					for (size_t l = 0; l < PAIR; ++l) {
						pop(items[k], second);
						first[i] = (first[i] << 1) | (first[i] >> 7);
						first[i] ^= items[k]->value;
						--k;
					}
					++i;
				}

				k = ITEMS - 1;
				while (i < SIZE) {
					for (size_t l = 0; l < PAIR; ++l) {
						pop(items[k], first);
						second[i] = (second[i] << 1) | (second[i] >> 7);
						second[i] ^= items[k]->value;
						--k;
					}
					++i;
				}

				i = 0;
				while (i < l) {
					for (size_t l = 0; l < PAIR; ++l) {
						pop(items[k], first);
						second[i] = (second[i] << 1) | (second[i] >> 7);
						second[i] ^= items[k]->value;
						--k;
					}
					++i;
				}

				if (++i >= SIZE) {
					i = 0;
				}
			}
			i = 0;
		}

		size_t size = fread(in, 1, SIZE, stream);
		while (size > 0) {
			if (ferror(stream)) {
				fclose(stream);
				return false;
			} else {
				size_t j = 0;

				if (size < SIZE) {
					if (feof(stream)) {
						size_t i = size;
						while (i < SIZE) {
							in[i] = out[j];
							++i;
							++j;
						}
						j = 0;
					} else {
						fclose(stream);
						return false;
					}
				}

				const size_t k = i;
				while (i < SIZE) {
					for (size_t k = 0; k < PAIR; ++k) {
						pop(items[j], in);
						out[i] = (out[i] << 1) | (out[i] >> 7);
						out[i] ^= items[j]->value;
						++j;
					}
					++i;
				}

				i = 0;
				while (i < k) {
					for (size_t k = 0; k < PAIR; ++k) {
						pop(items[j], in);
						out[i] = (out[i] << 1) | (out[i] >> 7);
						out[i] ^= items[j]->value;
						++j;
					}
					++i;
				}

				if (++i >= SIZE) {
					i = 0;
				}
			}

			size = fread(in, 1, SIZE, stream);
		}

		if (ferror(stream)) {
			fclose(stream);
			return false;
		} else if (feof(stream)) {
			fclose(stream);

			for (size_t j = 0; j < SIZE; ++j) {
				size_t k = 0;

				const size_t l = i;
				while (i < SIZE) {
					for (size_t l = 0; l < PAIR; ++l) {
						pop(items[k], out);
						in[i] = (in[i] << 1) | (in[i] >> 7);
						in[i] ^= items[k]->value;
						++k;
					}
					++i;
				}

				i = 0;
				while (i < l) {
					for (size_t l = 0; l < PAIR; ++l) {
						pop(items[k], out);
						in[i] = (in[i] << 1) | (in[i] >> 7);
						in[i] ^= items[k]->value;
						++k;
					}
					++i;
				}

				k = 0;
				while (i < SIZE) {
					for (size_t l = 0; l < PAIR; ++l) {
						pop(items[k], in);
						out[i] = (out[i] << 1) | (out[i] >> 7);
						out[i] ^= items[k]->value;
						++k;
					}
					++i;
				}

				i = 0;
				while (i < l) {
					for (size_t l = 0; l < PAIR; ++l) {
						pop(items[k], in);
						out[i] = (out[i] << 1) | (out[i] >> 7);
						out[i] ^= items[k]->value;
						++k;
					}
					++i;
				}

				if (++i >= SIZE) {
					i = 0;
				}
			}

			if (next->name) {
				for (size_t j = 0; j < SIZE; ++j) {
					out[j] ^= second[j];
				}
			}

			stream = fopen(index->out, "r");
			if (stream) {
				if (fseek(stream, index->index, SEEK_SET)) {
					perror(index->out);
					fclose(stream);
					return false;
				} else {
					size = fread(in, 1, index->size, stream);

					if (ferror(stream) || !size) {
						fclose(stream);
						return false;
					} else if (size < index->size) {
						if (feof(stream)) {
							i = 0;
							size_t j = index->next;
							while (i < size) {
								sum[j] = in[i] ^ out[j];
								++i;
								++j;
							}

							while (i < index->size) {
								sum[j] = out[j];
								++i;
								++j;
							}
						} else {
							fclose(stream);
							return false;
						}
					} else {
						i = 0;
						size_t j = index->next;
						while (i < index->size) {
							sum[j] = in[i] ^ out[j];
							++i;
							++j;
						}
					}
				}

				fclose(stream);
			} else {
				perror(index->out);
				return false;
			}

			return true;
		} else {
			fclose(stream);
			return false;
		}
	} else {
		perror(index->in);
	}

	return false;
}

bool get(FILE *stream, const struct first **first, const struct second *second, size_t size) {
	pair **items = (pair **) calloc(ITEMS, sizeof(pair *));
	if (items) {
		size_t i = 0;
		while (i < ITEMS) {
			pair *item = new();
			if (item) {
				items[i] = item;
				++i;
			} else {
				for (size_t j = 0; j < i; ++j) {
					delete(items[j]);
				}
				free(items);
				return false;
			}
		}

		i = 0;
		while (i < size) {
			if (set(items, first[i], second)) {
				++i;
			} else {
				for (i = 0; i < ITEMS; ++i) {
					delete(items[i]);
				}
				free(items);
				return false;
			}
		}

		fprintf(stream, "#include <stdint.h>\n\n#define PAIR %zu\n#define SIZE %zu\n#define ITEMS PAIR * SIZE\n\nuint8_t in[SIZE] = {%u", (size_t) PAIR, (size_t) SIZE, second->in[0]);
		for (i = 1; i < SIZE; ++i) {
			fprintf(stream, ", %u", second->in[i]);
		}

		fprintf(stream, "};\n\nuint8_t out[SIZE] = {%u", second->out[0]);
		for (i = 1; i < SIZE; ++i) {
			fprintf(stream, ", %u", second->out[i]);
		}

		if (second->name) {
			fprintf(stream, "};\n\nuint8_t first[SIZE] = {%u", second->first[0]);
			for (i = 1; i < SIZE; ++i) {
				fprintf(stream, ", %u", second->first[i]);
			}

			fprintf(stream, "};\n\nuint8_t second[SIZE] = {%u", second->second[0]);
			for (i = 1; i < SIZE; ++i) {
				fprintf(stream, ", %u", second->second[i]);
			}
		}

		fprintf(stream, "};\n\nuint8_t sum[SIZE] = {%u", sum[0]);
		for (i = 1; i < SIZE; ++i) {
			fprintf(stream, ", %u", sum[i]);
		}

		fputs("};\n\nuint8_t item0(const uint8_t *i) {\n\treturn ", stream);
		print(stream, items[0]);
		for (i = 1; i < ITEMS; ++i) {
			fprintf(stream, ";\n}\n\nuint8_t item%zu(const uint8_t *i) {\n\treturn ", i);
			print(stream, items[i]);
		}

		fputs(";\n}\n\nuint8_t (* items[ITEMS])(const uint8_t *i) = {item0", stream);
		delete(items[0]);
		for (i = 1; i < ITEMS; ++i) {
			fprintf(stream, ", item%zu", i);
			delete(items[i]);
		}

		fputs("};\n", stream);
		free(items);
		return true;
	}

	return false;
}

struct index {
	struct first **first;
	const struct second *second;
	size_t size;
	size_t sum;
};
