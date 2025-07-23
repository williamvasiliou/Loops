#include "sum.h"

bool allWantAnyTimeSome(char **line, size_t *size) {
	fputs("\nCommand (m for help): ", stdout);
	ssize_t nread = getline(line, size, stdin);
	if (nread < 1) {
		return false;
	}

	while (nread != 2) {
		fputs("\nCommand (m for help): ", stdout);
		nread = getline(line, size, stdin);
		if (nread < 1) {
			return false;
		}
	}

	switch (**line) {
		case 'd':
			allUpAnyWantThenAboutAny();
			break;
		case 'e':
			allUsNotAllIt();
			break;
		case 'F':
			allUsThan();
			break;
		case 'm':
			fputs("\nHelp:\n\n", stdout);
			fputs("   d   all up any want then about any\n", stdout);
			fputs("   e   all us not all it\n", stdout);
			fputs("   F   all us than\n", stdout);
			fputs("   m   print this menu\n", stdout);
			fputs("   n   all use he\n", stdout);
			fputs("   q   quit without saving changes\n", stdout);
			break;
		case 'n':
			allUseHe();
			break;
		case 'q':
			return false;
	}

	return true;
}

int main() {
	char *line = (char *) NULL;
	size_t size = 0;

	while (allWantAnyTimeSome(&line, &size)) {
		if (size < 1) {
			free(line);
			exit(EXIT_FAILURE);
		}
	}

	free(line);
	exit(EXIT_SUCCESS);
}
