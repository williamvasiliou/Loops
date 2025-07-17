#include <stdint.h>
#include <sys/random.h>

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

void splice(uint8_t *out, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		size_t i = index >> 3;
		const uint8_t j = index & 7;

		index = 0;
		size_t k = 0;
		if (j) {
			const uint8_t l = 8 - j;
			while (k < size) {
				out[index] = in[i] << j;
				k += l;
				++i;
				if (k < size) {
					out[index] |= in[i] >> l;
					k += j;
				}
				++index;
			}
		} else {
			while (k < size) {
				out[index] = in[i];
				++i;
				++index;
				k += 8;
			}
		}

		if (k > size) {
			out[index - 1] &= (uint8_t) -1 << (k - size);
		}
	}
}
