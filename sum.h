#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <sys/stat.h>
#include <sys/types.h>

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
			const uint8_t l = k & 7;
			if (l) {
				out[index - 1] &= (uint8_t) -1 << (8 - l + k - size);
			} else {
				out[index - 1] &= (uint8_t) -1 << (k - size);
			}
		}
	}
}

#define PAIR_FIRST 0
#define PAIR_SECOND 1

#define about 0
#define aboutAnDay 1
#define aboutAnYouHimTimeMake 2
#define aboutAnyTheirThere 3
#define aboutAsWhatTheOnlyHimWill 4
#define aboutAtLikeToThem 5
#define aboutAtToNotWellOnWork 6
#define aboutBackOnHim 7
#define aboutBackSeeWould 8
#define aboutBe 9
#define aboutBeTheirNotUpAs 10
#define aboutBut 11
#define aboutBy 12
#define aboutCanThem 13
#define aboutEvenUseBackWhenHis 14
#define aboutGiveUsGoodKnowShe 15
#define aboutGoOn 16
#define aboutGoPeopleYearHimAllWork 17
#define aboutHaveOurNoFrom 18
#define aboutHeSeeOur 19
#define aboutHowHaveOn 20
#define aboutIntoThisThisButCould 21
#define aboutItSayFromThis 22
#define aboutItsBecauseOr 23
#define aboutItsPeopleWellOther 24

typedef struct pair pair;

struct pair {
	uint8_t is;
	union {
		uint8_t *first;
		pair **second;
	};
	size_t size;
	size_t sum;
	size_t value;
};

pair *new() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(1, sizeof(pair *));
	item->sum = 1;
	return item;
}

pair *aboutKnowTwoOverNowAfterThe(uint8_t value) {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_FIRST;
	item->first = (uint8_t *) calloc(1, sizeof(uint8_t));
	item->value = value;
	return item;
}

pair *aboutKnowYourHim(const uint8_t *in, size_t index, size_t size, uint8_t value) {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_FIRST;
	if (size > 0) {
		item->first = (uint8_t *) calloc(1 + ((size - 1) >> 3), sizeof(uint8_t));
		item->size = size;
		splice(item->first, in, index, size);
	} else {
		item->first = (uint8_t *) calloc(1, sizeof(uint8_t));
	}
	item->value = value;
	return item;
}

pair *aboutLike(const uint8_t *in, size_t index, size_t size);

pair *aboutLikeWellNo(const uint8_t *in, size_t index, size_t size);

pair *aboutMake(const uint8_t *in, size_t index, size_t size);

pair *aboutMeWellWho(const uint8_t *in, size_t index, size_t size);

pair *aboutMyBe(const uint8_t *in, size_t index, size_t size);

pair *aboutNotMakeAndHowGoOther(const uint8_t *in, size_t index, size_t size);

pair *aboutNotUseGiveWantNo(const uint8_t *in, size_t index, size_t size) {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(1, sizeof(pair *));
	item->sum = 1;

	size_t i = 0;
	while (i < size) {
		if (item->size >= item->sum) {
			item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
			item->sum *= 2;
		}

		const size_t j = integer() % (size - i + 1);
		switch (integer() % 6) {
			case 0:
				item->second[item->size] = aboutLike(in, index + i, j);
				break;
			case 1:
				item->second[item->size] = aboutLikeWellNo(in, index + i, j);
				break;
			case 2:
				item->second[item->size] = aboutMake(in, index + i, j);
				break;
			case 3:
				item->second[item->size] = aboutMeWellWho(in, index + i, j);
				break;
			case 4:
				item->second[item->size] = aboutMyBe(in, index + i, j);
				break;
			default:
				item->second[item->size] = aboutNotMakeAndHowGoOther(in, index + i, j);
				break;
		}
		i += j;
		++item->size;
	}

	item->value = aboutAnDay;
	return item;
}

pair *aboutOnLikeGoBecauseAtDay(const uint8_t *in, size_t index, size_t size);

pair *aboutOnlyHimSayComeSheAnd(const uint8_t *in, size_t index, size_t size);

pair *aboutOnlySoBeOrWhen(const uint8_t *in, size_t index, size_t size) {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(1, sizeof(pair *));
	item->sum = 1;

	bool sum = 1;
	size_t i = 0;
	while (i < size) {
		if (item->size >= item->sum) {
			item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
			item->sum *= 2;
		}

		const size_t j = integer() % (size - i + 1);
		if (sum) {
			switch (byte() & 1) {
				case 0:
					item->second[item->size] = aboutOnLikeGoBecauseAtDay(in, index + i, j);
					break;
				default:
					item->second[item->size] = aboutOnlyHimSayComeSheAnd(in, index + i, j);
					sum = 0;
					break;
			}
		} else {
			item->second[item->size] = aboutOnLikeGoBecauseAtDay(in, index + i, j);
		}
		i += j;
		++item->size;
	}

	item->value = aboutAnyTheirThere;
	return item;
}

pair *aboutOther(const uint8_t *in, size_t index, size_t size) {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(1, sizeof(pair *));
	item->sum = 1;

	size_t i = 0;
	while (i < size) {
		if (item->size >= item->sum) {
			item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
			item->sum *= 2;
		}

		size_t j = integer() % (size - i + 1);
		if (item->size > 0 && item->second[item->size - 1]->value != aboutGoOn) {
			if (j > 3) {
				j = 3;
			}
			item->second[item->size] = aboutKnowYourHim(in, index + i, j, aboutGoOn);
		} else {
			switch (integer() % 3) {
				case 0:
					if (j > 5) {
						j = 5;
					}
					item->second[item->size] = aboutKnowYourHim(in, index + i, j, aboutAsWhatTheOnlyHimWill);
					break;
				case 1:
					item->second[item->size] = aboutKnowYourHim(in, index + i, j, aboutBy);
					break;
				default:
					item->second[item->size] = aboutKnowYourHim(in, index + i, j, aboutGiveUsGoodKnowShe);
					break;
			}
		}
		i += j;
		++item->size;
	}

	if (item->size > 0 && item->second[item->size - 1]->value == aboutGoOn) {
		if (item->size >= item->sum) {
			item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
			item->sum *= 2;
		}

		switch (integer() % 3) {
			case 0:
				item->second[item->size] = aboutKnowTwoOverNowAfterThe(aboutAsWhatTheOnlyHimWill);
				break;
			case 1:
				item->second[item->size] = aboutKnowTwoOverNowAfterThe(aboutBy);
				break;
			default:
				item->second[item->size] = aboutKnowTwoOverNowAfterThe(aboutGiveUsGoodKnowShe);
				break;
		}

		++item->size;
	}

	item->value = aboutBe;
	return item;
}

pair *aboutOurTheseLookSeeTheTwo(const uint8_t *in, size_t index, size_t size);

pair *aboutPeople(const uint8_t *in, size_t index, size_t size) {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(1, sizeof(pair *));
	item->sum = 1;

	size_t i = 0;
	while (i < size) {
		if (item->size >= item->sum) {
			item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
			item->sum *= 2;
		}

		const size_t j = integer() % (size - i + 1);
		item->second[item->size] = aboutOurTheseLookSeeTheTwo(in, index + i, j);
		i += j;
		++item->size;
	}

	item->value = aboutHeSeeOur;
	return item;
}

pair *aboutSayAs(uint8_t value) {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(1, sizeof(pair *));
	item->sum = 1;
	item->value = value;
	return item;
}

pair *aboutSayBack() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(3, sizeof(pair *));
	item->second[0] = aboutKnowTwoOverNowAfterThe(aboutBy);
	item->second[1] = aboutKnowTwoOverNowAfterThe(aboutGoPeopleYearHimAllWork);
	item->second[2] = aboutSayAs(aboutBe);
	item->size = 3;
	item->sum = 3;
	item->value = about;
	return item;
}

pair *aboutLike(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(3, sizeof(pair *));
		size_t i = integer() % (size + 1);
		item->second[0] = aboutKnowYourHim(in, index, i, aboutBy);
		size_t j = i;
		i = integer() % (size - j + 1);
		if (i > 3) {
			i = 3;
		}
		item->second[1] = aboutKnowYourHim(in, index + j, i, aboutGoPeopleYearHimAllWork);
		j += i;
		item->second[2] = aboutOther(in, index + j, size - j);
		item->size = 3;
		item->sum = 3;
		item->value = about;
		return item;
	} else {
		return aboutSayBack();
	}
}

pair *aboutSayOnBecauseWellBeNo() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(2, sizeof(pair *));
	item->second[0] = aboutSayAs(aboutBe);
	item->second[1] = aboutSayAs(aboutAnDay);
	item->size = 2;
	item->sum = 2;
	item->value = aboutAnYouHimTimeMake;
	return item;
}

pair *aboutOnLikeGoBecauseAtDay(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(2, sizeof(pair *));
		const size_t i = integer() % (size + 1);
		item->second[0] = aboutOther(in, index, i);
		item->second[1] = aboutNotUseGiveWantNo(in, index + i, size - i);
		item->size = 2;
		item->sum = 2;
		item->value = aboutAnYouHimTimeMake;
		return item;
	} else {
		return aboutSayOnBecauseWellBeNo();
	}
}

pair *aboutSeeNotEven() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(1, sizeof(pair *));
	item->second[0] = aboutSayAs(aboutAnDay);
	item->size = 1;
	item->sum = 1;
	item->value = aboutAtLikeToThem;
	return item;
}

pair *aboutOnlyHimSayComeSheAnd(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(1, sizeof(pair *));
		item->second[0] = aboutNotUseGiveWantNo(in, index, size);
		item->size = 1;
		item->sum = 1;
		item->value = aboutAtLikeToThem;
		return item;
	} else {
		return aboutSeeNotEven();
	}
}

pair *aboutSheAboutMakeAllHer() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(2, sizeof(pair *));
	item->second[0] = aboutKnowTwoOverNowAfterThe(aboutBy);
	item->second[1] = aboutKnowTwoOverNowAfterThe(aboutGiveUsGoodKnowShe);
	item->size = 2;
	item->sum = 2;
	item->value = aboutAtToNotWellOnWork;
	return item;
}

pair *aboutTakeThereTheYouThenAs(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(2, sizeof(pair *));
		const size_t i = integer() % (size + 1);
		item->second[0] = aboutKnowYourHim(in, index, i, aboutBy);
		item->second[1] = aboutKnowYourHim(in, index + i, size - i, aboutGiveUsGoodKnowShe);
		item->size = 2;
		item->sum = 2;
		item->value = aboutAtToNotWellOnWork;
		return item;
	} else {
		return aboutSheAboutMakeAllHer();
	}
}

pair *aboutThanBeHowTwo() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(1, sizeof(pair *));
	item->second[0] = aboutSayAs(aboutAnDay);
	item->size = 1;
	item->sum = 1;
	item->value = aboutBackOnHim;
	return item;
}

pair *aboutThat(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(1, sizeof(pair *));
		item->second[0] = aboutNotUseGiveWantNo(in, index, size);
		item->size = 1;
		item->sum = 1;
		item->value = aboutBackOnHim;
		return item;
	} else {
		return aboutThanBeHowTwo();
	}
}

pair *aboutTheirHimAfterThese() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(2, sizeof(pair *));
	item->second[0] = aboutSayAs(aboutBe);
	item->second[1] = aboutSayAs(aboutAnDay);
	item->size = 2;
	item->sum = 2;
	item->value = aboutBackSeeWould;
	return item;
}

pair *aboutThereBackTake(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(2, sizeof(pair *));
		const size_t i = integer() % (size + 1);
		item->second[0] = aboutOther(in, index, i);
		item->second[1] = aboutNotUseGiveWantNo(in, index + i, size - i);
		item->size = 2;
		item->sum = 2;
		item->value = aboutBackSeeWould;
		return item;
	} else {
		return aboutTheirHimAfterThese();
	}
}

pair *aboutThisHowWithInto();

pair *aboutThisIntoAtIts() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(4, sizeof(pair *));
	item->second[0] = aboutThisHowWithInto();
	item->second[1] = aboutSayAs(aboutBe);
	item->second[2] = aboutSayBack();
	item->second[3] = aboutSayAs(aboutAnDay);
	item->size = 4;
	item->sum = 4;
	item->value = aboutBeTheirNotUpAs;
	return item;
}

pair *aboutLikeWellNo(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(4, sizeof(pair *));
		size_t i = integer() % (size + 1);
		item->second[0] = aboutMyBe(in, index, i);
		size_t j = i;
		i = integer() % (size - j + 1);
		item->second[1] = aboutOther(in, index + j, i);
		j += i;
		i = integer() % (size - j + 1);
		item->second[2] = aboutLike(in, index + j, i);
		j += i;
		item->second[3] = aboutNotUseGiveWantNo(in, index + j, size - j);
		item->size = 4;
		item->sum = 4;
		item->value = aboutBeTheirNotUpAs;
		return item;
	} else {
		return aboutThisIntoAtIts();
	}
}

pair *aboutThisMyYour() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(4, sizeof(pair *));
	item->second[0] = aboutKnowTwoOverNowAfterThe(aboutItSayFromThis);
	item->second[1] = aboutKnowTwoOverNowAfterThe(aboutBy);
	item->second[2] = aboutSayAs(aboutHeSeeOur);
	item->second[3] = aboutSayAs(aboutAnDay);
	item->size = 4;
	item->sum = 4;
	item->value = aboutBut;
	return item;
}

pair *aboutTime(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(4, sizeof(pair *));
		size_t i = integer() % (size + 1);
		if (i > 3) {
			i = 3;
		}
		item->second[0] = aboutKnowYourHim(in, index, i, aboutItSayFromThis);
		size_t j = i;
		i = integer() % (size - j + 1);
		item->second[1] = aboutKnowYourHim(in, index + j, i, aboutBy);
		j += i;
		i = integer() % (size - j + 1);
		item->second[2] = aboutPeople(in, index + j, i);
		j += i;
		item->second[3] = aboutNotUseGiveWantNo(in, index + j, size - j);
		item->size = 4;
		item->sum = 4;
		item->value = aboutBut;
		return item;
	} else {
		return aboutThisMyYour();
	}
}

pair *aboutTwoHeHave() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(2, sizeof(pair *));
	item->second[0] = aboutSayAs(aboutBe);
	item->second[1] = aboutSayAs(aboutAnDay);
	item->size = 2;
	item->sum = 2;
	item->value = aboutCanThem;
	return item;
}

pair *aboutMake(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(2, sizeof(pair *));
		size_t i = integer() % (size + 1);
		item->second[0] = aboutOther(in, index, i);
		size_t j = i;
		i = integer() % (size - j + 1);
		item->second[1] = aboutNotUseGiveWantNo(in, index + j, i);
		j += i;
		item->size = 2;
		item->sum = 2;
		while (j < size) {
			if (item->size >= item->sum) {
				item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
				item->sum *= 2;
			}

			switch (byte() & 1) {
				case 0:
					i = size - j;
					item->second[item->size] = aboutThat(in, index + j, i);
					break;
				default:
					i = integer() % (size - j + 1);
					item->second[item->size] = aboutThereBackTake(in, index + j, i);
					break;
			}

			j += i;
			++item->size;
		}
		item->value = aboutCanThem;
		return item;
	} else {
		return aboutTwoHeHave();
	}
}

pair *aboutUpItMyWouldWant() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(1, sizeof(pair *));
	item->second[0] = aboutKnowTwoOverNowAfterThe(aboutHowHaveOn);
	item->size = 1;
	item->sum = 1;
	item->value = aboutEvenUseBackWhenHis;
	return item;
}

