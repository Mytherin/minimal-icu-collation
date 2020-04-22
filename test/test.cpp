#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <stdio.h>
#include "icu-collate.hpp"
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

TEST_CASE("Collation sort test", "[collation]") {
	vector<string> alphabet = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};

	UErrorCode status = U_ZERO_ERROR;
	auto lt_collator = std::unique_ptr<icu::Collator>(Collator::createInstance(Locale("lt"), status));
	auto de_collator = std::unique_ptr<icu::Collator>(Collator::createInstance(Locale("de"), status));
	REQUIRE(lt_collator);
	REQUIRE(de_collator);

	// sort the alphabet using both collators
	// lt alphabet places "y" between "i" and "j" characters
	string lt_alphabet = sort_strings(alphabet, *lt_collator);
	string de_alphabet = sort_strings(alphabet, *de_collator);
	REQUIRE(lt_alphabet == "abcdefghiyjklmnopqrstuvwxz");
	REQUIRE(de_alphabet == "abcdefghijklmnopqrstuvwxyz");

	// sort german names
	vector<string> german_names = {"Göbel", "Goethe", "Goldmann", "Göthe" "Götz", "Gabel"};
	string sorted_names = sort_strings(german_names, *de_collator);
	REQUIRE(sorted_names == "GabelGöbelGoetheGoldmannGötheGötz");


	// string comparison
	auto result = de_collator->compareUTF8(StringPiece("Göthe"), StringPiece("Gotz"), status);
	REQUIRE(result == UCOL_LESS);
	result = de_collator->compareUTF8(StringPiece("Gotz"), StringPiece("Göthe"), status);

	REQUIRE(result == UCOL_GREATER);
}
