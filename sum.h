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
		const size_t i = integer() % (size + 1);
		item->second[0] = aboutOther(in, index, i);
		item->second[1] = aboutNotUseGiveWantNo(in, index + i, size - i);
		item->size = 2;
		item->sum = 2;
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
				++i;
				if (j < size) {
					item->first[i] = in[index] << l;
					j += k;
				}
				++index;
			}
			++i;
		} else {
			while (j < size) {
				item->first[i] = in[index];
				++index;
				++i;
				j += 8;
			}
		}

		if (j > size) {
			const uint8_t k = j & 7;
			if (k) {
				item->first[i - 1] &= (uint8_t) -1 << (8 - k + j - size);
			} else {
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
			splice(&out, item->first, i, j);
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
			*in = 0;
			for (size_t i = 0; i < sum; ++i) {
				fputc(item->first[i], stream);
			}
		}

		uint8_t i = item->first[sum];
		const uint8_t j = size & 7;
		if (j) {
			const uint8_t k = j + value;
			if (k > 7) {
				*in |= i >> value;
				fputc(*in, stream);
				*in = i << (8 - value);
				*index = k & 7;
			} else {
				*in |= i >> value;
				*index += j;
			}
		} else {
			*in |= i >> value;
			fputc(*in, stream);
			*in = i << (8 - value);
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