pair *aboutUs(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(1, sizeof(pair *));
		item->second[0] = aboutKnowYourHim(in, index, size, aboutHowHaveOn);
		item->size = 1;
		item->sum = 1;
		item->value = aboutEvenUseBackWhenHis;
		return item;
	} else {
		return aboutUpItMyWouldWant();
	}
}

pair *aboutWellThisAndCanShe() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(2, sizeof(pair *));
	item->second[0] = aboutKnowTwoOverNowAfterThe(aboutItSayFromThis);
	item->second[1] = aboutKnowTwoOverNowAfterThe(aboutBy);
	item->size = 2;
	item->sum = 2;
	item->value = aboutHaveOurNoFrom;
	return item;
}

pair *aboutOurTheseLookSeeTheTwo(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(2, sizeof(pair *));
		size_t i = integer() % (size + 1);
		if (i > 3) {
			i = 3;
		}
		item->second[0] = aboutKnowYourHim(in, index, i, aboutItSayFromThis);
		item->second[1] = aboutKnowYourHim(in, index + i, size - i, aboutBy);
		item->size = 2;
		item->sum = 2;
		item->value = aboutHaveOurNoFrom;
		return item;
	} else {
		return aboutWellThisAndCanShe();
	}
}

pair *aboutWellUpComeSheTakeUs() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(2, sizeof(pair *));
	item->second[0] = aboutKnowTwoOverNowAfterThe(aboutBy);
	item->second[1] = aboutSayAs(aboutAnyTheirThere);
	item->size = 2;
	item->sum = 2;
	item->value = aboutIntoThisThisButCould;
	return item;
}

pair *aboutMeWellWho(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(2, sizeof(pair *));
		const size_t i = integer() % (size + 1);
		item->second[0] = aboutKnowYourHim(in, index, i, aboutBy);
		item->second[1] = aboutOnlySoBeOrWhen(in, index + i, size - i);
		item->size = 2;
		item->sum = 2;
		item->value = aboutIntoThisThisButCould;
		return item;
	} else {
		return aboutWellUpComeSheTakeUs();
	}
}

pair *aboutThisHowWithInto() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(3, sizeof(pair *));
	item->second[0] = aboutKnowTwoOverNowAfterThe(aboutItSayFromThis);
	item->second[1] = aboutKnowTwoOverNowAfterThe(aboutBy);
	item->second[2] = aboutSayAs(aboutBe);
	item->size = 3;
	item->sum = 3;
	item->value = aboutItsBecauseOr;
	return item;
}

pair *aboutMyBe(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(3, sizeof(pair *));
		size_t i = integer() % (size + 1);
		if (i > 3) {
			i = 3;
		}
		item->second[0] = aboutKnowYourHim(in, index, i, aboutItSayFromThis);
		size_t j = i;
		i = integer() % (size - j + 1);
		item->second[1] = aboutKnowYourHim(in, index + j, i, aboutBy);
		j += i;
		item->second[2] = aboutOther(in, index + j, size - j);
		item->size = 3;
		item->sum = 3;
		item->value = aboutItsBecauseOr;
		return item;
	} else {
		return aboutThisHowWithInto();
	}
}

pair *aboutWhenAnyUpYearThisHe() {
	pair *item = (pair *) calloc(1, sizeof(pair));
	item->is = PAIR_SECOND;
	item->second = (pair **) calloc(2, sizeof(pair *));
	item->second[0] = aboutSayAs(aboutBe);
	item->second[1] = aboutSayAs(aboutAnDay);
	item->size = 2;
	item->sum = 2;
	item->value = aboutItsPeopleWellOther;
	return item;
}

pair *aboutNotMakeAndHowGoOther(const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(2, sizeof(pair *));
		const size_t i = integer() % (size + 1);
		item->second[0] = aboutOther(in, index, i);
		item->second[1] = aboutNotUseGiveWantNo(in, index + i, size - i);
		item->size = 2;
		item->sum = 2;
		item->value = aboutItsPeopleWellOther;
		return item;
	} else {
		return aboutWhenAnyUpYearThisHe();
	}
}

void aboutWhenMake(pair *item, const uint8_t *in, size_t size) {
	if (size > 0) {
		size_t i = 1;
		if (item->size > 0) {
			i += (item->size - 1) >> 3;
		}

		size_t j = 1 + ((item->size + size - 1) >> 3);
		if (j > i) {
			item->first = (uint8_t *) reallocarray(item->first, j, sizeof(uint8_t));
		}

		size_t index = 0;
		i = item->size >> 3;
		j = 0;

		const uint8_t k = item->size & 7;
		if (k) {
			const uint8_t l = 8 - k;
			while (j < size) {
				item->first[i] |= in[index] >> k;
				j += l;
				if (j < size) {
					item->first[++i] = in[index] << l;
					j += k;
				}
				++index;
			}

			if (j > size) {
				const uint8_t value = size & 7;
				if (value) {
					const uint8_t j = (k + value) & 7;
					if (j) {
						item->first[i] &= (uint8_t) -1 << (8 - j);
					}
				} else {
					item->first[i] &= (uint8_t) -1 << l;
				}
			}
		} else {
			while (j < size) {
				item->first[i] = in[index];
				++index;
				++i;
				j += 8;
			}

			if (j > size) {
				item->first[i - 1] &= (uint8_t) -1 << (j - size);
			}
		}

		item->size += size;
	}
}

void aboutWillOtherUseNowJustWould(pair *item, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		uint8_t *out = (uint8_t *) calloc(1 + ((size - 1) >> 3), sizeof(uint8_t));
		splice(out, in, index, size);
		aboutWhenMake(item, out, size);
		free(out);
	}
}

void aboutWithMySomeThen(pair *item, const uint8_t *in, size_t index, size_t size);

void aboutWork(pair *item, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		size_t i = integer() % (size + 1);
		size_t j = 0;
		if (item->size > 0) {
			aboutWithMySomeThen(item->second[item->size - 1], in, index, i);
			j += i;
		}

		while (j < size) {
			if (item->size >= item->sum) {
				item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
				item->sum *= 2;
			}

			i = integer() % (size - j + 1);
			switch (integer() % 6) {
				case 0:
					item->second[item->size] = aboutLike(in, index + j, i);
					break;
				case 1:
					item->second[item->size] = aboutLikeWellNo(in, index + j, i);
					break;
				case 2:
					item->second[item->size] = aboutMake(in, index + j, i);
					break;
				case 3:
					item->second[item->size] = aboutMeWellWho(in, index + j, i);
					break;
				case 4:
					item->second[item->size] = aboutMyBe(in, index + j, i);
					break;
				default:
					item->second[item->size] = aboutNotMakeAndHowGoOther(in, index + j, i);
					break;
			}
			j += i;
			++item->size;
		}
	}
}

void aboutWould(pair *item, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		size_t i = integer() % (size + 1);
		size_t j = 0;
		if (item->size > 0) {
			switch (item->second[item->size - 1]->value) {
				case aboutBy:
				case aboutGiveUsGoodKnowShe:
					aboutWillOtherUseNowJustWould(item->second[item->size - 1], in, index, i);
					j += i;
					break;
				default:
					i = 0;
			}
		} else {
			if (item->size >= item->sum) {
				item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
				item->sum *= 2;
			}

			switch (integer() % 3) {
				case 0:
					if (i > 5) {
						i = 5;
					}
					item->second[0] = aboutKnowYourHim(in, index, i, aboutAsWhatTheOnlyHimWill);
					break;
				case 1:
					item->second[0] = aboutKnowYourHim(in, index, i, aboutBy);
					break;
				default:
					item->second[0] = aboutKnowYourHim(in, index, i, aboutGiveUsGoodKnowShe);
					break;
			}
			j += i;
			++item->size;
		}

		while (j < size) {
			if (item->size >= item->sum) {
				item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
				item->sum *= 2;
			}

			i = integer() % (size - j + 1);
			if (i > 3) {
				i = 3;
			}
			item->second[item->size] = aboutKnowYourHim(in, index + j, i, aboutGoOn);
			j += i;

			if (++item->size >= item->sum) {
				item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
				item->sum *= 2;
			}

			i = integer() % (size - j + 1);
			switch (integer() % 3) {
				case 0:
					if (i > 5) {
						i = 5;
					}
					item->second[item->size] = aboutKnowYourHim(in, index + j, i, aboutAsWhatTheOnlyHimWill);
					break;
				case 1:
					item->second[item->size] = aboutKnowYourHim(in, index + j, i, aboutBy);
					break;
				default:
					item->second[item->size] = aboutKnowYourHim(in, index + j, i, aboutGiveUsGoodKnowShe);
					break;
			}
			j += i;
			++item->size;
		}
	}
}

void aboutYearFrom(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWould(item->second[2], in, index, size);
}

void aboutYourThemAfterItsSayKnow(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWork(item->second[1], in, index, size);
}

void aboutYourWhat(pair *item, const uint8_t *in, size_t index, size_t size);

void after(pair *item, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		size_t i = integer() % (size + 1);
		size_t j = 0;
		if (item->size > 0) {
			switch (item->second[item->size - 1]->value) {
				case aboutAnYouHimTimeMake:
					aboutYourThemAfterItsSayKnow(item->second[item->size - 1], in, index, i);
					break;
				default:
					aboutYourWhat(item->second[item->size - 1], in, index, i);
					break;
			}
			j += i;
		}

		while (j < size) {
			if (item->size >= item->sum) {
				item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
				item->sum *= 2;
			}

			i = integer() % (size - j + 1);
			item->second[item->size] = aboutOnLikeGoBecauseAtDay(in, index + j, i);
			j += i;
			++item->size;
		}
	}
}

void aboutYourWhat(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWork(item->second[0], in, index, size);
}

void afterAboutBecauseMyIntoCouldAt(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWillOtherUseNowJustWould(item->second[1], in, index, size);
}

void afterAfterSay(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWork(item->second[0], in, index, size);
}

void afterAllEvenThereWhatOther(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWork(item->second[1], in, index, size);
}

void afterAnyAnyThereWhoGoodMake(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWork(item->second[3], in, index, size);
}

void afterAnyWill(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWork(item->second[3], in, index, size);
}

void afterAnyWouldHer(pair *item, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		size_t i = integer() % (size + 1);
		switch (item->second[item->size - 1]->value) {
			case aboutAnDay:
				aboutWork(item->second[item->size - 1], in, index, i);
				break;
			case aboutBackOnHim:
				afterAfterSay(item->second[item->size - 1], in, index, i);
				break;
			case aboutBackSeeWould:
				afterAllEvenThereWhatOther(item->second[item->size - 1], in, index, i);
				break;
			default:
				i = 0;
				break;
		}

		size_t j = i;
		while (j < size) {
			if (item->size >= item->sum) {
				item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
				item->sum *= 2;
			}

			i = integer() % (size - j + 1);
			switch (item->second[item->size - 1]->value) {
				case aboutAnDay:
				case aboutBackSeeWould:
					switch (byte() & 1) {
						case 0:
							item->second[item->size] = aboutThereBackTake(in, index + j, i);
							++item->size;
							break;
						default:
							item->second[item->size] = aboutThat(in, index + j, i);
							++item->size;
							break;
					}
					break;
				default:
					afterAfterSay(item->second[item->size - 1], in, index + j, i);
					break;
			}
			j += i;
		}
	}
}

void afterComeThen(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWillOtherUseNowJustWould(item->second[0], in, index, size);
}

void afterCouldAboutBeOther(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWillOtherUseNowJustWould(item->second[1], in, index, size);
}

void afterDayAnyPeopleGoWe(pair *item, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		size_t i = integer() % (size + 1);
		size_t j = 0;
		if (item->size > 0) {
			afterCouldAboutBeOther(item->second[item->size - 1], in, index, i);
			j += i;
		}

		while (j < size) {
			if (item->size >= item->sum) {
				item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
				item->sum *= 2;
			}

			i = integer() % (size - j + 1);
			item->second[item->size] = aboutOurTheseLookSeeTheTwo(in, index + j, i);
			j += i;
			++item->size;
		}
	}
}

void afterEvenHeNoWithUp(pair *item, const uint8_t *in, size_t index, size_t size) {
	after(item->second[1], in, index, size);
}

void afterEvenItsOnlyInto(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWould(item->second[2], in, index, size);
}

void afterEvenWellWithOneHim(pair *item, const uint8_t *in, size_t index, size_t size) {
	aboutWork(item->second[1], in, index, size);
}

void aboutWithMySomeThen(pair *item, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		switch (item->value) {
			case about:
				aboutYearFrom(item, in, index, size);
				break;
			case aboutAnDay:
				aboutWork(item, in, index, size);
				break;
			case aboutAnYouHimTimeMake:
				aboutYourThemAfterItsSayKnow(item, in, index, size);
				break;
			case aboutAnyTheirThere:
				after(item, in, index, size);
				break;
			case aboutAsWhatTheOnlyHimWill:
				aboutWillOtherUseNowJustWould(item, in, index, size);
				break;
			case aboutAtLikeToThem:
				aboutYourWhat(item, in, index, size);
				break;
			case aboutAtToNotWellOnWork:
				afterAboutBecauseMyIntoCouldAt(item, in, index, size);
				break;
			case aboutBackOnHim:
				afterAfterSay(item, in, index, size);
				break;
			case aboutBackSeeWould:
				afterAllEvenThereWhatOther(item, in, index, size);
				break;
			case aboutBe:
				aboutWould(item, in, index, size);
				break;
			case aboutBeTheirNotUpAs:
				afterAnyAnyThereWhoGoodMake(item, in, index, size);
				break;
			case aboutBut:
				afterAnyWill(item, in, index, size);
				break;
			case aboutBy:
				aboutWillOtherUseNowJustWould(item, in, index, size);
				break;
			case aboutCanThem:
				afterAnyWouldHer(item, in, index, size);
				break;
			case aboutEvenUseBackWhenHis:
				afterComeThen(item, in, index, size);
				break;
			case aboutGiveUsGoodKnowShe:
				aboutWillOtherUseNowJustWould(item, in, index, size);
				break;
			case aboutGoOn:
				aboutWillOtherUseNowJustWould(item, in, index, size);
				break;
			case aboutGoPeopleYearHimAllWork:
				aboutWillOtherUseNowJustWould(item, in, index, size);
				break;
			case aboutHaveOurNoFrom:
				afterCouldAboutBeOther(item, in, index, size);
				break;
			case aboutHeSeeOur:
				afterDayAnyPeopleGoWe(item, in, index, size);
				break;
			case aboutHowHaveOn:
				aboutWillOtherUseNowJustWould(item, in, index, size);
				break;
			case aboutIntoThisThisButCould:
				afterEvenHeNoWithUp(item, in, index, size);
				break;
			case aboutItSayFromThis:
				aboutWillOtherUseNowJustWould(item, in, index, size);
				break;
			case aboutItsBecauseOr:
				afterEvenItsOnlyInto(item, in, index, size);
				break;
			case aboutItsPeopleWellOther:
				afterEvenWellWithOneHim(item, in, index, size);
				break;
		}
	}
}

