#include "file.h"
#include <stdio.h>
#include <stdlib.h>

bool good(const char *line) {
	FILE *stream = fopen(line, "r");
	if (stream) {
		size_t i = 0;

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
						out[i] = (out[i] << 1) | (out[i] >> 7);
						out[i] ^= items[j](in);
						++j;
					}
					++i;
				}

				i = 0;
				while (i < k) {
					for (size_t k = 0; k < PAIR; ++k) {
						out[i] = (out[i] << 1) | (out[i] >> 7);
						out[i] ^= items[j](in);
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
						in[i] = (in[i] << 1) | (in[i] >> 7);
						in[i] ^= items[k](out);
						++k;
					}
					++i;
				}

				i = 0;
				while (i < l) {
					for (size_t l = 0; l < PAIR; ++l) {
						in[i] = (in[i] << 1) | (in[i] >> 7);
						in[i] ^= items[k](out);
						++k;
					}
					++i;
				}

				k = 0;
				while (i < SIZE) {
					for (size_t l = 0; l < PAIR; ++l) {
						out[i] = (out[i] << 1) | (out[i] >> 7);
						out[i] ^= items[k](in);
						++k;
					}
					++i;
				}

				i = 0;
				while (i < l) {
					for (size_t l = 0; l < PAIR; ++l) {
						out[i] = (out[i] << 1) | (out[i] >> 7);
						out[i] ^= items[k](in);
						++k;
					}
					++i;
				}

				if (++i >= SIZE) {
					i = 0;
				}
			}

			for (i = 0; i < SIZE; ++i) {
				fputc(out[i] ^ sum[i], stdout);
			}
			return true;
		} else {
			fclose(stream);
			return false;
		}
	} else {
		perror(line);
	}

	return false;
}

int main() {
	char *line = (char *) NULL;
	size_t size = 0;
	ssize_t nread = getline(&line, &size, stdin);
	bool is = false;
	if (nread > 0) {
		line[nread - 1] = 0;
		is = good(line);
	}

	free(line);
	if (is) {
		exit(EXIT_SUCCESS);
	} else {
		exit(EXIT_FAILURE);
	}
}
