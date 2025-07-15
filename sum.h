#include <stdint.h>

void splice(uint8_t *out, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		size_t i = index >> 3;
		const size_t j = index & 7;

		index = 0;
		size_t k = 0;
		if (j) {
			const size_t l = 8 - j;
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