void afterGoThatAfterWeWhenOr(pair *item, const uint8_t *in, size_t index, size_t size) {
	if (size > 0) {
		size_t i = integer() % (size + 1);
		size_t j = 0;
		if (item->size > 0) {
			switch (item->second[item->size - 1]->value) {
				case aboutAtToNotWellOnWork:
					afterAboutBecauseMyIntoCouldAt(item->second[item->size - 1], in, index, i);
					break;
				case aboutBut:
					afterAnyWill(item->second[item->size - 1], in, index, i);
					break;
				case aboutEvenUseBackWhenHis:
					afterComeThen(item->second[item->size - 1], in, index, i);
					break;
				default:
					i = 0;
					break;
			}
			j += i;
		}

		while (j < size) {
			if (item->size >= item->sum) {
				item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
				item->sum *= 2;
			}

			i = integer() % (size - j + 1);
			if (item->size > 0 && item->second[item->size - 1]->value != aboutEvenUseBackWhenHis) {
				switch (byte() & 1) {
					case 0:
						item->second[item->size] = aboutTakeThereTheYouThenAs(in, index + j, i);
						break;
					default:
						item->second[item->size] = aboutTime(in, index + j, i);
						break;
				}
			} else {
				switch (integer() % 3) {
					case 0:
						item->second[item->size] = aboutTakeThereTheYouThenAs(in, index + j, i);
						break;
					case 1:
						item->second[item->size] = aboutTime(in, index + j, i);
						break;
					default:
						item->second[item->size] = aboutUs(in, index + j, i);
						break;
				}
			}
			j += i;
			++item->size;
		}
	}
}

void afterHeWeWorkUsThat(const pair *item, FILE *stream) {
	const size_t size = item->size;
	if (size < 6) {
		uint8_t first = item->first[0];
		switch (size) {
			case 0:
				if (byte() & 1) {
					fputs("'!'", stream);
				} else {
					fputs("'\"'", stream);
				}
				break;
			case 1:
				if (byte() & 1) {
					fprintf(stream, "'%c'", '#' + (first >> 7));
				} else {
					fprintf(stream, "'%c'", '%' + (first >> 7));
				}
				break;
			case 2:
				if (byte() & 1) {
					first >>= 6;
					if (first) {
						fprintf(stream, "'%c'", '\'' + first);
					} else {
						fputs("'\\''", stream);
					}
				} else {
					fprintf(stream, "'%c'", '+' + (first >> 6));
				}
				break;
			case 3:
				if (byte() & 1) {
					fprintf(stream, "'%c'", '/' + (first >> 5));
				} else {
					fprintf(stream, "'%c'", '7' + (first >> 5));
				}
				break;
			case 4:
				if (byte() & 1) {
					fprintf(stream, "'%c'", '?' + (first >> 4));
				} else {
					first >>= 4;
					if (first == 13) {
						fputs("'\\\\'", stream);
					} else {
						fprintf(stream, "'%c'", 'O' + first);
					}
				}
				break;
			default:
				fprintf(stream, "'%c'", '_' + (first >> 3));
				break;
		}
	} else {
		fputs("' '", stream);
	}
}

static const char *afterHerMyTheyLikeToMy[] = {"be", "other", "than", "because", "all", "at", "not", "good", "how", "over", "up", "that", "or", "use", "work"};
static const char *afterHimWhatOver[] = {"as", "to", "just", "his", "with", "from", "but", "one", "like", "there", "she", "her", "the", "know", "want"};
static const char *afterHisInto[] = {"year", "us", "look", "these", "no", "they", "way", "time", "give", "people", "some", "it", "well", "have", "when"};
static const char *afterItsOurWouldThese[] = {"now", "so", "our", "he", "them", "my", "would", "only", "say", "two", "you", "come", "its", "of", "go"};

static const char **afterItsWantComeWithOf[] = {afterHerMyTheyLikeToMy, afterHimWhatOver, afterHisInto, afterItsOurWouldThese};

void afterJustEvenWeToAfterSo(const pair *item, FILE *stream) {
	const size_t size = item->size;
	if (size > 0) {
		uint8_t out = 0;
		size_t i = 0;
		size_t j = integer() % (size + 1);
		if (j > 3) {
			j = 3;
		}

		if (j) {
			splice(&out, item->first, 0, j);
			out >>= (8 - j);

			switch (j) {
				case 1:
					fputs(afterItsWantComeWithOf[byte() & 3][1 + out], stream);
					break;
				case 2:
					fputs(afterItsWantComeWithOf[byte() & 3][3 + out], stream);
					break;
				default:
					fputs(afterItsWantComeWithOf[byte() & 3][7 + out], stream);
					break;
			}
			i += j;
		} else {
			fputs(afterItsWantComeWithOf[byte() & 3][0], stream);
		}

		while (i < size) {
			j = integer() % (size - i + 1);
			if (j > 3) {
				j = 3;
			}

			if (j) {
				splice(&out, item->first, i, j);
				out >>= (8 - j);

				uint8_t k = 0;
				switch (j) {
					case 1:
						k = 1 + out;
						break;
					case 2:
						k = 3 + out;
						break;
					default:
						k = 7 + out;
						break;
				}

				const char *l = afterItsWantComeWithOf[byte() & 3][k];
				fputc(*l - 32, stream);
				fputs(l + 1, stream);

				i += j;
			} else {
				const char *j = afterItsWantComeWithOf[byte() & 3][0];
				fputc(*j - 32, stream);
				fputs(j + 1, stream);
			}
		}
	} else {
		fputs(afterItsWantComeWithOf[byte() & 3][0], stream);
		size_t i = byte() & 3;
		while (i > 0) {
			const char *j = afterItsWantComeWithOf[byte() & 3][0];
			fputc(*j - 32, stream);
			fputs(j + 1, stream);
			--i;
		}
	}
}

void afterJustSheUpAlsoJust(const pair *item, FILE *stream) {
	const size_t size = item->size;
	if (size > 0) {
		uint8_t out = 0;
		size_t i = 0;
		while (i < size) {
			size_t j = integer() % (size - i + 1);
			if (j > 2) {
				j = 2;
			}

			if (j) {
				splice(&out, item->first, i, j);
				out >>= (8 - j);

				switch (j) {
					case 1:
						if (byte() & 1) {
							fputc('1' + out, stream);
						} else {
							fputc('8' + out, stream);
						}
						break;
					default:
						fputc('3' + out, stream);
						break;
				}
				i += j;
			} else if (byte() & 1) {
				fputc('0', stream);
			} else {
				fputc('7', stream);
			}
		}
	} else if (byte() & 1) {
		fputc('0', stream);
	} else {
		fputc('7', stream);
	}
}

static const char *afterKnowKnow[] = {"+", "-", "*", "/", "%", "<", "<=", "!=", "==", ">", ">=", "&", "&&", "|", "||"};

void afterKnowOnlyWe(const pair *item, FILE *stream) {
	const size_t size = item->size;
	if (size < 4) {
		const uint8_t first = item->first[0];
		switch (size) {
			case 0:
				fputs(afterKnowKnow[0], stream);
				break;
			case 1:
				fputs(afterKnowKnow[1 + (first >> 7)], stream);
				break;
			case 2:
				fputs(afterKnowKnow[3 + (first >> 6)], stream);
				break;
			default:
				fputs(afterKnowKnow[7 + (first >> 5)], stream);
				break;
		}
	} else {
		fputs("++", stream);
	}
}

static const char *afterLikeSo[] = {"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "&&=", "||=", "=~", ".="};

void afterMeOnWithAndTheSay(const pair *item, FILE *stream) {
	const size_t size = item->size;
	if (size < 4) {
		const uint8_t first = item->first[0];
		switch (size) {
			case 0:
				fputs(afterLikeSo[0], stream);
				break;
			case 1:
				fputs(afterLikeSo[1 + (first >> 7)], stream);
				break;
			case 2:
				fputs(afterLikeSo[3 + (first >> 6)], stream);
				break;
			default:
				fputs(afterLikeSo[7 + (first >> 5)], stream);
				break;
		}
	} else {
		fputs("--", stream);
	}
}

static const char *afterMost = "/0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

void afterMy(const pair *item, FILE *stream) {
	const size_t size = item->size;
	if (size > 0) {
		uint8_t out = 0;
		size_t i = 0;
		while (i < size) {
			size_t j = integer() % (size - i + 1);
			if (j > 5) {
				j = 5;
			}

			if (j) {
				splice(&out, item->first, i, j);
				out >>= (8 - j);

				switch (j) {
					case 1:
						fputc(afterMost[1 + out], stream);
						break;
					case 2:
						fputc(afterMost[3 + out], stream);
						break;
					case 3:
						fputc(afterMost[7 + out], stream);
						break;
					case 4:
						fputc(afterMost[15 + out], stream);
						break;
					default:
						fputc(afterMost[31 + out], stream);
						break;
				}
				i += j;
			} else {
				fputc(*afterMost, stream);
			}
		}
	} else {
		fputc(*afterMost, stream);
	}
}

static const char *afterMyThanWhichOnCan[] = {"bool", "char", "double", "float", "int", "long", "long double", "long int", "long long", "short", "size_t", "unsigned char", "unsigned int", "unsigned long", "unsigned short"};

void afterNo(const pair *item, FILE *stream) {
	const size_t size = item->size;
	if (size < 4) {
		const uint8_t first = item->first[0];
		switch (size) {
			case 0:
				fputs(afterMyThanWhichOnCan[0], stream);
				break;
			case 1:
				fputs(afterMyThanWhichOnCan[1 + (first >> 7)], stream);
				break;
			case 2:
				fputs(afterMyThanWhichOnCan[3 + (first >> 6)], stream);
				break;
			default:
				fputs(afterMyThanWhichOnCan[7 + (first >> 5)], stream);
				break;
		}
	} else {
		fputs("void", stream);
	}
}

void print(const pair *item, size_t index, FILE *stream) {
	switch (item->value) {
		case about:
			print(item->second[0], index, stream);
			fputc(' ', stream);
			print(item->second[1], index, stream);
			fputc(' ', stream);
			print(item->second[2], index, stream);
			break;
		case aboutAnDay:
			if (item->size > 0) {
				const size_t size = item->size;
				pair **second = item->second;

				const uint8_t value = second[0]->value;
				switch (value) {
					case about:
					case aboutItsBecauseOr:
						for (size_t i = 0; i <= index; ++i) {
							fputc('\t', stream);
						}
						break;
					default:
						break;
				}
				print(second[0], ++index, stream);
				if (value == about) {
					fputc(';', stream);
				}

				for (size_t i = 1; i < size; ++i) {
					switch (second[i - 1]->value) {
						case about:
						case aboutItsBecauseOr:
							fputc('\n', stream);
							break;
						default:
							fputc('\n', stream);
							fputc('\n', stream);
							break;
					}

					const uint8_t value = second[i]->value;
					switch (value) {
						case about:
						case aboutItsBecauseOr:
							for (size_t j = 0; j < index; ++j) {
								fputc('\t', stream);
							}
							break;
						default:
							break;
					}
					print(second[i], index, stream);
					if (value == about) {
						fputc(';', stream);
					}
				}
			}
			break;
		case aboutAnYouHimTimeMake:
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputs("case ", stream);
			print(item->second[0], index, stream);
			fputc(':', stream);
			fputc('\n', stream);
			print(item->second[1], index, stream);
			break;
		case aboutAnyTheirThere:
			if (item->size > 0) {
				const size_t size = item->size;
				pair **second = item->second;
				print(second[0], ++index, stream);
				fputc('\n', stream);
				for (size_t i = 0; i <= index; ++i) {
					fputc('\t', stream);
				}
				fputs("break;", stream);
				for (size_t i = 1; i < size; ++i) {
					fputc('\n', stream);
					print(second[i], index, stream);
					fputc('\n', stream);
					for (size_t j = 0; j <= index; ++j) {
						fputc('\t', stream);
					}
					fputs("break;", stream);
				}
			}
			break;
		case aboutAsWhatTheOnlyHimWill:
			afterHeWeWorkUsThat(item, stream);
			break;
		case aboutAtLikeToThem:
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputs("default:\n", stream);
			print(item->second[0], index, stream);
			break;
		case aboutAtToNotWellOnWork:
			fputs("#define ", stream);
			print(item->second[0], index, stream);
			fputc(' ', stream);
			print(item->second[1], index, stream);
			break;
		case aboutBackOnHim:
			fputs(" else {\n", stream);
			print(item->second[0], index, stream);
			fputc('\n', stream);
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputc('}', stream);
			break;
		case aboutBackSeeWould:
			fputs(" else if (", stream);
			print(item->second[0], index, stream);
			fputs(") {\n", stream);
			print(item->second[1], index, stream);
			fputc('\n', stream);
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputc('}', stream);
			break;
		case aboutBe:
			if (item->size > 0) {
				const size_t size = item->size;
				pair **second = item->second;
				print(second[0], index, stream);
				for (size_t i = 1; i < size; ++i) {
					fputc(' ', stream);
					print(second[i], index, stream);
				}
			} else {
				switch (integer() % 3) {
					case 0:
						if (byte() & 1) {
							fputs("'!'", stream);
						} else {
							fputs("'\"'", stream);
						}
						break;
					case 1:
						fputs(afterItsWantComeWithOf[byte() & 3][0], stream);
						size_t i = byte() & 3;
						while (i > 0) {
							const char *j = afterItsWantComeWithOf[byte() & 3][0];
							fputc(*j - 32, stream);
							fputs(j + 1, stream);
							--i;
						}
						break;
					default:
						if (byte() & 1) {
							fputc('0', stream);
						} else {
							fputc('7', stream);
						}
						break;
				}

				size_t i = byte() & 3;
				while (i > 0) {
					fputc(' ', stream);
					fputs(afterKnowKnow[0], stream);
					fputc(' ', stream);

					switch (integer() % 3) {
						case 0:
							if (byte() & 1) {
								fputs("'!'", stream);
							} else {
								fputs("'\"'", stream);
							}
							break;
						case 1:
							fputs(afterItsWantComeWithOf[byte() & 3][0], stream);
							size_t j = byte() & 3;
							while (j > 0) {
								const char *k = afterItsWantComeWithOf[byte() & 3][0];
								fputc(*k - 32, stream);
								fputs(k + 1, stream);
								--j;
							}
							break;
						default:
							if (byte() & 1) {
								fputc('0', stream);
							} else {
								fputc('7', stream);
							}
							break;
					}
					--i;
				}
			}
			break;
		case aboutBeTheirNotUpAs:
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputs("for (", stream);
			print(item->second[0], index, stream);
			if (item->second[1]->size > 0) {
				fputc(' ', stream);
				print(item->second[1], index, stream);
			}
			fputs("; ", stream);
			print(item->second[2], index, stream);
			fputs(") {\n", stream);
			print(item->second[3], index, stream);
			fputc('\n', stream);
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputc('}', stream);
			break;
		case aboutBut:
			print(item->second[0], index, stream);
			fputc(' ', stream);
			print(item->second[1], index, stream);
			fputc('(', stream);
			print(item->second[2], index, stream);
			fputs(") {\n", stream);
			print(item->second[3], index, stream);
			fputc('\n', stream);
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputc('}', stream);
			break;
		case aboutBy:
			afterJustEvenWeToAfterSo(item, stream);
			break;
		case aboutCanThem:
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputs("if (", stream);
			print(item->second[0], index, stream);
			fputs(") {\n", stream);
			print(item->second[1], index, stream);
			fputc('\n', stream);
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputc('}', stream);
			if (item->size > 2) {
				const size_t size = item->size;
				pair **second = item->second;
				for (size_t i = 2; i < size; ++i) {
					print(second[i], index, stream);
				}
			}
			break;
		case aboutEvenUseBackWhenHis:
			fputs("#include <", stream);
			print(item->second[0], index, stream);
			fputc('.', stream);
			fputc('h', stream);
			fputc('>', stream);
			break;
		case aboutGiveUsGoodKnowShe:
			afterJustSheUpAlsoJust(item, stream);
			break;
		case aboutGoOn:
			afterKnowOnlyWe(item, stream);
			break;
		case aboutGoPeopleYearHimAllWork:
			afterMeOnWithAndTheSay(item, stream);
			break;
		case aboutHaveOurNoFrom:
			print(item->second[0], index, stream);
			fputc(' ', stream);
			print(item->second[1], index, stream);
			break;
		case aboutHeSeeOur:
			if (item->size > 0) {
				const size_t size = item->size;
				pair **second = item->second;
				print(second[0], index, stream);
				for (size_t i = 1; i < size; ++i) {
					fputc(',', stream);
					fputc(' ', stream);
					print(second[i], index, stream);
				}
			}
			break;
		case aboutHowHaveOn:
			afterMy(item, stream);
			break;
		case aboutIntoThisThisButCould:
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputs("switch (", stream);
			print(item->second[0], index, stream);
			fputs(") {\n", stream);
			print(item->second[1], index, stream);
			fputc('\n', stream);
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputc('}', stream);
			break;
		case aboutItSayFromThis:
			afterNo(item, stream);
			break;
		case aboutItsBecauseOr:
			print(item->second[0], index, stream);
			fputc(' ', stream);
			print(item->second[1], index, stream);
			fputc(' ', stream);
			fputc('=', stream);
			fputc(' ', stream);
			print(item->second[2], index, stream);
			fputc(';', stream);
			break;
		case aboutItsPeopleWellOther:
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputs("while (", stream);
			print(item->second[0], index, stream);
			fputs(") {\n", stream);
			print(item->second[1], index, stream);
			fputc('\n', stream);
			for (size_t i = 0; i < index; ++i) {
				fputc('\t', stream);
			}
			fputc('}', stream);
			break;
	}
}

void afterNoAlsoAll(const pair *item, uint8_t *in, uint8_t *index, FILE *stream) {
	const size_t size = item->size;
	if (size > 0) {
		const size_t sum = (size - 1) >> 3;
		const uint8_t value = *index & 7;
		if (value) {
			const uint8_t j = 8 - value;
			for (size_t i = 0; i < sum; ++i) {
				*in |= item->first[i] >> value;
				fputc(*in, stream);
				*in = item->first[i] << j;
			}
		} else {
			for (size_t i = 0; i < sum; ++i) {
				fputc(item->first[i], stream);
			}
		}

		uint8_t i = item->first[sum];
		const uint8_t j = size & 7;
		if (j) {
			*in |= i >> value;
			const uint8_t k = j + value;
			if (k > 7) {
				fputc(*in, stream);
				const uint8_t j = k & 7;
				*index = j;
				if (j) {
					*in = i << (8 - value);
				} else {
					*in = 0;
				}
			} else {
				*index += j;
			}
		} else if (value) {
			*in |= i >> value;
			fputc(*in, stream);
			*in = i << (8 - value);
		} else {
			fputc(i, stream);
			*in = 0;
		}
	}
}

void afterNotFrom(const pair *item, uint8_t *in, uint8_t *index, FILE *stream) {
	switch (item->is) {
		case PAIR_FIRST:
			afterNoAlsoAll(item, in, index, stream);
			break;
		case PAIR_SECOND:
			const size_t size = item->size;
			pair **second = item->second;
			for (size_t i = 0; i < size; ++i) {
				afterNotFrom(second[i], in, index, stream);
			}
			break;
	}
}

struct first {
	uint8_t *in;
	size_t size;
	size_t sum;
};

bool afterPeopleWhoThereOtherIt(struct first *first, size_t index, FILE *stream) {
	if (index < first->size) {
		return true;
	} else {
		while (index >= first->sum) {
			first->in = (uint8_t *) reallocarray(first->in, 2 * first->sum, sizeof(uint8_t));
			first->sum *= 2;
		}

		size_t i = first->sum - first->size;
		size_t j = fread(first->in + first->size, sizeof(uint8_t), i, stream);
		if (j > 0) {
			first->size += j;
			i -= j;
		}

		while (i > 0 && j > 0) {
			j = fread(first->in + first->size, sizeof(uint8_t), i, stream);
			if (j > 0) {
				first->size += j;
				i -= j;
			} else {
				break;
			}
		}

		if (index < first->size) {
			return true;
		}
	}

	return false;
}

size_t afterSaySomeThinkTheyOnlyEven(const uint8_t *in, struct first *first, size_t index, FILE *stream) {
	size_t i = 0;
	while (*in) {
		if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == *in) {
			++i;
			++in;
			++index;
			continue;
		}

		break;
	}

	return i;
}

