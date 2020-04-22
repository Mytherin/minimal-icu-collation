#include "catch.hpp"

#include <stdio.h>
#include "icu-collate.hpp"
#include <vector>
#include <string>

using namespace icu;

TEST_CASE("Test timezone", "[tz]") {
	UErrorCode success = U_ZERO_ERROR;
	UnicodeString curTZNameEn, curTZNameFr;

	// Create a Time Zone with America/Los_Angeles
	UnicodeString tzstr = UnicodeString::fromUTF8(StringPiece("America/Los_Angeles"));
	TimeZone *tzWest = TimeZone::createTimeZone(tzstr);

	REQUIRE(tzWest);
	// // Print out the Time Zone Name, GMT offset etc.
	// curTZNameEn = tzWest->getDisplayName(Locale::getEnglish(),curTZNameEn);
	// fprintf(stdout, "%s\n","Current Time Zone Name in English:");
	// fprintf(stdout, "%S\n", curTZNameEn.);

	// curTZNameFr = tzWest->getDisplayName(Locale::getCanadaFrench(),curTZNameFr);
	// fprintf(stdout, "%s\n","Current Time Zone Name in French:");
	// fprintf(stdout, "%S\n", curTZNameFr.getTerminatedBuffer());
}

