#include <stdio.h>
#include "unicode/unistr.h"
#include "unicode/utypes.h"
#include "unicode/locid.h"
#include "unicode/coll.h"
#include "unicode/tblcoll.h"
#include "unicode/coleitr.h"
#include "unicode/sortkey.h"
#include "unicode/udata.h"
#include <vector>
#include <string>

using namespace icu;
using namespace std;

static string createSortKey(icu::Collator &collator, const string &str) {
	int32_t size = collator.getSortKey(UnicodeString::fromUTF8(StringPiece(str)), nullptr, 0);
	auto buf = std::unique_ptr<uint8_t[]>(new uint8_t[size]);
	UnicodeString unicode_str = UnicodeString::fromUTF8(StringPiece(str));
	collator.getSortKey(unicode_str, buf.get(), size);
	return string((char*) buf.get(), size);
}

static string sort_strings(vector<string> &strings, icu::Collator &collator) {
	// warning: this is very inefficient
	std::sort(strings.begin(), strings.end(), [&](const string &a, const string &b) {
		return createSortKey(collator, a) < createSortKey(collator, b);
	});
	string res_concat;
	for(auto &str : strings) {
		res_concat += str;
	}
	return res_concat;
}

int main() {
	vector<string> alphabet = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};

	UErrorCode status = U_ZERO_ERROR;
	auto lt_collator = std::unique_ptr<icu::Collator>(Collator::createInstance(Locale("lt"), status));
	auto de_collator = std::unique_ptr<icu::Collator>(Collator::createInstance(Locale("de"), status));
	if (!lt_collator || !de_collator) {
		fprintf(stderr, "ERROR! Could not make LT or DE collator");
	}
	// sort the alphabet using both collators
	string lt_alphabet = sort_strings(alphabet, *lt_collator);
	string de_alphabet = sort_strings(alphabet, *de_collator);
	if (lt_alphabet != "abcdefghiyjklmnopqrstuvwxz") {
		fprintf(stderr, "ERROR: Incorrect LT alphabet sort\n");
	}
	if (de_alphabet != "abcdefghijklmnopqrstuvwxyz") {
		fprintf(stderr, "ERROR: Incorrect DE alphabet sort\n");
	}

	// sort german names
	vector<string> german_names = {"Göbel", "Goethe", "Goldmann", "Göthe" "Götz", "Gabel"};
	string sorted_names = sort_strings(german_names, *de_collator);
	if (sorted_names != "GabelGöbelGoetheGoldmannGötheGötz") {
		fprintf(stderr, "ERROR: Incorrect sort order of german names: %s\n", sorted_names.c_str());
	}

	// string comparison
	auto result = de_collator->compareUTF8(StringPiece("Göthe"), StringPiece("Gotz"), status);
	if (result != UCOL_LESS) {
		fprintf(stderr, "ERROR: Incorrect string comparison for german locale!\n");
	}
	result = de_collator->compareUTF8(StringPiece("Gotz"), StringPiece("Göthe"), status);
	if (result != UCOL_GREATER) {
		fprintf(stderr, "ERROR: Incorrect string comparison for german locale!\n");
	}
	return 0;
}