bool afterSeeUsMyWhichHerLook(pair **out, uint8_t in) {
	if (in < '!' || in > '~') {
		return false;
	} else {
		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_FIRST;
		item->first = (uint8_t *) calloc(1, sizeof(uint8_t));

		if (in > '^') {
			item->size = 5;
			item->first[0] = (in - '_') << 3;
		} else if (in > '>') {
			item->size = 4;
			if (in > 'N') {
				item->first[0] = (in - 'O') << 4;
			} else {
				item->first[0] = (in - '?') << 4;
			}
		} else if (in > '.') {
			item->size = 3;
			if (in > '6') {
				item->first[0] = (in - '7') << 5;
			} else {
				item->first[0] = (in - '/') << 5;
			}
		} else if (in > '&') {
			item->size = 2;
			if (in > '*') {
				item->first[0] = (in - '+') << 6;
			} else {
				item->first[0] = (in - '\'') << 6;
			}
		} else if (in > '"') {
			item->size = 1;
			if (in > '$') {
				item->first[0] = (in - '%') << 7;
			} else {
				item->first[0] = (in - '#') << 7;
			}
		}

		item->value = aboutAsWhatTheOnlyHimWill;
		*out = item;
		return true;
	}
}

size_t afterSheButMeOverTake(pair **out, struct first *first, size_t index, FILE *stream) {
	if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '\'') {
		++index;

		if (afterPeopleWhoThereOtherIt(first, index, stream)) {
			uint8_t in = first->in[index];
			switch (in) {
				case '\'':
					return 0;
				case '\\':
					++index;
					if (afterPeopleWhoThereOtherIt(first, index, stream)) {
						switch (first->in[index]) {
							case '\'':
								in = '\'';
								break;
							case '\\':
								break;
							default:
								return 0;
						}

						++index;
						if(afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '\'' && afterSeeUsMyWhichHerLook(out, in)) {
							return 4;
						}
					}
					break;
				default:
					++index;
					if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '\'' && afterSeeUsMyWhichHerLook(out, in)) {
						return 3;
					}
					break;
			}
		}
	}

	return 0;
}

static const char *afterSheOn[] = {"because", "people", "would", "these", "there", "other", "them", "only", "come", "year", "look", "they", "time", "give", "some", "well", "have", "when", "just", "with", "from", "like", "know", "want", "than", "good", "over", "that", "work", "now", "our", "say", "two", "you", "its", "way", "his", "but", "one", "she", "her", "the", "all", "not", "how", "use", "so", "he", "my", "of", "go", "us", "no", "it", "as", "to", "be", "at", "up", "or"};
static const uint8_t afterTakeSay[] = {7, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
static const uint8_t afterThatOurThem[] = {0, 64, 192, 0, 64, 0, 64, 0, 128, 0, 128, 128, 0, 32, 96, 160, 192, 224, 128, 64, 128, 32, 192, 224, 128, 0, 64, 128, 224, 0, 128, 32, 64, 96, 160, 192, 0, 192, 0, 96, 128, 160, 64, 192, 32, 192, 0, 0, 128, 192, 224, 0, 64, 128, 0, 0, 0, 128, 96, 160};
static const uint8_t afterTheirOfHisEvenWhen[] = {2, 3, 2, 2, 3, 1, 2, 3, 3, 0, 1, 2, 3, 3, 3, 3, 3, 3, 1, 2, 2, 3, 3, 3, 1, 3, 3, 3, 3, 0, 1, 3, 3, 3, 3, 2, 2, 2, 3, 3, 3, 3, 2, 2, 3, 3, 1, 2, 2, 3, 3, 1, 2, 3, 0, 1, 0, 2, 3, 3};

size_t afterThemLikeAn(pair **out, struct first *first, size_t index, FILE *stream) {
	pair *item = (pair *) NULL;

	size_t i = 0;
	const size_t size = sizeof(afterSheOn) / sizeof(afterSheOn[0]);
	for (size_t j = 0; j < size; ++j) {
		const size_t k = afterTakeSay[j];
		if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) afterSheOn[j], first, index, stream) == k) {
			item = (pair *) calloc(1, sizeof(pair));
			item->is = PAIR_FIRST;
			item->first = (uint8_t *) calloc(1, sizeof(uint8_t));
			item->first[0] = afterThatOurThem[j];
			item->size = afterTheirOfHisEvenWhen[j];
			item->value = aboutBy;
			*out = item;
			index += k;
			i = k;
			break;
		}
	}

	if (item) {
		bool in = true;
		while (in) {
			in = false;

			for (size_t j = 0; j < size; ++j) {
				const uint8_t *out = (const uint8_t *) afterSheOn[j];
				if (afterPeopleWhoThereOtherIt(first, index, stream)) {
					if (first->in[index] != *out - 32) {
						continue;
					}
				} else {
					break;
				}

				size_t k = afterTakeSay[j] - 1;
				if (afterSaySomeThinkTheyOnlyEven(out + 1, first, index + 1, stream) == k) {
					++k;

					const uint8_t l = afterTheirOfHisEvenWhen[j];
					if (l) {
						const uint8_t i = afterThatOurThem[j];
						aboutWhenMake(item, &i, l);
					}

					index += k;
					in = true;
					i += k;
					break;
				}
			}
		}

		return i;
	}

	return 0;
}

void afterThemOther(pair *item, uint8_t in) {
	if ('0' < in && in < '3') {
		const uint8_t i = (in - '1') << 7;
		aboutWhenMake(item, &i, 1);
	} else if ('2' < in && in < '7') {
		const uint8_t i = (in - '3') << 6;
		aboutWhenMake(item, &i, 2);
	} else if (in > '7') {
		const uint8_t i = (in - '8') << 7;
		aboutWhenMake(item, &i, 1);
	}
}

size_t afterThereHis(pair **out, struct first *first, size_t index, FILE *stream) {
	if (afterPeopleWhoThereOtherIt(first, index, stream)) {
		const uint8_t in = first->in[index];

		if ('0' <= in && in <= '9') {
			pair *item = (pair *) calloc(1, sizeof(pair));
			item->is = PAIR_FIRST;
			item->first = (uint8_t *) calloc(1, sizeof(uint8_t));
			afterThemOther(item, in);

			size_t i = 1;
			++index;

			while (afterPeopleWhoThereOtherIt(first, index, stream)) {
				const uint8_t in = first->in[index];
				if ('0' <= in && in <= '9') {
					afterThemOther(item, in);
					++i;
					++index;
				} else {
					break;
				}
			}

			item->value = aboutGiveUsGoodKnowShe;
			*out = item;
			return i;
		}
	}

	return 0;
}

static const char *afterTheseWantAsPeopleTake[] = {"<=", "!=", "==", ">=", "&&", "||", "+", "-", "*", "/", "%", "<", ">", "&", "|"};
static const uint8_t afterToMyNow[] = {2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1};
static const uint8_t afterUsTheirAfterWithThan[] = {192, 0, 32, 96, 160, 224, 0, 0, 128, 0, 64, 128, 64, 128, 192};
static const uint8_t afterUse[] = {2, 3, 3, 3, 3, 3, 0, 1, 1, 2, 2, 2, 3, 3, 3};

size_t afterUseUpBy(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t size = sizeof(afterTheseWantAsPeopleTake) / sizeof(afterTheseWantAsPeopleTake[0]);
	for (size_t i = 0; i < size; ++i) {
		const size_t j = afterToMyNow[i];
		if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) afterTheseWantAsPeopleTake[i], first, index, stream) == j) {
			pair *item = (pair *) calloc(1, sizeof(pair));
			item->is = PAIR_FIRST;
			item->first = (uint8_t *) calloc(1, sizeof(uint8_t));
			item->first[0] = afterUsTheirAfterWithThan[i];
			item->size = afterUse[i];
			item->value = aboutGoOn;
			*out = item;
			return j;
		}
	}

	return 0;
}

static const char *afterWant[] = {"<<=", ">>=", "&&=", "||=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "=~", ".=", "="};
static const uint8_t afterWe[] = {3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};
static const uint8_t afterWeBeWhenThen[] = {64, 96, 128, 160, 0, 128, 0, 64, 128, 192, 0, 32, 192, 224, 0};
static const uint8_t afterWeOf[] = {3, 3, 3, 3, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0};

size_t afterWellTimeWhoPeopleThat(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t size = sizeof(afterWant) / sizeof(afterWant[0]);
	for (size_t i = 0; i < size; ++i) {
		const size_t j = afterWe[i];
		if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) afterWant[i], first, index, stream) == j) {
			pair *item = (pair *) calloc(1, sizeof(pair));
			item->is = PAIR_FIRST;
			item->first = (uint8_t *) calloc(1, sizeof(uint8_t));
			item->first[0] = afterWeBeWhenThen[i];
			item->size = afterWeOf[i];
			item->value = aboutGoPeopleYearHimAllWork;
			*out = item;
			return j;
		}
	}

	return 0;
}

bool afterWellWayWell(uint8_t in) {
	if ('/' <= in && in <= '9') {
		return true;
	} else if ('A' <= in && in <= 'Z') {
		return true;
	} else if ('a' <= in && in <= 'z') {
		return true;
	} else {
		return false;
	}
}

void afterWhatMeAlsoAboutThatAt(pair *item, uint8_t in) {
	const uint8_t *out = (const uint8_t *) afterMost;

	size_t i = 0;
	while (*out) {
		if (*out == in) {
			break;
		}

		++i;
		++out;
	}

	if (i > 30) {
		const uint8_t in = (i - 31) << 3;
		aboutWhenMake(item, &in, 5);
	} else if (i > 14) {
		const uint8_t in = (i - 15) << 4;
		aboutWhenMake(item, &in, 4);
	} else if (i > 6) {
		const uint8_t in = (i - 7) << 5;
		aboutWhenMake(item, &in, 3);
	} else if (i > 2) {
		const uint8_t in = (i - 3) << 6;
		aboutWhenMake(item, &in, 2);
	} else if (i > 0) {
		const uint8_t in = (i - 1) << 7;
		aboutWhenMake(item, &in, 1);
	}
}

size_t afterWho(pair **out, struct first *first, size_t index, FILE *stream) {
	if (afterPeopleWhoThereOtherIt(first, index, stream)) {
		const uint8_t in = first->in[index];

		if (afterWellWayWell(in)) {
			pair *item = (pair *) calloc(1, sizeof(pair));
			item->is = PAIR_FIRST;
			item->first = (uint8_t *) calloc(1, sizeof(uint8_t));
			afterWhatMeAlsoAboutThatAt(item, in);

			size_t i = 1;
			++index;

			while (afterPeopleWhoThereOtherIt(first, index, stream)) {
				const uint8_t in = first->in[index];
				if (afterWellWayWell(in)) {
					afterWhatMeAlsoAboutThatAt(item, in);
					++i;
					++index;
				} else {
					break;
				}
			}

			item->value = aboutHowHaveOn;
			*out = item;
			return i;
		}
	}

	return 0;
}

static const char *afterWhoTheyAny[] = {"unsigned short", "unsigned char", "unsigned long", "unsigned int", "long double", "long long", "long int", "double", "size_t", "float", "short", "bool", "char", "long", "int"};
static const uint8_t afterWorkThat[] = {14, 13, 13, 12, 11, 9, 8, 6, 6, 5, 5, 4, 4, 4, 3};
static const uint8_t afterWould[] = {224, 128, 192, 160, 192, 32, 0, 128, 96, 0, 64, 0, 0, 128, 64};
static const uint8_t afterYearHerOverEven[] = {3, 3, 3, 3, 2, 3, 3, 1, 3, 2, 3, 0, 1, 2, 2};

size_t afterYearUp(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t size = sizeof(afterWhoTheyAny) / sizeof(afterWhoTheyAny[0]);
	for (size_t i = 0; i < size; ++i) {
		const size_t j = afterWorkThat[i];
		if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) afterWhoTheyAny[i], first, index, stream) == j) {
			pair *item = (pair *) calloc(1, sizeof(pair));
			item->is = PAIR_FIRST;
			item->first = (uint8_t *) calloc(1, sizeof(uint8_t));
			item->first[0] = afterWould[i];
			item->size = afterYearHerOverEven[i];
			item->value = aboutItSayFromThis;
			*out = item;
			return j;
		}
	}

	return 0;
}

size_t afterYourLookSoMeMy(struct first *first, size_t index, FILE *stream) {
	size_t i = 0;

	while (afterPeopleWhoThereOtherIt(first, index, stream)) {
		switch (first->in[index]) {
			case '\t':
			case '\n':
			case '\f':
			case '\r':
			case ' ':
				++i;
				++index;
				break;
			default:
				return i;
		}
	}

	return i;
}

void delete(pair *item) {
	switch (item->is) {
		case PAIR_FIRST:
			free(item->first);
			break;
		case PAIR_SECOND:
			const size_t size = item->size;
			pair **second = item->second;
			for (size_t i = 0; i < size; ++i) {
				delete(second[i]);
			}
			free(second);
			break;
	}
	free(item);
}

size_t afterYourSee(pair **out, struct first *first, size_t index, FILE *stream);
size_t all(pair **out, struct first *first, size_t index, FILE *stream);
size_t allAboutMeNow(pair **out, struct first *first, size_t index, FILE *stream);
size_t allAlsoWhoThatWillThanOnly(pair **out, struct first *first, size_t index, FILE *stream);
size_t allAnDayWhich(pair **out, struct first *first, size_t index, FILE *stream);
size_t allAnWantWeItAsLike(pair **out, struct first *first, size_t index, FILE *stream);

size_t allAndTheseNow(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;

	pair *item = aboutSayAs(aboutAnDay);
	*out = item;

	pair *second = (pair *) NULL;
	size_t j = afterYourSee(&second, first, index, stream);
	if (j > 0) {
		j += afterYourLookSoMeMy(first, index + j, stream);
		if (afterPeopleWhoThereOtherIt(first, index + j, stream) && first->in[index + j] == ';') {
			++j;
		} else {
			j = 0;
			delete(second);
			second = (pair *) NULL;
		}
	}

	if (j < 1) {
		j = all(&second, first, index, stream);
	}

	if (j < 1) {
		j = allAboutMeNow(&second, first, index, stream);
	}

	if (j < 1) {
		j = allAlsoWhoThatWillThanOnly(&second, first, index, stream);
	}

	if (j < 1) {
		j = allAnDayWhich(&second, first, index, stream);
	}

	if (j < 1) {
		j = allAnWantWeItAsLike(&second, first, index, stream);
	}

	if (j > 0) {
		index += j;
		size_t k = index;

		if (item->size >= item->sum) {
			item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
			item->sum *= 2;
		}

		item->second[item->size] = second;
		++item->size;

		while (j > 0) {
			index += afterYourLookSoMeMy(first, index, stream);

			j = afterYourSee(&second, first, index, stream);
			if (j > 0) {
				j += afterYourLookSoMeMy(first, index + j, stream);
				if (afterPeopleWhoThereOtherIt(first, index + j, stream) && first->in[index + j] == ';') {
					++j;
				} else {
					j = 0;
					delete(second);
					second = (pair *) NULL;
				}
			}

			if (j < 1) {
				j = all(&second, first, index, stream);
			}

			if (j < 1) {
				j = allAboutMeNow(&second, first, index, stream);
			}

			if (j < 1) {
				j = allAlsoWhoThatWillThanOnly(&second, first, index, stream);
			}

			if (j < 1) {
				j = allAnDayWhich(&second, first, index, stream);
			}

			if (j < 1) {
				j = allAnWantWeItAsLike(&second, first, index, stream);
			}

			if (j > 0) {
				index += j;
				k = index;

				if (item->size >= item->sum) {
					item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
					item->sum *= 2;
				}

				item->second[item->size] = second;
				++item->size;
			}
		}
		index = k;
	}

	return index - i;
}

size_t allAnyUpNowGoAn(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;

	pair *second = (pair *) NULL;
	size_t j = afterSheButMeOverTake(&second, first, index, stream);
	if (j < 1) {
		j = afterThemLikeAn(&second, first, index, stream);
	}

	if (j < 1) {
		j = afterThereHis(&second, first, index, stream);
	}

	if (j > 0) {
		index += j;

		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(1, sizeof(pair *));
		item->second[0] = second;
		item->size = 1;
		item->sum = 1;
		item->value = aboutBe;
		*out = item;

		size_t k = index;
		while (j > 0) {
			index += afterYourLookSoMeMy(first, index, stream);

			second = (pair *) NULL;
			j = afterUseUpBy(&second, first, index, stream);

			if (j > 0) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				pair *value = (pair *) NULL;
				j = afterSheButMeOverTake(&value, first, index, stream);
				if (j < 1) {
					j = afterThemLikeAn(&value, first, index, stream);
				}

				if (j < 1) {
					j = afterThereHis(&value, first, index, stream);
				}

				if (j > 0) {
					index += j;
					k = index;

					if (item->size >= item->sum) {
						item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
						item->sum *= 2;
					}
					item->second[item->size] = second;

					if (++item->size >= item->sum) {
						item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
						item->sum *= 2;
					}
					item->second[item->size] = value;

					++item->size;
					continue;
				}

				delete(second);
			}
		}
		index = k;

		return index - i;
	}

	return 0;
}

size_t afterYourSee(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;

	pair *second = (pair *) NULL;
	size_t j = afterThemLikeAn(&second, first, index, stream);
	if (j > 0) {
		index += j;
		index += afterYourLookSoMeMy(first, index, stream);

		pair *value = (pair *) NULL;
		j = afterWellTimeWhoPeopleThat(&value, first, index, stream);
		if (j > 0) {
			index += j;
			index += afterYourLookSoMeMy(first, index, stream);

			pair *sum = (pair *) NULL;
			j = allAnyUpNowGoAn(&sum, first, index, stream);
			if (j > 0) {
				index += j;

				pair *item = (pair *) calloc(1, sizeof(pair));
				item->is = PAIR_SECOND;
				item->second = (pair **) calloc(3, sizeof(pair *));
				item->second[0] = second;
				item->second[1] = value;
				item->second[2] = sum;
				item->size = 3;
				item->sum = 3;
				item->value = about;
				*out = item;

				return index - i;
			}

			delete(value);
		}

		delete(second);
	}

	return 0;
}

size_t allAnyWouldThis(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "case", first, index, stream) == 4) {
		index += 4;
		size_t j = afterYourLookSoMeMy(first, index, stream);

		if (j > 0) {
			index += j;

			pair *second = (pair *) NULL;
			j = allAnyUpNowGoAn(&second, first, index, stream);
			if (j > 0) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ':') {
					++index;
					index += afterYourLookSoMeMy(first, index, stream);

					pair *value = (pair *) NULL;
					j = allAndTheseNow(&value, first, index, stream);
					if (value) {
						index += j;
						index += afterYourLookSoMeMy(first, index, stream);
						if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "break;", first, index, stream) == 6) {
							index += 6;

							pair *item = (pair *) calloc(1, sizeof(pair));
							item->is = PAIR_SECOND;
							item->second = (pair **) calloc(2, sizeof(pair *));
							item->second[0] = second;
							item->second[1] = value;
							item->size = 2;
							item->sum = 2;
							item->value = aboutAnYouHimTimeMake;
							*out = item;

							return index - i;
						}

						delete(value);
					}
				}

				delete(second);
			}
		}
	}

	return 0;
}

size_t allAt(pair **out, struct first *first, size_t index, FILE *stream);

size_t allAtWillNow(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;

	pair *second = (pair *) NULL;
	size_t j = allAnyWouldThis(&second, first, index, stream);
	if (j < 1) {
		j = allAt(&second, first, index, stream);
	}

	if (j > 0) {
		index += j;
		index += afterYourLookSoMeMy(first, index, stream);

		pair *item = (pair *) calloc(1, sizeof(pair));
		item->is = PAIR_SECOND;
		item->second = (pair **) calloc(1, sizeof(pair *));
		item->second[0] = second;
		item->size = 1;
		item->sum = 1;
		item->value = aboutAnyTheirThere;

		while (j > 0) {
			second = (pair *) NULL;
			j = allAnyWouldThis(&second, first, index, stream);
			if (j < 1) {
				j = allAt(&second, first, index, stream);
			}

			if (j > 0) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				if (item->size >= item->sum) {
					item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
					item->sum *= 2;
				}

				item->second[item->size] = second;
				++item->size;
			}
		}

		*out = item;
		return index - i;
	} else {
		*out = aboutSayAs(aboutAnyTheirThere);
	}

	return 0;
}

size_t allAt(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "default", first, index, stream) == 7) {
		index += 7;
		index += afterYourLookSoMeMy(first, index, stream);

		if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ':') {
			++index;
			index += afterYourLookSoMeMy(first, index, stream);

			pair *second = (pair *) NULL;
			size_t j = allAndTheseNow(&second, first, index, stream);
			if (second) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);
				if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "break;", first, index, stream) == 6) {
					index += 6;

					pair *item = (pair *) calloc(1, sizeof(pair));
					item->is = PAIR_SECOND;
					item->second = (pair **) calloc(1, sizeof(pair *));
					item->second[0] = second;
					item->size = 1;
					item->sum = 1;
					item->value = aboutAtLikeToThem;
					*out = item;

					return index - i;
				}

				delete(second);
			}
		}
	}

	return 0;
}

size_t allBecause(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "#define", first, index, stream) == 7) {
		index += 7;
		size_t j = afterYourLookSoMeMy(first, index, stream);

		if (j > 0) {
			index += j;

			pair *second = (pair *) NULL;
			j = afterThemLikeAn(&second, first, index, stream);
			if (j > 0) {
				index += j;
				j = afterYourLookSoMeMy(first, index, stream);

				if (j > 0) {
					index += j;

					pair *value = (pair *) NULL;
					j = afterThereHis(&value, first, index, stream);
					if (j > 0) {
						index += j;

						pair *item = (pair *) calloc(1, sizeof(pair));
						item->is = PAIR_SECOND;
						item->second = (pair **) calloc(2, sizeof (pair *));
						item->second[0] = second;
						item->second[1] = value;
						item->size = 2;
						item->sum = 2;
						item->value = aboutAtToNotWellOnWork;
						*out = item;

						return index - i;
					}
				}

				delete(second);
			}
		}
	}

	return 0;
}

size_t allBy(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "else", first, index, stream) == 4) {
		index += 4;
		index += afterYourLookSoMeMy(first, index, stream);

		if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '{') {
			++index;
			index += afterYourLookSoMeMy(first, index, stream);

			pair *second = (pair *) NULL;
			size_t j = allAndTheseNow(&second, first, index, stream);
			if (second) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '}') {
					++index;

					pair *item = (pair *) calloc(1, sizeof(pair));
					item->is = PAIR_SECOND;
					item->second = (pair **) calloc(1, sizeof(pair *));
					item->second[0] = second;
					item->size = 1;
					item->sum = 1;
					item->value = aboutBackOnHim;
					*out = item;

					return index - i;
				}

				delete(second);
			}
		}
	}

	return 0;
}

size_t allCanAnd(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "else", first, index, stream) == 4) {
		index += 4;
		size_t j = afterYourLookSoMeMy(first, index, stream);

		if (j > 0) {
			index += j;

			if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "if", first, index, stream) == 2) {
				index += 2;
				index += afterYourLookSoMeMy(first, index, stream);

				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '(') {
					++index;
					index += afterYourLookSoMeMy(first, index, stream);

					pair *second = (pair *) NULL;
					j = allAnyUpNowGoAn(&second, first, index, stream);
					if (j > 0) {
						index += j;
						index += afterYourLookSoMeMy(first, index, stream);

						if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ')') {
							++index;
							index += afterYourLookSoMeMy(first, index, stream);

							if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '{') {
								++index;
								index += afterYourLookSoMeMy(first, index, stream);

								pair *value = (pair *) NULL;
								j = allAndTheseNow(&value, first, index, stream);
								if (value) {
									index += j;
									index += afterYourLookSoMeMy(first, index, stream);

									if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '}') {
										++index;

										pair *item = (pair *) calloc(1, sizeof(pair));
										item->is = PAIR_SECOND;
										item->second = (pair **) calloc(2, sizeof (pair *));
										item->second[0] = second;
										item->second[1] = value;
										item->size = 2;
										item->sum = 2;
										item->value = aboutBackSeeWould;
										*out = item;

										return index - i;
									}

									delete(value);
								}
							}
						}

						delete(second);
					}
				}
			}
		}
	}

	return 0;
}

size_t all(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "for", first, index, stream) == 3) {
		index += 3;
		index += afterYourLookSoMeMy(first, index, stream);

		if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '(') {
			++index;
			index += afterYourLookSoMeMy(first, index, stream);

			pair *second = (pair *) NULL;
			size_t j = allAnDayWhich(&second, first, index, stream);
			if (j > 0) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				j = 0;
				pair *value = (pair *) NULL;
				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ';') {
					++index;
					index += afterYourLookSoMeMy(first, index, stream);

					value = aboutSayAs(aboutBe);
					j = 1;
				}

				if (j < 1) {
					j = allAnyUpNowGoAn(&value, first, index, stream);
					if (j > 0) {
						index += j;
						index += afterYourLookSoMeMy(first, index, stream);

						if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ';') {
							++index;
							index += afterYourLookSoMeMy(first, index, stream);
						} else {
							delete(value);
							j = 0;
						}
					}
				}

				if (j > 0) {
					pair *sum = (pair *) NULL;
					j = afterYourSee(&sum, first, index, stream);
					if (j > 0) {
						index += j;
						index += afterYourLookSoMeMy(first, index, stream);

						if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ')') {
							++index;
							index += afterYourLookSoMeMy(first, index, stream);

							if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '{') {
								++index;
								index += afterYourLookSoMeMy(first, index, stream);

								pair *in = (pair *) NULL;
								j = allAndTheseNow(&in, first, index, stream);
								if (in) {
									index += j;
									index += afterYourLookSoMeMy(first, index, stream);
									if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '}') {
										++index;

										pair *item = (pair *) calloc(1, sizeof(pair));
										item->is = PAIR_SECOND;
										item->second = (pair **) calloc(4, sizeof (pair *));
										item->second[0] = second;
										item->second[1] = value;
										item->second[2] = sum;
										item->second[3] = in;
										item->size = 4;
										item->sum = 4;
										item->value = aboutBeTheirNotUpAs;
										*out = item;

										return index - i;
									}

									delete(in);
								}
							}
						}

						delete(sum);
					}

					delete(value);
				}

				delete(second);
			}
		}
	}

	return 0;
}

size_t allCanNotBut(pair **out, struct first *first, size_t index, FILE *stream);

size_t allCouldThemYourWhen(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;

	pair *second = (pair *) NULL;
	size_t j = afterYearUp(&second, first, index, stream);
	if (j > 0) {
		index += j;

		j = afterYourLookSoMeMy(first, index, stream);
		if (j > 0) {
			index += j;

			pair *value = (pair *) NULL;
			j = afterThemLikeAn(&value, first, index, stream);
			if (j > 0) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '(') {
					++index;
					index += afterYourLookSoMeMy(first, index, stream);

					pair *sum = (pair *) NULL;
					j = allCanNotBut(&sum, first, index, stream);
					if (sum) {
						index += j;
						index += afterYourLookSoMeMy(first, index, stream);

						if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ')') {
							++index;
							index += afterYourLookSoMeMy(first, index, stream);
							if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '{') {
								++index;
								index += afterYourLookSoMeMy(first, index, stream);

								pair *in = (pair *) NULL;
								j = allAndTheseNow(&in, first, index, stream);
								if (in) {
									index += j;
									index += afterYourLookSoMeMy(first, index, stream);
									if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '}') {
										++index;

										pair *item = (pair *) calloc(1, sizeof(pair));
										item->is = PAIR_SECOND;
										item->second = (pair **) calloc(4, sizeof (pair *));
										item->second[0] = second;
										item->second[1] = value;
										item->second[2] = sum;
										item->second[3] = in;
										item->size = 4;
										item->sum = 4;
										item->value = aboutBut;
										*out = item;

										return index - i;
									}

									delete(in);
								}
							}
						}

						delete(sum);
					}
				}

				delete(value);
			}
		}

		delete(second);
	}

	return 0;
}

size_t allAboutMeNow(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "if", first, index, stream) == 2) {
		index += 2;
		index += afterYourLookSoMeMy(first, index, stream);

		if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '(') {
			++index;
			index += afterYourLookSoMeMy(first, index, stream);

			pair *second = (pair *) NULL;
			size_t j = allAnyUpNowGoAn(&second, first, index, stream);
			if (j > 0) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ')') {
					++index;
					index += afterYourLookSoMeMy(first, index, stream);

					if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '{') {
						++index;
						index += afterYourLookSoMeMy(first, index, stream);

						pair *value = (pair *) NULL;
						j = allAndTheseNow(&value, first, index, stream);
						if (value) {
							index += j;
							index += afterYourLookSoMeMy(first, index, stream);

							if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '}') {
								++index;

								pair *item = (pair *) calloc(1, sizeof(pair));
								item->is = PAIR_SECOND;
								item->second = (pair **) calloc(2, sizeof (pair *));
								item->second[0] = second;
								item->second[1] = value;
								item->size = 2;
								item->sum = 2;
								item->value = aboutCanThem;
								*out = item;

								size_t k = index;
								index += afterYourLookSoMeMy(first, index, stream);

								second = (pair *) NULL;
								j = allCanAnd(&second, first, index, stream);
								while (j > 0) {
									index += j;

									k = index;
									index += afterYourLookSoMeMy(first, index, stream);

									if (item->size >= item->sum) {
										item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
										item->sum *= 2;
									}

									item->second[item->size] = second;
									++item->size;

									second = (pair *) NULL;
									j = allCanAnd(&second, first, index, stream);
								}

								index = k;
								index += afterYourLookSoMeMy(first, index, stream);
								second = (pair *) NULL;
								j = allBy(&second, first, index, stream);
								if (j > 0) {
									index += j;

									if (item->size >= item->sum) {
										item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
										item->sum *= 2;
									}

									item->second[item->size] = second;
									++item->size;
								} else {
									index = k;
								}

								return index - i;
							}

							delete(value);
						}
					}
				}

				delete(second);
			}
		}
	}

	return 0;
}

size_t allDay(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "#include", first, index, stream) == 8) {
		index += 8;
		index += afterYourLookSoMeMy(first, index, stream);

		if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '<') {
			++index;

			pair *second = (pair *) NULL;
			size_t j = afterWho(&second, first, index, stream);
			if (j > 0) {
				index += j;

				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '.') {
					++index;
					if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == 'h') {
						++index;
						if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '>') {
							++index;

							pair *item = (pair *) calloc(1, sizeof(pair));
							item->is = PAIR_SECOND;
							item->second = (pair **) calloc(1, sizeof (pair *));
							item->second[0] = second;
							item->size = 1;
							item->sum = 1;
							item->value = aboutEvenUseBackWhenHis;
							*out = item;

							return index - i;
						}
					}
				}

				delete(second);
			}
		}
	}

	return 0;
}

size_t allFromHer(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	pair *second = (pair *) NULL;
	size_t j = afterYearUp(&second, first, index, stream);
	if (j > 0) {
		index += j;
		j = afterYourLookSoMeMy(first, index, stream);

		if (j > 0) {
			index += j;

			pair *value = (pair *) NULL;
			j = afterThemLikeAn(&value, first, index, stream);
			if (j > 0) {
				index += j;

				pair *item = (pair *) calloc(1, sizeof(pair));
				item->is = PAIR_SECOND;
				item->second = (pair **) calloc(2, sizeof (pair *));
				item->second[0] = second;
				item->second[1] = value;
				item->size = 2;
				item->sum = 2;
				item->value = aboutHaveOurNoFrom;
				*out = item;

				return index - i;
			}
		}

		delete(second);
	}

	return 0;
}

size_t allCanNotBut(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	pair *item = aboutSayAs(aboutHeSeeOur);
	*out = item;

	pair *second = (pair *) NULL;
	size_t j = allFromHer(&second, first, index, stream);
	while (j > 0) {
		if (item->size >= item->sum) {
			item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
			item->sum *= 2;
		}

		item->second[item->size] = second;

		index += j;
		++item->size;

		const size_t k = index;
		index += afterYourLookSoMeMy(first, index, stream);
		if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ',') {
			++index;
			index += afterYourLookSoMeMy(first, index, stream);

			j = allFromHer(&second, first, index, stream);
			if (j < 1) {
				return k - i;
			}
		} else {
			return k - i;
		}
	}

	return index - i;
}

size_t allAlsoWhoThatWillThanOnly(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "switch", first, index, stream) == 6) {
		index += 6;
		index += afterYourLookSoMeMy(first, index, stream);

		if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '(') {
			++index;
			index += afterYourLookSoMeMy(first, index, stream);

			pair *second = (pair *) NULL;
			size_t j = afterThemLikeAn(&second, first, index, stream);
			if (j > 0) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ')') {
					++index;
					index += afterYourLookSoMeMy(first, index, stream);

					if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '{') {
						++index;
						index += afterYourLookSoMeMy(first, index, stream);

						pair *value = (pair *) NULL;
						j = allAtWillNow(&value, first, index, stream);
						if (value) {
							index += j;
							index += afterYourLookSoMeMy(first, index, stream);

							if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '}') {
								++index;

								pair *item = (pair *) calloc(1, sizeof(pair));
								item->is = PAIR_SECOND;
								item->second = (pair **) calloc(2, sizeof (pair *));
								item->second[0] = second;
								item->second[1] = value;
								item->size = 2;
								item->sum = 2;
								item->value = aboutIntoThisThisButCould;
								*out = item;

								return index - i;
							}

							delete(value);
						}
					}
				}

				delete(second);
			}
		}
	}

	return 0;
}

size_t allAnDayWhich(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;

	pair *second = (pair *) NULL;
	size_t j = afterYearUp(&second, first, index, stream);
	if (j > 0) {
		index += j;
		j = afterYourLookSoMeMy(first, index, stream);

		if (j > 0) {
			index += j;

			pair *value = (pair *) NULL;
			j = afterThemLikeAn(&value, first, index, stream);

			if (j > 0) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '=') {
					++index;
					index += afterYourLookSoMeMy(first, index, stream);

					pair *sum = (pair *) NULL;
					j = allAnyUpNowGoAn(&sum, first, index, stream);

					if (j > 0) {
						index += j;
						index += afterYourLookSoMeMy(first, index, stream);

						if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ';') {
							++index;

							pair *item = (pair *) calloc(1, sizeof(pair));
							item->is = PAIR_SECOND;
							item->second = (pair **) calloc(3, sizeof (pair *));
							item->second[0] = second;
							item->second[1] = value;
							item->second[2] = sum;
							item->size = 3;
							item->sum = 3;
							item->value = aboutItsBecauseOr;
							*out = item;

							return index - i;
						}

						delete(sum);
					}
				}

				delete(value);
			}
		}

		delete(second);
	}

	return 0;
}

size_t allAnWantWeItAsLike(pair **out, struct first *first, size_t index, FILE *stream) {
	const size_t i = index;
	if (afterSaySomeThinkTheyOnlyEven((const uint8_t *) "while", first, index, stream) == 5) {
		index += 5;
		index += afterYourLookSoMeMy(first, index, stream);

		if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '(') {
			++index;
			index += afterYourLookSoMeMy(first, index, stream);

			pair *second = (pair *) NULL;
			size_t j = allAnyUpNowGoAn(&second, first, index, stream);
			if (j > 0) {
				index += j;
				index += afterYourLookSoMeMy(first, index, stream);

				if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == ')') {
					++index;
					index += afterYourLookSoMeMy(first, index, stream);

					if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '{') {
						++index;
						index += afterYourLookSoMeMy(first, index, stream);

						pair *value = (pair *) NULL;
						j = allAndTheseNow(&value, first, index, stream);
						if (value) {
							index += j;
							index += afterYourLookSoMeMy(first, index, stream);

							if (afterPeopleWhoThereOtherIt(first, index, stream) && first->in[index] == '}') {
								++index;

								pair *item = (pair *) calloc(1, sizeof(pair));
								item->is = PAIR_SECOND;
								item->second = (pair **) calloc(2, sizeof (pair *));
								item->second[0] = second;
								item->second[1] = value;
								item->size = 2;
								item->sum = 2;
								item->value = aboutItsPeopleWellOther;
								*out = item;

								return index - i;
							}

							delete(value);
						}
					}
				}

				delete(second);
			}
		}
	}

	return 0;
}

void allGiveYourKnow(struct first *first, size_t index) {
	if (index > 0) {
		const size_t i = index;
		const size_t size = first->size;
		if (index < size) {
			while (index < size) {
				first->in[index - i] = first->in[index];
				++index;
			}

			first->size -= i;
		} else {
			first->size = 0;
		}
	}
}

size_t allGoBecauseThereThese(pair **out, struct first *first, FILE *stream) {
	size_t index = afterYourLookSoMeMy(first, 0, stream);
	size_t size = index;

	pair *item = new();
	*out = item;

	pair *second = (pair *) NULL;
	size_t j = allBecause(&second, first, index, stream);
	if (j < 1) {
		j = allCouldThemYourWhen(&second, first, index, stream);
	}

	if (j < 1) {
		j = allDay(&second, first, index, stream);
	}

	while (j > 0) {
		index += j;
		size += j;

		allGiveYourKnow(first, index);
		index = afterYourLookSoMeMy(first, 0, stream);
		size += index;

		if (item->size >= item->sum) {
			item->second = (pair **) reallocarray(item->second, 2 * item->sum, sizeof(pair *));
			item->sum *= 2;
		}

		item->second[item->size] = second;
		++item->size;

		second = (pair *) NULL;
		j = allBecause(&second, first, index, stream);
		if (j < 1) {
			j = allCouldThemYourWhen(&second, first, index, stream);
		}

		if (j < 1) {
			j = allDay(&second, first, index, stream);
		}
	}

	return size;
}

void allGoInto(const pair *item, FILE *stream) {
	const size_t size = item->size;
	if (size > 0) {
		pair **second = item->second;
		print(second[0], 0, stream);
		uint8_t value = second[0]->value;
		for (size_t i = 1; i < size; ++i) {
			const uint8_t item = second[i]->value;
			if (value == aboutBut || value != item) {
				fputc('\n', stream);
				fputc('\n', stream);
			} else {
				fputc('\n', stream);
			}
			print(second[i], 0, stream);
			value = item;
		}
		fputc('\n', stream);
	}
}

struct second {
	uint8_t *is;
	struct first first;
	union {
		struct second **second;
		pair *value;
	};
	size_t index;
	size_t size;
	size_t sum;
};

int allGoodGoSheYouHe(const char *first, const char *second) {
	while (*first && *second) {
		int i = *first - *second;
		if (i) {
			return i;
		}

		++first;
		++second;
	}

	return *first - *second;
}

struct second *allHerWhichButYouMe() {
	struct second *item = (struct second *) calloc(1, sizeof(struct second));
	item->is = (uint8_t *) calloc(1, sizeof(uint8_t));
	item->first.in = (uint8_t *) calloc(1, sizeof(uint8_t));
	item->first.sum = 1;
	item->value = new();
	return item;
}

struct second *allHimThe(uint8_t value) {
	struct second *item = (struct second *) calloc(1, sizeof(struct second));
	item->is = (uint8_t *) calloc(1, sizeof(uint8_t));
	item->first.in = (uint8_t *) calloc(1, sizeof(uint8_t));
	item->first.sum = 1;
	item->second = (struct second **) calloc(1, sizeof(struct second *));
	item->sum = 1;

	if (value & 3) {
		const uint8_t out = value + 1;

		switch (byte() & 1) {
			case 0:
				item->second[0] = allHimThe(out);
				break;
			default:
				item->second[0] = allHerWhichButYouMe();
				break;
		}
		item->size = 1;

		uint8_t i = byte() & 3;
		if (item->second[0]->sum) {
			if (item->size >= item->sum) {
				item->second = (struct second **) reallocarray(item->second, 2 * item->sum, sizeof(struct second *));
				item->sum *= 2;
			}

			switch (byte() & 1) {
				case 0:
					item->second[1] = allHimThe(out);
					break;
				default:
					item->second[1] = allHerWhichButYouMe();
					break;
			}
			item->size = 2;

			if (i > 0) {
				--i;
			}
		}

		while (i > 0) {
			if (item->size >= item->sum) {
				item->second = (struct second **) reallocarray(item->second, 2 * item->sum, sizeof(struct second *));
				item->sum *= 2;
			}

			switch (byte() & 1) {
				case 0:
					item->second[item->size] = allHimThe(out);
					break;
				default:
					item->second[item->size] = allHerWhichButYouMe();
					break;
			}

			++item->size;
			--i;
		}
	} else {
		item->second[0] = allHerWhichButYouMe();
		item->size = 1;

		uint8_t i = byte() & 7;
		while (i > 0) {
			if (item->size >= item->sum) {
				item->second = (struct second **) reallocarray(item->second, 2 * item->sum, sizeof(struct second *));
				item->sum *= 2;
			}

			item->second[item->size] = allHerWhichButYouMe();

			++item->size;
			--i;
		}
	}

	return item;
}

struct second *allItsThink(size_t size) {
	if (size > 0) {
		struct second *item = (struct second *) calloc(1, sizeof(struct second));
		const size_t i = (size - 1) >> 3;
		item->is = (uint8_t *) calloc(1 + i, sizeof(uint8_t));
		for (size_t j = 0; j <= i; ++j) {
			item->is[j] = byte();
		}
		const uint8_t j = size & 7;
		if (j) {
			item->is[i] &= (uint8_t) -1 << (8 - j);
		}

		item->first.in = (uint8_t *) calloc(1, sizeof(uint8_t));
		item->first.sum = 1;
		item->value = new();
		item->index = size;
		return item;
	} else {
		return allHerWhichButYouMe();
	}
}

struct second *allJustSeeThatOurCome(size_t size, uint8_t value) {
	if (size > 0) {
		struct second *item = (struct second *) calloc(1, sizeof(struct second));
		size_t i = integer() % (size + 1);
		if (i > 0) {
			const size_t j = (i - 1) >> 3;
			item->is = (uint8_t *) calloc(1 + j, sizeof(uint8_t));
			for (size_t k = 0; k <= j; ++k) {
				item->is[k] = byte();
			}
			const uint8_t k = i & 7;
			if (k) {
				item->is[j] &= (uint8_t) -1 << (8 - k);
			}

			item->index = i;
		} else {
			item->is = (uint8_t *) calloc(1, sizeof(uint8_t));
		}

		item->first.in = (uint8_t *) calloc(1, sizeof(uint8_t));
		item->first.sum = 1;
		item->second = (struct second **) calloc(1, sizeof(struct second *));
		item->sum = 1;

		size_t j = i;
		if (value & 3) {
			const uint8_t out = value + 1;

			uint8_t k = byte() & 3;
			if (k) {
				i = integer() % (size - j + 1);
				switch (byte() & 1) {
					case 0:
						item->second[0] = allJustSeeThatOurCome(i, out);
						break;
					default:
						item->second[0] = allItsThink(i);
						break;
				}
				item->size = 1;
				j += i;

				while (--k > 0) {
					if (item->size >= item->sum) {
						item->second = (struct second **) reallocarray(item->second, 2 * item->sum, sizeof(struct second *));
						item->sum *= 2;
					}

					i = integer() % (size - j + 1);
					switch (byte() & 1) {
						case 0:
							item->second[item->size] = allJustSeeThatOurCome(i, out);
							break;
						default:
							item->second[item->size] = allItsThink(i);
							break;
					}
					j += i;

					++item->size;
				}

				if (item->size >= item->sum) {
					item->second = (struct second **) reallocarray(item->second, 2 * item->sum, sizeof(struct second *));
					item->sum *= 2;
				}

				switch (byte() & 1) {
					case 0:
						item->second[item->size] = allJustSeeThatOurCome(size - j, out);
						break;
					default:
						item->second[item->size] = allItsThink(size - j);
						break;
				}
				++item->size;
			} else {
				item->second[0] = allItsThink(size - j);
				item->size = 1;
			}
		} else {
			uint8_t k = byte() & 7;
			while (k > 0) {
				if (item->size >= item->sum) {
					item->second = (struct second **) reallocarray(item->second, 2 * item->sum, sizeof(struct second *));
					item->sum *= 2;
				}

				i = integer() % (size - j + 1);
				item->second[item->size] = allItsThink(i);
				j += i;

				++item->size;
				--k;
			}

			if (item->size >= item->sum) {
				item->second = (struct second **) reallocarray(item->second, 2 * item->sum, sizeof(struct second *));
				item->sum *= 2;
			}

			item->second[item->size] = allItsThink(size - j);
			++item->size;
		}

		return item;
	} else {
		return allHimThe(value);
	}
}

void allLookGood(struct first *first, const char *second) {
	while (*second) {
		if (first->size >= first->sum) {
			first->in = (uint8_t *) reallocarray(first->in, 2 * first->sum, sizeof(uint8_t));
			first->sum *= 2;
		}

		first->in[first->size] = *second;
		++first->size;
		++second;
	}
}

void allLookWantGood(struct first *first, uint8_t in) {
	if (first->size >= first->sum) {
		first->in = (uint8_t *) reallocarray(first->in, 2 * first->sum, sizeof(uint8_t));
		first->sum *= 2;
	}

	first->in[first->size] = in;
	++first->size;
}

void allMy(struct second *item) {
	struct first *first = &item->first;
	first->size = 0;

	const size_t size = item->index;
	if (size > 0) {
		uint8_t out = 0;
		size_t i = integer() % (size + 1);
		if (i > 3) {
			i = 3;
		}

		if (i) {
			splice(&out, item->is, 0, i);
			out >>= (8 - i);

			switch (i) {
				case 1:
					allLookGood(first, afterItsWantComeWithOf[byte() & 3][1 + out]);
					break;
				case 2:
					allLookGood(first, afterItsWantComeWithOf[byte() & 3][3 + out]);
					break;
				default:
					allLookGood(first, afterItsWantComeWithOf[byte() & 3][7 + out]);
					break;
			}
		} else {
			allLookGood(first, afterItsWantComeWithOf[byte() & 3][0]);
		}

		size_t j = i;
		while (j < size) {
			i = integer() % (size - j + 1);
			if (i > 3) {
				i = 3;
			}

			if (i) {
				splice(&out, item->is, j, i);
				out >>= (8 - i);

				uint8_t k = out + 1;
				switch (i) {
					case 1:
						break;
					case 2:
						k += 2;
						break;
					default:
						k += 6;
						break;
				}

				const char *l = afterItsWantComeWithOf[byte() & 3][k];
				allLookWantGood(first, *l - 32);
				allLookGood(first, l + 1);

				j += i;
			} else {
				const char *j = afterItsWantComeWithOf[byte() & 3][0];
				allLookWantGood(first, *j - 32);
				allLookGood(first, j + 1);
			}
		}
	} else {
		allLookGood(first, afterItsWantComeWithOf[byte() & 3][0]);

		uint8_t i = byte() & 3;
		while (i > 0) {
			const char *j = afterItsWantComeWithOf[byte() & 3][0];
			allLookWantGood(first, *j - 32);
			allLookGood(first, j + 1);
			--i;
		}
	}

	if (!item->sum) {
		allLookWantGood(first, '.');
		switch (byte() & 1) {
			case 0:
				allLookWantGood(first, 'c');
				break;
			default:
				allLookWantGood(first, 'h');
				break;
		}
	}

	if (first->size >= first->sum) {
		first->in = (uint8_t *) reallocarray(first->in, 2 * first->sum, sizeof(uint8_t));
		first->sum *= 2;
	}
	first->in[first->size] = 0;
}

void allNoKnowThere(struct second *item) {
	allMy(item);

	if (item->sum) {
		const size_t size = item->size;
		if (size > 0) {
			struct second **second = item->second;
			allNoKnowThere(*second);

			for (size_t i = 1; i < size; ++i) {
				struct second *first = second[i];
				allNoKnowThere(first);

				uint8_t j = 1;
				while (j) {
					j = 0;

					for (size_t k = 0; k < i; ++k) {
						if (!allGoodGoSheYouHe((const char *) second[k]->first.in, (const char *) first->first.in)) {
							allMy(first);
							j = 1;
							break;
						}
					}
				}

				for (size_t j = i; j > 0; --j) {
					int k = allGoodGoSheYouHe((const char *) second[j - 1]->first.in, (const char *) first->first.in);
					if (k < 0) {
						break;
					} else {
						second[j] = second[j - 1];
						second[j - 1] = first;
					}
				}
			}
		}
	}
}

struct second *allNoWithWellBecauseAndThey() {
	struct second *item = (struct second *) calloc(1, sizeof(struct second));
	item->second = (struct second **) calloc(1, sizeof(struct second *));
	item->sum = 1;
	return item;
}

void allNot(struct second *first, struct second *item) {
	if (item->sum) {
		const size_t size = item->size;
		struct second **second = item->second;
		for (size_t i = 0; i < size; ++i) {
			allNot(first, *second);
			++second;
		}
	} else {
		if (first->size >= first->sum) {
			first->second = (struct second **) reallocarray(first->second, 2 * first->sum, sizeof(struct second *));
			first->sum *= 2;
		}

		first->second[first->size] = item;
		++first->size;
	}
}

void allOneWantEvenMake(struct second *item) {
	free(item->second);
	free(item);
}

void allOnlyIt(const struct second *item, uint8_t *in, uint8_t *index, struct first *first) {
	const size_t size = item->index;
	if (size > 0) {
		const size_t sum = (size - 1) >> 3;
		const uint8_t value = *index & 7;
		const uint8_t *out = item->first.in;
		if (value) {
			const uint8_t j = 8 - value;
			for (size_t i = 0; i < sum; ++i) {
				*in |= out[i] >> value;
				allLookWantGood(first, *in);
				*in = out[i] << j;
			}
		} else {
			*in = 0;
			for (size_t i = 0; i < sum; ++i) {
				allLookWantGood(first, out[i]);
			}
		}

		uint8_t i = out[sum];
		const uint8_t j = size & 7;
		if (j) {
			const uint8_t k = j + value;
			if (k > 7) {
				*in |= i >> value;
				allLookWantGood(first, *in);
				*in = i << (8 - value);
				*index = k & 7;
			} else {
				*in |= i >> value;
				*index += j;
			}
		} else {
			*in |= i >> value;
			allLookWantGood(first, *in);
			*in = i << (8 - value);
		}
	}
}

void allOr(const struct second *item, uint8_t *in, uint8_t *index, struct first *first) {
	allOnlyIt(item, in, index, first);

	if (item->sum) {
		const size_t size = item->size;
		struct second **second = item->second;
		for (size_t i = 0; i < size; ++i) {
			allOr(*second, in, index, first);
			++second;
		}
	}
}

size_t allOtherSoWould(const struct first *first) {
	if (first->size < sizeof(size_t)) {
		const size_t size = first->size;
		if (size > 0) {
			size_t i = first->in[0];
			for (size_t j = 1; j < size; ++j) {
				i <<= 8;
				i |= first->in[j];
			}
			return i;
		} else {
			return 0;
		}
	} else {
		size_t i = first->in[0];
		for (size_t j = 1; j < sizeof(size_t); ++j) {
			i <<= 8;
			i |= first->in[j];
		}
		return i;
	}
}

void allPeopleUseBack(pair *out, uint8_t *in, size_t size, FILE *stream) {
	size_t i = fread(in, sizeof(uint8_t), size, stream);

	while (i > 0) {
		afterGoThatAfterWeWhenOr(out, in, 0, i << 3);
		i = fread(in, sizeof(uint8_t), size, stream);
	}
}

void allSo(uint8_t *out, size_t size) {
	uint8_t *in = out + sizeof(size_t) - 1;
	for (size_t j = 0; j < sizeof(size_t); ++j) {
		*in = (uint8_t) -1 & size;
		size >>= 8;
		--in;
	}
}

void allSome(struct second *out, uint8_t *in, size_t index, size_t size, FILE *stream) {
	pair *second = out->second[index % out->size]->value;
	size_t i = fread(in, sizeof(uint8_t), size, stream);
	uint8_t value[sizeof(size_t)];

	if (i > 0) {
		i <<= 3;

		size_t j = integer() % i;
		allSo(value, ++j);
		afterGoThatAfterWeWhenOr(second, value, 0, 8 * sizeof(size_t));
		afterGoThatAfterWeWhenOr(second, in, 0, j);

		size_t k = j;
		while (k < i) {
			index = integer();
			allSo(value, index);
			afterGoThatAfterWeWhenOr(second, value, 0, 8 * sizeof(size_t));
			second = out->second[index % out->size]->value;

			j = integer() % (i - k);
			allSo(value, ++j);
			afterGoThatAfterWeWhenOr(second, value, 0, 8 * sizeof(size_t));
			afterGoThatAfterWeWhenOr(second, in, k, j);
			k += j;
		}

		i = fread(in, sizeof(uint8_t), size, stream);
	}

	while (i > 0) {
		i <<= 3;

		index = integer();
		allSo(value, index);
		afterGoThatAfterWeWhenOr(second, value, 0, 8 * sizeof(size_t));
		second = out->second[index % out->size]->value;

		size_t j = integer() % i;
		allSo(value, ++j);
		afterGoThatAfterWeWhenOr(second, value, 0, 8 * sizeof(size_t));
		afterGoThatAfterWeWhenOr(second, in, 0, j);

		size_t k = j;
		while (k < i) {
			index = integer();
			allSo(value, index);
			afterGoThatAfterWeWhenOr(second, value, 0, 8 * sizeof(size_t));
			second = out->second[index % out->size]->value;

			j = integer() % (i - k);
			allSo(value, ++j);
			afterGoThatAfterWeWhenOr(second, value, 0, 8 * sizeof(size_t));
			afterGoThatAfterWeWhenOr(second, in, k, j);
			k += j;
		}

		i = fread(in, sizeof(uint8_t), size, stream);
	}
}

void allTakeHisSoBackNoNo(const pair *item, uint8_t *in, uint8_t *index, struct first *first) {
	const size_t size = item->size;
	if (size > 0) {
		const size_t sum = (size - 1) >> 3;
		const uint8_t value = *index & 7;
		const uint8_t *out = item->first;
		if (value) {
			const uint8_t j = 8 - value;
			for (size_t i = 0; i < sum; ++i) {
				*in |= out[i] >> value;
				allLookWantGood(first, *in);
				*in = out[i] << j;
			}
		} else {
			*in = 0;
			for (size_t i = 0; i < sum; ++i) {
				allLookWantGood(first, out[i]);
			}
		}

		uint8_t i = out[sum];
		const uint8_t j = size & 7;
		if (j) {
			const uint8_t k = j + value;
			if (k > 7) {
				*in |= i >> value;
				allLookWantGood(first, *in);
				*in = i << (8 - value);
				*index = k & 7;
			} else {
				*in |= i >> value;
				*index += j;
			}
		} else {
			*in |= i >> value;
			allLookWantGood(first, *in);
			*in = i << (8 - value);
		}
	}
}

void allTakeWill(const pair *item, uint8_t *in, uint8_t *index, struct first *first) {
	switch (item->is) {
		case PAIR_FIRST:
			allTakeHisSoBackNoNo(item, in, index, first);
			break;
		case PAIR_SECOND:
			const size_t size = item->size;
			pair **second = item->second;
			for (size_t i = 0; i < size; ++i) {
				allTakeWill(*second, in, index, first);
				++second;
			}
			break;
	}
}

void allThatOurCanAnOf(const struct second *item, size_t index, FILE *stream) {
	const size_t size = item->size;
	const struct second **second = (const struct second **) item->second;
	uint8_t value[sizeof(size_t)];

	struct first **out = (struct first **) calloc(size, sizeof(struct first *));
	for (size_t i = 0; i < size; ++i) {
		struct first *first = (struct first *) calloc(1, sizeof(struct first));
		first->in = (uint8_t *) calloc(1, sizeof(uint8_t));
		first->sum = 1;

		uint8_t in = 0;
		uint8_t index = 0;
		allTakeWill(second[i]->value, &in, &index, first);
		if (index) {
			allLookWantGood(first, in);
		}
		first->size <<= 3;
		first->size |= index;
		first->sum = 0;

		out[i] = first;
	}

	struct first *first = out[index % size];
	uint8_t i = 0;
	uint8_t j = 0;
	while (first->size >= 8 * sizeof(size_t)) {
		splice(value, first->in, first->sum, 8 * sizeof(size_t));
		size_t k = *value;
		for (size_t i = 1; i < sizeof(size_t); ++i) {
			k <<= 8;
			k |= value[i];
		}

		first->size -= 8 * sizeof(size_t);
		first->sum += 8 * sizeof(size_t);
		if (first->size > 0 && k > 0) {
			const size_t index = (k - 1) >> 3;
			const uint8_t size = k & 7;

			uint8_t *in = (uint8_t *) calloc(1 + index, sizeof(uint8_t));
			splice(in, first->in, first->sum, k);

			if (j) {
				const uint8_t l = 8 - j;
				for (size_t k = 0; k < index; ++k) {
					i |= in[k] >> j;
					fputc(i, stream);
					i = in[k] << l;
				}
			} else {
				i = 0;
				for (size_t k = 0; k < index; ++k) {
					fputc(in[k], stream);
				}
			}

			if (size) {
				const uint8_t value = size + j;
				if (value > 7) {
					if (j) {
						i |= in[index] >> j;
						fputc(i, stream);
						i = in[index] << (8 - j);
						j = value & 7;
					} else {
						fputc(in[index], stream);
					}
				} else if (j) {
					i |= in[index] >> j;
					j = value;
				} else {
					i = in[index];
					j = value;
				}
			} else if (j) {
				i |= in[index] >> j;
				fputc(i, stream);
				i = in[index] << (8 - j);
			} else {
				fputc(in[index], stream);
			}

			free(in);
			first->size -= k;
			first->sum += k;
		}

		if (first->size < 8 * sizeof(size_t)) {
			break;
		} else {
			splice(value, first->in, first->sum, 8 * sizeof(size_t));
			index = *value;
			for (size_t i = 1; i < sizeof(size_t); ++i) {
				index <<= 8;
				index |= value[i];
			}
			first->size -= 8 * sizeof(size_t);
			first->sum += 8 * sizeof(size_t);
			first = out[index % size];
		}
	}

	for (size_t i = 0; i < size; ++i) {
		struct first *first = out[i];
		free(first->in);
		free(first);
	}
	free(out);
}

void allThemSayAlso(struct second *item) {
	if (item->sum) {
		const size_t size = item->size;
		struct second **second = item->second;
		for (size_t i = 0; i < size; ++i) {
			allThemSayAlso(second[i]);
		}
		free(second);
	} else {
		delete(item->value);
	}

	free(item->is);
	free(item->first.in);
	free(item);
}

void allThemWantItThanWant(const struct second *item, const char *in) {
	struct first *first = (struct first *) calloc(1, sizeof(struct first));
	first->in = (uint8_t *) calloc(1, sizeof(uint8_t));
	first->sum = 1;
	if (*in) {
		allLookGood(first, in);
		if (first->in[first->size - 1] != '/') {
			allLookWantGood(first, '/');
		}
	}
	allLookGood(first, (const char *) item->first.in);
	allLookWantGood(first, 0);

	if (item->sum) {
		const size_t size = item->size;
		if (size > 0) {
			const struct second **second = (const struct second **) item->second;
			if (!mkdir((const char *) first->in, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) {
				for (size_t i = 0; i < size; ++i) {
					allThemWantItThanWant(*second, (const char *) first->in);
					++second;
				}
			}
		}
	} else {
		FILE *stream = fopen((const char *) first->in, "w");
		if (stream) {
			allGoInto(item->value, stream);
			fclose(stream);
		}
	}

	free(first->in);
	free(first);
}

void allThenThanDayOverOn(struct second *item, uint8_t in, uint8_t size) {
	const uint8_t i = size & 7;
	if (size > 0) {
		const size_t index = item->index;
		if (index) {
			const size_t j = (index - 1) >> 3;
			const uint8_t k = index & 7;
			if (k) {
				item->is[j] |= in >> k;
				if (i + k > 8) {
					item->is = (uint8_t *) reallocarray(item->is, j + 2, sizeof(uint8_t));
					item->is[j + 1] = in << (8 - k);
				}
			} else {
				const uint8_t k = j + 1;
				item->is = (uint8_t *) reallocarray(item->is, k + 1, sizeof(uint8_t));
				item->is[k] = in;
				if (i) {
					item->is[k] &= (uint8_t) -1 << (8 - i);
				}
			}
			item->index += i;
		} else {
			item->is[0] = in;
			if (i) {
				item->is[0] &= (uint8_t) -1 << (8 - i);
				item->index = i;
			} else {
				item->index = 8;
			}
		}
	}
}

size_t allThink(const char *in, const char *first) {
	size_t i = 0;

	while (*in && *first) {
		if (*in == *first) {
			++i;
		} else {
			return i;
		}

		++in;
		++first;
	}

	return i;
}

bool allThinkJustMakeHowBe(struct second *item) {
	const char *first = (const char *) item->first.in;

	uint8_t i = 0;
	const size_t size = sizeof(afterSheOn) / sizeof(afterSheOn[0]);
	for (size_t j = 0; j < size; ++j) {
		const size_t k = afterTakeSay[j];
		if (allThink(afterSheOn[j], first) == k) {
			allThenThanDayOverOn(item, afterThatOurThem[j], afterTheirOfHisEvenWhen[j]);
			first += k;
			i = 1;
			break;
		}
	}

	if (i) {
		while (i) {
			i = 0;

			for (size_t j = 0; j < size; ++j) {
				const char *out = afterSheOn[j];
				if (*first) {
					if (*first != *out - 32) {
						continue;
					}
				} else {
					break;
				}

				size_t k = afterTakeSay[j] - 1;
				if (allThink(out + 1, first + 1) == k) {
					++k;

					const uint8_t l = afterTheirOfHisEvenWhen[j];
					if (l) {
						allThenThanDayOverOn(item, afterThatOurThem[j], l);
					}

					first += k;
					i = 1;
					break;
				}
			}
		}

		if (item->sum) {
			if (*first) {
				return false;
			} else {
				return true;
			}
		} else if (*first == '.') {
			++first;
			switch (*first) {
				case 'c':
				case 'h':
					++first;
					if (*first) {
						return false;
					} else {
						return true;
					}
					break;
				default:
					return false;
			}
		}
	}

	return false;
}

struct second *allTwoMakeOneThinkWill(const char *in, const char *value) {
	size_t sum = 0;
	const char *out = in;
	while (*out) {
		++sum;
		++out;
	}

	struct second *item = (struct second *) NULL;
	if (sum > 0) {
		item = (struct second *) calloc(1, sizeof(struct second));
		item->is = (uint8_t *) calloc(1, sizeof(uint8_t));
		item->first.in = (uint8_t *) calloc(++sum, sizeof(uint8_t));
		uint8_t *out = item->first.in;
		while (*in) {
			*out = *in;
			++out;
			++in;
		}
		item->first.size = sum;
		item->first.sum = sum;
		if (allThinkJustMakeHowBe(item)) {
			struct first *first = (struct first *) calloc(1, sizeof(struct first));
			first->in = (uint8_t *) calloc(1, sizeof(uint8_t));
			first->sum = 1;
			if (*value) {
				allLookGood(first, value);
				if (first->in[first->size - 1] != '/') {
					allLookWantGood(first, '/');
				}
			}
			allLookGood(first, (const char *) item->first.in);
			allLookWantGood(first, 0);

			FILE *stream = fopen((const char *) first->in, "r");
			if (stream) {
				first->size = 0;
				allGoBecauseThereThese(&item->value, first, stream);

				if (!feof(stream) || ferror(stream)) {
					delete(item->value);
					fclose(stream);
				} else {
					fclose(stream);
					free(first->in);
					free(first);
					return item;
				}
			}

			free(first->in);
			free(first);
		}

		free(item->first.in);
		free(item->is);
		free(item);
		item = (struct second *) NULL;
	}

	return item;
}

struct second *allTwoThanOverUse(const char *in, const char *value) {
	size_t sum = 0;
	const char *out = in;
	while (*out) {
		++sum;
		++out;
	}

	struct second *item = (struct second *) NULL;
	if (sum > 0) {
		item = (struct second *) calloc(1, sizeof(struct second));
		item->is = (uint8_t *) calloc(1, sizeof(uint8_t));
		item->second = (struct second **) calloc(1, sizeof(struct second *));
		item->sum = 1;
		item->first.in = (uint8_t *) calloc(++sum, sizeof(uint8_t));
		uint8_t *out = item->first.in;
		while (*in) {
			*out = *in;
			++out;
			++in;
		}
		item->first.size = sum;
		item->first.sum = sum;
		if (allThinkJustMakeHowBe(item)) {
			struct first *first = (struct first *) calloc(1, sizeof(struct first));
			first->in = (uint8_t *) calloc(1, sizeof(uint8_t));
			first->sum = 1;
			if (*value) {
				allLookGood(first, value);
				if (first->in[first->size - 1] != '/') {
					allLookWantGood(first, '/');
				}
			}
			allLookGood(first, (const char *) item->first.in);
			allLookWantGood(first, 0);

			DIR *stream = opendir((const char *) first->in);
			if (stream) {
				struct second *second = (struct second *) NULL;

				errno = 0;
				struct dirent *in = readdir(stream);
				while (in) {
					second = (struct second *) NULL;
					switch (in->d_type) {
						case DT_DIR:
							second = allTwoThanOverUse(in->d_name, (const char *) first->in);
							break;
						case DT_REG:
							second = allTwoMakeOneThinkWill(in->d_name, (const char *) first->in);
							break;
					}

					if (second) {
						if (item->size >= item->sum) {
							item->second = (struct second **) reallocarray(item->second, 2 * item->sum, sizeof(struct second *));
							item->sum *= 2;
						}

						const char *first = (const char *) second->first.in;
						item->second[item->size] = second;
						for (size_t i = item->size; i > 0; --i) {
							if (allGoodGoSheYouHe((const char *) item->second[i - 1]->first.in, first) < 0) {
								break;
							} else {
								item->second[i] = item->second[i - 1];
								item->second[i - 1] = second;
							}
						}

						++item->size;
					}

					errno = 0;
					in = readdir(stream);
				}

				if (errno || item->size < 1) {
					closedir(stream);
				} else {
					closedir(stream);
					if (item->size > 1 || item->second[0]->sum < 1) {
						free(first->in);
						free(first);
						return item;
					}
				}
			}

			free(first->in);
			free(first);
		}

		if (item->size > 0) {
			const size_t size = item->size;
			struct second **second = item->second;
			for (size_t i = 0; i < size; ++i) {
				allThemSayAlso(*second);
				++second;
			}
		}

		free(item->first.in);
		free(item->second);
		free(item->is);
		free(item);
		item = (struct second *) NULL;
	}

	return item;
}

#define allUp 256

void allUpAnyWantThenAboutAny() {
	char *line = (char *) NULL;
	size_t size = 0;

	fputs("\nName: ", stdout);
	ssize_t nread = getline(&line, &size, stdin);

	if (nread > 1) {
		line[nread - 1] = 0;

		FILE *stream = fopen(line, "r");
		if (stream) {
			pair *item = new();
			if (item) {
				uint8_t *in = (uint8_t *) calloc(allUp, sizeof(uint8_t));
				if (in) {
					allPeopleUseBack(item, in, allUp, stream);
					free(in);

					if (feof(stream) && !ferror(stream)) {
						fputs("\nName: ", stdout);
						nread = getline(&line, &size, stdin);

						if (nread > 1) {
							line[nread - 1] = 0;

							FILE *stream = fopen(line, "w");
							if (stream) {
								allGoInto(item, stream);
								fclose(stream);
							}
						}
					}
				}

				delete(item);
			}

			fclose(stream);
		}
	}

	free(line);
}

void allUsNotAllIt() {
	char *line = (char *) NULL;
	size_t size = 0;

	fputs("\nName: ", stdout);
	ssize_t nread = getline(&line, &size, stdin);

	if (nread > 1) {
		line[nread - 1] = 0;

		FILE *stream = fopen(line, "r");
		if (stream) {
			struct second *item = allJustSeeThatOurCome(8 * sizeof(size_t), 1);
			struct second *second = allNoWithWellBecauseAndThey();

			struct first *first = (struct first *) calloc(1, sizeof(struct first));
			if (first) {
				first->in = (uint8_t *) calloc(1, sizeof(uint8_t));
				first->sum = 1;
			}

			if (item && second && first && first->in) {
				allNoKnowThere(item);

				uint8_t in = 0;
				uint8_t index = 0;
				allOr(item, &in, &index, first);

				uint8_t *out = (uint8_t *) calloc(allUp, sizeof(uint8_t));
				if (out) {
					allNot(second, item);
					allSome(second, out, allOtherSoWould(first), allUp, stream);
					free(out);

					if (feof(stream) && !ferror(stream)) {
						fputs("\nName: ", stdout);
						nread = getline(&line, &size, stdin);

						if (nread > 1) {
							line[nread - 1] = 0;

							fputs("\nName: ", stdout);
							fputs((const char *) item->first.in, stdout);
							fputc('\n', stdout);

							allThemWantItThanWant(item, line);
						}
					}
				}
			}

			if (first) {
				if (first->in) {
					free(first->in);
				}

				free(first);
			}

			if (second) {
				allOneWantEvenMake(second);
			}

			if (item) {
				allThemSayAlso(item);
			}

			fclose(stream);
		}
	}

	free(line);
}

void allUsThan() {
	char *line = (char *) NULL;
	size_t size = 0;

	fputs("\nName: ", stdout);
	ssize_t nread = getline(&line, &size, stdin);

	if (nread > 1) {
		line[nread - 1] = 0;

		FILE *stream = fopen(line, "r");
		if (stream) {
			struct first *first = (struct first *) calloc(1, sizeof(struct first));
			if (first) {
				first->in = (uint8_t *) calloc(1, sizeof(uint8_t));
				first->sum = 1;

				if (first->in) {
					pair *item = (pair *) NULL;
					allGoBecauseThereThese(&item, first, stream);
					if (item) {
						if (feof(stream) && !ferror(stream)) {
							fputs("\nName: ", stdout);
							nread = getline(&line, &size, stdin);

							if (nread > 1) {
								line[nread - 1] = 0;

								FILE *stream = fopen(line, "w");
								if (stream) {
									uint8_t in = 0;
									uint8_t index = 0;
									afterNotFrom(item, &in, &index, stream);
									fclose(stream);
								}
							}
						}

						delete(item);
					}

					free(first->in);
				}

				free(first);
			}

			fclose(stream);
		}
	}

	free(line);
}

void allUseHe() {
	char *line = (char *) NULL;
	size_t size = 0;

	fputs("\nName: ", stdout);
	ssize_t nread = getline(&line, &size, stdin);

	if (nread > 1) {
		line[nread - 1] = 0;

		char *value = (char *) NULL;
		size_t size = 0;

		fputs("\nName: ", stdout);
		ssize_t nread = getline(&value, &size, stdin);

		if (nread > 1) {
			value[nread - 1] = 0;
			struct second *item = allTwoThanOverUse(value, line);

			if (item) {
				struct first *first = (struct first *) calloc(1, sizeof(struct first));
				if (first) {
					first->in = (uint8_t *) calloc(1, sizeof(uint8_t));
					first->sum = 1;

					if (first->in) {
						uint8_t in = 0;
						uint8_t index = 0;
						allOr(item, &in, &index, first);

						struct second *second = allNoWithWellBecauseAndThey();
						if (second) {
							allNot(second, item);

							fputs("\nName: ", stdout);
							nread = getline(&value, &size, stdin);

							if (nread > 1) {
								value[nread - 1] = 0;

								FILE *stream = fopen(value, "w");
								if (stream) {
									allThatOurCanAnOf(second, allOtherSoWould(first), stream);
									fclose(stream);
								}
							}

							allOneWantEvenMake(second);
						}

						free(first->in);
					}

					free(first);
				}

				allThemSayAlso(item);
			}
		}

		free(value);
	}

	free(line);
}
